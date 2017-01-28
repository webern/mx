// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MetronomeRelation.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MetronomeRelation::MetronomeRelation()
        :myValue( "equals" )
        {}


        MetronomeRelation::MetronomeRelation( const XsString& value )
        :myValue( value )
        {}


        bool MetronomeRelation::hasAttributes() const
        {
            return false;
        }


        bool MetronomeRelation::hasContents() const
        {
            return true;
        }


        std::ostream& MetronomeRelation::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MetronomeRelation::streamName( std::ostream& os ) const
        {
            os << "metronome-relation";
            return os;
        }


        std::ostream& MetronomeRelation::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString MetronomeRelation::getValue() const
        {
            return myValue;
        }


        void MetronomeRelation::setValue( const XsString& value )
        {
            myValue.setValue( value.getValue() );
        }


        bool MetronomeRelation::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
