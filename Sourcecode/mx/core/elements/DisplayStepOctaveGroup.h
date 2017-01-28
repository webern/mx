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

        MX_FORWARD_DECLARE_ELEMENT( DisplayOctave )
        MX_FORWARD_DECLARE_ELEMENT( DisplayStep )
        MX_FORWARD_DECLARE_ELEMENT( DisplayStepOctaveGroup )

        inline DisplayStepOctaveGroupPtr makeDisplayStepOctaveGroup() { return std::make_shared<DisplayStepOctaveGroup>(); }

        class DisplayStepOctaveGroup : public ElementInterface
        {
        public:
            DisplayStepOctaveGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ DisplayStep minOccurs = 1, maxOccurs = 1 _________ */
            DisplayStepPtr getDisplayStep() const;
            void setDisplayStep( const DisplayStepPtr& value );

            /* _________ DisplayOctave minOccurs = 1, maxOccurs = 1 _________ */
            DisplayOctavePtr getDisplayOctave() const;
            void setDisplayOctave( const DisplayOctavePtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            DisplayStepPtr myDisplayStep;
            DisplayOctavePtr myDisplayOctave;
        };
    }
}
