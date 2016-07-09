// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Ornaments.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccidentalMark.h"
#include "mx/core/elements/OrnamentsChoice.h"
#include <iostream>
#include <cstdio>

namespace mx
{
    namespace core
    {
        Ornaments::Ornaments()
        :myOrnamentsChoiceSet()
        ,myAccidentalMarkSet()
        {}


        bool Ornaments::hasAttributes() const
        {
            return false;
        }


        std::ostream& Ornaments::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Ornaments::streamName( std::ostream& os ) const
        {
            os << "ornaments";
            return os;
        }


        bool Ornaments::hasContents() const
        {
            return true;
        }


        std::ostream& Ornaments::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            for ( auto x : myOrnamentsChoiceSet )
            {
                os << std::endl;
                x->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            for ( auto x : myAccidentalMarkSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            isOneLineOnly = false;
            os << std::endl;
            return os;
        }
        
        
        const OrnamentsChoiceSet& Ornaments::getOrnamentsChoiceSet() const
        {
            return myOrnamentsChoiceSet;
        }
        
        
        void Ornaments::removeOrnamentsChoice( const OrnamentsChoiceSetIterConst& value )
        {
            if ( value != myOrnamentsChoiceSet.cend() )
            {
                myOrnamentsChoiceSet.erase( value );
            }
        }
        
        
        void Ornaments::addOrnamentsChoice( const OrnamentsChoicePtr& value )
        {
            if ( value )
            {
                myOrnamentsChoiceSet.push_back( value );
            }
        }
        
        
        void Ornaments::clearOrnamentsChoiceSet()
        {
            myOrnamentsChoiceSet.clear();
        }
        
        
        OrnamentsChoicePtr Ornaments::getOrnamentsChoice( const OrnamentsChoiceSetIterConst& setIterator ) const
        {
            if( setIterator != myOrnamentsChoiceSet.cend() )
            {
                return *setIterator;
            }
            return OrnamentsChoicePtr();
        }


        const AccidentalMarkSet& Ornaments::getAccidentalMarkSet() const
        {
            return myAccidentalMarkSet;
        }


        void Ornaments::removeAccidentalMark( const AccidentalMarkSetIterConst& value )
        {
            if ( value != myAccidentalMarkSet.cend() )
            {
                myAccidentalMarkSet.erase( value );
            }
        }


        void Ornaments::addAccidentalMark( const AccidentalMarkPtr& value )
        {
            if ( value )
            {
                myAccidentalMarkSet.push_back( value );
            }
        }


        void Ornaments::clearAccidentalMarkSet()
        {
            myAccidentalMarkSet.clear();
        }


        AccidentalMarkPtr Ornaments::getAccidentalMark( const AccidentalMarkSetIterConst& setIterator ) const
        {
            if( setIterator != myAccidentalMarkSet.cend() )
            {
                return *setIterator;
            }
            return AccidentalMarkPtr();
        }
        
        
        constexpr const char* const ornamentsChoices[] =
        {
              "trill-mark"
            , "turn"
            , "delayed-turn"
            , "inverted-turn"
            , "delayed-inverted-turn"
            , "vertical-turn"
            , "shake"
            , "wavy-line"
            , "mordent"
            , "inverted-mordent"
            , "schleifer"
            , "tremolo"
            , "other-ornament"
        };
        
        
        constexpr size_t ornamentsChoicesSize = 13;

        
        bool Ornaments::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if( isOrnamentsChoice( it->getName().c_str() ) )
                {
                    auto item = makeOrnamentsChoice();
                    isSuccess &= item->fromXElement( message, *it );
                    addOrnamentsChoice( item );
                    continue;
                }
                
                if( importElementSet( message, it, endIter, isSuccess, "accidental-mark", myAccidentalMarkSet ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }
        
        
        bool Ornaments::isOrnamentsChoice( const char* const name ) const
        {
            for( size_t i = 0; i < ornamentsChoicesSize; ++ i )
            {
                if( strcmp( name, ornamentsChoices[i] ) == 0 )
                {
                    return true;
                }
            }
            return false;
        }

    }
}
