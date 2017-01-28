// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Grace.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Grace::Grace()
        :ElementInterface()
        ,myAttributes( std::make_shared<GraceAttributes>() )
        {}


        bool Grace::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Grace::hasContents() const  { return false; }
        std::ostream& Grace::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Grace::streamName( std::ostream& os ) const  { os << "grace"; return os; }
        std::ostream& Grace::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        GraceAttributesPtr Grace::getAttributes() const
        {
            return myAttributes;
        }


        void Grace::setAttributes( const GraceAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Grace::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
