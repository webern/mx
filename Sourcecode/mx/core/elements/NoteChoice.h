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

        MX_FORWARD_DECLARE_ELEMENT( CueNoteGroup )
        MX_FORWARD_DECLARE_ELEMENT( GraceNoteGroup )
        MX_FORWARD_DECLARE_ELEMENT( NormalNoteGroup )
        MX_FORWARD_DECLARE_ELEMENT( NoteChoice )

        inline NoteChoicePtr makeNoteChoice() { return std::make_shared<NoteChoice>(); }

        class NoteChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                grace = 1,
                cue = 2,
                normal = 3
            };
            NoteChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            NoteChoice::Choice getChoice() const;
            void setChoice( const NoteChoice::Choice value );

            /* _________ GraceNoteGroup minOccurs = 1, maxOccurs = 1 _________ */
            GraceNoteGroupPtr getGraceNoteGroup() const;
            void setGraceNoteGroup( const GraceNoteGroupPtr& value );

            /* _________ CueNoteGroup minOccurs = 1, maxOccurs = 1 _________ */
            CueNoteGroupPtr getCueNoteGroup() const;
            void setCueNoteGroup( const CueNoteGroupPtr& value );

            /* _________ NormalNoteGroup minOccurs = 1, maxOccurs = 1 _________ */
            NormalNoteGroupPtr getNormalNoteGroup() const;
            void setNormalNoteGroup( const NormalNoteGroupPtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            GraceNoteGroupPtr myGraceNoteGroup;
            CueNoteGroupPtr myCueNoteGroup;
            NormalNoteGroupPtr myNormalNoteGroup;
        };
    }
}
