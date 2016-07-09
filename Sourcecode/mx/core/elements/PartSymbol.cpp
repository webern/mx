// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/PartSymbol.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PartSymbol::PartSymbol()
        :myValue( GroupSymbolValue::none )
        ,myAttributes( std::make_shared<PartSymbolAttributes>() )
        {}


        PartSymbol::PartSymbol( const GroupSymbolValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<PartSymbolAttributes>() )
        {}


        bool PartSymbol::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool PartSymbol::hasContents() const
        {
            return true;
        }


        std::ostream& PartSymbol::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& PartSymbol::streamName( std::ostream& os ) const
        {
            os << "part-symbol";
            return os;
        }


        std::ostream& PartSymbol::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PartSymbolAttributesPtr PartSymbol::getAttributes() const
        {
            return myAttributes;
        }


        void PartSymbol::setAttributes( const PartSymbolAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        GroupSymbolValue PartSymbol::getValue() const
        {
            return myValue;
        }


        void PartSymbol::setValue( const GroupSymbolValue& value )
        {
            myValue = value;
        }


        bool PartSymbol::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseGroupSymbolValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
