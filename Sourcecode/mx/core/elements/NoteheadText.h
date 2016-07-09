// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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

        MX_FORWARD_DECLARE_ELEMENT( AccidentalText )
        MX_FORWARD_DECLARE_ELEMENT( DisplayText )
        MX_FORWARD_DECLARE_ELEMENT( NoteheadTextChoice )
        MX_FORWARD_DECLARE_ELEMENT( NoteheadText )

        inline NoteheadTextPtr makeNoteheadText() { return std::make_shared<NoteheadText>(); }

        class NoteheadText : public ElementInterface
        {
        public:
            NoteheadText();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ NoteheadTextChoice minOccurs = 1, maxOccurs = unbounded _________ */
            const NoteheadTextChoiceSet& getNoteheadTextChoiceSet() const;
            void addNoteheadTextChoice( const NoteheadTextChoicePtr& value );
            void removeNoteheadTextChoice( const NoteheadTextChoiceSetIterConst& value );
            void clearNoteheadTextChoiceSet();
            NoteheadTextChoicePtr getNoteheadTextChoice( const NoteheadTextChoiceSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            NoteheadTextChoiceSet myNoteheadTextChoiceSet;
        };
    }
}
