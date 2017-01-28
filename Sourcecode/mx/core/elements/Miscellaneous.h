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

        MX_FORWARD_DECLARE_ELEMENT( MiscellaneousField )
        MX_FORWARD_DECLARE_ELEMENT( Miscellaneous )

        inline MiscellaneousPtr makeMiscellaneous() { return std::make_shared<Miscellaneous>(); }

        class Miscellaneous : public ElementInterface
        {
        public:
            Miscellaneous();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ MiscellaneousField minOccurs = 0, maxOccurs = unbounded _________ */
            const MiscellaneousFieldSet& getMiscellaneousFieldSet() const;
            void addMiscellaneousField( const MiscellaneousFieldPtr& value );
            void removeMiscellaneousField( const MiscellaneousFieldSetIterConst& value );
            void clearMiscellaneousFieldSet();
            MiscellaneousFieldPtr getMiscellaneousField( const MiscellaneousFieldSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            MiscellaneousFieldSet myMiscellaneousFieldSet;
        };
    }
}
