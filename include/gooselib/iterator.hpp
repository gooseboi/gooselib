#pragma once

#include <cstddef>
#include "type_traits.hpp"

namespace goose {
    struct input_iterator_tag { };
    struct output_iterator_tag { };
    struct forward_iterator_tag : public input_iterator_tag { };
    struct bidirectional_iterator_tag : public forward_iterator_tag { };
    struct random_access_iterator_tag : public bidirectional_iterator_tag { };
    struct contiguous_iterator_tag: public random_access_iterator_tag { };

    template<typename Iter>
    struct iterator_traits {
        using difference_type = typename Iter::difference_type;
        using value_type = typename Iter::value_type;
        using pointer = typename Iter::pointer;
        using reference = typename Iter::reference;
        using iterator_category = typename Iter::iterator_category;
    };

    template<typename T>
    struct iterator_traits<T*> {
        using difference_type = std::ptrdiff_t;
        using value_type = remove_cv_t<T>;
        using pointer = T*;
        using reference = T&;
        using iterator_category = random_access_iterator_tag;
    };

    template<typename T>
    struct iterator_traits<const T*> {
        using difference_type = std::ptrdiff_t;
        using value_type = remove_cv_t<T>;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = random_access_iterator_tag;
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
    Range<Begin, End> make_range(Begin begin, End end) noexcept {
        return {begin, end};
    }

    template<typename Iter>
    struct reverse_iterator {
        public:
            using iterator_type = Iter;
            using difference_type = std::ptrdiff_t;
            using value_type = typename iterator_traits<Iter>::value_type;
            using pointer = typename iterator_traits<Iter>::pointer;
            using reference = typename iterator_traits<Iter>::reference;
            using iterator_category = random_access_iterator_tag;
        
        public:
            reverse_iterator() = default;
            reverse_iterator(iterator_type _iter) : m_iter{_iter} {}
            template<typename U>
            reverse_iterator(reverse_iterator<U> other) : m_iter{other.base()} {}

            template<typename U>
            reverse_iterator& operator=(const reverse_iterator<U> &other) {
                if (this == &other) return *this;
                m_iter = other.base();
                return *this;
            }

        public:
            reverse_iterator& operator++() {
                --m_iter;
                return *this;
            }
            reverse_iterator operator++(int) {
                reverse_iterator tmp = *this;
                --m_iter;
                return tmp;
            }
            reverse_iterator operator+(difference_type n) const {
                return {m_iter - n};
            }
            reverse_iterator& operator+=(difference_type n) {
                m_iter -= n;
                return *this;
            }
            
            reverse_iterator& operator--() {
                ++m_iter;
                return *this;
            }
            reverse_iterator operator--(int) {
                reverse_iterator tmp = *this;
                ++m_iter;
                return tmp;
            }

            reverse_iterator operator-(difference_type n) const {
                return {m_iter + n};
            }

            reverse_iterator operator-=(difference_type n) {
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
            bool operator==(const reverse_iterator<It2> &other) const {
                return this->base() == other.base();
            }
        
            template<typename It2>
            bool operator!=(const reverse_iterator<It2> &other) const {
                return this->base() != other.base();
            }
        
            template<typename It2>
            bool operator<(const reverse_iterator<It2> &other) const {
                return this->base() < other.base();
            }
        
            template<typename It2>
            bool operator>(const reverse_iterator<It2> &other) const {
                return this->base() > other.base();
            }
        
            template<typename It2>
            bool operator<=(const reverse_iterator<It2> &other) const {
                return this->base() <= other.base();
            }
        
            template<typename It2>
            bool operator>=(const reverse_iterator<It2> &other) const {
                return this->base() >= other.base();
            }

        private:
            Iter m_iter;
    };

    
}