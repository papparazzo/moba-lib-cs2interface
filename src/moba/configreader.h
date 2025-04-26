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

#include "cancommandhandlerinterface.h"
#include "configreaderhandlerinterface.h"
#include "cs2cancommand.h"
#include "configreaderexception.h"
#include <exception>
#include <cstdint>
#include <string>
#include <vector>
#include <zlib.h>
#include <functional>
#include <map>

class ConfigReader: public CanCommandHandlerInterface {
public:

    ConfigReader() = default;

    ConfigReader(const ConfigReader& orig) = delete;

    virtual ~ConfigReader() noexcept = default;

    HandlerReturn handleCanCommand(const CS2CanCommand &cmd) override;

    void addHandler(const ConfigReaderHandlerPtr& handler);

protected:
    void handleConfigWriter();

    static std::uint16_t updateCRC(std::uint16_t crc, std::uint8_t input);

    static std::uint16_t getCRC(const std::uint8_t *data, std::size_t length);

    void unzipData();

    std::map<std::string, ConfigReaderHandlerPtr> handlers;

    struct ConfigData {
        std::uint32_t dataLengthDecompresed;
        std::uint32_t dataLengthCompressed;
        std::uint16_t crc;

        std::vector<std::uint8_t> dataCompressed;

    } cfgData;

    struct ZipStream {
        ZipStream() {
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            strm.avail_in = 0;
            strm.next_in = Z_NULL;

            if(inflateInit(&strm) != Z_OK){
                throw ConfigReaderException{"inflateInit failed"};
            }
        }
        virtual ~ZipStream() {
            inflateEnd(&strm);
        }
        z_stream strm{};
    };
};
