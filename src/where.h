#ifndef WH_H_
#define WH_H_

#include <functional>
#include <iterator>

namespace range
{

template<typename TIterator>
class WhereIterator : public std::iterator<std::input_iterator_tag, typename TIterator::value_type>
{
    using TItem = typename TIterator::value_type;
public:
    WhereIterator(TIterator iterator, const TIterator iterator_end, const std::function<bool(TItem)>& predicate)
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
    const TIterator m_iterator_end;
    const std::function<bool(TItem)>& m_predicate;
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

    WhereIterator<TInnerIterator> begin()
    {
        return WhereIterator<TInnerIterator>(m_container.begin(), m_container.end(), m_predicate);
    }

    WhereIterator<TInnerIterator> end()
    {
        return WhereIterator<TInnerIterator>(m_container.end(), m_container.end(), m_predicate);
    }

private:
    TContainer& m_container;
    const std::function<bool(TItem)>& m_predicate;
};

template <typename TContainer>
const WhereEnumerable<TContainer> where(TContainer& container, std::function<bool(typename TContainer::value_type)>&& predicate)
{
    return WhereEnumerable(container, predicate);
}

}

#endif
