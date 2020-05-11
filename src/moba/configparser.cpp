/*
 *  Project:    moba-lib-cs2interace
 *
 *  Copyright (C) 2019 Stefan Paproth <pappi-@gmx.de>
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

#include "configparser.h"

#include <cstring>
#include <iostream>

bool ConfigParser::handleCanCommand(const CS2CanCommand &cmd) {
    if(
        cmd.header[1] != static_cast<uint8_t>(CanCommand::CMD_CONFIG_DATA_STREAM)
    ) {
        return false;
    }

    static bool firstByte = false;
    static bool parsing = false;

    if((cmd.len == 6 && parsing) || (cmd.len == 8 && !parsing)) {
        throw ParseConfigException{"currently parsing"};
    }

    switch(cmd.len) {
        case 6:
        case 7:
            parsing = true;
            firstByte = true;
            configData.crc = cmd.getWordAt4();
            configData.dataLengthCompressed = cmd.getDoubleWordAt0();
            configData.dataCompressed.reserve(configData.dataLengthCompressed);
            return true;

        case 8:
            if(firstByte) {
                firstByte = false;
                configData.dataLengthDecompresed = cmd.getDoubleWordAt0();
            }
            for(int i = 0; i < 8; ++i) {
                configData.dataCompressed.push_back(cmd.data[i]);
            }

            if(configData.dataCompressed.size() >= configData.dataLengthCompressed) {
                parsing = false;
                handleConfigWriter();
            }
            return true;

        default:
            throw ParseConfigException{"invalid data length given"};
    }
}

std::uint16_t ConfigParser::updateCRC(std::uint16_t crc, std::uint8_t input) {
    crc = crc ^ (input << 8);
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

std::uint16_t ConfigParser::getCRC(std::uint8_t *data, std::size_t length) {
    std::uint16_t crc = 0xFFFF;

    for(size_t count = 0; count < length; ++count) {
        crc = updateCRC(crc, *data++);
    }
    return crc;
}

void ConfigParser::handleConfigWriter() {

    if(getCRC(&configData.dataCompressed[0], configData.dataCompressed.size()) != configData.crc) {
        throw ParseConfigException{"crc-check failed!"};
    }

    unzipData();
}

void ConfigParser::unzipData() {
    ZipStream zipStream;

    zipStream.strm.next_in = &configData.dataCompressed[4];
    zipStream.strm.avail_in = configData.dataCompressed.size() - 4;

    if(zipStream.strm.avail_in == 0) {
        return;
    }

    std::cout << configData.dataLengthDecompresed << std::endl;

    unsigned char out[configData.dataLengthDecompresed];

    zipStream.strm.avail_out = configData.dataLengthDecompresed;
    zipStream.strm.next_out = out;

    if(inflate(&zipStream.strm, Z_NO_FLUSH) != Z_STREAM_END) {
        throw ParseConfigException{"decompress of stream failed"};
    }

    std::cerr << out;
}

