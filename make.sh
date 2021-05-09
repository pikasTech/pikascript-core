#!/bin/bash


# copy sub CMakeLists.txt to sub sorce folder
SRC_FOLDER=src/kernal
SUB_CMAKE_DIR=setup/subCmake/CMakeLists.txt
for dir in $(ls $SRC_FOLDER)
do
        echo 'copy '$SUB_CMAKE_DIR' to '$SRC_FOLDER'/'$dir
        cp $SUB_CMAKE_DIR $SRC_FOLDER/$dir/CMakeLists.txt
done

# copy sub CMakeLists.txt to sub sorce folder
SRC_FOLDER=src/init
SUB_CMAKE_DIR=setup/subCmake_bin/CMakeLists.txt
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

# clear the include and lib folder
rm include -rf
# rm lib -rf
rm bin -rf

# build project
cd build && cmake .. && make && cd .. 

