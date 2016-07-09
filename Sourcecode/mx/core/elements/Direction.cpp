// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Direction.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/core/elements/EditorialVoiceDirectionGroup.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/Sound.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/Voice.h"


#include "mx/core/elements/AccordionRegistration.h"
#include "mx/core/elements/Bracket.h"
#include "mx/core/elements/Coda.h"
#include "mx/core/elements/Damp.h"
#include "mx/core/elements/DampAll.h"
#include "mx/core/elements/Dashes.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/elements/Eyeglasses.h"
#include "mx/core/elements/HarpPedals.h"
#include "mx/core/elements/Image.h"
#include "mx/core/elements/Metronome.h"
#include "mx/core/elements/OctaveShift.h"
#include "mx/core/elements/OtherDirection.h"
#include "mx/core/elements/Pedal.h"
#include "mx/core/elements/Percussion.h"
#include "mx/core/elements/PrincipalVoice.h"
#include "mx/core/elements/Rehearsal.h"
#include "mx/core/elements/Scordatura.h"
#include "mx/core/elements/Segno.h"
#include "mx/core/elements/StringMute.h"
#include "mx/core/elements/Wedge.h"
#include "mx/core/elements/Words.h"

#include <cstdio>
#include <cstring>
#include <iostream>

namespace
{
    
    constexpr const size_t directionsSize = 22;
    constexpr const char* const directions[] =
    {
        "rehearsal",
        "segno",
        "words",
        "coda",
        "wedge",
        "dynamics",
        "dashes",
        "bracket",
        "pedal",
        "metronome",
        "octave-shift",
        "harp-pedals",
        "damp",
        "damp-all",
        "eyeglasses",
        "string-mute",
        "scordatura",
        "image",
        "principal-voice",
        "accordion-registration",
        "percussion",
        "other-direction"
    };
    
    
    constexpr const size_t multiDirectionsSize = 6;
    constexpr const char* const multiDirections[] =
    {
        "rehearsal",
        "segno",
        "words",
        "coda",
        "dynamics",
        "percussion"
    };
}

namespace mx
{
    namespace core
    {
        Direction::Direction()
        :myAttributes( std::make_shared<DirectionAttributes>() )
        ,myDirectionTypeSet()
        ,myOffset( makeOffset() )
        ,myHasOffset( false )
        ,myEditorialVoiceDirectionGroup( makeEditorialVoiceDirectionGroup() )
        ,myStaff( makeStaff() )
        ,myHasStaff( false )
        ,mySound( makeSound() )
        ,myHasSound( false )
        {
            myDirectionTypeSet.push_back( makeDirectionType() );
        }


        bool Direction::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Direction::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Direction::streamName( std::ostream& os ) const
        {
            os << "direction";
            return os;
        }


        bool Direction::hasContents() const
        {
            return true;
        }


