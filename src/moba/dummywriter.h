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

#include <mutex>

#include "iwriter.h"

#include "cs2cancommand.h"
#include "printcancommand.h"

class DummyWriter final: public IWriter {
public:
    DummyWriter() = default;

    DummyWriter(const DummyWriter&) = delete;
    DummyWriter(DummyWriter&&) = delete;
    DummyWriter& operator=(const DummyWriter&) = delete;

    ~DummyWriter() noexcept override = default;

    void send(const CS2CanCommand &data) override;

private:
    PrintCanCommand printCanCommand;
    std::mutex m;
};

