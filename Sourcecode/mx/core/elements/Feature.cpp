// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Feature.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Feature::Feature()
        :myValue()
        ,myAttributes( std::make_shared<FeatureAttributes>() )
        {}


        Feature::Feature( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<FeatureAttributes>() )
        {}


        bool Feature::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Feature::hasContents() const
        {
            return true;
        }


        std::ostream& Feature::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Feature::streamName( std::ostream& os ) const
        {
            os << "feature";
            return os;
        }


        std::ostream& Feature::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        FeatureAttributesPtr Feature::getAttributes() const
        {
            return myAttributes;
        }


        void Feature::setAttributes( const FeatureAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Feature::getValue() const
        {
            return myValue;
        }


        void Feature::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Feature::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
