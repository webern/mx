// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Articulations.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/ArticulationsChoice.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Articulations::Articulations()
        :myArticulationsChoiceSet()
        {}


        bool Articulations::hasAttributes() const
        {
            return false;
        }


        std::ostream& Articulations::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Articulations::streamName( std::ostream& os ) const
        {
            return os << "articulations";
        }


        bool Articulations::hasContents() const
        {
            return myArticulationsChoiceSet.size() > 0;
        }


        std::ostream& Articulations::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                for ( auto x : myArticulationsChoiceSet )
                {
                    os << std::endl;
                    x->streamContents( os, indentLevel+1, isOneLineOnly );
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


        const ArticulationsChoiceSet& Articulations::getArticulationsChoiceSet() const
        {
            return myArticulationsChoiceSet;
        }


        void Articulations::addArticulationsChoice( const ArticulationsChoicePtr& value )
        {
            if ( value )
            {
                myArticulationsChoiceSet.push_back( value );
            }
        }


        void Articulations::removeArticulationsChoice( const ArticulationsChoiceSetIterConst& value )
        {
            if ( value != myArticulationsChoiceSet.cend() )
            {
                myArticulationsChoiceSet.erase( value );
            }
        }


        void Articulations::clearArticulationsChoiceSet()
        {
            myArticulationsChoiceSet.clear();
        }


        ArticulationsChoicePtr Articulations::getArticulationsChoice( const ArticulationsChoiceSetIterConst& setIterator ) const
        {
            if( setIterator != myArticulationsChoiceSet.cend() )
            {
                return *setIterator;
            }
            return ArticulationsChoicePtr();
        }


        bool Articulations::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                auto item = makeArticulationsChoice();
                isSuccess &= item->fromXElement( message, *it );
                myArticulationsChoiceSet.push_back( item );
            }

            return isSuccess;
        }

    }
}
