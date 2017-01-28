// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/OtherArticulationAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( OtherArticulationAttributes )
        MX_FORWARD_DECLARE_ELEMENT( OtherArticulation )

        inline OtherArticulationPtr makeOtherArticulation() { return std::make_shared<OtherArticulation>(); }
		inline OtherArticulationPtr makeOtherArticulation( const XsString& value ) { return std::make_shared<OtherArticulation>( value ); }
		inline OtherArticulationPtr makeOtherArticulation( XsString&& value ) { return std::make_shared<OtherArticulation>( std::move( value ) ); }

        class OtherArticulation : public ElementInterface
        {
        public:
            OtherArticulation();
            OtherArticulation( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OtherArticulationAttributesPtr getAttributes() const;
            void setAttributes( const OtherArticulationAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            OtherArticulationAttributesPtr myAttributes;
        };
    }
}
