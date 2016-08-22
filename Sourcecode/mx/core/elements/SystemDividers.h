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

        MX_FORWARD_DECLARE_ELEMENT( LeftDivider )
        MX_FORWARD_DECLARE_ELEMENT( RightDivider )
        MX_FORWARD_DECLARE_ELEMENT( SystemDividers )

        inline SystemDividersPtr makeSystemDividers() { return std::make_shared<SystemDividers>(); }

        class SystemDividers : public ElementInterface
        {
        public:
            SystemDividers();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ LeftDivider minOccurs = 1, maxOccurs = 1 _________ */
            LeftDividerPtr getLeftDivider() const;
            void setLeftDivider( const LeftDividerPtr& value );

            /* _________ RightDivider minOccurs = 1, maxOccurs = 1 _________ */
            RightDividerPtr getRightDivider() const;
            void setRightDivider( const RightDividerPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            LeftDividerPtr myLeftDivider;
            RightDividerPtr myRightDivider;
        };
    }
}
