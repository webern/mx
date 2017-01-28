// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/LineWidth.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        LineWidth::LineWidth()
        :myValue()
        ,myAttributes( std::make_shared<LineWidthAttributes>() )
        {}


        LineWidth::LineWidth( const TenthsValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<LineWidthAttributes>() )
        {}


        bool LineWidth::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool LineWidth::hasContents() const
        {
            return true;
        }


        std::ostream& LineWidth::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& LineWidth::streamName( std::ostream& os ) const
        {
            os << "line-width";
            return os;
        }


        std::ostream& LineWidth::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        LineWidthAttributesPtr LineWidth::getAttributes() const
        {
            return myAttributes;
        }


        void LineWidth::setAttributes( const LineWidthAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        TenthsValue LineWidth::getValue() const
        {
            return myValue;
        }


        void LineWidth::setValue( const TenthsValue& value )
        {
            myValue = value;
        }


        bool LineWidth::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            return isSuccess;
        }

    }
}
