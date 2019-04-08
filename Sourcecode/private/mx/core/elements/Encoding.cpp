// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Encoding.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/EncodingChoice.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Encoding::Encoding()
        :myEncodingChoiceSet()
        {}
        
        
        bool Encoding::hasAttributes() const
        {
            return false;
        }
        
        
        std::ostream& Encoding::streamAttributes( std::ostream& os ) const
        {
            return os;
        }
        
        
        std::ostream& Encoding::streamName( std::ostream& os ) const
        {
            os << "encoding";
            return os;
        }
        
        
        bool Encoding::hasContents() const
        {
            return myEncodingChoiceSet.size() > 0;
        }
        
        
        std::ostream& Encoding::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            bool isFirst = true;
            for ( auto x : myEncodingChoiceSet )
            {
                if( !isFirst )
                {
                    os << std::endl;
                    isFirst = false;
                }
                x->streamContents( os, indentLevel, isOneLineOnly );
            }
            if ( hasContents() )
            {
                os << std::endl;
            }
            isOneLineOnly = !hasContents();
            return os;
        }
        
        
        const EncodingChoiceSet& Encoding::getEncodingChoiceSet() const
        {
            return myEncodingChoiceSet;
        }
        
        
        void Encoding::removeEncodingChoice( const EncodingChoiceSetIterConst& value )
        {
            if ( value != myEncodingChoiceSet.cend() )
            {
                myEncodingChoiceSet.erase( value );
            }
        }
        
        
        void Encoding::addEncodingChoice( const EncodingChoicePtr& value )
        {
            if ( value )
            {
                myEncodingChoiceSet.push_back( value );
            }
        }
        
        
        void Encoding::clearEncodingChoiceSet()
        {
            myEncodingChoiceSet.clear();
        }
        
        
        bool Encoding::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            
            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                auto item = makeEncodingChoice();
                isSuccess &= item->fromXElement( message, *it );
                myEncodingChoiceSet.push_back( item );
            }

            MX_RETURN_IS_SUCCESS;
        }
        
    }
}
