#!/bin/bash

SRC_FOLDER=src
SUB_CMAKE_DIR=config/subCmake/CMakeLists.txt

# copy sub CMakeLists.txt to sub sorce folder
for dir in $(ls $SRC_FOLDER)
do
        echo 'copy '$SUB_CMAKE_DIR' to '$SRC_FOLDER'/'$dir
        cp $SUB_CMAKE_DIR $SRC_FOLDER/$dir/CMakeLists.txt
done

# make dir build to build project
if [ ! -d build ];then
	mkdir build
else
   echo dir build exist
fi


# build project and remove build folder
cd build && cmake .. && make && cd .. && rm build -rf 

# remove CMakeLists.txt from sub sorce folder
for dir in $(ls src)
do
   echo 'delete CMakeLists.txt from '$SRC_FOLDER'/'$dir
   rm  $SRC_FOLDER/$dir/CMakeLists.txt
done
