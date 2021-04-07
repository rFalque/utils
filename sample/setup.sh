cp -r sample/* ../
cd ..
mv 3D_tools/ utils/
git clone --recurse-submodules https://github.com/nmwsharp/polyscope.git
mkdir build
cd build
cmake ..
make -j3
