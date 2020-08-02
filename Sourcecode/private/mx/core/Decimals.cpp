// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Decimals.h"
#include "mx/core/PreciseDecimal.h"
#include "mx/core/UnusedParameter.h"

#include <sstream>
#include <iomanip>
#include <mutex>

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

        void Decimal::parse( const std::string& value )
        {
            std::stringstream ss( value );
            DecimalType temp = 0;
            if ( ( ss >> temp).fail() || !( ss >> std::ws).eof())
            {
                return;
            }
            setValue( temp );
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

        PositiveDecimal::PositiveDecimal( DecimalType value )
        : DecimalRange{
            std::function<DecimalType(DecimalType)>( []( DecimalType value ){ return minExclusive( 0.0, value ); } ),
            std::function<DecimalType(DecimalType)>( noOp ),
            value
        }
        {

        }

        PositiveDecimal::PositiveDecimal()
        : PositiveDecimal{ 1 }
        {

        }

        NonNegativeDecimal::NonNegativeDecimal( DecimalType value )
        : DecimalRange{
            std::function<DecimalType(DecimalType)>( []( DecimalType value ){ return minInclusive( 0.0, value ); } ),
            std::function<DecimalType(DecimalType)>( noOp ),
            value
        }
        {

        }

        NonNegativeDecimal::NonNegativeDecimal()
        : NonNegativeDecimal{ 0.0 }
        {

        }

        
        Percent::Percent( DecimalType value )
        : DecimalRange{
            std::function<DecimalType(DecimalType)>( []( DecimalType value ){ return minInclusive( 0.0, value ); } ),
            std::function<DecimalType(DecimalType)>( []( DecimalType value ){ return maxInclusive( 100.0, value ); } ),
            value
        }
        {

        }

        Percent::Percent()
        : Percent{ 0.0 }
        {

        }
        
        RotationDegrees::RotationDegrees( DecimalType value )
        : DecimalRange{
            std::function<DecimalType(DecimalType)>( []( DecimalType value ){ return minInclusive( -180.0, value ); } ),
            std::function<DecimalType(DecimalType)>( []( DecimalType value ){ return maxInclusive( 180.0, value ); } ),
            value
        }
        {

        }


        RotationDegrees::RotationDegrees()
        : RotationDegrees{ 0.0 }
        {

        }

        
        TrillBeats::TrillBeats( DecimalType value )
        : DecimalRange{
            std::function<DecimalType(DecimalType)>( []( DecimalType value ){ return minInclusive( 2.0, value ); } ),
            std::function<DecimalType(DecimalType)>( noOp ),
            value
        }
        {
            this->setValue( value );
        }

        TrillBeats::TrillBeats()
        : TrillBeats{ 0.0 }
        {

        }
    }
}
