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

        MX_FORWARD_DECLARE_ELEMENT( PageHeight )
        MX_FORWARD_DECLARE_ELEMENT( PageMargins )
        MX_FORWARD_DECLARE_ELEMENT( PageWidth )
        MX_FORWARD_DECLARE_ELEMENT( PageLayout )

        inline PageLayoutPtr makePageLayout() { return std::make_shared<PageLayout>(); }

        class PageLayout : public ElementInterface
        {
        public:
            PageLayout();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ PageHeight minOccurs = 1, maxOccurs = 1 _________ */
            PageHeightPtr getPageHeight() const;
            void setPageHeight( const PageHeightPtr& value );

            /* _________ PageWidth minOccurs = 1, maxOccurs = 1 _________ */
            PageWidthPtr getPageWidth() const;
            void setPageWidth( const PageWidthPtr& value );

            /* _________ PageMargins minOccurs = 0, maxOccurs = 2 _________ */
            const PageMarginsSet& getPageMarginsSet() const;
            void addPageMargins( const PageMarginsPtr& value );
            void removePageMargins( const PageMarginsSetIterConst& value );
            void clearPageMarginsSet();
            PageMarginsPtr getPageMargins( const PageMarginsSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            PageHeightPtr myPageHeight;
            PageWidthPtr myPageWidth;
            PageMarginsSet myPageMarginsSet;
        };
    }
}
