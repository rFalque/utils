/*
*   sample for batch processing
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

#include "utils/IO/folderListFiles.h"

// plotting functions
#include "utils/visualization/plotMesh.h"
#include "utils/visualization/plotCloud.h"

int main()
{
    // settings
    bool visualization = false;
    std::string folder_path = "/path_to_folder";

    // vars
    std::vector< std::string > filepaths;
    std::vector< std::string > filenames;
    Eigen::MatrixXd V, N;
    Eigen::MatrixXi F, C;

    // load folder
    load_folder(folder_path, filepaths, filenames);

    // process batch
    for (int file_id=0; file_id < filenames.size(); file_id++) {
        std::cout << "Process file: " << filepaths[file_id] << std::endl;
        readPLY(filepaths[file_id], V, F, N);

        if (visualization) {
            plot_mesh(V, F);
        }

        writePLY(filepaths[file_id], V, F, N, C, false);
    }

    return(0);
}
