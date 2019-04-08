// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/xml/XDoc.h"
#include <memory>
#include <stddef.h>
#include <stdint.h>

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
