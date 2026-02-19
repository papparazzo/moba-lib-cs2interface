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

#include "configdatacompressor.h"

#include <zlib.h>

#include "configwriter.h"

namespace {
    struct ZipStream {
        virtual ~ZipStream() = 0;

        ZipStream() {
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
        }

        z_stream strm{};
    };

    ZipStream::~ZipStream() = default;

    struct ZipInflateStream final: ZipStream {
        ZipInflateStream() {
            strm.avail_in = 0;
            strm.next_in = Z_NULL;

            if(inflateInit(&strm) != Z_OK){
                throw ConfigException{"inflateInit failed"};
            }
        }

        ~ZipInflateStream() override {
            inflateEnd(&strm);
        }
    };

    struct ZipDeflateStream final: ZipStream {
        ZipDeflateStream() {
            if(deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
                throw ConfigException{"deflateInit failed"};
            }
        }

        ~ZipDeflateStream() override {
            deflateEnd(&strm);
        }
    };
}

ConfigDataCompressor::ConfigData ConfigDataCompressor::zipData(const std::string& data) {
    ConfigData configData;

    ZipDeflateStream zipStream;

    configData.dataLengthDecompressed = static_cast<std::uint32_t>(data.size());

    if(configData.dataLengthDecompressed == 0) {
        configData.dataCompressed.clear();
        configData.dataLengthCompressed = 0;
        configData.crc = 0;
        return configData;
    }

    const auto maxCompressed = compressBound(configData.dataLengthDecompressed);
    if(maxCompressed > std::numeric_limits<uInt>::max()) {
        throw ConfigException{"compressed data too large"};
    }
    const auto outCapacity = static_cast<uInt>(maxCompressed);

    configData.dataCompressed.resize(4 + outCapacity);

    const std::uint32_t len = htonl(configData.dataLengthDecompressed);
    std::memcpy(configData.dataCompressed.data(), &len, 4);

    zipStream.strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(data.data()));
    zipStream.strm.avail_in = configData.dataLengthDecompressed;
    zipStream.strm.next_out = &configData.dataCompressed[4];
    zipStream.strm.avail_out = outCapacity;

    if(deflate(&zipStream.strm, Z_FINISH) != Z_STREAM_END) {
        throw ConfigException{"compress of stream failed"};
    }

    const auto compressedSize = outCapacity - zipStream.strm.avail_out;
    configData.dataCompressed.resize(4 + compressedSize);

    if(
        const auto remainder = configData.dataCompressed.size() % 8;
        remainder != 0
    ) {
        configData.dataCompressed.resize(configData.dataCompressed.size() + (8 - remainder), 0x00);
    }

    configData.dataLengthCompressed = configData.dataCompressed.size();
    configData.crc = getCRC(&configData.dataCompressed[0], configData.dataCompressed.size());
    return configData;
}

std::uint16_t ConfigDataCompressor::getCRC(const std::uint8_t *data, const std::size_t length) {
    std::uint16_t crc = 0xFFFF;

    for(std::size_t count = 0; count < length; ++count) {
        crc = updateCRC(crc, *data++);
    }
    return crc;
}

std::uint16_t ConfigDataCompressor::updateCRC(std::uint16_t crc, const std::uint8_t input) {
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
