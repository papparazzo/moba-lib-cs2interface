/*
 *  Project:    moba-lib-cs2interface
 *
 *  Copyright (C) 2020 Stefan Paproth <pappi-@gmx.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/agpl.txt>.
 *
 */

#include "cs2reader.h"
#include "cs2shared.h"

#include <unistd.h>
#include <cstring>

#include <cerrno>


#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

CS2Reader::CS2Reader(const std::string &host, const unsigned int port, const bool blocking) {

    // ---------- bind address ----------
    addrinfo hints{};
    hints.ai_family   = AF_INET6;      // IPv6 (Dual-Stack)
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags    = AI_PASSIVE;    // bind on local addresses

    addrinfo *res = nullptr;

    const std::string service = std::to_string(port);

    if(
        const int result = getaddrinfo(nullptr, service.c_str(), &hints, &res);
        result != 0
    ) {
        throw CS2ConnectorException{std::string{"Resolving host failed: "} + gai_strerror(result)};
    }

    std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> res_guard(res, freeaddrinfo);

    // ---------- create + bind socket ----------
    for (const addrinfo *iter = res; iter != nullptr; iter = iter->ai_next) {
        if((fd_read = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol)) == -1) {
            continue;
        }

        constexpr int off = 0;
        constexpr int yes = 1;

        if(
            setsockopt(fd_read, IPPROTO_IPV6, IPV6_V6ONLY, &off, sizeof(off)) == -1 ||
            setsockopt(fd_read, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1
        ) {
            close(fd_read);
            fd_read = -1;
            continue;
        }

        if (bind(fd_read, iter->ai_addr, iter->ai_addrlen) == 0) {
            break;
        }

        close(fd_read);
        fd_read = -1;
    }

    if (fd_read == -1) {
        throw CS2ConnectorException{"failed to bind UDP socket for reading"};
    }

    // ---------- non-blocking ----------
    if(!blocking) {
        if(
            const int flags = fcntl(fd_read, F_GETFL, 0);
            flags == -1 || fcntl(fd_read, F_SETFL, flags | O_NONBLOCK) == -1
        ) {
            close(fd_read);
            throw CS2ConnectorException{"failed to set non-blocking mode"};
        }
    }

    // ---------- connect peer ----------
    addrinfo peerHints{};
    peerHints.ai_family   = AF_UNSPEC;
    peerHints.ai_socktype = SOCK_DGRAM;

    addrinfo *peerRes = nullptr;

    if(
        const int result = getaddrinfo(host.c_str(), service.c_str(), &peerHints, &peerRes);
        result != 0
    ) {
        close(fd_read);
        throw CS2ConnectorException{std::string{"Resolving peer-host <"} + host + "> failed: " + gai_strerror(result)};
    }

    std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> peer_guard(peerRes, freeaddrinfo);

    for (const addrinfo *iter = peerRes; iter != nullptr; iter = iter->ai_next) {
        if (connect(fd_read, iter->ai_addr, iter->ai_addrlen) == 0 || errno == EINPROGRESS) {
            return;
        }
    }

    close(fd_read);
    throw CS2ConnectorException{"failed to connect UDP peer"};
}

CS2Reader::~CS2Reader() noexcept {
    if(fd_read != -1) {
        close(fd_read);
    }
}

bool CS2Reader::read(CS2CanCommand& data) const {

    while(true) {
        const ssize_t n = recv(fd_read, &data, sizeof(data), 0);
        const int saved_errno = errno;

        if(n == static_cast<ssize_t>(sizeof(data))) {
            return true;
        }

        if(n == -1) {
            if(saved_errno == EINTR) {
                continue;
            }
            // On non-blocking: No data received...
            if(saved_errno == EAGAIN) {
                return false;
            }
            throw CS2ConnectorException{std::strerror(saved_errno)};
        }

        if(n == 0) {
            throw CS2ConnectorException{"Connection closed by peer"};
        }
        throw CS2ConnectorException{"Partial read occurred"};
    }
}
