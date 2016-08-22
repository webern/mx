// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/DampAll.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        DampAll::DampAll()
        :ElementInterface()
        ,myAttributes( std::make_shared<EmptyPrintObjectStyleAlignAttributes>() )
        {}


        bool DampAll::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool DampAll::hasContents() const  { return false; }
        std::ostream& DampAll::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& DampAll::streamName( std::ostream& os ) const  { os << "damp-all"; return os; }
        std::ostream& DampAll::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        EmptyPrintObjectStyleAlignAttributesPtr DampAll::getAttributes() const
        {
            return myAttributes;
        }


        void DampAll::setAttributes( const EmptyPrintObjectStyleAlignAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool DampAll::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
