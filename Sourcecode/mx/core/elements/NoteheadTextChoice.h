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
        
        MX_FORWARD_DECLARE_ELEMENT( AccidentalText )
        MX_FORWARD_DECLARE_ELEMENT( DisplayText )
        MX_FORWARD_DECLARE_ELEMENT( NoteheadTextChoice )
        
        inline NoteheadTextChoicePtr makeNoteheadTextChoice() { return std::make_shared<NoteheadTextChoice>(); }
        
        class NoteheadTextChoice : public ElementInterface
        {
        public:
            
            enum class Choice
            {
                displayText = 0,
                accidentalText = 1
            };
            
            NoteheadTextChoice();
            
            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            
            NoteheadTextChoice::Choice getChoice() const;
            void setChoice( NoteheadTextChoice::Choice value );
            
            /* _________ DisplayText minOccurs = 1, maxOccurs = 1 _________ */
            DisplayTextPtr getDisplayText() const;
            void setDisplayText( const DisplayTextPtr& value );
            
            /* _________ AccidentalText minOccurs = 1, maxOccurs = 1 _________ */
            AccidentalTextPtr getAccidentalText() const;
            void setAccidentalText( const AccidentalTextPtr& value );
            
            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );
            
        private:
            Choice myChoice;
            DisplayTextPtr myDisplayText;
            AccidentalTextPtr myAccidentalText;
        };
    }
}
