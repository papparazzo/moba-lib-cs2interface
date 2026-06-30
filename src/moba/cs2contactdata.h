/*
 *  Project:    moba-lib-cs2interface
 *
 *  Copyright (C) 2026 Stefan Paproth <pappi-@gmx.de>
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

#include <string>

struct CS2ContactData {
    std::string host;
    int portIn;
    int portOut;

    friend std::ostream& operator<<(std::ostream& os, const CS2ContactData& data) {
        os << std::format("CS2ContactData: host<{}>, in<{}>, out<{}>", data.host, data.portIn, data.portOut);
        return os;
    }
};

