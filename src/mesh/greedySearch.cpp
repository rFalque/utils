#include "mesh/greedySearch.h"

#include <iostream>
#include <limits>  // for numeric_limits
#include <stdexcept>
#include <vector>

const weight_t max_weight = std::numeric_limits<double>::infinity();

double distance_between_points(Eigen::Vector3d a, Eigen::Vector3d b) {
  return sqrt(pow(a(0) - b(0), 2) + pow(a(1) - b(1), 2) + pow(a(2) - b(2), 2));
};

bool is_element_in_vector(int a, std::vector<int>& A, int& element_position) {
  bool point_is_in_set = false;

  for (int i = 0; i < A.size(); ++i)
    if (a == A.at(i)) {
      element_position = i;
      point_is_in_set = true;
    }

  return point_is_in_set;
};

GreedySearch::GreedySearch(Eigen::MatrixXd V, Eigen::MatrixXi F,
                           std::vector<int> indexes_of_sub_V_in_V) {
  // set private variables
  if (V.rows() == 3 && F.rows() == 3) {
    V_ = V;
    F_ = F;
  } else if (V.cols() == 3 && F.cols() == 3) {
    V_ = V.transpose();
    F_ = F.transpose();
  } else {
    throw std::invalid_argument("wrong input size");
  }
  // sub_V_ = sub_V;
  graph_size_ = V_.cols();

  // circulate once through each face
  adjacency_list_.resize(V_.cols());
  for (int i = 0; i < F_.cols(); ++i) {
    adjacency_list_.at(F_(0, i)).push_back(neighbor(
        F_(1, i), distance_between_points(V_.col(F_(0, i)), V_.col(F_(1, i)))));
    adjacency_list_.at(F_(1, i)).push_back(neighbor(
        F_(2, i), distance_between_points(V_.col(F_(1, i)), V_.col(F_(2, i)))));
    adjacency_list_.at(F_(2, i)).push_back(neighbor(
        F_(0, i), distance_between_points(V_.col(F_(2, i)), V_.col(F_(0, i)))));
  }

  // indexes_of_sub_V_in_V_ could be set as an input instead of sub_V
  indexes_of_sub_V_in_V_ = indexes_of_sub_V_in_V;
};

std::vector<int> GreedySearch::return_k_closest_points(int source, int k) {
  std::vector<int> output;

  // initialize the distances to all nodes:
  std::vector<double> min_distance;
  min_distance.resize(graph_size_, max_weight);

  min_distance.at(source) = 0;

  // set the set of visited nodes:
  std::vector<int> visited;
  std::vector<int> to_visit;

  // initialize the node to start from
  int u = source;
  int element_position;

  // start searching
  int sub_V_found = 0;
  while (sub_V_found != k) {
    for (int i = 0; i < adjacency_list_.at(u).size(); ++i) {
      if (not is_element_in_vector(adjacency_list_.at(u).at(i).target, to_visit,
                                   element_position))
        if (not is_element_in_vector(adjacency_list_.at(u).at(i).target,
                                     visited, element_position))
          to_visit.push_back(adjacency_list_.at(u).at(i).target);

      if (min_distance.at(u) + adjacency_list_.at(u).at(i).weight <
          min_distance.at(adjacency_list_.at(u).at(i).target)) {
        min_distance.at(adjacency_list_.at(u).at(i).target) =
            min_distance.at(u) + adjacency_list_.at(u).at(i).weight;
      }
    }

    visited.push_back(u);

    // check if the visited point is in sub_V
    if (is_element_in_vector(u, indexes_of_sub_V_in_V_, element_position)) {
      ++sub_V_found;
      output.push_back(element_position);
    }

    // set next u
    int index_of_next_point = 0;
    for (int i = 0; i < to_visit.size(); ++i)
      if (min_distance.at(to_visit.at(i)) <
          min_distance.at(to_visit.at(index_of_next_point)))
        index_of_next_point = i;

    u = to_visit.at(index_of_next_point);
    to_visit.erase(to_visit.begin() + index_of_next_point);
  }

  return output;
};

void GreedySearch::remove_element(int element_to_remove) {
  if (element_to_remove < indexes_of_sub_V_in_V_.size())
    indexes_of_sub_V_in_V_.erase(indexes_of_sub_V_in_V_.begin() +
                                 element_to_remove);
  else
    throw std::invalid_argument(
        "Error in " + std::string(__func__) + ": the element to remove is larger than the size of sub_V" );
};
