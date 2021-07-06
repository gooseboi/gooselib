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

    template<class T, typename U>
    T exchange(T& obj, U&& newVal) noexcept {
        T oldValue = std::move(obj);
        obj = std::forward<U>(newVal);
        return oldValue;
    }  

    template<class T>
    addRValueReference<T> declval() noexcept;
}