        std::ostream& Direction::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            for ( auto x : myDirectionTypeSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            if ( myHasOffset )
            {
                os << std::endl;
                myOffset->toStream( os, indentLevel+1 );
            }
            if ( myEditorialVoiceDirectionGroup->hasContents() )
            {
                os << std::endl;
                myEditorialVoiceDirectionGroup->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            if ( myHasStaff )
            {
                os << std::endl;
                myStaff->toStream( os, indentLevel+1 );
            }
            if ( myHasSound )
            {
                os << std::endl;
                mySound->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        DirectionAttributesPtr Direction::getAttributes() const
        {
            return myAttributes;
        }


        void Direction::setAttributes( const DirectionAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        const DirectionTypeSet& Direction::getDirectionTypeSet() const
        {
            return myDirectionTypeSet;
        }


        void Direction::addDirectionType( const DirectionTypePtr& value )
        {
            if ( value )
            {
                myDirectionTypeSet.push_back( value );
            }
        }


        void Direction::removeDirectionType( const DirectionTypeSetIterConst& value )
        {
            if ( value != myDirectionTypeSet.cend() )
            {
                if( myDirectionTypeSet.size() > 1 )
                {
                    myDirectionTypeSet.erase( value );
                }
            }
        }


        void Direction::clearDirectionTypeSet()
        {
            myDirectionTypeSet.clear();
            myDirectionTypeSet.push_back( makeDirectionType() );
        }


        DirectionTypePtr Direction::getDirectionType( const DirectionTypeSetIterConst& setIterator ) const
        {
            if( setIterator != myDirectionTypeSet.cend() )
            {
                return *setIterator;
            }
            return DirectionTypePtr();
        }


        OffsetPtr Direction::getOffset() const
        {
            return myOffset;
        }


        void Direction::setOffset( const OffsetPtr& value )
        {
            if ( value )
            {
                myOffset = value;
            }
        }


        bool Direction::getHasOffset() const
        {
            return myHasOffset;
        }


        void Direction::setHasOffset( const bool value )
        {
            myHasOffset = value;
        }


        EditorialVoiceDirectionGroupPtr Direction::getEditorialVoiceDirectionGroup() const
        {
            return myEditorialVoiceDirectionGroup;
        }


        void Direction::setEditorialVoiceDirectionGroup( const EditorialVoiceDirectionGroupPtr& value )
        {
            if ( value )
            {
                myEditorialVoiceDirectionGroup = value;
            }
        }


        StaffPtr Direction::getStaff() const
        {
            return myStaff;
        }


        void Direction::setStaff( const StaffPtr& value )
        {
            if ( value )
            {
                myStaff = value;
            }
        }


        bool Direction::getHasStaff() const
        {
            return myHasStaff;
        }


        void Direction::setHasStaff( const bool value )
        {
            myHasStaff = value;
        }


        SoundPtr Direction::getSound() const
        {
            return mySound;
        }


        void Direction::setSound( const SoundPtr& value )
        {
            if ( value )
            {
                mySound = value;
            }
        }


        bool Direction::getHasSound() const
        {
            return myHasSound;
        }


        void Direction::setHasSound( const bool value )
        {
            myHasSound = value;
        }
        

        bool Direction::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isDirectionTypeFound = false;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importDirectionTypeSet( message, it, endIter, isSuccess, isDirectionTypeFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myOffset, myHasOffset ) ) { continue; }
                importGroup( message, it, endIter, isSuccess, myEditorialVoiceDirectionGroup );
                if ( importElement( message, *it, isSuccess, *myStaff, myHasStaff ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *mySound, myHasSound ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }
        
        
        bool Direction::importDirectionTypeSet( std::ostream& message, xml::XElementIterator& iter, xml::XElementIterator& endIter, bool& isSuccess, bool& isFound )
        {
            if( iter == endIter )
            {
                isFound = false;
                return false;
            }
            
            if( iter->getName() != "direction-type" )
            {
                isFound = false;
                return false;
            }
            
            isFound = true;
            bool isIterIncremented = false;
            bool isFirstDirectionTypeAdded = false;
            
            while( ( iter != endIter ) && ( iter->getName() == "direction-type" ) )
            {
                auto subiter = iter->begin();
                auto subiterEnd = iter->end();
                
                auto directionType = createDirectionType( message, subiter, subiterEnd, isSuccess );
                
                if( !isFirstDirectionTypeAdded && myDirectionTypeSet.size() == 1 )
                {
                    *myDirectionTypeSet.begin() = directionType;
                    isFirstDirectionTypeAdded = true;
                }
                else
                {
                    myDirectionTypeSet.push_back( directionType );
                    isFirstDirectionTypeAdded = true;
                }
                
                isIterIncremented = true;
                ++iter;
            }
            
            if( isIterIncremented )
            {
                --iter;
            }
            return isFirstDirectionTypeAdded;
        }
        
        
        bool Direction::isDirectionType( const std::string& elementName ) const
        {
            for( size_t i = 0; i < directionsSize; ++i )
            {
                if( strcmp( directions[i], elementName.c_str() ) == 0 )
                {
                    return true;
                }
            }
            return false;
        }
        
        
        bool Direction::isMultiDirectionType( const std::string& elementName ) const
        {
            for( size_t i = 0; i < multiDirectionsSize; ++i )
            {
                if( strcmp( multiDirections[i], elementName.c_str() ) == 0 )
                {
                    return true;
                }
            }
            return false;
        }
        
        
        DirectionTypePtr Direction::createDirectionType( std::ostream& message, xml::XElementIterator& subIter, xml::XElementIterator& subIterEnd, bool& isSuccess )
        {
            auto directionType = makeDirectionType();
            
            if( subIter == subIterEnd )
            {
                message << "Direction: well thats weird - should not get here" << std::endl;
                isSuccess = false;
                return directionType;
            }
            
            if( subIter->getName() == "wedge" )
            {
                directionType->setChoice( DirectionType::Choice::wedge );
                isSuccess &= directionType->getWedge()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "dashes" )
            {
                directionType->setChoice( DirectionType::Choice::dashes );
                isSuccess &= directionType->getDashes()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "bracket" )
            {
                directionType->setChoice( DirectionType::Choice::bracket );
                isSuccess &= directionType->getBracket()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "pedal" )
            {
                directionType->setChoice( DirectionType::Choice::pedal );
                isSuccess &= directionType->getPedal()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "metronome" )
            {
                directionType->setChoice( DirectionType::Choice::metronome );
                isSuccess &= directionType->getMetronome()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "octave-shift" )
            {
                directionType->setChoice( DirectionType::Choice::octaveShift );
                isSuccess &= directionType->getOctaveShift()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "harp-pedals" )
            {
                directionType->setChoice( DirectionType::Choice::harpPedals );
                isSuccess &= directionType->getHarpPedals()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "damp" )
            {
                directionType->setChoice( DirectionType::Choice::damp );
                isSuccess &= directionType->getDamp()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "damp-all" )
            {
                directionType->setChoice( DirectionType::Choice::dampAll );
                isSuccess &= directionType->getDampAll()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "eyeglasses" )
            {
                directionType->setChoice( DirectionType::Choice::eyeglasses );
                isSuccess &= directionType->getEyeglasses()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "string-mute" )
            {
                directionType->setChoice( DirectionType::Choice::stringMute );
                isSuccess &= directionType->getStringMute()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "scordatura" )
            {
                directionType->setChoice( DirectionType::Choice::scordatura );
                isSuccess &= directionType->getScordatura()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "image" )
            {
                directionType->setChoice( DirectionType::Choice::image );
                isSuccess &= directionType->getImage()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "principal-voice" )
            {
                directionType->setChoice( DirectionType::Choice::principalVoice );
                isSuccess &= directionType->getPrincipalVoice()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "accordion-registration" )
            {
                directionType->setChoice( DirectionType::Choice::accordionRegistration );
                isSuccess &= directionType->getAccordionRegistration()->fromXElement( message, *subIter );
                return directionType;
            }
            
            if( subIter->getName() == "other-direction" )
            {
                directionType->setChoice( DirectionType::Choice::otherDirection );
                isSuccess &= directionType->getOtherDirection()->fromXElement( message, *subIter );
                return directionType;
            }
                
            std::string name = "rehearsal";
            if( subIter->getName() == name )
            {
                directionType->setChoice( DirectionType::Choice::rehearsal );
                bool isFirstSubItemAdded = false;
                
                while( subIter != subIterEnd )
                {
                    if( subIter->getName() != name )
                    {
                        message << "Direction: createDirectionType encountered an unexpected element '" << subIter->getName() << "' while parsing a collection of '" << name << "' elements" << std::endl;
                        isSuccess = false;
                        return directionType;
                    }
                    auto itemToAdd = makeRehearsal();
                    isSuccess &= itemToAdd->fromXElement( message, *subIter );
                    if( !isFirstSubItemAdded && directionType->getRehearsalSet().size() == 1 )
                    {
                        directionType->addRehearsal( itemToAdd );
                        directionType->removeRehearsal( directionType->getRehearsalSet().cbegin() );
                    }
                    else
                    {
                        directionType->addRehearsal( itemToAdd );
                    }
                    isFirstSubItemAdded = true;
                    ++subIter;
                } // end loop
                return directionType;
            } // end rehearsal
            
            
            name = "segno";
            if( subIter->getName() == name )
            {
                directionType->setChoice( DirectionType::Choice::segno );
                bool isFirstSubItemAdded = false;
                
                while( subIter != subIterEnd )
                {
                    if( subIter->getName() != name )
                    {
                        message << "Direction: createDirectionType encountered an unexpected element '" << subIter->getName() << "' while parsing a collection of '" << name << "' elements" << std::endl;
                        isSuccess = false;
                        return directionType;
                    }
                    auto itemToAdd = makeSegno();
                    isSuccess &= itemToAdd->fromXElement( message, *subIter );
                    if( !isFirstSubItemAdded && directionType->getSegnoSet().size() == 1 )
                    {
                        directionType->addSegno( itemToAdd );
                        directionType->removeSegno( directionType->getSegnoSet().cbegin() );
                    }
                    else
                    {
                        directionType->addSegno( itemToAdd );
                    }
                    isFirstSubItemAdded = true;
                    ++subIter;
                } // end loop
                return directionType;
            } // end segno
            
            
            name = "words";
            if( subIter->getName() == name )
            {
                directionType->setChoice( DirectionType::Choice::words );
                bool isFirstSubItemAdded = false;
                
                while( subIter != subIterEnd )
                {
                    if( subIter->getName() != name )
                    {
                        message << "Direction: createDirectionType encountered an unexpected element '" << subIter->getName() << "' while parsing a collection of '" << name << "' elements" << std::endl;
                        isSuccess = false;
                        return directionType;
                    }
                    auto itemToAdd = makeWords();
                    isSuccess &= itemToAdd->fromXElement( message, *subIter );
                    if( !isFirstSubItemAdded && directionType->getWordsSet().size() == 1 )
                    {
                        directionType->addWords( itemToAdd );
                        directionType->removeWords( directionType->getWordsSet().cbegin() );
                    }
                    else
                    {
                        directionType->addWords( itemToAdd );
                    }
                    isFirstSubItemAdded = true;
                    ++subIter;
                } // end loop
                return directionType;
            } // end words
            
            
            name = "coda";
            if( subIter->getName() == name )
            {
                directionType->setChoice( DirectionType::Choice::coda );
                bool isFirstSubItemAdded = false;
                
                while( subIter != subIterEnd )
                {
                    if( subIter->getName() != name )
                    {
                        message << "Direction: createDirectionType encountered an unexpected element '" << subIter->getName() << "' while parsing a collection of '" << name << "' elements" << std::endl;
                        isSuccess = false;
                        return directionType;
                    }
                    auto itemToAdd = makeCoda();
                    isSuccess &= itemToAdd->fromXElement( message, *subIter );
                    if( !isFirstSubItemAdded && directionType->getCodaSet().size() == 1 )
                    {
                        directionType->addCoda( itemToAdd );
                        directionType->removeCoda( directionType->getCodaSet().cbegin() );
                    }
                    else
                    {
                        directionType->addCoda( itemToAdd );
                    }
                    isFirstSubItemAdded = true;
                    ++subIter;
                } // end loop
                return directionType;
            } // end coda
            
            
            name = "dynamics";
            if( subIter->getName() == name )
            {
                directionType->setChoice( DirectionType::Choice::dynamics );
                bool isFirstSubItemAdded = false;
                
                while( subIter != subIterEnd )
                {
                    if( subIter->getName() != name )
                    {
                        message << "Direction: createDirectionType encountered an unexpected element '" << subIter->getName() << "' while parsing a collection of '" << name << "' elements" << std::endl;
                        isSuccess = false;
                        return directionType;
                    }
                    auto itemToAdd = makeDynamics();
                    isSuccess &= itemToAdd->fromXElement( message, *subIter );
                    if( !isFirstSubItemAdded && directionType->getDynamicsSet().size() == 1 )
                    {
                        directionType->addDynamics( itemToAdd );
                        directionType->removeDynamics( directionType->getDynamicsSet().cbegin() );
                    }
                    else
                    {
                        directionType->addDynamics( itemToAdd );
                    }
                    isFirstSubItemAdded = true;
                    ++subIter;
                } // end loop
                return directionType;
            } // end dynamics
            
            
            name = "percussion";
            if( subIter->getName() == name )
            {
                directionType->setChoice( DirectionType::Choice::percussion );
                bool isFirstSubItemAdded = false;
                
                while( subIter != subIterEnd )
                {
                    if( subIter->getName() != name )
                    {
                        message << "Direction: createDirectionType encountered an unexpected element '" << subIter->getName() << "' while parsing a collection of '" << name << "' elements" << std::endl;
                        isSuccess = false;
                        return directionType;
                    }
                    auto itemToAdd = makePercussion();
                    isSuccess &= itemToAdd->fromXElement( message, *subIter );
                    if( !isFirstSubItemAdded && directionType->getPercussionSet().size() == 1 )
                    {
                        directionType->addPercussion( itemToAdd );
                        directionType->removePercussion( directionType->getPercussionSet().cbegin() );
                    }
                    else
                    {
                        directionType->addPercussion( itemToAdd );
                    }
                    isFirstSubItemAdded = true;
                    ++subIter;
                } // end loop
                return directionType;
            } // end percussion

            return directionType;
        }
        
    }
}
