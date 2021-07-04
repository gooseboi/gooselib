#pragma once

#include "type_traits.hpp"

namespace goose {
    struct numRange {
       // TODO 
    };

    template<class T>
    void swap(T& a, T& b) noexcept {
        T temp = b;
        b = a;
        a = temp;
    }

    template<class T>
    addRValueReference<T> declval() noexcept;
}