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

        MX_FORWARD_DECLARE_ELEMENT( LeftMargin )
        MX_FORWARD_DECLARE_ELEMENT( RightMargin )
        MX_FORWARD_DECLARE_ELEMENT( SystemMargins )

        inline SystemMarginsPtr makeSystemMargins() { return std::make_shared<SystemMargins>(); }

        class SystemMargins : public ElementInterface
        {
        public:
            SystemMargins();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ LeftMargin minOccurs = 1, maxOccurs = 1 _________ */
            LeftMarginPtr getLeftMargin() const;
            void setLeftMargin( const LeftMarginPtr& value );

            /* _________ RightMargin minOccurs = 1, maxOccurs = 1 _________ */
            RightMarginPtr getRightMargin() const;
            void setRightMargin( const RightMarginPtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            LeftMarginPtr myLeftMargin;
            RightMarginPtr myRightMargin;
        };
    }
}
