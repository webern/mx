// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/FirstFret.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        FirstFret::FirstFret()
        :myValue()
        ,myAttributes( std::make_shared<FirstFretAttributes>() )
        {}


        FirstFret::FirstFret( const PositiveInteger& value )
        :myValue( value )
        ,myAttributes( std::make_shared<FirstFretAttributes>() )
        {}


        bool FirstFret::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool FirstFret::hasContents() const
        {
            return true;
        }


        std::ostream& FirstFret::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& FirstFret::streamName( std::ostream& os ) const
        {
            os << "first-fret";
            return os;
        }


        std::ostream& FirstFret::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        FirstFretAttributesPtr FirstFret::getAttributes() const
        {
            return myAttributes;
        }


        void FirstFret::setAttributes( const FirstFretAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        PositiveInteger FirstFret::getValue() const
        {
            return myValue;
        }


        void FirstFret::setValue( const PositiveInteger& value )
        {
            myValue = value;
        }


        bool FirstFret::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
