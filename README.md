# C++ tools for 3D data

Mixture of header only and compilable functions (the aim is to get everything as a clean compilable library at some stage).

## Content
### IO
* read / write PLY files            (tinyply)
* read / write OFF files            (libigl)
* read / write OBJ files            (libigl)
* read / write STL files            (libigl)
* read / write CSV files            (stackoverflow)
* write PNG file                    (stb)
* return list of files in folder    (BOOST)

### EigenTools
* wrapper for nanoflann
* remove cols /rows
* standardDeviation(Eigen::VectorXd)
* variance(Eigen::VectorXd)
* concatenate 2 matrices (sparse / dense)
* generate sparse diagonal matrix

### mesh
* compute the volume and surface area of a closed 3D mesh
* compute the normals from a mesh
* compute the faces centroids
* knn search on the manifold (greedySearch class)

### clouds
* centering to (0, 0, 0)

### visualization
* mesh / two meshes
* cloud
* correspondences

## To use
In the main CMakeLists.txt add
```bash
add_subdirectory(utils)

...

target_link_libraries(...
                      utils
                      ...
)
```
