// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/MetronomeTupletAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( MetronomeTupletAttributes )
        MX_FORWARD_DECLARE_ELEMENT( ActualNotes )
        MX_FORWARD_DECLARE_ELEMENT( NormalNotes )
        MX_FORWARD_DECLARE_ELEMENT( TimeModificationNormalTypeNormalDot )
        MX_FORWARD_DECLARE_ELEMENT( MetronomeTuplet )

        inline MetronomeTupletPtr makeMetronomeTuplet() { return std::make_shared<MetronomeTuplet>(); }
        

        class MetronomeTuplet : public ElementInterface
        {
        public:
            MetronomeTuplet();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MetronomeTupletAttributesPtr getAttributes() const;
            void setAttributes( const MetronomeTupletAttributesPtr& value );

            /* _________ ActualNotes minOccurs = 1, maxOccurs = 1 _________ */
            ActualNotesPtr getActualNotes() const;
            void setActualNotes( const ActualNotesPtr& value );

            /* _________ NormalNotes minOccurs = 1, maxOccurs = 1 _________ */
            NormalNotesPtr getNormalNotes() const;
            void setNormalNotes( const NormalNotesPtr& value );

            /* _________ TimeModificationNormalTypeNormalDot minOccurs = 0, maxOccurs = 1 _________ */
            TimeModificationNormalTypeNormalDotPtr getTimeModificationNormalTypeNormalDot() const;
            void setTimeModificationNormalTypeNormalDot( const TimeModificationNormalTypeNormalDotPtr& value );
            bool getHasTimeModificationNormalTypeNormalDot() const;
            void setHasTimeModificationNormalTypeNormalDot( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            MetronomeTupletAttributesPtr myAttributes;
            ActualNotesPtr myActualNotes;
            NormalNotesPtr myNormalNotes;
            TimeModificationNormalTypeNormalDotPtr myTimeModificationNormalTypeNormalDot;
            bool myHasTimeModificationNormalTypeNormalDot;
        };
    }
}
