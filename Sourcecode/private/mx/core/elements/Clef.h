// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/ClefAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( ClefAttributes )
        MX_FORWARD_DECLARE_ELEMENT( ClefOctaveChange )
        MX_FORWARD_DECLARE_ELEMENT( Line )
        MX_FORWARD_DECLARE_ELEMENT( Sign )
        MX_FORWARD_DECLARE_ELEMENT( Clef )

        inline ClefPtr makeClef() { return std::make_shared<Clef>(); }

        class Clef : public ElementInterface
        {
        public:
            Clef();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ClefAttributesPtr getAttributes() const;
            void setAttributes( const ClefAttributesPtr& value );

            /* _________ Sign minOccurs = 1, maxOccurs = 1 _________ */
            SignPtr getSign() const;
            void setSign( const SignPtr& value );

            /* _________ Line minOccurs = 0, maxOccurs = 1 _________ */
            LinePtr getLine() const;
            void setLine( const LinePtr& value );
            bool getHasLine() const;
            void setHasLine( const bool value );

            /* _________ ClefOctaveChange minOccurs = 0, maxOccurs = 1 _________ */
            ClefOctaveChangePtr getClefOctaveChange() const;
            void setClefOctaveChange( const ClefOctaveChangePtr& value );
            bool getHasClefOctaveChange() const;
            void setHasClefOctaveChange( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            ClefAttributesPtr myAttributes;
            SignPtr mySign;
            LinePtr myLine;
            bool myHasLine;
            ClefOctaveChangePtr myClefOctaveChange;
            bool myHasClefOctaveChange;
        };
    }
}
