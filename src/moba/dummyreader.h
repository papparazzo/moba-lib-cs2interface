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

#include <string>

#include "ireader.h"

#include "cs2cancommand.h"

class DummyReader final: public IReader {
public:
    explicit DummyReader(const std::string &fifo);

    DummyReader(const DummyReader&) = delete;
    DummyReader(DummyReader&&) = delete;
    DummyReader& operator=(const DummyReader&) = delete;

    ~DummyReader() noexcept override = default;

    [[nodiscard]]
    auto read(CS2CanCommand &data) const -> bool override;

private:
    int fd{-1};
};

