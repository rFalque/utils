cmake_minimum_required(VERSION 3.1)

project(main_project)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# specific version of eigen needed
find_package(Eigen3 REQUIRED)

# yaml-cpp if not found, run sudo apt-get install libyaml-cpp-dev
#find_package(yaml-cpp REQUIRED)

find_package(CGAL REQUIRED)

#Include boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS thread system REQUIRED)
include_directories(boost)


file(GLOB_RECURSE my_c_list RELATIVE ${CMAKE_SOURCE_DIR} "app/*")


add_subdirectory(utils)
add_subdirectory(polyscope)

foreach(file_path ${my_c_list})
    string( REPLACE ".cpp" "" new_name ${file_path} )
    get_filename_component(filename ${new_name} NAME)
    add_executable( ${filename} ${file_path})
    set_target_properties(${filename} PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)
    include_directories(${filename}
                        ${EIGEN3_INCLUDE_DIR}
			)

    target_link_libraries(${filename}
                        utils
                        polyscope
                        CGAL::CGAL
			)

endforeach()
