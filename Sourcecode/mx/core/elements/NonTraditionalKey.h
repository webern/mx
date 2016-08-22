// MusicXML Class Library v0.3.0
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

        MX_FORWARD_DECLARE_ELEMENT( KeyAccidental )
        MX_FORWARD_DECLARE_ELEMENT( KeyAlter )
        MX_FORWARD_DECLARE_ELEMENT( KeyStep )
        MX_FORWARD_DECLARE_ELEMENT( NonTraditionalKey )

        inline NonTraditionalKeyPtr makeNonTraditionalKey() { return std::make_shared<NonTraditionalKey>(); }

        class NonTraditionalKey : public ElementInterface
        {
        public:
            NonTraditionalKey();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            KeyStepPtr getKeyStep() const;
            void setKeyStep( const KeyStepPtr& value );
            KeyAlterPtr getKeyAlter() const;
            void setKeyAlter( const KeyAlterPtr& value );
            KeyAccidentalPtr getKeyAccidental() const;
            void setKeyAccidental( const KeyAccidentalPtr& value );
            bool getHasKeyAccidental() const;
            void setHasKeyAccidental( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            KeyStepPtr myKeyStep;
            KeyAlterPtr myKeyAlter;
            KeyAccidentalPtr myKeyAccidental;
            bool myHasKeyAccidental;
        };
    }
}
