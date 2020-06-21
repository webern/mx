// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/PageMarginsAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PageMarginsAttributes )
        MX_FORWARD_DECLARE_ELEMENT( BottomMargin )
        MX_FORWARD_DECLARE_ELEMENT( LeftMargin )
        MX_FORWARD_DECLARE_ELEMENT( RightMargin )
        MX_FORWARD_DECLARE_ELEMENT( TopMargin )
        MX_FORWARD_DECLARE_ELEMENT( PageMargins )

        inline PageMarginsPtr makePageMargins() { return std::make_shared<PageMargins>(); }

        class PageMargins : public ElementInterface
        {
        public:
            PageMargins();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PageMarginsAttributesPtr getAttributes() const;
            void setAttributes( const PageMarginsAttributesPtr& value );

            /* _________ LeftMargin minOccurs = 1, maxOccurs = 1 _________ */
            LeftMarginPtr getLeftMargin() const;
            void setLeftMargin( const LeftMarginPtr& value );

            /* _________ RightMargin minOccurs = 1, maxOccurs = 1 _________ */
            RightMarginPtr getRightMargin() const;
            void setRightMargin( const RightMarginPtr& value );

            /* _________ TopMargin minOccurs = 1, maxOccurs = 1 _________ */
            TopMarginPtr getTopMargin() const;
            void setTopMargin( const TopMarginPtr& value );

            /* _________ BottomMargin minOccurs = 1, maxOccurs = 1 _________ */
            BottomMarginPtr getBottomMargin() const;
            void setBottomMargin( const BottomMarginPtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            PageMarginsAttributesPtr myAttributes;
            LeftMarginPtr myLeftMargin;
            RightMarginPtr myRightMargin;
            TopMarginPtr myTopMargin;
            BottomMarginPtr myBottomMargin;
        };
    }
}
