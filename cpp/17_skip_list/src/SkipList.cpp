#include "SkipList.hpp"
#include <string>

namespace ds {

template class SkipList<int, int>;
template class SkipList<int, std::string>;
template class SkipList<std::string, std::string>;

} // namespace ds
