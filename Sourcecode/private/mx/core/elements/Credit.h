// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/CreditAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace ezxml
{
    class XElementIterator;
}

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( CreditAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Bookmark )
        MX_FORWARD_DECLARE_ELEMENT( CreditChoice )
        MX_FORWARD_DECLARE_ELEMENT( CreditType )
        MX_FORWARD_DECLARE_ELEMENT( Link )
        MX_FORWARD_DECLARE_ELEMENT( Credit )

        inline CreditPtr makeCredit() { return std::make_shared<Credit>(); }

        class Credit : public ElementInterface
        {
        public:
            Credit();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            CreditAttributesPtr getAttributes() const;
            void setAttributes( const CreditAttributesPtr& value );

            /* _________ CreditType minOccurs = 0, maxOccurs = unbounded _________ */
            const CreditTypeSet& getCreditTypeSet() const;
            void addCreditType( const CreditTypePtr& value );
            void removeCreditType( const CreditTypeSetIterConst& value );
            void clearCreditTypeSet();
            CreditTypePtr getCreditType( const CreditTypeSetIterConst& setIterator ) const;

            /* _________ Link minOccurs = 0, maxOccurs = unbounded _________ */
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

            /* _________ CreditChoice minOccurs = 1, maxOccurs = 1 _________ */
            CreditChoicePtr getCreditChoice() const;
            void setCreditChoice( const CreditChoicePtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            CreditAttributesPtr myAttributes;
            CreditTypeSet myCreditTypeSet;
            LinkSet myLinkSet;
            BookmarkSet myBookmarkSet;
            CreditChoicePtr myCreditChoice;

        };
    }
}
