#pragma once

#include "utility.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"

namespace goose {
    template<typename T, size_t N>
    struct _arrayIterator {
        public:
            using valueType = T;
            using differenceType = std::ptrdiff_t;
            using reference = T&;
            using pointer = T*;
            using iteratorCategory = goose::randomAccessIteratorTag;
        public:
            _arrayIterator() noexcept = default;
            _arrayIterator(pointer _ptr) noexcept : mPtr{_ptr} {}

        public:
            _arrayIterator& operator++() {
                ++mPtr;
                return *this;
            }

            _arrayIterator operator++(int) {
                auto tmp = *this;
                ++mPtr;
                return tmp;
            }

            _arrayIterator& operator+=(differenceType n) {
                mPtr += n;
                return *this;
            }

            _arrayIterator operator+(differenceType n) const {
                return {mPtr + n};
            }
            
            _arrayIterator& operator--() {
                --mPtr;
                return *this;
            }

            _arrayIterator operator--(int) {
                auto tmp = *this;
                --mPtr;
                return tmp;
            }

            _arrayIterator& operator-=(differenceType n) {
                mPtr -= n;
                return *this;
            }

            _arrayIterator operator-(differenceType n) const {
                return {mPtr - n};
            }

        public:
            bool operator==(const _arrayIterator &other) const {
                return mPtr == other.mPtr;
            }

            bool operator!=(const _arrayIterator &other) const {
                return !(*this == other);
            }
        
        public:
            T& operator*() const {
                return *mPtr;
            }
            T* operator->() const {
                return mPtr;
            }
        private:            
        T* mPtr;
    };

    template<typename T, size_t N>
    struct array {
        public:
            using valueType = T;
            using sizeType = size_t;
            using reference = valueType&;
            using constReference = const valueType&;
            using pointer = valueType*;
            using constPointer = const valueType*;
            using iterator = _arrayIterator<T, N>;
            using constIterator = _arrayIterator<const T, N>;
            using reverseIterator = goose::reverseIterator<iterator>;
            using constReverseIterator = goose::reverseIterator<constIterator>;
        public:
            constexpr reference operator[](size_t pos) noexcept { return _mElems[pos]; }
            constexpr constReference operator[](size_t pos) const noexcept { return _mElems[pos]; }
            constexpr reference at(size_t pos) noexcept { return _mElems[pos]; }
            constexpr constReference at(size_t pos) const noexcept { return _mElems[pos]; }
            
            constexpr reference front() noexcept { return _mElems[0]; }
            constexpr constReference front() const noexcept { return _mElems[0]; }
        
            constexpr reference back() noexcept { return _mElems[this->size()]; }
            constexpr constReference back() const noexcept { return _mElems[this->size()]; }
        
            constexpr pointer data(size_t pos) noexcept { return _mElems; }
            constexpr constPointer data(size_t pos) const noexcept { return _mElems; }

        public:
            constexpr sizeType size() const noexcept { return N; }
            constexpr sizeType empty() const noexcept { return N == 0; }

        public: 
            void fill(constReference val) noexcept {
                for (auto &i : *this) {
                    i = val;
                }
            }
            void swap(array& other) {
                for (size_t i = 0; i < N; ++i) {
                    goose::swap((*this)[i], other[i]);
                }
            }
        
        public:
            constexpr iterator begin() noexcept { return _mElems; }
            constexpr constIterator begin() const noexcept { return cbegin(); }
            constexpr constIterator cbegin() const noexcept { return _mElems; }

            constexpr iterator end() noexcept { return _mElems + this->size(); }
            constexpr constIterator end() const noexcept { return cend(); }
            constexpr constIterator cend() const noexcept { return _mElems + this->size(); }

            constexpr reverseIterator rbegin() noexcept { return end(); }
            constexpr constReverseIterator rbegin() const noexcept { return crbegin(); }
            constexpr constReverseIterator crbegin() const noexcept { return cend(); }

            constexpr reverseIterator rend() noexcept { return begin(); }
            constexpr constReverseIterator rend() const noexcept { return crend(); }
            constexpr constReverseIterator crend() const noexcept { return cbegin(); }

        public:
            bool operator==(const array &other) const {
                for (sizeType i = 0; i < this->size(); ++i) {
                    if((*this)[i] != other[i]) return false;
                }
                return true;
            }
        
            bool operator!=(const array &other) const {
                return !(*this == other);
            }
        
            bool operator<(const array &other) const {
                return goose::lexicographicalCompare(this->begin(), this->end(), other.begin(), other.end());
            }
        
            bool operator>(const array &other) const {
                return other < *this;
            }
        
            bool operator<=(const array &other) const {
                return !(*this > other);
            }
        
            bool operator>=(const array &other) const {
                return !(*this < other);
            }

        public:
            T _mElems[N];
    };

    template<typename T, typename... U>
    array(T, U...) -> array<T, sizeof...(U) + 1>;
}