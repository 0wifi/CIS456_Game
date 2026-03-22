#include "SystemList.h"

namespace Mage {

    struct SystemList::Impl
    {
        System** list;
        size_t   count;
    };

    struct SystemList::Iterator::Impl
    {
        pointer ptr;
    };

    struct SystemList::ConstIterator::Impl
    {
        pointer ptr;
    };

    SystemList::Iterator::Iterator(pointer ptr)
    {
        _impl = new Impl();
        _impl->ptr = ptr;
    }

    SystemList::Iterator::~Iterator()
    {
        delete _impl;
    }

    SystemList::Iterator::reference SystemList::Iterator::operator*() const
    {
        return *_impl->ptr;
    }

    SystemList::Iterator::pointer SystemList::Iterator::operator->() const
    {
        return _impl->ptr;
    }

    SystemList::Iterator& SystemList::Iterator::operator++()
    {
        _impl->ptr++;
        return *this;
    }

    SystemList::Iterator SystemList::Iterator::operator++(int)
    {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const SystemList::Iterator& a, const SystemList::Iterator& b)
    {
        return a._impl->ptr == b._impl->ptr;
    }

    bool operator!=(const SystemList::Iterator& a, const SystemList::Iterator& b)
    {
        return a._impl->ptr != b._impl->ptr;
    }

    SystemList::ConstIterator::ConstIterator(pointer ptr)
    {
        _impl = new Impl();
        _impl->ptr = ptr;
    }

    SystemList::ConstIterator::~ConstIterator()
    {
        delete _impl;
    }

    const SystemList::ConstIterator::reference SystemList::ConstIterator::operator*() const
    {
        return *_impl->ptr;
    }

    const SystemList::ConstIterator::pointer SystemList::ConstIterator::operator->() const
    {
        return _impl->ptr;
    }

    SystemList::ConstIterator& SystemList::ConstIterator::operator++()
    {
        _impl->ptr++;
        return *this;
    }

    SystemList::ConstIterator SystemList::ConstIterator::operator++(int)
    {
        ConstIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const SystemList::ConstIterator& a, const SystemList::ConstIterator& b)
    {
        return a._impl->ptr == b._impl->ptr;

    }

    bool operator!=(const SystemList::ConstIterator& a, const SystemList::ConstIterator& b)
    {
        return a._impl->ptr != b._impl->ptr;
    }

    SystemList::SystemList(System** list, size_t count)
    {
        _impl = new Impl();
        _impl->list = list;
        _impl->count = count;
    }

    SystemList::~SystemList()
    {
        delete _impl;
    }

    System* SystemList::at(size_t pos)
    {
        return _impl->list[pos];
    }

    const System* SystemList::at(size_t pos) const
    {
        return _impl->list[pos];
    }

    System* SystemList::operator[](size_t pos)
    {
        return _impl->list[pos];
    }

    const System* SystemList::operator[](size_t pos) const
    {
        return _impl->list[pos];
    }

    System* SystemList::front()
    {
        return _impl->list[0];
    }

    const System* SystemList::front() const
    {
        return _impl->list[0];
    }

    System* SystemList::back()
    {
        return _impl->list[_impl->count - 1];
    }

    const System* SystemList::back() const
    {
        return _impl->list[_impl->count - 1];
    }

    System** SystemList::data()
    {
        return _impl->list;
    }

    const System** SystemList::data() const
    {
        return const_cast<const System**>(_impl->list);
    }

    bool SystemList::empty() const
    {
        return _impl->count <= 0;
    }

    size_t SystemList::size() const
    {
        return _impl->count;
    }

    SystemList::Iterator SystemList::begin()
    {
        return Iterator(&_impl->list[0]);
    }

    SystemList::Iterator SystemList::end()
    {
        return Iterator(&_impl->list[_impl->count]);
    }

    SystemList::ConstIterator SystemList::cbegin() const
    {
        return ConstIterator(&_impl->list[0]);
    }

    SystemList::ConstIterator SystemList::cend() const
    {
        return ConstIterator(&_impl->list[_impl->count]);
    }
}
