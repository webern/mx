// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Notations.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/NotationsChoice.h"
#include "mx/core/elements/AccidentalMark.h"
#include "mx/core/elements/Arpeggiate.h"
#include "mx/core/elements/Articulations.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/Fermata.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Glissando.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/NonArpeggiate.h"
#include "mx/core/elements/Ornaments.h"
#include "mx/core/elements/OtherNotation.h"
#include "mx/core/elements/Slide.h"
#include "mx/core/elements/Slur.h"
#include "mx/core/elements/Technical.h"
#include "mx/core/elements/Tied.h"
#include "mx/core/elements/Tuplet.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Notations::Notations()
        :myAttributes( std::make_shared<NotationsAttributes>() )
        ,myEditorialGroup( makeEditorialGroup() )
        ,myNotationsChoiceSet()
        {}


        bool Notations::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Notations::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Notations::streamName( std::ostream& os ) const
        {
            os << "notations";
            return os;
        }


        bool Notations::hasContents() const
        {
            return myEditorialGroup->hasContents() || myNotationsChoiceSet.size() > 0;
        }


        std::ostream& Notations::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                if ( myEditorialGroup->hasContents() )
                {
                    os << std::endl;
                    myEditorialGroup->streamContents( os, indentLevel+1, isOneLineOnly );
                }
                for ( auto x : myNotationsChoiceSet )
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


        NotationsAttributesPtr Notations::getAttributes() const
        {
            return myAttributes;
        }


        void Notations::setAttributes( const NotationsAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        EditorialGroupPtr Notations::getEditorialGroup() const
        {
            return myEditorialGroup;
        }


        void Notations::setEditorialGroup( const EditorialGroupPtr& value )
        {
            if ( value )
            {
                myEditorialGroup = value;
            }
        }


        const NotationsChoiceSet& Notations::getNotationsChoiceSet() const
        {
            return myNotationsChoiceSet;
        }


        void Notations::addNotationsChoice( const NotationsChoicePtr& value )
        {
            if ( value )
            {
                myNotationsChoiceSet.push_back( value );
            }
        }


        void Notations::removeNotationsChoice( const NotationsChoiceSetIterConst& value )
        {
            if ( value != myNotationsChoiceSet.cend() )
            {
                myNotationsChoiceSet.erase( value );
            }
        }


        void Notations::clearNotationsChoiceSet()
        {
            myNotationsChoiceSet.clear();
        }


        NotationsChoicePtr Notations::getNotationsChoice( const NotationsChoiceSetIterConst& setIterator ) const
        {
            if( setIterator != myNotationsChoiceSet.cend() )
            {
                return *setIterator;
            }
            return NotationsChoicePtr();
        }

        
        bool Notations::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                const std::string elementName = it->getName();
                
                if( elementName == "footnote" )
                {
                    myEditorialGroup->setHasFootnote( true );
                    isSuccess &= myEditorialGroup->getFootnote()->fromXElement( message, *it );
                }
                else if ( elementName == "level" )
                {
                    myEditorialGroup->setHasLevel( true );
                    isSuccess &= myEditorialGroup->getLevel()->fromXElement( message, *it );
                }
                else if ( elementName == "tied" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::tied );
                    isSuccess &= choice->getTied()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else if ( elementName == "tuplet" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::tuplet );
                    isSuccess &= choice->getTuplet()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else if ( elementName == "slur" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::slur );
                    isSuccess &= choice->getSlur()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else if ( elementName == "glissando" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::glissando );
                    isSuccess &= choice->getGlissando()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else if ( elementName == "slide" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::slide );
                    isSuccess &= choice->getSlide()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else if ( elementName == "ornaments" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::ornaments );
                    isSuccess &= choice->getOrnaments()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else if ( elementName == "technical" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::technical );
                    isSuccess &= choice->getTechnical()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else if ( elementName == "articulations" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::articulations );
                    isSuccess &= choice->getArticulations()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else if ( elementName == "dynamics" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::dynamics );
                    isSuccess &= choice->getDynamics()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else if ( elementName == "fermata" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::fermata );
                    isSuccess &= choice->getFermata()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else if ( elementName == "arpeggiate" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::arpeggiate );
                    isSuccess &= choice->getArpeggiate()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else if ( elementName == "non-arpeggiate" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::nonArpeggiate );
                    isSuccess &= choice->getNonArpeggiate()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else if ( elementName == "accidental-mark" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::accidentalMark );
                    isSuccess &= choice->getAccidentalMark()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else if ( elementName == "other-notations" )
                {
                    auto choice = makeNotationsChoice();
                    choice->setChoice( NotationsChoice::Choice::otherNotation );
                    isSuccess &= choice->getOtherNotation()->fromXElement( message, *it );
                    myNotationsChoiceSet.push_back( choice );
                }
                else
                {
                    isSuccess = false;
                    message << "Notations: fromXElement unexpected element '" << elementName << "' encountered" << std::endl;
                }
            }
            
            MX_RETURN_IS_SUCCESS;
        }
    }
}
