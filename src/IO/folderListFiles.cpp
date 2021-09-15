#include "IO/folderListFiles.h"

#include <Eigen/Core>
#include <fstream>   // std::ifstream
#include <iostream>  // std::cout
#include <string>
#include <vector>

#include "boost/filesystem.hpp"  // includes all needed Boost.Filesystem declarations

std::vector<std::string> extension_list{".ply", ".stl", ".csv"};

struct sort_functor {
  bool operator()(const std::string& a, const std::string& b) {
    return a < b;  // or some custom code
  }
};

int load_folder(std::string folder_path, std::vector<std::string>& filepaths,
                std::vector<std::string>& filenames) {
  boost::filesystem::path sub_maps_path(folder_path);
  if (!boost::filesystem::exists(sub_maps_path)) return -1;

  int counter = 0;
  boost::filesystem::directory_iterator
      end_itr;  // default construction yields past-the-end

  // count the number of files to load
  for (boost::filesystem::directory_iterator itr(sub_maps_path); itr != end_itr;
       ++itr) {
    std::string filename = itr->path().stem().c_str();
    std::string filepath = itr->path().c_str();
    if (not boost::filesystem::is_directory(itr->status())) {
      bool file_has_good_extension = false;
      for (int i = 0; i < extension_list.size(); i++)
        if (filepath.substr(filepath.length() - 4) == extension_list[i])
          file_has_good_extension = true;

      if (file_has_good_extension) {
        filenames.push_back(filename);
        filepaths.push_back(filepath);
        counter++;
      }
    }
  }

  std::sort(filenames.begin(), filenames.end(), sort_functor());
  std::sort(filepaths.begin(), filepaths.end(), sort_functor());

  return 0;
};
