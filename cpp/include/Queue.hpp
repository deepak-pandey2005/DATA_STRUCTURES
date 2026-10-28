#ifndef CPP_QUEUE_HPP
#define CPP_QUEUE_HPP

#include <cstddef>

namespace ds {

template <typename T>
class Queue {
public:
    explicit Queue(std::size_t initial_cap = 8);
    ~Queue();

    // Copy semantics
    Queue(const Queue &other);
    Queue &operator=(const Queue &other);

    // Move semantics
    Queue(Queue &&other) noexcept;
    Queue &operator=(Queue &&other) noexcept;

    void swap(Queue &other) noexcept;

    void enqueue(const T &val);
    void enqueue(T &&val);
    T dequeue();

    T &front();
    const T &front() const;
    T &back();
    const T &back() const;

    void clear() noexcept;

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] std::size_t capacity() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

private:
    T *buffer_;
    std::size_t head_;
    std::size_t tail_;
    std::size_t size_;
    std::size_t capacity_;

    static T *allocate(std::size_t n);
    static void deallocate(T *p) noexcept;
    void grow();
};

} // namespace ds

#endif // CPP_QUEUE_HPP
