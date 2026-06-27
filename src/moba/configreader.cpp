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

#include "configreader.h"

#include "configdatacompressor.h"
#include "configexception.h"

#if __has_include(<format>)
#include <format>
#endif

void ConfigReader::addHandler(const ConfigReaderHandlerPtr& handler) {
    handlers[handler->getName()] = handler;
}

ConfigReader::HandlerReturn ConfigReader::handleCanCommand(const CS2CanCommand &cmd) {
    if(cmd.header[1] != static_cast<uint8_t>(CanCommand::CMD_CONFIG_DATA_STREAM)) {
        return NOT_HANDLED;
    }

    static bool firstByte = false;
    static bool parsing = false;

    if(((cmd.len == 6 || cmd.len == 7) && parsing) || (cmd.len == 8 && !parsing)) {
        throw ConfigException{"currently parsing"};
    }

    switch(cmd.len) {
        case 6:
        case 7:
            parsing = true;
            firstByte = true;
            configData.crc = cmd.getWordAt4();
            configData.dataLengthCompressed = cmd.getDoubleWordAt0();
            configData.dataCompressed.clear();
            configData.dataCompressed.reserve(configData.dataLengthCompressed);
            return HANDLED_MORE_TO_COME;

        case 8:
            if(firstByte) {
                firstByte = false;
                configData.dataLengthDecompressed = cmd.getDoubleWordAt0();
            }
            for(unsigned char i : cmd.data) {
                configData.dataCompressed.push_back(i);
            }

            if(configData.dataCompressed.size() >= configData.dataLengthCompressed) {
                parsing = false;
                handleConfigWriter();
                return HANDLED_AND_FINISHED;
            }
            return HANDLED_MORE_TO_COME;

        default:
// FIXME make this possible
#ifdef __cpp_lib_format
            throw ConfigException{
                std::format("invalid data length <{}> given", cmd.len)
            };
#else
            throw ConfigReaderException{"invalid data length given"};
#endif
    }
}

std::uint16_t ConfigReader::updateCRC(std::uint16_t crc, const std::uint8_t input) {
    crc = crc ^ input << 8;
    for(int i = 0; i < 8; i++) {
        if((crc & 0x8000) == 0x8000) {
            crc = crc << 1;
            crc ^= 0x1021;
        } else {
            crc = crc << 1;
        }
    }
    return crc;
}

std::uint16_t ConfigReader::getCRC(const std::uint8_t *data, const std::size_t length) {
    std::uint16_t crc = 0xFFFF;

    for(size_t count = 0; count < length; ++count) {
        crc = updateCRC(crc, *data++);
    }
    return crc;
}

void ConfigReader::handleConfigWriter() {

    const auto d = ConfigDataCompressor::unzipData(configData);

    const auto p = d.find(']');
    if(std::string::npos == p) {
        throw ConfigException{"missing ']' in stream"};
    }

    const auto k = d.substr(1, p - 1);
    const auto iter = handlers.find(k);

    if(handlers.end() == iter) {
        return;
    }

    iter->second->handleConfigData(d);
}

