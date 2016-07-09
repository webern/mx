// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/BreathMark.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BreathMark::BreathMark()
        :myValue( BreathMarkValue::emptystring )
        ,myAttributes( std::make_shared<BreathMarkAttributes>() )
        {}


        BreathMark::BreathMark( const BreathMarkValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<BreathMarkAttributes>() )
        {}


        bool BreathMark::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool BreathMark::hasContents() const
        {
            return true;
        }


        std::ostream& BreathMark::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& BreathMark::streamName( std::ostream& os ) const
        {
            os << "breath-mark";
            return os;
        }


        std::ostream& BreathMark::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        BreathMarkAttributesPtr BreathMark::getAttributes() const
        {
            return myAttributes;
        }


        void BreathMark::setAttributes( const BreathMarkAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        BreathMarkValue BreathMark::getValue() const
        {
            return myValue;
        }


        void BreathMark::setValue( const BreathMarkValue& value )
        {
            myValue = value;
        }


        bool BreathMark::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseBreathMarkValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
