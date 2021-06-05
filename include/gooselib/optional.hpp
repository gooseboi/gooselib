#pragma once

#include <utility>
#include "type_traits.hpp"
#include "cstddef.hpp"

namespace goose {
    template<typename T>
    struct optional {
        public:
            optional() noexcept = default;

            optional(const T& val) : m_has_value{true} {
                construct_emplace(val);
            }

            optional(T&& val) : m_has_value{true} { 
                construct_emplace(std::move(val));
            }

            template<typename U>
            optional(const U& val) : m_has_value{true} {
                construct_emplace(val);
            }

            optional(const optional& other) {
                if (other) {
                    construct_emplace(*other);
                    m_has_value = true;
                } 
            }

            optional(optional&& other) {
                if (other) {
                    construct_emplace(std::move(*other));
                    m_has_value = true;
                } 
            }               

            template<typename U>
            optional(const optional<U>& other) {
                if (other) {
                    construct_emplace(*other);
                    m_has_value = true;
                } 
            }

            template<typename U>
            optional(optional<U>&& other) {
                if (other) {
                    construct_emplace(std::move(*other));
                    m_has_value = true;
                } 
            }

            template<typename... Args>
            optional(std::in_place_t, Args&&... args) : m_has_value{true} {
               construct_emplace(std::forward<Args>(args)...);
            }

            optional& operator=(const optional& other) {
                if (this == &other) return *this;
                if (other) {
                    if (*this) {
                        **this = *other;
                    } else {
                        construct_emplace(*other);
                    }
                    m_has_value = true;
                } else {
                    clear();
                }
                return *this;
            }

            optional& operator=(optional&& other) {
                if (this == &other) return *this;
                if (other) {
                    if (*this) {
                        **this = std::move(*other);
                    } else {
                        construct_emplace(std::move(*other));
                    }
                    m_has_value = true;
                } else {
                    clear();
                }
                return *this;
            }    
            
            template<typename U>
            optional& operator=(const optional<U>& other) {
                if (this == &other) return *this;
                if (other) {
                    if (*this) {
                        **this = *other;
                    } else {
                        construct_emplace(*other);
                    }
                    m_has_value = true;
                } else {
                    clear();
                }
                return *this;
            }

            template<typename U>
            optional& operator=(optional<U>&& other) {
                if (this == &other) return *this;
                if (other) {
                    if (*this) {
                        **this = std::move(*other);
                    } else {
                        construct_emplace(std::move(*other));
                    }
                    m_has_value = true;
                } else {
                    clear();
                }
                return *this;
            }

            ~optional() {
                clear();
            }

        public:
            [[nodiscard]] T& operator*() { return value(); }
            [[nodiscard]] const T& operator*() const { return value(); }
            [[nodiscard]] T& value() { return *reinterpret_cast<T*>(&m_buffer); }
            [[nodiscard]] const T& value() const { return *reinterpret_cast<const T*>(&m_buffer); }

            bool has_value() const {
                return m_has_value;
            }

            operator bool() const {
                return has_value();
            }

            void clear() {
                if (m_has_value) {
                    this->value().~T();
                    m_has_value = false;
                }
            }

        private:
            template<typename... Args>
            void construct_emplace(Args&& ... args) {
                new(m_buffer) T(std::forward<Args>(args)...);
            }
        private:
            alignas(T) byte m_buffer[sizeof(T)]{};
            bool m_has_value{false};
    };
}