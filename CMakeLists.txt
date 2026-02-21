cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(moba-lib-cs2interface VERSION 0.1.0)

configure_file(config.h.in config.h)

include(GNUInstallDirs)
add_library(
    moba-lib-cs2interface STATIC

    src/moba/printcancommand.cpp
    src/moba/configloklistreader.cpp
    src/moba/configdumpreader.cpp
    src/moba/configreader.cpp
    src/moba/configwriter.cpp
    src/moba/cs2reader.cpp
    src/moba/cs2utils.cpp
    src/moba/cs2writer.cpp
    src/moba/locomotivefunctions.cpp
    src/moba/dummyreader.cpp
    src/moba/dummywriter.cpp
    src/moba/configdatacompressor.cpp
)

install(TARGETS moba-lib-cs2interface)

target_include_directories(moba-lib-cs2interface PUBLIC "${PROJECT_BINARY_DIR}")
