#!/bin/bash
BUILD_DIR=package

# copy sub CMakeLists.txt to sub sorce folder
SRC_FOLDER=src/$BUILD_DIR
SUB_CMAKE_DIR=setup/cmake/$BUILD_DIR/sub/CMakeLists.txt
TOP_CMAKE_DIR=setup/cmake/$BUILD_DIR/top/CMakeLists.txt
for dir in $(ls $SRC_FOLDER)
do
        echo 'copy '$SUB_CMAKE_DIR' to '$SRC_FOLDER'/'$dir
        cp $SUB_CMAKE_DIR $SRC_FOLDER/$dir/CMakeLists.txt
done
# copy top CMakeLists.txt to top sorce folder
cp $TOP_CMAKE_DIR .

# make dir build to build project
if [ ! -d build ];then
	mkdir build
else
   echo dir build exist
fi

# make dir build to build project
if [ ! -d build/$BUILD_DIR ];then
	mkdir build/$BUILD_DIR
else
   echo dir build exist
fi


# clear the include and lib folder
echo remove $PWD/include
rm include -rf
echo remove $PWD/lib
rm lib -rf

# build project
cd build/$BUILD_DIR && cmake ../../ && make && cd .. 
