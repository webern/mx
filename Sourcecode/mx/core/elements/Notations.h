// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/NotationsAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( NotationsAttributes )
        MX_FORWARD_DECLARE_ELEMENT( EditorialGroup )
        MX_FORWARD_DECLARE_ELEMENT( NotationsChoice )
        MX_FORWARD_DECLARE_ELEMENT( Notations )

        inline NotationsPtr makeNotations() { return std::make_shared<Notations>(); }

        class Notations : public ElementInterface
        {
        public:
            Notations();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NotationsAttributesPtr getAttributes() const;
            void setAttributes( const NotationsAttributesPtr& value );

            /* _________ EditorialGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialGroupPtr getEditorialGroup() const;
            void setEditorialGroup( const EditorialGroupPtr& value );

            /* _________ NotationsChoice minOccurs = 0, maxOccurs = unbounded _________ */
            const NotationsChoiceSet& getNotationsChoiceSet() const;
            void addNotationsChoice( const NotationsChoicePtr& value );
            void removeNotationsChoice( const NotationsChoiceSetIterConst& value );
            void clearNotationsChoiceSet();
            NotationsChoicePtr getNotationsChoice( const NotationsChoiceSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            NotationsAttributesPtr myAttributes;
            EditorialGroupPtr myEditorialGroup;
            NotationsChoiceSet myNotationsChoiceSet;
        };
    }
}
