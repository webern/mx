// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model; never regenerated.

#pragma once

#include <span>
#include <utility>
#include <vector>

namespace mx::core
{

/// A never-empty collection: the representation of a minOccurs >= 1
/// repeated member, so "zero of a required repeat" is unrepresentable
/// (plan §2.3). The natural zero is one default-constructed item;
/// replacing the items with an empty list repairs back to that.
template <typename T> class OneOrMore
{
  public:
    OneOrMore() : m_items(1)
    {
    }

    explicit OneOrMore(T first)
    {
        m_items.push_back(std::move(first));
    }

    std::span<const T> items() const noexcept
    {
        return m_items;
    }

    const T &front() const noexcept
    {
        return m_items.front();
    }

    void add(T item)
    {
        m_items.push_back(std::move(item));
    }

    /// Deterministic repair: an empty replacement becomes one default item.
    void setItems(std::vector<T> items)
    {
        if (items.empty())
        {
            items.emplace_back();
        }
        m_items = std::move(items);
    }

    bool operator==(const OneOrMore &other) const
    {
        return m_items == other.m_items;
    }

    bool operator!=(const OneOrMore &other) const
    {
        return !(*this == other);
    }

  private:
    std::vector<T> m_items;
};

} // namespace mx::core
