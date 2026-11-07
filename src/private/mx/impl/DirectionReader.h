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
#include "mx/impl/DiagnosticsContext.h"
#include "mx/impl/MeasureCursor.h"
#include "mx/impl/PositionFunctions.h"

#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace mx
{
namespace core
{
class PartwisePart;
class Wedge;
} // namespace core

namespace impl
{
// MusicXML writes a diminuendo's niente on the stop that closes it, which comes later in the
// part than the start that api::WedgeStart::niente lives on. This is what a scan of the whole
// part learns about that pairing before the part's directions are read.
struct WedgeNiente
{
    // The niente stated by the stop of each diminuendo whose stop states one.
    std::unordered_map<const core::Wedge *, api::Bool> diminuendoNiente;

    // Every stop that closes a diminuendo. niente on any other stop is not at the hairpin's point.
    std::unordered_set<const core::Wedge *> diminuendoStops;
};

class DirectionReader
{
  public:
    // inWedgeNiente is the scan of the part this direction belongs to (see scanWedgeNiente). When
    // it is null, a diminuendo's niente is not read.
    DirectionReader(const core::Direction &inDirection, MeasureCursor inCursor, DiagnosticsContext diagnostics = {},
                    const WedgeNiente *inWedgeNiente = nullptr);
    DirectionReader(const core::Harmony &inHarmony, MeasureCursor inCursor, DiagnosticsContext diagnostics = {});
    api::DirectionData getDirectionData();

    // Pairs the part's wedge stops with the starts they close, in one pass in document order and
    // by the rule in SpannerPairing.h, and records what each diminuendo's stop says about niente.
    static WedgeNiente scanWedgeNiente(const core::PartwisePart &inPart);

  private:
    const core::Direction *const myDirection;
    const core::Harmony *const myHarmony;
    const MeasureCursor myCursor;
    const Converter myConverter;
    DiagnosticsContext myDiagnostics;
    const WedgeNiente *const myWedgeNiente;
    api::DirectionData myOutDirectionData;

  private:
    mx::api::DirectionData initializeData();
    void report(api::Severity severity, api::DiagnosticCode code, std::string message) const;
    void reportRounded(const char *name, double value, int rounded) const;
    void parseOffset();
    void parsePlacement();
    void parseSystemRelation();
    void parseDirective();
    void parseValues();
    mx::api::DirectionData returnData();
    void parseStaffIndex();
    void parseDirectionType(const core::DirectionType &directionType);
    void parseRehearsal(const core::DirectionType &directionType);
    void parseSegno(const core::DirectionType &directionType);
    void parseWordsRun(const core::DirectionType &directionType);
    void parseCoda(const core::DirectionType &directionType);
    void parseWedge(const core::DirectionType &directionType);
    api::Bool parseWedgeStartNiente(const core::Wedge &wedge) const;
    void parseDynamics(const core::DirectionType &directionType);
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
