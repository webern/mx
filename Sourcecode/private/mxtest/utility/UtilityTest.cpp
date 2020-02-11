// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"

#ifdef MX_COMPILE_UTILTIY_TESTS

    #include "cpul/testFramework.h"
    #include "mx/utility/Utility.h"

    #include <string>
    #include <sstream>
    #include <memory>


using namespace std;
using namespace mx::core;
using namespace mx::utility;


TEST( setTitle, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    setTitle( doc, "My Title" );

    bool isElementEnabled = doc->getScorePartwise()->getScoreHeaderGroup()->getHasWork();
    isElementEnabled &= doc->getScorePartwise()->getScoreHeaderGroup()->getWork()->getHasWorkTitle();
    CHECK( isElementEnabled );

    StringType expected = "My Title";
    StringType
        actual = doc->getScorePartwise()->getScoreHeaderGroup()->getWork()->getWorkTitle()->getValue().getValue();
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( getTitleFalse, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    doc->getScorePartwise()->getScoreHeaderGroup()->getWork()->getWorkTitle()->setValue( XsString( "AnyString" ) );

    bool isElementEnabled = doc->getScorePartwise()->getScoreHeaderGroup()->getHasWork();
    isElementEnabled &= doc->getScorePartwise()->getScoreHeaderGroup()->getWork()->getHasWorkTitle();
    CHECK( !isElementEnabled );

    StringType expected = "";
    StringType actual = getTitle( doc );
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( getTitle, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    doc->getScorePartwise()->getScoreHeaderGroup()->getWork()->getWorkTitle()->setValue( XsString( "AnyString" ) );
    doc->getScorePartwise()->getScoreHeaderGroup()->setHasWork( true );
    doc->getScorePartwise()->getScoreHeaderGroup()->getWork()->setHasWorkTitle( true );
    bool isElementEnabled = doc->getScorePartwise()->getScoreHeaderGroup()->getHasWork();
    isElementEnabled &= doc->getScorePartwise()->getScoreHeaderGroup()->getWork()->getHasWorkTitle();
    CHECK( isElementEnabled );

    StringType expected = "AnyString";
    StringType actual = getTitle( doc );
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( setMovementTitle, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    setMovementTitle( doc, "My Title" );

    bool isElementEnabled = doc->getScorePartwise()->getScoreHeaderGroup()->getHasMovementTitle();
    CHECK( isElementEnabled );

    StringType expected = "My Title";
    StringType actual = doc->getScorePartwise()->getScoreHeaderGroup()->getMovementTitle()->getValue().getValue();
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( getMovementTitleFalse, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    doc->getScorePartwise()->getScoreHeaderGroup()->getMovementTitle()->setValue( XsString( "AnyString" ) );

    bool isElementEnabled = doc->getScorePartwise()->getScoreHeaderGroup()->getHasMovementTitle();
    CHECK( !isElementEnabled );

    StringType expected = "";
    StringType actual = getMovementTitle( doc );
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( getMovementTitle, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    doc->getScorePartwise()->getScoreHeaderGroup()->getMovementTitle()->setValue( XsString( "AnyString" ) );
    doc->getScorePartwise()->getScoreHeaderGroup()->setHasMovementTitle( true );
    bool isElementEnabled = doc->getScorePartwise()->getScoreHeaderGroup()->getHasMovementTitle();
    CHECK( isElementEnabled );

    StringType expected = "AnyString";
    StringType actual = getMovementTitle( doc );
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( setMovementNumber, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    setMovementNumber( doc, 1 );

    bool isElementEnabled = doc->getScorePartwise()->getScoreHeaderGroup()->getHasMovementNumber();
    CHECK( isElementEnabled );

    StringType expected = "1";
    StringType actual = doc->getScorePartwise()->getScoreHeaderGroup()->getMovementNumber()->getValue().getValue();
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( getMovementNumberFalse, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    doc->getScorePartwise()->getScoreHeaderGroup()->getMovementNumber()->setValue( XsString( "1" ) );

    bool isElementEnabled = doc->getScorePartwise()->getScoreHeaderGroup()->getHasMovementNumber();
    CHECK( !isElementEnabled );

    int expected = 0;
    int actual = getMovementNumber( doc );
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( getMovementNumber, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    doc->getScorePartwise()->getScoreHeaderGroup()->getMovementNumber()->setValue( XsString( "2" ) );
    doc->getScorePartwise()->getScoreHeaderGroup()->setHasMovementNumber( true );
    bool isElementEnabled = doc->getScorePartwise()->getScoreHeaderGroup()->getHasMovementNumber();
    CHECK( isElementEnabled );

    int expected = 2;
    int actual = getMovementNumber( doc );
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( setComposer, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    setComposer( doc, "AnyString" );

    CHECK_EQUAL( 1, doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->getCreatorSet().size() )
    auto
        composerCredit = *doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->getCreatorSet().cbegin();
    StringType expected = "AnyString";
    StringType actual = composerCredit->getValue().getValue();
    CHECK_EQUAL( expected, actual )
    expected = "composer";
    actual = composerCredit->getAttributes()->type.getValue();
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( getComposerFalse, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    auto composerCreator = makeCreator();
    composerCreator->setValue( XsString( "Bishop the Cat" ) );
    composerCreator->getAttributes()->type = XsToken( "cat" );
    doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->addCreator( composerCreator );
    StringType expected = "";
    StringType actual = getComposer( doc );
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( getComposer, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    auto composerCreator = makeCreator();
    composerCreator->setValue( XsString( "Bishop the Cat" ) );
    composerCreator->getAttributes()->type = XsToken( "composer" );
    doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->addCreator( composerCreator );
    StringType expected = "Bishop the Cat";
    StringType actual = getComposer( doc );
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( setCopyright, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    setCopyright( doc, "AnyString" );

    CHECK_EQUAL( 1, doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->getRightsSet().size() )
    auto item = *doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->getRightsSet().cbegin();
    StringType expected = "AnyString";
    StringType actual = item->getValue().getValue();
    CHECK_EQUAL( expected, actual )
    expected = "copyright";
    actual = item->getAttributes()->type.getValue();
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( getCopyrightFalse, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    auto item = makeRights();
    item->setValue( XsString( "Bishop the Cat" ) );
    item->getAttributes()->type = XsToken( "cat" );
    doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->addRights( item );
    StringType expected = "";
    StringType actual = getCopyright( doc );
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( getCopyright, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    auto item = makeRights();
    item->setValue( XsString( "Bishop the Cat" ) );
    item->getAttributes()->type = XsToken( "copyright" );
    doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->addRights( item );
    StringType expected = "Bishop the Cat";
    StringType actual = getCopyright( doc );
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( setSoftware, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    setSoftware( doc, "AnyString" );
    auto encoding = doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->getEncoding();
    const auto& encodingChoiceSet = encoding->getEncodingChoiceSet();
    auto encodingChoice = *encodingChoiceSet.cbegin();
    auto software = encodingChoice->getSoftware();
    auto choice = encodingChoice->getChoice();
    CHECK( EncodingChoice::Choice::software == choice );

    auto actual = software->getValue().getValue();
    auto expected = "AnyString";
    CHECK_EQUAL( expected, actual );

//    doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->getEncoding()->getEncodingChoiceSet().cbegin()->getSoftware()->getValue().getValue();
//    StringType expected = "AnyString";
//    StringType actual = doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->getEncoding()->getEncodingChoiceSet().cbegin()->getSoftware()->getValue().getValue();
}

T_END


TEST( getSoftwareFalse, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    auto encoding = makeEncoding();
    auto encodingChoice = makeEncodingChoice();
    encodingChoice->setChoice( EncodingChoice::Choice::software );
    encodingChoice->getSoftware()->setValue( XsString( "Some Value" ) );
    encoding->addEncodingChoice( encodingChoice );
    doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->setEncoding( encoding );
    doc->getScorePartwise()->getScoreHeaderGroup()->setHasIdentification( true );
    StringType expected = "";
    StringType actual = getSoftware( doc );
    CHECK_EQUAL( expected, actual );
}

T_END


TEST( getSoftware, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    auto encoding = makeEncoding();
    auto encodingChoice = makeEncodingChoice();
    encodingChoice->setChoice( EncodingChoice::Choice::software );
    encodingChoice->getSoftware()->setValue( XsString( "Some Value" ) );
    encoding->addEncodingChoice( encodingChoice );
    doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->setEncoding( encoding );
    doc->getScorePartwise()->getScoreHeaderGroup()->setHasIdentification( true );
    doc->getScorePartwise()->getScoreHeaderGroup()->getIdentification()->setHasEncoding( true );
    StringType expected = "Some Value";
    StringType actual = getSoftware( doc );
    CHECK_EQUAL( expected, actual );
}

T_END


TEST( startPartGroupInitialGroup, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    PartGroupParams params( "Woodwinds" );
    params.abbreviation = "Ww.";
    params.isGroupBarline = true;
    params.groupSymbol = GroupSymbolValue::bracket;
    params.number = 1;
    startPartGroup( doc, params );
    size_t expected = 1;
    size_t actual = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList()->getPartGroupSet().size();
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( startPartGroupTwoGroupsAtBeginning, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    PartGroupParams params( "Woodwinds" );
    params.abbreviation = "Ww.";
    params.isGroupBarline = true;
    params.groupSymbol = GroupSymbolValue::bracket;
    params.number = 1;
    startPartGroup( doc, params );
    startPartGroup( doc, params );
    size_t expected = 2;
    size_t actual = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList()->getPartGroupSet().size();
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( startPartGroupOneAtBeginningOneAfterFirstPart, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    PartGroupParams params( "Woodwinds" );
    params.abbreviation = "Ww.";
    params.isGroupBarline = true;
    params.groupSymbol = GroupSymbolValue::bracket;
    params.number = 1;
    startPartGroup( doc, params );
    auto partList = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList();
    auto firstPart = partList->getScorePart();
    firstPart->getAttributes()->id = XsID( "XXX" );
    startPartGroup( doc, params );
    size_t expected = 1;
    size_t actual = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList()->getPartGroupSet().size();
    CHECK_EQUAL( expected, actual )
    expected = 1;
    actual = partList->getPartGroupOrScorePartSet().size();
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( stopPartGroup, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    stopPartGroup( doc );
    size_t expected = 1;
    size_t actual = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList()->getPartGroupOrScorePartSet().size();
    CHECK_EQUAL( expected, actual )
    auto groupStopContainer =
        *( doc->getScorePartwise()->getScoreHeaderGroup()->getPartList()->getPartGroupOrScorePartSet().cbegin() );
    CHECK( groupStopContainer->getChoice() == PartGroupOrScorePart::Choice::partGroup );
    auto groupStopElement = groupStopContainer->getPartGroup();
    CHECK( !groupStopElement->getAttributes()->hasNumber );
    CHECK( groupStopElement->getAttributes()->type == StartStop::stop );
}

T_END


TEST( stopPartGroupInt, MxUtility )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    stopPartGroup( doc, 10 );
    size_t expected = 1;
    size_t actual = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList()->getPartGroupOrScorePartSet().size();
    CHECK_EQUAL( expected, actual )
    auto groupStopContainer =
        *( doc->getScorePartwise()->getScoreHeaderGroup()->getPartList()->getPartGroupOrScorePartSet().cbegin() );
    CHECK( groupStopContainer->getChoice() == PartGroupOrScorePart::Choice::partGroup );
    auto groupStopElement = groupStopContainer->getPartGroup();
    CHECK( groupStopElement->getAttributes()->hasNumber );
    CHECK_EQUAL( "10", groupStopElement->getAttributes()->number.getValue() )
    CHECK( groupStopElement->getAttributes()->type == StartStop::stop );
}

T_END


TEST( addPart, MxUtilityImpl )
{
    PartParams p1;
    p1.uniqueId = "P1";
    p1.name = "Part 1";
    PartParams p2;
    p2.uniqueId = "P2";
    p2.name = "Part 2";
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    PartGroupParams group1;
    group1.name = "These Parts";
    group1.number = 1;
    startPartGroup( doc, group1 );
    addPart( doc, p1 );
    addPart( doc, p2 );
    stopPartGroup( doc, 1 );
    std::stringstream ss;
    ss << R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)" << endl;
    ss
        << R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 3.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">)"
        << endl;
    ss << R"(<score-partwise>)" << endl;
    ss << R"(   <part-list>)" << endl;
    ss << R"(      <part-group type="start" number="1">)" << endl;
    ss << R"(         <group-name>These Parts</group-name>)" << endl;
    ss << R"(         <group-name-display>)" << endl;
    ss << R"(            <display-text></display-text>)" << endl;
    ss << R"(         </group-name-display>)" << endl;
    ss << R"(         <group-abbreviation-display>)" << endl;
    ss << R"(            <display-text></display-text>)" << endl;
    ss << R"(         </group-abbreviation-display>)" << endl;
    ss << R"(         <group-symbol>bracket</group-symbol>)" << endl;
    ss << R"(         <group-barline>yes</group-barline>)" << endl;
    ss << R"(      </part-group>)" << endl;
    ss << R"(      <score-part id="P1">)" << endl;
    ss << R"(         <part-name>Part 1</part-name>)" << endl;
    ss << R"(      </score-part>)" << endl;
    ss << R"(      <score-part id="P2">)" << endl;
    ss << R"(         <part-name>Part 2</part-name>)" << endl;
    ss << R"(      </score-part>)" << endl;
    ss << R"(      <part-group type="stop" number="1"/>)" << endl;
    ss << R"(   </part-list>)" << endl;
    ss << R"(   <part id="P1">)" << endl;
    ss << R"(      <measure number=""/>)" << endl;
    ss << R"(   </part>)" << endl;
    ss << R"(   <part id="P2">)" << endl;
    ss << R"(      <measure number=""/>)" << endl;
    ss << R"(   </part>)" << endl;
    ss << R"(</score-partwise>)";
    StringType expected = ss.str();
    ss.str( "" );
    doc->toStream( ss );
    StringType actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

T_END


TEST( measure, MxUtilityImpl )
{
    auto measure = createMeasure( 10 );
    std::stringstream ss;
    ss << R"(<measure number="10"/>)";
    StringType expected = ss.str();
    ss.str( "" );
    measure->toStream( ss, 0 );
    StringType actual = ss.str();
    CHECK_EQUAL( expected, actual );
}

T_END


TEST( measureTimeSignature, MxUtilityImpl )
{
    auto measure = createMeasure( 9 );
    addTimeSignatureGroup( measure, 5, 4 );
    std::stringstream ss;
    ss << R"(<measure number="9">)" << endl;
    ss << R"(   <attributes>)" << endl;
    ss << R"(      <time>)" << endl;
    ss << R"(         <beats>5</beats>)" << endl;
    ss << R"(         <beat-type>4</beat-type>)" << endl;
    ss << R"(      </time>)" << endl;
    ss << R"(   </attributes>)" << endl;
    ss << R"(</measure>)";
    StringType expected = ss.str();
    ss.str( "" );
    measure->toStream( ss, 0 );
    StringType actual = ss.str();
    CHECK_EQUAL( expected, actual );
}

T_END


TEST( measureStaves, MxUtilityImpl )
{
    auto measure = createMeasure( 9 );
    addStaveCount( measure, 2 );
    addTimeSignatureGroup( measure, 5, 4 );
    std::stringstream ss;
    ss << R"(<measure number="9">)" << endl;
    ss << R"(   <attributes>)" << endl;
    ss << R"(      <time>)" << endl;
    ss << R"(         <beats>5</beats>)" << endl;
    ss << R"(         <beat-type>4</beat-type>)" << endl;
    ss << R"(      </time>)" << endl;
    ss << R"(      <staves>2</staves>)" << endl;
    ss << R"(   </attributes>)" << endl;
    ss << R"(</measure>)";
    StringType expected = ss.str();
    ss.str( "" );
    measure->toStream( ss, 0 );
    StringType actual = ss.str();
    CHECK_EQUAL( expected, actual );
}

T_END


TEST( measureDivisions, MxUtilityImpl )
{
    auto measure = createMeasure( 9 );
    addDivisions( measure, 123456789 );
    addStaveCount( measure, 2 );
    addTimeSignatureGroup( measure, 5, 4 );
    std::stringstream ss;
    ss << R"(<measure number="9">)" << endl;
    ss << R"(   <attributes>)" << endl;
    ss << R"(      <divisions>123456789</divisions>)" << endl;
    ss << R"(      <time>)" << endl;
    ss << R"(         <beats>5</beats>)" << endl;
    ss << R"(         <beat-type>4</beat-type>)" << endl;
    ss << R"(      </time>)" << endl;
    ss << R"(      <staves>2</staves>)" << endl;
    ss << R"(   </attributes>)" << endl;
    ss << R"(</measure>)";
    StringType expected = ss.str();
    ss.str( "" );
    measure->toStream( ss, 0 );
    StringType actual = ss.str();
    CHECK_EQUAL( expected, actual );
}

T_END


TEST( getDivisions, MxUtilityImpl )
{
    auto measure = createMeasure( 9 );
    addDivisions( measure, 65 );
    int expected = 65;
    int actual = getDivisions( measure );
    CHECK_EQUAL( expected, actual )
}

T_END


// TODO createNote tests

#endif