// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/xml/XDoc.h"
#include "mx/xml/XElement.h"
#include "mx/xml/XAttribute.h"
#include "mx/xml/XElementIterator.h"
#include "mx/xml/XAttributeIterator.h"

#include <string>
#include <map>
#include <set>

namespace MxTest
{
    
    using StringMap = std::map<std::string, std::string>;
    
    // These functions recursively change the XML Document for test cases where
    // either the input is questionable or the Mx library produces an acceptable
    // output that does not precisely match the input  See ExpectedFiles.cpp
    // for the specific applications of these functions to the input test files.
    
    void convertValues( mx::xml::XDoc& xdoc, const std::string& elementName, const std::string& existingValue, const std::string desiredValue );
    void convertValues( mx::xml::XElement& xelement, const std::string& elementName, const std::string& existingValue, const std::string desiredValue );
    
    void addChildIfNone( mx::xml::XDoc& xdoc, const std::string& elementName, const std::string& childNameToAdd, const std::string childValueToAdd );
    void addChildIfNone( mx::xml::XElement& xelement, const std::string& elementName, const std::string& childNameToAdd, const std::string childValueToAdd );
    
    void addAttributeIfNone( mx::xml::XDoc& xdoc, const std::string& elementName, const std::string& attributeNameToAdd, const std::string attributeValueToAdd );
    void addAttributeIfNone( mx::xml::XElement& xelement, const std::string& elementName, const std::string& attributeNameToAdd, const std::string attributeValueToAdd );
    
    void removeAttribute( mx::xml::XDoc& xdoc, const std::string& elementName, const std::string& attributeNameToRemove );
    void removeAttribute( mx::xml::XElement& xelement, const std::string& elementName, const std::string& attributeNameToRemove );
    
    void changeAttributeValue( mx::xml::XDoc& xdoc, const std::string& elementName, const std::string& attributeName, const std::string& undesiredValueToReplace, const std::string& replacementValue );
    void changeAttributeValue( mx::xml::XElement& xelement, const std::string& elementName, const std::string& attributeName,  const std::string& undesiredValueToReplace, const std::string& replacementValue);
    
    void stripZerosFromDecimalFields( mx::xml::XDoc& xdoc );
    void stripZerosFromMatchingFields( mx::xml::XDoc& xdoc, const std::set<std::string>& fields );
    void stripZerosFromMatchingFields( mx::xml::XElement& xelement, const std::set<std::string>& fields );

    struct InsertChildAfterIfMissingParams
    {
        std::string parentElement;
        std::string insertAfterElement;
        std::string elementToInsert;
        std::string value;
    };
    
    void insertChildAfterIfMissing( const InsertChildAfterIfMissingParams& params, mx::xml::XElement& xelement );
    
    void removeElement( mx::xml::XElement& xelement, const std::string& elementName );
    
}