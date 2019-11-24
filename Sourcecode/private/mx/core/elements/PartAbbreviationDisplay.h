// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/PartAbbreviationDisplayAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PartAbbreviationDisplayAttributes )
        MX_FORWARD_DECLARE_ELEMENT( DisplayTextOrAccidentalText )
        MX_FORWARD_DECLARE_ELEMENT( PartAbbreviationDisplay )

        inline PartAbbreviationDisplayPtr makePartAbbreviationDisplay() { return std::make_shared<PartAbbreviationDisplay>(); }

        class PartAbbreviationDisplay : public ElementInterface
        {
        public:
            PartAbbreviationDisplay();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PartAbbreviationDisplayAttributesPtr getAttributes() const;
            void setAttributes( const PartAbbreviationDisplayAttributesPtr& value );

            /* _________ DisplayTextOrAccidentalText minOccurs = 0, maxOccurs = unbounded _________ */
            const DisplayTextOrAccidentalTextSet& getDisplayTextOrAccidentalText() const;
            void addDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextPtr& value );
            void removeDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextSetIterConst& setIterator );
            void clearDisplayTextOrAccidentalTextSet();
            DisplayTextOrAccidentalTextPtr getDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextSetIterConst& setIterator ) const;
            const DisplayTextOrAccidentalTextSet& getDisplayTextOrAccidentalTextSet() const;
            
            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            PartAbbreviationDisplayAttributesPtr myAttributes;
            DisplayTextOrAccidentalTextSet myDisplayTextOrAccidentalTextSet;
        };
    }
}
