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

        MX_FORWARD_DECLARE_ELEMENT( Cancel )
        MX_FORWARD_DECLARE_ELEMENT( Fifths )
        MX_FORWARD_DECLARE_ELEMENT( Mode )
        MX_FORWARD_DECLARE_ELEMENT( TraditionalKey )

        inline TraditionalKeyPtr makeTraditionalKey() { return std::make_shared<TraditionalKey>(); }

        class TraditionalKey : public ElementInterface
        {
        public:
            TraditionalKey();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            CancelPtr getCancel() const;
            void setCancel( const CancelPtr& value );
            bool getHasCancel() const;
            void setHasCancel( const bool value );
            FifthsPtr getFifths() const;
            void setFifths( const FifthsPtr& value );
            ModePtr getMode() const;
            void setMode( const ModePtr& value );
            bool getHasMode() const;
            void setHasMode( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            CancelPtr myCancel;
            FifthsPtr myFifths;
            ModePtr myMode;
            bool myHasCancel;
            bool myHasMode;
        };
    }
}
