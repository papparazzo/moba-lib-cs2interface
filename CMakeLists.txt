cmake_minimum_required(VERSION 3.20)

project(moba-cs2interface VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

configure_file(config.h.in config.h)

include(GNUInstallDirs)
add_library(
    moba-cs2interface STATIC 

    src/moba/printcancommand.cpp
    src/moba/configloklistreader.cpp
    src/moba/configdumpreader.cpp
    src/moba/configreader.cpp
    src/moba/cs2reader.cpp
    src/moba/cs2utils.cpp
    src/moba/cs2writer.cpp
)

install(TARGETS moba-cs2interface)

target_include_directories(moba-cs2interface PUBLIC "${PROJECT_BINARY_DIR}")
