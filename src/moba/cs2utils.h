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
 *  along with this program. If not, see <http://www.gnu.org/licenses/agpl.txt>.
 *
 */

#pragma once

#include <string>
#include <cstring>

#include "cs2cancommand.h"

std::string getCommmandAsString(int cmd);
std::string getSystemSubCommandAsString(int subCmd);

inline CS2CanCommand setLocSpeed(std::uint32_t locId, std::uint16_t speed) {
    std::uint8_t low  = speed & 0xFF;
    std::uint8_t high = (speed >> 8) & 0xFF;
    return CS2CanCommand{CanCommand::CMD_LOCO_SPEED, 6, locId, low, high};
}

inline CS2CanCommand setEmergencyStop() {
    return CS2CanCommand{CanCommand::CMD_SYSTEM, 5, 0x00000000, static_cast<std::uint8_t>(CanSystemSubCommand::SYS_SUB_CMD_SYSTEM_STOP)};
}

inline CS2CanCommand setEmergencyStopClearing() {
    return CS2CanCommand{CanCommand::CMD_SYSTEM, 5, 0x00000000, static_cast<std::uint8_t>(CanSystemSubCommand::SYS_SUB_CMD_SYSTEM_GO)};
}

inline CS2CanCommand ping() {
    return CS2CanCommand(CanCommand::CMD_PING);
}

inline CS2CanCommand getLoklist() {
    // Komplette Lokliste -> "loks"
    CS2CanCommand cmd;
    //                     | cmd      | hash      | len | Ascii-code "loks" 6C->l, 6F->o, 6B->k, ...
    unsigned char data[] = {0x00, 0x40, 0x03, 0x00, 0x08, 0x6C, 0x6F, 0x6B, 0x73, 0x00, 0x00, 0x00, 0x00};
    memcpy((void*)&cmd, data, 13);
    return cmd;
}

inline CS2CanCommand getLokStat() {
    // Aktueller Zustand der Loks -> "lokstat"
    CS2CanCommand cmd;
    //                     | cmd      | hash      | len | Ascii-code "loks" 6C->l, 6F->o, 6B->k, ...
    unsigned char data[] = {0x00, 0x40, 0x03, 0x00, 0x08, 0x6C, 0x6F, 0x6B, 0x73, 0x74, 0x61, 0x74, 0x00};
    memcpy((void*)&cmd, data, 13);
    return cmd;
}

inline CanCommand operator |(CanCommand a, CanCommand b) {
    return static_cast<CanCommand>(static_cast<int>(a) | static_cast<int>(b));
}

inline CanCommand operator |(CanCommand a, int b) {
    return static_cast<CanCommand>(static_cast<int>(a) | b);
}
