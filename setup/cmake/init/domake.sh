#!/bin/bash
PROJECT=init
BUILD_DIR=$PROJECT
SRC_DIR=$PROJECT

# copy sub CMakeLists.txt to sub sorce folder
SRC_FOLDER=src/$SRC_DIR
SUB_CMAKE_DIR=setup/cmake/$PROJECT/sub/CMakeLists.txt
TOP_CMAKE_DIR=setup/cmake/$PROJECT/top/CMakeLists.txt
for dir in $(ls $SRC_FOLDER)
do
        echo 'copy '$SUB_CMAKE_DIR' to '$SRC_FOLDER'/'$dir
        cp $SUB_CMAKE_DIR $SRC_FOLDER/$dir/CMakeLists.txt
done
# copy top CMakeLists.txt to top sorce folder
echo copy $TOP_CMAKE_DIR to $PWD
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

# clear the bin older
echo remove $PWD/bin
rm bin -rf

# build project
cd build/$BUILD_DIR && cmake ../../ && make && cd .. 
