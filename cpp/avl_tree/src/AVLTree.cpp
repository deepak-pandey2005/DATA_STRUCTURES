/**
 * @file AVLTree.cpp
 * @brief AVLTree explicit instantiations.
 */

#include "AVLTree.hpp"
#include <string>

namespace ds {
    template class AVLTree<int>;
    template class AVLTree<double>;
    template class AVLTree<std::string>;
}
