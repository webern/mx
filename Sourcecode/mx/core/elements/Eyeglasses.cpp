// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Eyeglasses.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Eyeglasses::Eyeglasses()
        :ElementInterface()
        ,myAttributes( std::make_shared<EmptyPrintObjectStyleAlignAttributes>() )
        {}


        bool Eyeglasses::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Eyeglasses::hasContents() const  { return false; }
        std::ostream& Eyeglasses::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Eyeglasses::streamName( std::ostream& os ) const  { os << "eyeglasses"; return os; }
        std::ostream& Eyeglasses::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        EmptyPrintObjectStyleAlignAttributesPtr Eyeglasses::getAttributes() const
        {
            return myAttributes;
        }


        void Eyeglasses::setAttributes( const EmptyPrintObjectStyleAlignAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Eyeglasses::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
