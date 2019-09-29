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
        MX_FORWARD_DECLARE_ELEMENT( DisplayTextOrAccidentalText )

        inline DisplayTextOrAccidentalTextPtr makeDisplayTextOrAccidentalText() { return std::make_shared<DisplayTextOrAccidentalText>(); }

        class DisplayTextOrAccidentalText : public ElementInterface
        {
        public:
            enum class Choice
            {
                displayText = 1,
                accidentalText = 2
            };
            DisplayTextOrAccidentalText();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DisplayTextOrAccidentalText::Choice getChoice() const;
            void setChoice( const DisplayTextOrAccidentalText::Choice value );
            DisplayTextPtr getDisplayText() const;
            void setDisplayText( const DisplayTextPtr& value );
            AccidentalTextPtr getAccidentalText() const;
            void setAccidentalText( const AccidentalTextPtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            Choice myChoice;
            DisplayTextPtr myDisplayText;
            AccidentalTextPtr myAccidentalText;
        };
    }
}
