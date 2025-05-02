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
        return PrintCanCommand::NOT_HANDLED;
    }

    printf(
        "%02X %02X - %02X %02X - %02X - %02X %02X %02X %02X - %02X %02X %02X %02X\n",
        static_cast<unsigned int>(cmd.header[0]),
        static_cast<unsigned int>(cmd.header[1]),
        static_cast<unsigned int>(cmd.hash[0]),
        static_cast<unsigned int>(cmd.hash[1]),
        static_cast<unsigned int>(cmd.len),
        static_cast<unsigned int>(cmd.data[0]),
        static_cast<unsigned int>(cmd.data[1]),
        static_cast<unsigned int>(cmd.data[2]),
        static_cast<unsigned int>(cmd.data[3]),
        static_cast<unsigned int>(cmd.data[4]),
        static_cast<unsigned int>(cmd.data[5]),
        static_cast<unsigned int>(cmd.data[6]),
        static_cast<unsigned int>(cmd.data[7])
    );

    bool response = false;

    if(cmd.header[1] & 0x01) {
        response = true;
    }

    std::uint8_t head = cmd.header[1] & ~0x01;

    std::cout << (response ? "[R] " : "[ ] ") << getCommandAsString(head);

    if(head == CMD_SYSTEM) {
        std::cout << " - " << getSystemSubCommandAsString(cmd.data[4]);
    }

    std::cout << std::endl;
    std::cout << "---------------------------------------------------------------";
    std::cout << std::endl;
    return PrintCanCommand::HANDLED_AND_FINISHED;
}

bool PrintCanCommand::printCommand(const CS2CanCommand &cmd) {
    if(allowedCommands.empty() && allowedSubCommands.empty()) {
        return true;
    }

    auto head = static_cast<CanCommand>(cmd.header[1] & ~0x01);

    if(allowedCommands.find(head) != allowedCommands.end()) {
        return true;
    }

    if(head != CanCommand::CMD_SYSTEM) {
        return false;
    }

    if(allowedSubCommands.empty()) {
        return true;
    }

    auto sub = static_cast<CanSystemSubCommand>(cmd.data[0]);

    if(allowedSubCommands.find(sub) != allowedSubCommands.end()) {
        return true;
    }

    return false;
}