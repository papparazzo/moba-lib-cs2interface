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

#include <memory>

#include "ireader.h"

class CS2Reader final: public IReader {
public:
    static constexpr int DEFAULT_PORT = 15730;

    explicit CS2Reader(int port = DEFAULT_PORT);

    CS2Reader(const CS2Reader&) = delete;
    CS2Reader(CS2Reader&&) = delete;
    CS2Reader& operator=(const CS2Reader&) = delete;

    ~CS2Reader() noexcept override;

    [[nodiscard]]
    auto read() const -> CS2CanCommand override;

protected:
    int fd_read{-1};
};

using CS2ReaderPtr = std::shared_ptr<CS2Reader>;