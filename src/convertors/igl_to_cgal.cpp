#include "convertors/igl_to_cgal.h"

#include <CGAL/boost/graph/copy_face_graph.h>

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

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

void convert_mesh_from_IGL_to_CGAL_Polygon(const Eigen::MatrixXd& V,
                                           const Eigen::MatrixXi& F,
                                           CGAL_Poly& CGAL_mesh) {
  /*
  CGAL_Mesh CGAL_mesh_temp;
  convert_mesh_from_IGL_to_CGAL(V, F, CGAL_mesh_temp);
  copy_face_graph(CGAL_mesh_temp, CGAL_mesh);
  */

  //std::cout << "entry point to convert_mesh_from_IGL_to_CGAL_Polygon" << std::endl;

  typedef typename Polyhedron::HalfedgeDS HalfedgeDS;
  CGAL::Polyhedron_incremental_builder_3<HalfedgeDS> B(CGAL_mesh.hds());
  B.begin_surface(V.cols(),F.cols());
  typedef typename HalfedgeDS::Vertex   Vertex;
  typedef typename Vertex::Point Point;
  assert(V.rows() == 3 && "V must be 3 by #V");
  for(int v = 0;v<V.cols();v++)
  {
    B.add_vertex(Point(V(0,v),V(1,v),V(2,v)));
    //std::cout << "add point: " << V.col(v).transpose() << std::endl;
  }

  assert(F.rows() == 3 && "F must be #F by 3");
  for(int f=0;f<F.cols();f++)
  {
    B.begin_facet();
    for(int c = 0;c<3;c++)
    {
      B.add_vertex_to_facet(F(c,f));
    }
    B.end_facet();
  }
  if(B.error())
  {
    B.rollback();
  }
  B.end_surface();
}


void convert_mesh_from_CGAL_poly_to_IGL(const CGAL_Poly& poly,
                                        Eigen::MatrixXd& V,
                                        Eigen::MatrixXi& F) {
  /*
  CGAL_Mesh CGAL_mesh_temp;
  copy_face_graph(CGAL_mesh, CGAL_mesh_temp);
  convert_mesh_from_CGAL_to_IGL(CGAL_mesh_temp, V, F);
  */

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
      //assign_scalar(p->point().x(),V(0,v));
      //assign_scalar(p->point().y(),V(1,v));
      //assign_scalar(p->point().z(),V(2,v));
      
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











/*
#include "mesh_to_polyhedron.h"


template <
  typename DerivedV,
  typename DerivedF,
  typename Polyhedron>
IGL_INLINE bool igl::copyleft::cgal::mesh_to_polyhedron(
  const Eigen::MatrixBase<DerivedV>& V, 
  const Eigen::MatrixBase<DerivedF>& F,
  Polyhedron& poly) 
{
  typedef typename Polyhedron::HalfedgeDS HalfedgeDS;
  // Postcondition: hds is a valid polyhedral surface.
  CGAL::Polyhedron_incremental_builder_3<HalfedgeDS> B(poly.hds());
  B.begin_surface(V.rows(),F.rows());


  typedef typename HalfedgeDS::Vertex   Vertex;
  typedef typename Vertex::Point Point;
  assert(V.cols() == 3 && "V must be #V by 3");
  
  for(int v = 0;v<V.rows();v++)
  {
    B.add_vertex(Point(V(v,0),V(v,1),V(v,2)));
  }
  
  assert(F.cols() == 3 && "F must be #F by 3");
  for(int f=0;f<F.rows();f++)
  {
    B.begin_facet();
    for(int c = 0;c<3;c++)
    {
      B.add_vertex_to_facet(F(f,c));
    }
    B.end_facet();
  }
  if(B.error())
  {
    B.rollback();
    return false;
  }
  B.end_surface();
  return poly.is_valid();

}



// This file is part of libigl, a simple c++ geometry processing library.
// 
// Copyright (C) 2015 Alec Jacobson <alecjacobson@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#include "polyhedron_to_mesh.h"
#include <CGAL/Polyhedron_3.h>
#include "assign_scalar.h"

template <
  typename Polyhedron,
  typename DerivedV,
  typename DerivedF>
IGL_INLINE void igl::copyleft::cgal::polyhedron_to_mesh(
  const Polyhedron & poly,
  Eigen::PlainObjectBase<DerivedV> & V,
  Eigen::PlainObjectBase<DerivedF> & F)
{
  using namespace std;
  V.resize(poly.size_of_vertices(),3);
  F.resize(poly.size_of_facets(),3);


  typedef typename Polyhedron::Vertex_const_iterator Vertex_iterator;
  std::map<Vertex_iterator,size_t> vertex_to_index;
  {
    size_t v = 0;
    for(
      typename Polyhedron::Vertex_const_iterator p = poly.vertices_begin();
      p != poly.vertices_end();
      p++)
    {
      assign_scalar(p->point().x(),V(v,0));
      assign_scalar(p->point().y(),V(v,1));
      assign_scalar(p->point().z(),V(v,2));
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
        F(f,c) = vertex_to_index[he->vertex()];
        c++;
      } while ( ++he != facet->facet_begin());
      f++;
    }
  }
}

*/
