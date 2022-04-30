/*
 *   compute the normals using the faces
 *   by R. Falque
 *   16/01/2020
 */

#include "mesh/computeNormals.h"

Eigen::MatrixXd compute_vertices_normals(Eigen::MatrixXd V, Eigen::MatrixXi F) {
  Eigen::MatrixXd normals;
  Eigen::Vector3d normal_temp, v1, v2;

  if (V.rows() == 3 && F.rows() == 3) {
    normals = Eigen::MatrixXd::Zero(3, V.cols());

    for (int i = 0; i < F.cols(); i++) {
      v1 = V.col(F(1, i)) - V.col(F(0, i));
      v2 = V.col(F(2, i)) - V.col(F(0, i));
      normal_temp = v1.cross(v2);
      normals.col(F(0, i)) += normal_temp;
      normals.col(F(1, i)) += normal_temp;
      normals.col(F(2, i)) += normal_temp;
    }

    for (int i = 0; i < normals.cols(); i++) {
      if (normals.col(i).norm() != 0)
        normals.col(i) = normals.col(i).normalized();
      else
        std::cout << "Warning in " + std::string(__func__) + ": normals with zero values!\n";
    }

  } else if (V.cols() == 3 && F.cols() == 3) {
    normals = Eigen::MatrixXd::Zero(V.rows(), 3);

    for (int i = 0; i < F.rows(); i++) {
      v1 = V.row(F(i, 1)) - V.row(F(i, 0));
      v2 = V.row(F(i, 2)) - V.row(F(i, 0));
      normal_temp = v1.cross(v2);
      normals.row(F(i, 0)) += normal_temp;
      normals.row(F(i, 1)) += normal_temp;
      normals.row(F(i, 2)) += normal_temp;
    }

    for (int i = 0; i < normals.rows(); i++) {
      if (normals.row(i).norm() != 0)
        normals.row(i) = normals.row(i).normalized();
      else
        std::cout << "Warning in " + std::string(__func__) + ": normals with zero values!\n";
    }

  } else {
    throw std::invalid_argument("Error in " + std::string(__func__) + ": wrong input size");
  }

  return normals;
};

Eigen::MatrixXd compute_faces_normals(Eigen::MatrixXd V, Eigen::MatrixXi F) {
  Eigen::MatrixXd normals;
  Eigen::Vector3d v1, v2;
  if (V.rows() == 3 && F.rows() == 3) {
    normals = Eigen::MatrixXd::Zero(3, F.cols());

    for (int i = 0; i < F.cols(); i++) {
      v1 = V.col(F(1, i)) - V.col(F(0, i));
      v2 = V.col(F(2, i)) - V.col(F(0, i));
      normals.col(i) = (v1.cross(v2)).normalized();
    }

  } else if (V.cols() == 3 && F.cols() == 3) {
    normals = Eigen::MatrixXd::Zero(F.rows(), 3);

    for (int i = 0; i < F.rows(); i++) {
      v1 = V.row(F(i, 1)) - V.row(F(i, 0));
      v2 = V.row(F(i, 2)) - V.row(F(i, 0));
      normals.row(i) = (v1.cross(v2)).normalized();
    }

  } else {
    throw std::invalid_argument("Error in " + std::string(__func__) + ": wrong input size");
  }

  return normals;
};
