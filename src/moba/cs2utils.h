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
#include <cstring>
#include <algorithm>

#include "cs2cancommand.h"

std::string getCommandAsString(const CS2CanCommand &cmd);
std::string getCommandName(const CS2CanCommand &cmd);
std::string getCommandName(int cmd);
std::string getSystemSubCommandName(const CS2CanCommand &cmd);
std::string getSystemSubCommandName(int subCmd);

CanCommand getCommandFromString(const std::string &cmd);
CanSystemSubCommand getSystemSubCommandFromString(const std::string &cmd);

inline bool isResponse(const CS2CanCommand &cmd) {
    return cmd.header[1] & 0x01;
}

inline CS2CanCommand setLocSpeed(const std::uint32_t localId, const std::uint16_t speed) {
    return CS2CanCommand{CanCommand::CMD_LOCO_SPEED, 6, localId, speed};
}

inline CS2CanCommand setLocFunction(const std::uint32_t localId, const std::uint8_t function, const bool on) {
    return CS2CanCommand{CanCommand::CMD_LOCO_FUNCTION, 6, localId, function, static_cast<std::uint8_t>(on ? 1 : 0)};
}

inline CS2CanCommand setLocDirection(const std::uint32_t localId, const std::uint8_t direction) {
    return CS2CanCommand{CanCommand::CMD_LOCO_DIRECTION, 5, localId, direction};
}

inline CS2CanCommand setSwitch(const std::uint32_t localId, const bool red, const bool on) {
    return CS2CanCommand{CanCommand::CMD_SET_SWITCH, 6, localId, static_cast<std::uint8_t>(red ? 1 : 0), static_cast<std::uint8_t>(on ? 0 : 1)};
}

inline CS2CanCommand setLocoHalt(const std::uint32_t localId) {
    return CS2CanCommand{CanCommand::CMD_SYSTEM, 5, localId, static_cast<std::uint8_t>(CanSystemSubCommand::SYS_SUB_CMD_LOCO_EMERGENCY_STOP)};
}

inline CS2CanCommand setEmergencyStop() {
    return CS2CanCommand{CanCommand::CMD_SYSTEM, 5, static_cast<std::uint32_t>(0x0), static_cast<std::uint8_t>(CanSystemSubCommand::SYS_SUB_CMD_SYSTEM_STOP)};
}

inline CS2CanCommand setEmergencyStopClearing() {
    return CS2CanCommand{CanCommand::CMD_SYSTEM, 5, static_cast<std::uint32_t>(0x0), static_cast<std::uint8_t>(CanSystemSubCommand::SYS_SUB_CMD_SYSTEM_GO)};
}

inline CS2CanCommand setHalt() {
    return CS2CanCommand{CanCommand::CMD_SYSTEM, 5, static_cast<std::uint32_t>(0x0), static_cast<std::uint8_t>(CanSystemSubCommand::SYS_SUB_CMD_SYSTEM_HALT)};
}

inline CS2CanCommand ping() {
    return CS2CanCommand{CanCommand::CMD_PING};
}

inline CS2CanCommand getConfigData(const char *filename) {
    CS2CanCommand cmd;
    unsigned char data[] = {0x00, 0x40, 0x03, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    constexpr std::size_t maxLength = 8;
    memcpy(&data[5], filename, std::min(strlen(filename), maxLength));
    memcpy(&cmd, data, 13);
    return cmd;
}

inline CS2CanCommand getLokList() {
    return getConfigData("loks");
}

inline CS2CanCommand getLokStat() {
    return getConfigData("lokstat");
}

inline CS2CanCommand getMagList() {
    return getConfigData("mags");
}

inline CS2CanCommand getMagStat() {
    return getConfigData("magstat");
}

inline std::uint32_t convertMMToLocId(const std::uint8_t addr) {
    return 12288 + addr - 1;
}

inline CanCommand operator |(const CanCommand a, const CanCommand b) {
    return static_cast<CanCommand>(static_cast<int>(a) | static_cast<int>(b));
}

inline CanCommand operator |(const CanCommand a, const int b) {
    return static_cast<CanCommand>(static_cast<int>(a) | b);
}
