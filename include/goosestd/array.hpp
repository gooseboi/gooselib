#pragma once

#include "utility.hpp"

namespace goose {
    template<typename T, size_t N>
    struct array {        
       public:
        constexpr T& operator[](size_t pos) noexcept { return _m_elems[pos]; }
        constexpr const T& operator[](size_t pos) const noexcept { return _m_elems[pos]; }
        constexpr T& at(size_t pos) noexcept { return _m_elems[pos]; }
        constexpr const T& at(size_t pos) const noexcept { return _m_elems[pos]; }
        
        constexpr T& front() noexcept { return _m_elems[0]; }
        constexpr const T& front() const noexcept { return _m_elems[0]; }
    
        constexpr T& back() noexcept { return _m_elems[this->size()]; }
        constexpr const T& back() const noexcept { return _m_elems[this->size()]; }
    
        constexpr T* data(size_t pos) noexcept { return _m_elems; }
        constexpr const T* data(size_t pos) const noexcept { return _m_elems; }

        public:
            constexpr size_t size() const noexcept { return N; }
            constexpr size_t empty() const noexcept { return N == 0; }

        public: 
            void fill(const T& val) noexcept {
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
            constexpr T* begin() noexcept { return _m_elems; }
            constexpr const T* begin() const noexcept { return _m_elems; }
            constexpr T* end() noexcept { return _m_elems + this->size(); }
            constexpr const T* end() const noexcept { return _m_elems + this->size(); }

        public:
            T _m_elems[N];
    };
}