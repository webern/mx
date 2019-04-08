// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/FigureNumber.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        FigureNumber::FigureNumber()
        :myValue()
        ,myAttributes( std::make_shared<FigureNumberAttributes>() )
        {}


        FigureNumber::FigureNumber( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<FigureNumberAttributes>() )
        {}


        bool FigureNumber::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool FigureNumber::hasContents() const
        {
            return true;
        }


        std::ostream& FigureNumber::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& FigureNumber::streamName( std::ostream& os ) const
        {
            os << "figure-number";
            return os;
        }


        std::ostream& FigureNumber::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        FigureNumberAttributesPtr FigureNumber::getAttributes() const
        {
            return myAttributes;
        }


        void FigureNumber::setAttributes( const FigureNumberAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString FigureNumber::getValue() const
        {
            return myValue;
        }


        void FigureNumber::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool FigureNumber::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            return isSuccess;
        }

    }
}
