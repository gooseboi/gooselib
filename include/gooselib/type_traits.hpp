#pragma once

namespace goose {
    template<typename T>
    struct remove_const { using type = T; };
    
    template<typename T>
    struct remove_const<const T> { using type = T; };

    template<typename T>
    using remove_const_t = typename remove_const<T>::type;

    template<typename T>
    struct remove_volatile { using type = T; };

    template<typename T>
    struct remove_volatile<volatile T> { using type = T; };

    template<typename T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    template<typename T>
    struct remove_cv { using type = remove_const_t<remove_volatile_t<T>>; };

    template<typename T>
    using remove_cv_t = typename remove_cv<T>::type;

    template<typename T>
    struct remove_reference { using type = T; };

    template<typename T>
    struct remove_reference<T&> { using type = T; };

    template<typename T>
    struct remove_reference<T&&> { using type = T; };

    template<typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    template<typename T>
    struct add_lvalue_reference { using type = T&; };

    template<typename T>
    struct add_lvalue_reference<T&> { using type = T; };

    template<typename T>
    using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

    template<typename T>
    struct add_rvalue_reference { using type = T&&; };

    template<typename T>
    struct add_rvalue_reference<T&&> { using type = T; };

    template<typename T>
    using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

    template<bool B, typename T = void>
    struct enable_if {};

    template<typename T>
    struct enable_if<true, T> { using type = T; };

    template<bool B, typename T = void>
    using enable_if_t = typename enable_if<B, T>::type;

    template<class T, T v>
    struct integral_constant {
        static constexpr T value = v;
        using value_type = T;
        using type = integral_constant; 
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; } 
    };

    template<bool B>
    using bool_constant = integral_constant<bool, B>;

    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;
    
    template<bool B, typename T, typename F>
    struct conditional { using type = F; };

    template<typename T, typename F>
    struct conditional<true, T, F> { using type = T; };

    template<bool B, typename T, typename F>
    using conditional_t = typename conditional<B, T, F>::type;

    template<typename... T>
    using void_t = void;

    template<typename D, typename Void, template<typename...> class Op, typename... Args>
    struct detector {
        using type = D;
    };

    template<typename D, template<typename...> class Op, typename... Args>
    struct detector<D, goose::void_t<Op<Args...>>, Op, Args...> {
        using type = Op<Args...>;
    };
    
    template<typename D, template<typename...> class Op, typename... Args>
    using detected_or = detector<D, void, Op, Args...>;
    
    template<typename D, template<typename...> class Op, typename... Args>
    using detected_or_t = typename detected_or<D, Op, Args...>::type;
}