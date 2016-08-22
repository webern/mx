// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test0, ConvenienceMakeFunctions )
{
    auto val = DynamicsValue{ DynamicsEnum::ffffff };
	auto a = makeDynamics( val );
	auto b = makeDynamics( val );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test1, ConvenienceMakeFunctions )
{
    auto val = AccidentalValue::flatDown;
	auto a = makeAccidentalText( val );
	auto b = makeAccidentalText( val );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test2, ConvenienceMakeFunctions )
{
	auto a = makeDisplayText( XsString { "something" } );
	auto b = makeDisplayText( XsString { "something" } );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test3, ConvenienceMakeFunctions )
{
	auto a = makeOtherPlay(  );
	auto b = makeOtherPlay(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test4, ConvenienceMakeFunctions )
{
	auto a = makePartSymbol( GroupSymbolValue::square );
	auto b = makePartSymbol( GroupSymbolValue::square );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test5, ConvenienceMakeFunctions )
{
	auto a = makeKeyOctave(  );
	auto b = makeKeyOctave(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test6, ConvenienceMakeFunctions )
{
	auto a = makeMultipleRest(  );
	auto b = makeMultipleRest(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test7, ConvenienceMakeFunctions )
{
	auto a = makeMeasureRepeat(  );
	auto b = makeMeasureRepeat(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test8, ConvenienceMakeFunctions )
{
	auto a = makeBarStyle(  );
	auto b = makeBarStyle(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test9, ConvenienceMakeFunctions )
{
	auto a = makeFermata(  );
	auto b = makeFermata(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test10, ConvenienceMakeFunctions )
{
	auto a = makeEnding(  );
	auto b = makeEnding(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test11, ConvenienceMakeFunctions )
{
	auto a = makeBassStep(  );
	auto b = makeBassStep(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test12, ConvenienceMakeFunctions )
{
	auto a = makeBassAlter(  );
	auto b = makeBassAlter(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test13, ConvenienceMakeFunctions )
{
	auto a = makeDegreeValue(  );
	auto b = makeDegreeValue(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test14, ConvenienceMakeFunctions )
{
	auto a = makeDegreeAlter(  );
	auto b = makeDegreeAlter(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test15, ConvenienceMakeFunctions )
{
	auto a = makeDegreeType(  );
	auto b = makeDegreeType(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test16, ConvenienceMakeFunctions )
{
	auto a = makeOffset(  );
	auto b = makeOffset(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test17, ConvenienceMakeFunctions )
{
	auto a = makeRehearsal(  );
	auto b = makeRehearsal(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test18, ConvenienceMakeFunctions )
{
	auto a = makeWords(  );
	auto b = makeWords(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test19, ConvenienceMakeFunctions )
{
	auto a = makePrincipalVoice(  );
	auto b = makePrincipalVoice(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test20, ConvenienceMakeFunctions )
{
	auto a = makeOtherDirection(  );
	auto b = makeOtherDirection(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test21, ConvenienceMakeFunctions )
{
	auto a = makeFirstFret(  );
	auto b = makeFirstFret(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test22, ConvenienceMakeFunctions )
{
	auto a = makeFret(  );
	auto b = makeFret(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test23, ConvenienceMakeFunctions )
{
	auto a = makeFingering(  );
	auto b = makeFingering(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test24, ConvenienceMakeFunctions )
{
	auto a = makeFeature(  );
	auto b = makeFeature(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test25, ConvenienceMakeFunctions )
{
	auto a = makePerMinute(  );
	auto b = makePerMinute(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test26, ConvenienceMakeFunctions )
{
	auto a = makeMetronomeBeam(  );
	auto b = makeMetronomeBeam(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test27, ConvenienceMakeFunctions )
{
	auto a = makeBeater(  );
	auto b = makeBeater(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test28, ConvenienceMakeFunctions )
{
	auto a = makeMeasureNumbering(  );
	auto b = makeMeasureNumbering(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test29, ConvenienceMakeFunctions )
{
	auto a = makeRootStep(  );
	auto b = makeRootStep(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test30, ConvenienceMakeFunctions )
{
	auto a = makeRootAlter(  );
	auto b = makeRootAlter(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test31, ConvenienceMakeFunctions )
{
	auto a = makeMidiDevice(  );
	auto b = makeMidiDevice(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test32, ConvenienceMakeFunctions )
{
	auto a = makeEncoder(  );
	auto b = makeEncoder(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test33, ConvenienceMakeFunctions )
{
	auto a = makeCreator(  );
	auto b = makeCreator(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test34, ConvenienceMakeFunctions )
{
	auto a = makeRights(  );
	auto b = makeRights(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test35, ConvenienceMakeFunctions )
{
	auto a = makeRelation(  );
	auto b = makeRelation(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test36, ConvenienceMakeFunctions )
{
	auto a = makeMiscellaneousField(  );
	auto b = makeMiscellaneousField(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test37, ConvenienceMakeFunctions )
{
	auto a = makeLineWidth(  );
	auto b = makeLineWidth(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test38, ConvenienceMakeFunctions )
{
	auto a = makeNoteSize(  );
	auto b = makeNoteSize(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test39, ConvenienceMakeFunctions )
{
	auto a = makeDistance(  );
	auto b = makeDistance(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test40, ConvenienceMakeFunctions )
{
	auto a = makeOtherAppearance(  );
	auto b = makeOtherAppearance(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test41, ConvenienceMakeFunctions )
{
	auto a = makeBreathMark(  );
	auto b = makeBreathMark(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test42, ConvenienceMakeFunctions )
{
	auto a = makeOtherArticulation(  );
	auto b = makeOtherArticulation(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test43, ConvenienceMakeFunctions )
{
	auto a = makeWithBar(  );
	auto b = makeWithBar(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test44, ConvenienceMakeFunctions )
{
	auto a = makePrefix(  );
	auto b = makePrefix(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test45, ConvenienceMakeFunctions )
{
	auto a = makeFigureNumber(  );
	auto b = makeFigureNumber(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test46, ConvenienceMakeFunctions )
{
	auto a = makeSuffix(  );
	auto b = makeSuffix(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test47, ConvenienceMakeFunctions )
{
	auto a = makeHoleClosed(  );
	auto b = makeHoleClosed(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test48, ConvenienceMakeFunctions )
{
	auto a = makeText(  );
	auto b = makeText(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test49, ConvenienceMakeFunctions )
{
	auto a = makeElision(  );
	auto b = makeElision(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test50, ConvenienceMakeFunctions )
{
	auto a = makeGlissando(  );
	auto b = makeGlissando(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test51, ConvenienceMakeFunctions )
{
	auto a = makeSlide(  );
	auto b = makeSlide(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test52, ConvenienceMakeFunctions )
{
	auto a = makeAccidentalMark(  );
	auto b = makeAccidentalMark(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test53, ConvenienceMakeFunctions )
{
	auto a = makeOtherNotation(  );
	auto b = makeOtherNotation(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test54, ConvenienceMakeFunctions )
{
	auto a = makeType(  );
	auto b = makeType(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test55, ConvenienceMakeFunctions )
{
	auto a = makeAccidental(  );
	auto b = makeAccidental(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test56, ConvenienceMakeFunctions )
{
	auto a = makeStem(  );
	auto b = makeStem(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test57, ConvenienceMakeFunctions )
{
	auto a = makeNotehead(  );
	auto b = makeNotehead(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test58, ConvenienceMakeFunctions )
{
	auto a = makeBeam(  );
	auto b = makeBeam(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test59, ConvenienceMakeFunctions )
{
	auto a = makeTremolo(  );
	auto b = makeTremolo(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test60, ConvenienceMakeFunctions )
{
	auto a = makeOtherOrnament(  );
	auto b = makeOtherOrnament(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test61, ConvenienceMakeFunctions )
{
	auto a = makePluck(  );
	auto b = makePluck(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test62, ConvenienceMakeFunctions )
{
	auto a = makeHammerOn(  );
	auto b = makeHammerOn(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test63, ConvenienceMakeFunctions )
{
	auto a = makePullOff(  );
	auto b = makePullOff(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test64, ConvenienceMakeFunctions )
{
	auto a = makeTap(  );
	auto b = makeTap(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test65, ConvenienceMakeFunctions )
{
	auto a = makeHandbell(  );
	auto b = makeHandbell(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test66, ConvenienceMakeFunctions )
{
	auto a = makeOtherTechnical(  );
	auto b = makeOtherTechnical(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test67, ConvenienceMakeFunctions )
{
	auto a = makeTupletNumber(  );
	auto b = makeTupletNumber(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test68, ConvenienceMakeFunctions )
{
	auto a = makeTupletType(  );
	auto b = makeTupletType(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test69, ConvenienceMakeFunctions )
{
	auto a = makeCreditWords(  );
	auto b = makeCreditWords(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test70, ConvenienceMakeFunctions )
{
	auto a = makeGroupName(  );
	auto b = makeGroupName(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test71, ConvenienceMakeFunctions )
{
	auto a = makeGroupAbbreviation(  );
	auto b = makeGroupAbbreviation(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test72, ConvenienceMakeFunctions )
{
	auto a = makeGroupSymbol(  );
	auto b = makeGroupSymbol(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test73, ConvenienceMakeFunctions )
{
	auto a = makeGroupBarline(  );
	auto b = makeGroupBarline(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test74, ConvenienceMakeFunctions )
{
	auto a = makePartName(  );
	auto b = makePartName(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test75, ConvenienceMakeFunctions )
{
	auto a = makePartAbbreviation(  );
	auto b = makePartAbbreviation(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test76, ConvenienceMakeFunctions )
{
	auto a = makeFootnote(  );
	auto b = makeFootnote(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test77, ConvenienceMakeFunctions )
{
	auto a = makeLevel(  );
	auto b = makeLevel(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test78, ConvenienceMakeFunctions )
{
	auto a = makeCancel(  );
	auto b = makeCancel(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test79, ConvenienceMakeFunctions )
{
	auto a = makeFunction(  );
	auto b = makeFunction(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test80, ConvenienceMakeFunctions )
{
	auto a = makeKind(  );
	auto b = makeKind(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test81, ConvenienceMakeFunctions )
{
	auto a = makeInversion(  );
	auto b = makeInversion(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test82, ConvenienceMakeFunctions )
{
	auto a = makeMidiChannel(  );
	auto b = makeMidiChannel(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test83, ConvenienceMakeFunctions )
{
	auto a = makeMidiName(  );
	auto b = makeMidiName(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test84, ConvenienceMakeFunctions )
{
	auto a = makeMidiBank(  );
	auto b = makeMidiBank(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test85, ConvenienceMakeFunctions )
{
	auto a = makeMidiProgram(  );
	auto b = makeMidiProgram(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test86, ConvenienceMakeFunctions )
{
	auto a = makeMidiUnpitched(  );
	auto b = makeMidiUnpitched(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test87, ConvenienceMakeFunctions )
{
	auto a = makeVolume(  );
	auto b = makeVolume(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test88, ConvenienceMakeFunctions )
{
	auto a = makePan(  );
	auto b = makePan(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test89, ConvenienceMakeFunctions )
{
	auto a = makeElevation(  );
	auto b = makeElevation(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test90, ConvenienceMakeFunctions )
{
	auto a = makeIpa(  );
	auto b = makeIpa(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test91, ConvenienceMakeFunctions )
{
	auto a = makeMute(  );
	auto b = makeMute(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test92, ConvenienceMakeFunctions )
{
	auto a = makeSemiPitched(  );
	auto b = makeSemiPitched(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test93, ConvenienceMakeFunctions )
{
	auto a = makeDivisions(  );
	auto b = makeDivisions(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test94, ConvenienceMakeFunctions )
{
	auto a = makeStaves(  );
	auto b = makeStaves(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test95, ConvenienceMakeFunctions )
{
	auto a = makeInstruments(  );
	auto b = makeInstruments(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test96, ConvenienceMakeFunctions )
{
	auto a = makeSign(  );
	auto b = makeSign(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test97, ConvenienceMakeFunctions )
{
	auto a = makeLine(  );
	auto b = makeLine(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test98, ConvenienceMakeFunctions )
{
	auto a = makeClefOctaveChange(  );
	auto b = makeClefOctaveChange(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test99, ConvenienceMakeFunctions )
{
	auto a = makeTimeRelation(  );
	auto b = makeTimeRelation(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test100, ConvenienceMakeFunctions )
{
	auto a = makeStaffType(  );
	auto b = makeStaffType(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test101, ConvenienceMakeFunctions )
{
	auto a = makeStaffLines(  );
	auto b = makeStaffLines(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test102, ConvenienceMakeFunctions )
{
	auto a = makeCapo(  );
	auto b = makeCapo(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test103, ConvenienceMakeFunctions )
{
	auto a = makeStaffSize(  );
	auto b = makeStaffSize(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test104, ConvenienceMakeFunctions )
{
	auto a = makeSenzaMisura(  );
	auto b = makeSenzaMisura(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test105, ConvenienceMakeFunctions )
{
	auto a = makeDiatonic(  );
	auto b = makeDiatonic(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test106, ConvenienceMakeFunctions )
{
	auto a = makeChromatic(  );
	auto b = makeChromatic(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test107, ConvenienceMakeFunctions )
{
	auto a = makeOctaveChange(  );
	auto b = makeOctaveChange(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test108, ConvenienceMakeFunctions )
{
	auto a = makeAccordionMiddle(  );
	auto b = makeAccordionMiddle(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test109, ConvenienceMakeFunctions )
{
	auto a = makeFrameStrings(  );
	auto b = makeFrameStrings(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test110, ConvenienceMakeFunctions )
{
	auto a = makeFrameFrets(  );
	auto b = makeFrameFrets(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test111, ConvenienceMakeFunctions )
{
	auto a = makeMetronomeType(  );
	auto b = makeMetronomeType(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test112, ConvenienceMakeFunctions )
{
	auto a = makePedalStep(  );
	auto b = makePedalStep(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test113, ConvenienceMakeFunctions )
{
	auto a = makePedalAlter(  );
	auto b = makePedalAlter(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test114, ConvenienceMakeFunctions )
{
	auto a = makeGlass(  );
	auto b = makeGlass(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test115, ConvenienceMakeFunctions )
{
	auto a = makeMetal(  );
	auto b = makeMetal(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test116, ConvenienceMakeFunctions )
{
	auto a = makeWood(  );
	auto b = makeWood(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test117, ConvenienceMakeFunctions )
{
	auto a = makePitched(  );
	auto b = makePitched(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test118, ConvenienceMakeFunctions )
{
	auto a = makeMembrane(  );
	auto b = makeMembrane(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test119, ConvenienceMakeFunctions )
{
	auto a = makeEffect(  );
	auto b = makeEffect(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test120, ConvenienceMakeFunctions )
{
	auto a = makeStickLocation(  );
	auto b = makeStickLocation(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test121, ConvenienceMakeFunctions )
{
	auto a = makeOtherPercussion(  );
	auto b = makeOtherPercussion(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test122, ConvenienceMakeFunctions )
{
	auto a = makeStickType(  );
	auto b = makeStickType(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test123, ConvenienceMakeFunctions )
{
	auto a = makeStickMaterial(  );
	auto b = makeStickMaterial(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test124, ConvenienceMakeFunctions )
{
	auto a = makeEncodingDate(  );
	auto b = makeEncodingDate(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test125, ConvenienceMakeFunctions )
{
	auto a = makeSoftware(  );
	auto b = makeSoftware(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test126, ConvenienceMakeFunctions )
{
	auto a = makeEncodingDescription(  );
	auto b = makeEncodingDescription(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test127, ConvenienceMakeFunctions )
{
	auto a = makeSource(  );
	auto b = makeSource(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test128, ConvenienceMakeFunctions )
{
	auto a = makeMeasureDistance(  );
	auto b = makeMeasureDistance(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test129, ConvenienceMakeFunctions )
{
	auto a = makePageHeight(  );
	auto b = makePageHeight(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test130, ConvenienceMakeFunctions )
{
	auto a = makePageWidth(  );
	auto b = makePageWidth(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test131, ConvenienceMakeFunctions )
{
	auto a = makeMillimeters(  );
	auto b = makeMillimeters(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test132, ConvenienceMakeFunctions )
{
	auto a = makeTenths(  );
	auto b = makeTenths(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test133, ConvenienceMakeFunctions )
{
	auto a = makeStaffDistance(  );
	auto b = makeStaffDistance(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test134, ConvenienceMakeFunctions )
{
	auto a = makeSystemDistance(  );
	auto b = makeSystemDistance(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test135, ConvenienceMakeFunctions )
{
	auto a = makeTopSystemDistance(  );
	auto b = makeTopSystemDistance(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test136, ConvenienceMakeFunctions )
{
	auto a = makeArrowDirection(  );
	auto b = makeArrowDirection(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test137, ConvenienceMakeFunctions )
{
	auto a = makeArrowStyle(  );
	auto b = makeArrowStyle(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test138, ConvenienceMakeFunctions )
{
	auto a = makeCircularArrow(  );
	auto b = makeCircularArrow(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test139, ConvenienceMakeFunctions )
{
	auto a = makeBendAlter(  );
	auto b = makeBendAlter(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test140, ConvenienceMakeFunctions )
{
	auto a = makeHoleType(  );
	auto b = makeHoleType(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test141, ConvenienceMakeFunctions )
{
	auto a = makeHoleShape(  );
	auto b = makeHoleShape(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test142, ConvenienceMakeFunctions )
{
	auto a = makeSyllabic(  );
	auto b = makeSyllabic(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test143, ConvenienceMakeFunctions )
{
	auto a = makeStep(  );
	auto b = makeStep(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test144, ConvenienceMakeFunctions )
{
	auto a = makeAlter(  );
	auto b = makeAlter(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test145, ConvenienceMakeFunctions )
{
	auto a = makeOctave(  );
	auto b = makeOctave(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test146, ConvenienceMakeFunctions )
{
	auto a = makeActualNotes(  );
	auto b = makeActualNotes(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test147, ConvenienceMakeFunctions )
{
	auto a = makeNormalNotes(  );
	auto b = makeNormalNotes(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test148, ConvenienceMakeFunctions )
{
	auto a = makeNormalType(  );
	auto b = makeNormalType(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test149, ConvenienceMakeFunctions )
{
	auto a = makeCreditType(  );
	auto b = makeCreditType(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test150, ConvenienceMakeFunctions )
{
	auto a = makeInstrumentName(  );
	auto b = makeInstrumentName(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test151, ConvenienceMakeFunctions )
{
	auto a = makeInstrumentAbbreviation(  );
	auto b = makeInstrumentAbbreviation(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test152, ConvenienceMakeFunctions )
{
	auto a = makeInstrumentSound(  );
	auto b = makeInstrumentSound(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test153, ConvenienceMakeFunctions )
{
	auto a = makeEnsemble(  );
	auto b = makeEnsemble(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test154, ConvenienceMakeFunctions )
{
	auto a = makeGroup(  );
	auto b = makeGroup(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test155, ConvenienceMakeFunctions )
{
	auto a = makeVirtualLibrary(  );
	auto b = makeVirtualLibrary(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test156, ConvenienceMakeFunctions )
{
	auto a = makeVirtualName(  );
	auto b = makeVirtualName(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test157, ConvenienceMakeFunctions )
{
	auto a = makeWorkNumber(  );
	auto b = makeWorkNumber(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test158, ConvenienceMakeFunctions )
{
	auto a = makeWorkTitle(  );
	auto b = makeWorkTitle(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test159, ConvenienceMakeFunctions )
{
	auto a = makeStaff(  );
	auto b = makeStaff(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test160, ConvenienceMakeFunctions )
{
	auto a = makeTuningStep(  );
	auto b = makeTuningStep(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test161, ConvenienceMakeFunctions )
{
	auto a = makeTuningAlter(  );
	auto b = makeTuningAlter(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test162, ConvenienceMakeFunctions )
{
	auto a = makeTuningOctave(  );
	auto b = makeTuningOctave(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test163, ConvenienceMakeFunctions )
{
	auto a = makeVoice(  );
	auto b = makeVoice(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test164, ConvenienceMakeFunctions )
{
	auto a = makeKeyStep(  );
	auto b = makeKeyStep(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test165, ConvenienceMakeFunctions )
{
	auto a = makeKeyAlter(  );
	auto b = makeKeyAlter(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test166, ConvenienceMakeFunctions )
{
	auto a = makeKeyAccidental(  );
	auto b = makeKeyAccidental(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test167, ConvenienceMakeFunctions )
{
	auto a = makeSlashType(  );
	auto b = makeSlashType(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test168, ConvenienceMakeFunctions )
{
	auto a = makeBeats(  );
	auto b = makeBeats(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test169, ConvenienceMakeFunctions )
{
	auto a = makeBeatType(  );
	auto b = makeBeatType(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test170, ConvenienceMakeFunctions )
{
	auto a = makeFifths(  );
	auto b = makeFifths(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test171, ConvenienceMakeFunctions )
{
	auto a = makeMode(  );
	auto b = makeMode(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test172, ConvenienceMakeFunctions )
{
	auto a = makeBeatUnit(  );
	auto b = makeBeatUnit(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test173, ConvenienceMakeFunctions )
{
	auto a = makeTopMargin(  );
	auto b = makeTopMargin(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test174, ConvenienceMakeFunctions )
{
	auto a = makeBottomMargin(  );
	auto b = makeBottomMargin(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test175, ConvenienceMakeFunctions )
{
	auto a = makeLeftMargin(  );
	auto b = makeLeftMargin(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test176, ConvenienceMakeFunctions )
{
	auto a = makeRightMargin(  );
	auto b = makeRightMargin(  );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test177, ConvenienceMakeFunctions )
{
    auto val = PositiveDivisionsValue{ 3.214234 };
	auto a = makeDuration( val );
	auto b = makeDuration( val );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test178, ConvenienceMakeFunctions )
{
    auto val = StepEnum::e;
	auto a = makeDisplayStep( val );
	auto b = makeDisplayStep(  val );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test179, ConvenienceMakeFunctions )
{
	auto a = makeDisplayOctave( OctaveValue{ 2 } );
	auto b = makeDisplayOctave( OctaveValue{ 2 } );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test180, ConvenienceMakeFunctions )
{
	auto a = makeMovementNumber( XsString{ "1" } );
	auto b = makeMovementNumber( XsString{ "1" } );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}
TEST( Test181, ConvenienceMakeFunctions )
{
	auto a = makeMovementTitle( XsString{ "Bones" } );
	auto b = makeMovementTitle( XsString{ "Bones" } );
	stringstream aa;
	stringstream bb;
	aa << *a;
	bb << *b;
	string expected = aa.str();
	string actual = bb.str();
	CHECK( a )
	CHECK( b )
	CHECK_EQUAL( expected, actual )
}

#endif
