# Project Title

3D tools for c++11


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
* compute_normals_from_mesh
* greedySearch (knn search on the manifold)

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
