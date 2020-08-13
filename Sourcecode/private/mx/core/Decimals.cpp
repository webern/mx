// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// self
#include "mx/core/Decimals.h"

// lib
#include "mx/core/UnusedParameter.h"

// std
#include <cmath>
#include <sstream>

namespace mx
{
    namespace core
    {
        PreciseDecimal::PreciseDecimal( short inMaxIntegerDigits, short inMaxDecimalDigits, DecimalType inValue )
                : myIsNegative{ inValue < 0.0 }
                , myInteger{ 0 }
                , myDecimal{ 0 }
                , myMaxIntegerDigits{ DEFAULT_MAX_INTEGER_DIGITS }
                , myMaxDecimalDigits{ DEFAULT_MAX_DECIMAL_DIGITS }
        {
            setMaxIntegerDigits( inMaxIntegerDigits );
            setMaxDecimalDigits( inMaxDecimalDigits );
            setMaxExpressibleNumber();
            setValue( inValue );
        }

        PreciseDecimal::PreciseDecimal( short inMaxIntegerDigits, short inMaxDecimalDigits )
        : PreciseDecimal{ inMaxIntegerDigits, inMaxDecimalDigits, 0.0 }
        {

        }

        PreciseDecimal::PreciseDecimal( DecimalType inValue )
        : PreciseDecimal{ DEFAULT_MAX_INTEGER_DIGITS, DEFAULT_MAX_DECIMAL_DIGITS, inValue }
        {

        }

        PreciseDecimal::PreciseDecimal()
        : PreciseDecimal{ DEFAULT_MAX_INTEGER_DIGITS, DEFAULT_MAX_DECIMAL_DIGITS, 0.0 }
        {

        }

        DecimalType PreciseDecimal::getValue() const
        {
            return express( myInteger, myDecimal, myMaxDecimalDigits, myIsNegative );
        }

        static constexpr const long double POINT_FIVE = 0.4999999;

        void PreciseDecimal::setValue( DecimalType inValue )
        {
            if ( inValue < 0 )
            {
                myIsNegative = true;
                inValue *= -1;
            }
            else
            {
                myIsNegative = false;
            }

            const auto integer = static_cast<decltype( myInteger )>( inValue );
            const bool isTooBig = integer > myMaxExpressibleInteger;

            if( isTooBig )
            {
                myInteger = myMaxExpressibleInteger;
                myDecimal = myMaxExpressibleDecimal;
                return;
            }

            const DecimalType decimalPartAsFloat = inValue - static_cast<DecimalType>( integer );
            const auto powerMultiplierAsFloat = std::pow( static_cast<DecimalType>( 10 ), static_cast<DecimalType>( getMaxDecimalDigits() ) );
            const auto powerMultiplier = static_cast<uint64_t>( std::ceil( powerMultiplierAsFloat - POINT_FIVE ) );
            const auto decimalDigitsShifted = decimalPartAsFloat * static_cast<decltype( decimalPartAsFloat )>( powerMultiplier );
            const auto decimalDigits = static_cast<decltype( myDecimal )>( std::ceil( decimalDigitsShifted - POINT_FIVE ) );

            if( decimalDigits > myMaxExpressibleDecimal )
            {
                myInteger = integer + 1;
                myDecimal = 0;
                return;
            }

            myInteger = integer;
            myDecimal = decimalDigits;
        }

        short PreciseDecimal::getMaxIntegerDigits() const
        {
            return myMaxIntegerDigits;
        }

        short PreciseDecimal::getMaxDecimalDigits() const
        {
            return myMaxDecimalDigits;
        }

        DecimalType PreciseDecimal::getMaxExpressibleNumber() const
        {
            return express( myMaxExpressibleInteger, myMaxExpressibleDecimal, myMaxDecimalDigits, myIsNegative );
        }

        DecimalType PreciseDecimal::getMinExpressibleNumber() const
        {
            return -1 * getMaxExpressibleNumber();
        }

        std::ostream& PreciseDecimal::toStream( std::ostream& os ) const
        {
            if( myInteger == 0 && myDecimal == 0 )
            {
                os << "0";
                return os;
            }

            if( myIsNegative )
            {
                os << "-";
            }

            os << myInteger;

            if( myDecimal == 0 )
            {
                return os;
            }

            os << ".";

            auto decIntStr = std::to_string( myDecimal );
            std::stringstream ss;

            bool isNonZeroReached = false;

            for ( auto c = decIntStr.crbegin();
                  c != decIntStr.crend();
                  ++c )
            {
                if( *c == '0' && !isNonZeroReached )
                {
                    continue;
                }

                isNonZeroReached = true;
                ss << *c;
            }

            const auto backwards = ss.str();

            short digits = 0;
            decltype( myDecimal ) tens = 1;

            for( int i = 0; i <= myMaxDecimalDigits; ++i )
            {
                ++digits;
                tens *= 10;
                const auto r = myDecimal / tens;

                if( r < 1 )
                {
                    break;
                }
            }

            short leadingZeros = myMaxDecimalDigits - digits;

            for ( short i = 0; i < leadingZeros; ++i )
            {
                os << "0";
            }

            for ( auto c = backwards.crbegin();
                  c != backwards.crend();
                  ++c )
            {
                os << *c;
            }

            return os;
        }

