// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/xml/XDoc.h"
#include <memory>

namespace mx
{
    namespace xml
    {
        class XFactory
        {
        public:
            static XDocPtr makeXDoc();
            static XElementPtr makeXElement();
        };
    }
}
