#pragma once

#include "type_traits.hpp"

namespace goose {
    struct numrange {
       // TODO 
    };

    template<class T>
    void swap(T& a, T& b) noexcept {
        T temp = b;
        b = a;
        a = temp;
    }

    template<class T>
    add_rvalue_reference_t<T> declval() noexcept;
}