// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/SlashAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( SlashAttributes )
        MX_FORWARD_DECLARE_ELEMENT( SlashDot )
        MX_FORWARD_DECLARE_ELEMENT( SlashType )
        MX_FORWARD_DECLARE_ELEMENT( Slash )

        inline SlashPtr makeSlash() { return std::make_shared<Slash>(); }

        class Slash : public ElementInterface
        {
        public:
            Slash();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            SlashAttributesPtr getAttributes() const;
            void setAttributes( const SlashAttributesPtr& value );

            /* _________ SlashType minOccurs = 1, maxOccurs = 1 _________ */
            SlashTypePtr getSlashType() const;
            void setSlashType( const SlashTypePtr& value );

            /* _________ SlashDot minOccurs = 0, maxOccurs = unbounded _________ */
            const SlashDotSet& getSlashDotSet() const;
            void addSlashDot( const SlashDotPtr& value );
            void removeSlashDot( const SlashDotSetIterConst& value );
            void clearSlashDotSet();

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            SlashAttributesPtr myAttributes;
            SlashTypePtr mySlashType;
            SlashDotSet mySlashDotSet;
        };
    }
}
