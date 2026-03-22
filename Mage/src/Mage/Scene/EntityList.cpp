#include "EntityList.h"

namespace Mage {

    EntityList::Iterator::Iterator(pointer ptr)
        : ptr(ptr)
    {
    }

    EntityList::Iterator::reference EntityList::Iterator::operator*() const
    {
        return *ptr;
    }

    EntityList::Iterator::pointer EntityList::Iterator::operator->() const
    {
        return ptr;
    }

    EntityList::Iterator& EntityList::Iterator::operator++()
    {
        ptr++;
        return *this;
    }

    EntityList::Iterator EntityList::Iterator::operator++(int)
    {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const EntityList::Iterator& a, const EntityList::Iterator& b)
    {
        return a.ptr == b.ptr;
    }

    bool operator!=(const EntityList::Iterator& a, const EntityList::Iterator& b)
    {
        return a.ptr != b.ptr;
    }

    EntityList::ConstIterator::ConstIterator(pointer ptr)
        : ptr(ptr)
    {
    }

    const EntityList::ConstIterator::reference EntityList::ConstIterator::operator*() const
    {
        return *ptr;
    }

    const EntityList::ConstIterator::pointer EntityList::ConstIterator::operator->() const
    {
        return ptr;
    }

    EntityList::ConstIterator& EntityList::ConstIterator::operator++()
    {
        ptr++;
        return *this;
    }

    EntityList::ConstIterator EntityList::ConstIterator::operator++(int)
    {
        ConstIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const EntityList::ConstIterator& a, const EntityList::ConstIterator& b)
    {
        return a.ptr == b.ptr;
    }

    bool operator!=(const EntityList::ConstIterator& a, const EntityList::ConstIterator& b)
    {
        return a.ptr != b.ptr;
    }

    EntityList::EntityList(Entity** list, size_t count)
        : list(list)
        , count(count)
    {
    }

    Entity* EntityList::at(size_t pos)
    {
        return list[pos];
    }

    const Entity* EntityList::at(size_t pos) const
    {
        return list[pos];
    }

    Entity* EntityList::operator[](size_t pos)
    {
        return list[pos];
    }

    const Entity* EntityList::operator[](size_t pos) const
    {
        return list[pos];
    }

    Entity* EntityList::front()
    {
        return list[0];
    }

    const Entity* EntityList::front() const
    {
        return list[0];
    }

    Entity* EntityList::back()
    {
        return list[count - 1];
    }

    const Entity* EntityList::back() const
    {
        return list[count - 1];
    }

    Entity** EntityList::data()
    {
        return list;
    }

    const Entity** EntityList::data() const
    {
        return const_cast<const Entity**>(list);
    }

    bool EntityList::empty() const
    {
        return count <= 0;
    }

    size_t EntityList::size() const
    {
        return count;
    }

    EntityList::Iterator EntityList::begin()
    {
        if (count < 1)
            return Iterator(nullptr);
        return Iterator(&list[0]);
    }

    EntityList::Iterator EntityList::end()
    {
        if (count < 1)
            return Iterator(nullptr);
        return Iterator(&list[count]);
    }

    EntityList::ConstIterator EntityList::cbegin() const
    {
        if (count < 1)
            return ConstIterator(nullptr);
        return ConstIterator(&list[0]);
    }

    EntityList::ConstIterator EntityList::cend() const
    {
        if (count < 1)
            return ConstIterator(nullptr);
        return ConstIterator(&list[count]);
    }
}
