#ifndef RANGE_X_H
#define RANGE_X_H

#include <functional>
#include <iterator>
#include <map>
#include <vector>

#include "print.h"

namespace range
{

template <typename TIterator>
class GroupIterator : public std::iterator<std::input_iterator_tag, typename TIterator::value_type>
{
public:
    GroupIterator(TIterator iterator, TIterator iterator_end)
        : m_iterator(iterator), m_iterator_end(iterator_end)
    {

    }

    friend bool operator==(const GroupIterator &a, const GroupIterator &b)
    {
        return a.m_iterator == b.m_iterator;
    }

    friend bool operator!=(const GroupIterator &a, const GroupIterator &b)
    {
        return a.m_iterator != b.m_iterator;
    }

    auto key() const -> typename TIterator::value_type::first_type { return m_iterator->first; }
    // const typename TIterator::value_type& operator*() const { return m_iterator->second; }
    auto operator*() const -> typename TIterator::value_type::second_type { return m_iterator->second; }

    GroupIterator &operator++()
    {
        ++m_iterator;
        return *this;
    }

    GroupIterator operator++(int)
    {
        GroupIterator tmp = *this;
        ++m_iterator;
        return tmp;
    }
private:
    TIterator m_iterator;
    TIterator m_iterator_end;
};

template <typename TIterator>
class WhereIterator : public std::iterator<std::input_iterator_tag, typename TIterator::value_type>
{
    using TItem = typename TIterator::value_type;

  public:
    WhereIterator(TIterator iterator, TIterator iterator_end, const std::function<bool(TItem)> &predicate)
        : m_iterator(iterator), m_iterator_end(iterator_end), m_predicate(predicate)
    {
        if (m_iterator != m_iterator_end &&
            !m_predicate(*m_iterator))
        {
            this->operator++();
        }
    }

    friend bool operator==(const WhereIterator &a, const WhereIterator &b)
    {
        return a.m_iterator == b.m_iterator;
    }

    friend bool operator!=(const WhereIterator &a, const WhereIterator &b)
    {
        return a.m_iterator != b.m_iterator;
    }

    TItem &operator*() const { return *m_iterator; }

    WhereIterator &operator++()
    {
        do
        {
            ++m_iterator;
        } while (m_iterator != m_iterator_end && !m_predicate(*m_iterator));
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
    WhereEnumerable(TContainer &container, const std::function<bool(TItem)> &predicate)
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
    TContainer m_container;
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
    TakeEnumerable(TContainer &container, int count)
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
        while (count > 0 && it != end)
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
    SkipEnumerable(TContainer &container, int count)
        : m_container(container), m_count(count)
    {
    }

    iterator begin()
    {
        auto end = std::end(m_container);
        auto it = std::begin(m_container);
        int count = m_count;
        while (count > 0 && it != end)
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
    TContainer m_container;
    int m_count;
};

template <typename TContainer, typename GroupType>
class GroupEnumerable
{
    using TItem = typename TContainer::value_type;
    using TInnerIterator = typename std::map<GroupType, std::vector<TItem>>::iterator;

  public:
    using value_type = typename TContainer::value_type;
    using iterator = GroupIterator<TInnerIterator>;

    GroupEnumerable(TContainer &container, std::function<GroupType(value_type&)> group_function)
        : m_container(container), m_group_function(group_function)
    {
    }

    iterator begin()
    {
        m_map.clear();
        for(auto item : m_container)
        {
            //TODO: Skapa en egen typ att använda istället för vector, som kan hålla key() också
            m_map[m_group_function(item)].push_back(item);
        }

        return GroupIterator<TInnerIterator>(std::begin(m_map), std::end(m_map));
    }

    iterator end()
    {
        return GroupIterator<TInnerIterator>(std::end(m_map), std::end(m_map));
    }


  private:
    TContainer m_container;
    std::function<GroupType(value_type&)> m_group_function;
    std::map<GroupType, std::vector<TItem>> m_map;
};

template <typename T>
struct AsFunction : public AsFunction<decltype(&T::operator())>
{
};

template <typename ReturnType, typename... Args>
struct AsFunction<ReturnType(Args...)>
{
    using type = std::function<ReturnType(Args...)>;
};

template <typename ReturnType, typename... Args>
struct AsFunction<ReturnType (*)(Args...)>
{
    using type = std::function<ReturnType(Args...)>;
};

template <typename Class, typename ReturnType, typename... Args>
struct AsFunction<ReturnType (Class::*)(Args...) const>
{
    using type = std::function<ReturnType(Args...)>;
};

template <typename F>
constexpr auto toFunction(F f) -> typename AsFunction<F>::type
{
    return {f};
}

template <typename TContainer>
class Range
{
  public:
    using value_type = typename TContainer::value_type;
    using iterator = typename TContainer::iterator;

    explicit Range(TContainer &&container)
        : m_container(container)
    {
    }

    iterator begin()
    {
        return std::begin(m_container);
    }

    iterator end()
    {
        return std::end(m_container);
    }

    Range<WhereEnumerable<TContainer>> where(std::function<bool(typename TContainer::value_type)> &&predicate)
    {
        return Range<WhereEnumerable<TContainer>>(WhereEnumerable<TContainer>(m_container, predicate));
    }

    Range<SkipEnumerable<TContainer>> skip(int count)
    {
        return Range<SkipEnumerable<TContainer>>(SkipEnumerable<TContainer>(m_container, count));
    }

    Range<TakeEnumerable<TContainer>> take(int count)
    {
        return Range<TakeEnumerable<TContainer>>(TakeEnumerable<TContainer>(m_container, count));
    }

  private:
    TContainer m_container;
};

template <typename TContainer>
class RefRange
{
  private:
    template <typename GroupType>
    Range<GroupEnumerable<RefRange<TContainer>, GroupType>> group_internal(std::function<GroupType(typename TContainer::value_type&)> predicate)
    {
        return Range<GroupEnumerable<RefRange<TContainer>, GroupType>>(GroupEnumerable<RefRange<TContainer>, GroupType>(*this, predicate));
    }

    TContainer &m_container;

  public:
    using value_type = typename TContainer::value_type;
    using iterator = typename TContainer::iterator;

    explicit RefRange(TContainer &container)
        : m_container(container)
    {
    }

    iterator begin()
    {
        return std::begin(m_container);
    }

    iterator end()
    {
        return std::end(m_container);
    }

    Range<WhereEnumerable<RefRange<TContainer>>> where(std::function<bool(typename TContainer::value_type)> &&predicate)
    {
        return Range<WhereEnumerable<RefRange<TContainer>>>(WhereEnumerable<RefRange<TContainer>>(*this, predicate));
    }

    Range<SkipEnumerable<RefRange<TContainer>>> skip(int count)
    {
        return Range<SkipEnumerable<RefRange<TContainer>>>(SkipEnumerable<RefRange<TContainer>>(*this, count));
    }

    Range<TakeEnumerable<RefRange<TContainer>>> take(int count)
    {
        return Range<TakeEnumerable<RefRange<TContainer>>>(TakeEnumerable<RefRange<TContainer>>(*this, count));
    }

    template <typename Function>
    auto group(Function &&function) -> decltype(group_internal(toFunction(function))) 
    {
        return group_internal(toFunction(function));
    }
};

template <typename TContainer>
RefRange<TContainer> from(TContainer &container)
{
    return RefRange<TContainer>{container};
}
} // namespace range

#endif
