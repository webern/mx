// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// self
#include "mx/core/YesNoNumber.h"

// std
#include <sstream>
#include <type_traits>

namespace mx
{
    namespace core
    {
        template<class> inline constexpr bool always_false_v = false;

        YesNoNumber::YesNoNumber()
        : myValue{ YesNo::yes }
        {

        }

        YesNoNumber::YesNoNumber( YesNo value )
        : myValue{ value }
        {

        }

        YesNoNumber::YesNoNumber( Decimal value )
        : myValue{ std::move( value ) }
        {

        }
        
        YesNoNumber::YesNoNumber( const std::string& value )
        : YesNoNumber{}
        {
            parse( value );
        }

        bool YesNoNumber::getIsYesNo() const
        {
            return myValue.index() == 0;
        }

        bool YesNoNumber::getIsDecimal() const
        {
            return myValue.index() == 1;
        }

        void YesNoNumber::setYesNo( YesNo value ) const
        {
            myValue.emplace<YesNo>( value );
        }

        void YesNoNumber::setDecimal( Decimal value ) const
        {
            myValue.emplace<Decimal>( value );
        }

        YesNo YesNoNumber::getValueYesNo() const
        {
            auto result = YesNo::yes;
            std::visit([&](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr( std::is_same_v<T, YesNo> )
                    result = arg;
                else if constexpr( std::is_same_v<T, Decimal> )
                    result = Decimal{};
                else
                    static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }, myValue);
            return result;
        }

        Decimal YesNoNumber::getValueDecimal() const
        {
            auto result = Decimal{};
            std::visit([&](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr( std::is_same_v<T, YesNo> )
                    result = YesNo::yes;
                else if constexpr( std::is_same_v<T, Decimal> )
                    result = arg;
                else
                    static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }, myValue);
            return result;
        }

        bool YesNoNumber::parse( const std::string& value )
        {
            const auto yesNo = tryParseYesNo( value );
            if( yesNo )
            {
                setValue( *yesNo );
                return true;
            }
            auto decimal = Decimal{};
            if( decimal.parse( value ) )
            {
                setValue( decimal );
                return true;
            }
            return false;
        }

        std::string toString( const YesNoNumber& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }

		std::ostream& toStream( std::ostream& os, const YesNoNumber& value )
        {
            if( getIsYesNo() )
            {
                toStream( os, value.getValueYesNo() );
            }
            if( getIsDecimal() )
            {
                toStream( os, value.getValueDecimal() );
            }
            return os;
        }

		std::ostream& operator<<( std::ostream& os, const YesNoNumber& value )
        {
            return toStream( os, value );
        }
    }
}
