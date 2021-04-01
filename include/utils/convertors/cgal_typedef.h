#ifndef CGAL_TYPEDEF
#define CGAL_TYPEDEF
/*
*   by R. Falque
*   06/01/2020
*/

#include <CGAL/Surface_mesh.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Mean_curvature_flow_skeletonization.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>

typedef CGAL::Simple_cartesian<double>							Kernel;
typedef Kernel::Point_3											Point;
typedef CGAL::Surface_mesh<Point>								CGAL_Mesh;
typedef CGAL_Mesh::Vertex_index               					CGAL_vertex;
typedef CGAL_Mesh::Face_index                 					CGAL_face;
typedef CGAL_Mesh::Vertex_iterator                              CGAL_vertex_iterator;

typedef CGAL::Exact_predicates_inexact_constructions_kernel     CGAL_Poly_Kernel;
typedef CGAL::Polyhedron_3<Kernel>                              CGAL_Poly;
typedef CGAL_Poly::Halfedge_handle                              CGAL_Poly_Halfedge_handle;
typedef CGAL_Poly::Facet_handle                                 CGAL_Poly_Facet_handle;
typedef CGAL_Poly::Vertex_handle                                CGAL_Poly_Vertex_handle;

typedef CGAL::Mean_curvature_flow_skeletonization<CGAL_Mesh>	Skeletonization;
typedef Skeletonization::Skeleton								CGAL_Skeleton;
typedef CGAL_Skeleton::vertex_descriptor						CGAL_Skeleton_vertex;
typedef CGAL_Skeleton::edge_descriptor							CGAL_Skeleton_edge;

#endif