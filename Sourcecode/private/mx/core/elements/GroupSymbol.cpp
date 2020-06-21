// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/GroupSymbol.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        GroupSymbol::GroupSymbol()
        :myValue( GroupSymbolValue::none )
        ,myAttributes( std::make_shared<GroupSymbolAttributes>() )
        {}


        GroupSymbol::GroupSymbol( const GroupSymbolValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<GroupSymbolAttributes>() )
        {}


        bool GroupSymbol::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool GroupSymbol::hasContents() const
        {
            return true;
        }


        std::ostream& GroupSymbol::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& GroupSymbol::streamName( std::ostream& os ) const
        {
            os << "group-symbol";
            return os;
        }


        std::ostream& GroupSymbol::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        GroupSymbolAttributesPtr GroupSymbol::getAttributes() const
        {
            return myAttributes;
        }


        void GroupSymbol::setAttributes( const GroupSymbolAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        GroupSymbolValue GroupSymbol::getValue() const
        {
            return myValue;
        }


        void GroupSymbol::setValue( const GroupSymbolValue& value )
        {
            myValue = value;
        }


        bool GroupSymbol::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseGroupSymbolValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
