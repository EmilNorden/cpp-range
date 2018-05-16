#ifndef WHERE_H_
#define WHERE_H_

#include <functional>
#include <iterator>
#include <iostream>

namespace range
{

template<typename TIterator>
class WhereIterator : public std::iterator<std::input_iterator_tag, typename TIterator::value_type>
{
    using TItem = typename TIterator::value_type;
public:
    WhereIterator(TIterator iterator, TIterator iterator_end, std::function<bool(TItem)>& predicate)
        : m_iterator(iterator), m_iterator_end(iterator_end), m_predicate(predicate)
    {
        std::cout << "do we reach this?" << std::endl;
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

template <typename TIterator>
class TakeIterator : public std::iterator<std::input_iterator_tag, typename TIterator::value_type>
{
    using TItem = typename TIterator::value_type;
public:
    TakeIterator(TIterator iterator, TIterator iterator_end, int count)
        : m_iterator(iterator), m_iterator_end(iterator_end), m_count(count)
    {

    }

    friend bool operator==(const TakeIterator& a, const TakeIterator& b) 
    {
        return a.m_iterator == b.m_iterator;
    }

    friend bool operator!=(const TakeIterator& a, const TakeIterator& b)
    {
        return a.m_iterator != b.m_iterator;
    }

    TItem& operator*() const { return *m_iterator; }
private:
    TIterator m_iterator;
    const TIterator m_iterator_end;
    int m_count;
};


template <typename TContainer>
class WhereEnumerable
{
    using TItem = typename TContainer::value_type;
    using TInnerIterator = typename TContainer::iterator;
public:
    using value_type = typename TContainer::value_type;
    using iterator = WhereIterator<TInnerIterator>;
    WhereEnumerable(TContainer& container, const std::function<bool(TItem)>& predicate)
        : m_container(container), m_predicate(predicate)
    {

    }

    iterator begin()
    {
        return WhereIterator<TInnerIterator>(std::begin(m_container), std::end(m_container), m_predicate);
    }

    iterator end()
    {
        return WhereIterator<TInnerIterator>(std::end(m_container), std::end(m_container), m_predicate);
    }

private:
    TContainer& m_container;
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
    TakeEnumerable(TContainer& container, int count)
        : m_container(container), m_count(count)
    {

    }

    iterator begin()
    {
        return std::begin(m_container);
    }

    iterator end()
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
    TContainer& m_container;
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
    SkipEnumerable(TContainer& container, int count)
        : m_container(container), m_count(count)
    {

    }

    iterator begin()
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

    iterator end()
    {
        return std::end(m_container);
    }
private:
    TContainer& m_container;
    int m_count;
};

template <typename TContainer>
const WhereEnumerable<TContainer> where(TContainer& container, std::function<bool(typename TContainer::value_type)>&& predicate)
{
    return WhereEnumerable(container, predicate);
}

template <typename TContainer>
const TakeEnumerable<TContainer> take(TContainer& container, int count)
{
    return TakeEnumerable(container, count);
}

template <typename TContainer>
const SkipEnumerable<TContainer> skip(TContainer& container, int count)
{
    return SkipEnumerable(container, count);
}

template <typename TContainer>
class Range
{
public:
    using value_type = typename TContainer::value_type;
    using iterator = typename TContainer::iterator;

    explicit Range(TContainer& container)
        : m_container(container)
    {

    }

    explicit Range(TContainer&& container)
        : m_container(container)
    {
        std::cout << "invoked rvalue ref constructor" << std::endl;
    }

    iterator begin()
    {
        return std::begin(m_container);
    }

    iterator end()
    {
        return std::end(m_container);
    }

    Range<WhereEnumerable<TContainer>> where(std::function<bool(typename TContainer::value_type)>&& predicate) const
    {
        return Range<WhereEnumerable<TContainer>>(WhereEnumerable<TContainer>(m_container, predicate));
    }

    Range<WhereEnumerable<TContainer>> where2(std::function<bool(typename TContainer::value_type)>& predicate) const
    {
        return Range<WhereEnumerable<TContainer>>(WhereEnumerable<TContainer>(m_container, predicate));
    }

    Range<Range<TContainer>> foo()
    {
        return Range<Range<TContainer>>{Range<TContainer>{m_container}};
    }
    
private:
    TContainer& m_container;
};

template <typename TContainer>
const Range<TContainer> from(TContainer& container)
{
    return Range(container);
}   

}



#endif
