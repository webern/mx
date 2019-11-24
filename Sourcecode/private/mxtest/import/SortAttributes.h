// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "ezxml/XDoc.h"
#include "ezxml/XElement.h"
#include "ezxml/XAttribute.h"
#include "ezxml/XElementIterator.h"
#include "ezxml/XAttributeIterator.h"

#include <map>
#include <string>

namespace mxtest
{
    
    using StringMap = std::map<std::string, std::string>;
    
    // unfortunately xml does not care about the order of attributes
    // so both the expected and actual files will need to have their
    // attributes sorted into alphabetical order so that they match.
    
    void sortAttributes( ::ezxml::XDoc& xdoc );
    void sortAttributes ( ::ezxml::XElement& xelement );
    void deleteFirstAttribute( ::ezxml::XElement& xelement );
}
