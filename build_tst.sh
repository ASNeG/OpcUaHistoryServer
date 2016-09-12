# create build directory and move into it
rm -rf build_tst
mkdir -p build_tst
cd build_tst

# configure cmake
cmake ../tst 

#build 
make
