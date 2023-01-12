#! /usr/bin/env bash

(
    mkdir build
    cd build
    cmake -G'MSYS Makefiles' -D CMAKE_C_COMPILER=/mingw64/bin/gcc.exe -D CMAKE_CXX_COMPILER=/mingw64/bin/g++.exe ..
    make
)