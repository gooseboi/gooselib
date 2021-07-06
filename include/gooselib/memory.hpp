#pragma once

#include "type_traits.hpp"
#include "utility.hpp"
#include <cstddef>
#include <type_traits>
#include <utility>
#include <new>

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
    
    template<typename T, typename... Args>
    T* constructAt(T* ptr, Args&&... args) {
        return new (const_cast<void*>(static_cast<const volatile void*>(ptr)))T(std::forward<Args>(args)...);
    }
    template<typename T>
    void destroyAt(T* ptr) {
        ptr->~T();
    }

    template<typename Ptr>
    struct pointerTraits {
        private:
            template<typename Tp>
            using _elementType = typename Tp::elementType;
            template<typename Tp>
            using _differenceType = typename Tp::differenceType;    
            template<typename Tp, typename Up, typename = void>
            struct _rebind : _implementation::replace_first_arg<Tp, Up> { };
            template<typename Tp, typename Up>
            struct _rebind<Tp, Up, voidT<typename Tp::template rebind<Up>>>
            { using type = typename Tp::template rebind<Up>; };

        public: 
            using pointer = Ptr;
            using elementType = detectedOr<_implementation::get_first_arg_t<Ptr>, _elementType, Ptr>;
            using differenceType = detectedOr<std::ptrdiff_t, _differenceType, Ptr>;
            template<typename U>
            using rebind = typename _rebind<Ptr, U>::type;
    };

    template<typename T>
    struct pointerTraits<T*> {
            using pointer = T*;
            using elementType = T;
            using differenceType = std::ptrdiff_t;
            template<typename U>
            using rebind = U*;
    };

    template<typename T>
    struct allocator {
        public:
            using valueType = T;
        public:
            constexpr allocator() = default;
            constexpr allocator(const allocator&) = default;
            constexpr allocator(allocator&&) = default;
        public:
            friend bool operator==(const allocator& lhs, const allocator& rhs) { return true; }
            friend bool operator!=(const allocator& lhs, const allocator& rhs) { return false; }
        public:
            T* allocate(size_t bufSize) { return reinterpret_cast<T*>(new char[sizeof(T) * bufSize]); }

            void deallocate(T* allocated, size_t allocatedSize) {
                delete[] allocated;
            }
    };

    template<typename Alloc>
    struct allocatorTraits {
        private:
            template<typename Tp>
            using _pointer = typename Tp::pointer;
            template<typename Tp>
            using _constPointer = typename Tp::constPointer;   
            template<typename Tp>
            using _voidPointer = typename Tp::voidPointer; 
            template<typename Tp>
            using _constVoidPointer = typename Tp::constVoidPointer; 
            template<typename Tp>
            using _differenceType = typename Tp::differenceType;
            template<typename Tp>
            using _sizeType = typename Tp::sizeType;    
            
            template<typename Tp, typename... Args>
            struct _construct_helper {
                template<typename Alloc2,
                typename = decltype(std::declval<Alloc2*>()->construct(
                std::declval<Tp*>(), std::declval<Args>()...))>
                static trueType _test(int);

                template<typename>
                static falseType _test(...);

                using type = decltype(_test<Alloc>(0));
            };

            template<typename Tp, typename... Args>
            using _has_construct = typename _construct_helper<Tp, Args...>::type;

            template<typename Tp>
            struct _destroy_helper {
                template<typename Alloc2,
                typename = decltype(std::declval<Alloc2*>()->destroy(
                std::declval<Tp*>()))>
                static trueType _test(int);

                template<typename>
                static falseType _test(...);

                using type = decltype(_test<Alloc>(0));
            };

            template<typename Tp>
            using _has_destroy = typename _destroy_helper<Tp>::type;
        public:
            using allocatorType = Alloc;
            using valueType = typename Alloc::valueType;
            using pointer = detectedOr<valueType*, _pointer, Alloc>;
            using constPointer = detectedOr<typename pointerTraits<pointer>::rebind<const valueType>, _constPointer, Alloc>;
            using voidPointer = detectedOr<typename pointerTraits<pointer>::rebind<void>, _voidPointer, Alloc>;
            using constVoidPointer = detectedOr<typename pointerTraits<pointer>::rebind<const void>, _constVoidPointer, Alloc>;
            using differenceType = detectedOr<typename pointerTraits<pointer>::differenceType, _differenceType, Alloc>;
            using sizeType = detectedOr<std::make_unsigned_t<differenceType>, _sizeType, Alloc>;
        public:
            static pointer allocate(Alloc& alloc, sizeType size) { return alloc.allocate(size); }
            static void deallocate(Alloc& alloc, pointer ptr, sizeType size) { alloc.deallocate(ptr, size); }
            template<typename T, typename... Args> static void construct(Alloc& alloc, T* ptr, Args&&... args) { 
                if constexpr(_has_construct<T, Args...>::value) {
                    alloc.construct(ptr, std::forward<Args>(args)...); 
                } else {
                    constructAt(ptr, std::forward<Args>(args)...);
                }
            }
            template<typename T> static void destroy(Alloc& alloc, T* ptr) { 
                if constexpr(_has_destroy<T>::value) {
                    alloc.destroy(ptr); 
                } else {
                    destroyAt(ptr);
                }
            }
    };
}