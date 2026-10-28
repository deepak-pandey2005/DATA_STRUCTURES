/**
 * @file BinaryHeap.cpp
 * @brief Explicit template instantiations for BinaryHeap translation unit.
 */

#include "BinaryHeap.hpp"
#include <string>

namespace ds {
    template class BinaryHeap<int>;
    template class BinaryHeap<double>;
    template class BinaryHeap<std::string>;
    template class BinaryHeap<int, std::greater<int>>;
}
