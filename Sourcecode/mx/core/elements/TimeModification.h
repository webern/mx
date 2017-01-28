// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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

        MX_FORWARD_DECLARE_ELEMENT( ActualNotes )
        MX_FORWARD_DECLARE_ELEMENT( NormalNotes )
        MX_FORWARD_DECLARE_ELEMENT( NormalTypeNormalDotGroup )
        MX_FORWARD_DECLARE_ELEMENT( TimeModification )

        inline TimeModificationPtr makeTimeModification() { return std::make_shared<TimeModification>(); }

        class TimeModification : public ElementInterface
        {
        public:
            TimeModification();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ ActualNotes minOccurs = 1, maxOccurs = 1 _________ */
            ActualNotesPtr getActualNotes() const;
            void setActualNotes( const ActualNotesPtr& value );

            /* _________ NormalNotes minOccurs = 1, maxOccurs = 1 _________ */
            NormalNotesPtr getNormalNotes() const;
            void setNormalNotes( const NormalNotesPtr& value );

            /* _________ NormalTypeNormalDotGroup minOccurs = 0, maxOccurs = 1 _________ */
            NormalTypeNormalDotGroupPtr getNormalTypeNormalDotGroup() const;
            void setNormalTypeNormalDotGroup( const NormalTypeNormalDotGroupPtr& value );
            bool getHasNormalTypeNormalDotGroup() const;
            void setHasNormalTypeNormalDotGroup( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            ActualNotesPtr myActualNotes;
            NormalNotesPtr myNormalNotes;
            NormalTypeNormalDotGroupPtr myNormalTypeNormalDotGroup;
            bool myHasNormalTypeNormalDotGroup;
        };
    }
}
