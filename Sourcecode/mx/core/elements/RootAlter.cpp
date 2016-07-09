// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/RootAlter.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        RootAlter::RootAlter()
        :myValue()
        ,myAttributes( std::make_shared<RootAlterAttributes>() )
        {}


        RootAlter::RootAlter( const Semitones& value )
        :myValue( value )
        ,myAttributes( std::make_shared<RootAlterAttributes>() )
        {}


        bool RootAlter::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool RootAlter::hasContents() const
        {
            return true;
        }


        std::ostream& RootAlter::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& RootAlter::streamName( std::ostream& os ) const
        {
            os << "root-alter";
            return os;
        }


        std::ostream& RootAlter::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        RootAlterAttributesPtr RootAlter::getAttributes() const
        {
            return myAttributes;
        }


        void RootAlter::setAttributes( const RootAlterAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        Semitones RootAlter::getValue() const
        {
            return myValue;
        }


        void RootAlter::setValue( const Semitones& value )
        {
            myValue = value;
        }


        bool RootAlter::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
