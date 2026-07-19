// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/DirectionData.h"
#include "mx/core/generated/Direction.h"
#include "mx/core/generated/DirectionType.h"
#include "mx/core/generated/Dynamics.h"
#include "mx/core/generated/Harmony.h"
#include "mx/core/generated/HarmonyChordGroup.h"
#include "mx/core/generated/PercussionChoice.h"
#include "mx/impl/Converter.h"
#include "mx/impl/Cursor.h"
#include "mx/impl/PositionFunctions.h"

#include <mutex>
#include <vector>

namespace mx
{
namespace impl
{
class DirectionReader
{
  public:
    DirectionReader(const core::Direction &inDirection, Cursor inCursor);
    DirectionReader(const core::Harmony &inHarmony, Cursor inCursor);
    api::DirectionData getDirectionData();

  private:
    const core::Direction *const myDirection;
    const core::Harmony *const myHarmony;
    const Cursor myCursor;
    const Converter myConverter;
    api::DirectionData myOutDirectionData;

  private:
    mx::api::DirectionData initializeData();
    void parseOffset();
    void parsePlacement();
    void parseValues();
    void fixTimes();
    mx::api::DirectionData returnData();
    void appendOrderedComponent(api::DirectionComponentKind kind, int index);
    void parseStaffIndex();
    void parseDirectionType(const core::DirectionType &directionType);
    void parseRehearsal(const core::DirectionType &directionType);
    void parseSegno(const core::DirectionType &directionType);
    void parseWords(const core::DirectionType &directionType);
    void parseCoda(const core::DirectionType &directionType);
    void parseWedge(const core::DirectionType &directionType);
    void parseDynamics(const core::DirectionType &directionType);
    void parseDynamic(const core::Dynamics &dynamic);
    void parseDashes(const core::DirectionType &directionType);
    void parseBracket(const core::DirectionType &directionType);
    void parsePedal(const core::DirectionType &directionType);
    void parseMetronome(const core::DirectionType &directionType);
    void parseOctaveShift(const core::DirectionType &directionType);
    void parseHarpPedals(const core::DirectionType &directionType);
    void parseDamp(const core::DirectionType &directionType);
    void parseDampAll(const core::DirectionType &directionType);
    void parseEyeglasses(const core::DirectionType &directionType);
    void parseStringMute(const core::DirectionType &directionType);
    void parseStaffDivide(const core::DirectionType &directionType);
    void parseScordatura(const core::DirectionType &directionType);
    void parseImage(const core::DirectionType &directionType);
    void parsePrincipalVoice(const core::DirectionType &directionType);
    void parseAccordionRegistration(const core::DirectionType &directionType);
    api::PercussionDataChoice getPercussionChoice(const core::PercussionChoice &choice) const;
    void parsePercussion(const core::DirectionType &directionType);
    void parseOtherDirection(const core::DirectionType &directionType);
    void parseHarmony(const core::Harmony &inHarmony, const core::HarmonyChordGroup &inGrp);

    template <typename ATTRIBUTES_TYPE> api::PositionData parsePositionData(const ATTRIBUTES_TYPE &attributes)
    {
        api::PositionData positionData = impl::getPositionData(attributes);

        if (positionData.placement == api::Placement::unspecified)
        {
            if (myDirection && myDirection->placement().has_value())
            {
                positionData.placement = myConverter.convert(*myDirection->placement());
            }
        }

        return positionData;
    }
};
} // namespace impl
} // namespace mx
