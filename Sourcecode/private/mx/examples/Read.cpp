#include <string>
#include <iostream>
#include <cstdint>
#include <sstream>

#include "mx/api/DocumentManager.h"
#include "mx/api/ScoreData.h"

#define MX_IS_A_SUCCESS 0
#define MX_IS_A_FAILURE 1

constexpr const char* const xml = R"(
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
        <divisions>1</divisions>
        <key>
          <fifths>0</fifths>
        </key>
        <time>
          <beats>4</beats>
          <beat-type>4</beat-type>
        </time>
        <clef>
          <sign>G</sign>
          <line>2</line>
        </clef>
      </attributes>
      <note>
        <pitch>
          <step>C</step>
          <octave>4</octave>
        </pitch>
        <duration>4</duration>
        <type>whole</type>
      </note>
    </measure>
  </part>
</score-partwise>
)";

int main(int argc, const char * argv[])
{
    using namespace mx::api;

    // create a reference to the singleton which holds documents in memory for us
    auto& mgr = DocumentManager::getInstance();

    // place the xml from above into a stream object
    std::istringstream istr{ xml };

    // ask the document manager to parse the xml into memory for us, returns a document ID.
    const auto documentID = mgr.createFromStream( istr );

    // get the structural representation of the score from the document manager
    const auto score = mgr.getData( documentID );

    // we need to explicitly destroy the document from memory
    mgr.destroyDocument(documentID);

    // make sure we have exactly one part
    if( score.parts.size() != 1 )
    {
        return MX_IS_A_FAILURE;
    }

    // drill down into the data structure to retrieve the note
    const auto& part = score.parts.at( 0 );
    const auto& measure = part.measures.at( 0 );
    const auto& staff = measure.staves.at( 0 );
    const auto& voice = staff.voices.at( 0 );
    const auto& note = voice.notes.at( 0 );

    if( note.durationData.durationName != DurationName::whole )
    {
        return MX_IS_A_FAILURE;
    }

    if( note.pitchData.step != Step::c )
    {
        return MX_IS_A_FAILURE;
    }

    return MX_IS_A_SUCCESS;
}