#pragma once

#include <memory>

namespace Containers {

    template<typename T>
    class Vector;

    template<typename T>
    class VectorIterator;

    template<typename T>
    class Vector {
    public:

        friend VectorIterator<T>;


        Vector() = default;

        Vector(size_t size) : size_(size), data_(new T[size]) {}

        Vector(size_t size, T default_val) : size_(size), data_(new T[size]) {
            for (size_t i = 0; i < size; ++i) {
                data_.get()[i] = default_val;
            }
        }

        ~Vector() = default;

        Vector(const Vector &) = delete;

        Vector(Vector &&) = delete;

        T &operator[](size_t index) {
            if (index >= size_) {
                throw std::out_of_range("Out of bounds");
            }
            return data_.get()[index];
        }

        const T &operator[](size_t index) const {
            if (index >= size_) {
                throw std::out_of_range("Out of bounds");
            }
            return data_.get()[index];
        }

        void Resize(size_t new_size) {
            if (new_size == size_) {
                return;
            }
            if (new_size == 0) {
                data_ = nullptr;
                size_ = 0;
                return;
            }
            if (size_ == 0) {
                data_ = std::shared_ptr<T[]>(new T[new_size]);
                size_ = new_size;
                return;
            }
            std::shared_ptr<T[]> new_data(new T[new_size]);
            for (size_t i = 0; i < std::min(new_size, size_); ++i) {
                new_data.get()[i] = data_.get()[i];
            }
            data_ = new_data;
            size_ = new_size;
        }


        VectorIterator<T> begin() {
            return VectorIterator<T>(this, 0);
        }

        VectorIterator<T> end() {
            return VectorIterator<T>(this, size_);
        }

        void Insert(VectorIterator<T> it, T elem) {
            if (it.container_ != this || it.pos_ > size_) {
                throw std::runtime_error("Wrong iterator");
            }
            size_t position = it.pos_;
            Resize(size_ + 1);
            for (size_t i = size_ - 1; i > position; --i) {
                data_.get()[i] = data_.get()[i - 1];
            }
            data_.get()[position] = elem;
        }

        void Erase(VectorIterator<T> it) {
            if (it.container_ != this || it.pos_ > size_) {
                throw std::runtime_error("Wrong iterator");
            }
            size_t position = it.pos_;
            for (size_t i = position; i < size_ - 1; ++i) {
                data_.get()[i] = data_.get()[i + 1];
            }
            Resize(size_ - 1);
        }


        size_t Size() const {
            return size_;
        }

    private:
        std::shared_ptr<T[]> data_ = nullptr;
        size_t size_ = 0;
    };


    template<typename T>
    class VectorIterator {
    public:

        friend Vector<T>;

        using value_type = T;
        using reference = T &;
        using pointer = T *;
        using difference_type = ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;


        VectorIterator(Vector<T> *cont, size_t pos)
                : container_(cont), pos_(pos) {}

        T &operator*() {
            return (*container_)[pos_];
        }

        bool operator==(const VectorIterator &other) {
            return container_ == other.container_ && pos_ == other.pos_;
        }

        bool operator!=(const VectorIterator &other) {
            return !(*this == other);
        }

        VectorIterator &operator++() {
            if (pos_ + 1 > container_->size_) {
                throw std::runtime_error("Out of bounds");
            } else {
                pos_++;
            }
            return *this;
        };

        VectorIterator operator++(int) {
            VectorIterator result = *this;
            ++(*this);
            return result;
        }

    private:
        Vector<T> *container_;
        size_t pos_;
    };

}