// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/PositionData.h"

#include <optional>
#include <string>
#include <vector>

namespace mx
{
namespace api
{
enum class BracketType
{
    unspecified,
    none,
    brace,
    line,
    bracket,
    square
};

// Whether the group should have common barlines, mirroring the MusicXML
// <group-barline> element. `unspecified` means the source carried no
// <group-barline> (and none is written back).
enum class GroupBarline
{
    unspecified,
    yes,
    no,
    mensurstrich
};

// The part-group element indicates groupings of parts in the score, usually indicated
// by braces and brackets. Braces that are used for multi-staff parts should be defined
// in the attributes element for that part. The part-group start element appears before
// the first score-part in the group. The part-group stop element appears after the
// last score-part in the group.

// The number attribute is used to distinguish overlapping and nested part-groups, not
// the sequence of groups. As with parts, groups can have a name and abbreviation.
// Values for the child elements are ignored at the stop of a group.

// A part-group element is not needed for a single multi-staff part. By default, multi-
// staff parts include a brace symbol and (if appropriate given the bar-style) common
// barlines. The symbol formatting for a multi-staff part can be more fully specified
// using the part-symbol element.

class PartGroupData
{
  public:
    int firstPartIndex;
    int lastPartIndex;
    std::string name;
    std::string displayName;
    std::string abbreviation;
    std::string displayAbbreviation;
    BracketType bracketType;
    // Where the bracket or brace is drawn, in tenths of a staff space. defaultX moves it left or
    // right of the start of the system, so a negative value pulls it out into the left margin.
    // defaultY moves it up or down from the top line of the group's first staff. relativeX and
    // relativeY shift it from wherever it would otherwise be drawn. Set the matching
    // is...Specified flag along with each value. A group symbol does not have a placement or an
    // alignment, so those members are not written. Leave this alone to let the program place the
    // symbol. Used only when bracketType is set.
    PositionData bracketPositionData;
    // The color of the bracket or brace. Leave it empty to draw it in the score's usual color. Used
    // only when bracketType is set.
    std::optional<ColorData> bracketColor;
    GroupBarline groupBarline;
    // TODO - group time
    // TODO - group editorial

    // The number attribute is used to distinguish overlapping
    // and nested part-groups, not the sequence of groups.
    // NUMBER_LEVEL_UNSPECIFIED indicates the absence of a number attribute
    int number;

    PartGroupData()
        : firstPartIndex{INDEX_UNSPECIFIED}, lastPartIndex{INDEX_UNSPECIFIED}, name{},
          bracketType{BracketType::unspecified}, bracketPositionData{}, bracketColor{},
          groupBarline{GroupBarline::unspecified}, number{NUMBER_LEVEL_UNSPECIFIED}
    {
    }
};

MXAPI_EQUALS_BEGIN(PartGroupData)
MXAPI_EQUALS_MEMBER(firstPartIndex)
MXAPI_EQUALS_MEMBER(lastPartIndex)
MXAPI_EQUALS_MEMBER(name)
MXAPI_EQUALS_MEMBER(displayName)
MXAPI_EQUALS_MEMBER(abbreviation)
MXAPI_EQUALS_MEMBER(displayAbbreviation)
MXAPI_EQUALS_MEMBER(bracketType)
MXAPI_EQUALS_MEMBER(bracketPositionData)
MXAPI_EQUALS_MEMBER(bracketColor)
MXAPI_EQUALS_MEMBER(groupBarline)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(PartGroupData);
} // namespace api
} // namespace mx
