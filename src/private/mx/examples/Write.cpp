#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

#include "mx/api/DocumentManager.h"
#include "mx/api/ScoreData.h"

// set this to 1 if you want to see the xml in your console
#define MX_WRITE_THIS_TO_THE_CONSOLE 0

int main(int argc, const char *argv[])
{
    using namespace mx::api;
    const auto qticks = 4;

    // create a score
    auto score = ScoreData{};
    score.workTitle = "Mx Example";
    score.composer = "Matthew James Briggs";
    score.copyright = "Copyright (c) 2019";
    score.ticksPerQuarter = qticks;

    // create a part
    score.parts.emplace_back(PartData{});
    auto &part = score.parts.back();

    // give the part a name
    part.name = "Flute";
    part.abbreviation = "Fl.";
    part.displayName = "Flute";
    part.displayAbbreviation = "Fl.";

    // give the part an instrument
    part.instrumentData.soundID = SoundID::windFlutesFlute;
    part.instrumentData.midiData.channel = 1;
    part.instrumentData.midiData.program = 74;

    // add a measure
    part.measures.emplace_back(MeasureData{});
    auto &measure = part.measures.back();
    measure.timeSignatures.front().setSimple("4", "4");
    measure.timeSignatures.front().isImplicit = false;

    // add a staff
    measure.staves.emplace_back(StaffData{});
    auto &staff = measure.staves.back();

    // set the clef
    auto clef = ClefData{};
    clef.setTreble();
    staff.clefs.emplace_back(clef);

    // add a voice
    staff.voices[0] = VoiceData{};
    auto &voice = staff.voices.at(0);

    const auto quarter = qticks;
    const auto half = qticks * 2;
    const auto eighth = qticks / 2;

    // add a few notes
    auto currentTime = 0;
    auto note = NoteData{};
    note.pitchData.step = Step::d;
    note.pitchData.alter = 1;
    note.pitchData.octave = 5;
    note.pitchData.accidental = Accidental::sharp;
    note.durationData.durationName = DurationName::half;
    note.durationData.durationTimeTicks = half;
    note.tickTimePosition = currentTime;
    voice.notes.push_back(note);

    // advance our time
    currentTime += half;

    note.pitchData.step = Step::e;
    note.pitchData.alter = 0;
    note.pitchData.octave = 5;
    note.pitchData.accidental = Accidental::none;
    note.durationData.durationName = DurationName::eighth;
    note.durationData.durationTimeTicks = eighth;
    note.tickTimePosition = currentTime;
    // beams are handled explicitly in musicxml
    note.beams.push_back(Beam::begin); // start an eighth-note beam
    voice.notes.push_back(note);
    currentTime += eighth;

    note.pitchData.step = Step::f;
    note.pitchData.alter = 0;
    note.pitchData.octave = 5;
    note.pitchData.accidental = Accidental::none;
    note.durationData.durationName = DurationName::eighth;
    note.tickTimePosition = currentTime;
    note.durationData.durationTimeTicks = eighth;
    note.beams.clear();
    note.beams.push_back(Beam::end); // end the eighth-note beam
    voice.notes.push_back(note);
    currentTime += eighth;

    note.pitchData.step = Step::e;
    note.pitchData.alter = 0;
    note.pitchData.octave = 5;
    note.pitchData.accidental = Accidental::none;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = quarter;
    note.tickTimePosition = currentTime;
    note.beams.clear();
    voice.notes.push_back(note);

    // the document manager is the liaison between our score data and the MusicXML DOM.
    // it completely hides the MusicXML DOM from us when using mx::api
    auto &mgr = DocumentManager::getInstance();
    const auto idResult = mgr.createFromScore(score);
    if (!idResult.ok())
    {
        return 1;
    }
    const auto documentID = idResult.value();

// write to the console
#if MX_WRITE_THIS_TO_THE_CONSOLE
    (void)mgr.writeToStream(documentID, std::cout);
    std::cout << std::endl;
#endif

    // write to a file. argv[1] overrides the default output path so the build
    // system can send the file to a gitignored location during automated runs;
    // see issue #150.
    const std::string outputPath = (argc > 1) ? argv[1] : "./example.musicxml";
    const auto writeResult = mgr.writeToFile(documentID, outputPath);

    // we need to explicitly delete the object held by the manager
    mgr.destroyDocument(documentID);

    return writeResult.ok() ? 0 : 1;
}