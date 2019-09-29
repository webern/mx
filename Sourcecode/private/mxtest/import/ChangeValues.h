// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "ezxml/XDoc.h"
#include "ezxml/XElement.h"
#include "ezxml/XAttribute.h"
#include "ezxml/XElementIterator.h"
#include "ezxml/XAttributeIterator.h"

#include <string>
#include <map>
#include <set>

namespace mxtest
{
    
    using StringMap = std::map<std::string, std::string>;
    
    // These functions recursively change the XML Document for test cases where
    // either the input is questionable or the Mx library produces an acceptable
    // output that does not precisely match the input  See ExpectedFiles.cpp
    // for the specific applications of these functions to the input test files.
    
    void convertValues( ::ezxml::XDoc& xdoc, const std::string& elementName, const std::string& existingValue, const std::string desiredValue );
    void convertValues( ::ezxml::XElement& xelement, const std::string& elementName, const std::string& existingValue, const std::string desiredValue );
    
    void addChildIfNone( ::ezxml::XDoc& xdoc, const std::string& elementName, const std::string& childNameToAdd, const std::string childValueToAdd );
    void addChildIfNone( ::ezxml::XElement& xelement, const std::string& elementName, const std::string& childNameToAdd, const std::string childValueToAdd );
    
    void addAttributeIfNone( ::ezxml::XDoc& xdoc, const std::string& elementName, const std::string& attributeNameToAdd, const std::string attributeValueToAdd );
    void addAttributeIfNone( ::ezxml::XElement& xelement, const std::string& elementName, const std::string& attributeNameToAdd, const std::string attributeValueToAdd );
    
    void removeAttribute( ::ezxml::XDoc& xdoc, const std::string& elementName, const std::string& attributeNameToRemove );
    void removeAttribute( ::ezxml::XElement& xelement, const std::string& elementName, const std::string& attributeNameToRemove );
    
    void changeAttributeValue( ::ezxml::XDoc& xdoc, const std::string& elementName, const std::string& attributeName, const std::string& undesiredValueToReplace, const std::string& replacementValue );
    void changeAttributeValue( ::ezxml::XElement& xelement, const std::string& elementName, const std::string& attributeName,  const std::string& undesiredValueToReplace, const std::string& replacementValue);
    
    void stripZerosFromDecimalFields( ::ezxml::XDoc& xdoc );
    void stripZerosFromMatchingFields( ::ezxml::XDoc& xdoc, const std::set<std::string>& fields );
    void stripZerosFromMatchingFields( ::ezxml::XElement& xelement, const std::set<std::string>& fields );

    struct InsertChildAfterIfMissingParams
    {
        std::string parentElement;
        std::string insertAfterElement;
        std::string elementToInsert;
        std::string value;
    };
    
    void insertChildAfterIfMissing( const InsertChildAfterIfMissingParams& params, ::ezxml::XElement& xelement );
    
    void removeElement( ::ezxml::XElement& xelement, const std::string& elementName );
    
}
