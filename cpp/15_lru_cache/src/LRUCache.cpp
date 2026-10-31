#include "LRUCache.hpp"
#include <string>

namespace ds {

template class LRUCache<int, int>;
template class LRUCache<std::string, std::string>;
template class LRUCache<int, std::string>;

} // namespace ds
