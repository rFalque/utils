#ifndef POISSON_SURFACE_RECONSTRUCTION_H
#define POISSON_SURFACE_RECONSTRUCTION_H

#include <Eigen/Core>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/poisson_surface_reconstruction.h>

typedef std::pair<CGAL_Poly_Kernel::Point_3, CGAL_Poly_Kernel::Vector_3> Pwn;

inline void poisson_surface_reconstruction(Eigen::MatrixXd& V, Eigen::MatrixXd& N, Eigen::MatrixXd& V_out, Eigen::MatrixXi& F_out) {
    int x=0, y=1, z=2;  // used out of convenience

    // transform into CGAL format:
    std::cout << "convert to CGAL format\n";
    std::vector<Pwn> points;
    for (int i=0; i<V.cols(); i++) {
    CGAL_Poly_Kernel::Point_3 v(V.col(i)(x), V.col(i)(y), V.col(i)(z));
    CGAL_Poly_Kernel::Vector_3 n(N.col(i)(x), N.col(i)(y), N.col(i)(z));
    points.push_back(std::make_pair(v, n));
    }

    CGAL_Poly output_mesh;
    double average_spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag>
                             (points, 10, CGAL::parameters::point_map(CGAL::First_of_pair_property_map<Pwn>()));
    std::cout << "Poisson reconstruction\n";
    CGAL::poisson_surface_reconstruction_delaunay(points.begin(), points.end(),
                                                 CGAL::First_of_pair_property_map<Pwn>(),
                                                 CGAL::Second_of_pair_property_map<Pwn>(),
                                                 output_mesh, average_spacing/5);

    // transform back to own format
    std::cout << "convert back to normal format\n";

    convert_mesh_from_CGAL_poly_to_IGL(output_mesh, V_out, F_out);
};

#endif
  
