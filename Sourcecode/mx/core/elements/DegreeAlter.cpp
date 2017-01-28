// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/DegreeAlter.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        DegreeAlter::DegreeAlter()
        :myValue()
        ,myAttributes( std::make_shared<DegreeAlterAttributes>() )
        {}


        DegreeAlter::DegreeAlter( const Semitones& value )
        :myValue( value )
        ,myAttributes( std::make_shared<DegreeAlterAttributes>() )
        {}


        bool DegreeAlter::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool DegreeAlter::hasContents() const
        {
            return true;
        }


        std::ostream& DegreeAlter::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& DegreeAlter::streamName( std::ostream& os ) const
        {
            os << "degree-alter";
            return os;
        }


        std::ostream& DegreeAlter::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        DegreeAlterAttributesPtr DegreeAlter::getAttributes() const
        {
            return myAttributes;
        }


        void DegreeAlter::setAttributes( const DegreeAlterAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        Semitones DegreeAlter::getValue() const
        {
            return myValue;
        }


        void DegreeAlter::setValue( const Semitones& value )
        {
            myValue = value;
        }


        bool DegreeAlter::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
