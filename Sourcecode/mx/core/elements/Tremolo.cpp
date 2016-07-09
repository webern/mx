// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Tremolo.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Tremolo::Tremolo()
        :myValue( 3 )
        ,myAttributes( std::make_shared<TremoloAttributes>() )
        {}


        Tremolo::Tremolo( const TremoloMarks& value )
        :myValue( value )
        ,myAttributes( std::make_shared<TremoloAttributes>() )
        {}


        bool Tremolo::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Tremolo::hasContents() const
        {
            return true;
        }


        std::ostream& Tremolo::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Tremolo::streamName( std::ostream& os ) const
        {
            os << "tremolo";
            return os;
        }


        std::ostream& Tremolo::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TremoloAttributesPtr Tremolo::getAttributes() const
        {
            return myAttributes;
        }


        void Tremolo::setAttributes( const TremoloAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        TremoloMarks Tremolo::getValue() const
        {
            return myValue;
        }


        void Tremolo::setValue( const TremoloMarks& value )
        {
            myValue = value;
        }


        bool Tremolo::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
