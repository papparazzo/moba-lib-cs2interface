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
 *  along with this program. If not, see <http://www.gnu.org/licenses/agpl.txt>.
 *
 */

#include "configloklistreader.h"

void ConfigLoklistReader::handleConfigData(const std::string &data) {
    locomotives.clear();

    std::string::size_type p = 0;
    std::string::size_type f = 0;

    std::string n;
    std::string v;

    std::string name;

    std::shared_ptr<Locomotive> curLoco;

    while((f = data.find('\n', p)) != std::string::npos) {
        auto t = data.substr(p, f - p);
        p = f + 1;

        auto k = getToken(t, v);

        if(k == " .minor" && v != "4") {
            throw ConfigReaderException{"minor version != 4"};
        } else if(k == " .name") {
            name = v;
        } else if(k == " .uid") {
            curLoco = std::make_shared<Locomotive>();
            curLoco->name = name;
            locomotives[std::stoul(v, nullptr, 16)] = curLoco;
        } else if(k == " ..nr") {
            n = v;
        } else if(k == " ..typ") {
            curLoco->functions[std::stoi(v)] = std::stoi(n);
        }
    };
}

std::string ConfigLoklistReader::getToken(const std::string &t, std::string &v) {
    auto p = t.find('=');

    if(p == std::string::npos) {
        v = "";
        return t;
    }
    v = t.substr(p + 1);
    return t.substr(0, p);
}
