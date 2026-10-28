#ifndef CPP_VECTOR_HPP
#define CPP_VECTOR_HPP

#include <cstddef>
#include <initializer_list>

namespace ds {

template <typename T>
class Vector {
public:
    Vector();
    explicit Vector(std::size_t initial_cap);
    Vector(std::initializer_list<T> init);
    ~Vector();

    // Copy semantics
    Vector(const Vector &other);
    Vector &operator=(const Vector &other);

    // Move semantics
    Vector(Vector &&other) noexcept;
    Vector &operator=(Vector &&other) noexcept;

    void swap(Vector &other) noexcept;

    void push_back(const T &val);
    void push_back(T &&val);
    void pop_back();

    T &operator[](std::size_t index) noexcept;
    const T &operator[](std::size_t index) const noexcept;
    T &at(std::size_t index);
    const T &at(std::size_t index) const;

    T &front();
    const T &front() const;
    T &back();
    const T &back() const;

    void insert(std::size_t index, const T &val);
    void erase(std::size_t index);
    void reserve(std::size_t new_cap);
    void shrink_to_fit();
    void clear() noexcept;

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] std::size_t capacity() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    T *data() noexcept;
    const T *data() const noexcept;

    // Iterators
    T *begin() noexcept;
    const T *begin() const noexcept;
    T *end() noexcept;
    const T *end() const noexcept;
    const T *cbegin() const noexcept;
    const T *cend() const noexcept;

private:
    T *data_;
    std::size_t size_;
    std::size_t capacity_;

    static T *allocate(std::size_t n);
    static void deallocate(T *p) noexcept;
    void ensure_capacity(std::size_t min_cap);
};

} // namespace ds

#endif // CPP_VECTOR_HPP
