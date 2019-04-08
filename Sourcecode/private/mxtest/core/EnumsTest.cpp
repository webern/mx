// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Enums.h"

using namespace mx::core;

TEST( AboveBelow_BadParse, Enums )
{
	AboveBelow actual = parseAboveBelow( "above" );
	AboveBelow expected = AboveBelow::above;
	CHECK_EQUAL( expected, actual )
}

TEST( AboveBelow_above, Enums )
{
	AboveBelow e = AboveBelow::above;
	std::string expected = "above";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AboveBelow e2 = parseAboveBelow( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AboveBelow_below, Enums )
{
	AboveBelow e = AboveBelow::below;
	std::string expected = "below";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AboveBelow e2 = parseAboveBelow( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( CssFontSize_BadParse, Enums )
{
	CssFontSize actual = parseCssFontSize( "xx-small" );
	CssFontSize expected = CssFontSize::xxSmall;
	CHECK_EQUAL( expected, actual )
}

TEST( CssFontSize_xxSmall, Enums )
{
	CssFontSize e = CssFontSize::xxSmall;
	std::string expected = "xx-small";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	CssFontSize e2 = parseCssFontSize( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( CssFontSize_xSmall, Enums )
{
	CssFontSize e = CssFontSize::xSmall;
	std::string expected = "x-small";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	CssFontSize e2 = parseCssFontSize( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( CssFontSize_small, Enums )
{
	CssFontSize e = CssFontSize::small;
	std::string expected = "small";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	CssFontSize e2 = parseCssFontSize( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( CssFontSize_medium, Enums )
{
	CssFontSize e = CssFontSize::medium;
	std::string expected = "medium";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	CssFontSize e2 = parseCssFontSize( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( CssFontSize_large, Enums )
{
	CssFontSize e = CssFontSize::large;
	std::string expected = "large";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	CssFontSize e2 = parseCssFontSize( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( CssFontSize_xLarge, Enums )
{
	CssFontSize e = CssFontSize::xLarge;
	std::string expected = "x-large";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	CssFontSize e2 = parseCssFontSize( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( CssFontSize_xxLarge, Enums )
{
	CssFontSize e = CssFontSize::xxLarge;
	std::string expected = "xx-large";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	CssFontSize e2 = parseCssFontSize( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( EnclosureShape_BadParse, Enums )
{
	EnclosureShape actual = parseEnclosureShape( "rectangle" );
	EnclosureShape expected = EnclosureShape::rectangle;
	CHECK_EQUAL( expected, actual )
}

TEST( EnclosureShape_rectangle, Enums )
{
	EnclosureShape e = EnclosureShape::rectangle;
	std::string expected = "rectangle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EnclosureShape e2 = parseEnclosureShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( EnclosureShape_square, Enums )
{
	EnclosureShape e = EnclosureShape::square;
	std::string expected = "square";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EnclosureShape e2 = parseEnclosureShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( EnclosureShape_oval, Enums )
{
	EnclosureShape e = EnclosureShape::oval;
	std::string expected = "oval";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EnclosureShape e2 = parseEnclosureShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( EnclosureShape_circle, Enums )
{
	EnclosureShape e = EnclosureShape::circle;
	std::string expected = "circle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EnclosureShape e2 = parseEnclosureShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( EnclosureShape_bracket, Enums )
{
	EnclosureShape e = EnclosureShape::bracket;
	std::string expected = "bracket";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EnclosureShape e2 = parseEnclosureShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( EnclosureShape_triangle, Enums )
{
	EnclosureShape e = EnclosureShape::triangle;
	std::string expected = "triangle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EnclosureShape e2 = parseEnclosureShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( EnclosureShape_diamond, Enums )
{
	EnclosureShape e = EnclosureShape::diamond;
	std::string expected = "diamond";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EnclosureShape e2 = parseEnclosureShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( EnclosureShape_none, Enums )
{
	EnclosureShape e = EnclosureShape::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EnclosureShape e2 = parseEnclosureShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( FermataShape_BadParse, Enums )
{
	FermataShape actual = parseFermataShape( "normal" );
	FermataShape expected = FermataShape::normal;
	CHECK_EQUAL( expected, actual )
}

TEST( FermataShape_normal, Enums )
{
	FermataShape e = FermataShape::normal;
	std::string expected = "normal";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	FermataShape e2 = parseFermataShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( FermataShape_angled, Enums )
{
	FermataShape e = FermataShape::angled;
	std::string expected = "angled";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	FermataShape e2 = parseFermataShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( FermataShape_square, Enums )
{
	FermataShape e = FermataShape::square;
	std::string expected = "square";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	FermataShape e2 = parseFermataShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( FermataShape_emptystring, Enums )
{
	FermataShape e = FermataShape::emptystring;
	std::string expected = "";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	FermataShape e2 = parseFermataShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( FontStyle_BadParse, Enums )
{
	FontStyle actual = parseFontStyle( "normal" );
	FontStyle expected = FontStyle::normal;
	CHECK_EQUAL( expected, actual )
}

TEST( FontStyle_normal, Enums )
{
	FontStyle e = FontStyle::normal;
	std::string expected = "normal";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	FontStyle e2 = parseFontStyle( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( FontStyle_italic, Enums )
{
	FontStyle e = FontStyle::italic;
	std::string expected = "italic";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	FontStyle e2 = parseFontStyle( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( FontWeight_BadParse, Enums )
{
	FontWeight actual = parseFontWeight( "normal" );
	FontWeight expected = FontWeight::normal;
	CHECK_EQUAL( expected, actual )
}

TEST( FontWeight_normal, Enums )
{
	FontWeight e = FontWeight::normal;
	std::string expected = "normal";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	FontWeight e2 = parseFontWeight( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( FontWeight_bold, Enums )
{
	FontWeight e = FontWeight::bold;
	std::string expected = "bold";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	FontWeight e2 = parseFontWeight( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LeftCenterRight_BadParse, Enums )
{
	LeftCenterRight actual = parseLeftCenterRight( "left" );
	LeftCenterRight expected = LeftCenterRight::left;
	CHECK_EQUAL( expected, actual )
}

TEST( LeftCenterRight_left, Enums )
{
	LeftCenterRight e = LeftCenterRight::left;
	std::string expected = "left";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LeftCenterRight e2 = parseLeftCenterRight( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LeftCenterRight_center, Enums )
{
	LeftCenterRight e = LeftCenterRight::center;
	std::string expected = "center";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LeftCenterRight e2 = parseLeftCenterRight( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LeftCenterRight_right, Enums )
{
	LeftCenterRight e = LeftCenterRight::right;
	std::string expected = "right";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LeftCenterRight e2 = parseLeftCenterRight( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LeftRight_BadParse, Enums )
{
	LeftRight actual = parseLeftRight( "left" );
	LeftRight expected = LeftRight::left;
	CHECK_EQUAL( expected, actual )
}

TEST( LeftRight_left, Enums )
{
	LeftRight e = LeftRight::left;
	std::string expected = "left";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LeftRight e2 = parseLeftRight( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LeftRight_right, Enums )
{
	LeftRight e = LeftRight::right;
	std::string expected = "right";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LeftRight e2 = parseLeftRight( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineShape_BadParse, Enums )
{
	LineShape actual = parseLineShape( "straight" );
	LineShape expected = LineShape::straight;
	CHECK_EQUAL( expected, actual )
}

TEST( LineShape_straight, Enums )
{
	LineShape e = LineShape::straight;
	std::string expected = "straight";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LineShape e2 = parseLineShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineShape_curved, Enums )
{
	LineShape e = LineShape::curved;
	std::string expected = "curved";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LineShape e2 = parseLineShape( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineType_BadParse, Enums )
{
	LineType actual = parseLineType( "solid" );
	LineType expected = LineType::solid;
	CHECK_EQUAL( expected, actual )
}

TEST( LineType_solid, Enums )
{
	LineType e = LineType::solid;
	std::string expected = "solid";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LineType e2 = parseLineType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineType_dashed, Enums )
{
	LineType e = LineType::dashed;
	std::string expected = "dashed";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LineType e2 = parseLineType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineType_dotted, Enums )
{
	LineType e = LineType::dotted;
	std::string expected = "dotted";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LineType e2 = parseLineType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineType_wavy, Enums )
{
	LineType e = LineType::wavy;
	std::string expected = "wavy";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LineType e2 = parseLineType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_BadParse, Enums )
{
	MuteEnum actual = parseMuteEnum( "on" );
	MuteEnum expected = MuteEnum::on;
	CHECK_EQUAL( expected, actual )
}

TEST( Mute_on, Enums )
{
	MuteEnum e = MuteEnum::on;
	std::string expected = "on";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_off, Enums )
{
	MuteEnum e = MuteEnum::off;
	std::string expected = "off";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_straight, Enums )
{
	MuteEnum e = MuteEnum::straight;
	std::string expected = "straight";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_cup, Enums )
{
	MuteEnum e = MuteEnum::cup;
	std::string expected = "cup";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_harmonNoStem, Enums )
{
	MuteEnum e = MuteEnum::harmonNoStem;
	std::string expected = "harmon-no-stem";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_harmonStem, Enums )
{
	MuteEnum e = MuteEnum::harmonStem;
	std::string expected = "harmon-stem";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_bucket, Enums )
{
	MuteEnum e = MuteEnum::bucket;
	std::string expected = "bucket";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_plunger, Enums )
{
	MuteEnum e = MuteEnum::plunger;
	std::string expected = "plunger";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_hat, Enums )
{
	MuteEnum e = MuteEnum::hat;
	std::string expected = "hat";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_solotone, Enums )
{
	MuteEnum e = MuteEnum::solotone;
	std::string expected = "solotone";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_practice, Enums )
{
	MuteEnum e = MuteEnum::practice;
	std::string expected = "practice";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_stopMute, Enums )
{
	MuteEnum e = MuteEnum::stopMute;
	std::string expected = "stop-mute";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_stopHand, Enums )
{
	MuteEnum e = MuteEnum::stopHand;
	std::string expected = "stop-hand";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_echo, Enums )
{
	MuteEnum e = MuteEnum::echo;
	std::string expected = "echo";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mute_palm, Enums )
{
	MuteEnum e = MuteEnum::palm;
	std::string expected = "palm";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MuteEnum e2 = parseMuteEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( OverUnder_BadParse, Enums )
{
	OverUnder actual = parseOverUnder( "over" );
	OverUnder expected = OverUnder::over;
	CHECK_EQUAL( expected, actual )
}

TEST( OverUnder_over, Enums )
{
	OverUnder e = OverUnder::over;
	std::string expected = "over";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	OverUnder e2 = parseOverUnder( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( OverUnder_under, Enums )
{
	OverUnder e = OverUnder::under;
	std::string expected = "under";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	OverUnder e2 = parseOverUnder( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( SemiPitchedEnum_BadParse, Enums )
{
	SemiPitchedEnum actual = parseSemiPitchedEnum( "high" );
	SemiPitchedEnum expected = SemiPitchedEnum::high;
	CHECK_EQUAL( expected, actual )
}

TEST( SemiPitchedEnum_high, Enums )
{
	SemiPitchedEnum e = SemiPitchedEnum::high;
	std::string expected = "high";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	SemiPitchedEnum e2 = parseSemiPitchedEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( SemiPitchedEnum_mediumHigh, Enums )
{
	SemiPitchedEnum e = SemiPitchedEnum::mediumHigh;
	std::string expected = "medium-high";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	SemiPitchedEnum e2 = parseSemiPitchedEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( SemiPitchedEnum_medium, Enums )
{
	SemiPitchedEnum e = SemiPitchedEnum::medium;
	std::string expected = "medium";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	SemiPitchedEnum e2 = parseSemiPitchedEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( SemiPitchedEnum_mediumLow, Enums )
{
	SemiPitchedEnum e = SemiPitchedEnum::mediumLow;
	std::string expected = "medium-low";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	SemiPitchedEnum e2 = parseSemiPitchedEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( SemiPitchedEnum_low, Enums )
{
	SemiPitchedEnum e = SemiPitchedEnum::low;
	std::string expected = "low";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	SemiPitchedEnum e2 = parseSemiPitchedEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( SemiPitchedEnum_veryLow, Enums )
{
	SemiPitchedEnum e = SemiPitchedEnum::veryLow;
	std::string expected = "very-low";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	SemiPitchedEnum e2 = parseSemiPitchedEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartNote_BadParse, Enums )
{
	StartNote actual = parseStartNote( "upper" );
	StartNote expected = StartNote::upper;
	CHECK_EQUAL( expected, actual )
}

TEST( StartNote_upper, Enums )
{
	StartNote e = StartNote::upper;
	std::string expected = "upper";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartNote e2 = parseStartNote( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartNote_main, Enums )
{
	StartNote e = StartNote::main;
	std::string expected = "main";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartNote e2 = parseStartNote( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartNote_below, Enums )
{
	StartNote e = StartNote::below;
	std::string expected = "below";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartNote e2 = parseStartNote( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStop_BadParse, Enums )
{
	StartStop actual = parseStartStop( "start" );
	StartStop expected = StartStop::start;
	CHECK_EQUAL( expected, actual )
}

TEST( StartStop_start, Enums )
{
	StartStop e = StartStop::start;
	std::string expected = "start";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStop e2 = parseStartStop( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStop_stop, Enums )
{
	StartStop e = StartStop::stop;
	std::string expected = "stop";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStop e2 = parseStartStop( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStopContinue_BadParse, Enums )
{
	StartStopContinue actual = parseStartStopContinue( "start" );
	StartStopContinue expected = StartStopContinue::start;
	CHECK_EQUAL( expected, actual )
}

TEST( StartStopContinue_start, Enums )
{
	StartStopContinue e = StartStopContinue::start;
	std::string expected = "start";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStopContinue e2 = parseStartStopContinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStopContinue_stop, Enums )
{
	StartStopContinue e = StartStopContinue::stop;
	std::string expected = "stop";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStopContinue e2 = parseStartStopContinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStopContinue_continue_, Enums )
{
	StartStopContinue e = StartStopContinue::continue_;
	std::string expected = "continue";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStopContinue e2 = parseStartStopContinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStopSingle_BadParse, Enums )
{
	StartStopSingle actual = parseStartStopSingle( "start" );
	StartStopSingle expected = StartStopSingle::start;
	CHECK_EQUAL( expected, actual )
}

TEST( StartStopSingle_start, Enums )
{
	StartStopSingle e = StartStopSingle::start;
	std::string expected = "start";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStopSingle e2 = parseStartStopSingle( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStopSingle_stop, Enums )
{
	StartStopSingle e = StartStopSingle::stop;
	std::string expected = "stop";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStopSingle e2 = parseStartStopSingle( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStopSingle_single, Enums )
{
	StartStopSingle e = StartStopSingle::single;
	std::string expected = "single";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStopSingle e2 = parseStartStopSingle( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( SymbolSize_BadParse, Enums )
{
	SymbolSize actual = parseSymbolSize( "full" );
	SymbolSize expected = SymbolSize::full;
	CHECK_EQUAL( expected, actual )
}

TEST( SymbolSize_full, Enums )
{
	SymbolSize e = SymbolSize::full;
	std::string expected = "full";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	SymbolSize e2 = parseSymbolSize( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( SymbolSize_cue, Enums )
{
	SymbolSize e = SymbolSize::cue;
	std::string expected = "cue";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	SymbolSize e2 = parseSymbolSize( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( SymbolSize_large, Enums )
{
	SymbolSize e = SymbolSize::large;
	std::string expected = "large";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	SymbolSize e2 = parseSymbolSize( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TextDirection_BadParse, Enums )
{
	TextDirection actual = parseTextDirection( "ltr" );
	TextDirection expected = TextDirection::ltr;
	CHECK_EQUAL( expected, actual )
}

TEST( TextDirection_ltr, Enums )
{
	TextDirection e = TextDirection::ltr;
	std::string expected = "ltr";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TextDirection e2 = parseTextDirection( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TextDirection_rtl, Enums )
{
	TextDirection e = TextDirection::rtl;
	std::string expected = "rtl";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TextDirection e2 = parseTextDirection( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TextDirection_lro, Enums )
{
	TextDirection e = TextDirection::lro;
	std::string expected = "lro";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TextDirection e2 = parseTextDirection( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TextDirection_rlo, Enums )
{
	TextDirection e = TextDirection::rlo;
	std::string expected = "rlo";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TextDirection e2 = parseTextDirection( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TopBottom_BadParse, Enums )
{
	TopBottom actual = parseTopBottom( "top" );
	TopBottom expected = TopBottom::top;
	CHECK_EQUAL( expected, actual )
}

TEST( TopBottom_top, Enums )
{
	TopBottom e = TopBottom::top;
	std::string expected = "top";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TopBottom e2 = parseTopBottom( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TopBottom_bottom, Enums )
{
	TopBottom e = TopBottom::bottom;
	std::string expected = "bottom";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TopBottom e2 = parseTopBottom( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TrillStep_BadParse, Enums )
{
	TrillStep actual = parseTrillStep( "whole" );
	TrillStep expected = TrillStep::whole;
	CHECK_EQUAL( expected, actual )
}

TEST( TrillStep_whole, Enums )
{
	TrillStep e = TrillStep::whole;
	std::string expected = "whole";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TrillStep e2 = parseTrillStep( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TrillStep_half, Enums )
{
	TrillStep e = TrillStep::half;
	std::string expected = "half";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TrillStep e2 = parseTrillStep( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TrillStep_unison, Enums )
{
	TrillStep e = TrillStep::unison;
	std::string expected = "unison";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TrillStep e2 = parseTrillStep( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TwoNoteTurn_BadParse, Enums )
{
	TwoNoteTurn actual = parseTwoNoteTurn( "whole" );
	TwoNoteTurn expected = TwoNoteTurn::whole;
	CHECK_EQUAL( expected, actual )
}

TEST( TwoNoteTurn_whole, Enums )
{
	TwoNoteTurn e = TwoNoteTurn::whole;
	std::string expected = "whole";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TwoNoteTurn e2 = parseTwoNoteTurn( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TwoNoteTurn_half, Enums )
{
	TwoNoteTurn e = TwoNoteTurn::half;
	std::string expected = "half";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TwoNoteTurn e2 = parseTwoNoteTurn( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TwoNoteTurn_none, Enums )
{
	TwoNoteTurn e = TwoNoteTurn::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TwoNoteTurn e2 = parseTwoNoteTurn( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( UpDown_BadParse, Enums )
{
	UpDown actual = parseUpDown( "up" );
	UpDown expected = UpDown::up;
	CHECK_EQUAL( expected, actual )
}

TEST( UpDown_up, Enums )
{
	UpDown e = UpDown::up;
	std::string expected = "up";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	UpDown e2 = parseUpDown( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( UpDown_down, Enums )
{
	UpDown e = UpDown::down;
	std::string expected = "down";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	UpDown e2 = parseUpDown( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( UprightInverted_BadParse, Enums )
{
	UprightInverted actual = parseUprightInverted( "upright" );
	UprightInverted expected = UprightInverted::upright;
	CHECK_EQUAL( expected, actual )
}

TEST( UprightInverted_upright, Enums )
{
	UprightInverted e = UprightInverted::upright;
	std::string expected = "upright";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	UprightInverted e2 = parseUprightInverted( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( UprightInverted_inverted, Enums )
{
	UprightInverted e = UprightInverted::inverted;
	std::string expected = "inverted";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	UprightInverted e2 = parseUprightInverted( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Valign_BadParse, Enums )
{
	Valign actual = parseValign( "top" );
	Valign expected = Valign::top;
	CHECK_EQUAL( expected, actual )
}

TEST( Valign_top, Enums )
{
	Valign e = Valign::top;
	std::string expected = "top";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	Valign e2 = parseValign( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Valign_middle, Enums )
{
	Valign e = Valign::middle;
	std::string expected = "middle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	Valign e2 = parseValign( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Valign_bottom, Enums )
{
	Valign e = Valign::bottom;
	std::string expected = "bottom";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	Valign e2 = parseValign( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Valign_baseline, Enums )
{
	Valign e = Valign::baseline;
	std::string expected = "baseline";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	Valign e2 = parseValign( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ValignImage_BadParse, Enums )
{
	ValignImage actual = parseValignImage( "top" );
	ValignImage expected = ValignImage::top;
	CHECK_EQUAL( expected, actual )
}

TEST( ValignImage_top, Enums )
{
	ValignImage e = ValignImage::top;
	std::string expected = "top";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ValignImage e2 = parseValignImage( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ValignImage_middle, Enums )
{
	ValignImage e = ValignImage::middle;
	std::string expected = "middle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ValignImage e2 = parseValignImage( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ValignImage_bottom, Enums )
{
	ValignImage e = ValignImage::bottom;
	std::string expected = "bottom";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ValignImage e2 = parseValignImage( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( YesNo_BadParse, Enums )
{
	YesNo actual = parseYesNo( "yes" );
	YesNo expected = YesNo::yes;
	CHECK_EQUAL( expected, actual )
}

TEST( YesNo_yes, Enums )
{
	YesNo e = YesNo::yes;
	std::string expected = "yes";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	YesNo e2 = parseYesNo( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( YesNo_no, Enums )
{
	YesNo e = YesNo::no;
	std::string expected = "no";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	YesNo e2 = parseYesNo( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( CancelLocation_BadParse, Enums )
{
	CancelLocation actual = parseCancelLocation( "left" );
	CancelLocation expected = CancelLocation::left;
	CHECK_EQUAL( expected, actual )
}

TEST( CancelLocation_left, Enums )
{
	CancelLocation e = CancelLocation::left;
	std::string expected = "left";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	CancelLocation e2 = parseCancelLocation( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( CancelLocation_right, Enums )
{
	CancelLocation e = CancelLocation::right;
	std::string expected = "right";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	CancelLocation e2 = parseCancelLocation( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( CancelLocation_beforeBarline, Enums )
{
	CancelLocation e = CancelLocation::beforeBarline;
	std::string expected = "before-barline";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	CancelLocation e2 = parseCancelLocation( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ClefSign_BadParse, Enums )
{
	ClefSign actual = parseClefSign( "G" );
	ClefSign expected = ClefSign::g;
	CHECK_EQUAL( expected, actual )
}

TEST( ClefSign_g, Enums )
{
	ClefSign e = ClefSign::g;
	std::string expected = "G";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ClefSign e2 = parseClefSign( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ClefSign_f, Enums )
{
	ClefSign e = ClefSign::f;
	std::string expected = "F";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ClefSign e2 = parseClefSign( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ClefSign_c, Enums )
{
	ClefSign e = ClefSign::c;
	std::string expected = "C";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ClefSign e2 = parseClefSign( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ClefSign_percussion, Enums )
{
	ClefSign e = ClefSign::percussion;
	std::string expected = "percussion";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ClefSign e2 = parseClefSign( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ClefSign_tab, Enums )
{
	ClefSign e = ClefSign::tab;
	std::string expected = "TAB";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ClefSign e2 = parseClefSign( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ClefSign_jianpu, Enums )
{
	ClefSign e = ClefSign::jianpu;
	std::string expected = "jianpu";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ClefSign e2 = parseClefSign( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ClefSign_none, Enums )
{
	ClefSign e = ClefSign::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ClefSign e2 = parseClefSign( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ShowFrets_BadParse, Enums )
{
	ShowFrets actual = parseShowFrets( "numbers" );
	ShowFrets expected = ShowFrets::numbers;
	CHECK_EQUAL( expected, actual )
}

TEST( ShowFrets_numbers, Enums )
{
	ShowFrets e = ShowFrets::numbers;
	std::string expected = "numbers";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ShowFrets e2 = parseShowFrets( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ShowFrets_letters, Enums )
{
	ShowFrets e = ShowFrets::letters;
	std::string expected = "letters";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ShowFrets e2 = parseShowFrets( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StaffType_BadParse, Enums )
{
	StaffTypeEnum actual = parseStaffTypeEnum ( "ossia" );
	StaffTypeEnum expected = StaffTypeEnum::ossia;
	CHECK_EQUAL( expected, actual )
}

TEST( StaffType_ossia, Enums )
{
	StaffTypeEnum e = StaffTypeEnum::ossia;
	std::string expected = "ossia";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StaffTypeEnum e2 = parseStaffTypeEnum ( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StaffType_cue, Enums )
{
	StaffTypeEnum e = StaffTypeEnum::cue;
	std::string expected = "cue";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StaffTypeEnum e2 = parseStaffTypeEnum ( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StaffType_editorial, Enums )
{
	StaffTypeEnum e = StaffTypeEnum::editorial;
	std::string expected = "editorial";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StaffTypeEnum e2 = parseStaffTypeEnum ( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StaffType_regular, Enums )
{
	StaffTypeEnum e = StaffTypeEnum::regular;
	std::string expected = "regular";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StaffTypeEnum e2 = parseStaffTypeEnum ( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StaffType_alternate, Enums )
{
	StaffTypeEnum e = StaffTypeEnum::alternate;
	std::string expected = "alternate";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StaffTypeEnum e2 = parseStaffTypeEnum ( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeRelation_BadParse, Enums )
{
	TimeRelationEnum actual = parseTimeRelationEnum( "parentheses" );
	TimeRelationEnum expected = TimeRelationEnum::parentheses;
	CHECK_EQUAL( expected, actual )
}

TEST( TimeRelation_parentheses, Enums )
{
	TimeRelationEnum e = TimeRelationEnum::parentheses;
	std::string expected = "parentheses";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeRelationEnum e2 = parseTimeRelationEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeRelation_bracket, Enums )
{
	TimeRelationEnum e = TimeRelationEnum::bracket;
	std::string expected = "bracket";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeRelationEnum e2 = parseTimeRelationEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeRelation_equals, Enums )
{
	TimeRelationEnum e = TimeRelationEnum::equals;
	std::string expected = "equals";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeRelationEnum e2 = parseTimeRelationEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeRelation_slash, Enums )
{
	TimeRelationEnum e = TimeRelationEnum::slash;
	std::string expected = "slash";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeRelationEnum e2 = parseTimeRelationEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeRelation_space, Enums )
{
	TimeRelationEnum e = TimeRelationEnum::space;
	std::string expected = "space";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeRelationEnum e2 = parseTimeRelationEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeRelation_hyphen, Enums )
{
	TimeRelationEnum e = TimeRelationEnum::hyphen;
	std::string expected = "hyphen";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeRelationEnum e2 = parseTimeRelationEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeSeparator_BadParse, Enums )
{
	TimeSeparator actual = parseTimeSeparator( "none" );
	TimeSeparator expected = TimeSeparator::none;
	CHECK_EQUAL( expected, actual )
}

TEST( TimeSeparator_none, Enums )
{
	TimeSeparator e = TimeSeparator::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeSeparator e2 = parseTimeSeparator( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeSeparator_horizontal, Enums )
{
	TimeSeparator e = TimeSeparator::horizontal;
	std::string expected = "horizontal";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeSeparator e2 = parseTimeSeparator( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeSeparator_diagonal, Enums )
{
	TimeSeparator e = TimeSeparator::diagonal;
	std::string expected = "diagonal";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeSeparator e2 = parseTimeSeparator( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeSeparator_vertical, Enums )
{
	TimeSeparator e = TimeSeparator::vertical;
	std::string expected = "vertical";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeSeparator e2 = parseTimeSeparator( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeSeparator_adjacent, Enums )
{
	TimeSeparator e = TimeSeparator::adjacent;
	std::string expected = "adjacent";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeSeparator e2 = parseTimeSeparator( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeSymbol_BadParse, Enums )
{
	TimeSymbol actual = parseTimeSymbol( "common" );
	TimeSymbol expected = TimeSymbol::common;
	CHECK_EQUAL( expected, actual )
}

TEST( TimeSymbol_common, Enums )
{
	TimeSymbol e = TimeSymbol::common;
	std::string expected = "common";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeSymbol e2 = parseTimeSymbol( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeSymbol_cut, Enums )
{
	TimeSymbol e = TimeSymbol::cut;
	std::string expected = "cut";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeSymbol e2 = parseTimeSymbol( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeSymbol_singleNumber, Enums )
{
	TimeSymbol e = TimeSymbol::singleNumber;
	std::string expected = "single-number";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeSymbol e2 = parseTimeSymbol( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeSymbol_note, Enums )
{
	TimeSymbol e = TimeSymbol::note;
	std::string expected = "note";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeSymbol e2 = parseTimeSymbol( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeSymbol_dottedNote, Enums )
{
	TimeSymbol e = TimeSymbol::dottedNote;
	std::string expected = "dotted-note";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeSymbol e2 = parseTimeSymbol( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TimeSymbol_normal, Enums )
{
	TimeSymbol e = TimeSymbol::normal;
	std::string expected = "normal";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TimeSymbol e2 = parseTimeSymbol( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BackwardForward_BadParse, Enums )
{
	BackwardForward actual = parseBackwardForward( "backward" );
	BackwardForward expected = BackwardForward::backward;
	CHECK_EQUAL( expected, actual )
}

TEST( BackwardForward_backward, Enums )
{
	BackwardForward e = BackwardForward::backward;
	std::string expected = "backward";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BackwardForward e2 = parseBackwardForward( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BackwardForward_forward, Enums )
{
	BackwardForward e = BackwardForward::forward;
	std::string expected = "forward";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BackwardForward e2 = parseBackwardForward( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BarStyleEnum_BadParse, Enums )
{
	BarStyleEnum actual = parseBarStyleEnum( "regular" );
	BarStyleEnum expected = BarStyleEnum::regular;
	CHECK_EQUAL( expected, actual )
}

TEST( BarStyleEnum_regular, Enums )
{
	BarStyleEnum e = BarStyleEnum::regular;
	std::string expected = "regular";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BarStyleEnum e2 = parseBarStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BarStyleEnum_dotted, Enums )
{
	BarStyleEnum e = BarStyleEnum::dotted;
	std::string expected = "dotted";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BarStyleEnum e2 = parseBarStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BarStyleEnum_dashed, Enums )
{
	BarStyleEnum e = BarStyleEnum::dashed;
	std::string expected = "dashed";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BarStyleEnum e2 = parseBarStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BarStyleEnum_heavy, Enums )
{
	BarStyleEnum e = BarStyleEnum::heavy;
	std::string expected = "heavy";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BarStyleEnum e2 = parseBarStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BarStyleEnum_lightLight, Enums )
{
	BarStyleEnum e = BarStyleEnum::lightLight;
	std::string expected = "light-light";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BarStyleEnum e2 = parseBarStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BarStyleEnum_lightHeavy, Enums )
{
	BarStyleEnum e = BarStyleEnum::lightHeavy;
	std::string expected = "light-heavy";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BarStyleEnum e2 = parseBarStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BarStyleEnum_heavyLight, Enums )
{
	BarStyleEnum e = BarStyleEnum::heavyLight;
	std::string expected = "heavy-light";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BarStyleEnum e2 = parseBarStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BarStyleEnum_heavyHeavy, Enums )
{
	BarStyleEnum e = BarStyleEnum::heavyHeavy;
	std::string expected = "heavy-heavy";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BarStyleEnum e2 = parseBarStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BarStyleEnum_tick, Enums )
{
	BarStyleEnum e = BarStyleEnum::tick;
	std::string expected = "tick";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BarStyleEnum e2 = parseBarStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BarStyleEnum_short_, Enums )
{
	BarStyleEnum e = BarStyleEnum::short_;
	std::string expected = "short";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BarStyleEnum e2 = parseBarStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BarStyleEnum_none, Enums )
{
	BarStyleEnum e = BarStyleEnum::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BarStyleEnum e2 = parseBarStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( RightLeftMiddle_BadParse, Enums )
{
	RightLeftMiddle actual = parseRightLeftMiddle( "right" );
	RightLeftMiddle expected = RightLeftMiddle::right;
	CHECK_EQUAL( expected, actual )
}

TEST( RightLeftMiddle_right, Enums )
{
	RightLeftMiddle e = RightLeftMiddle::right;
	std::string expected = "right";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	RightLeftMiddle e2 = parseRightLeftMiddle( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( RightLeftMiddle_left, Enums )
{
	RightLeftMiddle e = RightLeftMiddle::left;
	std::string expected = "left";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	RightLeftMiddle e2 = parseRightLeftMiddle( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( RightLeftMiddle_middle, Enums )
{
	RightLeftMiddle e = RightLeftMiddle::middle;
	std::string expected = "middle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	RightLeftMiddle e2 = parseRightLeftMiddle( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStopDiscontinue_BadParse, Enums )
{
	StartStopDiscontinue actual = parseStartStopDiscontinue( "start" );
	StartStopDiscontinue expected = StartStopDiscontinue::start;
	CHECK_EQUAL( expected, actual )
}

TEST( StartStopDiscontinue_start, Enums )
{
	StartStopDiscontinue e = StartStopDiscontinue::start;
	std::string expected = "start";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStopDiscontinue e2 = parseStartStopDiscontinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStopDiscontinue_stop, Enums )
{
	StartStopDiscontinue e = StartStopDiscontinue::stop;
	std::string expected = "stop";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStopDiscontinue e2 = parseStartStopDiscontinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStopDiscontinue_discontinue, Enums )
{
	StartStopDiscontinue e = StartStopDiscontinue::discontinue;
	std::string expected = "discontinue";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStopDiscontinue e2 = parseStartStopDiscontinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Winged_BadParse, Enums )
{
	Winged actual = parseWinged( "none" );
	Winged expected = Winged::none;
	CHECK_EQUAL( expected, actual )
}

TEST( Winged_none, Enums )
{
	Winged e = Winged::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	Winged e2 = parseWinged( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Winged_straight, Enums )
{
	Winged e = Winged::straight;
	std::string expected = "straight";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	Winged e2 = parseWinged( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Winged_curved, Enums )
{
	Winged e = Winged::curved;
	std::string expected = "curved";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	Winged e2 = parseWinged( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Winged_doubleStraight, Enums )
{
	Winged e = Winged::doubleStraight;
	std::string expected = "double-straight";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	Winged e2 = parseWinged( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Winged_doubleCurved, Enums )
{
	Winged e = Winged::doubleCurved;
	std::string expected = "double-curved";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	Winged e2 = parseWinged( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_BadParse, Enums )
{
	BeaterValue actual = parseBeaterValue( "bow" );
	BeaterValue expected = BeaterValue::bow;
	CHECK_EQUAL( expected, actual )
}

TEST( BeaterValue_bow, Enums )
{
	BeaterValue e = BeaterValue::bow;
	std::string expected = "bow";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_chimeHammer, Enums )
{
	BeaterValue e = BeaterValue::chimeHammer;
	std::string expected = "chime hammer";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_coin, Enums )
{
	BeaterValue e = BeaterValue::coin;
	std::string expected = "coin";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_finger, Enums )
{
	BeaterValue e = BeaterValue::finger;
	std::string expected = "finger";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_fingernail, Enums )
{
	BeaterValue e = BeaterValue::fingernail;
	std::string expected = "fingernail";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_fist, Enums )
{
	BeaterValue e = BeaterValue::fist;
	std::string expected = "fist";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_guiroScraper, Enums )
{
	BeaterValue e = BeaterValue::guiroScraper;
	std::string expected = "guiro scraper";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_hammer, Enums )
{
	BeaterValue e = BeaterValue::hammer;
	std::string expected = "hammer";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_hand, Enums )
{
	BeaterValue e = BeaterValue::hand;
	std::string expected = "hand";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_jazzStick, Enums )
{
	BeaterValue e = BeaterValue::jazzStick;
	std::string expected = "jazz stick";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_knittingNeedle, Enums )
{
	BeaterValue e = BeaterValue::knittingNeedle;
	std::string expected = "knitting needle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_metalHammer, Enums )
{
	BeaterValue e = BeaterValue::metalHammer;
	std::string expected = "metal hammer";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_snareStick, Enums )
{
	BeaterValue e = BeaterValue::snareStick;
	std::string expected = "snare stick";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_spoonMallet, Enums )
{
	BeaterValue e = BeaterValue::spoonMallet;
	std::string expected = "spoon mallet";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_triangleBeater, Enums )
{
	BeaterValue e = BeaterValue::triangleBeater;
	std::string expected = "triangle beater";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_triangleBeaterPlain, Enums )
{
	BeaterValue e = BeaterValue::triangleBeaterPlain;
	std::string expected = "triangle beater plain";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeaterValue_wireBrush, Enums )
{
	BeaterValue e = BeaterValue::wireBrush;
	std::string expected = "wire brush";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeaterValue e2 = parseBeaterValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( DegreeSymbolValue_BadParse, Enums )
{
	DegreeSymbolValue actual = parseDegreeSymbolValue( "major" );
	DegreeSymbolValue expected = DegreeSymbolValue::major;
	CHECK_EQUAL( expected, actual )
}

TEST( DegreeSymbolValue_major, Enums )
{
	DegreeSymbolValue e = DegreeSymbolValue::major;
	std::string expected = "major";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	DegreeSymbolValue e2 = parseDegreeSymbolValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( DegreeSymbolValue_minor, Enums )
{
	DegreeSymbolValue e = DegreeSymbolValue::minor;
	std::string expected = "minor";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	DegreeSymbolValue e2 = parseDegreeSymbolValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( DegreeSymbolValue_augmented, Enums )
{
	DegreeSymbolValue e = DegreeSymbolValue::augmented;
	std::string expected = "augmented";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	DegreeSymbolValue e2 = parseDegreeSymbolValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( DegreeSymbolValue_diminished, Enums )
{
	DegreeSymbolValue e = DegreeSymbolValue::diminished;
	std::string expected = "diminished";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	DegreeSymbolValue e2 = parseDegreeSymbolValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( DegreeSymbolValue_halfDiminished, Enums )
{
	DegreeSymbolValue e = DegreeSymbolValue::halfDiminished;
	std::string expected = "half-diminished";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	DegreeSymbolValue e2 = parseDegreeSymbolValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( DegreeTypeValue_BadParse, Enums )
{
	DegreeTypeValue actual = parseDegreeTypeValue( "add" );
	DegreeTypeValue expected = DegreeTypeValue::add;
	CHECK_EQUAL( expected, actual )
}

TEST( DegreeTypeValue_add, Enums )
{
	DegreeTypeValue e = DegreeTypeValue::add;
	std::string expected = "add";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	DegreeTypeValue e2 = parseDegreeTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( DegreeTypeValue_alter, Enums )
{
	DegreeTypeValue e = DegreeTypeValue::alter;
	std::string expected = "alter";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	DegreeTypeValue e2 = parseDegreeTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( DegreeTypeValue_subtract, Enums )
{
	DegreeTypeValue e = DegreeTypeValue::subtract;
	std::string expected = "subtract";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	DegreeTypeValue e2 = parseDegreeTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_BadParse, Enums )
{
	EffectEnum actual = parseEffectEnum( "anvil" );
	EffectEnum expected = EffectEnum::anvil;
	CHECK_EQUAL( expected, actual )
}

TEST( Effect_anvil, Enums )
{
	EffectEnum e = EffectEnum::anvil;
	std::string expected = "anvil";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_autoHorn, Enums )
{
	EffectEnum e = EffectEnum::autoHorn;
	std::string expected = "auto horn";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_birdWhistle, Enums )
{
	EffectEnum e = EffectEnum::birdWhistle;
	std::string expected = "bird whistle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_cannon, Enums )
{
	EffectEnum e = EffectEnum::cannon;
	std::string expected = "cannon";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_duckCall, Enums )
{
	EffectEnum e = EffectEnum::duckCall;
	std::string expected = "duck call";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_gunShot, Enums )
{
	EffectEnum e = EffectEnum::gunShot;
	std::string expected = "gun shot";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_klaxonHorn, Enums )
{
	EffectEnum e = EffectEnum::klaxonHorn;
	std::string expected = "klaxon horn";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_lionsRoar, Enums )
{
	EffectEnum e = EffectEnum::lionsRoar;
	std::string expected = "lions roar";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_policeWhistle, Enums )
{
	EffectEnum e = EffectEnum::policeWhistle;
	std::string expected = "police whistle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_siren, Enums )
{
	EffectEnum e = EffectEnum::siren;
	std::string expected = "siren";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_slideWhistle, Enums )
{
	EffectEnum e = EffectEnum::slideWhistle;
	std::string expected = "slide whistle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_thunderSheet, Enums )
{
	EffectEnum e = EffectEnum::thunderSheet;
	std::string expected = "thunder sheet";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_windMachine, Enums )
{
	EffectEnum e = EffectEnum::windMachine;
	std::string expected = "wind machine";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Effect_windWhistle, Enums )
{
	EffectEnum e = EffectEnum::windWhistle;
	std::string expected = "wind whistle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	EffectEnum e2 = parseEffectEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Glass_BadParse, Enums )
{
	GlassEnum actual = parseGlassEnum( "wind chimes" );
	GlassEnum expected = GlassEnum::windChimes;
	CHECK_EQUAL( expected, actual )
}

TEST( Glass_windChimes, Enums )
{
	GlassEnum e = GlassEnum::windChimes;
	std::string expected = "wind chimes";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	GlassEnum e2 = parseGlassEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HarmonyType_BadParse, Enums )
{
	HarmonyType actual = parseHarmonyType( "explicit" );
	HarmonyType expected = HarmonyType::explicit_;
	CHECK_EQUAL( expected, actual )
}

TEST( HarmonyType_explicit_, Enums )
{
	HarmonyType e = HarmonyType::explicit_;
	std::string expected = "explicit";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HarmonyType e2 = parseHarmonyType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HarmonyType_implied, Enums )
{
	HarmonyType e = HarmonyType::implied;
	std::string expected = "implied";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HarmonyType e2 = parseHarmonyType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HarmonyType_alternate, Enums )
{
	HarmonyType e = HarmonyType::alternate;
	std::string expected = "alternate";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HarmonyType e2 = parseHarmonyType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_BadParse, Enums )
{
	KindValue actual = parseKindValue( "major" );
	KindValue expected = KindValue::major;
	CHECK_EQUAL( expected, actual )
}

TEST( KindValue_major, Enums )
{
	KindValue e = KindValue::major;
	std::string expected = "major";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_minor, Enums )
{
	KindValue e = KindValue::minor;
	std::string expected = "minor";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_augmented, Enums )
{
	KindValue e = KindValue::augmented;
	std::string expected = "augmented";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_diminished, Enums )
{
	KindValue e = KindValue::diminished;
	std::string expected = "diminished";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_dominant, Enums )
{
	KindValue e = KindValue::dominant;
	std::string expected = "dominant";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_majorSeventh, Enums )
{
	KindValue e = KindValue::majorSeventh;
	std::string expected = "major-seventh";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_minorSeventh, Enums )
{
	KindValue e = KindValue::minorSeventh;
	std::string expected = "minor-seventh";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_diminishedSeventh, Enums )
{
	KindValue e = KindValue::diminishedSeventh;
	std::string expected = "diminished-seventh";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_augmentedSeventh, Enums )
{
	KindValue e = KindValue::augmentedSeventh;
	std::string expected = "augmented-seventh";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_halfDiminished, Enums )
{
	KindValue e = KindValue::halfDiminished;
	std::string expected = "half-diminished";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_majorMinor, Enums )
{
	KindValue e = KindValue::majorMinor;
	std::string expected = "major-minor";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_majorSixth, Enums )
{
	KindValue e = KindValue::majorSixth;
	std::string expected = "major-sixth";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_minorSixth, Enums )
{
	KindValue e = KindValue::minorSixth;
	std::string expected = "minor-sixth";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_dominantNinth, Enums )
{
	KindValue e = KindValue::dominantNinth;
	std::string expected = "dominant-ninth";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_majorNinth, Enums )
{
	KindValue e = KindValue::majorNinth;
	std::string expected = "major-ninth";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_minorNinth, Enums )
{
	KindValue e = KindValue::minorNinth;
	std::string expected = "minor-ninth";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_dominant11Th, Enums )
{
	KindValue e = KindValue::dominant11Th;
	std::string expected = "dominant-11th";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_major11Th, Enums )
{
	KindValue e = KindValue::major11Th;
	std::string expected = "major-11th";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_minor11Th, Enums )
{
	KindValue e = KindValue::minor11Th;
	std::string expected = "minor-11th";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_dominant13Th, Enums )
{
	KindValue e = KindValue::dominant13Th;
	std::string expected = "dominant-13th";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_major13Th, Enums )
{
	KindValue e = KindValue::major13Th;
	std::string expected = "major-13th";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_minor13Th, Enums )
{
	KindValue e = KindValue::minor13Th;
	std::string expected = "minor-13th";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_suspendedSecond, Enums )
{
	KindValue e = KindValue::suspendedSecond;
	std::string expected = "suspended-second";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_suspendedFourth, Enums )
{
	KindValue e = KindValue::suspendedFourth;
	std::string expected = "suspended-fourth";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_neapolitan, Enums )
{
	KindValue e = KindValue::neapolitan;
	std::string expected = "Neapolitan";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_italian, Enums )
{
	KindValue e = KindValue::italian;
	std::string expected = "Italian";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_french, Enums )
{
	KindValue e = KindValue::french;
	std::string expected = "French";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_german, Enums )
{
	KindValue e = KindValue::german;
	std::string expected = "German";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_pedal, Enums )
{
	KindValue e = KindValue::pedal;
	std::string expected = "pedal";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_power, Enums )
{
	KindValue e = KindValue::power;
	std::string expected = "power";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_tristan, Enums )
{
	KindValue e = KindValue::tristan;
	std::string expected = "Tristan";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_other, Enums )
{
	KindValue e = KindValue::other;
	std::string expected = "other";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( KindValue_none, Enums )
{
	KindValue e = KindValue::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	KindValue e2 = parseKindValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineEnd_BadParse, Enums )
{
	LineEnd actual = parseLineEnd( "up" );
	LineEnd expected = LineEnd::up;
	CHECK_EQUAL( expected, actual )
}

TEST( LineEnd_up, Enums )
{
	LineEnd e = LineEnd::up;
	std::string expected = "up";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LineEnd e2 = parseLineEnd( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineEnd_down, Enums )
{
	LineEnd e = LineEnd::down;
	std::string expected = "down";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LineEnd e2 = parseLineEnd( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineEnd_both, Enums )
{
	LineEnd e = LineEnd::both;
	std::string expected = "both";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LineEnd e2 = parseLineEnd( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineEnd_arrow, Enums )
{
	LineEnd e = LineEnd::arrow;
	std::string expected = "arrow";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LineEnd e2 = parseLineEnd( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineEnd_none, Enums )
{
	LineEnd e = LineEnd::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	LineEnd e2 = parseLineEnd( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( MeasureNumberingValue_BadParse, Enums )
{
	MeasureNumberingValue actual = parseMeasureNumberingValue( "none" );
	MeasureNumberingValue expected = MeasureNumberingValue::none;
	CHECK_EQUAL( expected, actual )
}

TEST( MeasureNumberingValue_none, Enums )
{
	MeasureNumberingValue e = MeasureNumberingValue::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MeasureNumberingValue e2 = parseMeasureNumberingValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( MeasureNumberingValue_measure, Enums )
{
	MeasureNumberingValue e = MeasureNumberingValue::measure;
	std::string expected = "measure";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MeasureNumberingValue e2 = parseMeasureNumberingValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( MeasureNumberingValue_system, Enums )
{
	MeasureNumberingValue e = MeasureNumberingValue::system;
	std::string expected = "system";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MeasureNumberingValue e2 = parseMeasureNumberingValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Membrane_BadParse, Enums )
{
	MembraneEnum actual = parseMembraneEnum( "bass drum" );
	MembraneEnum expected = MembraneEnum::bassDrum;
	CHECK_EQUAL( expected, actual )
}

TEST( Membrane_bassDrum, Enums )
{
	MembraneEnum e = MembraneEnum::bassDrum;
	std::string expected = "bass drum";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MembraneEnum e2 = parseMembraneEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Membrane_bassDrumOnSide, Enums )
{
	MembraneEnum e = MembraneEnum::bassDrumOnSide;
	std::string expected = "bass drum on side";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MembraneEnum e2 = parseMembraneEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Membrane_bongos, Enums )
{
	MembraneEnum e = MembraneEnum::bongos;
	std::string expected = "bongos";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MembraneEnum e2 = parseMembraneEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Membrane_congaDrum, Enums )
{
	MembraneEnum e = MembraneEnum::congaDrum;
	std::string expected = "conga drum";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MembraneEnum e2 = parseMembraneEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Membrane_gobletDrum, Enums )
{
	MembraneEnum e = MembraneEnum::gobletDrum;
	std::string expected = "goblet drum";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MembraneEnum e2 = parseMembraneEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Membrane_militaryDrum, Enums )
{
	MembraneEnum e = MembraneEnum::militaryDrum;
	std::string expected = "military drum";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MembraneEnum e2 = parseMembraneEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Membrane_snareDrum, Enums )
{
	MembraneEnum e = MembraneEnum::snareDrum;
	std::string expected = "snare drum";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MembraneEnum e2 = parseMembraneEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Membrane_snareDrumSnaresOff, Enums )
{
	MembraneEnum e = MembraneEnum::snareDrumSnaresOff;
	std::string expected = "snare drum snares off";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MembraneEnum e2 = parseMembraneEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Membrane_tambourine, Enums )
{
	MembraneEnum e = MembraneEnum::tambourine;
	std::string expected = "tambourine";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MembraneEnum e2 = parseMembraneEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Membrane_tenorDrum, Enums )
{
	MembraneEnum e = MembraneEnum::tenorDrum;
	std::string expected = "tenor drum";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MembraneEnum e2 = parseMembraneEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Membrane_timbales, Enums )
{
	MembraneEnum e = MembraneEnum::timbales;
	std::string expected = "timbales";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MembraneEnum e2 = parseMembraneEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Membrane_tomtom, Enums )
{
	MembraneEnum e = MembraneEnum::tomtom;
	std::string expected = "tomtom";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MembraneEnum e2 = parseMembraneEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_BadParse, Enums )
{
	MetalEnum actual = parseMetalEnum( "almglocken" );
	MetalEnum expected = MetalEnum::almglocken;
	CHECK_EQUAL( expected, actual )
}

TEST( Metal_almglocken, Enums )
{
	MetalEnum e = MetalEnum::almglocken;
	std::string expected = "almglocken";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_bell, Enums )
{
	MetalEnum e = MetalEnum::bell;
	std::string expected = "bell";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_bellPlate, Enums )
{
	MetalEnum e = MetalEnum::bellPlate;
	std::string expected = "bell plate";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_brakeDrum, Enums )
{
	MetalEnum e = MetalEnum::brakeDrum;
	std::string expected = "brake drum";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_chineseCymbal, Enums )
{
	MetalEnum e = MetalEnum::chineseCymbal;
	std::string expected = "Chinese cymbal";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_cowbell, Enums )
{
	MetalEnum e = MetalEnum::cowbell;
	std::string expected = "cowbell";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_crashCymbals, Enums )
{
	MetalEnum e = MetalEnum::crashCymbals;
	std::string expected = "crash cymbals";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_crotale, Enums )
{
	MetalEnum e = MetalEnum::crotale;
	std::string expected = "crotale";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_cymbalTongs, Enums )
{
	MetalEnum e = MetalEnum::cymbalTongs;
	std::string expected = "cymbal tongs";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_domedGong, Enums )
{
	MetalEnum e = MetalEnum::domedGong;
	std::string expected = "domed gong";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_fingerCymbals, Enums )
{
	MetalEnum e = MetalEnum::fingerCymbals;
	std::string expected = "finger cymbals";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_flexatone, Enums )
{
	MetalEnum e = MetalEnum::flexatone;
	std::string expected = "flexatone";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_gong, Enums )
{
	MetalEnum e = MetalEnum::gong;
	std::string expected = "gong";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_hiHat, Enums )
{
	MetalEnum e = MetalEnum::hiHat;
	std::string expected = "hi-hat";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_highHatCymbals, Enums )
{
	MetalEnum e = MetalEnum::highHatCymbals;
	std::string expected = "high-hat cymbals";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_handbell, Enums )
{
	MetalEnum e = MetalEnum::handbell;
	std::string expected = "handbell";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_sistrum, Enums )
{
	MetalEnum e = MetalEnum::sistrum;
	std::string expected = "sistrum";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_sizzleCymbal, Enums )
{
	MetalEnum e = MetalEnum::sizzleCymbal;
	std::string expected = "sizzle cymbal";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_sleighBells, Enums )
{
	MetalEnum e = MetalEnum::sleighBells;
	std::string expected = "sleigh bells";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_suspendedCymbal, Enums )
{
	MetalEnum e = MetalEnum::suspendedCymbal;
	std::string expected = "suspended cymbal";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_tamTam, Enums )
{
	MetalEnum e = MetalEnum::tamTam;
	std::string expected = "tam tam";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_triangle, Enums )
{
	MetalEnum e = MetalEnum::triangle;
	std::string expected = "triangle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Metal_vietnameseHat, Enums )
{
	MetalEnum e = MetalEnum::vietnameseHat;
	std::string expected = "Vietnamese hat";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MetalEnum e2 = parseMetalEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( OnOff_BadParse, Enums )
{
	OnOff actual = parseOnOff( "on" );
	OnOff expected = OnOff::on;
	CHECK_EQUAL( expected, actual )
}

TEST( OnOff_on, Enums )
{
	OnOff e = OnOff::on;
	std::string expected = "on";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	OnOff e2 = parseOnOff( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( OnOff_off, Enums )
{
	OnOff e = OnOff::off;
	std::string expected = "off";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	OnOff e2 = parseOnOff( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Pitched_BadParse, Enums )
{
	PitchedEnum actual = parsePitchedEnum( "chimes" );
	PitchedEnum expected = PitchedEnum::chimes;
	CHECK_EQUAL( expected, actual )
}

TEST( Pitched_chimes, Enums )
{
	PitchedEnum e = PitchedEnum::chimes;
	std::string expected = "chimes";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	PitchedEnum e2 = parsePitchedEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Pitched_glockenspiel, Enums )
{
	PitchedEnum e = PitchedEnum::glockenspiel;
	std::string expected = "glockenspiel";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	PitchedEnum e2 = parsePitchedEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Pitched_mallet, Enums )
{
	PitchedEnum e = PitchedEnum::mallet;
	std::string expected = "mallet";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	PitchedEnum e2 = parsePitchedEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Pitched_marimba, Enums )
{
	PitchedEnum e = PitchedEnum::marimba;
	std::string expected = "marimba";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	PitchedEnum e2 = parsePitchedEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Pitched_tubularChimes, Enums )
{
	PitchedEnum e = PitchedEnum::tubularChimes;
	std::string expected = "tubular chimes";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	PitchedEnum e2 = parsePitchedEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Pitched_vibraphone, Enums )
{
	PitchedEnum e = PitchedEnum::vibraphone;
	std::string expected = "vibraphone";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	PitchedEnum e2 = parsePitchedEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Pitched_xylophone, Enums )
{
	PitchedEnum e = PitchedEnum::xylophone;
	std::string expected = "xylophone";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	PitchedEnum e2 = parsePitchedEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( PrincipalVoiceSymbol_BadParse, Enums )
{
	PrincipalVoiceSymbol actual = parsePrincipalVoiceSymbol( "Hauptstimme" );
	PrincipalVoiceSymbol expected = PrincipalVoiceSymbol::hauptstimme;
	CHECK_EQUAL( expected, actual )
}

TEST( PrincipalVoiceSymbol_hauptstimme, Enums )
{
	PrincipalVoiceSymbol e = PrincipalVoiceSymbol::hauptstimme;
	std::string expected = "Hauptstimme";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	PrincipalVoiceSymbol e2 = parsePrincipalVoiceSymbol( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( PrincipalVoiceSymbol_nebenstimme, Enums )
{
	PrincipalVoiceSymbol e = PrincipalVoiceSymbol::nebenstimme;
	std::string expected = "Nebenstimme";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	PrincipalVoiceSymbol e2 = parsePrincipalVoiceSymbol( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( PrincipalVoiceSymbol_plain, Enums )
{
	PrincipalVoiceSymbol e = PrincipalVoiceSymbol::plain;
	std::string expected = "plain";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	PrincipalVoiceSymbol e2 = parsePrincipalVoiceSymbol( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( PrincipalVoiceSymbol_none, Enums )
{
	PrincipalVoiceSymbol e = PrincipalVoiceSymbol::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	PrincipalVoiceSymbol e2 = parsePrincipalVoiceSymbol( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStopChangeContinue_BadParse, Enums )
{
	StartStopChangeContinue actual = parseStartStopChangeContinue( "start" );
	StartStopChangeContinue expected = StartStopChangeContinue::start;
	CHECK_EQUAL( expected, actual )
}

TEST( StartStopChangeContinue_start, Enums )
{
	StartStopChangeContinue e = StartStopChangeContinue::start;
	std::string expected = "start";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStopChangeContinue e2 = parseStartStopChangeContinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStopChangeContinue_stop, Enums )
{
	StartStopChangeContinue e = StartStopChangeContinue::stop;
	std::string expected = "stop";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStopChangeContinue e2 = parseStartStopChangeContinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStopChangeContinue_change, Enums )
{
	StartStopChangeContinue e = StartStopChangeContinue::change;
	std::string expected = "change";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStopChangeContinue e2 = parseStartStopChangeContinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StartStopChangeContinue_continue_, Enums )
{
	StartStopChangeContinue e = StartStopChangeContinue::continue_;
	std::string expected = "continue";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StartStopChangeContinue e2 = parseStartStopChangeContinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TipDirection_BadParse, Enums )
{
	TipDirection actual = parseTipDirection( "up" );
	TipDirection expected = TipDirection::up;
	CHECK_EQUAL( expected, actual )
}

TEST( TipDirection_up, Enums )
{
	TipDirection e = TipDirection::up;
	std::string expected = "up";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TipDirection e2 = parseTipDirection( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TipDirection_down, Enums )
{
	TipDirection e = TipDirection::down;
	std::string expected = "down";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TipDirection e2 = parseTipDirection( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TipDirection_left, Enums )
{
	TipDirection e = TipDirection::left;
	std::string expected = "left";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TipDirection e2 = parseTipDirection( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TipDirection_right, Enums )
{
	TipDirection e = TipDirection::right;
	std::string expected = "right";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TipDirection e2 = parseTipDirection( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TipDirection_northwest, Enums )
{
	TipDirection e = TipDirection::northwest;
	std::string expected = "northwest";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TipDirection e2 = parseTipDirection( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TipDirection_northeast, Enums )
{
	TipDirection e = TipDirection::northeast;
	std::string expected = "northeast";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TipDirection e2 = parseTipDirection( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TipDirection_southeast, Enums )
{
	TipDirection e = TipDirection::southeast;
	std::string expected = "southeast";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TipDirection e2 = parseTipDirection( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( TipDirection_southwest, Enums )
{
	TipDirection e = TipDirection::southwest;
	std::string expected = "southwest";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	TipDirection e2 = parseTipDirection( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickLocation_BadParse, Enums )
{
	StickLocationEnum actual = parseStickLocationEnum( "center" );
	StickLocationEnum expected = StickLocationEnum::center;
	CHECK_EQUAL( expected, actual )
}

TEST( StickLocation_center, Enums )
{
	StickLocationEnum e = StickLocationEnum::center;
	std::string expected = "center";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickLocationEnum e2 = parseStickLocationEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickLocation_rim, Enums )
{
	StickLocationEnum e = StickLocationEnum::rim;
	std::string expected = "rim";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickLocationEnum e2 = parseStickLocationEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickLocation_cymbalBell, Enums )
{
	StickLocationEnum e = StickLocationEnum::cymbalBell;
	std::string expected = "cymbal bell";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickLocationEnum e2 = parseStickLocationEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickLocation_cymbalEdge, Enums )
{
	StickLocationEnum e = StickLocationEnum::cymbalEdge;
	std::string expected = "cymbal edge";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickLocationEnum e2 = parseStickLocationEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickMaterial_BadParse, Enums )
{
	StickMaterialEnum actual = parseStickMaterialEnum( "soft" );
	StickMaterialEnum expected = StickMaterialEnum::soft;
	CHECK_EQUAL( expected, actual )
}

TEST( StickMaterial_soft, Enums )
{
	StickMaterialEnum e = StickMaterialEnum::soft;
	std::string expected = "soft";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickMaterialEnum e2 = parseStickMaterialEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickMaterial_medium, Enums )
{
	StickMaterialEnum e = StickMaterialEnum::medium;
	std::string expected = "medium";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickMaterialEnum e2 = parseStickMaterialEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickMaterial_hard, Enums )
{
	StickMaterialEnum e = StickMaterialEnum::hard;
	std::string expected = "hard";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickMaterialEnum e2 = parseStickMaterialEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickMaterial_shaded, Enums )
{
	StickMaterialEnum e = StickMaterialEnum::shaded;
	std::string expected = "shaded";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickMaterialEnum e2 = parseStickMaterialEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickMaterial_x, Enums )
{
	StickMaterialEnum e = StickMaterialEnum::x;
	std::string expected = "x";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickMaterialEnum e2 = parseStickMaterialEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickType_BadParse, Enums )
{
	StickTypeEnum actual = parseStickTypeEnum( "bass drum" );
	StickTypeEnum expected = StickTypeEnum::bassDrum;
	CHECK_EQUAL( expected, actual )
}

TEST( StickType_bassDrum, Enums )
{
	StickTypeEnum e = StickTypeEnum::bassDrum;
	std::string expected = "bass drum";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickTypeEnum e2 = parseStickTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickType_doubleBassDrum, Enums )
{
	StickTypeEnum e = StickTypeEnum::doubleBassDrum;
	std::string expected = "double bass drum";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickTypeEnum e2 = parseStickTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickType_timpani, Enums )
{
	StickTypeEnum e = StickTypeEnum::timpani;
	std::string expected = "timpani";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickTypeEnum e2 = parseStickTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickType_xylophone, Enums )
{
	StickTypeEnum e = StickTypeEnum::xylophone;
	std::string expected = "xylophone";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickTypeEnum e2 = parseStickTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StickType_yarn, Enums )
{
	StickTypeEnum e = StickTypeEnum::yarn;
	std::string expected = "yarn";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StickTypeEnum e2 = parseStickTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( UpDownStopContinue_BadParse, Enums )
{
	UpDownStopContinue actual = parseUpDownStopContinue( "up" );
	UpDownStopContinue expected = UpDownStopContinue::up;
	CHECK_EQUAL( expected, actual )
}

TEST( UpDownStopContinue_up, Enums )
{
	UpDownStopContinue e = UpDownStopContinue::up;
	std::string expected = "up";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	UpDownStopContinue e2 = parseUpDownStopContinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( UpDownStopContinue_down, Enums )
{
	UpDownStopContinue e = UpDownStopContinue::down;
	std::string expected = "down";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	UpDownStopContinue e2 = parseUpDownStopContinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( UpDownStopContinue_stop, Enums )
{
	UpDownStopContinue e = UpDownStopContinue::stop;
	std::string expected = "stop";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	UpDownStopContinue e2 = parseUpDownStopContinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( UpDownStopContinue_continue_, Enums )
{
	UpDownStopContinue e = UpDownStopContinue::continue_;
	std::string expected = "continue";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	UpDownStopContinue e2 = parseUpDownStopContinue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( WedgeType_BadParse, Enums )
{
	WedgeType actual = parseWedgeType( "crescendo" );
	WedgeType expected = WedgeType::crescendo;
	CHECK_EQUAL( expected, actual )
}

TEST( WedgeType_crescendo, Enums )
{
	WedgeType e = WedgeType::crescendo;
	std::string expected = "crescendo";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WedgeType e2 = parseWedgeType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( WedgeType_diminuendo, Enums )
{
	WedgeType e = WedgeType::diminuendo;
	std::string expected = "diminuendo";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WedgeType e2 = parseWedgeType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( WedgeType_stop, Enums )
{
	WedgeType e = WedgeType::stop;
	std::string expected = "stop";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WedgeType e2 = parseWedgeType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( WedgeType_continue_, Enums )
{
	WedgeType e = WedgeType::continue_;
	std::string expected = "continue";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WedgeType e2 = parseWedgeType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_BadParse, Enums )
{
	WoodEnum actual = parseWoodEnum( "board clapper" );
	WoodEnum expected = WoodEnum::boardClapper;
	CHECK_EQUAL( expected, actual )
}

TEST( Wood_boardClapper, Enums )
{
	WoodEnum e = WoodEnum::boardClapper;
	std::string expected = "board clapper";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_cabasa, Enums )
{
	WoodEnum e = WoodEnum::cabasa;
	std::string expected = "cabasa";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_castanets, Enums )
{
	WoodEnum e = WoodEnum::castanets;
	std::string expected = "castanets";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_claves, Enums )
{
	WoodEnum e = WoodEnum::claves;
	std::string expected = "claves";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_guiro, Enums )
{
	WoodEnum e = WoodEnum::guiro;
	std::string expected = "guiro";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_logDrum, Enums )
{
	WoodEnum e = WoodEnum::logDrum;
	std::string expected = "log drum";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_maraca, Enums )
{
	WoodEnum e = WoodEnum::maraca;
	std::string expected = "maraca";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_maracas, Enums )
{
	WoodEnum e = WoodEnum::maracas;
	std::string expected = "maracas";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_ratchet, Enums )
{
	WoodEnum e = WoodEnum::ratchet;
	std::string expected = "ratchet";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_sandpaperBlocks, Enums )
{
	WoodEnum e = WoodEnum::sandpaperBlocks;
	std::string expected = "sandpaper blocks";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_slitDrum, Enums )
{
	WoodEnum e = WoodEnum::slitDrum;
	std::string expected = "slit drum";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_templeBlock, Enums )
{
	WoodEnum e = WoodEnum::templeBlock;
	std::string expected = "temple block";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_vibraslap, Enums )
{
	WoodEnum e = WoodEnum::vibraslap;
	std::string expected = "vibraslap";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Wood_woodBlock, Enums )
{
	WoodEnum e = WoodEnum::woodBlock;
	std::string expected = "wood block";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	WoodEnum e2 = parseWoodEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( MarginType_BadParse, Enums )
{
	MarginType actual = parseMarginType( "odd" );
	MarginType expected = MarginType::odd;
	CHECK_EQUAL( expected, actual )
}

TEST( MarginType_odd, Enums )
{
	MarginType e = MarginType::odd;
	std::string expected = "odd";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MarginType e2 = parseMarginType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( MarginType_even, Enums )
{
	MarginType e = MarginType::even;
	std::string expected = "even";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MarginType e2 = parseMarginType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( MarginType_both, Enums )
{
	MarginType e = MarginType::both;
	std::string expected = "both";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	MarginType e2 = parseMarginType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteSizeType_BadParse, Enums )
{
	NoteSizeType actual = parseNoteSizeType( "cue" );
	NoteSizeType expected = NoteSizeType::cue;
	CHECK_EQUAL( expected, actual )
}

TEST( NoteSizeType_cue, Enums )
{
	NoteSizeType e = NoteSizeType::cue;
	std::string expected = "cue";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteSizeType e2 = parseNoteSizeType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteSizeType_grace, Enums )
{
	NoteSizeType e = NoteSizeType::grace;
	std::string expected = "grace";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteSizeType e2 = parseNoteSizeType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteSizeType_large, Enums )
{
	NoteSizeType e = NoteSizeType::large;
	std::string expected = "large";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteSizeType e2 = parseNoteSizeType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_BadParse, Enums )
{
	AccidentalValue actual = parseAccidentalValue( "sharp" );
	AccidentalValue expected = AccidentalValue::sharp;
	CHECK_EQUAL( expected, actual )
}

TEST( AccidentalValue_sharp, Enums )
{
	AccidentalValue e = AccidentalValue::sharp;
	std::string expected = "sharp";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_natural, Enums )
{
	AccidentalValue e = AccidentalValue::natural;
	std::string expected = "natural";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_flat, Enums )
{
	AccidentalValue e = AccidentalValue::flat;
	std::string expected = "flat";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_doubleSharp, Enums )
{
	AccidentalValue e = AccidentalValue::doubleSharp;
	std::string expected = "double-sharp";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_sharpSharp, Enums )
{
	AccidentalValue e = AccidentalValue::sharpSharp;
	std::string expected = "sharp-sharp";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_flatFlat, Enums )
{
	AccidentalValue e = AccidentalValue::flatFlat;
	std::string expected = "flat-flat";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_naturalSharp, Enums )
{
	AccidentalValue e = AccidentalValue::naturalSharp;
	std::string expected = "natural-sharp";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_naturalFlat, Enums )
{
	AccidentalValue e = AccidentalValue::naturalFlat;
	std::string expected = "natural-flat";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_quarterFlat, Enums )
{
	AccidentalValue e = AccidentalValue::quarterFlat;
	std::string expected = "quarter-flat";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_quarterSharp, Enums )
{
	AccidentalValue e = AccidentalValue::quarterSharp;
	std::string expected = "quarter-sharp";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_threeQuartersFlat, Enums )
{
	AccidentalValue e = AccidentalValue::threeQuartersFlat;
	std::string expected = "three-quarters-flat";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_threeQuartersSharp, Enums )
{
	AccidentalValue e = AccidentalValue::threeQuartersSharp;
	std::string expected = "three-quarters-sharp";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_sharpDown, Enums )
{
	AccidentalValue e = AccidentalValue::sharpDown;
	std::string expected = "sharp-down";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_sharpUp, Enums )
{
	AccidentalValue e = AccidentalValue::sharpUp;
	std::string expected = "sharp-up";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_naturalDown, Enums )
{
	AccidentalValue e = AccidentalValue::naturalDown;
	std::string expected = "natural-down";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_naturalUp, Enums )
{
	AccidentalValue e = AccidentalValue::naturalUp;
	std::string expected = "natural-up";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_flatDown, Enums )
{
	AccidentalValue e = AccidentalValue::flatDown;
	std::string expected = "flat-down";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_flatUp, Enums )
{
	AccidentalValue e = AccidentalValue::flatUp;
	std::string expected = "flat-up";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_tripleSharp, Enums )
{
	AccidentalValue e = AccidentalValue::tripleSharp;
	std::string expected = "triple-sharp";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_tripleFlat, Enums )
{
	AccidentalValue e = AccidentalValue::tripleFlat;
	std::string expected = "triple-flat";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_slashQuarterSharp, Enums )
{
	AccidentalValue e = AccidentalValue::slashQuarterSharp;
	std::string expected = "slash-quarter-sharp";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_slashSharp, Enums )
{
	AccidentalValue e = AccidentalValue::slashSharp;
	std::string expected = "slash-sharp";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_slashFlat, Enums )
{
	AccidentalValue e = AccidentalValue::slashFlat;
	std::string expected = "slash-flat";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_doubleSlashFlat, Enums )
{
	AccidentalValue e = AccidentalValue::doubleSlashFlat;
	std::string expected = "double-slash-flat";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_sharp1, Enums )
{
	AccidentalValue e = AccidentalValue::sharp1;
	std::string expected = "sharp-1";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_sharp2, Enums )
{
	AccidentalValue e = AccidentalValue::sharp2;
	std::string expected = "sharp-2";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_sharp3, Enums )
{
	AccidentalValue e = AccidentalValue::sharp3;
	std::string expected = "sharp-3";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_sharp5, Enums )
{
	AccidentalValue e = AccidentalValue::sharp5;
	std::string expected = "sharp-5";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_flat1, Enums )
{
	AccidentalValue e = AccidentalValue::flat1;
	std::string expected = "flat-1";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_flat2, Enums )
{
	AccidentalValue e = AccidentalValue::flat2;
	std::string expected = "flat-2";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_flat3, Enums )
{
	AccidentalValue e = AccidentalValue::flat3;
	std::string expected = "flat-3";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_flat4, Enums )
{
	AccidentalValue e = AccidentalValue::flat4;
	std::string expected = "flat-4";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_sori, Enums )
{
	AccidentalValue e = AccidentalValue::sori;
	std::string expected = "sori";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( AccidentalValue_koron, Enums )
{
	AccidentalValue e = AccidentalValue::koron;
	std::string expected = "koron";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	AccidentalValue e2 = parseAccidentalValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowDirectionEnum_BadParse, Enums )
{
	ArrowDirectionEnum actual = parseArrowDirectionEnum( "left" );
	ArrowDirectionEnum expected = ArrowDirectionEnum::left;
	CHECK_EQUAL( expected, actual )
}

TEST( ArrowDirectionEnum_left, Enums )
{
	ArrowDirectionEnum e = ArrowDirectionEnum::left;
	std::string expected = "left";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowDirectionEnum e2 = parseArrowDirectionEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowDirectionEnum_up, Enums )
{
	ArrowDirectionEnum e = ArrowDirectionEnum::up;
	std::string expected = "up";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowDirectionEnum e2 = parseArrowDirectionEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowDirectionEnum_right, Enums )
{
	ArrowDirectionEnum e = ArrowDirectionEnum::right;
	std::string expected = "right";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowDirectionEnum e2 = parseArrowDirectionEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowDirectionEnum_down, Enums )
{
	ArrowDirectionEnum e = ArrowDirectionEnum::down;
	std::string expected = "down";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowDirectionEnum e2 = parseArrowDirectionEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowDirectionEnum_northwest, Enums )
{
	ArrowDirectionEnum e = ArrowDirectionEnum::northwest;
	std::string expected = "northwest";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowDirectionEnum e2 = parseArrowDirectionEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowDirectionEnum_northeast, Enums )
{
	ArrowDirectionEnum e = ArrowDirectionEnum::northeast;
	std::string expected = "northeast";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowDirectionEnum e2 = parseArrowDirectionEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowDirectionEnum_southeast, Enums )
{
	ArrowDirectionEnum e = ArrowDirectionEnum::southeast;
	std::string expected = "southeast";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowDirectionEnum e2 = parseArrowDirectionEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowDirectionEnum_southwest, Enums )
{
	ArrowDirectionEnum e = ArrowDirectionEnum::southwest;
	std::string expected = "southwest";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowDirectionEnum e2 = parseArrowDirectionEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowDirectionEnum_leftRight, Enums )
{
	ArrowDirectionEnum e = ArrowDirectionEnum::leftRight;
	std::string expected = "left right";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowDirectionEnum e2 = parseArrowDirectionEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowDirectionEnum_upDown, Enums )
{
	ArrowDirectionEnum e = ArrowDirectionEnum::upDown;
	std::string expected = "up down";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowDirectionEnum e2 = parseArrowDirectionEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowDirectionEnum_northwestSoutheast, Enums )
{
	ArrowDirectionEnum e = ArrowDirectionEnum::northwestSoutheast;
	std::string expected = "northwest southeast";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowDirectionEnum e2 = parseArrowDirectionEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowDirectionEnum_northeastSouthwest, Enums )
{
	ArrowDirectionEnum e = ArrowDirectionEnum::northeastSouthwest;
	std::string expected = "northeast southwest";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowDirectionEnum e2 = parseArrowDirectionEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowDirectionEnum_other, Enums )
{
	ArrowDirectionEnum e = ArrowDirectionEnum::other;
	std::string expected = "other";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowDirectionEnum e2 = parseArrowDirectionEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowStyle_BadParse, Enums )
{
	ArrowStyleEnum actual = parseArrowStyleEnum( "single" );
	ArrowStyleEnum expected = ArrowStyleEnum::single;
	CHECK_EQUAL( expected, actual )
}

TEST( ArrowStyle_single, Enums )
{
	ArrowStyleEnum e = ArrowStyleEnum::single;
	std::string expected = "single";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowStyleEnum e2 = parseArrowStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowStyle_double_, Enums )
{
	ArrowStyleEnum e = ArrowStyleEnum::double_;
	std::string expected = "double";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowStyleEnum e2 = parseArrowStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowStyle_filled, Enums )
{
	ArrowStyleEnum e = ArrowStyleEnum::filled;
	std::string expected = "filled";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowStyleEnum e2 = parseArrowStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowStyle_hollow, Enums )
{
	ArrowStyleEnum e = ArrowStyleEnum::hollow;
	std::string expected = "hollow";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowStyleEnum e2 = parseArrowStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowStyle_paired, Enums )
{
	ArrowStyleEnum e = ArrowStyleEnum::paired;
	std::string expected = "paired";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowStyleEnum e2 = parseArrowStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowStyle_combined, Enums )
{
	ArrowStyleEnum e = ArrowStyleEnum::combined;
	std::string expected = "combined";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowStyleEnum e2 = parseArrowStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ArrowStyle_other, Enums )
{
	ArrowStyleEnum e = ArrowStyleEnum::other;
	std::string expected = "other";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ArrowStyleEnum e2 = parseArrowStyleEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeamValue_BadParse, Enums )
{
	BeamValue actual = parseBeamValue( "begin" );
	BeamValue expected = BeamValue::begin;
	CHECK_EQUAL( expected, actual )
}

TEST( BeamValue_begin, Enums )
{
	BeamValue e = BeamValue::begin;
	std::string expected = "begin";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeamValue e2 = parseBeamValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeamValue_continue_, Enums )
{
	BeamValue e = BeamValue::continue_;
	std::string expected = "continue";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeamValue e2 = parseBeamValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeamValue_end, Enums )
{
	BeamValue e = BeamValue::end;
	std::string expected = "end";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeamValue e2 = parseBeamValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeamValue_forwardHook, Enums )
{
	BeamValue e = BeamValue::forwardHook;
	std::string expected = "forward hook";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeamValue e2 = parseBeamValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BeamValue_backwardHook, Enums )
{
	BeamValue e = BeamValue::backwardHook;
	std::string expected = "backward hook";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BeamValue e2 = parseBeamValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BreathMarkValue_BadParse, Enums )
{
	BreathMarkValue actual = parseBreathMarkValue( "" );
	BreathMarkValue expected = BreathMarkValue::emptystring;
	CHECK_EQUAL( expected, actual )
}

TEST( BreathMarkValue_emptystring, Enums )
{
	BreathMarkValue e = BreathMarkValue::emptystring;
	std::string expected = "";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BreathMarkValue e2 = parseBreathMarkValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BreathMarkValue_comma, Enums )
{
	BreathMarkValue e = BreathMarkValue::comma;
	std::string expected = "comma";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BreathMarkValue e2 = parseBreathMarkValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( BreathMarkValue_tick, Enums )
{
	BreathMarkValue e = BreathMarkValue::tick;
	std::string expected = "tick";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	BreathMarkValue e2 = parseBreathMarkValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( CircularArrow_BadParse, Enums )
{
	CircularArrowEnum actual = parseCircularArrowEnum( "clockwise" );
	CircularArrowEnum expected = CircularArrowEnum::clockwise;
	CHECK_EQUAL( expected, actual )
}

TEST( CircularArrow_clockwise, Enums )
{
	CircularArrowEnum e = CircularArrowEnum::clockwise;
	std::string expected = "clockwise";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	CircularArrowEnum e2 = parseCircularArrowEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( CircularArrow_anticlockwise, Enums )
{
	CircularArrowEnum e = CircularArrowEnum::anticlockwise;
	std::string expected = "anticlockwise";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	CircularArrowEnum e2 = parseCircularArrowEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Fan_BadParse, Enums )
{
	Fan actual = parseFan( "accel" );
	Fan expected = Fan::accel;
	CHECK_EQUAL( expected, actual )
}

TEST( Fan_accel, Enums )
{
	Fan e = Fan::accel;
	std::string expected = "accel";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	Fan e2 = parseFan( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Fan_rit, Enums )
{
	Fan e = Fan::rit;
	std::string expected = "rit";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	Fan e2 = parseFan( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Fan_none, Enums )
{
	Fan e = Fan::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	Fan e2 = parseFan( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HandbellValue_BadParse, Enums )
{
	HandbellValue actual = parseHandbellValue( "damp" );
	HandbellValue expected = HandbellValue::damp;
	CHECK_EQUAL( expected, actual )
}

TEST( HandbellValue_damp, Enums )
{
	HandbellValue e = HandbellValue::damp;
	std::string expected = "damp";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HandbellValue e2 = parseHandbellValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HandbellValue_echo, Enums )
{
	HandbellValue e = HandbellValue::echo;
	std::string expected = "echo";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HandbellValue e2 = parseHandbellValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HandbellValue_gyro, Enums )
{
	HandbellValue e = HandbellValue::gyro;
	std::string expected = "gyro";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HandbellValue e2 = parseHandbellValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HandbellValue_handMartellato, Enums )
{
	HandbellValue e = HandbellValue::handMartellato;
	std::string expected = "hand martellato";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HandbellValue e2 = parseHandbellValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HandbellValue_malletLift, Enums )
{
	HandbellValue e = HandbellValue::malletLift;
	std::string expected = "mallet lift";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HandbellValue e2 = parseHandbellValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HandbellValue_malletTable, Enums )
{
	HandbellValue e = HandbellValue::malletTable;
	std::string expected = "mallet table";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HandbellValue e2 = parseHandbellValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HandbellValue_martellato, Enums )
{
	HandbellValue e = HandbellValue::martellato;
	std::string expected = "martellato";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HandbellValue e2 = parseHandbellValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HandbellValue_martellatoLift, Enums )
{
	HandbellValue e = HandbellValue::martellatoLift;
	std::string expected = "martellato lift";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HandbellValue e2 = parseHandbellValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HandbellValue_mutedMartellato, Enums )
{
	HandbellValue e = HandbellValue::mutedMartellato;
	std::string expected = "muted martellato";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HandbellValue e2 = parseHandbellValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HandbellValue_pluckLift, Enums )
{
	HandbellValue e = HandbellValue::pluckLift;
	std::string expected = "pluck lift";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HandbellValue e2 = parseHandbellValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HandbellValue_swing, Enums )
{
	HandbellValue e = HandbellValue::swing;
	std::string expected = "swing";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HandbellValue e2 = parseHandbellValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HoleClosedLocation_BadParse, Enums )
{
	HoleClosedLocation actual = parseHoleClosedLocation( "right" );
	HoleClosedLocation expected = HoleClosedLocation::right;
	CHECK_EQUAL( expected, actual )
}

TEST( HoleClosedLocation_right, Enums )
{
	HoleClosedLocation e = HoleClosedLocation::right;
	std::string expected = "right";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HoleClosedLocation e2 = parseHoleClosedLocation( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HoleClosedLocation_bottom, Enums )
{
	HoleClosedLocation e = HoleClosedLocation::bottom;
	std::string expected = "bottom";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HoleClosedLocation e2 = parseHoleClosedLocation( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HoleClosedLocation_left, Enums )
{
	HoleClosedLocation e = HoleClosedLocation::left;
	std::string expected = "left";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HoleClosedLocation e2 = parseHoleClosedLocation( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HoleClosedLocation_top, Enums )
{
	HoleClosedLocation e = HoleClosedLocation::top;
	std::string expected = "top";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HoleClosedLocation e2 = parseHoleClosedLocation( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HoleClosedValue_BadParse, Enums )
{
	HoleClosedValue actual = parseHoleClosedValue( "yes" );
	HoleClosedValue expected = HoleClosedValue::yes;
	CHECK_EQUAL( expected, actual )
}

TEST( HoleClosedValue_yes, Enums )
{
	HoleClosedValue e = HoleClosedValue::yes;
	std::string expected = "yes";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HoleClosedValue e2 = parseHoleClosedValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HoleClosedValue_no, Enums )
{
	HoleClosedValue e = HoleClosedValue::no;
	std::string expected = "no";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HoleClosedValue e2 = parseHoleClosedValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( HoleClosedValue_half, Enums )
{
	HoleClosedValue e = HoleClosedValue::half;
	std::string expected = "half";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	HoleClosedValue e2 = parseHoleClosedValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_BadParse, Enums )
{
	NoteTypeValue actual = parseNoteTypeValue( "1024th" );
	NoteTypeValue expected = NoteTypeValue::oneThousandTwentyFourth;
	CHECK_EQUAL( expected, actual )
}

TEST( NoteTypeValue_oneThousandTwentyFourth, Enums )
{
	NoteTypeValue e = NoteTypeValue::oneThousandTwentyFourth;
	std::string expected = "1024th";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_fiveHundredTwelfth, Enums )
{
	NoteTypeValue e = NoteTypeValue::fiveHundredTwelfth;
	std::string expected = "512th";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_twoHundredFifthySixth, Enums )
{
	NoteTypeValue e = NoteTypeValue::twoHundredFifthySixth;
	std::string expected = "256th";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_oneHundredTwentyEighth, Enums )
{
	NoteTypeValue e = NoteTypeValue::oneHundredTwentyEighth;
	std::string expected = "128th";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_sixtyFourth, Enums )
{
	NoteTypeValue e = NoteTypeValue::sixtyFourth;
	std::string expected = "64th";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_thirtySecond, Enums )
{
	NoteTypeValue e = NoteTypeValue::thirtySecond;
	std::string expected = "32nd";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_sixteenth, Enums )
{
	NoteTypeValue e = NoteTypeValue::sixteenth;
	std::string expected = "16th";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_eighth, Enums )
{
	NoteTypeValue e = NoteTypeValue::eighth;
	std::string expected = "eighth";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_quarter, Enums )
{
	NoteTypeValue e = NoteTypeValue::quarter;
	std::string expected = "quarter";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_half, Enums )
{
	NoteTypeValue e = NoteTypeValue::half;
	std::string expected = "half";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_whole, Enums )
{
	NoteTypeValue e = NoteTypeValue::whole;
	std::string expected = "whole";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_breve, Enums )
{
	NoteTypeValue e = NoteTypeValue::breve;
	std::string expected = "breve";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_long_, Enums )
{
	NoteTypeValue e = NoteTypeValue::long_;
	std::string expected = "long";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteTypeValue_maxima, Enums )
{
	NoteTypeValue e = NoteTypeValue::maxima;
	std::string expected = "maxima";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteTypeValue e2 = parseNoteTypeValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_BadParse, Enums )
{
	NoteheadValue actual = parseNoteheadValue( "slash" );
	NoteheadValue expected = NoteheadValue::slash;
	CHECK_EQUAL( expected, actual )
}

TEST( NoteheadValue_slash, Enums )
{
	NoteheadValue e = NoteheadValue::slash;
	std::string expected = "slash";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_triangle, Enums )
{
	NoteheadValue e = NoteheadValue::triangle;
	std::string expected = "triangle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_diamond, Enums )
{
	NoteheadValue e = NoteheadValue::diamond;
	std::string expected = "diamond";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_square, Enums )
{
	NoteheadValue e = NoteheadValue::square;
	std::string expected = "square";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_cross, Enums )
{
	NoteheadValue e = NoteheadValue::cross;
	std::string expected = "cross";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_x, Enums )
{
	NoteheadValue e = NoteheadValue::x;
	std::string expected = "x";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_circleX, Enums )
{
	NoteheadValue e = NoteheadValue::circleX;
	std::string expected = "circle-x";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_invertedTriangle, Enums )
{
	NoteheadValue e = NoteheadValue::invertedTriangle;
	std::string expected = "inverted triangle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_arrowDown, Enums )
{
	NoteheadValue e = NoteheadValue::arrowDown;
	std::string expected = "arrow down";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_arrowUp, Enums )
{
	NoteheadValue e = NoteheadValue::arrowUp;
	std::string expected = "arrow up";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_slashed, Enums )
{
	NoteheadValue e = NoteheadValue::slashed;
	std::string expected = "slashed";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_backSlashed, Enums )
{
	NoteheadValue e = NoteheadValue::backSlashed;
	std::string expected = "back slashed";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_normal, Enums )
{
	NoteheadValue e = NoteheadValue::normal;
	std::string expected = "normal";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_cluster, Enums )
{
	NoteheadValue e = NoteheadValue::cluster;
	std::string expected = "cluster";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_circleDot, Enums )
{
	NoteheadValue e = NoteheadValue::circleDot;
	std::string expected = "circle dot";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_leftTriangle, Enums )
{
	NoteheadValue e = NoteheadValue::leftTriangle;
	std::string expected = "left triangle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_rectangle, Enums )
{
	NoteheadValue e = NoteheadValue::rectangle;
	std::string expected = "rectangle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_none, Enums )
{
	NoteheadValue e = NoteheadValue::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_do_, Enums )
{
	NoteheadValue e = NoteheadValue::do_;
	std::string expected = "do";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_re, Enums )
{
	NoteheadValue e = NoteheadValue::re;
	std::string expected = "re";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_mi, Enums )
{
	NoteheadValue e = NoteheadValue::mi;
	std::string expected = "mi";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_fa, Enums )
{
	NoteheadValue e = NoteheadValue::fa;
	std::string expected = "fa";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_faUp, Enums )
{
	NoteheadValue e = NoteheadValue::faUp;
	std::string expected = "fa up";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_so, Enums )
{
	NoteheadValue e = NoteheadValue::so;
	std::string expected = "so";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_la, Enums )
{
	NoteheadValue e = NoteheadValue::la;
	std::string expected = "la";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( NoteheadValue_ti, Enums )
{
	NoteheadValue e = NoteheadValue::ti;
	std::string expected = "ti";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	NoteheadValue e2 = parseNoteheadValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ShowTuplet_BadParse, Enums )
{
	ShowTuplet actual = parseShowTuplet( "actual" );
	ShowTuplet expected = ShowTuplet::actual;
	CHECK_EQUAL( expected, actual )
}

TEST( ShowTuplet_actual, Enums )
{
	ShowTuplet e = ShowTuplet::actual;
	std::string expected = "actual";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ShowTuplet e2 = parseShowTuplet( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ShowTuplet_both, Enums )
{
	ShowTuplet e = ShowTuplet::both;
	std::string expected = "both";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ShowTuplet e2 = parseShowTuplet( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ShowTuplet_none, Enums )
{
	ShowTuplet e = ShowTuplet::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	ShowTuplet e2 = parseShowTuplet( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StemValue_BadParse, Enums )
{
	StemValue actual = parseStemValue( "down" );
	StemValue expected = StemValue::down;
	CHECK_EQUAL( expected, actual )
}

TEST( StemValue_down, Enums )
{
	StemValue e = StemValue::down;
	std::string expected = "down";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StemValue e2 = parseStemValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StemValue_up, Enums )
{
	StemValue e = StemValue::up;
	std::string expected = "up";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StemValue e2 = parseStemValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StemValue_double_, Enums )
{
	StemValue e = StemValue::double_;
	std::string expected = "double";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StemValue e2 = parseStemValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( StemValue_none, Enums )
{
	StemValue e = StemValue::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StemValue e2 = parseStemValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Step_BadParse, Enums )
{
	StepEnum actual = parseStepEnum( "A" );
	StepEnum expected = StepEnum::a;
	CHECK_EQUAL( expected, actual )
}

TEST( Step_a, Enums )
{
	StepEnum e = StepEnum::a;
	std::string expected = "A";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StepEnum e2 = parseStepEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Step_b, Enums )
{
	StepEnum e = StepEnum::b;
	std::string expected = "B";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StepEnum e2 = parseStepEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Step_c, Enums )
{
	StepEnum e = StepEnum::c;
	std::string expected = "C";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StepEnum e2 = parseStepEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Step_d, Enums )
{
	StepEnum e = StepEnum::d;
	std::string expected = "D";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StepEnum e2 = parseStepEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Step_e, Enums )
{
	StepEnum e = StepEnum::e;
	std::string expected = "E";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StepEnum e2 = parseStepEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Step_f, Enums )
{
	StepEnum e = StepEnum::f;
	std::string expected = "F";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StepEnum e2 = parseStepEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Step_g, Enums )
{
	StepEnum e = StepEnum::g;
	std::string expected = "G";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	StepEnum e2 = parseStepEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Syllabic_BadParse, Enums )
{
	SyllabicEnum actual = parseSyllabicEnum( "single" );
	SyllabicEnum expected = SyllabicEnum::single;
	CHECK_EQUAL( expected, actual )
}

TEST( Syllabic_single, Enums )
{
	SyllabicEnum e = SyllabicEnum::single;
	std::string expected = "single";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	SyllabicEnum e2 = parseSyllabicEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Syllabic_begin, Enums )
{
	SyllabicEnum e = SyllabicEnum::begin;
	std::string expected = "begin";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	SyllabicEnum e2 = parseSyllabicEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Syllabic_end, Enums )
{
	SyllabicEnum e = SyllabicEnum::end;
	std::string expected = "end";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	SyllabicEnum e2 = parseSyllabicEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Syllabic_middle, Enums )
{
	SyllabicEnum e = SyllabicEnum::middle;
	std::string expected = "middle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	SyllabicEnum e2 = parseSyllabicEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( GroupBarlineValue_BadParse, Enums )
{
	GroupBarlineValue actual = parseGroupBarlineValue( "yes" );
	GroupBarlineValue expected = GroupBarlineValue::yes;
	CHECK_EQUAL( expected, actual )
}

TEST( GroupBarlineValue_yes, Enums )
{
	GroupBarlineValue e = GroupBarlineValue::yes;
	std::string expected = "yes";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	GroupBarlineValue e2 = parseGroupBarlineValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( GroupBarlineValue_no, Enums )
{
	GroupBarlineValue e = GroupBarlineValue::no;
	std::string expected = "no";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	GroupBarlineValue e2 = parseGroupBarlineValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( GroupBarlineValue_mensurstrich, Enums )
{
	GroupBarlineValue e = GroupBarlineValue::mensurstrich;
	std::string expected = "Mensurstrich";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	GroupBarlineValue e2 = parseGroupBarlineValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( GroupSymbolValue_BadParse, Enums )
{
	GroupSymbolValue actual = parseGroupSymbolValue( "none" );
	GroupSymbolValue expected = GroupSymbolValue::none;
	CHECK_EQUAL( expected, actual )
}

TEST( GroupSymbolValue_none, Enums )
{
	GroupSymbolValue e = GroupSymbolValue::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	GroupSymbolValue e2 = parseGroupSymbolValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( GroupSymbolValue_brace, Enums )
{
	GroupSymbolValue e = GroupSymbolValue::brace;
	std::string expected = "brace";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	GroupSymbolValue e2 = parseGroupSymbolValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( GroupSymbolValue_line, Enums )
{
	GroupSymbolValue e = GroupSymbolValue::line;
	std::string expected = "line";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	GroupSymbolValue e2 = parseGroupSymbolValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( GroupSymbolValue_bracket, Enums )
{
	GroupSymbolValue e = GroupSymbolValue::bracket;
	std::string expected = "bracket";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	GroupSymbolValue e2 = parseGroupSymbolValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( GroupSymbolValue_square, Enums )
{
	GroupSymbolValue e = GroupSymbolValue::square;
	std::string expected = "square";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );

	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );

	GroupSymbolValue e2 = parseGroupSymbolValue( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ModeEnum_major, Enums )
{
	ModeEnum e = ModeEnum::major;
	std::string expected = "major";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	ModeEnum e2 = parseModeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ModeEnum_minor, Enums )
{
	ModeEnum e = ModeEnum::minor;
	std::string expected = "minor";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	ModeEnum e2 = parseModeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ModeEnum_dorian, Enums )
{
	ModeEnum e = ModeEnum::dorian;
	std::string expected = "dorian";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	ModeEnum e2 = parseModeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ModeEnum_phrygian, Enums )
{
	ModeEnum e = ModeEnum::phrygian;
	std::string expected = "phrygian";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	ModeEnum e2 = parseModeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ModeEnum_lydian, Enums )
{
	ModeEnum e = ModeEnum::lydian;
	std::string expected = "lydian";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	ModeEnum e2 = parseModeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ModeEnum_mixolydian, Enums )
{
	ModeEnum e = ModeEnum::mixolydian;
	std::string expected = "mixolydian";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	ModeEnum e2 = parseModeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ModeEnum_aeolian, Enums )
{
	ModeEnum e = ModeEnum::aeolian;
	std::string expected = "aeolian";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	ModeEnum e2 = parseModeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ModeEnum_ionian, Enums )
{
	ModeEnum e = ModeEnum::ionian;
	std::string expected = "ionian";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	ModeEnum e2 = parseModeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ModeEnum_locrian, Enums )
{
	ModeEnum e = ModeEnum::locrian;
	std::string expected = "locrian";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	ModeEnum e2 = parseModeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ModeEnum_none, Enums )
{
	ModeEnum e = ModeEnum::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	ModeEnum e2 = parseModeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( ModeEnum_other, Enums )
{
	ModeEnum e = ModeEnum::other;
	std::string expected = "other";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	ModeEnum e2 = parseModeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( Mode_default, Enums )
{
	ModeValue object;
    ModeEnum expected = ModeEnum::major;
    ModeEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( Mode_major, Enums )
{
    ModeEnum enumval = ModeEnum::major;
    ModeEnum diffval = ModeEnum::other;
    std::string strval = toString( enumval );
    std::string difval = "none";
    std::string badstr = "EORIUT";
    ModeValue object( enumval );
    ModeEnum expected = enumval;
    ModeEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object2( diffval );
    expected = diffval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    object2.setValue( enumval );
    expected = enumval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object3( diffval );
    object3 = parseModeValue( strval );
    expected = enumval;
    actual = object3.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object4( strval );
    expected = enumval;
    actual = object4.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object5( badstr );
    expected = ModeEnum::other;
    actual = object5.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object6( enumval );
    std::string expected_str = strval;
    std::string actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    std::stringstream ss;
    toStream( ss, object6 );
    expected_str = strval;
    actual_str = ss.str();
    CHECK_EQUAL( expected_str, actual_str )
    
    object6 = parseModeValue( badstr );
    expected_str = badstr;
    actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    ss.str( "" );
    ss << object6;
    CHECK_EQUAL( expected_str, actual_str )
}

TEST( Mode_lydian, Enums )
{
    ModeEnum enumval = ModeEnum::lydian;
    ModeEnum diffval = ModeEnum::locrian;
    std::string strval = toString( enumval );
    std::string difval = "locrian";
    std::string badstr = "Lydian";
    ModeValue object( enumval );
    ModeEnum expected = enumval;
    ModeEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object2( diffval );
    expected = diffval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    object2.setValue( enumval );
    expected = enumval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object3( diffval );
    object3 = parseModeValue( strval );
    expected = enumval;
    actual = object3.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object4( strval );
    expected = enumval;
    actual = object4.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object5( badstr );
    expected = ModeEnum::other;
    actual = object5.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object6( enumval );
    std::string expected_str = strval;
    std::string actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    std::stringstream ss;
    toStream( ss, object6 );
    expected_str = strval;
    actual_str = ss.str();
    CHECK_EQUAL( expected_str, actual_str )
    
    object6 = parseModeValue( badstr );
    expected_str = badstr;
    actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    ss.str( "" );
    ss << object6;
    CHECK_EQUAL( expected_str, actual_str )
}

TEST( Mode_locrian, Enums )
{
    ModeEnum enumval = ModeEnum::locrian;
    ModeEnum diffval = ModeEnum::aeolian;
    std::string strval = toString( enumval );
    std::string difval = "aeolian";
    std::string badstr = "pelog";
    ModeValue object( enumval );
    ModeEnum expected = enumval;
    ModeEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object2( diffval );
    expected = diffval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    object2.setValue( enumval );
    expected = enumval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object3( diffval );
    object3 = parseModeValue( strval );
    expected = enumval;
    actual = object3.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object4( strval );
    expected = enumval;
    actual = object4.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object5( badstr );
    expected = ModeEnum::other;
    actual = object5.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object6( enumval );
    std::string expected_str = strval;
    std::string actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    std::stringstream ss;
    toStream( ss, object6 );
    expected_str = strval;
    actual_str = ss.str();
    CHECK_EQUAL( expected_str, actual_str )
    
    object6 = parseModeValue( badstr );
    expected_str = badstr;
    actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    ss.str( "" );
    ss << object6;
    CHECK_EQUAL( expected_str, actual_str )
}

TEST( Mode_other, Enums )
{
    ModeEnum enumval = ModeEnum::other;
    ModeEnum diffval = ModeEnum::aeolian;
    std::string strval = ""; // toString( enumval );
    std::string difval = "aeolian";
    std::string badstr = "pelog";
    ModeValue object( enumval );
    ModeEnum expected = enumval;
    ModeEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object2( diffval );
    expected = diffval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    object2.setValue( enumval );
    expected = enumval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object3( diffval );
    object3 = parseModeValue( strval );
    expected = enumval;
    actual = object3.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object4( strval );
    expected = enumval;
    actual = object4.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object5( badstr );
    expected = ModeEnum::other;
    actual = object5.getValue();
    CHECK_EQUAL( expected, actual )
    
    ModeValue object6( enumval );
    std::string expected_str = strval;
    std::string actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    std::stringstream ss;
    toStream( ss, object6 );
    expected_str = strval;
    actual_str = ss.str();
    CHECK_EQUAL( expected_str, actual_str )
    
    object6 = parseModeValue( badstr );
    expected_str = badstr;
    actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    ss.str( "" );
    ss << object6;
    CHECK_EQUAL( expected_str, actual_str )
}

TEST( DistanceTypeEnum_beam, Enums )
{
	DistanceTypeEnum e = DistanceTypeEnum::beam;
	std::string expected = "beam";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	DistanceTypeEnum e2 = parseDistanceTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( DistanceTypeEnum_hyphen, Enums )
{
	DistanceTypeEnum e = DistanceTypeEnum::hyphen;
	std::string expected = "hyphen";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	DistanceTypeEnum e2 = parseDistanceTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( DistanceTypeEnum_other, Enums )
{
	DistanceTypeEnum e = DistanceTypeEnum::other;
	std::string expected = "other";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	DistanceTypeEnum e2 = parseDistanceTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( DistanceType_beam, Enums )
{
    DistanceTypeEnum enumval = DistanceTypeEnum::beam;
    DistanceTypeEnum diffval = DistanceTypeEnum::hyphen;
    std::string strval = toString( enumval );
    std::string difval = "hyphen";
    std::string badstr = "someothertype";
    DistanceType object( enumval );
    DistanceTypeEnum expected = enumval;
    DistanceTypeEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object2( diffval );
    expected = diffval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    object2.setValue( enumval );
    expected = enumval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object3( diffval );
    object3 = parseDistanceType( strval );
    expected = enumval;
    actual = object3.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object4( strval );
    expected = enumval;
    actual = object4.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object5( badstr );
    expected = DistanceTypeEnum::other;
    actual = object5.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object6( enumval );
    std::string expected_str = strval;
    std::string actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    std::stringstream ss;
    toStream( ss, object6 );
    expected_str = strval;
    actual_str = ss.str();
    CHECK_EQUAL( expected_str, actual_str )
    
    object6 = parseDistanceType( badstr );
    expected_str = badstr;
    actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    ss.str( "" );
    ss << object6;
    CHECK_EQUAL( expected_str, actual_str )
}

TEST( DistanceType_hyphen, Enums )
{
    DistanceTypeEnum enumval = DistanceTypeEnum::hyphen;
    DistanceTypeEnum diffval = DistanceTypeEnum::beam;
    std::string strval = toString( enumval );
    std::string difval = "beam";
    std::string badstr = "someothertype";
    DistanceType object( enumval );
    DistanceTypeEnum expected = enumval;
    DistanceTypeEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object2( diffval );
    expected = diffval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    object2.setValue( enumval );
    expected = enumval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object3( diffval );
    object3 = parseDistanceType( strval );
    expected = enumval;
    actual = object3.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object4( strval );
    expected = enumval;
    actual = object4.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object5( badstr );
    expected = DistanceTypeEnum::other;
    actual = object5.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object6( enumval );
    std::string expected_str = strval;
    std::string actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    std::stringstream ss;
    toStream( ss, object6 );
    expected_str = strval;
    actual_str = ss.str();
    CHECK_EQUAL( expected_str, actual_str )
    
    object6 = parseDistanceType( badstr );
    expected_str = badstr;
    actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    ss.str( "" );
    ss << object6;
    CHECK_EQUAL( expected_str, actual_str )
}


TEST( DistanceType_other, Enums )
{
    DistanceTypeEnum enumval = DistanceTypeEnum::other;
    DistanceTypeEnum diffval = DistanceTypeEnum::beam;
    std::string strval = ""; // toString( enumval );
    std::string difval = "beam";
    std::string badstr = "someothertype";
    DistanceType object( enumval );
    DistanceTypeEnum expected = enumval;
    DistanceTypeEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object2( diffval );
    expected = diffval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    object2.setValue( enumval );
    expected = enumval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object3( diffval );
    object3 = parseDistanceType( strval );
    expected = enumval;
    actual = object3.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object4( strval );
    expected = enumval;
    actual = object4.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object5( badstr );
    expected = DistanceTypeEnum::other;
    actual = object5.getValue();
    CHECK_EQUAL( expected, actual )
    
    DistanceType object6( enumval );
    std::string expected_str = strval;
    std::string actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    std::stringstream ss;
    toStream( ss, object6 );
    expected_str = strval;
    actual_str = ss.str();
    CHECK_EQUAL( expected_str, actual_str )
    
    object6 = parseDistanceType( badstr );
    expected_str = badstr;
    actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    ss.str( "" );
    ss << object6;
    CHECK_EQUAL( expected_str, actual_str )
}

TEST( LineWidthTypeEnum_beam, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::beam;
	std::string expected = "beam";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_bracket, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::bracket;
	std::string expected = "bracket";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_dashes, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::dashes;
	std::string expected = "dashes";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_enclosure, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::enclosure;
	std::string expected = "enclosure";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_ending, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::ending;
	std::string expected = "ending";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_extend, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::extend;
	std::string expected = "extend";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_heavyBarline, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::heavyBarline;
	std::string expected = "heavy barline";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_leger, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::leger;
	std::string expected = "leger";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_lightBarline, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::lightBarline;
	std::string expected = "light barline";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_octaveShift, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::octaveShift;
	std::string expected = "octave shift";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_pedal, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::pedal;
	std::string expected = "pedal";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_slurMiddle, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::slurMiddle;
	std::string expected = "slur middle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_hyphen, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::beam;
	std::string expected = "beam";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_slurTip, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::slurTip;
	std::string expected = "slur tip";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_staff, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::staff;
	std::string expected = "staff";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_stem, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::stem;
	std::string expected = "stem";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_tieMiddle, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::tieMiddle;
	std::string expected = "tie middle";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_tieTip, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::tieTip;
	std::string expected = "tie tip";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_tupletBracket, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::tupletBracket;
	std::string expected = "tuplet bracket";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_wedge, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::wedge;
	std::string expected = "wedge";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthTypeEnum_other, Enums )
{
	LineWidthTypeEnum e = LineWidthTypeEnum::other;
	std::string expected = "other";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	LineWidthTypeEnum e2 = parseLineWidthTypeEnum( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( LineWidthType_beam, Enums )
{
    LineWidthTypeEnum enumval = LineWidthTypeEnum::beam;
    LineWidthTypeEnum diffval = LineWidthTypeEnum::enclosure;
    std::string strval = toString( enumval );
    std::string difval = "enclosure";
    std::string badstr = "someothertype";
    LineWidthType object( enumval );
    LineWidthTypeEnum expected = enumval;
    LineWidthTypeEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object2( diffval );
    expected = diffval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    object2.setValue( enumval );
    expected = enumval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object3( diffval );
    object3 = parseLineWidthType( strval );
    expected = enumval;
    actual = object3.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object4( strval );
    expected = enumval;
    actual = object4.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object5( badstr );
    expected = LineWidthTypeEnum::other;
    actual = object5.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object6( enumval );
    std::string expected_str = strval;
    std::string actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    std::stringstream ss;
    toStream( ss, object6 );
    expected_str = strval;
    actual_str = ss.str();
    CHECK_EQUAL( expected_str, actual_str )
    
    object6 = parseLineWidthType( badstr );
    expected_str = badstr;
    actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    ss.str( "" );
    ss << object6;
    CHECK_EQUAL( expected_str, actual_str )
}

TEST( LineWidthType_tieTip, Enums )
{
    LineWidthTypeEnum enumval = LineWidthTypeEnum::tieTip;
    LineWidthTypeEnum diffval = LineWidthTypeEnum::leger;
    std::string strval = toString( enumval );
    std::string difval = "leger";
    std::string badstr = "someothertype";
    LineWidthType object( enumval );
    LineWidthTypeEnum expected = enumval;
    LineWidthTypeEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object2( diffval );
    expected = diffval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    object2.setValue( enumval );
    expected = enumval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object3( diffval );
    object3 = parseLineWidthType( strval );
    expected = enumval;
    actual = object3.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object4( strval );
    expected = enumval;
    actual = object4.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object5( badstr );
    expected = LineWidthTypeEnum::other;
    actual = object5.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object6( enumval );
    std::string expected_str = strval;
    std::string actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    std::stringstream ss;
    toStream( ss, object6 );
    expected_str = strval;
    actual_str = ss.str();
    CHECK_EQUAL( expected_str, actual_str )
    
    object6 = parseLineWidthType( badstr );
    expected_str = badstr;
    actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    ss.str( "" );
    ss << object6;
    CHECK_EQUAL( expected_str, actual_str )
}

TEST( LineWidthType_other, Enums )
{
    LineWidthTypeEnum enumval = LineWidthTypeEnum::other;
    LineWidthTypeEnum diffval = LineWidthTypeEnum::wedge;
    std::string strval = ""; // toString( enumval );
    std::string difval = "enclosure";
    std::string badstr = "someothertype";
    LineWidthType object( enumval );
    LineWidthTypeEnum expected = enumval;
    LineWidthTypeEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object2( diffval );
    expected = diffval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    object2.setValue( enumval );
    expected = enumval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object3( diffval );
    object3 = parseLineWidthType( strval );
    expected = enumval;
    actual = object3.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object4( strval );
    expected = enumval;
    actual = object4.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object5( badstr );
    expected = LineWidthTypeEnum::other;
    actual = object5.getValue();
    CHECK_EQUAL( expected, actual )
    
    LineWidthType object6( enumval );
    std::string expected_str = strval;
    std::string actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    std::stringstream ss;
    toStream( ss, object6 );
    expected_str = strval;
    actual_str = ss.str();
    CHECK_EQUAL( expected_str, actual_str )
    
    object6 = parseLineWidthType( badstr );
    expected_str = badstr;
    actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    ss.str( "" );
    ss << object6;
    CHECK_EQUAL( expected_str, actual_str )
}

TEST( XlinkActuate_BadParse, Enums )
{
	XlinkActuate actual = parseXlinkActuate( "onrequest" );
	XlinkActuate expected = XlinkActuate::onLoad;
	CHECK_EQUAL( expected, actual )
}

TEST( XlinkActuate_onLoad, Enums )
{
	XlinkActuate e = XlinkActuate::onLoad;
	std::string expected = "onLoad";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkActuate e2 = parseXlinkActuate( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XlinkActuate_onRequest, Enums )
{
	XlinkActuate e = XlinkActuate::onRequest;
	std::string expected = "onRequest";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkActuate e2 = parseXlinkActuate( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XlinkActuate_other, Enums )
{
	XlinkActuate e = XlinkActuate::other;
	std::string expected = "other";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkActuate e2 = parseXlinkActuate( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XlinkActuate_none, Enums )
{
	XlinkActuate e = XlinkActuate::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkActuate e2 = parseXlinkActuate( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( XlinkShow_BadParse, Enums )
{
	XlinkShow actual = parseXlinkShow( "xyz" );
	XlinkShow expected = XlinkShow::new_;
	CHECK_EQUAL( expected, actual )
}
TEST( XlinkShow_new_, Enums )
{
	XlinkShow e = XlinkShow::new_;
	std::string expected = "new";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkShow e2 = parseXlinkShow( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XlinkShow_replace, Enums )
{
	XlinkShow e = XlinkShow::replace;
	std::string expected = "replace";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkShow e2 = parseXlinkShow( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XlinkShow_, Enums )
{
	XlinkShow e = XlinkShow::new_;
	std::string expected = "new";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkShow e2 = parseXlinkShow( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XlinkShow_embed, Enums )
{
	XlinkShow e = XlinkShow::embed;
	std::string expected = "embed";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkShow e2 = parseXlinkShow( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XlinkShow_other, Enums )
{
	XlinkShow e = XlinkShow::other;
	std::string expected = "other";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkShow e2 = parseXlinkShow( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XlinkShow_none, Enums )
{
	XlinkShow e = XlinkShow::none;
	std::string expected = "none";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkShow e2 = parseXlinkShow( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( XlinkType_BadParse, Enums )
{
	XlinkType actual = parseXlinkType( "xyz" );
	XlinkType expected = XlinkType::simple;
	CHECK_EQUAL( expected, actual )
}
TEST( XlinkType_simple, Enums )
{
	XlinkType e = XlinkType::simple;
	std::string expected = "simple";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkType e2 = parseXlinkType( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XlinkType_extended, Enums )
{
	XlinkType e = XlinkType::extended;
	std::string expected = "extended";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkType e2 = parseXlinkType( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XlinkType_title, Enums )
{
	XlinkType e = XlinkType::title;
	std::string expected = "title";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkType e2 = parseXlinkType( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XlinkType_resource, Enums )
{
	XlinkType e = XlinkType::resource;
	std::string expected = "resource";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkType e2 = parseXlinkType( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XlinkType_locator, Enums )
{
	XlinkType e = XlinkType::locator;
	std::string expected = "locator";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkType e2 = parseXlinkType( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XlinkType_arc, Enums )
{
	XlinkType e = XlinkType::arc;
	std::string expected = "arc";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XlinkType e2 = parseXlinkType( expected );
	CHECK_EQUAL( e, e2 )
}

TEST( XmlSpace_BadParse, Enums )
{
	XmlSpace actual = parseXmlSpace( "xyz" );
	XmlSpace expected = XmlSpace::default_;
	CHECK_EQUAL( expected, actual )
}
TEST( XmlSpace_default_, Enums )
{
	XmlSpace e = XmlSpace::default_;
	std::string expected = "default";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XmlSpace e2 = parseXmlSpace( expected );
	CHECK_EQUAL( e, e2 )
}
TEST( XmlSpace_preserve, Enums )
{
	XmlSpace e = XmlSpace::preserve;
	std::string expected = "preserve";
	std::string actual = toString( e );
	CHECK_EQUAL( expected, actual );
    
	std::stringstream sstr;
	toStream( sstr, e );
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	sstr.str( "" );
	sstr << e;
	actual = sstr.str();
	CHECK_EQUAL( expected, actual );
    
	XmlSpace e2 = parseXmlSpace( expected );
	CHECK_EQUAL( e, e2 )
}


TEST( DynamicsValue_beam, Enums )
{
    DynamicsEnum enumval = DynamicsEnum::ppppp;
    DynamicsEnum diffval = DynamicsEnum::pp;
    std::string strval = toString( enumval );
    std::string difval = "enclosure";
    std::string badstr = "someothertype";
    DynamicsValue object( enumval );
    DynamicsEnum expected = enumval;
    DynamicsEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object2( diffval );
    expected = diffval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    object2.setValue( enumval );
    expected = enumval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object3( diffval );
    object3 = parseDynamicsValue( strval );
    expected = enumval;
    actual = object3.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object4( strval );
    expected = enumval;
    actual = object4.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object5( badstr );
    expected = DynamicsEnum::otherDynamics;
    actual = object5.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object6( enumval );
    std::string expected_str = strval;
    std::string actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    std::stringstream ss;
    toStream( ss, object6 );
    expected_str = strval;
    actual_str = ss.str();
    CHECK_EQUAL( expected_str, actual_str )
    
    object6 = parseDynamicsValue( badstr );
    expected_str = badstr;
    actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    ss.str( "" );
    ss << object6;
    CHECK_EQUAL( expected_str, actual_str )
}

TEST( DynamicsValue_tieTip, Enums )
{
    DynamicsEnum enumval = DynamicsEnum::ff;
    DynamicsEnum diffval = DynamicsEnum::sffz;
    std::string strval = toString( enumval );
    std::string difval = "leger";
    std::string badstr = "someothertype";
    DynamicsValue object( enumval );
    DynamicsEnum expected = enumval;
    DynamicsEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object2( diffval );
    expected = diffval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    object2.setValue( enumval );
    expected = enumval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object3( diffval );
    object3 = parseDynamicsValue( strval );
    expected = enumval;
    actual = object3.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object4( strval );
    expected = enumval;
    actual = object4.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object5( badstr );
    expected = DynamicsEnum::otherDynamics;
    actual = object5.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object6( enumval );
    std::string expected_str = strval;
    std::string actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    std::stringstream ss;
    toStream( ss, object6 );
    expected_str = strval;
    actual_str = ss.str();
    CHECK_EQUAL( expected_str, actual_str )
    
    object6 = parseDynamicsValue( badstr );
    expected_str = badstr;
    actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    ss.str( "" );
    ss << object6;
    CHECK_EQUAL( expected_str, actual_str )
}

TEST( DynamicsValue_other, Enums )
{
    DynamicsEnum enumval = DynamicsEnum::otherDynamics;
    DynamicsEnum diffval = DynamicsEnum::rfz;
    std::string strval = ""; // toString( enumval );
    std::string difval = "enclosure";
    std::string badstr = "someothertype";
    DynamicsValue object( enumval );
    DynamicsEnum expected = enumval;
    DynamicsEnum actual = object.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object2( diffval );
    expected = diffval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    object2.setValue( enumval );
    expected = enumval;
    actual = object2.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object3( diffval );
    object3 = parseDynamicsValue( strval );
    expected = enumval;
    actual = object3.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object4( strval );
    expected = enumval;
    actual = object4.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object5( badstr );
    expected = DynamicsEnum::otherDynamics;
    actual = object5.getValue();
    CHECK_EQUAL( expected, actual )
    
    DynamicsValue object6( enumval );
    std::string expected_str = strval;
    std::string actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    std::stringstream ss;
    toStream( ss, object6 );
    expected_str = strval;
    actual_str = ss.str();
    CHECK_EQUAL( expected_str, actual_str )
    
    object6 = parseDynamicsValue( badstr );
    expected_str = badstr;
    actual_str = toString( object6 );
    CHECK_EQUAL( expected_str, actual_str )
    
    ss.str( "" );
    ss << object6;
    CHECK_EQUAL( expected_str, actual_str )
}

#endif
