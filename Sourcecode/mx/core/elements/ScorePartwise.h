// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/ScorePartwiseAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( ScorePartwiseAttributes )
        MX_FORWARD_DECLARE_ELEMENT( PartwisePart )
        MX_FORWARD_DECLARE_ELEMENT( ScoreHeaderGroup )
        MX_FORWARD_DECLARE_ELEMENT( ScorePartwise )

        inline ScorePartwisePtr makeScorePartwise() { return std::make_shared<ScorePartwise>(); }

        class ScorePartwise : public ElementInterface
        {
        public:
            ScorePartwise();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ScorePartwiseAttributesPtr getAttributes() const;
            void setAttributes( const ScorePartwiseAttributesPtr& value );

            /* _________ ScoreHeaderGroup minOccurs = 1, maxOccurs = 1 _________ */
            ScoreHeaderGroupPtr getScoreHeaderGroup() const;
            void setScoreHeaderGroup( const ScoreHeaderGroupPtr& value );

            /* _________ PartwisePart minOccurs = 1, maxOccurs = unbounded _________ */
            const PartwisePartSet& getPartwisePartSet() const;
            void addPartwisePart( const PartwisePartPtr& value );
            void removePartwisePart( const PartwisePartSetIterConst& value );
            void clearPartwisePartSet();
            PartwisePartPtr getPartwisePart( const PartwisePartSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
            
        private:
            ScorePartwiseAttributesPtr myAttributes;
            ScoreHeaderGroupPtr myScoreHeaderGroup;
            PartwisePartSet myPartwisePartSet;
        };
    }
}
