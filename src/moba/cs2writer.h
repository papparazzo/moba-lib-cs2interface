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

#pragma once

#include "cs2cancommand.h"

#include <string>
#include <memory>
#include <mutex>

#include <netinet/ip.h>

class CS2Writer final {
public:
    static constexpr int DEFAULT_PORT_WRITE = 15731;

    CS2Writer() = default;
    CS2Writer(const CS2Writer&) = delete;
    CS2Writer(CS2Writer&&) = delete;
    CS2Writer& operator=(const CS2Writer&) = delete;

    ~CS2Writer() noexcept;

    void connect(const std::string &host, int port = DEFAULT_PORT_WRITE);
    void send(const CS2CanCommand &data);

protected:
    sockaddr_in s_addr_write{};
    int fd_write{-1};
    std::mutex m;
};

using CS2WriterPtr = std::shared_ptr<CS2Writer>;