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

#include "locomotivefunctions.h"

void LocomotiveFunctions::clear() {
    std::lock_guard l{m};
    locomotives.clear();
}

std::string LocomotiveFunctions::getName(const std::uint32_t localId) {
    std::lock_guard l{m};

    const auto iter = locomotives.find(localId);
    if(iter == locomotives.end()) {
        throw std::runtime_error("given localId <" + std::to_string(localId) + "> does not exist");
    }

    return iter->second->name;
}

std::uint32_t LocomotiveFunctions::getFunction(const std::uint32_t localId, const std::uint32_t functionId) {
    std::lock_guard l{m};

    const auto iter = locomotives.find(localId);
    if(iter == locomotives.end()) {
        throw std::runtime_error("given localId <" + std::to_string(localId) + "> does not exist");
    }

    auto &func = iter->second->functions;

    // TODO: Try alternative functions...
    const auto funcIter = func.find(functionId);

    if(funcIter == func.end()) {
        return 0;
    }

    return funcIter->second;
}

LocomotivePtr LocomotiveFunctions::getFunctions(const std::uint32_t localId) {
    std::lock_guard l{m};

    const auto iter = locomotives.find(localId);
    if(iter == locomotives.end()) {
        throw std::runtime_error("given localId <" + std::to_string(localId) + "> does not exist");
    }
    return iter->second;
}

void LocomotiveFunctions::addLocomotive(const std::uint32_t localId, LocomotivePtr locomotive) {
    std::lock_guard l{m};
    locomotives[localId] = std::move(locomotive);
}