// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/Backup.h"
#include "mx/core/elements/Barline.h"
#include "mx/core/elements/Bookmark.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/FiguredBass.h"
#include "mx/core/elements/Forward.h"
#include "mx/core/elements/Grouping.h"
#include "mx/core/elements/Harmony.h"
#include "mx/core/elements/Link.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/Print.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Sound.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PartwiseMeasure::PartwiseMeasure()
        :myAttributes( nullptr )
        ,myMusicDataGroup( makeMusicDataGroup() )
        {}


        bool PartwiseMeasure::hasAttributes() const
        {
            return getAttributes()->hasValues();
        }


        std::ostream& PartwiseMeasure::streamAttributes( std::ostream& os ) const
        {
            return getAttributes()->toStream( os );
        }


        std::ostream& PartwiseMeasure::streamName( std::ostream& os ) const
        {
            os << "measure";
            return os;
        }


        bool PartwiseMeasure::hasContents() const
        {
            return myMusicDataGroup->hasContents();
        }


        std::ostream& PartwiseMeasure::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                os << std::endl;
                myMusicDataGroup->streamContents( os, indentLevel+1, isOneLineOnly );
                os << std::endl;
                isOneLineOnly = false;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        MeasureAttributesPtr PartwiseMeasure::getAttributes() const
        {
            MX_LOCK
            MX_JIT_ALLOCATE_ATTRIBUTES( MeasureAttributes );
            return myAttributes;
        }


        void PartwiseMeasure::setAttributes( const MeasureAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        MusicDataGroupPtr PartwiseMeasure::getMusicDataGroup() const
        {
            return myMusicDataGroup;
        }


        void PartwiseMeasure::setMusicDataGroup( const MusicDataGroupPtr& value )
        {
            if ( value )
            {
                myMusicDataGroup = value;
            }
        }
        
        
        bool PartwiseMeasure::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isFirstMusicDataChoiceAdded = false;
            isSuccess &= getAttributes()->fromXElement( message, xelement );
            
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                const std::string elementName = it->getName();
                
                auto choiceObject = makeMusicDataChoice();
                bool choiceObjectShouldBeAdded = true;
                
                if( elementName == "note" )
                {
                    choiceObject->setChoice( MusicDataChoice::Choice::note );
                    isSuccess &= choiceObject->getNote()->fromXElement( message, *it );
                    MX_DEBUG_THROW_ON_PARSE_ISSUE;
                }
                else if ( elementName == "backup" )
                {
                    choiceObject->setChoice( MusicDataChoice::Choice::backup );
                    isSuccess &= choiceObject->getBackup()->fromXElement( message, *it );
                    MX_DEBUG_THROW_ON_PARSE_ISSUE;
                }
                else if ( elementName == "forward" )
                {
                    choiceObject->setChoice( MusicDataChoice::Choice::forward );
                    isSuccess &= choiceObject->getForward()->fromXElement( message, *it );
                    MX_DEBUG_THROW_ON_PARSE_ISSUE;
                }
                else if ( elementName == "direction" )
                {
                    choiceObject->setChoice( MusicDataChoice::Choice::direction );
                    isSuccess &= choiceObject->getDirection()->fromXElement( message, *it );
                    MX_DEBUG_THROW_ON_PARSE_ISSUE;
                }
                else if ( elementName == "attributes" )
                {
                    choiceObject->setChoice( MusicDataChoice::Choice::properties );
                    isSuccess &= choiceObject->getProperties()->fromXElement( message, *it );
                    MX_DEBUG_THROW_ON_PARSE_ISSUE;
                }
                else if ( elementName == "harmony" )
                {
                    choiceObject->setChoice( MusicDataChoice::Choice::harmony );
                    isSuccess &= choiceObject->getHarmony()->fromXElement( message, *it );
                    MX_DEBUG_THROW_ON_PARSE_ISSUE;
                }
                else if ( elementName == "figured-bass" )
                {
                    choiceObject->setChoice( MusicDataChoice::Choice::figuredBass );
                    isSuccess &= choiceObject->getFiguredBass()->fromXElement( message, *it );
                    MX_DEBUG_THROW_ON_PARSE_ISSUE;
                }
                else if ( elementName == "print" )
                {
                    choiceObject->setChoice( MusicDataChoice::Choice::print );
                    isSuccess &= choiceObject->getPrint()->fromXElement( message, *it );
                    MX_DEBUG_THROW_ON_PARSE_ISSUE;
                }
                else if ( elementName == "sound" )
                {
                    choiceObject->setChoice( MusicDataChoice::Choice::sound );
                    isSuccess &= choiceObject->getSound()->fromXElement( message, *it );
                    MX_DEBUG_THROW_ON_PARSE_ISSUE;
                }
                else if ( elementName == "barline" )
                {
                    choiceObject->setChoice( MusicDataChoice::Choice::barline );
                    isSuccess &= choiceObject->getBarline()->fromXElement( message, *it );
                    MX_DEBUG_THROW_ON_PARSE_ISSUE;
                }
                else if ( elementName == "grouping" )
                {
                    choiceObject->setChoice( MusicDataChoice::Choice::grouping );
                    isSuccess &= choiceObject->getGrouping()->fromXElement( message, *it );
                    MX_DEBUG_THROW_ON_PARSE_ISSUE;
                }
                else if ( elementName == "link" )
                {
                    choiceObject->setChoice( MusicDataChoice::Choice::link );
                    isSuccess &= choiceObject->getLink()->fromXElement( message, *it );
                    MX_DEBUG_THROW_ON_PARSE_ISSUE;
                }
                else if ( elementName == "bookmark" )
                {
                    choiceObject->setChoice( MusicDataChoice::Choice::bookmark );
                    isSuccess &= choiceObject->getBookmark()->fromXElement( message, *it );
                    MX_DEBUG_THROW_ON_PARSE_ISSUE;
                }
                else
                {
                    choiceObjectShouldBeAdded = false;
                }
                
                if( choiceObjectShouldBeAdded )
                {
                    if( !isFirstMusicDataChoiceAdded && myMusicDataGroup->getMusicDataChoiceSet().size() == 1 )
                    {
                        myMusicDataGroup->addMusicDataChoice( choiceObject );
                        myMusicDataGroup->removeMusicDataChoice( myMusicDataGroup->getMusicDataChoiceSet().cbegin() );
                        isFirstMusicDataChoiceAdded = true;
                    }
                    else
                    {
                        myMusicDataGroup->addMusicDataChoice( choiceObject );
                        isFirstMusicDataChoiceAdded = true;
                    }
                }
            }
            
            MX_RETURN_IS_SUCCESS;
        }

    }
}
