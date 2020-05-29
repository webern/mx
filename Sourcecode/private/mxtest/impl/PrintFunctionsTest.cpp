// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"

#ifdef MX_COMPILE_IMPL_TESTS

    #include "cpul/cpulTestHarness.h"
    #include "mx/impl/PrintFunctions.h"
    #include "mx/core/elements/FermataAttributes.h"
    #include "mx/core/elements/DynamicsAttributes.h"

using namespace mx;
using namespace mx::impl;

TEST( getColor, PrintFunctions )
{
    core::FermataAttributes attr;
    attr.hasColor = true;
    attr.color.setRed( 1 );
    attr.color.setGreen( 2 );
    attr.color.setBlue( 3 );
    attr.color.setColorType( core::Color::ColorType::RGB );
    auto printData = impl::getPrintData( attr );
    auto colorData = printData.color;
    CHECK( printData.isColorSpecified );
    CHECK( !colorData.isAlphaSpecified );
    CHECK_EQUAL( static_cast<int>( 1 ), static_cast<int>( colorData.red ) );
    CHECK_EQUAL( static_cast<int>( 2 ), static_cast<int>( colorData.green ) );
    CHECK_EQUAL( static_cast<int>( 3 ), static_cast<int>( colorData.blue ) );
}

T_END


TEST( getColorAlpha, PrintFunctions )
{
    core::FermataAttributes attr;
    attr.hasColor = true;
    attr.color.setRed( 4 );
    attr.color.setGreen( 3 );
    attr.color.setBlue( 2 );
    attr.color.setColorType( core::Color::ColorType::ARGB );
    attr.color.setAlpha( 1 );
    auto printData = impl::getPrintData( attr );
    auto colorData = printData.color;
    CHECK( printData.isColorSpecified );
    CHECK( colorData.isAlphaSpecified );
    CHECK_EQUAL( static_cast<int>( 4 ), static_cast<int>( colorData.red ) );
    CHECK_EQUAL( static_cast<int>( 3 ), static_cast<int>( colorData.green ) );
    CHECK_EQUAL( static_cast<int>( 2 ), static_cast<int>( colorData.blue ) );
    CHECK_EQUAL( static_cast<int>( 1 ), static_cast<int>( colorData.alpha ) );
}

T_END


TEST( getColorFalse, PrintFunctions )
{
    core::FermataAttributes attr;
    attr.hasColor = false;
    attr.color.setRed( 4 );
    attr.color.setGreen( 3 );
    attr.color.setBlue( 2 );
    attr.color.setColorType( core::Color::ColorType::ARGB );
    attr.color.setAlpha( 1 );
    auto printData = impl::getPrintData( attr );
    auto colorData = printData.color;
    CHECK( !printData.isColorSpecified );
    CHECK( !colorData.isAlphaSpecified );
    CHECK_EQUAL( static_cast<int>( 255 ), static_cast<int>( colorData.red ) );
    CHECK_EQUAL( static_cast<int>( 255 ), static_cast<int>( colorData.green ) );
    CHECK_EQUAL( static_cast<int>( 255 ), static_cast<int>( colorData.blue ) );
    CHECK_EQUAL( static_cast<int>( 255 ), static_cast<int>( colorData.alpha ) );
}

T_END


TEST( setColor, PrintFunctions )
{
    api::ColorData color;
    color.red = 1;
    color.green = 2;
    color.blue = 3;
    color.isAlphaSpecified = false;
    color.alpha = 4;
    api::PrintData printData;
    printData.isColorSpecified = true;
    printData.color = color;
    core::FermataAttributes attr;
    impl::setAttributesFromPrintData( printData, attr );
    CHECK( attr.hasColor );
    const auto& core = attr.color;
    CHECK_EQUAL( 1, static_cast<int>( core.getRed() ) );
    CHECK_EQUAL( 2, static_cast<int>( core.getGreen() ) );
    CHECK_EQUAL( 3, static_cast<int>( core.getBlue() ) );
    CHECK( core::Color::ColorType::RGB == core.getColorType() );
    CHECK_EQUAL( 255, static_cast<int>( core.getAlpha() ) );
}

T_END


TEST( setColorAlpha, PrintFunctions )
{
    api::ColorData color;
    color.red = 4;
    color.green = 3;
    color.blue = 2;
    color.isAlphaSpecified = true;
    color.alpha = 1;
    api::PrintData printData;
    printData.isColorSpecified = true;
    printData.color = color;
    core::FermataAttributes attr;
    impl::setAttributesFromPrintData( printData, attr );
    CHECK( attr.hasColor );
    const auto& core = attr.color;
    CHECK_EQUAL( 4, static_cast<int>( core.getRed() ) );
    CHECK_EQUAL( 3, static_cast<int>( core.getGreen() ) );
    CHECK_EQUAL( 2, static_cast<int>( core.getBlue() ) );
    CHECK( core::Color::ColorType::ARGB == core.getColorType() );
    CHECK_EQUAL( 1, static_cast<int>( core.getAlpha() ) );
}

