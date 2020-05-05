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

#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

enum CanCommand {
    CMD_SYSTEM                                     = 0x00,
    CMD_LOCO_DISCOVERY                             = 0x02,
    CMD_MFX_BIND                                   = 0x04,
    CMD_MFX_VERIFY                                 = 0x06,
    CMD_LOCO_SPEED                                 = 0x08,
    CMD_LOCO_DIRECTION                             = 0x0A,
    CMD_LOCO_FUNCTION                              = 0x0C,
    CMD_READ_CONFIG                                = 0x0E,
    CMD_WRITE_CONFIG                               = 0x10,
    CMD_SET_SWITCH                                 = 0x16,
    CMD_ATTACHMENTS_CONFIG                         = 0x18,
    CMD_S88_POLLING                                = 0x20,
    CMD_S88_EVENT                                  = 0x22,
    CMD_SX1_EVENT                                  = 0x24,
    CMD_PING                                       = 0x30,
    CMD_UPDATE_OFFER                               = 0x32,
    CMD_READ_CONFIG_DATA                           = 0x34,
    CMD_BOOTLOADER_CAN                             = 0x36,
    CMD_BOOTLOADER_TRACK                           = 0x38,
    CMD_STATUS_DATA_CONFIGURATION                  = 0x3A,
    CMD_CONFIG_DATA_QUERY                          = 0x40,
    CMD_CONFIG_DATA_STREAM                         = 0x42,
    CMD_60128_CONNECT_6021_DATA_STREAM             = 0x44,
};

enum CanSystemSubCommand {
    SYS_SUB_CMD_SYSTEM_STOP                        = 0x00,
    SYS_SUB_CMD_SYSTEM_GO                          = 0x01,
    SYS_SUB_CMD_SYSTEM_HALT                        = 0x02,
    SYS_SUB_CMD_LOCO_EMERGENCY_STOP                = 0x03,
    SYS_SUB_CMD_LOCO_CYCLE_STOP                    = 0x04,
//    SYS_SUB_CMD_Lok Datenprotokoll                 = 0x05,
    SYS_SUB_CMD_CIRCUIT_TIME_ATTACHMENTS_DECODER   = 0x06,
    SYS_SUB_CMD_FAST_READ_MFX                      = 0x07,
//    SYS_SUB_CMD_Gleisprotokoll frei schalten       = 0x08,
//    SYS_SUB_CMD_System MFX Neuanmeldezähler setzen = 0x09,
    SYS_SUB_CMD_SYSTEM_OVERLAOD                    = 0x0A,
    SYS_SUB_CMD_SYSTEM_STATUS                      = 0x0B,
    SYS_SUB_CMD_SYSTEM_IDENTIFIER                  = 0x0C,
    SYS_SUB_CMD_MFX_SEEK                           = 0x30,
    SYS_SUB_CMD_SYSTEM_RESET                       = 0x80,
};

struct CS2CanCommand {
    CS2CanCommand(CanCommand cmd = CanCommand::CMD_SYSTEM) {
        setHeader(cmd);
    }

    CS2CanCommand(
        CanCommand cmd, std::uint8_t length, std::uint8_t data0, std::uint8_t data1 = 0x00, std::uint8_t data2 = 0x00,
        std::uint8_t data3 = 0x00, std::uint8_t data4 = 0x00, std::uint8_t data5 = 0x00, std::uint8_t data6 = 0x00,
        std::uint8_t data7 = 0x00
    ) {
        setHeader(cmd, length);

        uid[0] = data0;
        uid[1] = data1;
        uid[2] = data2;
        uid[3] = data3;

        data[0] = data4;
        data[1] = data5;
        data[2] = data6;
        data[3] = data7;
    }

    CS2CanCommand(
        CanCommand cmd, std::uint8_t length, std::uint32_t uident, std::uint16_t dataWord, std::uint8_t data2 = 0x00, std::uint8_t data3 = 0x00
    ) {
        setHeader(cmd, length);

        uident = htonl(uident);
        memcpy(uid, &uident, 4);

        dataWord = htons(dataWord);
        memcpy(data, &dataWord, 2);

        data[2] = data2;
        data[3] = data3;
    }

    CS2CanCommand(
        CanCommand cmd, std::uint8_t length, std::uint32_t uident,
        std::uint8_t data0, std::uint8_t data1 = 0x00, std::uint8_t data2 = 0x00, std::uint8_t data3 = 0x00
    ) {
        setHeader(cmd, length);
        uident = htonl(uident);
        memcpy(uid, &uident, 4);

        data[0] = data0;
        data[1] = data1;
        data[2] = data2;
        data[3] = data3;
    }

    CS2CanCommand(CanCommand cmd, std::uint8_t length, std::uint32_t uident, std::uint8_t data0, std::uint16_t dataWord1) {
        setHeader(cmd, length);
        uident = htonl(uident);
        memcpy(uid, &uident, 4);

        data[0] = data0;

        dataWord1 = htons(dataWord1);
        memcpy(&data[1], &dataWord1, 2);
    }

    std::uint32_t getUID() {
        std::uint32_t uident;
        std::memcpy(&uident, uid, 4);
        return ntohl(uident);
    }

    std::uint16_t getShortAt4() {
        return getShort(0);
    }

    std::uint16_t getShortAt5() {
        return getShort(1);
    }

    std::uint16_t getShortAt6() {
        return getShort(2);
    }

    std::uint8_t header[2];
    std::uint8_t hash[2];
    std::uint8_t len;
    std::uint8_t uid[4];
    std::uint8_t data[4];

private:
    void setHeader(CanCommand cmd, std::uint8_t length = 0x00) {
        header[0] = 0x00;
        header[1] = static_cast<std::uint8_t>(cmd);
        hash[0] = 0x03;
        hash[1] = 0x00;
        len = length;
    }

    std::uint16_t getShort(int pos) {
        uint16_t data16;
        std::memcpy(&data16, &data[pos], 2);
        return ntohs(data16);
    }
};
