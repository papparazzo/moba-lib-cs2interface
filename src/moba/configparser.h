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

#include <exception>
#include <string>

class ConfigParserException : public std::exception {
public:
    virtual ~ConfigParserException() noexcept {
    }

    ConfigParserException(const std::string &what) {
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
};