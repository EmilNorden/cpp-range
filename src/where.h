#ifndef WHERE_H_
#define WHERE_H_

#include <functional>
#include <iterator>

#include "print.h"

namespace range
{

template<typename TIterator>
class WhereIterator : public std::iterator<std::input_iterator_tag, typename TIterator::value_type>
{
    using TItem = typename TIterator::value_type;
public:
    WhereIterator(TIterator iterator, TIterator iterator_end, const std::function<bool(TItem)>& predicate)
        : m_iterator(iterator), m_iterator_end(iterator_end), m_predicate(predicate)
    {
        if(m_iterator != m_iterator_end &&
            !m_predicate(*m_iterator))
        {
            this->operator++();
        }
    }

    friend bool operator==(const WhereIterator& a, const WhereIterator& b) 
    {
        return a.m_iterator == b.m_iterator;
    }

    friend bool operator!=(const WhereIterator& a, const WhereIterator& b)
    {
        return a.m_iterator != b.m_iterator;
    }

    TItem& operator*() const { return *m_iterator; }

    WhereIterator& operator++()
    {
        do
        {
            ++m_iterator;
        }while(m_iterator != m_iterator_end && !m_predicate(*m_iterator));
        return *this;
    }

    WhereIterator operator++(int)
    {
        WhereIterator tmp = *this;
        ++m_iterator;
        return tmp;
    }
private:
    TIterator m_iterator;
    TIterator m_iterator_end;
    std::function<bool(TItem)> m_predicate;
};

template <typename TContainer>
class WhereEnumerable
{
    using TItem = typename TContainer::value_type;
    using TInnerIterator = typename TContainer::iterator;
public:
    using value_type = typename TContainer::value_type;
    using iterator = WhereIterator<TInnerIterator>;
    WhereEnumerable(const TContainer& container, const std::function<bool(TItem)>& predicate)
        : m_container(container), m_predicate(predicate)
    {

    }

    iterator begin() const
    {
        return WhereIterator<TInnerIterator>(std::begin(m_container), std::end(m_container), m_predicate);
    }

    iterator end() const
    {
        return WhereIterator<TInnerIterator>(std::end(m_container), std::end(m_container), m_predicate);
    }

private:
    const TContainer m_container;
    std::function<bool(TItem)> m_predicate;
};

template <typename TContainer>
class TakeEnumerable
{
    using TItem = typename TContainer::value_type;
    using TInnerIterator = typename TContainer::iterator;
public:
    using value_type = typename TContainer::value_type;
    using iterator = TInnerIterator;
    TakeEnumerable(const TContainer& container, int count)
        : m_container(container), m_count(count)
    {

    }

    iterator begin() const
    {
        return std::begin(m_container);
    }

    iterator end() const
    {
        auto end = std::end(m_container);
        auto it = begin();
        int count = m_count;
        while(count > 0 && it != end)
        {
            --count;
            ++it;
        }
        return it;
    }
private:
    TContainer m_container;
    int m_count;
};

template <typename TContainer>
class SkipEnumerable
{
    using TItem = typename TContainer::value_type;
    using TInnerIterator = typename TContainer::iterator;
public:
    using value_type = typename TContainer::value_type;
    using iterator = TInnerIterator;
    SkipEnumerable(const TContainer& container, int count)
        : m_container(container), m_count(count)
    {

    }

    iterator begin() const
    {
        auto end = std::end(m_container);
        auto it = std::begin(m_container);
        int count = m_count;
        while(count > 0 && it != end)
        {
            --count;
            ++it;
        }
        return it;
    }

    iterator end() const
    {
        return std::end(m_container);
    }
private:
    const TContainer m_container;
    int m_count;
};

template <typename TContainer>
class Range
{
public:
    using value_type = typename TContainer::value_type;
    using iterator = typename TContainer::iterator;

    explicit Range(TContainer&& container)
        : m_container(container)
    {
    }   

    iterator begin() const
    {
        return std::begin(m_container);
    }

    iterator end() const
    {
        return std::end(m_container);
    }

    Range<WhereEnumerable<TContainer>> where(std::function<bool(typename TContainer::value_type)>&& predicate) const
    {
        return Range<WhereEnumerable<TContainer>>(WhereEnumerable<TContainer>(m_container, predicate));
    }

    Range<SkipEnumerable<TContainer>> skip(int count) const
    {
        return Range<SkipEnumerable<TContainer>>(SkipEnumerable<TContainer>(m_container, count));
    }

    Range<TakeEnumerable<TContainer>> take(int count) const
    {
        return Range<TakeEnumerable<TContainer>>(TakeEnumerable<TContainer>(m_container, count));
    }
    
private:
    TContainer m_container;
};

template <typename TContainer>
class RefRange
{
public:
    using value_type = typename TContainer::value_type;
    using iterator = typename TContainer::iterator;

    explicit RefRange(TContainer& container)
        : m_container(container)
    {

    }

    iterator begin() const
    {
        return std::begin(m_container);
    }

    iterator end() const
    {
        return std::end(m_container);
    }

    Range<WhereEnumerable<RefRange<TContainer>>> where(std::function<bool(typename TContainer::value_type)>&& predicate) const
    {
        return Range<WhereEnumerable<RefRange<TContainer>>>(WhereEnumerable<RefRange<TContainer>>(*this, predicate));
    }

    Range<SkipEnumerable<RefRange<TContainer>>> skip(int count) const
    {
        return Range<SkipEnumerable<RefRange<TContainer>>>(SkipEnumerable<RefRange<TContainer>>(*this, count));
    }
    
    Range<TakeEnumerable<RefRange<TContainer>>> take(int count) const
    {
        return Range<TakeEnumerable<RefRange<TContainer>>>(TakeEnumerable<RefRange<TContainer>>(*this, count));
    }
private:
    TContainer& m_container;
};

template <typename TContainer>
const RefRange<TContainer> from(TContainer& container)
{
    return RefRange<TContainer>{container};
}   

}



#endif
