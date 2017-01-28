// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Image.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Image::Image()
        :ElementInterface()
        ,myAttributes( std::make_shared<ImageAttributes>() )
        {}


        bool Image::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Image::hasContents() const  { return false; }
        std::ostream& Image::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Image::streamName( std::ostream& os ) const  { os << "image"; return os; }
        std::ostream& Image::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        ImageAttributesPtr Image::getAttributes() const
        {
            return myAttributes;
        }


        void Image::setAttributes( const ImageAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Image::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
