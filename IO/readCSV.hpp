#ifndef EIGEN_TOOLS_READCSV_H
#define EIGEN_TOOLS_READCSV_H

#include <Eigen/Dense>
#include <vector>
#include <string>
#include <fstream>

//using namespace Eigen;

// from: https://stackoverflow.com/a/39146048/2562693
template<typename M> M load_csv (const std::string & path) {
    std::ifstream indata;
    indata.open(path);
    std::string line;
    std::vector<double> values;
    uint rows = 0;
    while (std::getline(indata, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        while (std::getline(lineStream, cell, ',')) {
            values.push_back(std::stod(cell));
        }
        ++rows;
    }
    return Eigen::Map<const Eigen::Matrix<typename M::Scalar, M::RowsAtCompileTime, M::ColsAtCompileTime, Eigen::RowMajor>>(values.data(), rows, values.size()/rows);
};

#endif