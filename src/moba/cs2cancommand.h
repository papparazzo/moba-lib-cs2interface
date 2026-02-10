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

#include <iomanip>
#include <cstring>
#include <stdexcept>
#include <arpa/inet.h>

enum class CanCommand {
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

enum class CanSystemSubCommand {
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
    SYS_SUB_CMD_SYSTEM_OVERLOAD                    = 0x0A,
    SYS_SUB_CMD_SYSTEM_STATUS                      = 0x0B,
    SYS_SUB_CMD_SYSTEM_IDENTIFIER                  = 0x0C,
    SYS_SUB_CMD_MFX_SEEK                           = 0x30,
    SYS_SUB_CMD_SYSTEM_RESET                       = 0x80,
};

struct CS2CanCommand {
    explicit CS2CanCommand(const CanCommand cmd = CanCommand::CMD_SYSTEM) {
        setHeader(cmd);
    }

    CS2CanCommand(
        const CanCommand cmd,
        const std::uint8_t length,
        const std::uint8_t data0,
        const std::uint8_t data1 = 0x00,
        const std::uint8_t data2 = 0x00,
        const std::uint8_t data3 = 0x00,
        const std::uint8_t data4 = 0x00,
        const std::uint8_t data5 = 0x00,
        const std::uint8_t data6 = 0x00,
        const std::uint8_t data7 = 0x00
    ) {
        setHeader(cmd, length);

        data[0] = data0;
        data[1] = data1;
        data[2] = data2;
        data[3] = data3;

        data[4] = data4;
        data[5] = data5;
        data[6] = data6;
        data[7] = data7;
    }

    CS2CanCommand(
        const CanCommand cmd,
        const std::uint8_t length,
        std::uint32_t ident,
        std::uint16_t dataWord,
        const std::uint8_t data2 = 0x00,
        const std::uint8_t data3 = 0x00
    ) {
        setHeader(cmd, length);

        ident = htonl(ident);
        memcpy(data, &ident, 4);

        dataWord = htons(dataWord);
        memcpy(&data[4], &dataWord, 2);

        data[6] = data2;
        data[7] = data3;
    }

    CS2CanCommand(
        const CanCommand cmd, const std::uint8_t length,
        std::uint32_t ident,
        const std::uint8_t data4,
        const std::uint8_t data5 = 0x00,
        const std::uint8_t data6 = 0x00,
        const std::uint8_t data7 = 0x00
    ) {
        setHeader(cmd, length);
        ident = htonl(ident);
        memcpy(data, &ident, 4);

        data[4] = data4;
        data[5] = data5;
        data[6] = data6;
        data[7] = data7;
    }

    CS2CanCommand(
        const CanCommand cmd,
        const std::uint8_t length,
        std::uint32_t ident,
        const std::uint8_t data0,
        std::uint16_t dataWord1
    ) {
        setHeader(cmd, length);
        ident = htonl(ident);
        memcpy(data, &ident, 4);

        data[4] = data0;

        dataWord1 = htons(dataWord1);
        memcpy(&data[5], &dataWord1, 2);
    }

    // S88 Event...
    CS2CanCommand(
        const CanCommand cmd,
        const std::uint8_t length,
        std::uint16_t module,
        std::uint16_t contact,
        const std::uint8_t oldState,
        const std::uint8_t newState,
        std::uint16_t time
    ) {
        setHeader(cmd, length);
        module = htons(module);
        memcpy(&data[0], &module, 2);
        contact = htons(contact);
        memcpy(&data[2], &contact, 2);
        data[4] = oldState;
        data[5] = newState;
        time = htons(time);
        memcpy(&data[6], &time, 2);
    }

    [[nodiscard]]
    std::uint32_t getUID() const {
        return getDoubleWordAt0();
    }

    [[nodiscard]]
    std::uint32_t getDoubleWordAt0() const {
        return getDoubleWord(0);
    }

    [[nodiscard]]
    std::uint32_t getDoubleWordAt4() const {
        return getDoubleWord(4);
    }

