#pragma once

#include "utility.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include <algorithm>

namespace goose {
    template<typename T, size_t N>
    struct _Array_iterator {
        public:
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using reference = T&;
            using pointer = T*;
            using iterator_category = goose::random_access_iterator_tag;
        public:
            _Array_iterator() noexcept = default;
            _Array_iterator(pointer _ptr) noexcept : m_ptr{_ptr} {}

        public:
            _Array_iterator& operator++() {
                ++m_ptr;
                return *this;
            }

            _Array_iterator operator++(int) {
                auto tmp = *this;
                ++m_ptr;
                return tmp;
            }

            _Array_iterator& operator+=(difference_type n) {
                m_ptr += n;
                return *this;
            }

            _Array_iterator operator+(difference_type n) const {
                return {m_ptr + n};
            }

        public:
            bool operator==(const _Array_iterator &other) const {
                return m_ptr == other.m_ptr;
            }

            bool operator!=(const _Array_iterator &other) const {
                return !(*this == other);
            }
        
        public:
            T& operator*() const {
                return *m_ptr;
            }
            T* operator->() const {
                return m_ptr;
            }
        private:            
        T* m_ptr;
    };

    template<typename T, size_t N>
    struct array {
        public:
            using value_type = T;
            using size_type = size_t;
            using reference = value_type&;
            using const_reference = const value_type&;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using iterator = _Array_iterator<T, N>;
            using const_iterator = _Array_iterator<const T, N>;
            using reverse_iterator = goose::reverse_iterator<iterator>;
            using const_reverse_iterator = goose::reverse_iterator<const_iterator>;
        public:
            constexpr reference operator[](size_t pos) noexcept { return _m_elems[pos]; }
            constexpr const_reference operator[](size_t pos) const noexcept { return _m_elems[pos]; }
            constexpr reference at(size_t pos) noexcept { return _m_elems[pos]; }
            constexpr const_reference at(size_t pos) const noexcept { return _m_elems[pos]; }
            
            constexpr reference front() noexcept { return _m_elems[0]; }
            constexpr const_reference front() const noexcept { return _m_elems[0]; }
        
            constexpr reference back() noexcept { return _m_elems[this->size()]; }
            constexpr const_reference back() const noexcept { return _m_elems[this->size()]; }
        
            constexpr pointer data(size_t pos) noexcept { return _m_elems; }
            constexpr const_pointer data(size_t pos) const noexcept { return _m_elems; }

        public:
            constexpr size_type size() const noexcept { return N; }
            constexpr size_type empty() const noexcept { return N == 0; }

        public: 
            void fill(const_reference val) noexcept {
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
            constexpr iterator begin() noexcept { return _m_elems; }
            constexpr const_iterator begin() const noexcept { return cbegin(); }
            constexpr const_iterator cbegin() const noexcept { return _m_elems; }

            constexpr iterator end() noexcept { return _m_elems + this->size(); }
            constexpr const_iterator end() const noexcept { return cend(); }
            constexpr const_iterator cend() const noexcept { return _m_elems + this->size(); }

            constexpr reverse_iterator rbegin() noexcept { return end(); }
            constexpr const_reverse_iterator rbegin() const noexcept { return crbegin(); }
            constexpr const_reverse_iterator crbegin() const noexcept { return cend(); }

            constexpr reverse_iterator rend() noexcept { return begin(); }
            constexpr const_reverse_iterator rend() const noexcept { return crend(); }
            constexpr const_reverse_iterator crend() const noexcept { return cbegin(); }

        public:
            bool operator==(const array &other) const {
                for (size_type i = 0; i < this->size(); ++i) {
                    if((*this)[i] != other[i]) return false;
                }
                return true;
            }
        
            bool operator!=(const array &other) const {
                return !(*this == other);
            }
        
            bool operator<(const array &other) const {
                return goose::lexicographical_compare(this->begin(), this->end(), other.begin(), other.end());
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
            T _m_elems[N];
    };
}