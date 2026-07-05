// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// This example answers a recurring question: "I have a ScoreData, how do I get
// at the notes and their timing?" It shows:
//
//   1. The containment hierarchy you walk to reach notes: ScoreData -> PartData
//      -> MeasureData -> StaffData -> VoiceData -> NoteData. See the comment
//      above ScoreData in ScoreData.h for a one-line pointer back to this file.
//
//   2. That NoteData::tickTimePosition is *measure-relative*, not a global
//      timeline position. To get a note's absolute tick position within the
//      part, you must add up the tick-length of every prior measure yourself;
//      mx::api does not do this for you because measure length is not stored
//      directly (it is implied by whichever voice/staff extends furthest).
//
//   3. That NoteData::beams (the Beam enum: begin/continue/end/...) is
//      MusicXML's beaming notation -- the lines joining eighth notes and
//      shorter -- and has nothing to do with "how many staves/hands a part
//      has." That confusion is what prompted this example (see issue #129).

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "mx/api/DocumentManager.h"
#include "mx/api/ScoreData.h"

#define MX_IS_A_SUCCESS 0
#define MX_IS_A_FAILURE 1

namespace
{
// A single part, two measures, 4/4, divisions="2" (so a quarter note is 2
// ticks and an eighth note is 1 tick). Measure 1 is a beamed run of four
// eighth notes followed by a half note. Measure 2 is two half notes. Every
// measure sums to 8 ticks (4 beats * 2 ticks-per-quarter), so the running
// absolute-tick total advances by 8 at each measure boundary below.
constexpr const char *const xml = R"(
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE score-partwise PUBLIC
    "-//Recordare//DTD MusicXML 3.1 Partwise//EN"
    "http://www.musicxml.org/dtds/partwise.dtd">
<score-partwise version="3.1">
  <part-list>
    <score-part id="P1">
      <part-name>Music</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>2</divisions>
        <key><fifths>0</fifths></key>
        <time><beats>4</beats><beat-type>4</beat-type></time>
        <clef><sign>G</sign><line>2</line></clef>
      </attributes>
      <note>
        <pitch><step>C</step><octave>4</octave></pitch>
        <duration>1</duration>
        <type>eighth</type>
        <beam number="1">begin</beam>
      </note>
      <note>
        <pitch><step>D</step><octave>4</octave></pitch>
        <duration>1</duration>
        <type>eighth</type>
        <beam number="1">continue</beam>
      </note>
      <note>
        <pitch><step>E</step><octave>4</octave></pitch>
        <duration>1</duration>
        <type>eighth</type>
        <beam number="1">continue</beam>
      </note>
      <note>
        <pitch><step>F</step><octave>4</octave></pitch>
        <duration>1</duration>
        <type>eighth</type>
        <beam number="1">end</beam>
      </note>
      <note>
        <pitch><step>G</step><octave>4</octave></pitch>
        <duration>4</duration>
        <type>half</type>
      </note>
    </measure>
    <measure number="2">
      <note>
        <pitch><step>C</step><octave>5</octave></pitch>
        <duration>4</duration>
        <type>half</type>
      </note>
      <note>
        <pitch><step>C</step><octave>5</octave></pitch>
        <duration>4</duration>
        <type>half</type>
      </note>
    </measure>
  </part>
</score-partwise>
)";

std::string stepToString(mx::api::Step step)
{
    switch (step)
    {
    case mx::api::Step::c:
        return "C";
    case mx::api::Step::d:
        return "D";
    case mx::api::Step::e:
        return "E";
    case mx::api::Step::f:
        return "F";
    case mx::api::Step::g:
        return "G";
    case mx::api::Step::a:
        return "A";
    case mx::api::Step::b:
        return "B";
    default:
        return "?";
    }
}

