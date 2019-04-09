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

        MX_FORWARD_DECLARE_ELEMENT( PreBend )
        MX_FORWARD_DECLARE_ELEMENT( Release )
        MX_FORWARD_DECLARE_ELEMENT( BendChoice )

        inline BendChoicePtr makeBendChoice() { return std::make_shared<BendChoice>(); }

        class BendChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                preBend = 1,
                release = 2
            };
            BendChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BendChoice::Choice getChoice() const;
            void setChoice( BendChoice::Choice value );

            /* _________ PreBend minOccurs = 1, maxOccurs = 1 _________ */
            PreBendPtr getPreBend() const;
            void setPreBend( const PreBendPtr& value );

            /* _________ Release minOccurs = 1, maxOccurs = 1 _________ */
            ReleasePtr getRelease() const;
            void setRelease( const ReleasePtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            PreBendPtr myPreBend;
            ReleasePtr myRelease;
        };
    }
}
