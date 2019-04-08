// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/BendAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( BendAttributes )
        MX_FORWARD_DECLARE_ELEMENT( BendAlter )
        MX_FORWARD_DECLARE_ELEMENT( BendChoice )
        MX_FORWARD_DECLARE_ELEMENT( WithBar )
        MX_FORWARD_DECLARE_ELEMENT( Bend )

        inline BendPtr makeBend() { return std::make_shared<Bend>(); }

        class Bend : public ElementInterface
        {
        public:
            Bend();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BendAttributesPtr getAttributes() const;
            void setAttributes( const BendAttributesPtr& value );

            /* _________ BendAlter minOccurs = 1, maxOccurs = 1 _________ */
            BendAlterPtr getBendAlter() const;
            void setBendAlter( const BendAlterPtr& value );

            /* _________ BendChoice minOccurs = 0, maxOccurs = 1 _________ */
            BendChoicePtr getBendChoice() const;
            void setBendChoice( const BendChoicePtr& value );
            bool getHasBendChoice() const;
            void setHasBendChoice( const bool value );

            /* _________ WithBar minOccurs = 0, maxOccurs = 1 _________ */
            WithBarPtr getWithBar() const;
            void setWithBar( const WithBarPtr& value );
            bool getHasWithBar() const;
            void setHasWithBar( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            BendAttributesPtr myAttributes;
            BendAlterPtr myBendAlter;
            BendChoicePtr myBendChoice;
            bool myHasBendChoice;
            WithBarPtr myWithBar;
            bool myHasWithBar;
        };
    }
}
