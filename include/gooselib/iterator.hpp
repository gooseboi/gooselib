#pragma once

#include <cstddef>
#include "type_traits.hpp"

namespace goose {
    struct inputIteratorTag { };
    struct outputIteratorTag { };
    struct forwardIteratorTag : public inputIteratorTag { };
    struct bidirectionalIteratorTag : public forwardIteratorTag { };
    struct randomAccessIteratorTag : public bidirectionalIteratorTag { };
    struct contiguousIteratorTag: public randomAccessIteratorTag { };

    template<typename Iter>
    struct iteratorTraits {
        using differenceType = typename Iter::differenceType;
        using valueType = typename Iter::valueType;
        using pointer = typename Iter::pointer;
        using reference = typename Iter::reference;
        using iteratorCategory = typename Iter::iteratorCategory;
    };

    template<typename T>
    struct iteratorTraits<T*> {
        using differenceType = std::ptrdiff_t;
        using valueType = removeCV<T>;
        using pointer = T*;
        using reference = T&;
        using iteratorCategory = randomAccessIteratorTag;
    };

    template<typename T>
    struct iteratorTraits<const T*> {
        using differenceType = std::ptrdiff_t;
        using valueType = removeCV<T>;
        using pointer = const T*;
        using reference = const T&;
        using iteratorCategory = randomAccessIteratorTag;
    };

    template<typename Begin, typename End>
    struct Range {
        public:
            Range(Begin _begin, End _end) : m_begin{_begin}, m_end{_end} {}
            Begin begin() {
                return m_begin;
            }
            End end() {
                return m_end;
            }
        private:
            Begin m_begin;
            End m_end;
    };
    
    template<typename Begin, typename End = Begin>
    Range<Begin, End> makeRange(Begin begin, End end) noexcept {
        return {begin, end};
    }

    template<typename Iter>
    struct reverseIterator {
        public:
            using iteratorType = Iter;
            using differenceType = std::ptrdiff_t;
            using valueType = typename iteratorTraits<Iter>::valueType;
            using pointer = typename iteratorTraits<Iter>::pointer;
            using reference = typename iteratorTraits<Iter>::reference;
            using iteratorCategory = randomAccessIteratorTag;
        
        public:
            reverseIterator() = default;
            reverseIterator(iteratorType _iter) : m_iter{_iter} {}
            template<typename U>
            reverseIterator(reverseIterator<U> other) : m_iter{other.base()} {}

            template<typename U>
            reverseIterator& operator=(const reverseIterator<U> &other) {
                if (this == &other) return *this;
                m_iter = other.base();
                return *this;
            }

        public:
            reverseIterator& operator++() {
                --m_iter;
                return *this;
            }
            reverseIterator operator++(int) {
                reverseIterator tmp = *this;
                --m_iter;
                return tmp;
            }
            reverseIterator operator+(differenceType n) const {
                return {m_iter - n};
            }
            reverseIterator& operator+=(differenceType n) {
                m_iter -= n;
                return *this;
            }
            
            reverseIterator& operator--() {
                ++m_iter;
                return *this;
            }
            reverseIterator operator--(int) {
                reverseIterator tmp = *this;
                ++m_iter;
                return tmp;
            }

            reverseIterator operator-(differenceType n) const {
                return {m_iter + n};
            }

            reverseIterator operator-=(differenceType n) {
                m_iter += n;
                return *this;
            }

        public:
            Iter base() const {
                return m_iter;
            }

            reference operator*() const {
                return *(m_iter - 1);
            }

            pointer operator->() const {
                return m_iter - 1;
            }
        public:
            template<typename It2>
            bool operator==(const reverseIterator<It2> &other) const {
                return this->base() == other.base();
            }
        
            template<typename It2>
            bool operator!=(const reverseIterator<It2> &other) const {
                return this->base() != other.base();
            }
        
            template<typename It2>
            bool operator<(const reverseIterator<It2> &other) const {
                return this->base() < other.base();
            }
        
            template<typename It2>
            bool operator>(const reverseIterator<It2> &other) const {
                return this->base() > other.base();
            }
        
            template<typename It2>
            bool operator<=(const reverseIterator<It2> &other) const {
                return this->base() <= other.base();
            }
        
            template<typename It2>
            bool operator>=(const reverseIterator<It2> &other) const {
                return this->base() >= other.base();
            }

        private:
            Iter m_iter;
    };

    
}