// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Integers.h"
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
        : myValue{}
        {
            // for child classes that clamp the range
            setValue( 0 );
        }

        IntType Integer::getValue() const
        {
            return myValue;
        }

        void Integer::setValue( IntType value )
        {
            myValue = value;
        }

        void Integer::parse( const std::string& value )
        {
            std::stringstream ss( value );
            IntType temp = 0;
            if ((ss >> temp).fail() || !(ss >> std::ws).eof())
                return;
            }
            setValue( temp );
        }

        /// AccordionMiddleValue ///////////////////////////////////////////////////////////////////

        AccordionMiddleValue::AccordionMiddleValue()
        : Integer{}
        {

        }

        AccordionMiddleValue::AccordionMiddleValue( IntType value )
        : Integer{ value }
        {

        }

        void AccordionMiddleValue::setValue( IntType value )
        {
            if( value < 1 )
            {
                myValue = 1;
            }
            else if( value > 3 )
            {
                myValue = 3;
            }
            else
            {
                myValue = value;
            }
        }

        /// BeamLevel //////////////////////////////////////////////////////////////////////////////

        BeamLevel::BeamLevel()
        : Integer{}
        {

        }

        BeamLevel::BeamLevel( IntType value )
        : Integer{ value }
        {

        }

        void BeamLevel::setValue( IntType value )
        {
            if( value < 1 )
            {
                myValue = 1;
            }
            else if( value > 8 )
            {
                myValue = 8;
            }
            else
            {
                myValue = value;
            }
        }

        /// Byte ///////////////////////////////////////////////////////////////////////////////////

        Byte::Byte()
        : Integer{}
        {

        }

        Byte::Byte( IntType value )
        : Integer{ value }
        {

        }

        void Byte::setValue( IntType value )
        {
            if( value < 0 )
            {
                myValue = 0;
            }
            else if( value > 255 )
            {
                myValue = 255;
            }
            else
            {
                myValue = value;
            }
        }

        /// FifthsValue ////////////////////////////////////////////////////////////////////////////

        FifthsValue::FifthsValue()
        : Integer{}
        {

        }

        FifthsValue::FifthsValue( IntType value )
        : Integer{ value }
        {

        }

        void FifthsValue::setValue( IntType value )
        {
            myValue = value;
        }

        /// Midi128 ////////////////////////////////////////////////////////////////////////////////

        Midi128::Midi128()
        : Integer{}
        {

        }

        Midi128::Midi128( IntType value )
        : Integer{ value }
        {

        }

        void Midi128::setValue( IntType value )
        {
            if( value < 1 )
            {
                myValue = 1;
            }
            else if( value > 128 )
            {
                myValue = 128;
            }
            else
            {
                myValue = value;
            }
        }

        /// Midi16 /////////////////////////////////////////////////////////////////////////////////

        Midi16::Midi16()
        : Integer{}
        {

        }

        Midi16::Midi16( IntType value )
        : Integer{ value }
        {

        }

        void Midi16::setValue( IntType value )
        {
            if( value < 1 )
            {
                myValue = 1;
            }
            else if( value > 16 )
            {
                myValue = 16;
            }
            else
            {
                myValue = value;
            }
        }

        /// Midi16384 //////////////////////////////////////////////////////////////////////////////

        Midi16384::Midi16384()
        : Integer{}
        {

        }

        Midi16384::Midi16384( IntType value )
        : Integer{ value }
        {

        }

        void Midi16384::setValue( IntType value )
        {
            if( value < 1 )
            {
                myValue = 1;
            }
            else if( value > 16384 )
            {
                myValue = 16384;
            }
            else
            {
                myValue = value;
            }
        }

        /// NonNegativeInteger /////////////////////////////////////////////////////////////////////

        NonNegativeInteger::NonNegativeInteger()
        : Integer{}
        {

        }

        NonNegativeInteger::NonNegativeInteger( IntType value )
        : Integer{ value }
        {

        }

        void NonNegativeInteger::setValue( IntType value )
        {
            if( value < 0 )
            {
                myValue = 0;
            }
            else
            {
                myValue = value;
            }
        }

        /// NumberLevel ////////////////////////////////////////////////////////////////////////////

        NumberLevel::NumberLevel()
        : Integer{}
        {

        }

        NumberLevel::NumberLevel( IntType value )
        : Integer{ value }
        {

        }

        void NumberLevel::setValue( IntType value )
        {
            if( value < 1 )
            {
                myValue = 1;
            }
            else if( value > 6 )
            {
                myValue = 6;
            }
            else
            {
                myValue = value;
            }
        }

        /// NumberOfLines //////////////////////////////////////////////////////////////////////////

        NumberOfLines::NumberOfLines()
        : Integer{}
        {

        }

        NumberOfLines::NumberOfLines( IntType value )
        : Integer{ value }
        {

        }

        void NumberOfLines::setValue( IntType value )
        {
            if( value < 0 )
            {
                myValue = 0;
            }
            else if( value > 3 )
            {
                myValue = 3;
            }
            else
            {
                myValue = value;
            }
        }

        /// OctaveValue ////////////////////////////////////////////////////////////////////////////

        OctaveValue::OctaveValue()
        : Integer{}
        {

        }

        OctaveValue::OctaveValue( IntType value )
        : Integer{ value }
        {

        }

        void OctaveValue::setValue( IntType value )
        {
            if( value < 0 )
            {
                myValue = 0;
            }
            else if( value > 9 )
            {
                myValue = 9;
            }
            else
            {
                myValue = value;
            }
        }

        /// PositiveInteger ////////////////////////////////////////////////////////////////////////

        PositiveInteger::PositiveInteger()
        : Integer{}
        {

        }

        PositiveInteger::PositiveInteger( IntType value )
        : Integer{ value }
        {

        }

        void PositiveInteger::setValue( IntType value )
        {
            if( value < 1 )
            {
                myValue = 1;
            }
            else
            {
                myValue = value;
            }
        }

        /// StaffLine //////////////////////////////////////////////////////////////////////////////

        StaffLine::StaffLine()
        : Integer{}
        {

        }

        StaffLine::StaffLine( IntType value )
        : Integer{ value }
        {

        }

        void StaffLine::setValue( IntType value )
        {
            myValue = value;
        }

        /// StaffNumber ////////////////////////////////////////////////////////////////////////////

        StaffNumber::StaffNumber()
        : Integer{}
        {

        }

        StaffNumber::StaffNumber( IntType value )
        : Integer{ value }
        {

        }

        void StaffNumber::setValue( IntType value )
        {
            if( value < 1 )
            {
                myValue = 1;
            }
            else
            {
                myValue = value;
            }
        }

        /// StringNumber ///////////////////////////////////////////////////////////////////////////

        StringNumber::StringNumber()
        : Integer{}
        {

        }

        StringNumber::StringNumber( IntType value )
        : Integer{ value }
        {

        }

        void StringNumber::setValue( IntType value )
        {
            if( value < 1 )
            {
                myValue = 1;
            }
            else
            {
                myValue = value;
            }
        }

        /// TremoloMarks ///////////////////////////////////////////////////////////////////////////

        TremoloMarks::TremoloMarks()
        : Integer{}
        {

        }

        TremoloMarks::TremoloMarks( IntType value )
        : Integer{ value }
        {

        }

        void TremoloMarks::setValue( IntType value )
        {
            if( value < 0 )
            {
                myValue = 0;
            }
            else if( value > 8 )
            {
                myValue = 8;
            }
            else
            {
                myValue = value;
            }
        }
    }
}
