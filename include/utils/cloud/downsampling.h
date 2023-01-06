#ifndef DOWNSAMPLING_H
#define DOWNSAMPLING_H

#include <Eigen/Core>
#include <vector>

inline void downsampling_skip(Eigen::MatrixXd& V, Eigen::MatrixXd& N,
                              Eigen::MatrixXi& C, int skip) {
  int cloud_new_size = V.cols() / skip;
  Eigen::MatrixXd V_temp(3, cloud_new_size);
  Eigen::MatrixXd N_temp(3, cloud_new_size);
  Eigen::MatrixXi C_temp(3, cloud_new_size);

  for (int i = 0; i < cloud_new_size; i++) {
    V_temp.col(i) = V.col(i * skip);
  }

  if (N.cols() == V.cols()) {
    for (int i = 0; i < cloud_new_size; i++) {
      N_temp.col(i) = N.col(i * skip);
    }
  }

  if (C.cols() == V.cols()) {
    for (int i = 0; i < cloud_new_size; i++) {
      C_temp.col(i) = C.col(i * skip);
    }
  }

  V = V_temp;
  N = N_temp;
  C = C_temp;
};

inline void downsampling_skip(Eigen::MatrixXd& V, int skip) {
  int cloud_new_size = V.cols() / skip;
  Eigen::MatrixXd V_temp(3, cloud_new_size);

  for (int i = 0; i < cloud_new_size; i++) {
    V_temp.col(i) = V.col(i * skip);
  }

  V = V_temp;
};

bool farthest_sampling_by_sphere(Eigen::MatrixXd & in_cloud, double sample_radius, Eigen::MatrixXd & nodes, Eigen::VectorXi & correspondences);
bool farthest_sampling_by_sphere(Eigen::MatrixXd & in_cloud, double sample_radius, Eigen::MatrixXd & nodes);
void voxel_grid_downsampling(Eigen::MatrixXd & in_cloud, double leaf_size, Eigen::MatrixXd & out_cloud);
void downsampling(Eigen::MatrixXd & in_cloud, 
                  Eigen::MatrixXd & out_cloud, 
                  std::vector<int> & in_cloud_samples, 
                  double grid_resolution,
                  double leaf_size, 
                  bool use_farthest_sampling, 
                  bool use_relative_grid);

#endif
