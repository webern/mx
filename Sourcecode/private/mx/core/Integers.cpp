// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// self
#include "mx/core/Integers.h"

// std
#include <sstream>

namespace mx
{
    namespace core
    {
        Integer::Integer()
        : Integer{ 0 }
        {

        }

        Integer::Integer( IntType value )
        : myValue{ value }
        {

        }
        
        IntType Integer::getValue() const
        {
            return myValue;
        }

        void Integer::setValue( IntType value )
        {
            myValue = value;
        }

        bool Integer::parse( const std::string& value )
        {
            std::stringstream ss( value );
            IntType temp = 0;
            if ( ( ss >> temp ).fail() || !( ss >> std::ws ).eof())
            {
                return false;
            }
            setValue( temp );
            return true;
        }

        std::string toString( const Integer& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }

        std::ostream& toStream( std::ostream& os, const Integer& value )
        {
            return os << value.getValue();
        }


        std::ostream& operator<<( std::ostream& os, const Integer& value )
        {
            return toStream( os, value );
        }

        inline IntType clamp( IntType min, IntType max, IntType value )
        {
            if( value < min )
            {
                return min;
            }
            else if( value > max )
            {
                return max;
            }
            else
            {
                return value;
            }
        }

        IntRange::IntRange( IntType min, IntType max, IntType value )
        : Integer{ clamp( min, max, value ) }
        , myMin{ min }
        , myMax{ max }
        {
            
        }

        void IntRange::setValue( IntType value )
        {
            Integer::setValue( clamp( myMin, myMax, value ) );
        }

        AccordionMiddleValue::AccordionMiddleValue()
        :AccordionMiddleValue{ 0 }
        {

        }

        AccordionMiddleValue::AccordionMiddleValue( IntType value )
        :IntRange( 0, 3, value )
        {

        }

        BeamLevel::BeamLevel()
        :BeamLevel{ 0 }
        {

        }

        BeamLevel::BeamLevel( IntType value )
        :IntRange( 1, 8, value )
        {

        }

        Byte::Byte()
        :Byte{ 0 }
        {

        }

        Byte::Byte( IntType value )
        :IntRange( 0, 255, value )
        {

        }

        FifthsValue::FifthsValue()
        :FifthsValue{ 0 }
        {

        }

        FifthsValue::FifthsValue( IntType value )
        :IntRange( IntMin, IntMax, value )
        {

        }

        Midi128::Midi128()
        :Midi128{ 0 }
        {

        }

        Midi128::Midi128( IntType value )
        :IntRange( 1, 128, value )
        {

        }

        Midi16::Midi16()
        :Midi16{ 0 }
        {

        }

        Midi16::Midi16( IntType value )
        :IntRange( 1, 16, value )
        {

        }

        Midi16384::Midi16384()
        :Midi16384{ 0 }
        {

        }

        Midi16384::Midi16384( IntType value )
        :IntRange( 1, 16384, value )
        {

        }

        NonNegativeInteger::NonNegativeInteger()
        :NonNegativeInteger{ 0 }
        {

        }

        NonNegativeInteger::NonNegativeInteger( IntType value )
        :IntRange( 0, IntMax, value )
        {

        }

        NumberLevel::NumberLevel()
        :NumberLevel{ 0 }
        {

        }

        NumberLevel::NumberLevel( IntType value )
        :IntRange( 1, 6, value )
        {

        }

        NumberOfLines::NumberOfLines()
        :NumberOfLines{ 0 }
        {

        }

        NumberOfLines::NumberOfLines( IntType value )
        :IntRange( 0, 3, value )
        {

        }

        OctaveValue::OctaveValue()
        :OctaveValue{ 0 }
        {

        }

        OctaveValue::OctaveValue( IntType value )
        :IntRange( 0, 9, value )
        {

        }

        PositiveInteger::PositiveInteger()
        :PositiveInteger{ 0 }
        {

        }

        PositiveInteger::PositiveInteger( IntType value )
        :IntRange( 1, IntMax, value )
        {

        }

        StaffLine::StaffLine()
        :StaffLine{ 0 }
        {

        }

        StaffLine::StaffLine( IntType value )
        :IntRange( IntMin, IntMax, value )
        {

        }

        StaffNumber::StaffNumber()
        :StaffNumber{ 0 }
        {

        }

        StaffNumber::StaffNumber( IntType value )
        :IntRange( 1, IntMax, value )
        {

        }

        StringNumber::StringNumber()
        :StringNumber{ 0 }
        {

        }

        StringNumber::StringNumber( IntType value )
        :IntRange( 1, IntMax, value )
        {

        }

        TremoloMarks::TremoloMarks()
        :TremoloMarks{ 0 }
        {

        }

        TremoloMarks::TremoloMarks( IntType value )
        :IntRange( 0, 8, value )
        {

        }
    }
}
