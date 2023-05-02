// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/file/MxFileRepository.h"
#include "mxtest/api/TestHelpers.h"
#include "ezxml/XAttributeIterator.h"
#include "ezxml/XElement.h"
#include "ezxml/XElementIterator.h"
#include "ezxml/XFactory.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
    constexpr const char* const fileName = "musescore-slur-start-stop.musicxml";
}


TEST( startStop, Slurs )
{
    const auto scoreData = mxtest::MxFileRepository::loadFile( fileName );
    CHECK_EQUAL( 1, scoreData.parts.size() );
    const auto& part = scoreData.parts.front();
    CHECK_EQUAL( 2, part.measures.size() )

    auto measure = part.measures.front();
    auto staff = measure.staves.front();
    auto voice = staff.voices.at(0);
    auto note = voice.notes.at(3);
    auto stop = note.noteAttachmentData.curveStops.at(0);
    CHECK_EQUAL( 1, stop.numberLevel );
    auto start = note.noteAttachmentData.curveStarts.at(0);
    CHECK_EQUAL( 1, start.numberLevel );

    // Write to XML and assert that stop happens before start
    const auto xml = toXml( scoreData );
    auto xdoc = ::ezxml::XFactory::makeXDoc();
    istringstream is(xml);
    xdoc->loadStream( is );
    auto root = xdoc->getRoot();
    auto iter = root->begin();
    std::advance(iter, 5);
    CHECK_EQUAL( "part", iter->getName() );
    auto measureItemsIter = iter->begin()->begin();
    CHECK_EQUAL( "print", measureItemsIter->getName() );
    ++measureItemsIter;
    CHECK_EQUAL( "attributes", measureItemsIter->getName() );
    ++measureItemsIter;
    CHECK_EQUAL( "note", measureItemsIter->getName() );
    ++measureItemsIter;
    CHECK_EQUAL( "note", measureItemsIter->getName() );
    ++measureItemsIter;
    CHECK_EQUAL( "note", measureItemsIter->getName() );
    ++measureItemsIter;
    CHECK_EQUAL( "note", measureItemsIter->getName() );
    auto noteItemsIter = measureItemsIter->begin();
    CHECK_EQUAL( "pitch", noteItemsIter->getName() );
    ++noteItemsIter;
    CHECK_EQUAL( "duration", noteItemsIter->getName() );
    ++noteItemsIter;
    CHECK_EQUAL( "voice", noteItemsIter->getName() );
    ++noteItemsIter;
    CHECK_EQUAL( "type", noteItemsIter->getName() );
    ++noteItemsIter;
    CHECK_EQUAL( "stem", noteItemsIter->getName() );
    ++noteItemsIter;
    CHECK_EQUAL( "notations", noteItemsIter->getName() );

    auto notationIter = noteItemsIter->begin();
    CHECK_EQUAL( "slur", notationIter->getName() );
    auto stopValue = notationIter->attributesBegin()->getValue();
    CHECK_EQUAL( "stop", stopValue );
    
    ++notationIter;
    CHECK_EQUAL( "slur", notationIter->getName() );
    auto startValue = notationIter->attributesBegin()->getValue();
    CHECK_EQUAL( "start", startValue );
}
T_END

#endif
