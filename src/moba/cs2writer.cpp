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

#include "cs2writer.h"
#include "cs2shared.h"

#include <unistd.h>
#include <cstring>
#include <iostream>
#include <system_error>

#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h>

#include "cs2utils.h"

CS2Writer::CS2Writer(const std::string &host, const unsigned int port) {

    addrinfo hints{};
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    const std::string service = std::to_string(port);

    addrinfo *res = nullptr;

    if(
        const int result = getaddrinfo(host.c_str(), service.c_str(), &hints, &res);
        result != 0
    ) {
        throw CS2ConnectorException{std::string{"Resolving host <"} + host + "> for writing failed: " + gai_strerror(result)};
    }

    std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> res_guard(res, freeaddrinfo);

    // ---------- create + bind socket ----------
    for (const addrinfo *iter = res; iter != nullptr; iter = iter->ai_next) {
        if((fd_write = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol)) == -1) {
            continue;
        }

        if (
            constexpr int off = 0;
            iter->ai_family == AF_INET6 &&
            setsockopt(fd_write, IPPROTO_IPV6, IPV6_V6ONLY, &off, sizeof(off)) == -1
        ) {
            close(fd_write);
            fd_write = -1;
            continue;
        }

        if (connect(fd_write, iter->ai_addr, iter->ai_addrlen) == 0 || errno == EINPROGRESS) {
            break;
        }
    }

    if (fd_write == -1) {
        throw CS2ConnectorException{"failed to bind UDP socket for writing"};
    }
}

CS2Writer::~CS2Writer() noexcept {
    if(fd_write != -1) {
        close(fd_write);
    }
}

bool CS2Writer::trySend(const CS2CanCommand &data) {
    std::lock_guard l{m};

    const auto buffer = reinterpret_cast<const char*>(&data);
    constexpr size_t size = sizeof(data);

    if(::send(fd_write, buffer, size, 0) != static_cast<ssize_t>(size)) {
        return false;
    }
    return true;
}

void CS2Writer::send(const CS2CanCommand &data) {
    std::lock_guard l{m};

    const auto buffer = reinterpret_cast<const char*>(&data);
    constexpr size_t size = sizeof(data);

    const ssize_t sent = ::send(fd_write, buffer, size, 0);

    if(sent == -1) {
        std::stringstream ss;
        ss << "sending <" << getCommandName(data.getCanCommand()) << "> failed : " << std::system_error(errno, std::system_category()).what();
        throw CS2ConnectorException{ss.str()};
    }

    if(sent != static_cast<ssize_t>(size)) {
        std::stringstream ss;
        ss << "sending <" << getCommandName(data.getCanCommand()) << "> failed : only " + std::to_string(sent) + " bytes sent";
        throw CS2ConnectorException{ss.str()};
    }
}
