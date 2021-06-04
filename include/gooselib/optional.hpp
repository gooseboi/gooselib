#pragma once

#include <utility>
#include "type_traits.hpp"
#include "cstddef.hpp"

namespace goose {
    template<typename T>
    struct optional {
        public:
            optional() noexcept {}

            template<typename U>
            optional(const U& val) : m_has_value{true} {
                new (m_buffer) T(val);
            }

            optional(T&& val) : m_has_value{true} {
                new (m_buffer) T(std::move(val));
            }

            ~optional() {
                clear();
            }

        public:
            T& operator*() { return value(); }
            const T& operator*() const { return value(); }
            T& value() { return reinterpret_cast<T&>(m_buffer); }
            const T& value() const { return reinterpret_cast<const T&>(m_buffer); }

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
            byte m_buffer[sizeof(T)]{};
            bool m_has_value{false};
    };
}