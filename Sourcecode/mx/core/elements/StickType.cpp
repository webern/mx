// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/StickType.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StickType::StickType()
        :myValue( StickTypeEnum::yarn )
        {}


        StickType::StickType( const StickTypeEnum& value )
        :myValue( value )
        {}


        bool StickType::hasAttributes() const
        {
            return false;
        }


        bool StickType::hasContents() const
        {
            return true;
        }


        std::ostream& StickType::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& StickType::streamName( std::ostream& os ) const
        {
            os << "stick-type";
            return os;
        }


        std::ostream& StickType::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        StickTypeEnum StickType::getValue() const
        {
            return myValue;
        }


        void StickType::setValue( const StickTypeEnum& value )
        {
            myValue = value;
        }


        bool StickType::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseStickTypeEnum( xelement.getValue() );
            return true;
        }

    }
}
