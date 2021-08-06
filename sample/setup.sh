cp -r sample/!(setup.sh) ../
cd ..
mv 3D_tools/ utils/
git clone --depth 1 --recurse-submodules https://github.com/nmwsharp/polyscope.git
mkdir build
cd build
cmake ..
make -j3
