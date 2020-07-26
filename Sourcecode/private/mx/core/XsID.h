// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/XsString.h"

namespace mx
{
    namespace core
    {
        class XsID : public XsString
        {
        public:
            XsID();
            XsID( const StringType& value );
            virtual void setValue( const StringType& value );
        };
    }
}
