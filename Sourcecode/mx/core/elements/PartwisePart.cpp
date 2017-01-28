// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PartwisePart::PartwisePart()
        :myAttributes( std::make_shared<PartAttributes>() )
        ,myPartwiseMeasureSet()
        {
            myPartwiseMeasureSet.push_back( makePartwiseMeasure() );
        }


        bool PartwisePart::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& PartwisePart::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& PartwisePart::streamName( std::ostream& os ) const
        {
            os << "part";
            return os;
        }


        bool PartwisePart::hasContents() const
        {
            return true;
        }


        std::ostream& PartwisePart::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                for ( auto x : myPartwiseMeasureSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                os << std::endl;
                isOneLineOnly = false;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        PartAttributesPtr PartwisePart::getAttributes() const
        {
            return myAttributes;
        }


        void PartwisePart::setAttributes( const PartAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        const PartwiseMeasureSet& PartwisePart::getPartwiseMeasureSet() const
        {
            return myPartwiseMeasureSet;
        }


        void PartwisePart::addPartwiseMeasure( const PartwiseMeasurePtr& value )
        {
            if ( value )
            {
                myPartwiseMeasureSet.push_back( value );
            }
        }


        void PartwisePart::removePartwiseMeasure( const PartwiseMeasureSetIterConst& value )
        {
            if ( value != myPartwiseMeasureSet.cend() )
            {
                if ( myPartwiseMeasureSet.size() > 1 )
                {
                    myPartwiseMeasureSet.erase( value );
                }
            }
        }


        void PartwisePart::clearPartwiseMeasureSet()
        {
            myPartwiseMeasureSet.clear();
            myPartwiseMeasureSet.push_back( makePartwiseMeasure() );
        }


        PartwiseMeasurePtr PartwisePart::getPartwiseMeasure( const PartwiseMeasureSetIterConst& setIterator ) const
        {
            if( setIterator != myPartwiseMeasureSet.cend() )
            {
                return *setIterator;
            }
            return PartwiseMeasurePtr();
        }

        bool PartwisePart::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isFirstAdded = false;
            
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                if( it->getName() != "measure" )
                {
                    message << "PartwisePart: encountered an unexpected element '" << it->getName() << "'" << std::endl;
                    isSuccess = false;
                }
                else
                {
                    auto measure = makePartwiseMeasure();
                    isSuccess &= measure->fromXElement( message, *it );
                    if( !isFirstAdded && myPartwiseMeasureSet.size() == 1 )
                    {
                        *myPartwiseMeasureSet.begin() = measure;
                        isFirstAdded = true;
                    }
                    else
                    {
                        myPartwiseMeasureSet.push_back( measure );
                        isFirstAdded = true;
                    }
                }
            }
            
            MX_RETURN_IS_SUCCESS;
        }
    }
}
