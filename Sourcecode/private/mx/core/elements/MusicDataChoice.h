// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( Backup )
        MX_FORWARD_DECLARE_ELEMENT( Barline )
        MX_FORWARD_DECLARE_ELEMENT( Bookmark )
        MX_FORWARD_DECLARE_ELEMENT( Direction )
        MX_FORWARD_DECLARE_ELEMENT( FiguredBass )
        MX_FORWARD_DECLARE_ELEMENT( Forward )
        MX_FORWARD_DECLARE_ELEMENT( Grouping )
        MX_FORWARD_DECLARE_ELEMENT( Harmony )
        MX_FORWARD_DECLARE_ELEMENT( Link )
        MX_FORWARD_DECLARE_ELEMENT( Note )
        MX_FORWARD_DECLARE_ELEMENT( Print )
        MX_FORWARD_DECLARE_ELEMENT( Properties )
        MX_FORWARD_DECLARE_ELEMENT( Sound )
        MX_FORWARD_DECLARE_ELEMENT( MusicDataChoice )

        inline MusicDataChoicePtr makeMusicDataChoice() { return std::make_shared<MusicDataChoice>(); }

        class MusicDataChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                note = 1,
                backup = 2,
                forward = 3,
                direction = 4,
                properties = 5,
                harmony = 6,
                figuredBass = 7,
                print = 8,
                sound = 9,
                barline = 10,
                grouping = 11,
                link = 12,
                bookmark = 13
            };
            MusicDataChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            MusicDataChoice::Choice getChoice() const;
            void setChoice( const MusicDataChoice::Choice value );

            /* _________ Note minOccurs = 1, maxOccurs = 1 _________ */
            NotePtr getNote() const;
            void setNote( const NotePtr& value );

            /* _________ Backup minOccurs = 1, maxOccurs = 1 _________ */
            BackupPtr getBackup() const;
            void setBackup( const BackupPtr& value );

            /* _________ Forward minOccurs = 1, maxOccurs = 1 _________ */
            ForwardPtr getForward() const;
            void setForward( const ForwardPtr& value );

            /* _________ Direction minOccurs = 1, maxOccurs = 1 _________ */
            DirectionPtr getDirection() const;
            void setDirection( const DirectionPtr& value );

            /* _________ Properties minOccurs = 1, maxOccurs = 1 _________ */
            PropertiesPtr getProperties() const;
            void setProperties( const PropertiesPtr& value );

            /* _________ Harmony minOccurs = 1, maxOccurs = 1 _________ */
            HarmonyPtr getHarmony() const;
            void setHarmony( const HarmonyPtr& value );

            /* _________ FiguredBass minOccurs = 1, maxOccurs = 1 _________ */
            FiguredBassPtr getFiguredBass() const;
            void setFiguredBass( const FiguredBassPtr& value );

            /* _________ Print minOccurs = 1, maxOccurs = 1 _________ */
            PrintPtr getPrint() const;
            void setPrint( const PrintPtr& value );

            /* _________ Sound minOccurs = 1, maxOccurs = 1 _________ */
            SoundPtr getSound() const;
            void setSound( const SoundPtr& value );

            /* _________ Barline minOccurs = 1, maxOccurs = 1 _________ */
            BarlinePtr getBarline() const;
            void setBarline( const BarlinePtr& value );

            /* _________ Grouping minOccurs = 1, maxOccurs = 1 _________ */
            GroupingPtr getGrouping() const;
            void setGrouping( const GroupingPtr& value );

            /* _________ Link minOccurs = 1, maxOccurs = 1 _________ */
            LinkPtr getLink() const;
            void setLink( const LinkPtr& value );

            /* _________ Bookmark minOccurs = 1, maxOccurs = 1 _________ */
            BookmarkPtr getBookmark() const;
            void setBookmark( const BookmarkPtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            MX_MUTEX
            Choice myChoice;
            mutable NotePtr myNote;
            mutable BackupPtr myBackup;
            mutable ForwardPtr myForward;
            mutable DirectionPtr myDirection;
            mutable PropertiesPtr myProperties;
            mutable HarmonyPtr myHarmony;
            mutable FiguredBassPtr myFiguredBass;
            mutable PrintPtr myPrint;
            mutable SoundPtr mySound;
            mutable BarlinePtr myBarline;
            mutable GroupingPtr myGrouping;
            mutable LinkPtr myLink;
            mutable BookmarkPtr myBookmark;
        };
    }
}
