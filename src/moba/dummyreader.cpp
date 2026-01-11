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

#include "dummyreader.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "cs2shared.h"

DummyReader::DummyReader(const std::string &fifo) {
    // TODO: create fifo here!

    //    if(mkfifo())

    fd = open(fifo.c_str(), O_RDONLY);
    if(fd < 0) {
        throw CS2ConnectorException{"open returns < 0"};
    }
}

bool DummyReader::read(CS2CanCommand &data) const {
    if(::read(fd, &data, sizeof(data)) < 0) {
        throw CS2ConnectorException{"::read returns < 0"};
    }

    return true;
}