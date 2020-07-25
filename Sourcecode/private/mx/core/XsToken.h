// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/XsString.h"

namespace mx
{
    namespace core
    {
    	class XsToken : public XsString
        {
        public:
            XsToken();
            XsToken( const StringType& value );
            virtual void setValue( const StringType& value );
        };
    }
}
