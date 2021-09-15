#include "convertors/igl_to_cgal.h"

#include <CGAL/boost/graph/copy_face_graph.h>

#include <vector>

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

void convert_mesh_from_IGL_to_CGAL_Polygon(const Eigen::MatrixXd& V,
                                           const Eigen::MatrixXi& F,
                                           CGAL_Poly& CGAL_mesh) {
  CGAL_Mesh CGAL_mesh_temp;
  convert_mesh_from_IGL_to_CGAL(V, F, CGAL_mesh_temp);
  copy_face_graph(CGAL_mesh_temp, CGAL_mesh);
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

void convert_mesh_from_CGAL_poly_to_IGL(const CGAL_Poly& CGAL_mesh,
                                        Eigen::MatrixXd& V,
                                        Eigen::MatrixXi& F) {
  CGAL_Mesh CGAL_mesh_temp;
  copy_face_graph(CGAL_mesh, CGAL_mesh_temp);
  convert_mesh_from_CGAL_to_IGL(CGAL_mesh_temp, V, F);
}