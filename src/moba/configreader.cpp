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
        throw ConfigReaderException{"currently parsing"};
    }

    switch(cmd.len) {
        case 6:
        case 7:
            parsing = true;
            firstByte = true;
            cfgData.crc = cmd.getWordAt4();
            cfgData.dataLengthCompressed = cmd.getDoubleWordAt0();
            cfgData.dataCompressed.clear();
            cfgData.dataCompressed.reserve(cfgData.dataLengthCompressed);
            return HANDLED_MORE_TO_COME;

        case 8:
            if(firstByte) {
                firstByte = false;
                cfgData.dataLengthDecompressed = cmd.getDoubleWordAt0();
            }
            for(unsigned char i : cmd.data) {
                cfgData.dataCompressed.push_back(i);
            }

            if(cfgData.dataCompressed.size() >= cfgData.dataLengthCompressed) {
                parsing = false;
                handleConfigWriter();
                return HANDLED_AND_FINISHED;
            }
            return HANDLED_MORE_TO_COME;

        default:
// FIXME make this possible
#ifdef __cpp_lib_format
            throw ConfigReaderException{
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

    if(getCRC(&cfgData.dataCompressed[0], cfgData.dataCompressed.size()) != cfgData.crc) {
        throw ConfigReaderException{"crc-check failed!"};
    }
    unzipData();
}

void ConfigReader::unzipData() {
    ZipStream zipStream;

    zipStream.strm.next_in = &cfgData.dataCompressed[4];
    zipStream.strm.avail_in = cfgData.dataCompressed.size() - 4;

    if(zipStream.strm.avail_in == 0) {
        return;
    }

    unsigned char out[cfgData.dataLengthDecompressed];

    zipStream.strm.avail_out = cfgData.dataLengthDecompressed;
    zipStream.strm.next_out = out;

    if(inflate(&zipStream.strm, Z_NO_FLUSH) != Z_STREAM_END) {
        throw ConfigReaderException{"decompress of stream failed"};
    }

    const auto d = std::string(reinterpret_cast<char*>(out), cfgData.dataLengthDecompressed);

    const auto p = d.find(']');
    if(std::string::npos == p) {
        throw ConfigReaderException{"missing ']' in stream"};
    }

    const auto k = d.substr(1, p - 1);
    const auto iter = handlers.find(k);

    if(handlers.end() == iter) {
        return;
    }

    iter->second->handleConfigData(d);
}

