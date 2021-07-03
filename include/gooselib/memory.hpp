#pragma once

#include <cstddef>
#include <type_traits>

namespace goose {
    namespace _implementation {
        template<typename _Tp>
        struct get_first_arg {};
        
        template<template<typename, typename...> class _Template, typename _Tp, typename... _Types>
        struct get_first_arg<_Template<_Tp, _Types...>> { using type = _Tp; };
        
        template<typename _Tp>
        using get_first_arg_t = typename get_first_arg<_Tp>::type;

        template<typename Tp, typename Up>
        struct replace_first_arg {};

        template<template<typename, typename...> class Template, typename Up, typename Tp, typename... Types>
        struct replace_first_arg<Template<Tp, Types...>, Up>
        { using type = Template<Up, Types...>; };

        template<typename Tp, typename Up>
        using replace_first_arg_t = typename replace_first_arg<Tp, Up>::type;
    }

    template<typename Ptr>
    struct pointer_traits {
        private:
            template<typename Tp>
            using _element_type = typename Tp::element_type;
            template<typename Tp>
            using _difference_type = typename Tp::difference_type;    
            template<typename Tp, typename Up, typename = void>
            struct _rebind : _implementation::replace_first_arg<Tp, Up> { };
            template<typename Tp, typename Up>
            struct _rebind<Tp, Up, void_t<typename Tp::template rebind<Up>>>
            { using type = typename Tp::template rebind<Up>; };

        public: 
            using pointer = Ptr;
            using element_type = detected_or_t<_implementation::get_first_arg_t<Ptr>, _element_type, Ptr>;
            using difference_type = detected_or_t<std::ptrdiff_t, _difference_type, Ptr>;
            template<typename U>
            using rebind = typename _rebind<Ptr, U>::type;
    };

    template<typename T>
    struct allocator {
        public:
            using value_type = T;
        public:
            constexpr allocator() = default;
            constexpr allocator(const allocator&) = default;
            constexpr allocator(allocator&&) = default;
        public:
            friend bool operator==(const allocator& lhs, const allocator& rhs) { return true; }
            friend bool operator!=(const allocator& lhs, const allocator& rhs) { return false; }
        public:
            T* allocate(size_t bufSize) { return reinterpret_cast<T*>(new char[sizeof(T) * bufSize]); }

            void deallocate(T* allocated, size_t allocated_size) {
                delete[] allocated;
            }
    };

    template<typename Alloc>
    struct allocator_traits {
        private:
            template<typename Tp>
            using _pointer = typename Tp::pointer;
            template<typename Tp>
            using _const_pointer = typename Tp::const_pointer;   
            template<typename Tp>
            using _void_pointer = typename Tp::void_pointer; 
            template<typename Tp>
            using _const_void_pointer = typename Tp::const_void_pointer; 
            template<typename Tp>
            using _difference_type = typename Tp::difference_type;
            template<typename Tp>
            using _size_type = typename Tp::size_type;
        public:
            using allocator_type = Alloc;
            using value_type = typename Alloc::value_type;
            using pointer = detected_or_t<value_type*, _pointer, Alloc>;
            using const_pointer = detected_or_t<typename pointer_traits<pointer>::rebind<const value_type>, _const_pointer, Alloc>;
            using void_pointer = detected_or_t<typename pointer_traits<pointer>::rebind<void>, _void_pointer, Alloc>;
            using const_void_pointer = detected_or_t<typename pointer_traits<pointer>::rebind<const void>, _const_void_pointer, Alloc>;
            using difference_type = detected_or_t<typename pointer_traits<pointer>::difference_type, _difference_type, Alloc>;
            using size_type = detected_or_t<std::make_unsigned_t<difference_type>, _size_type, Alloc>;
    };
}