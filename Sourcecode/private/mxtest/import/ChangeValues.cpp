// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "ChangeValues.h"
#include "DecimalFields.h"

namespace mxtest
{
    void convertValues( ::ezxml::XDoc& xdoc, const std::string& elementName, const std::string& existingValue, const std::string desiredValue )
    {
        convertValues( *xdoc.getRoot(), elementName, existingValue, desiredValue );
    }
    
    
    void convertValues( ::ezxml::XElement& xelement, const std::string& elementName, const std::string& existingValue, const std::string desiredValue )
    {
        if( xelement.getType() == ::ezxml::XElementType::element )
        {
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                convertValues( *it, elementName, existingValue, desiredValue );
            }
        }
        else if ( xelement.getType() == ::ezxml::XElementType::text || xelement.getType() == ::ezxml::XElementType::empty )
        {
            if( xelement.getName() == elementName && xelement.getValue() == existingValue )
            {
                xelement.setValue( desiredValue );
            }
        }
    }
    
    
    void addChildIfNone( ::ezxml::XDoc& xdoc, const std::string& elementName, const std::string& childNameToAdd, const std::string childValueToAdd )
    {
        addChildIfNone( *xdoc.getRoot(), elementName, childNameToAdd, childValueToAdd );
    }
    
    
    void addChildIfNone( ::ezxml::XElement& xelement, const std::string& elementName, const std::string& childNameToAdd, const std::string childValueToAdd )
    {
        if( xelement.getType() != ::ezxml::XElementType::text && xelement.getName() == elementName )
        {
            if( xelement.begin() == xelement.end() )
            {
                auto x = xelement.appendChild( childNameToAdd );
                if( !childValueToAdd.empty() )
                {
                    x->setValue( childValueToAdd );
                }
            }
        }
        else if ( xelement.getType() == ::ezxml::XElementType::element || xelement.getType() == ::ezxml::XElementType::empty )
        {
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                addChildIfNone( *it, elementName, childNameToAdd, childValueToAdd );
            }
        }
    }
    
    
    void addAttributeIfNone( ::ezxml::XDoc& xdoc, const std::string& elementName, const std::string& attributeNameToAdd, const std::string attributeValueToAdd )
    {
        addAttributeIfNone( *xdoc.getRoot(), elementName, attributeNameToAdd, attributeValueToAdd );
    }
    
    
    void addAttributeIfNone( ::ezxml::XElement& xelement, const std::string& elementName, const std::string& attributeNameToAdd, const std::string attributeValueToAdd )
    {
        
        if( xelement.getName() == elementName )
        {
            bool isAttributeFound = false;
            
            for( auto it = xelement.attributesBegin(); it != xelement.attributesEnd(); ++it )
            {
                if( it->getName() == attributeNameToAdd )
                {
                    isAttributeFound = true;
                    break;
                }
            }
            
            if( !isAttributeFound )
            {
                xelement.appendAttribute( attributeNameToAdd )->setValue( attributeValueToAdd );
            }
        }
        
        if ( xelement.getType() == ::ezxml::XElementType::element )
        {
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                addAttributeIfNone( *it, elementName, attributeNameToAdd, attributeValueToAdd );
            }
        }
    }
    
    
    void removeAttribute( ::ezxml::XDoc& xdoc, const std::string& elementName, const std::string& attributeNameToRemove )
    {
        removeAttribute( *xdoc.getRoot(), elementName, attributeNameToRemove );
    }
    
    
    void removeAttribute( ::ezxml::XElement& xelement, const std::string& elementName, const std::string& attributeNameToRemove )
    {
        if( xelement.getName() == elementName )
        {
            for( auto it = xelement.attributesBegin(); it != xelement.attributesEnd(); ++it )
            {
                if( it->getName() == attributeNameToRemove )
                {
                    xelement.removeAttribute( it );
                    break;
                }
            }
        }
        
        if ( xelement.getType() == ::ezxml::XElementType::element )
        {
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                removeAttribute( *it, elementName, attributeNameToRemove );
            }
        }
    }
    
    
    void changeAttributeValue( ::ezxml::XDoc& xdoc, const std::string& elementName, const std::string& attributeName, const std::string& undesiredValueToReplace, const std::string& replacementValue )
    {
        changeAttributeValue( *xdoc.getRoot(), elementName, attributeName, undesiredValueToReplace, replacementValue );
    }
    
    
    void changeAttributeValue( ::ezxml::XElement& xelement, const std::string& elementName, const std::string& attributeName,  const std::string& undesiredValueToReplace, const std::string& replacementValue)
    {
        if( xelement.getName() == elementName )
        {
            for( auto it = xelement.attributesBegin(); it != xelement.attributesEnd(); ++it )
            {
                if( it->getName() == attributeName )
                {
                    if( it->getValue() == undesiredValueToReplace )
                    {
                        it->setValue( replacementValue );
                        break;
                    }
                }
            }
        }
        
        if( xelement.getType() == ::ezxml::XElementType::element )
        {
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                changeAttributeValue( *it, elementName, attributeName, undesiredValueToReplace, replacementValue );
            }
        }
    }


    void stripZerosFromDecimalFields( ::ezxml::XDoc& xdoc )
    {
        stripZerosFromMatchingFields( xdoc, decimalFields );
    }


    void stripZerosFromMatchingFields( ::ezxml::XDoc& xdoc, const std::set<std::string>& fields )
    {
        stripZerosFromMatchingFields( *xdoc.getRoot(), fields );
    }


    void stripZerosFromMatchingFields( ::ezxml::XElement& xelement, const std::set<std::string>& fields )
    {
        if ( xelement.getType() == ::ezxml::XElementType::text )
        {
            auto foundIter = fields.find( xelement.getName() );
            if ( foundIter != fields.end() )
            {
                stripZeros( xelement );
            }
        }

        for ( auto a = xelement.attributesBegin(); a != xelement.attributesEnd(); ++a )
        {
            auto foundIter = fields.find( a->getName() );
            if ( foundIter != fields.end() )
            {
                stripZeros( *a );
            }
        }

        if ( xelement.getType() == ::ezxml::XElementType::element )
        {
            for ( auto e = xelement.begin(); e != xelement.end(); ++e )
            {
                //recursive
                stripZerosFromMatchingFields( *e, fields );
            }
        }

    }
    
    
    void insertChildAfterIfMissing( const InsertChildAfterIfMissingParams& params, ::ezxml::XElement& xelement )
    {
        
        if( xelement.getType() == ::ezxml::XElementType::null )
        {
            return;
        }
        
        if( xelement.getType() != ::ezxml::XElementType::text && xelement.getName() == params.parentElement )
        {

            bool isTriggerFound = false;
            bool isFollowerFound = false;
            auto foundIter = xelement.end();
            
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                if( it->getName() == params.insertAfterElement )
                {
                    isTriggerFound = true;
                    foundIter = it;
                }
                
                if( it->getName() == params.elementToInsert )
                {
                    isFollowerFound = true;
                    break;
                }
            }
            
            if( isTriggerFound && !isFollowerFound )
            {
                auto newElement = foundIter->insertSiblingAfter( params.elementToInsert.c_str() );
                newElement->setValue( params.value );
            }
            
        }
        else if ( xelement.getType() == ::ezxml::XElementType::element || xelement.getType() == ::ezxml::XElementType::empty )
        {
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                insertChildAfterIfMissing( params, *it );
            }
        }
    }
    
    void removeElement( ::ezxml::XElement& xelement, const std::string& elementName )
    {
        while( xelement.removeChild( elementName ) ) {}
        
        for( auto it = xelement.begin(); it != xelement.end(); ++it )
        {
            if( it->getType() == ::ezxml::XElementType::element )
            {
                removeElement( *it, elementName );
            }
        }
    }
}
