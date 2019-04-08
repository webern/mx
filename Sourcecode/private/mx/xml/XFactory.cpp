// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/xml/XFactory.h"
#include "mx/xml/PugiDoc.h"
#include "mx/xml/PugiElement.h"

namespace mx
{
    namespace xml
    {
        XDocPtr XFactory::makeXDoc()
        {
            return XDocPtr{ new PugiDoc{} };
        }
        
        XElementPtr XFactory::makeXElement()
        {
            return XElementPtr{ new PugiElement{} };
        }
    }
}