std::string beamsToString(const std::vector<mx::api::Beam> &beams)
{
    // Real notes usually carry one Beam value per beam level (1 for an
    // eighth note, 2 for a 16th, etc). An empty vector means "not beamed."
    if (beams.empty())
    {
        return "none";
    }
    std::string result;
    for (const auto beam : beams)
    {
        if (!result.empty())
        {
            result += "+";
        }
        switch (beam)
        {
        case mx::api::Beam::begin:
            result += "begin";
            break;
        case mx::api::Beam::end:
            result += "end";
            break;
        case mx::api::Beam::extend:
            result += "extend";
            break;
        case mx::api::Beam::forwardBroken:
            result += "forwardBroken";
            break;
        case mx::api::Beam::backwardBroken:
            result += "backwardBroken";
            break;
        case mx::api::Beam::unspecified:
        default:
            result += "unspecified";
            break;
        }
    }
    return result;
}

// mx::api does not store a measure's tick-length directly: it is whatever the
// furthest-extending note/rest, across every staff and voice, adds up to.
// Summing tickTimePosition + durationTimeTicks over every note is how you
// recover it.
int measureLengthInTicks(const mx::api::MeasureData &measure)
{
    int length = 0;
    for (const auto &staff : measure.staves)
    {
        for (const auto &voiceEntry : staff.voices)
        {
            const mx::api::VoiceData &voice = voiceEntry.second;
            for (const auto &note : voice.notes)
            {
                const int noteEnd = note.tickTimePosition + note.durationData.durationTimeTicks;
                length = std::max(length, noteEnd);
            }
        }
    }
    return length;
}
} // namespace

int main(int argc, const char *argv[])
{
    using namespace mx::api;

    auto &mgr = DocumentManager::getInstance();
    std::istringstream istr{xml};

    const auto idResult = mgr.createFromStream(istr);
    if (!idResult.ok())
    {
        return MX_IS_A_FAILURE;
    }
    const auto documentID = idResult.value();

    const auto scoreResult = mgr.getData(documentID);
    mgr.destroyDocument(documentID);
    if (!scoreResult.ok())
    {
        return MX_IS_A_FAILURE;
    }
    const ScoreData &score = scoreResult.value();

    std::cout << "ticksPerQuarter = " << score.ticksPerQuarter << "\n\n";

    for (std::size_t partIndex = 0; partIndex < score.parts.size(); ++partIndex)
    {
        const PartData &part = score.parts.at(partIndex);
        std::cout << "part[" << partIndex << "] \"" << part.name << "\"\n";

        // Running total of ticks consumed by all measures seen so far in this
        // part. Adding it to a note's (measure-relative) tickTimePosition
        // gives that note's absolute tick position in the part's timeline.
        int absoluteTickCursor = 0;

        for (std::size_t measureIndex = 0; measureIndex < part.measures.size(); ++measureIndex)
        {
            const MeasureData &measure = part.measures.at(measureIndex);
            std::cout << "  measure[" << measureIndex << "]\n";

            for (std::size_t staffIndex = 0; staffIndex < measure.staves.size(); ++staffIndex)
            {
                const StaffData &staff = measure.staves.at(staffIndex);

                for (const auto &voiceEntry : staff.voices)
                {
                    const int voiceNumber = voiceEntry.first;
                    const VoiceData &voice = voiceEntry.second;

                    for (const auto &note : voice.notes)
                    {
                        const int absoluteTick = absoluteTickCursor + note.tickTimePosition;
                        const double quarterNotePosition =
                            static_cast<double>(absoluteTick) / static_cast<double>(score.ticksPerQuarter);

                        std::cout << "    staff[" << staffIndex << "] voice[" << voiceNumber << "] "
                                  << (note.isRest ? "rest" : stepToString(note.pitchData.step))
                                  << (note.isRest ? "" : std::to_string(note.pitchData.octave))
                                  << " tickTimePosition(measure-relative)=" << note.tickTimePosition
                                  << " absoluteTick=" << absoluteTick << " (quarter note " << std::fixed
                                  << std::setprecision(2) << quarterNotePosition << ")"
                                  << " durationTicks=" << note.durationData.durationTimeTicks
                                  << " beams=" << beamsToString(note.beams) << "\n";
                    }
                }
            }

            absoluteTickCursor += measureLengthInTicks(measure);
        }
    }

    return MX_IS_A_SUCCESS;
}
