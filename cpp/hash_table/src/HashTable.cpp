/**
 * @file HashTable.cpp
 * @brief Explicit template instantiations for HashTable translation unit.
 */

#include "HashTable.hpp"
#include <string>

namespace ds {
    template class HashTable<std::string, int>;
    template class HashTable<int, int>;
    template class HashTable<std::string, std::string>;
}
