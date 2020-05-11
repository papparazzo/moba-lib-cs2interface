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

#pragma once

#include "cs2cancommand.h"
#include <exception>
#include <cstdint>
#include <string>
#include <vector>
#include <zlib.h>

class ParseConfigException : public std::exception {
public:
    virtual ~ParseConfigException() noexcept {
    }

    ParseConfigException(const std::string &what) {
        this->what__ = what;
    }

    virtual const char* what() const noexcept {
        return this->what__.c_str();
    }

private:
    std::string what__;
};

class ConfigParser {
public:
    ConfigParser() {
    }

    virtual ~ConfigParser() noexcept {
    }

    bool handleCanCommand(const CS2CanCommand &cmd);

protected:
    void handleConfigWriter();

    std::uint16_t updateCRC(std::uint16_t crc, std::uint8_t input);
    std::uint16_t getCRC(std::uint8_t *data, std::size_t length);

    void unzipData();

    struct ConfigData {
        std::uint32_t dataLengthDecompresed;
        std::uint32_t dataLengthCompressed;
        std::uint16_t crc;

        std::vector<std::uint8_t> dataCompressed;

    } configData;

    struct ZipStream {
        ZipStream() {
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            strm.avail_in = 0;
            strm.next_in = Z_NULL;

            if(inflateInit(&strm) != Z_OK){
                throw ParseConfigException{"inflateInit failed"};
            }
        }
        virtual ~ZipStream() {
            inflateEnd(&strm);
        }
        z_stream strm;
    };
};
