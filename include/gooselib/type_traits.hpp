#pragma once

#include <type_traits>

namespace goose {
    template<typename T>
    struct _removeConst { using type = T; };
    
    template<typename T>
    struct _removeConst<const T> { using type = T; };

    template<typename T>
    using removeConst = typename _removeConst<T>::type;

    template<typename T>
    struct _removeVolatile { using type = T; };

    template<typename T>
    struct _removeVolatile<volatile T> { using type = T; };

    template<typename T>
    using removeVolatile = typename _removeVolatile<T>::type;

    template<typename T>
    struct _removeCV { using type = removeConst<removeVolatile<T>>; };

    template<typename T>
    using removeCV = typename _removeCV<T>::type;

    template<typename T>
    struct _removeReference { using type = T; };

    template<typename T>
    struct _removeReference<T&> { using type = T; };

    template<typename T>
    struct _removeReference<T&&> { using type = T; };

    template<typename T>
    using removeReference = typename _removeReference<T>::type;

    template<typename T>
    struct _addLvalueReference { using type = T&; };

    template<typename T>
    struct _addLvalueReference<T&> { using type = T; };

    template<typename T>
    using addLvalueReference = typename _addLvalueReference<T>::type;

    template<typename T>
    struct _addRValueReference { using type = T&&; };

    template<typename T>
    struct _addRValueReference<T&&> { using type = T; };

    template<typename T>
    using addRValueReference = typename _addRValueReference<T>::type;

    template<bool B, typename T = void>
    struct enableIf {};

    template<typename T>
    struct enableIf<true, T> { using type = T; };

    template<bool B, typename T = void>
    using enableIfT = typename enableIf<B, T>::type;

    template<class T, T v>
    struct integralConstant {
        static constexpr T value = v;
        using valueType = T;
        using type = integralConstant; 
        constexpr operator valueType() const noexcept { return value; }
        constexpr valueType operator()() const noexcept { return value; } 
    };

    template<bool B>
    using boolConstant = integralConstant<bool, B>;

    using trueType = boolConstant<true>;
    using falseType = boolConstant<false>;
    
    template<bool B, typename T, typename F>
    struct _conditional { using type = F; };

    template<typename T, typename F>
    struct _conditional<true, T, F> { using type = T; };

    template<bool B, typename T, typename F>
    using conditional = typename _conditional<B, T, F>::type;

    template<typename... T>
    using voidT = void;

    template<typename D, typename Void, template<typename...> class Op, typename... Args>
    struct detector {
        using type = D;
    };

    template<typename D, template<typename...> class Op, typename... Args>
    struct detector<D, goose::voidT<Op<Args...>>, Op, Args...> {
        using type = Op<Args...>;
    };
    
    template<typename D, template<typename...> class Op, typename... Args>
    using _detectedOr = detector<D, void, Op, Args...>;
    
    template<typename D, template<typename...> class Op, typename... Args>
    using detectedOr = typename _detectedOr<D, Op, Args...>::type;

    template<typename T, typename U>
    struct isSame : trueType {};

    template<typename T>
    struct isSame<T, T> : trueType {};

    template<typename T, typename U>
    inline constexpr bool isSameV = isSame<T, U>::value;
}