#include "convertors/igl_to_cgal.h"

#include <CGAL/boost/graph/copy_face_graph.h>

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>

#include <vector>

#include <iostream>

void convert_mesh_from_IGL_to_CGAL(const Eigen::MatrixXd& V,
                                   const Eigen::MatrixXi& F,
                                   CGAL_Mesh& CGAL_mesh) {
  // clear and reserve the
  CGAL_mesh.clear();
  int n = V.cols();
  int f = F.cols();
  int e = 0;
  CGAL_mesh.reserve(n, 2 * f, e);

  // copy the vertices
  double x, y, z;
  for (int i = 0; i < n; i++) {
    Point p;
    x = V(0, i);
    y = V(1, i);
    z = V(2, i);
    p = Point(x, y, z);
    // CGAL_mesh.add_vertex( Point(V(i,0), V(i,1), V(i,2)) );
    CGAL_mesh.add_vertex(p);
  }

  // copy the faces
  std::vector<int> vertices;
  for (int i = 0; i < f; i++) {
    vertices.resize(3);
    vertices[0] = F(0, i);
    vertices[1] = F(1, i);
    vertices[2] = F(2, i);
    CGAL_mesh.add_face(CGAL_Mesh::Vertex_index(vertices[0]),
                       CGAL_Mesh::Vertex_index(vertices[1]),
                       CGAL_Mesh::Vertex_index(vertices[2]));
  }
}

void convert_mesh_from_CGAL_to_IGL(const CGAL_Mesh& CGAL_mesh,
                                   Eigen::MatrixXd& V, Eigen::MatrixXi& F) {
  V.resize(3, CGAL_mesh.number_of_vertices());
  F.resize(3, CGAL_mesh.number_of_faces());

  int i = 0;
  for (auto v : CGAL_mesh.vertices()) {
    V.col(i) << CGAL_mesh.point(v).x(), CGAL_mesh.point(v).y(),
        CGAL_mesh.point(v).z();
    i++;
  }

  i = 0;
  for (auto f : CGAL_mesh.faces()) {
    /*
    int j=0;
    for (auto vd : vertices_around_face(CGAL_mesh.halfedge(f), CGAL_mesh)){
        F(j, i) = vd;
        j++;
    }
    i++;
    */

    auto he = halfedge(f, CGAL_mesh);
    F(0, i) = source(he, CGAL_mesh);
    F(1, i) = target(he, CGAL_mesh);
    F(2, i) = target(next(he, CGAL_mesh), CGAL_mesh);
    i++;
  }
}

// See discussion at https://github.com/CGAL/cgal/issues/7209
// the code from https://github.com/libigl/libigl/blob/main/include/igl/copyleft/cgal/polyhedron_to_mesh.cpp
// would sometimes create an empty mesh
void convert_mesh_from_IGL_to_CGAL_Polygon(const Eigen::MatrixXd& V,
                                           const Eigen::MatrixXi& F,
                                           CGAL_Poly& CGAL_mesh) {
  

  std::vector<Point> vertices;
  std::vector<std::vector<std::size_t>> faces;
  
  for (int i = 0; i < V.cols(); i++) {
    Point p;
    p = Point(V(0, i), V(1, i), V(2, i));
    vertices.push_back(p);
  }

  for (int i = 0; i < F.cols(); i++) {
    std::vector<std::size_t> face;
    face.push_back(F(0, i));
    face.push_back(F(1, i));
    face.push_back(F(2, i));
    faces.push_back(face);
  }

  CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(vertices, faces, CGAL_mesh);

  // print a warning if the number of vertices is not the same
  if (CGAL_mesh.size_of_vertices() != V.cols()) {
    std::cout << "WARNING: number of vertices in CGAL mesh is different from "
                 "number of vertices in the Eigen matrix"
              << std::endl;
  }

  // print a warning if the number of faces is not the same
  if (CGAL_mesh.size_of_facets() != F.cols()) {
    std::cout << "WARNING: number of faces in CGAL mesh is different from "
                 "number of faces in IGL Eigen matrix"
              << std::endl;
  }

}

// code from https://github.com/libigl/libigl/blob/main/include/igl/copyleft/cgal/polyhedron_to_mesh.cpp
void convert_mesh_from_CGAL_poly_to_IGL(const CGAL_Poly& poly,
                                        Eigen::MatrixXd& V,
                                        Eigen::MatrixXi& F) {

  V.resize(3, poly.size_of_vertices());
  F.resize(3, poly.size_of_facets());

  typedef typename Polyhedron::Vertex_const_iterator Vertex_iterator;
  std::map<Vertex_iterator,size_t> vertex_to_index;
  {
    size_t v = 0;
    for(
      typename Polyhedron::Vertex_const_iterator p = poly.vertices_begin();
      p != poly.vertices_end();
      p++)
    {      
      V(0,v) = p->point().x();
      V(1,v) = p->point().y();
      V(2,v) = p->point().z();
      
      vertex_to_index[p] = v;
      v++;
    }
  }
  {
    size_t f = 0;
    for(
      typename Polyhedron::Facet_const_iterator facet = poly.facets_begin();
      facet != poly.facets_end();
      ++facet)
    {
      typename Polyhedron::Halfedge_around_facet_const_circulator he = 
        facet->facet_begin();
      // Facets in polyhedral surfaces are at least triangles.
      assert(CGAL::circulator_size(he) == 3 && "Facets should be triangles");
      size_t c = 0;
      do {
        //// This is stooopidly slow
        // F(f,c) = std::distance(poly.vertices_begin(), he->vertex());
        F(c,f) = vertex_to_index[he->vertex()];
        c++;
      } while ( ++he != facet->facet_begin());
      f++;
    }
  }
}
