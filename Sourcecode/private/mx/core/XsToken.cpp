// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/XsToken.h"
#include "mx/core/StringUtils.h"

namespace mx
{
    namespace core
    {
        XsToken::XsToken()
        :XsString()
        {

        }
        
        XsToken::XsToken( const StringType& value )
        :XsString( value )
        {
            XsToken::setValue( value );
        }


        void XsToken::setValue( const StringType& value )
        {
            XsString::setValue( reduce( value ) );
        }
    }
}
