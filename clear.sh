SRC_FOLDER=src
SUB_CMAKE_DIR=config/subCmake/CMakeLists.txt

# clear CMakeLists.txt from sub sorce folder
for dir in $(ls src)
do
   echo 'delete CMakeLists.txt from '$SRC_FOLDER'/'$dir
   rm $SRC_FOLDER/$dir/CMakeLists.txt
done

# clear the include and lib folder
echo remove build/
rm build -rf
echo remove includ/
rm include -rf
echo remove lib/
rm lib -rf
echo remove bin/
rm bin -rf

