git pull

rm build -rf
# remove CMakeLists.txt from sub sorce folder
for dir in $(ls src)
do
   echo 'delete CMakeLists.txt from '$SRC_FOLDER'/'$dir
   rm  $SRC_FOLDER/$dir/CMakeLists.txt
done

# clear the include and lib folder
rm include -rf
rm lib -rf
rm bin -rf

git add *
git commit
git push