T_END


TEST( setColorFalse, PrintFunctions )
{
    api::ColorData color;
    color.red = 4;
    color.green = 3;
    color.blue = 2;
    color.isAlphaSpecified = true;
    color.alpha = 1;
    api::PrintData printData;
    printData.isColorSpecified = false;
    printData.color = color;
    core::FermataAttributes attr;
    impl::setAttributesFromPrintData( printData, attr );
    CHECK( !attr.hasColor );
    const auto& core = attr.color;
    CHECK_EQUAL( 255, static_cast<int>( core.getRed() ) );
    CHECK_EQUAL( 255, static_cast<int>( core.getGreen() ) );
    CHECK_EQUAL( 255, static_cast<int>( core.getBlue() ) );
    CHECK( core::Color::ColorType::RGB == core.getColorType() );
    CHECK_EQUAL( 255, static_cast<int>( core.getAlpha() ) );
}

T_END


TEST( everythingSet, PrintFunctions )
{
    api::PrintData p;
    p.fontData.lineThrough = 1;
    p.fontData.overline = 2;
    p.fontData.underline = 3;
    p.fontData.sizeType = api::FontSizeType::point;
    p.fontData.sizePoint = 2.0;
    p.fontData.style = api::FontStyle::italic;
    p.fontData.weight = api::FontWeight::bold;
    p.fontData.fontFamily.emplace_back( "z" );
    p.fontData.fontFamily.emplace_back( "ABC" );
    core::DynamicsAttributes attr;
    impl::setAttributesFromPrintData( p, attr );
    CHECK( attr.hasLineThrough );
    CHECK_EQUAL( 1, attr.lineThrough.getValue() );
    CHECK( attr.hasOverline );
    CHECK_EQUAL( 2, attr.overline.getValue() );
    CHECK( attr.hasUnderline );
    CHECK_EQUAL( 3, attr.underline.getValue() );
    CHECK( attr.hasFontSize );
    CHECK( attr.fontSize.getIsNumber() );
    CHECK_DOUBLES_EQUAL( 2.0, attr.fontSize.getValueNumber().getValue(), 0.01 );
    CHECK( attr.hasFontStyle );
    CHECK( core::FontStyle::italic == attr.fontStyle );
    CHECK( attr.hasFontWeight );
    CHECK( core::FontWeight::bold == attr.fontWeight );
    CHECK( attr.hasFontFamily );
    CHECK_EQUAL( 2, attr.fontFamily.getValues().size() );
    CHECK_EQUAL( "z", attr.fontFamily.getValues().front().getValue() );
    CHECK_EQUAL( "ABC", attr.fontFamily.getValues().back().getValue() );
}

T_END


TEST( everythingGet, PrintFunctions )
{
    core::DynamicsAttributes attr;
    attr.hasLineThrough = true;
    attr.lineThrough.setValue( 1 );
    attr.hasOverline = true;
    attr.overline.setValue( 2 );
    attr.hasUnderline = true;
    attr.underline.setValue( 3 );
    attr.hasFontSize = true;
    attr.fontSize.setValue( core::PositiveDecimal{ 2.0 } );
    attr.hasFontStyle = true;
    attr.fontStyle = core::FontStyle::italic;
    attr.hasFontWeight = true;
    attr.fontWeight = core::FontWeight::bold;
    attr.hasFontFamily = true;
    core::XsTokenSet values;
    values.emplace_back( "z" );
    values.emplace_back( "ABC" );
    attr.fontFamily.setValues( values );
    auto p = impl::getPrintData( attr );
    CHECK_EQUAL( 1, p.fontData.lineThrough );
    CHECK_EQUAL( 2, p.fontData.overline );
    CHECK_EQUAL( 3, p.fontData.underline );
    CHECK( api::FontSizeType::point == p.fontData.sizeType );
    CHECK_DOUBLES_EQUAL( 2.0, p.fontData.sizePoint, 0.01 );
    CHECK( api::FontStyle::italic == p.fontData.style );
    CHECK( api::FontWeight::bold == p.fontData.weight );
    CHECK_EQUAL( 2, p.fontData.fontFamily.size() );
    CHECK_EQUAL( "z", p.fontData.fontFamily.front() );
    CHECK_EQUAL( "ABC", p.fontData.fontFamily.back() );

}

T_END

#endif
