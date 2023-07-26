/*
 *  Project:    moba-lib-cs2interface
 *
 *  Copyright (C) 2023 Stefan Paproth <pappi-@gmx.de>
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

class CanCommandException: public std::exception {
    
    std::string what_;
    
public:
    ~CanCommandException() noexcept override = default;

    explicit CanCommandException(const std::string &what) {
        this->what_ = what;
    }

    [[nodiscard]] const char* what() const noexcept override {
        return this->what_.c_str();
    }
};