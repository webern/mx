// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// self
#include "mx/core/FontSize.h"

namespace mx
{
    namespace core
    {
        template<class> inline constexpr bool always_false_v = false;

        classname::classname()
        : myValue{ default_value }
        {

        }
        
        // classname::classname( const Decimal& value )
        // : myValue{ value }
        // {}
        
        // classname::classname( const SomeEnumType value )
        // : myValue{ value }
        // {}
variants_ctor_def        

        classname::classname( const std::string& value )
        : classname{}
        {
            parse( value );
        }

        // bool classname::getIsSomeEnumType() const
        // {
        //     return myValue.index() == 0;
        // }

        // bool classname::getIsNumber() const
        // {
        //     return myValue.index() == 1;
        // }
variants_get_is_def

        // void classname::setValue( const SomeEnumType value )
        // {
        //     myValue.emplace<SomeEnumType>( value );
        // }

        // void classname::setValue( const Decimal& value )
        // {
        //     myValue.emplace<Decimal>( value );
        // }
variants_set_def

        // SomeEnumType classname::getValueSomeEnumType() const
        // {
        //     auto result = SomeEnumType::medium;
        //     std::visit([&](auto&& arg) {
        //         using T = std::decay_t<decltype(arg)>;
        //         if constexpr (std::is_same_v<T, SomeEnumType>)
        //             result = arg;
        //         else if constexpr (std::is_same_v<T, Decimal>)
        //             result = SomeEnumType::medium;
        //         else
        //             static_assert(always_false_v<T>, "non-exhaustive visitor!");
        //     }, myValue);
        //     return result;
        // }

        // Decimal classname::getValueNumber() const
        // {
        //     auto result = Decimal{};
        //     std::visit([&](auto&& arg) {
        //         using T = std::decay_t<decltype(arg)>;
        //         if constexpr (std::is_same_v<T, SomeEnumType>)
        //             result = Decimal{};
        //         else if constexpr (std::is_same_v<T, Decimal>)
        //             result = arg;
        //         else
        //             static_assert(always_false_v<T>, "non-exhaustive visitor!");
        //     }, myValue);
        //     return result;
        // }
variants_get_def

        bool classname::parse( const std::string& value )
        {
            // const auto SomeEnumType = tryParseSomeEnumType( value );
            // if( SomeEnumType )
            // {
            //     setValue( *SomeEnumType );
            //     return true;
            // }
            // auto decimal = Decimal{};
            // if( decimal.parse( value ) )
            // {
            //     setValue( decimal );
            //     return true;
            // }
            // return false;
parse_def
        }

        std::string toString( const classname& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }

		std::ostream& toStream( std::ostream& os, const classname& value )
        {
            if ( value.getIsNumber() )
            {
                toStream( os, value.getValueNumber() );
            }
            else
            {
                toStream( os, value.getValueSomeEnumType() );
            }
            return os;
        }

		std::ostream& operator<<( std::ostream& os, const classname& value )
        {
            return toStream( os, value );
        }
    }
}