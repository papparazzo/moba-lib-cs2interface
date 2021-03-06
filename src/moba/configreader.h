/*
 *  Project:    moba-lib-cs2interace
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

#include "cancommandhandlerinterface.h"
#include "cs2cancommand.h"
#include <exception>
#include <cstdint>
#include <string>
#include <vector>
#include <zlib.h>
#include <functional>

class ConfigReaderException : public std::exception {
public:
    virtual ~ConfigReaderException() noexcept {
    }

    ConfigReaderException(const std::string &what) {
        this->what__ = what;
    }

    virtual const char* what() const noexcept {
        return this->what__.c_str();
    }

private:
    std::string what__;
};

class ConfigReader: public CanCommadHandlerInterface {
public:
    ConfigReader(std::function<void(unsigned char*, std::uint32_t length)> callback);

    virtual ~ConfigReader() noexcept {
    }

    bool handleCanCommand(const CS2CanCommand &cmd);

protected:
    void handleConfigWriter();

    std::uint16_t updateCRC(std::uint16_t crc, std::uint8_t input);
    std::uint16_t getCRC(std::uint8_t *data, std::size_t length);

    void unzipData();

    std::function<void(unsigned char*, std::uint32_t length)> callback;

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
                throw ConfigReaderException{"inflateInit failed"};
            }
        }
        virtual ~ZipStream() {
            inflateEnd(&strm);
        }
        z_stream strm;
    };
};

