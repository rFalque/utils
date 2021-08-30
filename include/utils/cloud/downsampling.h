#ifndef DOWNSAMPLING_H
#define DOWNSAMPLING_H

#include <Eigen/Core>

inline void downsampling_skip(Eigen::MatrixXd& V, Eigen::MatrixXd& N, Eigen::MatrixXi& C, int skip) {
    Eigen::MatrixXd V_temp;
    Eigen::MatrixXd N_temp;
    Eigen::MatrixXi C_temp;
    int cloud_new_size = V.cols() / skip;

    V_temp.resize(3, cloud_new_size);
    N_temp.resize(3, cloud_new_size);
    C_temp.resize(3, cloud_new_size);

    for (int i=0; i<cloud_new_size; i++)
        V_temp.col(i) = V.col(i*skip);

    if ( N.cols() == V.cols() )
        for (int i=0; i<cloud_new_size; i++)
            N_temp.col(i) = N.col(i*skip);

    if ( C.cols() == V.cols() )
        for (int i=0; i<cloud_new_size; i++)
            C_temp.col(i) = C.col(i*skip);

    V = V_temp;
    N = N_temp;
    C = C_temp;
};

#endif