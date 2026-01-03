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

#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include <cerrno>

CS2Reader::CS2Reader(const int port) {
    sockaddr_in s_addr_read{};

    if((fd_read = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        throw CS2ConnectorException{"socket-creation for reading failed"};
    }

    memset(&s_addr_read, 0, sizeof(s_addr_read));
    s_addr_read.sin_family = AF_INET;
    s_addr_read.sin_port = htons(port);
    s_addr_read.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(fd_read, reinterpret_cast<sockaddr *>(&s_addr_read), sizeof(s_addr_read)) == -1) {
        throw CS2ConnectorException{"binding failed"};
    }
}

CS2Reader::~CS2Reader() noexcept {
    if(fd_read != -1) {
        close(fd_read);
    }
}

CS2CanCommand CS2Reader::read() const {
    CS2CanCommand data;

    sockaddr_in s_addr_other{};
    socklen_t sockAddrLen = sizeof(s_addr_other);

    // Try again on the interrupted function call
    while(recvfrom(fd_read, &data, sizeof(data), 0, reinterpret_cast<sockaddr *>(&s_addr_other), &sockAddrLen) == -1) {
        if(errno != EINTR) {
            throw CS2ConnectorException{std::strerror(errno)};
        }
    }
    return data;
}