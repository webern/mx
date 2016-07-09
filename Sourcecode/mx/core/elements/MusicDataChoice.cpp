// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/FromXElement.h"
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

#define MX_MAKE_JUST_IN_TIME( class_type ) \
if( !my##class_type ){ my##class_type = make##class_type(); }

namespace mx
{
    namespace core
    {
        MusicDataChoice::MusicDataChoice()
        :myChoice( Choice::note )
        ,myNote( nullptr )
        ,myBackup( nullptr )
        ,myForward( nullptr )
        ,myDirection( nullptr )
        ,myProperties( nullptr )
        ,myHarmony( nullptr )
        ,myFiguredBass( nullptr )
        ,myPrint( nullptr )
        ,mySound( nullptr )
        ,myBarline( nullptr )
        ,myGrouping( nullptr )
        ,myLink( nullptr )
        ,myBookmark( nullptr )
        {}


        bool MusicDataChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& MusicDataChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MusicDataChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool MusicDataChoice::hasContents() const
        {
            return true;
        }


        std::ostream& MusicDataChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            switch ( myChoice )
            {
                case Choice::note:
                    getNote()->toStream( os, indentLevel );
                    break;
                case Choice::backup:
                    getBackup()->toStream( os, indentLevel );
                    break;
                case Choice::forward:
                    getForward()->toStream( os, indentLevel );
                    break;
                case Choice::direction:
                    getDirection()->toStream( os, indentLevel );
                    break;
                case Choice::properties:
                    getProperties()->toStream( os, indentLevel );
                    break;
                case Choice::harmony:
                    getHarmony()->toStream( os, indentLevel );
                    break;
                case Choice::figuredBass:
                    getFiguredBass()->toStream( os, indentLevel );
                    break;
                case Choice::print:
                    getPrint()->toStream( os, indentLevel );
                    break;
                case Choice::sound:
                    getSound()->toStream( os, indentLevel );
                    break;
                case Choice::barline:
                    getBarline()->toStream( os, indentLevel );
                    break;
                case Choice::grouping:
                    getGrouping()->toStream( os, indentLevel );
                    break;
                case Choice::link:
                    getLink()->toStream( os, indentLevel );
                    break;
                case Choice::bookmark:
                    getBookmark()->toStream( os, indentLevel );
                    break;
                default:
                    break;
            }
            isOneLineOnly = false;
            return os;
        }


        MusicDataChoice::Choice MusicDataChoice::getChoice() const
        {
            return myChoice;
        }


        void MusicDataChoice::setChoice( const MusicDataChoice::Choice value )
        {
            myChoice = value;
        }


        NotePtr MusicDataChoice::getNote() const
        {
            MX_MAKE_JUST_IN_TIME( Note );
            return myNote;
        }


        void MusicDataChoice::setNote( const NotePtr& value )
        {
            if ( value )
            {
                myNote = value;
            }
        }


        BackupPtr MusicDataChoice::getBackup() const
        {
            MX_MAKE_JUST_IN_TIME( Backup );
            return myBackup;
        }


        void MusicDataChoice::setBackup( const BackupPtr& value )
        {
            if ( value )
            {
                myBackup = value;
            }
        }


        ForwardPtr MusicDataChoice::getForward() const
        {
            MX_MAKE_JUST_IN_TIME( Forward );
            return myForward;
        }


        void MusicDataChoice::setForward( const ForwardPtr& value )
        {
            if ( value )
            {
                myForward = value;
            }
        }


        DirectionPtr MusicDataChoice::getDirection() const
        {
            MX_MAKE_JUST_IN_TIME( Direction );
            return myDirection;
        }


        void MusicDataChoice::setDirection( const DirectionPtr& value )
        {
            if ( value )
            {
                myDirection = value;
            }
        }


        PropertiesPtr MusicDataChoice::getProperties() const
        {
            MX_MAKE_JUST_IN_TIME( Properties );
            return myProperties;
        }


        void MusicDataChoice::setProperties( const PropertiesPtr& value )
        {
            if ( value )
            {
                myProperties = value;
            }
        }


        HarmonyPtr MusicDataChoice::getHarmony() const
        {
            MX_MAKE_JUST_IN_TIME( Harmony );
            return myHarmony;
        }


        void MusicDataChoice::setHarmony( const HarmonyPtr& value )
        {
            if ( value )
            {
                myHarmony = value;
            }
        }


        FiguredBassPtr MusicDataChoice::getFiguredBass() const
        {
            MX_MAKE_JUST_IN_TIME( FiguredBass );
            return myFiguredBass;
        }


        void MusicDataChoice::setFiguredBass( const FiguredBassPtr& value )
        {
            if ( value )
            {
                myFiguredBass = value;
            }
        }


        PrintPtr MusicDataChoice::getPrint() const
        {
            MX_MAKE_JUST_IN_TIME( Print );
            return myPrint;
        }


        void MusicDataChoice::setPrint( const PrintPtr& value )
        {
            if ( value )
            {
                myPrint = value;
            }
        }


        SoundPtr MusicDataChoice::getSound() const
        {
            MX_MAKE_JUST_IN_TIME( Sound );
            return mySound;
        }


        void MusicDataChoice::setSound( const SoundPtr& value )
        {
            if ( value )
            {
                mySound = value;
            }
        }


        BarlinePtr MusicDataChoice::getBarline() const
        {
            MX_MAKE_JUST_IN_TIME( Barline );
            return myBarline;
        }


        void MusicDataChoice::setBarline( const BarlinePtr& value )
        {
            if ( value )
            {
                myBarline = value;
            }
        }


        GroupingPtr MusicDataChoice::getGrouping() const
        {
            MX_MAKE_JUST_IN_TIME( Grouping );
            return myGrouping;
        }


        void MusicDataChoice::setGrouping( const GroupingPtr& value )
        {
            if ( value )
            {
                myGrouping = value;
            }
        }


        LinkPtr MusicDataChoice::getLink() const
        {
            MX_MAKE_JUST_IN_TIME( Link );
            return myLink;
        }


        void MusicDataChoice::setLink( const LinkPtr& value )
        {
            if ( value )
            {
                myLink = value;
            }
        }


        BookmarkPtr MusicDataChoice::getBookmark() const
        {
            MX_MAKE_JUST_IN_TIME( Bookmark );
            return myBookmark;
        }


        void MusicDataChoice::setBookmark( const BookmarkPtr& value )
        {
            if ( value )
            {
                myBookmark = value;
            }
        }

        
        MX_FROM_XELEMENT_UNUSED( MusicDataChoice );

    }
}
