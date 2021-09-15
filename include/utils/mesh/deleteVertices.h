/*
 *   delete vertices
 *   by R. Falque
 *   31/03/2020
 */

#ifndef DELETE_VERTICES_H
#define DELETE_VERTICES_H

#include <Eigen/Core>
#include <iostream>
#include <vector>

inline void deleteVertices(Eigen::MatrixXd& V, Eigen::MatrixXi& F,
                           std::vector<int> vertices_to_delete) {
  // start by sorting vertices_to_delete in
  std::sort(vertices_to_delete.begin(), vertices_to_delete.end(),
            std::greater<int>());

  // create the complement from vertices_to_delete
  std::vector<int> vertices_to_keep(V.cols());
  for (int i = 0; i < vertices_to_keep.size(); i++) {
    vertices_to_keep[i] = i;
  }
  for (auto vertex : vertices_to_delete)
    vertices_to_keep.erase(vertices_to_keep.begin() + vertex);

  // build the list of faces to keep
  std::vector<int> faces_to_keep;
  for (int i = 0; i < F.cols(); i++) {
    bool keep_face = true;

    // check if the face has a vertex that will be removed
    for (int j = 0; j < F.rows(); j++) {
      auto it =
          find(vertices_to_delete.begin(), vertices_to_delete.end(), F(j, i));
      if (it != vertices_to_delete.end()) keep_face = false;
    }

    if (keep_face == true) {
      faces_to_keep.push_back(i);
    }
  }

  // update the faces
  Eigen::MatrixXi F_temp;
  F_temp.resize(3, faces_to_keep.size());
  for (int i = 0; i < faces_to_keep.size(); i++) {
    F_temp.col(i) = F.col(faces_to_keep[i]);
  }
  F = F_temp;

  // start by vertex referred by the faces
  for (int i = 0; i < F.cols(); i++) {
    for (int j = 0; j < F.rows(); j++) {
      for (auto vertex : vertices_to_delete) {
        if (F(j, i) >= vertex) {
          F(j, i) = F(j, i) - 1;
        }
      }
    }
  }

  // update the vertices
  Eigen::MatrixXd V_temp;
  V_temp.resize(3, vertices_to_keep.size());
  for (int i = 0; i < vertices_to_keep.size(); i++) {
    V_temp.col(i) = V.col(vertices_to_keep[i]);
  }
  V = V_temp;
}

#endif