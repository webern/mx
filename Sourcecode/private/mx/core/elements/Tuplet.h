// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/TupletAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( TupletAttributes )
        MX_FORWARD_DECLARE_ELEMENT( TupletActual )
        MX_FORWARD_DECLARE_ELEMENT( TupletNormal )
        MX_FORWARD_DECLARE_ELEMENT( Tuplet )

        inline TupletPtr makeTuplet() { return std::make_shared<Tuplet>(); }

        class Tuplet : public ElementInterface
        {
        public:
            Tuplet();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TupletAttributesPtr getAttributes() const;
            void setAttributes( const TupletAttributesPtr& value );

            /* _________ TupletActual minOccurs = 0, maxOccurs = 1 _________ */
            TupletActualPtr getTupletActual() const;
            void setTupletActual( const TupletActualPtr& value );
            bool getHasTupletActual() const;
            void setHasTupletActual( const bool value );

            /* _________ TupletNormal minOccurs = 0, maxOccurs = 1 _________ */
            TupletNormalPtr getTupletNormal() const;
            void setTupletNormal( const TupletNormalPtr& value );
            bool getHasTupletNormal() const;
            void setHasTupletNormal( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            TupletAttributesPtr myAttributes;
            TupletActualPtr myTupletActual;
            bool myHasTupletActual;
            TupletNormalPtr myTupletNormal;
            bool myHasTupletNormal;
        };
    }
}
