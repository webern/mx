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

        MX_FORWARD_DECLARE_ELEMENT( Bookmark )
        MX_FORWARD_DECLARE_ELEMENT( CreditWords )
        MX_FORWARD_DECLARE_ELEMENT( Link )
        MX_FORWARD_DECLARE_ELEMENT( CreditWordsGroup )

        inline CreditWordsGroupPtr makeCreditWordsGroup() { return std::make_shared<CreditWordsGroup>(); }

        class CreditWordsGroup : public ElementInterface
        {
        public:
            CreditWordsGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ LinkSet minOccurs = 0, maxOccurs = unbounded _________ */
            const LinkSet& getLinkSet() const;
            void addLink( const LinkPtr& value );
            void removeLink( const LinkSetIterConst& value );
            void clearLinkSet();
            LinkPtr getLink( const LinkSetIterConst& setIterator ) const;

            /* _________ Bookmark minOccurs = 0, maxOccurs = unbounded _________ */
            const BookmarkSet& getBookmarkSet() const;
            void addBookmark( const BookmarkPtr& value );
            void removeBookmark( const BookmarkSetIterConst& value );
            void clearBookmarkSet();
            BookmarkPtr getBookmark( const BookmarkSetIterConst& setIterator ) const;

            /* _________ CreditWords minOccurs = 1, maxOccurs = 1 _________ */
            CreditWordsPtr getCreditWords() const;
            void setCreditWords( const CreditWordsPtr& value );
            
            bool fromXElement( std::ostream& message, xml::XElement& xelement );
            
        private:
            LinkSet myLinkSet;
            BookmarkSet myBookmarkSet;
            CreditWordsPtr myCreditWords;
        };
    }
}
