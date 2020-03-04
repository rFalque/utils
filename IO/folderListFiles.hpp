/*
to add in CMakeList:

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 COMPONENTS filesystem) 

include_directories(${filename} ${Boost_INCLUDE_DIRS} )
target_link_libraries(${filename} ${Boost_LIBRARIES} )

*/

#pragma once

#include <iostream>      // std::cout
#include <fstream>      // std::ifstream
#include <string>
#include <vector>
#include <Eigen/Core>

#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations

struct sort_functor
{
    bool operator ()(const std::string & a,const std::string & b)
    {
        return a < b;// or some custom code
    }
};

int load_folder(std::string folder_path, 
                std::vector< std::string >& filepaths,
                std::vector< std::string >& filenames)
{
    boost::filesystem::path sub_maps_path( folder_path );
    if ( !boost::filesystem::exists( sub_maps_path ) ) return -1;

    int counter = 0;
    boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end

    // count the number of files to load
    for ( boost::filesystem::directory_iterator itr( sub_maps_path );
          itr != end_itr;
          ++itr )
    {
        std::string filename = itr->path().stem().c_str();
        std::string filepath = itr->path().c_str();
        if( not boost::filesystem::is_directory(itr->status()))
            if(filepath.substr( filepath.length() - 4 ) == ".ply" || filepath.substr( filepath.length() - 4 ) == ".csv")
            {
                filenames.push_back(filename);
                filepaths.push_back(filepath);
                counter ++;
            }
    }

    std::sort(filenames.begin(),filenames.end(),sort_functor());
    std::sort(filepaths.begin(),filepaths.end(),sort_functor());

    return 0;
};
