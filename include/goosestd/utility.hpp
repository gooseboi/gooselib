#pragma once

namespace goose {
    struct numrange {
        
    }
    template<class T>
    void swap(T& a, T& b) noexcept {
        T temp = b;
        b = a;
        a = temp;
    }
}