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

#include "cs2utils.h"
#include "cs2cancommand.h"
#include "cancommandexception.h"

std::string getCommandAsString(int cmd) {
    switch(cmd) {
        case CMD_SYSTEM:
            return "CMD_SYSTEM";

        case CMD_LOCO_DISCOVERY:
            return "CMD_LOCO_DISCOVERY";

        case CMD_MFX_BIND:
            return "CMD_MFX_BIND";

        case CMD_MFX_VERIFY:
            return "CMD_MFX_VERIFY";

        case CMD_LOCO_SPEED:
            return "CMD_LOCO_SPEED";

        case CMD_LOCO_DIRECTION:
            return "CMD_LOCO_DIRECTION";

        case CMD_LOCO_FUNCTION:
            return "CMD_LOCO_FUNCTION";

        case CMD_READ_CONFIG:
            return "CMD_READ_CONFIG";

        case CMD_WRITE_CONFIG:
            return "CMD_WRITE_CONFIG";

        case CMD_SET_SWITCH:
            return "CMD_SET_SWITCH";

        case CMD_ATTACHMENTS_CONFIG:
            return "CMD_ATTACHMENTS_CONFIG";

        case CMD_S88_POLLING:
            return "CMD_S88_POLLING";

        case CMD_S88_EVENT:
            return "CMD_S88_EVENT";

        case CMD_SX1_EVENT:
            return "CMD_SX1_EVENT";

        case CMD_PING:
            return "CMD_PING";

        case CMD_UPDATE_OFFER:
            return "CMD_UPDATE_OFFER";

        case CMD_READ_CONFIG_DATA:
            return "CMD_READ_CONFIG_DATA";

        case CMD_BOOTLOADER_CAN:
            return "CMD_BOOTLOADER_CAN";

        case CMD_BOOTLOADER_TRACK:
            return "CMD_BOOTLOADER_TRACK";

        case CMD_STATUS_DATA_CONFIGURATION:
            return "CMD_STATUS_DATA_CONFIGURATION";

        case CMD_CONFIG_DATA_QUERY:
            return "CMD_CONFIG_DATA_QUERY";

        case CMD_CONFIG_DATA_STREAM:
            return "CMD_CONFIG_DATA_STREAM";

        case CMD_60128_CONNECT_6021_DATA_STREAM:
            return "CMD_60128_CONNECT_6021_DATA_STREAM";

        default:
            return "UNBEKANNT";
    }
}

std::string getSystemSubCommandAsString(int subCmd) {
    switch(subCmd) {
        case SYS_SUB_CMD_SYSTEM_STOP:
            return "SYS_SUB_CMD_SYSTEM_STOP";

        case SYS_SUB_CMD_SYSTEM_GO:
            return "SYS_SUB_CMD_SYSTEM_GO";

        case SYS_SUB_CMD_SYSTEM_HALT:
            return "SYS_SUB_CMD_SSYSTEM_HALT";

        case SYS_SUB_CMD_LOCO_EMERGENCY_STOP:
            return "SYS_SUB_CMD_LOCO_EMERGENCY_STOP";

        case SYS_SUB_CMD_LOCO_CYCLE_STOP:
            return "SYS_SUB_CMD_LOCO_CYCLE_STOP";

        //case         SYS_SUB_CMD_Lok Datenprotokoll
        case SYS_SUB_CMD_CIRCUIT_TIME_ATTACHMENTS_DECODER:
            return "SYS_SUB_CMD_CIRCUIT_TIME_ATTACHMENTS_DECODER";

        case SYS_SUB_CMD_FAST_READ_MFX:
            return "SYS_SUB_CMD_FAST_READ_MFX";

        //case         SYS_SUB_CMD_Gleisprotokoll frei schalten
        //case         SYS_SUB_CMD_System MFX Neuanmeldezähler s

        case SYS_SUB_CMD_SYSTEM_OVERLAOD:
            return "SYS_SUB_CMD_SYSTEM_OVERLAOD";

        case SYS_SUB_CMD_SYSTEM_STATUS:
            return "SYS_SUB_CMD_SYSTEM_STATUS";

        case SYS_SUB_CMD_SYSTEM_IDENTIFIER:
            return "SYS_SUB_CMD_SYSTEM_IDENTIFIER";

        case SYS_SUB_CMD_MFX_SEEK:
            return "SYS_SUB_CMD_MFX_SEEK";

        case SYS_SUB_CMD_SYSTEM_RESET:
            return "SYS_SUB_CMD_SYSTEM_RESET";

        default:
            return "UNBEKANNT";
    }
}

