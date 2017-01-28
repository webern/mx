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

        MX_FORWARD_DECLARE_ELEMENT( VirtualLibrary )
        MX_FORWARD_DECLARE_ELEMENT( VirtualName )
        MX_FORWARD_DECLARE_ELEMENT( VirtualInstrument )

        inline VirtualInstrumentPtr makeVirtualInstrument() { return std::make_shared<VirtualInstrument>(); }

        class VirtualInstrument : public ElementInterface
        {
        public:
            VirtualInstrument();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ VirtualLibrary minOccurs = 0, maxOccurs = 1 _________ */
            VirtualLibraryPtr getVirtualLibrary() const;
            void setVirtualLibrary( const VirtualLibraryPtr& value );
            bool getHasVirtualLibrary() const;
            void setHasVirtualLibrary( const bool value );

            /* _________ VirtualName minOccurs = 0, maxOccurs = 1 _________ */
            VirtualNamePtr getVirtualName() const;
            void setVirtualName( const VirtualNamePtr& value );
            bool getHasVirtualName() const;
            void setHasVirtualName( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            VirtualLibraryPtr myVirtualLibrary;
            bool myHasVirtualLibrary;
            VirtualNamePtr myVirtualName;
            bool myHasVirtualName;
        };
    }
}