        std::string PreciseDecimal::toString() const
        {
            std::stringstream ss;
            toStream( ss );
            return ss.str();
        }

        void PreciseDecimal::setMaxIntegerDigits( short inNumDigits )
        {
            if( inNumDigits < 1 )
            {
                myMaxIntegerDigits = 1;
                return;
            }

            if( inNumDigits > MAX_STORAGE_TYPE_DIGITS )
            {
                myMaxIntegerDigits = MAX_STORAGE_TYPE_DIGITS;
                return;
            }

            myMaxIntegerDigits = inNumDigits;
        }

        void PreciseDecimal::setMaxDecimalDigits( short inNumDigits )
        {
            if( inNumDigits < 1 )
            {
                myMaxDecimalDigits = 1;
                return;
            }

            if( inNumDigits > MAX_STORAGE_TYPE_DIGITS )
            {
                myMaxDecimalDigits = MAX_STORAGE_TYPE_DIGITS;
                return;
            }

            myMaxDecimalDigits = inNumDigits;
        }

        decltype( PreciseDecimal::myInteger ) PreciseDecimal::findMax( short inNumDigits )
        {
            decltype( myInteger ) mult = 0;
            decltype( myInteger ) result = 0;

            for( short i = 0; i < inNumDigits; ++i )
            {
                mult = static_cast<decltype( myInteger )>( std::ceil( std::pow( 10, i ) - 0.5) );
                const auto add = mult * 9;
                result += add;
            }

            return result;
        }

        void PreciseDecimal::setMaxExpressibleInteger()
        {
            myMaxExpressibleInteger = findMax( getMaxIntegerDigits() );
        }

        void PreciseDecimal::setMaxExpressibleDecimal()
        {
            myMaxExpressibleDecimal = findMax( getMaxDecimalDigits() );
        }

        void PreciseDecimal::setMaxExpressibleNumber()
        {
            setMaxExpressibleInteger();
            setMaxExpressibleDecimal();
        }

        DecimalType PreciseDecimal::express( uint64_t inInteger, uint64_t inDecimal, short inMaxDecimalDigits, bool inIsNegative )
        {
            const auto integer = static_cast<DecimalType>( inInteger );
            const auto decimalPartAsFloat = static_cast<DecimalType>( inDecimal );
            const auto powerMultiplierAsFloat = std::pow( static_cast<DecimalType>( 10 ), static_cast<DecimalType>( inMaxDecimalDigits ) );
            const auto powerMultiplier = static_cast<DecimalType>( std::ceil( powerMultiplierAsFloat - 0.5 ) );
            const auto decimal = decimalPartAsFloat / powerMultiplier;
            auto result = integer + decimal;

            if( inIsNegative )
            {
                result *= -1;
            }

            return result;
        }

        Decimal::Decimal( DecimalType value )
        : myValue{ 19, DEFAULT_PRECISION, value }
        {

        }

        Decimal::Decimal()
        : Decimal{ 0.0 }
        {

        }

        DecimalType Decimal::getValue() const
        {
            return myValue.getValue();
        }

        void Decimal::setValue( DecimalType value )
        {
            myValue.setValue( value );
        }

        bool Decimal::parse( const std::string& value )
        {
            std::stringstream ss( value );
            DecimalType temp = 0;
            if ( ( ss >> temp ).fail() || !( ss >> std::ws ).eof())
            {
                return false;
            }
            setValue( temp );
            return true;
        }

        std::string Decimal::toString() const
        {
            return myValue.toString();
        }


        std::ostream& Decimal::toStream( std::ostream& os ) const
        {
            return myValue.toStream( os );
        }


        std::string toString( const Decimal& value, unsigned int precision )
        {
            MX_UNUSED( precision );
            return value.toString();
        }


        std::ostream& toStream( std::ostream& os, const Decimal& value, unsigned int precision )
        {
            MX_UNUSED( precision );
            return value.toStream( os );
        }

