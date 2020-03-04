#ifndef EIGEN_TOOLS_WRITECSV_H
#define EIGEN_TOOLS_WRITECSV_H

#include <Eigen/Core>
#include <vector>
#include <string>
#include <fstream>

//using namespace Eigen;

template<typename M> void writeCSV(std::string name, M matrix)
{
    const static Eigen::IOFormat CSVFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", "\n");
    std::ofstream file(name.c_str());
    file << matrix.format(CSVFormat);
    file.close();
 }

#endif