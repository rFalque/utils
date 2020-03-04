/*
*   compute the normals using the faces
*   by R. Falque
*   16/01/2020
*/

#ifndef COMPUTE_NORMALS_FROM_MESH_H
#define COMPUTE_NORMALS_FROM_MESH_H

#include <iostream>
#include <Eigen/Dense>
#include <stdexcept>

inline Eigen::MatrixXd compute_normals(Eigen::MatrixXd V, Eigen::MatrixXi F) {

    if (V.rows() == 3 && F.rows() == 3){
        Eigen::MatrixXd normals = Eigen::MatrixXd::Zero(3, V.cols());
        Eigen::Vector3d normal_temp, v1, v2;

        for (int i=0; i<F.cols(); i++) {
            v1 = V.col(F(1, i)) - V.col(F(0, i));
            v2 = V.col(F(2, i)) - V.col(F(0, i));
            normal_temp = v1.cross(v2);
            normals.col(F(0, i)) += normal_temp;
            normals.col(F(1, i)) += normal_temp;
            normals.col(F(2, i)) += normal_temp;
        }

        for (int i=0; i<normals.cols(); i++) {
            if (normals.col(i).norm() != 0) 
                normals.col(i) = normals.col(i).normalized();
            else
                std::cout << "Warning: normals with zero values!\n";
        }

        return normals;

    } else if (V.cols() == 3 && F.cols() == 3){
        Eigen::MatrixXd normals = Eigen::MatrixXd::Zero(V.rows(), 3);
        Eigen::Vector3d normal_temp, v1, v2;

        for (int i=0; i<F.rows(); i++) {
            v1 = V.row(F(i,1)) - V.row(F(i,0));
            v2 = V.row(F(i,2)) - V.row(F(i,0));
            normal_temp = v1.cross(v2);
            normals.row(F(i, 0)) += normal_temp;
            normals.row(F(i, 1)) += normal_temp;
            normals.row(F(i, 2)) += normal_temp;
        }

        for (int i=0; i<normals.rows(); i++) {
            if (normals.row(i).norm() != 0) 
                normals.row(i) = normals.row(i).normalized();
            else
                std::cout << "Warning: normals with zero values!\n";
        }

        return normals;

    } else {
        throw std::invalid_argument( "wrong input size" );
    }
};

#endif
