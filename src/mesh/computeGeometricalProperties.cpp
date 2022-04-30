/*
 *   compute the surface area and volume using the faces
 *   by R. Falque
 *   16/01/2020
 */
#include "mesh/computeGeometricalProperties.h"

#include <Eigen/Dense>
#include <iostream>

#include "mesh/computeNormals.h"

double compute_volume(Eigen::MatrixXd V, Eigen::MatrixXi F, Eigen::MatrixXd N) {
  if (V.rows() != 3 || F.rows() != 3 || N.rows() != 3)
    throw std::invalid_argument(
        "Error in " + std::string(__func__) + ": wrong input size");

  double mesh_volume = 0;

  for (int i = 0; i < F.cols(); i++) {
    // get the points of the triangle
    Eigen::Vector3d v1 = V.col(F.col(i)(0));
    Eigen::Vector3d v2 = V.col(F.col(i)(1));
    Eigen::Vector3d v3 = V.col(F.col(i)(2));

    // get area of the face with the cross product between the two vectors of
    // the triangle
    double surface_area_temp = (v3 - v1).cross(v2 - v1).norm() / 2;

    // get the signed height of the tetrahedron formed with (v1, v2, v3,
    // (0,0,0))
    Eigen::Vector3d n = N.col(i);
    Eigen::Vector3d centroid = (v1 + v2 + v3) / 3;
    double dot_product = n.dot(centroid);

    // sums things up
    mesh_volume += surface_area_temp * dot_product / 3;
  }

  return mesh_volume;
};

double compute_volume(Eigen::MatrixXd V, Eigen::MatrixXi F) {
  if (V.rows() != 3 || F.rows() != 3)
    throw std::invalid_argument(
        "Error in " + std::string(__func__) + ": wrong input size");

  Eigen::MatrixXd N = compute_faces_normals(V, F);
  return compute_volume(V, F, N);
};

double compute_surface_area(Eigen::MatrixXd V, Eigen::MatrixXi F) {
  if (V.rows() != 3 || F.rows() != 3)
    throw std::invalid_argument(
        "Error in " + std::string(__func__) + ": wrong input size");

  double surface_area = 0;

  for (int i = 0; i < F.cols(); i++) {
    // vector A & B
    Eigen::Vector3d v1 = V.col(F.col(i)(0));
    Eigen::Vector3d v2 = V.col(F.col(i)(1));
    Eigen::Vector3d v3 = V.col(F.col(i)(2));

    // using Heron's formula: https://en.wikipedia.org/wiki/Heron%27s_formula
    // (same results as cross product)
    double a, b, c, s;
    a = (v3 - v1).norm();
    b = (v2 - v1).norm();
    c = (v2 - v3).norm();
    s = (a + b + c) / 2;

    double surface_area_temp = sqrt(s * (s - a) * (s - b) * (s - c));

    // sums things up
    surface_area += surface_area_temp;
  }

  return surface_area;
};
