// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/RestAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( RestAttributes )
        MX_FORWARD_DECLARE_ELEMENT( DisplayStepOctaveGroup )
        MX_FORWARD_DECLARE_ELEMENT( Rest )

        inline RestPtr makeRest() { return std::make_shared<Rest>(); }

        class Rest : public ElementInterface
        {
        public:
            Rest();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RestAttributesPtr getAttributes() const;
            void setAttributes( const RestAttributesPtr& value );

            /* _________ DisplayStepOctaveGroup minOccurs = 0, maxOccurs = 1 _________ */
            DisplayStepOctaveGroupPtr getDisplayStepOctaveGroup() const;
            void setDisplayStepOctaveGroup( const DisplayStepOctaveGroupPtr& value );
            bool getHasDisplayStepOctaveGroup() const;
            void setHasDisplayStepOctaveGroup( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            RestAttributesPtr myAttributes;
            DisplayStepOctaveGroupPtr myDisplayStepOctaveGroup;
            bool myHasDisplayStepOctaveGroup;
        };
    }
}
