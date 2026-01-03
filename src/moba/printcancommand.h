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

#include "cancommandhandlerinterface.h"
#include "cs2cancommand.h"
#include <set>

class PrintCanCommand final : public CanCommandHandlerInterface {
public:
    PrintCanCommand() = default;

    explicit PrintCanCommand(const std::set<CanCommand> &allowedCommands);

    PrintCanCommand(const std::set<CanCommand> &allowedCommands, const std::set<CanSystemSubCommand> &allowedSubCommands);

    ~PrintCanCommand() noexcept override = default;

    HandlerReturn handleCanCommand(const CS2CanCommand &cmd) override;

protected:
    std::set<CanCommand> allowedCommands;
    std::set<CanSystemSubCommand> allowedSubCommands;

    bool printCommand(const CS2CanCommand &cmd) const;
};

