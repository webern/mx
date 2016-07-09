// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/DegreeValue.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        DegreeValue::DegreeValue()
        :myValue()
        ,myAttributes( std::make_shared<DegreeValueAttributes>() )
        {}


        DegreeValue::DegreeValue( const PositiveInteger& value )
        :myValue( value )
        ,myAttributes( std::make_shared<DegreeValueAttributes>() )
        {}


        bool DegreeValue::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool DegreeValue::hasContents() const
        {
            return true;
        }


        std::ostream& DegreeValue::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& DegreeValue::streamName( std::ostream& os ) const
        {
            os << "degree-value";
            return os;
        }


        std::ostream& DegreeValue::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        DegreeValueAttributesPtr DegreeValue::getAttributes() const
        {
            return myAttributes;
        }


        void DegreeValue::setAttributes( const DegreeValueAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        PositiveInteger DegreeValue::getValue() const
        {
            return myValue;
        }


        void DegreeValue::setValue( const PositiveInteger& value )
        {
            myValue = value;
        }


        bool DegreeValue::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
