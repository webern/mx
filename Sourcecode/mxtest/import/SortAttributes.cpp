// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "SortAttributes.h"

namespace mxtest
{
    void sortAttributes( mx::xml::XDoc& xdoc )
    {
        sortAttributes( *xdoc.getRoot() );
    }
    
    
    void sortAttributes ( mx::xml::XElement& xelement )
    {
        StringMap stringMap;
        
        for( auto it = xelement.attributesBegin(); it != xelement.attributesEnd(); ++ it )
        {
            stringMap[it->getName()] = it->getValue();
        }
        
        while( xelement.attributesBegin() != xelement.attributesEnd() )
        {
            deleteFirstAttribute( xelement );
        }
        
        for( auto mapIter = stringMap.cbegin(); mapIter != stringMap.cend(); ++mapIter )
        {
            xelement.appendAttribute( mapIter->first )->setValue( mapIter->second );
        }
        
        if( xelement.getType() == mx::xml::XElementType::element )
        {
            for( auto elementIter = xelement.begin(); elementIter != xelement.end(); ++elementIter )
            {
                sortAttributes( *elementIter );
            }
        }
    }
    
    
    void deleteFirstAttribute( mx::xml::XElement& xelement )
    {
        if( xelement.attributesBegin() != xelement.attributesEnd() )
        {
            xelement.removeAttribute( xelement.attributesBegin() );
        }
    }
}
