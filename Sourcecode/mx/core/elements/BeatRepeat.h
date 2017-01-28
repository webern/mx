// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/BeatRepeatAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( BeatRepeatAttributes )
        MX_FORWARD_DECLARE_ELEMENT( SlashDot )
        MX_FORWARD_DECLARE_ELEMENT( SlashType )
        MX_FORWARD_DECLARE_ELEMENT( BeatRepeat )

        inline BeatRepeatPtr makeBeatRepeat() { return std::make_shared<BeatRepeat>(); }

        class BeatRepeat : public ElementInterface
        {
        public:
            BeatRepeat();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BeatRepeatAttributesPtr getAttributes() const;
            void setAttributes( const BeatRepeatAttributesPtr& value );

            /* _________ SlashType minOccurs = 1, maxOccurs = 1 _________ */
            SlashTypePtr getSlashType() const;
            void setSlashType( const SlashTypePtr& value );

            /* _________ SlashDot minOccurs = 0, maxOccurs = unbounded _________ */
            const SlashDotSet& getSlashDotSet() const;
            void addSlashDot( const SlashDotPtr& value );
            void removeSlashDot( const SlashDotSetIterConst& value );
            void clearSlashDotSet();

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            BeatRepeatAttributesPtr myAttributes;
            SlashTypePtr mySlashType;
            SlashDotSet mySlashDotSet;
        };
    }
}
