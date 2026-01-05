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

#include "printcancommand.h"
#include "cs2utils.h"
#include <iostream>
#include <cstdio>

PrintCanCommand::PrintCanCommand(const std::set<CanCommand> &allowedCommands): allowedCommands{allowedCommands} {
}

PrintCanCommand::PrintCanCommand(const std::set<CanCommand> &allowedCommands, const std::set<CanSystemSubCommand> &allowedSubCommands) :
allowedCommands{allowedCommands}, allowedSubCommands{allowedSubCommands} {
}

PrintCanCommand::HandlerReturn PrintCanCommand::handleCanCommand(const CS2CanCommand &cmd) {
    if(!printCommand(cmd)) {
        return NOT_HANDLED;
    }

    std::cout <<
        getCommandAsString(cmd) << std::endl <<
        (isResponse(cmd) ? "[R] " : "[ ] ") << getCommandName(cmd) << " - " << getSystemSubCommandName(cmd) << std::endl <<
        "---------------------------------------------------------------" << std::endl;

    return HANDLED_AND_FINISHED;
}

bool PrintCanCommand::printCommand(const CS2CanCommand &cmd) const {
    if(allowedCommands.empty() && allowedSubCommands.empty()) {
        return true;
    }

    const auto head = static_cast<CanCommand>(cmd.header[1] & ~0x01);

    if(allowedCommands.contains(head)) {
        return true;
    }

    if(head != CanCommand::CMD_SYSTEM) {
        return false;
    }

    if(allowedSubCommands.empty()) {
        return true;
    }

    if(allowedSubCommands.contains(static_cast<CanSystemSubCommand>(cmd.data[0]))) {
        return true;
    }

    return false;
}
