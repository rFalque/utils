cp -r sample/!(setup.sh) ../
cd ..
git clone --depth 1 --recurse-submodules https://github.com/nmwsharp/polyscope.git
mkdir build
cd build
cmake ..
make -j3
