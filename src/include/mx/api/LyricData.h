// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/Id.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"

#include <optional>
#include <string>
#include <vector>

namespace mx
{
namespace api
{
// unspecified -> <syllabic> is omitted (a plain, non-hyphenated lyric word).
// single/begin/end/middle -> <syllabic> is written verbatim.
enum class LyricSyllabic
{
    unspecified,
    single,
    begin,
    end,
    middle
};

// unspecified writes the legacy bare <extend/> form. The other values identify the
// beginning, continuation, or end of a word extension across notes.
enum class LyricExtendType
{
    unspecified,
    start,
    continue_,
    stop
};

// One additional syllabic/text run appended to a <lyric>, joined to the run before it by
// <elision>. A syllable is occasionally written as more than one run - for example a source
// that embeds a non-breaking space or an undertie (‿) inside otherwise ordinary lyric text.
// The common case of a lyric with a single run needs none of this; see LyricData::continuations.
class LyricTextSegment
{
  public:
    LyricSyllabic syllabic{LyricSyllabic::unspecified};
    std::string text;

    // The <elision> element's content, copied verbatim from the character that joins this run
    // to the previous one (commonly U+00A0, U+005F, or U+203F). Absent writes a bare <elision/>
    // (or, if elisionSmufl is set, an <elision smufl="...">).
    std::optional<std::string> elisionText;

    // The canonical SMuFL glyph name for the <elision>'s smufl attribute -- any name starting
    // with "lyrics" is schema-legal (e.g. "lyricsElision", "lyricsElisionWide"), so this is a
    // plain name rather than a closed set. MusicXML only consults it when elisionText is absent
    // or empty, but both may be set at once -- for round-trip fidelity, a source that sets both
    // is preserved as both rather than collapsed to one.
    std::optional<std::string> elisionSmufl;
};

class LyricData
{
  public:
    LyricData()
        : text{}, verseNumber{}, verseName{}, syllabic{LyricSyllabic::unspecified}, hasExtend{false},
          extendType{LyricExtendType::unspecified}, positionData{}, printData{}, id{}
    {
    }

    std::string text;
    std::string verseNumber;
    std::string verseName;
    LyricSyllabic syllabic;

    // Additional runs after the first, each joined to the one before it by <elision>. Empty for
    // an ordinary single-run lyric, which is the common case.
    std::vector<LyricTextSegment> continuations;

    bool hasExtend;
    LyricExtendType extendType;
    PositionData positionData;
    PrintData printData;

    // The <lyric> element's id attribute (see Id.h).
    std::optional<Id> id;
};

MXAPI_EQUALS_BEGIN(LyricTextSegment)
MXAPI_EQUALS_MEMBER(syllabic)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(elisionText)
MXAPI_EQUALS_MEMBER(elisionSmufl)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(LyricTextSegment);

MXAPI_EQUALS_BEGIN(LyricData)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(verseNumber)
MXAPI_EQUALS_MEMBER(verseName)
MXAPI_EQUALS_MEMBER(syllabic)
MXAPI_EQUALS_MEMBER(continuations)
MXAPI_EQUALS_MEMBER(hasExtend)
MXAPI_EQUALS_MEMBER(extendType)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(printData)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(LyricData);
} // namespace api
} // namespace mx
