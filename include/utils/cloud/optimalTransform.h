#ifndef OPTIMAL_TRANSFORM_H
#define OPTIMAL_TRANSFORM_H

#include <Eigen/Core>
#include <Eigen/SVD>
#include <Eigen/Dense>

// see http://nghiaho.com/?page_id=671
inline void optimal_transform(Eigen::MatrixXd A, Eigen::MatrixXd B,
                              Eigen::Matrix3d &R, Eigen::Vector3d &t) {
  // get centroids
  Eigen::Vector3d centroid_A = A.rowwise().mean();
  Eigen::Vector3d centroid_B = B.rowwise().mean();
  
  // SVD
  Eigen::MatrixXd H = ( A.colwise() - centroid_A ) * ( B.colwise() - centroid_B ).transpose();
  Eigen::JacobiSVD<Eigen::MatrixXd> svd(H, Eigen::ComputeThinU | Eigen::ComputeThinV);
  Eigen::Matrix3d U = svd.matrixU();
  Eigen::Matrix3d V = svd.matrixV();


  // get R
  R = V * U.transpose();
  
  if (R.determinant() < 0) {
      V.col(2) = -V.col(2);
      R = V * U.transpose();
  }

  // get t
  t = centroid_B - R*centroid_A;

};

#endif
