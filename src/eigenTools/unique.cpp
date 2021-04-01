#include "eigenTools/unique.h"

void unique(Eigen::MatrixXi& M, std::vector< Eigen::Vector3i >& unique_elements, bool verbose)
{
    // build the list of element and count the occurences for each of them
    std::vector< int > element_counter;
    bool add_element;
    for (int i=0; i<M.cols(); i++)
    {
        add_element = true;
        for (int j=0; j<unique_elements.size(); j++)
        {
            if (unique_elements[j] == M.col(i))
            {
                add_element = false;
                element_counter[j] ++;
            }
        }
        if (add_element)
        {
            unique_elements.push_back(M.col(i));
            element_counter.push_back(1);
        }
    }

    // print the color list
    if (verbose) {
        for (int i=0; i<unique_elements.size(); i++)
            std::cout<<"element : [" << unique_elements[i].transpose() << "] \t seen " << element_counter[i] << " times" << std::endl;
    }
}


void unique(Eigen::MatrixXi& M, std::vector< Eigen::Vector3i >& unique_elements)
{
    bool verbose = false;
    unique(M, unique_elements);
}
