#!/usr/bin/bash
cmake ..
cp test test.bak
rm test
make
./test

