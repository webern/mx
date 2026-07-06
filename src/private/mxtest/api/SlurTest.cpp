// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/api/TestHelpers.h"
#include "mxtest/file/MxFileRepository.h"
#include "pugixml.hpp"

#include <sstream>

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
constexpr const char *const fileName = "musescore-slur-start-stop.musicxml";
}

TEST(startStop, Slurs)
{
    const auto scoreData = mxtest::MxFileRepository::loadFile(fileName);
    CHECK_EQUAL(1, scoreData.parts.size());
    const auto &part = scoreData.parts.front();
    CHECK_EQUAL(2, part.measures.size())

    auto measure = part.measures.front();
    auto staff = measure.staves.front();
    auto voice = staff.voices.at(0);
    auto note = voice.notes.at(3);
    auto stop = note.noteAttachmentData.curveStops.at(0);
    CHECK(SpannerNumber::makeLevel(1) == stop.number);
    auto start = note.noteAttachmentData.curveStarts.at(0);
    CHECK(SpannerNumber::makeLevel(1) == start.number);

    // Write to XML and assert that stop happens before start
    const auto xml = toXml(scoreData);

    pugi::xml_document xdoc;
    xdoc.load_string(xml.c_str());
    auto root = xdoc.document_element();

    // Walk to the 6th child element (0-indexed: 5) which is "part"
    auto iter = root.begin();
    int idx = 0;
    while (iter != root.end() && idx < 5)
    {
        ++iter;
        ++idx;
    }
    REQUIRE(iter != root.end());
    CHECK_EQUAL(std::string{"part"}, std::string{iter->name()});

    // iter->begin() is the first measure; iter->begin()->begin() is first child of measure
    auto measureNode = iter->begin();
    REQUIRE(measureNode != iter->end());
    auto measureItemsIter = measureNode->begin();

    CHECK_EQUAL(std::string{"print"}, std::string{measureItemsIter->name()});
    ++measureItemsIter;
    CHECK_EQUAL(std::string{"attributes"}, std::string{measureItemsIter->name()});
    ++measureItemsIter;
    CHECK_EQUAL(std::string{"note"}, std::string{measureItemsIter->name()});
    ++measureItemsIter;
    CHECK_EQUAL(std::string{"note"}, std::string{measureItemsIter->name()});
    ++measureItemsIter;
    CHECK_EQUAL(std::string{"note"}, std::string{measureItemsIter->name()});
    ++measureItemsIter;
    CHECK_EQUAL(std::string{"note"}, std::string{measureItemsIter->name()});

    auto noteItemsIter = measureItemsIter->begin();
    CHECK_EQUAL(std::string{"pitch"}, std::string{noteItemsIter->name()});
    ++noteItemsIter;
    CHECK_EQUAL(std::string{"duration"}, std::string{noteItemsIter->name()});
    ++noteItemsIter;
    CHECK_EQUAL(std::string{"voice"}, std::string{noteItemsIter->name()});
    ++noteItemsIter;
    CHECK_EQUAL(std::string{"type"}, std::string{noteItemsIter->name()});
    ++noteItemsIter;
    CHECK_EQUAL(std::string{"stem"}, std::string{noteItemsIter->name()});
    ++noteItemsIter;
    CHECK_EQUAL(std::string{"notations"}, std::string{noteItemsIter->name()});

    auto notationIter = noteItemsIter->begin();
    CHECK_EQUAL(std::string{"slur"}, std::string{notationIter->name()});
    auto stopValue = std::string{notationIter->attribute("type").value()};
    CHECK_EQUAL(std::string{"stop"}, stopValue);

    ++notationIter;
    CHECK_EQUAL(std::string{"slur"}, std::string{notationIter->name()});
    auto startValue = std::string{notationIter->attribute("type").value()};
    CHECK_EQUAL(std::string{"start"}, startValue);
}

T_END

#endif
