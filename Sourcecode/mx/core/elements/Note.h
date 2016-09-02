// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/NoteAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    
    namespace xml
    {
        class XElementIterator;
    }
    
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( NoteAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Accidental )
        MX_FORWARD_DECLARE_ELEMENT( Beam )
        MX_FORWARD_DECLARE_ELEMENT( Dot )
        MX_FORWARD_DECLARE_ELEMENT( EditorialVoiceGroup )
        MX_FORWARD_DECLARE_ELEMENT( FullNoteGroup )
        MX_FORWARD_DECLARE_ELEMENT( Instrument )
        MX_FORWARD_DECLARE_ELEMENT( Lyric )
        MX_FORWARD_DECLARE_ELEMENT( Notations )
        MX_FORWARD_DECLARE_ELEMENT( NoteChoice )
        MX_FORWARD_DECLARE_ELEMENT( Notehead )
        MX_FORWARD_DECLARE_ELEMENT( NoteheadText )
        MX_FORWARD_DECLARE_ELEMENT( Play )
        MX_FORWARD_DECLARE_ELEMENT( Staff )
        MX_FORWARD_DECLARE_ELEMENT( Stem )
        MX_FORWARD_DECLARE_ELEMENT( TimeModification )
        MX_FORWARD_DECLARE_ELEMENT( Type )
        MX_FORWARD_DECLARE_ELEMENT( Note )

        inline NotePtr makeNote() { return std::make_shared<Note>(); }

        class Note : public ElementInterface
        {
        public:
            Note();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NoteAttributesPtr getAttributes() const;
            void setAttributes( const NoteAttributesPtr& value );

            /* _________ NoteChoice minOccurs = 1, maxOccurs = 1 _________ */
            NoteChoicePtr getNoteChoice() const;
            void setNoteChoice( const NoteChoicePtr& value );

            /* _________ Instrument minOccurs = 0, maxOccurs = 1 _________ */
            InstrumentPtr getInstrument() const;
            void setInstrument( const InstrumentPtr& value );
            bool getHasInstrument() const;
            void setHasInstrument( const bool value );

            /* _________ EditorialVoiceGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialVoiceGroupPtr getEditorialVoiceGroup() const;
            void setEditorialVoiceGroup( const EditorialVoiceGroupPtr& value );

            /* _________ Type minOccurs = 0, maxOccurs = 1 _________ */
            TypePtr getType() const;
            void setType( const TypePtr& value );
            bool getHasType() const;
            void setHasType( const bool value );

            /* _________ Dot minOccurs = 0, maxOccurs = unbounded _________ */
            const DotSet& getDotSet() const;
            void addDot( const DotPtr& value );
            void removeDot( const DotSetIterConst& value );
            void clearDotSet();
            DotPtr getDot( const DotSetIterConst& setIterator ) const;

            /* _________ Accidental minOccurs = 0, maxOccurs = 1 _________ */
            AccidentalPtr getAccidental() const;
            void setAccidental( const AccidentalPtr& value );
            bool getHasAccidental() const;
            void setHasAccidental( const bool value );

            /* _________ TimeModification minOccurs = 0, maxOccurs = 1 _________ */
            TimeModificationPtr getTimeModification() const;
            void setTimeModification( const TimeModificationPtr& value );
            bool getHasTimeModification() const;
            void setHasTimeModification( const bool value );

            /* _________ Stem minOccurs = 0, maxOccurs = 1 _________ */
            StemPtr getStem() const;
            void setStem( const StemPtr& value );
            bool getHasStem() const;
            void setHasStem( const bool value );

            /* _________ Notehead minOccurs = 0, maxOccurs = 1 _________ */
            NoteheadPtr getNotehead() const;
            void setNotehead( const NoteheadPtr& value );
            bool getHasNotehead() const;
            void setHasNotehead( const bool value );

            /* _________ NoteheadText minOccurs = 0, maxOccurs = 1 _________ */
            NoteheadTextPtr getNoteheadText() const;
            void setNoteheadText( const NoteheadTextPtr& value );
            bool getHasNoteheadText() const;
            void setHasNoteheadText( const bool value );

            /* _________ Staff minOccurs = 0, maxOccurs = 1 _________ */
            StaffPtr getStaff() const;
            void setStaff( const StaffPtr& value );
            bool getHasStaff() const;
            void setHasStaff( const bool value );

            /* _________ Beam minOccurs = 0, maxOccurs = 8 _________ */
            const BeamSet& getBeamSet() const;
            void addBeam( const BeamPtr& value );
            void removeBeam( const BeamSetIterConst& value );
            void clearBeamSet();
            BeamPtr getBeam( const BeamSetIterConst& setIterator ) const;

            /* _________ Notations minOccurs = 0, maxOccurs = unbounded _________ */
            const NotationsSet& getNotationsSet() const;
            void addNotations( const NotationsPtr& value );
            void removeNotations( const NotationsSetIterConst& value );
            void clearNotationsSet();
            NotationsPtr getNotations( const NotationsSetIterConst& setIterator ) const;

            /* _________ Lyric minOccurs = 0, maxOccurs = unbounded _________ */
            const LyricSet& getLyricSet() const;
            void addLyric( const LyricPtr& value );
            void removeLyric( const LyricSetIterConst& value );
            void clearLyricSet();
            LyricPtr getLyric( const LyricSetIterConst& setIterator ) const;

            /* _________ Play minOccurs = 0, maxOccurs = 1 _________ */
            PlayPtr getPlay() const;
            void setPlay( const PlayPtr& value );
            bool getHasPlay() const;
            void setHasPlay( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            MX_MUTEX
            mutable NoteAttributesPtr myAttributes;
            mutable NoteChoicePtr myNoteChoice;
            mutable InstrumentPtr myInstrument;
            bool myHasInstrument;
            mutable EditorialVoiceGroupPtr myEditorialVoiceGroup;
            mutable TypePtr myType;
            bool myHasType;
            DotSet myDotSet;
            mutable AccidentalPtr myAccidental;
            bool myHasAccidental;
            mutable TimeModificationPtr myTimeModification;
            bool myHasTimeModification;
            mutable StemPtr myStem;
            bool myHasStem;
            mutable NoteheadPtr myNotehead;
            bool myHasNotehead;
            mutable NoteheadTextPtr myNoteheadText;
            bool myHasNoteheadText;
            mutable StaffPtr myStaff;
            bool myHasStaff;
            BeamSet myBeamSet;
            NotationsSet myNotationsSet;
            LyricSet myLyricSet;
            mutable PlayPtr myPlay;
            bool myHasPlay;
            
            bool parseNoteChoice(
                std::ostream& message,
                xml::XElement& noteElement,
                xml::XElementIterator& iter );
            
            bool parseFullNoteGroup(
                std::ostream& message,
                xml::XElement& noteElement,
                xml::XElementIterator& iter,
                FullNoteGroupPtr& outFullNoteGroup );
            
            bool parseEditorialVoiceGroup(
                std::ostream& message,
                xml::XElement& noteElement,
                xml::XElementIterator& iter );
        };
    }
}
