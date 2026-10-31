#include "BloomFilter.hpp"
#include <string>

namespace ds {

template class BloomFilter<std::string>;
template class BloomFilter<int>;

} // namespace ds
