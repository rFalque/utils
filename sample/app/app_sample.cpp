/*
*   sample
*   by R.FALQUE
*   08/04/2021
*/

// Basics
#include <iostream>
#include <vector>
#include <string>

#include <Eigen/Core>
#include <boost/filesystem.hpp>

#include "utils/IO/readPLY.h"
#include "utils/IO/writePLY.h"

// plotting functions
#include "utils/visualization/plotMesh.h"
#include "utils/visualization/plotCloud.h"

int main()
{
    // settings
    bool visualization = false;

    // vars
    string ply_path = "../data/box.ply";
    string out_path = "../data/output.ply";

    // load mesh
    Eigen::MatrixXd V, N;
    Eigen::MatrixXi F, C;

    readPLY(ply_path, V, F, N, C);

    if (visualization) {
        plotMesh(V, F);
        plotCloud(V);
    }

    writePLY(out_path, V, F, N, C);

    return(0);
}
