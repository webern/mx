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

        MX_FORWARD_DECLARE_ELEMENT( NormalDot )
        MX_FORWARD_DECLARE_ELEMENT( NormalType )
        MX_FORWARD_DECLARE_ELEMENT( TimeModificationNormalTypeNormalDot )

        inline TimeModificationNormalTypeNormalDotPtr makeTimeModificationNormalTypeNormalDot() { return std::make_shared<TimeModificationNormalTypeNormalDot>(); }

        class TimeModificationNormalTypeNormalDot : public ElementInterface
        {
        public:
            TimeModificationNormalTypeNormalDot();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ NormalType minOccurs = 1, maxOccurs = 1 _________ */
            NormalTypePtr getNormalType() const;
            void setNormalType( const NormalTypePtr& value );

            /* _________ NormalDot minOccurs = 0, maxOccurs = unbounded _________ */
            const NormalDotSet& getNormalDotSet() const;
            void addNormalDot( const NormalDotPtr& value );
            void removeNormalDot( const NormalDotSetIterConst& value );
            void clearNormalDotSet();
            NormalDotPtr getNormalDot( const NormalDotSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            NormalTypePtr myNormalType;
            NormalDotSet myNormalDotSet;
        };
    }
}
