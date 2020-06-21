// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Fermata.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Fermata::Fermata()
        :myValue( FermataShape::normal )
        ,myAttributes( std::make_shared<FermataAttributes>() )
        {}


        Fermata::Fermata( const FermataShape& value )
        :myValue( value )
        ,myAttributes( std::make_shared<FermataAttributes>() )
        {}


        bool Fermata::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Fermata::hasContents() const
        {
            return true;
        }


        std::ostream& Fermata::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Fermata::streamName( std::ostream& os ) const
        {
            os << "fermata";
            return os;
        }


        std::ostream& Fermata::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        FermataAttributesPtr Fermata::getAttributes() const
        {
            return myAttributes;
        }


        void Fermata::setAttributes( const FermataAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        FermataShape Fermata::getValue() const
        {
            return myValue;
        }


        void Fermata::setValue( const FermataShape& value )
        {
            myValue = value;
        }


        bool Fermata::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseFermataShape( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
