// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/XsID.h"
#include "mx/core/StringUtils.h"

namespace mx
{
    namespace core
    {
        XsID::XsID()
        :XsString( "ID" )
        {

        }

        
        XsID::XsID( const StringType& value )
        :XsString( value )
        {
            XsID::setValue( value );
        }
        

        void XsID::setValue( const StringType& value )
        {
            std::string scrubbed = onlyAllow( value );
            if ( scrubbed.length() == 0 )
            {
                scrubbed = "ID";
            }
            else if ( isdigit( scrubbed[0] ) )
            {
                scrubbed = "ID" + scrubbed;
            }
            XsString::setValue( scrubbed );
        }
    }
}
