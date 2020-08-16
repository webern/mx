// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// self
#include "mx/core/NumberOrNormal.h"

// std
#include <sstream>
#include <type_traits>

namespace mx
{
    namespace core
    {
        template<class> inline constexpr bool always_false_v = false;

        NumberOrNormal::NumberOrNormal()
        : myValue{ Decimal{} }
        {

        }

        NumberOrNormal::NumberOrNormal( Decimal value )
        : myValue{ std::move( value ) }
        {

        }
        
        NumberOrNormal::NumberOrNormal( const std::string& value )
        : NumberOrNormal{}
        {
            parse( value );
        }

        bool NumberOrNormal::getIsDecimal() const
        {
            return myValue.index() == 0;
        }

        void NumberOrNormal::setDecimal( Decimal value ) const
        {
            myValue.emplace<Decimal>( value );
        }

        Decimal NumberOrNormal::getValueDecimal() const
        {
            auto result = Decimal{};
            std::visit([&](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr( std::is_same_v<T, Decimal> )
                    result = arg;
                else
                    static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }, myValue);
            return result;
        }

        bool NumberOrNormal::parse( const std::string& value )
        {
            auto decimal = Decimal{};
            if( decimal.parse( value ) )
            {
                setValue( decimal );
                return true;
            }
            return false;
        }

        std::string toString( const NumberOrNormal& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }

		std::ostream& toStream( std::ostream& os, const NumberOrNormal& value )
        {
            if( getIsDecimal() )
            {
                toStream( os, value.getValueDecimal() );
            }
            return os;
        }

		std::ostream& operator<<( std::ostream& os, const NumberOrNormal& value )
        {
            return toStream( os, value );
        }
    }
}