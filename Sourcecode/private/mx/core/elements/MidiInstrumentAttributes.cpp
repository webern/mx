// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MidiInstrumentAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MidiInstrumentAttributes::MidiInstrumentAttributes()
        :id()
        ,hasId( true )
        {}


        bool MidiInstrumentAttributes::hasValues() const
        {
            return hasId;
        }


        std::ostream& MidiInstrumentAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, id, "id", hasId );
            }
            return os;
        }


        bool MidiInstrumentAttributes::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "MidiInstrumentAttributes";
            bool isSuccess = true;
            bool isIdFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, id, isIdFound, "id" ) ) { continue; }
            }
        
            if( !isIdFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
