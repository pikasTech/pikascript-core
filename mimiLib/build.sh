#!/bin/bash

for dir in $(ls src)
do
        echo 'copy CMakeLists.txt to src/'$dir
        cp config/subCmake/CMakeLists.txt src/$dir/CMakeLists.txt
done

if [ ! -d build ];then
	mkdir build
else
   echo dir build exist
fi

rm include -rf

cd build && cmake .. && make && cd .. && rm build -rf 

for dir in $(ls src)
do
   echo 'delete CMakeLists.txt from src/'$dir
   rm  src/$dir/CMakeLists.txt
done
