cmake_minimum_required(VERSION 3.11)

project(utils)

SET(INCLUDE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/include/utils/")
SET(SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src/")

SET(IGL_STATIC_LIBRARY "true")

find_package(Eigen3 REQUIRED)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS filesystem)

set(SRCS
    # coud
    ${SRC_ROOT}/cloud/downsampling.cpp
    ${SRC_ROOT}/cloud/embeddedDeformation.cpp

    # convertors

    # eigenTools
    ${SRC_ROOT}/eigenTools/unique.cpp

    # IO
    ${SRC_ROOT}/IO/folderListFiles.cpp

    # IO_libIGL (not compiled because of the templates)

    # mesh
    ${SRC_ROOT}/mesh/computeGeometricalProperties.cpp
    ${SRC_ROOT}/mesh/computeNormals.cpp
    ${SRC_ROOT}/mesh/greedySearch.cpp

    # STDTools

    # visualization

)

set(HEADERS
    # cloud
    ${INCLUDE_ROOT}/cloud/centering.h
    ${INCLUDE_ROOT}/cloud/downsampling.h
    ${INCLUDE_ROOT}/cloud/embeddedDeformation.h
    ${INCLUDE_ROOT}/cloud/optimalTransform.h
    ${INCLUDE_ROOT}/cloud/poissonReconstruction.h
    ${INCLUDE_ROOT}/cloud/scaleWithSVD.h

    # eigenTools
    ${INCLUDE_ROOT}/eigenTools/concatenate.h
    ${INCLUDE_ROOT}/eigenTools/convertorVector.h
    ${INCLUDE_ROOT}/eigenTools/getMinMax.h
    ${INCLUDE_ROOT}/eigenTools/nanoflann.h
    ${INCLUDE_ROOT}/eigenTools/nanoflannWrapper.h
    ${INCLUDE_ROOT}/eigenTools/removeCol.h
    ${INCLUDE_ROOT}/eigenTools/removeRow.h
    ${INCLUDE_ROOT}/eigenTools/sparseICP.h
    ${INCLUDE_ROOT}/eigenTools/sparse.h
    ${INCLUDE_ROOT}/eigenTools/standardDeviation.h
    ${INCLUDE_ROOT}/eigenTools/unique.h
    ${INCLUDE_ROOT}/eigenTools/variance.h

    # IO
    ${INCLUDE_ROOT}/IO/folderChecks.h
    ${INCLUDE_ROOT}/IO/folderListFiles.h
    ${INCLUDE_ROOT}/IO/readCSV.h
    ${INCLUDE_ROOT}/IO/stb_image_write.h
    ${INCLUDE_ROOT}/IO/structures.h
    ${INCLUDE_ROOT}/IO/writeCSV.h
    ${INCLUDE_ROOT}/IO/writePLY.h
    ${INCLUDE_ROOT}/IO/writePNG.h

    # IO_libIGL
    ${INCLUDE_ROOT}/IO_libIGL/list_to_matrix.h
    ${INCLUDE_ROOT}/IO_libIGL/max_size.h
    ${INCLUDE_ROOT}/IO_libIGL/min_size.h
    ${INCLUDE_ROOT}/IO_libIGL/ply.h
    ${INCLUDE_ROOT}/IO_libIGL/readPLY.h
    ${INCLUDE_ROOT}/IO_libIGL/readOBJ.h
    ${INCLUDE_ROOT}/IO_libIGL/readOFF.h
    ${INCLUDE_ROOT}/IO_libIGL/readSTL.h
    ${INCLUDE_ROOT}/IO_libIGL/writeOBJ.h
    ${INCLUDE_ROOT}/IO_libIGL/writeOFF.h

    # mesh
    ${INCLUDE_ROOT}/mesh/computeFacesCentroids.h
    ${INCLUDE_ROOT}/mesh/computeGeometricalProperties.h
    ${INCLUDE_ROOT}/mesh/computeNormals.h
    ${INCLUDE_ROOT}/mesh/greedySearch.h

    # STDTools
    ${INCLUDE_ROOT}/STDTools/sgn.h

    # visualization
    ${INCLUDE_ROOT}/visualization/correspondences.h
    ${INCLUDE_ROOT}/visualization/matplotlibcpp.h
    ${INCLUDE_ROOT}/visualization/plotCloud.h
    ${INCLUDE_ROOT}/visualization/plotTwoMeshes.h
    ${INCLUDE_ROOT}/visualization/sourceAndTarget.h
    ${INCLUDE_ROOT}/visualization/polyscopeWrapper.h

)

IF( CGAL_FOUND )
    set(CGAL_SRCS
        # coud
        ${SRC_ROOT}/cloud/poissonReconstruction.cpp

        # convertors
        ${SRC_ROOT}/convertors/igl_to_cgal.cpp

        # mesh
        ${SRC_ROOT}/mesh/nonRigidDeformation.cpp
    )

    set(CGAL_HEADERS
        # cloud
        ${INCLUDE_ROOT}/cloud/poissonReconstruction.h

        # convertors
        ${INCLUDE_ROOT}/convertors/igl_to_cgal.h
        ${INCLUDE_ROOT}/convertors/cgal_typedef.h

        # mesh
        ${INCLUDE_ROOT}/mesh/fillHoles.h
        ${INCLUDE_ROOT}/mesh/nonRigidDeformation.h
        ${INCLUDE_ROOT}/mesh/raycasting.h

    )
ENDIF()


add_library(utils ${SRCS} ${HEADERS} ${CGAL_SRCS} ${CGAL_HEADERS})

target_include_directories(utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(utils PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/utils")
target_include_directories(utils PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(utils PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(utils polyscope ${Boost_LIBRARIES} )
