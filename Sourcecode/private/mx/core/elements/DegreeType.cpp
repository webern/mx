// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/DegreeType.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        DegreeType::DegreeType()
        :myValue( DegreeTypeValue::add )
        ,myAttributes( std::make_shared<DegreeTypeAttributes>() )
        {}


        DegreeType::DegreeType( const DegreeTypeValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<DegreeTypeAttributes>() )
        {}


        bool DegreeType::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool DegreeType::hasContents() const
        {
            return true;
        }


        std::ostream& DegreeType::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& DegreeType::streamName( std::ostream& os ) const
        {
            os << "degree-type";
            return os;
        }


        std::ostream& DegreeType::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        DegreeTypeAttributesPtr DegreeType::getAttributes() const
        {
            return myAttributes;
        }


        void DegreeType::setAttributes( const DegreeTypeAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        DegreeTypeValue DegreeType::getValue() const
        {
            return myValue;
        }


        void DegreeType::setValue( const DegreeTypeValue& value )
        {
            myValue = value;
        }


        bool DegreeType::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseDegreeTypeValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
