/*
 *  Project:    moba-connector
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

#include "configreaderhandlerinterface.h"
#include "configreaderexception.h"
#include <map>

class ConfigLoklistReader: public ConfigReaderHandlerInterface {
public:

    struct Locomotive {
        std::string name;
        std::map<std::uint32_t, std::uint32_t> functions;
    };

    std::map<std::uint32_t, std::shared_ptr<Locomotive>> locomotives;

    std::string getName() {
        return "lokomotive";
    }

    virtual void handleConfigData(const std::string &data);

protected:
    std::string getToken(const std::string &t, std::string &v);
};

using Locomotives = std::map<std::uint32_t, std::shared_ptr<ConfigLoklistReader::Locomotive>>;
using LocomotivesPtr = std::shared_ptr<Locomotives>;