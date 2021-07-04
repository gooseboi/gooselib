#pragma once

#include <utility>
#include "type_traits.hpp"
#include <cstddef>

namespace goose {
    template<typename T>
    struct optional {
        public:
            optional() noexcept = default;

            optional(const T& val) : mHasValue{true} {
                constructEmplace(val);
            }

            optional(T&& val) : mHasValue{true} { 
                constructEmplace(std::move(val));
            }

            template<typename U>
            optional(const U& val) : mHasValue{true} {
                constructEmplace(val);
            }

            optional(const optional& other) {
                if (other) {
                    constructEmplace(*other);
                    mHasValue = true;
                } 
            }

            optional(optional&& other) {
                if (other) {
                    constructEmplace(std::move(*other));
                    mHasValue = true;
                } 
            }               

            template<typename U>
            optional(const optional<U>& other) {
                if (other) {
                    constructEmplace(*other);
                    mHasValue = true;
                } 
            }

            template<typename U>
            optional(optional<U>&& other) {
                if (other) {
                    constructEmplace(std::move(*other));
                    mHasValue = true;
                } 
            }

            template<typename... Args>
            optional(std::in_place_t, Args&&... args) : mHasValue{true} {
               constructEmplace(std::forward<Args>(args)...);
            }

            optional& operator=(const optional& other) {
                if (this == &other) return *this;
                if (other) {
                    if (*this) {
                        **this = *other;
                    } else {
                        constructEmplace(*other);
                    }
                    mHasValue = true;
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
                        constructEmplace(std::move(*other));
                    }
                    mHasValue = true;
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
                        constructEmplace(*other);
                    }
                    mHasValue = true;
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
                        constructEmplace(std::move(*other));
                    }
                    mHasValue = true;
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
            [[nodiscard]] T& value() { return *reinterpret_cast<T*>(&mBuffer); }
            [[nodiscard]] const T& value() const { return *reinterpret_cast<const T*>(&mBuffer); }

            bool hasValue() const {
                return mHasValue;
            }

            operator bool() const {
                return hasValue();
            }

            void clear() {
                if (mHasValue) {
                    this->value().~T();
                    mHasValue = false;
                }
            }

        private:
            template<typename... Args>
            void constructEmplace(Args&& ... args) {
                construct_at(mBuffer);
            }
        private:
            alignas(T) std::byte mBuffer[sizeof(T)]{};
            bool mHasValue{false};
    };
}