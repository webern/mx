// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MeasureNumbering.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MeasureNumbering::MeasureNumbering()
        :myValue( MeasureNumberingValue::none )
        ,myAttributes( std::make_shared<MeasureNumberingAttributes>() )
        {}


        MeasureNumbering::MeasureNumbering( const MeasureNumberingValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<MeasureNumberingAttributes>() )
        {}


        bool MeasureNumbering::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool MeasureNumbering::hasContents() const
        {
            return true;
        }


        std::ostream& MeasureNumbering::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& MeasureNumbering::streamName( std::ostream& os ) const
        {
            os << "measure-numbering";
            return os;
        }


        std::ostream& MeasureNumbering::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        MeasureNumberingAttributesPtr MeasureNumbering::getAttributes() const
        {
            return myAttributes;
        }


        void MeasureNumbering::setAttributes( const MeasureNumberingAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        MeasureNumberingValue MeasureNumbering::getValue() const
        {
            return myValue;
        }


        void MeasureNumbering::setValue( const MeasureNumberingValue& value )
        {
            myValue = value;
        }


        bool MeasureNumbering::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseMeasureNumberingValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
