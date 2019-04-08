// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MiscellaneousField.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MiscellaneousField::MiscellaneousField()
        :myValue()
        ,myAttributes( std::make_shared<MiscellaneousFieldAttributes>() )
        {}


        MiscellaneousField::MiscellaneousField( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<MiscellaneousFieldAttributes>() )
        {}


        bool MiscellaneousField::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool MiscellaneousField::hasContents() const
        {
            return true;
        }


        std::ostream& MiscellaneousField::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& MiscellaneousField::streamName( std::ostream& os ) const
        {
            os << "miscellaneous-field";
            return os;
        }


        std::ostream& MiscellaneousField::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        MiscellaneousFieldAttributesPtr MiscellaneousField::getAttributes() const
        {
            return myAttributes;
        }


        void MiscellaneousField::setAttributes( const MiscellaneousFieldAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString MiscellaneousField::getValue() const
        {
            return myValue;
        }


        void MiscellaneousField::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool MiscellaneousField::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
