// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/xml/XFactory.h"
#include "mx/xml/PugiDoc.h"
#include "mx/xml/PugiElement.h"
#include <memory>

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