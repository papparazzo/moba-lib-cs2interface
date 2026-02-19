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

#include "configexception.h"

#include <vector>
#include <cstdint>

class ConfigDataCompressor {
public:
    struct ConfigData final {
        std::uint32_t dataLengthDecompressed;
        std::uint32_t dataLengthCompressed;
        std::uint16_t crc;

        std::vector<std::uint8_t> dataCompressed;
    };

    static ConfigData zipData(const std::string& data);

private:
    static std::uint16_t getCRC(const std::uint8_t *data, const std::size_t length);

    static std::uint16_t updateCRC(std::uint16_t crc, const std::uint8_t input);
};

