// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MeasureRepeat.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MeasureRepeat::MeasureRepeat()
        :myValue()
        ,myAttributes( std::make_shared<MeasureRepeatAttributes>() )
        {}


        MeasureRepeat::MeasureRepeat( const PositiveIntegerOrEmpty& value )
        :myValue( value )
        ,myAttributes( std::make_shared<MeasureRepeatAttributes>() )
        {}


        bool MeasureRepeat::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool MeasureRepeat::hasContents() const
        {
            return true;
        }


        std::ostream& MeasureRepeat::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& MeasureRepeat::streamName( std::ostream& os ) const
        {
            os << "measure-repeat";
            return os;
        }


        std::ostream& MeasureRepeat::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        MeasureRepeatAttributesPtr MeasureRepeat::getAttributes() const
        {
            return myAttributes;
        }


        void MeasureRepeat::setAttributes( const MeasureRepeatAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        PositiveIntegerOrEmpty MeasureRepeat::getValue() const
        {
            return myValue;
        }


        void MeasureRepeat::setValue( const PositiveIntegerOrEmpty& value )
        {
            myValue = value;
        }


        bool MeasureRepeat::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            return isSuccess;
        }

    }
}
