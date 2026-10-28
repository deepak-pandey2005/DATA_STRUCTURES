#ifndef CPP_STACK_HPP
#define CPP_STACK_HPP

#include "Vector.hpp"
#include <cstddef>

namespace ds {

template <typename T>
class Stack {
public:
    Stack();

    void push(const T &val);
    void push(T &&val);
    T pop();

    T &top();
    const T &top() const;

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    void clear() noexcept;

private:
    Vector<T> container_;
};

} // namespace ds

#endif // CPP_STACK_HPP
