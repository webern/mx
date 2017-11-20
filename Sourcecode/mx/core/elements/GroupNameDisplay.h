// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/GroupNameDisplayAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( GroupNameDisplayAttributes )
        MX_FORWARD_DECLARE_ELEMENT( DisplayTextOrAccidentalText )
        MX_FORWARD_DECLARE_ELEMENT( GroupNameDisplay )

        inline GroupNameDisplayPtr makeGroupNameDisplay() { return std::make_shared<GroupNameDisplay>(); }

        class GroupNameDisplay : public ElementInterface
        {
        public:

            GroupNameDisplay();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            GroupNameDisplayAttributesPtr getAttributes() const;
            void setAttributes( const GroupNameDisplayAttributesPtr& value );

            /* _________ DisplayTextOrAccidentalText minOccurs = 0, maxOccurs = unbounded _________ */
            const DisplayTextOrAccidentalTextSet& getDisplayTextOrAccidentalText() const;
            void addDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextPtr& value );
            void removeDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextSetIterConst& setIterator );
            void clearDisplayTextOrAccidentalTextSet();
            DisplayTextOrAccidentalTextPtr getDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextSetIterConst& setIterator ) const;
            const DisplayTextOrAccidentalTextSet& getDisplayTextOrAccidentalTextSet() const;

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            GroupNameDisplayAttributesPtr myAttributes;
            DisplayTextOrAccidentalTextSet myDisplayTextOrAccidentalTextSet;
            
        };
    }
}
