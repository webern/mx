// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/Integers.h"
#include <sstream>

namespace mx
{
    namespace core
    {
        Integer::Integer( IntType value )
        :myValue( value )
        {}
        
        
        Integer::Integer()
        :myValue( 0 )
        {}
        
        
        Integer::~Integer() {}
        
        
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
            {
                return;
            }
            ss >> temp;
            setValue( temp );
        }
        
        
        IntRange::IntRange( IntType min, IntType max, IntType value )
        :Integer( value )
        ,myMin( min )
        ,myMax( max )
        {
            setValue( value );
        }
        
        
        IntRange::~IntRange() {}
        
        
        IntRange::IntRange( IntRange&& other )
        :Integer( std::move( other ) )
        ,myMin( std::move( other.myMin ) )
        ,myMax( std::move( other.myMax ) )
        {}
        
        
        IntRange& IntRange::operator=( const IntRange& other )
        {
            this->setValue( other.getValue() );
            return *this;
        }
        
        
        IntRange& IntRange::operator=( IntRange&& other )
        {
            this->setValue( other.getValue() );
            return *this;
        }
        
        
        void IntRange::setValue( IntType value )
        {
            if ( value < myMin )
            {
                Integer::setValue( myMin );
            }
            else if ( value > myMax )
            {
                Integer::setValue( myMax );
            }
            else
            {
                Integer::setValue( value );
            }
        }
        
        
        void IntRange::parse( const std::string& value )
        {
            std::stringstream ss( value );
            IntType temp = 0;
            if ((ss >> temp).fail() || !(ss >> std::ws).eof())
            {
                return;
            }
            setValue( temp );
        }
        
        
        PositiveInteger::PositiveInteger( IntType value )
        :Integer( value )
        {
            setValue( value );
        }
        
        
        PositiveInteger::PositiveInteger()
        :Integer( 1 ) {}
        
        
        PositiveInteger::~PositiveInteger() {}
        
        
        void PositiveInteger::setValue( IntType value )
        {
            if ( value < 1 )
            {
                Integer::setValue( 1 );
            }
            else
            {
                Integer::setValue( value );
            }
        }
        
        
        void PositiveInteger::parse( const std::string& value )
        {
            std::stringstream ss( value );
            IntType temp = 1;
            if ((ss >> temp).fail() || !(ss >> std::ws).eof())
            {
                return;
            }
            setValue( temp );
        }
        
        
        NonNegativeInteger::NonNegativeInteger( IntType value )
        :Integer( value )
        {
            setValue( value );
        }
        
        
        NonNegativeInteger::NonNegativeInteger()
        :Integer( 1 )
        {
            setValue( 1 );
        }
        
        
        NonNegativeInteger::~NonNegativeInteger() {}
        
        
        void NonNegativeInteger::setValue( IntType value )
        {
            if ( value < 0 )
            {
                Integer::setValue( 0 );
            }
            else
            {
                Integer::setValue( value );
            }
        }
        
        
        void NonNegativeInteger::parse( const std::string& value )
        {
            std::stringstream ss( value );
            IntType temp = 0;
            if ((ss >> temp).fail() || !(ss >> std::ws).eof())
            {
                return;
            }
            this->setValue( temp );
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
        
        
        AccordionMiddleValue::AccordionMiddleValue( IntType value )
        :IntRange( 0, 3, value ) {}
        AccordionMiddleValue::AccordionMiddleValue()
        :IntRange( 0, 3, 0 ) {}
        
        
        BeamLevel::BeamLevel( IntType value )
        :IntRange( 1, 8, value ) {}
        BeamLevel::BeamLevel()
        :IntRange( 1, 8, 1 ) {}
        
        
        FifthsValue::FifthsValue( IntType value )
        :Integer( value ) {}
        FifthsValue::FifthsValue()
        :Integer( 0 ) {}
        
        
        Midi16::Midi16( IntType value )
        :IntRange( 1, 16, value ) {}
        Midi16::Midi16()
        :IntRange( 1, 16, 1 ) {}
        
        
        Midi128::Midi128( IntType value )
        :IntRange( 1, 128, value ) {}
        Midi128::Midi128()
        :IntRange( 1, 128, 1 ) {}
        
        
        Midi16384::Midi16384( IntType value )
        :IntRange( 1, 16384, value ) {}
        Midi16384::Midi16384()
        :IntRange( 1, 16384, 1 ) {}
        
        
        NumberLevel::NumberLevel( IntType value )
        :IntRange( 1, 6, value ) {}
        NumberLevel::NumberLevel()
        :IntRange( 1, 6, 1 ) {}
        
        
        NumberOfLines::NumberOfLines( IntType value )
        :IntRange( 0, 3, value ) {}
        NumberOfLines::NumberOfLines()
        :IntRange( 0, 3, 0 ) {}
        
        
        OctaveValue::OctaveValue( IntType value )
        :IntRange( 0, 9, value ) {}
        OctaveValue::OctaveValue()
        :IntRange( 0, 9, 0 ) {}
        
        
        StaffLine::StaffLine( IntType value )
        :Integer( value ) {}
        StaffLine::StaffLine()
        :Integer( 0 ) {}
        
        
        TremoloMarks::TremoloMarks( IntType value )
        :IntRange( 0, 8, value ) {}
        TremoloMarks::TremoloMarks()
        :IntRange( 0, 8, 0 ) {}
        
    }
}
