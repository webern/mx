// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// self
#include "mx/core/PositiveIntegerOrEmpty.h"

namespace mx
{
    namespace core
    {
        template<class> inline constexpr bool always_false_v = false;

        PositiveIntegerOrEmpty::PositiveIntegerOrEmpty()
        : myValue{ PositiveInteger{} }
        {

        }

        PositiveIntegerOrEmpty::PositiveIntegerOrEmpty( PositiveInteger value )
        : myValue{ std::move( value ) }
        {

        }
        
        PositiveIntegerOrEmpty::PositiveIntegerOrEmpty( const std::string& value )
        : PositiveIntegerOrEmpty{}
        {
            parse( value );
        }

        bool PositiveIntegerOrEmpty::getIsPositiveInteger() const
        {
            return myValue.index() == 0;
        }

        void PositiveIntegerOrEmpty::setPositiveInteger( PositiveInteger value ) const
        {
            myValue.emplace<PositiveInteger>( value );
        }

        PositiveInteger PositiveIntegerOrEmpty::getValuePositiveInteger() const
        {
            auto result = PositiveInteger{};
            std::visit([&](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr( std::is_same_v<T, PositiveInteger> )
                    result = arg;
                else
                    static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }, myValue);
            return result;
        }

        bool PositiveIntegerOrEmpty::parse( const std::string& value )
        {
            auto positiveInteger = PositiveInteger{};
            if( positiveInteger.parse( value ) )
            {
                setValue( positiveInteger );
                return true;
            }
            return false;
        }

        std::string toString( const PositiveIntegerOrEmpty& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }

		std::ostream& toStream( std::ostream& os, const PositiveIntegerOrEmpty& value )
        {
            if( getIsPositiveInteger() )
            {
                toStream( os, value.getValuePositiveInteger() );
            }
            return os;
        }

		std::ostream& operator<<( std::ostream& os, const PositiveIntegerOrEmpty& value )
        {
            return toStream( os, value );
        }
    }
}