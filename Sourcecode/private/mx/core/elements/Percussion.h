// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/PercussionAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PercussionAttributes )
        MX_FORWARD_DECLARE_ELEMENT( PercussionChoice )
        MX_FORWARD_DECLARE_ELEMENT( Percussion )

        inline PercussionPtr makePercussion() { return std::make_shared<Percussion>(); }

        class Percussion : public ElementInterface
        {
        public:
            Percussion();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PercussionAttributesPtr getAttributes() const;
            void setAttributes( const PercussionAttributesPtr& value );
            PercussionChoicePtr getPercussionChoice() const;
            void setPercussionChoice( const PercussionChoicePtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            PercussionAttributesPtr myAttributes;
            PercussionChoicePtr myChoice;
        };
    }
}