CanCommand getCommandFromString(const std::string &cmd) {
    if(cmd == "CMD_SYSTEM") {
        return CMD_SYSTEM;
    }
    if(cmd == "CMD_LOCO_DISCOVERY") {
        return CMD_LOCO_DISCOVERY;
    }
    if(cmd == "CMD_MFX_BIND") {
        return CMD_MFX_BIND;
    }
    if(cmd == "CMD_MFX_VERIFY") {
        return CMD_MFX_VERIFY;
    }
    if(cmd == "CMD_LOCO_SPEED") {
        return CMD_LOCO_SPEED;
    }
    if(cmd == "CMD_LOCO_DIRECTION") {
        return CMD_LOCO_DIRECTION;
    }
    if(cmd == "CMD_LOCO_FUNCTION") {
        return CMD_LOCO_FUNCTION;
    }
    if(cmd == "CMD_READ_CONFIG") {
        return CMD_READ_CONFIG;
    }
    if(cmd == "CMD_WRITE_CONFIG") {
        return CMD_WRITE_CONFIG;
    }
    if(cmd == "CMD_SET_SWITCH") {
        return CMD_SET_SWITCH;
    }
    if(cmd == "CMD_ATTACHMENTS_CONFIG") {
        return CMD_ATTACHMENTS_CONFIG;
    }
    if(cmd == "CMD_S88_POLLING") {
        return CMD_S88_POLLING;
    }
    if(cmd == "CMD_S88_EVENT") {
        return CMD_S88_EVENT;
    }
    if(cmd == "CMD_SX1_EVENT") {
        return CMD_SX1_EVENT;
    }
    if(cmd == "CMD_PING") {
        return CMD_PING;
    }
    if(cmd == "CMD_UPDATE_OFFER") {
        return CMD_UPDATE_OFFER;
    }
    if(cmd == "CMD_READ_CONFIG_DATA") {
        return CMD_READ_CONFIG_DATA;
    }
    if(cmd == "CMD_BOOTLOADER_CAN") {
        return CMD_BOOTLOADER_CAN;
    }
    if(cmd == "CMD_BOOTLOADER_TRACK") {
        return CMD_BOOTLOADER_TRACK;
    }
    if(cmd == "CMD_STATUS_DATA_CONFIGURATION") {
        return CMD_STATUS_DATA_CONFIGURATION;
    }
    if(cmd == "CMD_CONFIG_DATA_QUERY") {
        return CMD_CONFIG_DATA_QUERY;
    }
    if(cmd == "CMD_CONFIG_DATA_STREAM") {
        return CMD_CONFIG_DATA_STREAM;
    }
    if(cmd == "CMD_60128_CONNECT_6021_DATA_STREAM") {
        return CMD_60128_CONNECT_6021_DATA_STREAM;
    }
    throw CanCommandException{"Invalid can command given"};
}

CanSystemSubCommand getSystemSubCommandFromString(const std::string &cmd) {
    if(cmd == "SYS_SUB_CMD_SYSTEM_STOP") {
        return SYS_SUB_CMD_SYSTEM_STOP;
    }
    if(cmd == "SYS_SUB_CMD_SYSTEM_GO") {
        return SYS_SUB_CMD_SYSTEM_GO;
    }
    if(cmd == "SYS_SUB_CMD_SSYSTEM_HALT") {
        return SYS_SUB_CMD_SYSTEM_HALT;
    }
    if(cmd == "SYS_SUB_CMD_LOCO_EMERGENCY_STOP") {
        return SYS_SUB_CMD_LOCO_EMERGENCY_STOP;
    }
    if(cmd == "SYS_SUB_CMD_LOCO_CYCLE_STOP") {
        return SYS_SUB_CMD_LOCO_CYCLE_STOP;
    }
    //    return         SYS_SUB_CMD_Lok Datenprotokoll
    if(cmd == "SYS_SUB_CMD_CIRCUIT_TIME_ATTACHMENTS_DECODER") {
        return SYS_SUB_CMD_CIRCUIT_TIME_ATTACHMENTS_DECODER;
    }
    if(cmd == "SYS_SUB_CMD_FAST_READ_MFX") {
        return SYS_SUB_CMD_FAST_READ_MFX;
    }
    //    return         SYS_SUB_CMD_Gleisprotokoll frei schalten
    //    return         SYS_SUB_CMD_System MFX Neuanmeldezähler s
    if(cmd == "SYS_SUB_CMD_SYSTEM_OVERLAOD") {
        return SYS_SUB_CMD_SYSTEM_OVERLAOD;
    }
    if(cmd == "SYS_SUB_CMD_SYSTEM_STATUS") {
        return SYS_SUB_CMD_SYSTEM_STATUS;
    }
    if(cmd == "SYS_SUB_CMD_SYSTEM_IDENTIFIER") {
        return SYS_SUB_CMD_SYSTEM_IDENTIFIER;
    }
    if(cmd == "SYS_SUB_CMD_MFX_SEEK") {
        return SYS_SUB_CMD_MFX_SEEK;
    }
    if(cmd == "SYS_SUB_CMD_SYSTEM_RESET") {
        return SYS_SUB_CMD_SYSTEM_RESET;
    }
    throw CanCommandException{"Invalid sub command given"};
}

