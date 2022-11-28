#include "mesh/nonRigidDeformation.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/property_map.h>
#include <CGAL/Surface_mesh_deformation.h>

#include <vector>
#include <Eigen/Dense>
#include <iostream>
#include <stdexcept>
#include <limits>


// dumb exhaustive search, should replace with kdtree but got no time
int find_closest_vertex(CGAL_Poly & CGAL_mesh, Eigen::Vector3d point) {
    double point_distance = std::numeric_limits<double>::infinity();
    int closest_vertex_index = -1;

    int counter = 0;
    for (auto it = CGAL_mesh.vertices_begin(); it != CGAL_mesh.vertices_end(); ++it) {
        Eigen::Vector3d vertex;
        vertex(0) = it->point()[0];
        vertex(1) = it->point()[1];
        vertex(2) = it->point()[2];
        
        if ( (vertex-point).norm() < point_distance ) {
            point_distance = (vertex-point).norm();
            closest_vertex_index = counter;
        }

        ++counter;
    }

    return closest_vertex_index;
};

typedef CGAL::Simple_cartesian<double>                                   Kernel;
typedef CGAL::Polyhedron_3<Kernel>                                   Polyhedron;
typedef boost::graph_traits<Polyhedron>::vertex_descriptor    vertex_descriptor;
typedef boost::graph_traits<Polyhedron>::vertex_iterator        vertex_iterator;
typedef boost::graph_traits<Polyhedron>::halfedge_descriptor halfedge_descriptor;
typedef boost::graph_traits<Polyhedron>::halfedge_iterator    halfedge_iterator;

// Define the maps
typedef std::map<vertex_descriptor, std::size_t>                  Vertex_id_map;
typedef std::map<halfedge_descriptor, std::size_t>                 Hedge_id_map;
typedef boost::associative_property_map<Vertex_id_map>           Vertex_id_pmap;
typedef boost::associative_property_map<Hedge_id_map>             Hedge_id_pmap;

typedef CGAL::Surface_mesh_deformation<Polyhedron, Vertex_id_pmap, Hedge_id_pmap> Surface_mesh_deformation;

void deform(CGAL_Poly & CGAL_mesh, std::vector<int> handles_ids, std::vector<Eigen::Vector3d> handles_target) {

  // Init the indices of the vertices from 0 to num_vertices(mesh)-1
  Vertex_id_map vertex_index_map;
  std::size_t counter = 0;
  for(vertex_descriptor v : vertices(CGAL_mesh))
    vertex_index_map[v]=counter++;
  // Init the indices of the halfedges from 0 to 2*num_edges(mesh)-1
  Hedge_id_map hedge_index_map;
  counter = 0;
  for(halfedge_descriptor h : halfedges(CGAL_mesh))
    hedge_index_map[h]=counter++;
  Surface_mesh_deformation deform_mesh( CGAL_mesh,
                                        Vertex_id_pmap(vertex_index_map),
                                        Hedge_id_pmap(hedge_index_map) );

  // Definition of the region of interest (use the whole mesh)
  vertex_iterator vb,ve;
  boost::tie(vb, ve) = vertices(CGAL_mesh);
  deform_mesh.insert_roi_vertices(vb, ve);

  // Select the control vertices ...
  std::vector<vertex_descriptor> control_handles;
  for (auto handle : handles_ids) {
    control_handles.push_back(*std::next(vb, handle));
  }  

  // .. and insert them
  for (auto control_handle : control_handles) {
    deform_mesh.insert_control_vertex(control_handle);
  }
  
  // The definition of the ROI and the control vertices is done, call preprocess
  bool is_matrix_factorization_OK = deform_mesh.preprocess();
  if(!is_matrix_factorization_OK){
    throw std::runtime_error("Error in preprocessing, check documentation of preprocess()");
  }
  
  for (int i=0; i<handles_target.size(); ++i) {
    Surface_mesh_deformation::Point constrained_position(handles_target[i](0), handles_target[i](1), handles_target[i](2));
    deform_mesh.set_target_position(control_handles[i], constrained_position);
  }

  // Deform the mesh, the positions of vertices of 'mesh' are updated
  deform_mesh.deform(20, 0.0);
};

// built upon the sample codes from https://doc.cgal.org/latest/Surface_mesh_deformation/index.html
void deform(Eigen::MatrixXd& V, Eigen::MatrixXi& F, std::vector<Eigen::Vector3d> handle_sources, std::vector<Eigen::Vector3d> handles_target) {
  
  // convert to CGAL format
  CGAL_Poly CGAL_mesh;
  convert_mesh_from_IGL_to_CGAL_Polygon(V, F, CGAL_mesh);

  std::vector<int> handles_ids;
  for (auto handle_source : handle_sources) {
      handles_ids.push_back(find_closest_vertex(CGAL_mesh, handle_source));
  }

  // Init the indices of the vertices from 0 to num_vertices(mesh)-1
  Vertex_id_map vertex_index_map;
  std::size_t counter = 0;
  for(vertex_descriptor v : vertices(CGAL_mesh))
    vertex_index_map[v]=counter++;
  // Init the indices of the halfedges from 0 to 2*num_edges(mesh)-1
  Hedge_id_map hedge_index_map;
  counter = 0;
  for(halfedge_descriptor h : halfedges(CGAL_mesh))
    hedge_index_map[h]=counter++;
  Surface_mesh_deformation deform_mesh( CGAL_mesh,
                                        Vertex_id_pmap(vertex_index_map),
                                        Hedge_id_pmap(hedge_index_map) );

  // Definition of the region of interest (use the whole mesh)
  vertex_iterator vb,ve;
  boost::tie(vb, ve) = vertices(CGAL_mesh);
  deform_mesh.insert_roi_vertices(vb, ve);

  // Select the control vertices ...
  std::vector<vertex_descriptor> control_handles;
  for (auto handle : handles_ids) {
    control_handles.push_back(*std::next(vb, handle));
  }  

  // .. and insert them
  for (auto control_handle : control_handles) {
    deform_mesh.insert_control_vertex(control_handle);
  }
  
  // The definition of the ROI and the control vertices is done, call preprocess
  bool is_matrix_factorization_OK = deform_mesh.preprocess();
  if(!is_matrix_factorization_OK){
    throw std::runtime_error("Error in preprocessing, check documentation of preprocess()");
  }
  
  for (int i=0; i<handles_target.size(); ++i) {
    Surface_mesh_deformation::Point constrained_position(handles_target[i](0), handles_target[i](1), handles_target[i](2));
    deform_mesh.set_target_position(control_handles[i], constrained_position);
  }

  // Deform the mesh, the positions of vertices of 'mesh' are updated
  deform_mesh.deform(20, 0.0);
  
  // get back into eigen format
  convert_mesh_from_CGAL_poly_to_IGL(CGAL_mesh, V, F);
};

