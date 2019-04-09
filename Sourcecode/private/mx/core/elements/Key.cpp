// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Key.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/KeyChoice.h"
#include "mx/core/elements/KeyOctave.h"
#include "mx/core/elements/TraditionalKey.h"
#include "mx/core/elements/NonTraditionalKey.h"
#include "mx/core/elements/Cancel.h"
#include "mx/core/elements/Mode.h"
#include "mx/core/elements/Fifths.h"
#include "mx/core/elements/KeyAccidental.h"
#include "mx/core/elements/KeyAlter.h"
#include "mx/core/elements/KeyStep.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Key::Key()
        :myAttributes( std::make_shared<KeyAttributes>() )
        ,myKeyChoice( std::make_shared<KeyChoice>() )
        ,myKeyOctaveSet()
        {}


        bool Key::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Key::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Key::streamName( std::ostream& os ) const
        {
            os << "key";
            return os;
        }


        bool Key::hasContents() const
        {
            return myKeyChoice->hasContents() || myKeyOctaveSet.size() > 0;
        }


        std::ostream& Key::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myKeyChoice->hasContents() )
            {
                os << std::endl;
                if ( myKeyChoice )
                {
                    myKeyChoice->streamContents( os, indentLevel+1, isOneLineOnly );
                }
                os << std::endl;
            }
            for ( auto it = myKeyOctaveSet.cbegin();
                 it != myKeyOctaveSet.cend();
                 ++it )
            {
                if ( it == myKeyOctaveSet.cbegin() )
                {
                    os << std::endl;
                }
                (*it)->toStream( os, indentLevel+1 );
                os << std::endl;
            }
            if ( myKeyChoice->hasContents() )
            {
                isOneLineOnly = false;
            }
            if ( myKeyOctaveSet.size() > 0 )
            {
                isOneLineOnly = false;
            }
            return os;
        }


        KeyAttributesPtr Key::getAttributes() const
        {
            return myAttributes;
        }


        void Key::setAttributes( const KeyAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        KeyChoicePtr Key::getKeyChoice() const
        {
            return myKeyChoice;
        }


        void Key::setKeyChoice( const KeyChoicePtr& value )
        {
            if ( value )
            {
                myKeyChoice = value;
            }
        }


        const KeyOctaveSet& Key::getKeyOctaveSet() const
        {
            return myKeyOctaveSet;
        }


        void Key::removeKeyOctave( const KeyOctaveSetIterConst& value )
        {
            if ( value != myKeyOctaveSet.cend() )
            {
                myKeyOctaveSet.erase( value );
            }
        }


        void Key::addKeyOctave( const KeyOctavePtr& value )
        {
            if ( value )
            {
                myKeyOctaveSet.push_back( value );
            }
        }


        void Key::clearKeyOctaveSet()
        {
            myKeyOctaveSet.clear();
        }


        bool Key::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importTraditionalKey( message, it, endIter, isSuccess ) ) { continue; }
                if ( importNonTraditionalKey( message, it, endIter, isSuccess ) ) { continue; }
                if ( importElementSet( message, it, endIter, isSuccess, "key-octave", myKeyOctaveSet ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

        
        bool Key::importTraditionalKey( std::ostream& message, xml::XElementIterator& iter, xml::XElementIterator& endIter, bool& isSuccess )
        {
            if( iter == endIter )
            {
                return false;
            }
            
            if( iter->getName() != "cancel" && iter->getName() != "fifths" && iter->getName() != "mode" )
            {
                return false;
            }
            
            bool isIterIncremented = false;
            
            if( iter != endIter && iter->getName() == "cancel" )
            {
                myKeyChoice->setChoice( KeyChoice::Choice::traditionalKey );
                myKeyChoice->getTraditionalKey()->setHasCancel( true );
                isSuccess &= myKeyChoice->getTraditionalKey()->getCancel()->fromXElement( message, *iter );
                isIterIncremented = true;
                ++iter;
            }
            
            if( iter != endIter && iter->getName() == "fifths" )
            {
                myKeyChoice->setChoice( KeyChoice::Choice::traditionalKey );
                isSuccess &= myKeyChoice->getTraditionalKey()->getFifths()->fromXElement( message, *iter );
                isIterIncremented = true;
                ++iter;
            }
            
            if( iter != endIter && iter->getName() == "mode" )
            {
                myKeyChoice->setChoice( KeyChoice::Choice::traditionalKey );
                myKeyChoice->getTraditionalKey()->setHasMode( true );
                isSuccess &= myKeyChoice->getTraditionalKey()->getMode()->fromXElement( message, *iter );
                isIterIncremented = true;
                ++iter;
            }
            
            if( isIterIncremented )
            {
                --iter;
            }
            
            return true;
        }

        
        bool Key::importNonTraditionalKey( std::ostream& message, xml::XElementIterator& iter, xml::XElementIterator& endIter, bool& isSuccess )
        {
            if( iter == endIter )
            {
                return false;
            }
            
            if( iter->getName() != "key-step" && iter->getName() != "key-alter" && iter->getName() != "key-accidental" )
            {
                return false;
            }
            
            bool isIterIncremented = false;
            bool isFirstItemAdded = false;
            
            while ( iter != endIter && ( iter->getName() == "key-step" || iter->getName() == "key-alter" || iter->getName() == "key-accidental" ) )
            {
                auto item = makeNonTraditionalKey();
                const auto& items = myKeyChoice->getNonTraditionalKeySet();
                myKeyChoice->setChoice( KeyChoice::Choice::nonTraditionalKey );
                
                if( iter != endIter && iter->getName() == "key-step" )
                {
                    isSuccess &= item->getKeyStep()->fromXElement( message, *iter );
                    isIterIncremented = true;
                    ++iter;
                }
                
                if( iter != endIter && iter->getName() == "key-alter" )
                {
                    isSuccess &= item->getKeyAlter()->fromXElement( message, *iter );
                    isIterIncremented = true;
                    ++iter;
                }
                
                if( iter != endIter && iter->getName() == "key-accidental" )
                {
                    item->setHasKeyAccidental( true );
                    isSuccess &= item->getKeyAccidental()->fromXElement( message, *iter );
                    isIterIncremented = true;
                    ++iter;
                }
                
                if( !isFirstItemAdded && items.size() == 1 )
                {
                    myKeyChoice->addNonTraditionalKey( item );
                    myKeyChoice->removeNonTraditionalKey( items.cbegin() );
                    isFirstItemAdded = true;
                }
                else
                {
                    myKeyChoice->addNonTraditionalKey( item );
                    isFirstItemAdded = true;
                }
            }
            
            if( isIterIncremented )
            {
                --iter;
            }
            
            return true;
        }
        
        
    }
}
