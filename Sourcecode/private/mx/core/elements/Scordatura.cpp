// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Scordatura.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Accord.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Scordatura::Scordatura()
        :myAccordSet()
        {
            myAccordSet.push_back( makeAccord() );
        }


        bool Scordatura::hasAttributes() const
        {
            return false;
        }


        std::ostream& Scordatura::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Scordatura::streamName( std::ostream& os ) const
        {
            os << "scordatura";
            return os;
        }


        bool Scordatura::hasContents() const
        {
            return true;
        }


        std::ostream& Scordatura::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            for ( auto x : myAccordSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            return os;
        }


        const AccordSet& Scordatura::getAccordSet() const
        {
            return myAccordSet;
        }


        void Scordatura::removeAccord( const AccordSetIterConst& value )
        {
            if ( value != myAccordSet.cend() )
            {
                if ( myAccordSet.size() > 1 )
                {
                    myAccordSet.erase( value );
                }
            }
        }


        void Scordatura::addAccord( const AccordPtr& value )
        {
            if ( value )
            {
                myAccordSet.push_back( value );
            }
        }


        void Scordatura::clearAccordSet()
        {
            myAccordSet.clear();
            myAccordSet.push_back( makeAccord() );
        }


        AccordPtr Scordatura::getAccord( const AccordSetIterConst& setIterator ) const
        {
            if( setIterator != myAccordSet.cend() )
            {
                return *setIterator;
            }
            return AccordPtr();
        }


        bool Scordatura::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                importElementSet( message, it, endIter, isSuccess, "accord", myAccordSet );
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
