// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// self
#include "mx/core/FontSize.h"

// std
#include <sstream>
#include <type_traits>

namespace mx
{
    namespace core
    {
        template<class> inline constexpr bool always_false_v = false;

        FontSize::FontSize()
        : myValue{ Decimal{} }
        {

        }

        FontSize::FontSize( Decimal value )
        : myValue{ std::move( value ) }
        {

        }

        FontSize::FontSize( CssFontSize value )
        : myValue{ value }
        {

        }
        
        FontSize::FontSize( const std::string& value )
        : FontSize{}
        {
            parse( value );
        }

        bool FontSize::getIsDecimal() const
        {
            return myValue.index() == 0;
        }

        bool FontSize::getIsCssFontSize() const
        {
            return myValue.index() == 1;
        }

        void FontSize::setDecimal( Decimal value )
        {
            myValue.emplace<Decimal>( value );
        }

        void FontSize::setCssFontSize( CssFontSize value )
        {
            myValue.emplace<CssFontSize>( value );
        }

        Decimal FontSize::getValueDecimal() const
        {
            auto result = Decimal{};
            std::visit([&](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr( std::is_same_v<T, Decimal> )
                    result = arg;
                else if constexpr( std::is_same_v<T, CssFontSize> )
                    result = Decimal{};
                else
                    static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }, myValue);
            return result;
        }

        CssFontSize FontSize::getValueCssFontSize() const
        {
            auto result = CssFontSize::xxSmall;
            std::visit([&](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr( std::is_same_v<T, Decimal> )
                    result = CssFontSize::xxSmall;
                else if constexpr( std::is_same_v<T, CssFontSize> )
                    result = arg;
                else
                    static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }, myValue);
            return result;
        }

        bool FontSize::parse( const std::string& value )
        {
            auto decimal = Decimal{};
            if( decimal.parse( value ) )
            {
                setDecimal( decimal );
                return true;
            }
            const auto cssFontSize = tryParseCssFontSize( value );
            if( cssFontSize )
            {
                setCssFontSize( *cssFontSize );
                return true;
            }
            return false;
        }

        std::string toString( const FontSize& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }

		std::ostream& toStream( std::ostream& os, const FontSize& value )
        {
            if( value.getIsDecimal() )
            {
                toStream( os, value.getValueDecimal() );
            }
            if( value.getIsCssFontSize() )
            {
                toStream( os, value.getValueCssFontSize() );
            }
            return os;
        }

		std::ostream& operator<<( std::ostream& os, const FontSize& value )
        {
            return toStream( os, value );
        }
    }
}
