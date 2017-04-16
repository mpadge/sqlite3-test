#!/bin/bash

cmake -DCMAKE_CXX_COMPILER=clang ..
make
rm -r CMakeFiles