    [[nodiscard]]
    std::uint64_t getLongWord() const {
        std::uint64_t word;
        std::memcpy(&word, data, 8);
        return word;
    }

    [[nodiscard]]
    std::uint16_t getWordAt0() const {
        return getWord(0);
    }

    [[nodiscard]]
    std::uint16_t getWordAt2() const {
        return getWord(2);
    }

    [[nodiscard]]
    std::uint16_t getWordAt4() const {
        return getWord(4);
    }

    [[nodiscard]]
    std::uint16_t getWordAt5() const {
        return getWord(5);
    }

    [[nodiscard]]
    std::uint16_t getWordAt6() const {
        return getWord(6);
    }

    [[nodiscard]]
    bool isSystemCommand() const {
        return (header[1] & ~0x01) == static_cast<std::uint8_t>(CanCommand::CMD_SYSTEM);
    }

    [[nodiscard]]
    CanSystemSubCommand getSystemSubCommand() const {
        if(!isSystemCommand()) {
            throw std::out_of_range{"Command is not a sub command"};
        }
        return static_cast<CanSystemSubCommand>(data[4]);
    }

    [[nodiscard]]
    CanCommand getCanCommand() const {
        const std::uint8_t head = header[1] & ~0x01;
        return static_cast<CanCommand>(head);
    }

    [[nodiscard]]
    bool isResponse() const {
        return header[1] & 0x01;
    }

    CS2CanCommand setResponse() {
        header[1] |= 0x01;
        return *this;
    }

    CS2CanCommand unsetResponse() {
        header[1] &= ~0x01;
        return *this;
    }

    [[nodiscard]]
    std::string getAsString() const {
        std::stringstream ss;
        ss << *this;
        return  ss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const CS2CanCommand& cmd) {
        const std::ios_base::fmtflags flags(os.flags());

        os << std::uppercase << std::hex << std::setfill('0')
           << std::setw(2) << static_cast<unsigned int>(cmd.header[0]) << " "
           << std::setw(2) << static_cast<unsigned int>(cmd.header[1]) << " - "
           << std::setw(2) << static_cast<unsigned int>(cmd.hash[0]) << " "
           << std::setw(2) << static_cast<unsigned int>(cmd.hash[1]) << " - "
           << std::setw(2) << static_cast<unsigned int>(cmd.len) << " - "
           << std::setw(2) << static_cast<unsigned int>(cmd.data[0]) << " "
           << std::setw(2) << static_cast<unsigned int>(cmd.data[1]) << " "
           << std::setw(2) << static_cast<unsigned int>(cmd.data[2]) << " "
           << std::setw(2) << static_cast<unsigned int>(cmd.data[3]) << " - "
           << std::setw(2) << static_cast<unsigned int>(cmd.data[4]) << " "
           << std::setw(2) << static_cast<unsigned int>(cmd.data[5]) << " "
           << std::setw(2) << static_cast<unsigned int>(cmd.data[6]) << " "
           << std::setw(2) << static_cast<unsigned int>(cmd.data[7]);

        os.flags(flags);
        return os;
    }

    std::uint8_t header[2]{};
    std::uint8_t hash[2]{};
    std::uint8_t len{};
    std::uint8_t data[8]{};

private:
    void setHeader(const CanCommand cmd, const std::uint8_t length = 0x00) {
        header[0] = 0x00;
        header[1] = static_cast<std::uint8_t>(cmd);
        hash[0] = 0x03;
        hash[1] = 0x00;
        len = length;
    }

    [[nodiscard]]
    std::uint16_t getWord(const unsigned int pos) const {
        std::uint16_t data16;
        std::memcpy(&data16, &data[pos], 2);
        return ntohs(data16);
    }

    [[nodiscard]]
    std::uint32_t getDoubleWord(const unsigned int pos) const {
        std::uint32_t data32;
        std::memcpy(&data32, &data[pos], 4);
        return ntohl(data32);
    }
};
