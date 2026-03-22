// ReSharper disable CppInconsistentNaming
#pragma once

#include "../Core/Core.h"
#include "Entity.h"

namespace Mage {

    class MAGE_API EntityList
    {
    public:
        struct MAGE_API Iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = Entity*;
            using pointer = Entity**;
            using reference = Entity*&;

            explicit Iterator(pointer ptr);

            Iterator(Iterator&&) = default;
            Iterator(Iterator const&) = default;
            Iterator& operator=(Iterator&&) = default;
            Iterator& operator=(Iterator const&) = default;

            ~Iterator() = default;

            reference operator*() const;
            pointer  operator->() const;

            Iterator& operator++();
            Iterator operator++(int);

            friend bool MAGE_API operator==(const Iterator& a, const Iterator& b);
            friend bool MAGE_API operator!=(const Iterator& a, const Iterator& b);

        private:
            pointer ptr = nullptr;
        };

        struct MAGE_API ConstIterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = Entity*;
            using pointer = Entity**;
            using reference = Entity*&;

            explicit ConstIterator(pointer ptr);

            ConstIterator(ConstIterator&&) = default;
            ConstIterator(ConstIterator const&) = default;
            ConstIterator& operator=(ConstIterator&&) = default;
            ConstIterator& operator=(ConstIterator const&) = default;

            ~ConstIterator() = default;

            const reference operator*() const;
            const pointer  operator->() const;

            ConstIterator& operator++();
            ConstIterator operator++(int);

            friend bool MAGE_API operator==(const ConstIterator& a, const ConstIterator& b);
            friend bool MAGE_API operator!=(const ConstIterator& a, const ConstIterator& b);

        private:
            pointer ptr = nullptr;

        };

        EntityList(Entity** list, size_t count);

        ~EntityList() = default;

        EntityList(EntityList&&) = delete;
        EntityList(EntityList const&) = delete;
        EntityList& operator=(EntityList&&) = delete;
        EntityList& operator=(EntityList const&) = delete;

        Entity* at(size_t pos);
        const Entity* at(size_t pos) const;

        Entity* operator[](size_t pos);
        const Entity* operator[](size_t pos) const;

        Entity* front();
        const Entity* front() const;

        Entity* back();
        const Entity* back() const;

        Entity** data();
        const Entity** data() const;

        bool empty() const;

        size_t size() const;

        Iterator begin();
        Iterator end();

        ConstIterator cbegin() const;
        ConstIterator cend() const;

    private:
        Entity** list;
        size_t   count;

    };

}
