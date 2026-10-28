#include "Stack.hpp"
#include <stdexcept>
#include <utility>
#include <string>

namespace ds {

template <typename T>
Stack<T>::Stack() = default;

template <typename T>
void Stack<T>::push(const T &val) {
    container_.push_back(val);
}

template <typename T>
void Stack<T>::push(T &&val) {
    container_.push_back(std::move(val));
}

template <typename T>
T Stack<T>::pop() {
    if (empty()) {
        throw std::underflow_error("Stack is empty, cannot pop");
    }
    T top_val = std::move(container_.back());
    container_.pop_back();
    return top_val;
}

template <typename T>
T &Stack<T>::top() {
    if (empty()) {
        throw std::underflow_error("Stack is empty, cannot peek top");
    }
    return container_.back();
}

template <typename T>
const T &Stack<T>::top() const {
    if (empty()) {
        throw std::underflow_error("Stack is empty, cannot peek top");
    }
    return container_.back();
}

template <typename T>
std::size_t Stack<T>::size() const noexcept {
    return container_.size();
}

template <typename T>
bool Stack<T>::empty() const noexcept {
    return container_.empty();
}

template <typename T>
void Stack<T>::clear() noexcept {
    container_.clear();
}

// Explicit template instantiations
template class Stack<int>;
template class Stack<double>;
template class Stack<char>;
template class Stack<std::string>;

} // namespace ds
