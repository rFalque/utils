# C++ tools for 3D data

Toolbox of low level functions for 3D shape manipulation. The data structure is similar to the one used in libIGL (`Eigen::MatrixXd` for vertices and `Eigen::MatrixXi` for faces), but prioritise XYZ to be stored in the rows.

## Content
### IO
* read / write PLY files            (tinyply)
* read / write OFF files            (libigl)
* read / write OBJ files            (libigl)
* read / write STL files            (libigl)
* read / write CSV files            (stackoverflow)
* write PNG file                    (stb)
* return list of files in folder    (BOOST)

### Convertors
* IO to CGAL

### EigenTools
* wrapper for nanoflann
* remove cols /rows
* standardDeviation(Eigen::VectorXd)
* variance(Eigen::VectorXd)
* concatenate 2 matrices (sparse / dense)
* generate sparse diagonal matrix

### STDTools
* return the sign

### mesh
* compute the volume and surface area of a closed 3D mesh
* compute the normals from a mesh
* compute the faces centroids
* knn search on the manifold (greedySearch class)
* fill the holes (require CGAL)
* mesh downsampling (require CGAL)

### clouds
* centering to (0, 0, 0)
* cloud downsampling (skip points)
* Poisson surface reconstruction (require CGAL)

### visualization
* mesh / two meshes
* cloud
* correspondences

## Dependencies
Some of the algorithms require [CGAL](https://www.cgal.org/index.html) which can be installed as:
```bash
sudo apt-get install libcgal-dev
```

## To use
Clone the git repository and generate a minimalistic project:
```bash
git clone https://github.com/rFalque/3D_tools.git
cd 3D_tools
sudo chmod +x sample/setup.sh
./sample/setup.sh
```

In the CMakeLists.txt of the main project add
```bash
add_subdirectory(utils)

...

target_link_libraries(...
                      utils
                      ...
)
```

## formating
auto formatting is used with clang-format using:
```bash
find ./ -iname *.h -o -iname *.cpp | xargs clang-format -i -style=Google
```
