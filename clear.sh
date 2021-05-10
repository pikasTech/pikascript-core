#!/bin/bash
SRC_FOLDER=src

# clear CMakeLists.txt from sub sorce folder
find src -name "*CMakeLists.txt" | xargs rm
rm CMakeLists.txt

# clear the include and lib folder
echo remove build/
rm build -rf
echo remove includ/
rm include -rf
echo remove lib/
rm lib -rf
echo remove bin/
rm bin -rf

