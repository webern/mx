// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/FontSize.h"
#include <sstream>
#include <type_traits>

namespace
{
    template<class> inline constexpr bool always_false_v = false;
}

namespace mx
{
    namespace core
    {
        FontSize::FontSize()
        : myValue{ CssFontSize::medium }
        {}
        
        FontSize::FontSize( const Decimal& value )
        : myValue{ value }
        {}
        
        FontSize::FontSize( const CssFontSize value )
        : myValue{ value }
        {}
        
        FontSize::FontSize( const std::string& value )
        : FontSize{}
        {
            parse( value );
        }

        bool FontSize::getIsCssFontSize() const
        {
            return myValue.index() == 0;
        }

        bool FontSize::getIsNumber() const
        {
            return myValue.index() == 1;
        }

        void FontSize::setValue( const CssFontSize value )
        {
            myValue.emplace<CssFontSize>( value );
        }

        void FontSize::setValue( const Decimal& value )
        {
            myValue.emplace<Decimal>( value );
        }

        CssFontSize FontSize::getValueCssFontSize() const
        {
            auto result = CssFontSize::medium;
            std::visit([&](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, CssFontSize>)
                    result = arg;
                else if constexpr (std::is_same_v<T, Decimal>)
                    result = CssFontSize::medium;
                else
                    static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }, myValue);
            return result;
        }

        Decimal FontSize::getValueNumber() const
        {
            auto result = Decimal{};
            std::visit([&](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, CssFontSize>)
                    result = Decimal{};
                else if constexpr (std::is_same_v<T, Decimal>)
                    result = arg;
                else
                    static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }, myValue);
            return result;
        }
  
        bool FontSize::parse( const std::string& value )
        {
            const auto cssFontSize = tryParseCssFontSize( value );
            if( cssFontSize )
            {
                setValue( *cssFontSize );
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

        std::string toString( const FontSize& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }

		std::ostream& toStream( std::ostream& os, const FontSize& value )
        {
            if ( value.getIsNumber() )
            {
                toStream( os, value.getValueNumber() );
            }
            else
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
