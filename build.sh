cmake -S ./ -B bin/
cd bin/
make
rm -rf plugins
mkdir plugins
cp ./libtmexp.dylib plugins/ 
rm ./libtmexp.dylib