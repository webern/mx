// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/WordsChoice.h"

#include <utility>

namespace mx
{
namespace api
{

WordsChoice::WordsChoice() : myValue{WordsData{}}
{
}

WordsChoice::WordsChoice(WordsData value) : myValue{std::move(value)}
{
}

WordsChoice::WordsChoice(SymbolData value) : myValue{std::move(value)}
{
}

WordsChoice::Kind WordsChoice::kind() const
{
    return isSymbol() ? Kind::symbol : Kind::words;
}

bool WordsChoice::isWords() const
{
    return std::holds_alternative<WordsData>(myValue);
}

bool WordsChoice::isSymbol() const
{
    return std::holds_alternative<SymbolData>(myValue);
}

const WordsData WordsChoice::words() const
{
    if (const auto *value = std::get_if<WordsData>(&myValue))
    {
        return *value;
    }
    return WordsData{};
}

const SymbolData WordsChoice::symbol() const
{
    if (const auto *value = std::get_if<SymbolData>(&myValue))
    {
        return *value;
    }
    return SymbolData{};
}

bool WordsChoice::operator==(const WordsChoice &other) const
{
    return myValue == other.myValue;
}

} // namespace api
} // namespace mx
