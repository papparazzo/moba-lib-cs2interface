/*
 *  Project:    moba-lib-cs2interface
 *
 *  Copyright (C) 2022 Stefan Paproth <pappi-@gmx.de>
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

#include "configdumpreader.h"

#include <iostream>
#include <utility>

ConfigDumpReader::ConfigDumpReader(std::string name): name{std::move(name)} {
}

std::string ConfigDumpReader::getName() const {
    return name;
}

void ConfigDumpReader::handleConfigData(const std::string &data) {
    std::cout << data << std::endl;
}