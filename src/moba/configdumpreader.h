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

#pragma once

#include "configreaderhandlerinterface.h"

#include <string>

class ConfigDumpReader final: public ConfigReaderHandlerInterface {

    std::string name;

public:
    explicit ConfigDumpReader(std::string name);

    ~ConfigDumpReader() noexcept override = default;

    [[nodiscard]] std::string getName() const override;

    void handleConfigData(const std::string &data) override;
};
