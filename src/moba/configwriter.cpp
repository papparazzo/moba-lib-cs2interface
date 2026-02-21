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

#include "configwriter.h"

#include <cstring>
#include <utility>

#include "configdatacompressor.h"
#include "configexception.h"

ConfigWriter::ConfigWriter(CS2WriterPtr cs2writer)
: cs2writer{std::move(cs2writer)} {
    if(!this->cs2writer) {
        throw ConfigException{"cs2writer is null"};
    }
}

void ConfigWriter::handleData(const std::string& data) const {
    const auto configData = ConfigDataCompressor::zipData(data);

    if(configData.dataCompressed.empty()) {
        return;
    }

    const CS2CanCommand startCmd{
        CanCommand::CMD_CONFIG_DATA_STREAM,
        6,
        configData.dataLengthCompressed,
        configData.crc
    };
    cs2writer->send(startCmd);

    for(std::size_t offset = 0; offset < configData.dataCompressed.size(); offset += 8) {
        std::uint8_t chunk[8]{};
        const auto remaining = configData.dataCompressed.size() - offset;
        const auto copySize = remaining >= 8 ? 8 : remaining;
        std::memcpy(chunk, &configData.dataCompressed[offset], copySize);

        CS2CanCommand cmd{
            CanCommand::CMD_CONFIG_DATA_STREAM,
            8,
            chunk[0], chunk[1], chunk[2], chunk[3],
            chunk[4], chunk[5], chunk[6], chunk[7]
        };
        cs2writer->send(cmd);
    }
}
