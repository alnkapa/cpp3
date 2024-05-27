#!/bin/bash

# mkdir -p build
#cd build
# rm -rf *
cmake .. -DPATCH_VERSION=3  -DBUILD_GMOCK=OFF -DINSTALL_GTEST=OFF -DCMAKE_INSTALL_PREFIX=~
cmake --build .
cmake --build . --target=install
cmake --build . --target=package
