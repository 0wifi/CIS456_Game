// ReSharper disable CppInconsistentNaming
#pragma once

#include "../Core/Core.h"
#include "System.h"

namespace Mage {

    class MAGE_API SystemList
    {
        friend class SystemManager;

    public:
        struct MAGE_API Iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = System*;
            using pointer = System**;
            using reference = System*&;

            explicit Iterator(pointer ptr);

            Iterator(Iterator&&) = default;
            Iterator(Iterator const&) = default;
            Iterator& operator=(Iterator&&) = default;
            Iterator& operator=(Iterator const&) = default;

            ~Iterator();

            reference operator*() const;
            pointer  operator->() const;

            Iterator& operator++();
            Iterator operator++(int);

            friend bool operator==(const Iterator& a, const Iterator& b);
            friend bool operator!=(const Iterator& a, const Iterator& b);

        private:
            struct Impl;
            Impl* _impl;
        };

        struct MAGE_API ConstIterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = System*;
            using pointer = System**;
            using reference = System*&;

            explicit ConstIterator(pointer ptr);

            ConstIterator(ConstIterator&&) = default;
            ConstIterator(ConstIterator const&) = default;
            ConstIterator& operator=(ConstIterator&&) = default;
            ConstIterator& operator=(ConstIterator const&) = default;

            ~ConstIterator();

            const reference operator*() const;
            const pointer  operator->() const;

            ConstIterator& operator++();
            ConstIterator operator++(int);

            friend bool operator==(const ConstIterator& a, const ConstIterator& b);
            friend bool operator!=(const ConstIterator& a, const ConstIterator& b);

        private:
            struct Impl;
            Impl* _impl;

        };

        ~SystemList();

        SystemList(SystemList&&) = delete;
        SystemList(SystemList const&) = delete;
        SystemList& operator=(SystemList&&) = delete;
        SystemList& operator=(SystemList const&) = delete;

        System* at(size_t pos);
        const System* at(size_t pos) const;

        System* operator[](size_t pos);
        const System* operator[](size_t pos) const;

        System* front();
        const System* front() const;

        System* back();
        const System* back() const;

        System** data();
        const System** data() const;

        bool empty() const;

        size_t size() const;

        Iterator begin();
        Iterator end();

        ConstIterator cbegin() const;
        ConstIterator cend() const;

    private:
        SystemList(System** list, size_t count);

        struct Impl;
        Impl* _impl;

    };

}
