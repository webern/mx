// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/SymbolData.h"
#include "mx/api/WordsData.h"

#include <variant>

namespace mx
{
namespace api
{
// One item in a direction's text run: either a span of words or a single musical symbol.
// MusicXML lets a <direction-type> hold an ordered mix of <words> and <symbol> elements that
// reads as one visual sequence -- "gliss." followed by an arrow glyph, "più" followed by a
// forte glyph. WordsChoice models one element of that sequence; the whole ordered run is a
// DirectionChoice wordsRun (std::vector<WordsChoice>).
//
// Defaults to words, empty.
class WordsChoice
{
  public:
    enum class Kind
    {
        words,
        symbol
    };

    WordsChoice();

    WordsChoice(WordsData value);

    WordsChoice(SymbolData value);

    Kind kind() const;
    bool isWords() const;
    bool isSymbol() const;

    // Returns a copy of the internally held WordsData.
    //
    // Check isWords() first. If this is not a words item, a default constructed WordsData is
    // returned.
    const WordsData words() const;

    // Returns a copy of the internally held SymbolData.
    //
    // Check isSymbol() first. If this is not a symbol item, a default constructed SymbolData is
    // returned.
    const SymbolData symbol() const;

    bool operator==(const WordsChoice &other) const;

  private:
    std::variant<WordsData, SymbolData> myValue;
};

MXAPI_NOT_EQUALS_AND_VECTORS(WordsChoice);

} // namespace api
} // namespace mx
