#pragma once

#include "memory.hpp"
#include "iterator.hpp"
#include <initializer_list>

namespace goose {
    template<typename T, typename Alloc = allocator<T>>
    struct vector {
        private:
            using myAllocTraits = allocatorTraits<Alloc>;
            static constexpr size_t growthFactor = 2;
        public:
            using valueType = T;
            using allocatorType = Alloc;
            using sizeType = size_t;
            using differenceType = std::ptrdiff_t;
            using reference = valueType&;
            using constReference = const valueType&;
            using pointer = typename myAllocTraits::pointer;
            using constPointer = typename myAllocTraits::constPointer;
            using iterator = valueType*;
            using constIterator = valueType*;
            using reverseIterator = goose::reverseIterator<iterator>;
            using constReverseIterator = goose::reverseIterator<constIterator>;
        public:
            vector() = default;
            vector(const Alloc& alloc) : mAlloc{alloc} {}

            vector(sizeType count, const T& value, const Alloc& alloc = Alloc()) : mAlloc{alloc}, mCap{count}, mSize{count} {
                mElems = myAllocTraits::allocate(mAlloc, mCap);
                for (size_t i{}; i < mSize; ++i) {
                    myAllocTraits::construct(mAlloc, mElems + i, value);
                }
            }

            vector(sizeType count, const Alloc& alloc = Alloc()) : mAlloc{alloc}, mCap{count}, mSize{count} {
                mElems = myAllocTraits::allocate(mAlloc, mCap);
                for (size_t i{}; i < mSize; ++i) {
                    myAllocTraits::construct(mAlloc, mElems + i);
                }
            }

            template<typename InputIt>
            vector(InputIt first, InputIt last, const Alloc& alloc = Alloc()) : mAlloc{alloc} {
                mCap = last - first;
                mSize = mCap;
                mElems = myAllocTraits::allocate(mAlloc, mCap);
                for (sizeType index{}; first != last; ++first, ++index) {
                    myAllocTraits::construct(mAlloc, mElems + index, *first);

                }
            }

            vector(const vector& other) : mCap{other.mCap}, mSize{other.mSize}, mAlloc{other.mAlloc} {
               mElems = myAllocTraits::allocate(mAlloc, mCap);
               sizeType index{};
               for (auto &item : other) {
                   myAllocTraits::construct(mAlloc, mElems + index, item);
                   ++index;
               }
            }

            vector(const vector& other, const Alloc& alloc) : mCap{other.mCap}, mSize{other.mSize}, mAlloc{alloc} {
               mElems = myAllocTraits::allocate(mAlloc, mCap);
               sizeType index{};
               for (auto &item : other) {
                   myAllocTraits::construct(mAlloc, mElems + index, item);
                   ++index;
               }
            }

            vector(vector&& other) : mCap{exchange(other.mCap, 0)}, mSize{exchange(other.mSize, 0)}, mAlloc{std::move(other.mAlloc)} {
               mElems = myAllocTraits::allocate(mAlloc, mCap);
               sizeType index{};
               for (auto &item : other) {
                   myAllocTraits::construct(mAlloc, mElems + index, std::move(item));
                   ++index;
               }
            }

            vector(vector&& other, const Alloc& alloc) : mCap{exchange(other.mCap, 0)}, mSize{exchange(other.mSize, 0)}, mAlloc{alloc} {
               mElems = myAllocTraits::allocate(mAlloc, mCap);
               sizeType index{};
               for (auto &item : other) {
                   myAllocTraits::construct(mAlloc, mElems + index, std::move(item));
                   ++index;
               }
            }

            vector(std::initializer_list<T> list, const Alloc& alloc = Alloc()) : 
                mAlloc{alloc}, mCap{list.size()}, mSize{list.size()} {
                    mElems = myAllocTraits::allocate(mAlloc, list.size());
                    size_t index = 0;
                    for(auto &item : list) {
                        myAllocTraits::construct(mAlloc, mElems + index, item);
                        ++index;
                    }
            }

            ~vector() {
                for(size_t i{}; i < mSize; ++i) myAllocTraits::destroy(mAlloc, mElems + i);
                myAllocTraits::deallocate(mAlloc, mElems, mCap);
            }

        public:
            iterator begin() { return mElems; }
            constIterator begin() const { return cbegin(); }
            constIterator cbegin() const { return mElems; }
            iterator end() { return mElems + mSize; }
            constIterator end() const { return cend(); }
            constIterator cend() const { return mElems + mSize; }
        public:
            sizeType size() const { return mSize; }
            sizeType capacity() const { return mCap; }
            bool empty() const { return mSize == 0; }
        public:
            void swap(const vector& other) {

            }
        private:
            T* mElems;
            sizeType mSize;
            sizeType mCap;
            Alloc mAlloc;
    };
    
} 