        std::string toString( const Decimal& value, unsigned short precision )
        {
            MX_UNUSED( precision );
            return value.toString();
        }

        std::ostream& toStream( std::ostream& os, const Decimal& value, unsigned short precision )
        {
            MX_UNUSED( precision );
            return value.toStream( os );
        }

        std::ostream& operator<<( std::ostream& os, const Decimal& value )
        {
            return value.toStream( os );
        }

        DecimalType minExclusive( DecimalType min, DecimalType val )
        {
            if( val <= min )
            {
                return min + NON_ZERO_AMOUNT;
            }

            return val;
        }

        DecimalType minInclusive( DecimalType min, DecimalType val )
        {
            if( val < min )
            {
                return min;
            }

            return val;
        }

        DecimalType maxExclusive( DecimalType max, DecimalType val )
        {
            if( val >= max )
            {
                return max - NON_ZERO_AMOUNT;
            }

            return val;
        }

        DecimalType maxInclusive( DecimalType max, DecimalType val )
        {
            if( val > max )
            {
                return max;
            }

            return val;
        }

        DecimalType noOp( DecimalType value )
        {
            return value;
        }

        DecimalRange::DecimalRange( DecimalClamp min, DecimalClamp max, DecimalType value )
        : Decimal{ min( max( value ) ) }
        , myMinClamp{ min }
        , myMaxClamp{ max }
        {

        }

        void DecimalRange::setValue( DecimalType value )
        {
            Decimal::setValue( myMinClamp( myMaxClamp( value ) ) );
        }

#define MXMINEX( minbound ) DecimalClamp( []( DecimalType value ){ return minExclusive( minbound, value ); } )
#define MXMAXEX( maxbound ) DecimalClamp( []( DecimalType value ){ return maxExclusive( maxbound, value ); } )
#define MXMININ( minbound ) DecimalClamp( []( DecimalType value ){ return minInclusive( minbound, value ); } )
#define MXMAXIN( maxbound ) DecimalClamp( []( DecimalType value ){ return maxInclusive( maxbound, value ); } )
#define MX_NOOP DecimalClamp( noOp )

        DivisionsValue::DivisionsValue( DecimalType value )
        : DecimalRange{ MX_NOOP, MX_NOOP, value }
        {

        }

        DivisionsValue::DivisionsValue()
        : DivisionsValue{ 0.0 }
        {

        }

        Millimeters::Millimeters( DecimalType value )
        : DecimalRange{ MX_NOOP, MX_NOOP, value }
        {

        }

        Millimeters::Millimeters()
        : Millimeters{ 0.0 }
        {

        }

        NonNegativeDecimal::NonNegativeDecimal( DecimalType value )
        : DecimalRange{ MXMININ( 0 ), MX_NOOP, value }
        {

        }

        NonNegativeDecimal::NonNegativeDecimal()
        : NonNegativeDecimal{ 0.0 }
        {

        }

        Percent::Percent( DecimalType value )
        : DecimalRange{ MXMININ( 0 ), MXMAXIN( 100 ), value }
        {

        }

        Percent::Percent()
        : Percent{ 0.0 }
        {

        }

        PositiveDecimal::PositiveDecimal( DecimalType value )
        : DecimalRange{ MXMINEX( 0 ), MX_NOOP, value }
        {

        }

        PositiveDecimal::PositiveDecimal()
        : PositiveDecimal{ 1 }
        {

        }

        PositiveDivisions::PositiveDivisions( DecimalType value )
        : DecimalRange{ MXMINEX( 0 ), MX_NOOP, value }
        {

        }

        PositiveDivisions::PositiveDivisions()
        : PositiveDivisions{ 1 }
        {

        }

        RotationDegrees::RotationDegrees( DecimalType value )
        : DecimalRange{ MXMININ( -180 ), MXMAXIN( 180 ), value }
        {

        }

        RotationDegrees::RotationDegrees()
        : RotationDegrees{ 0.0 }
        {

        }

        Semitones::Semitones( DecimalType value )
        : DecimalRange{ MX_NOOP, MX_NOOP, value }
        {

        }

        Semitones::Semitones()
        : Semitones{ 0.0 }
        {

        }

        TenthsValue::TenthsValue( DecimalType value )
        : DecimalRange{ MX_NOOP, MX_NOOP, value }
        {

        }

        TenthsValue::TenthsValue()
        : TenthsValue{ 0.0 }
        {

        }

        TrillBeats::TrillBeats( DecimalType value )
        : DecimalRange{ MXMININ( 2 ), MX_NOOP, value }
        {

        }

        TrillBeats::TrillBeats()
        : TrillBeats{ 2 }
        {

        }
    }
}