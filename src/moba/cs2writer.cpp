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

#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

CS2Writer::~CS2Writer() noexcept {
    if(fd_write != -1) {
        close(fd_write);
    }
}

void CS2Writer::connect(const std::string &host, const int port) {
    if(fd_write != -1) {
        close(fd_write);
    }

    if((fd_write = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        throw CS2ConnectorException{"socket-creation for writing failed"};
    }

    memset(&s_addr_write, 0, sizeof(s_addr_write));
    s_addr_write.sin_family = AF_INET;
    s_addr_write.sin_port = htons(port);

    if(inet_aton(host.c_str(), &s_addr_write.sin_addr) == 0) {
        throw CS2ConnectorException{"inet_aton failed"};
    }
}

void CS2Writer::send(const CS2CanCommand &data) {
    std::lock_guard l{m};

    if(sendto(
        fd_write,
        &data,
        sizeof(data),
        0,
        reinterpret_cast<sockaddr *>(&s_addr_write),
        sizeof(s_addr_write)
    ) == -1) {
        throw CS2ConnectorException("sending failed");
    }
}


