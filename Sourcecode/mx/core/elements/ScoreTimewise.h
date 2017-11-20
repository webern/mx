// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/ScoreTimewiseAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( ScoreTimewiseAttributes )
        MX_FORWARD_DECLARE_ELEMENT( ScoreHeaderGroup )
        MX_FORWARD_DECLARE_ELEMENT( TimewiseMeasure )
        MX_FORWARD_DECLARE_ELEMENT( ScoreTimewise )

        inline ScoreTimewisePtr makeScoreTimewise() { return std::make_shared<ScoreTimewise>(); }

        class ScoreTimewise : public ElementInterface
        {
        public:
            ScoreTimewise();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ScoreTimewiseAttributesPtr getAttributes() const;
            void setAttributes( const ScoreTimewiseAttributesPtr& value );

            /* _________ ScoreHeaderGroup minOccurs = 1, maxOccurs = 1 _________ */
            ScoreHeaderGroupPtr getScoreHeaderGroup() const;
            void setScoreHeaderGroup( const ScoreHeaderGroupPtr& value );

            /* _________ TimewiseMeasure minOccurs = 1, maxOccurs = unbounded _________ */
            const TimewiseMeasureSet& getTimewiseMeasureSet() const;
            void addTimewiseMeasure( const TimewiseMeasurePtr& value );
            void removeTimewiseMeasure( const TimewiseMeasureSetIterConst& value );
            void clearTimewiseMeasureSet();
            TimewiseMeasurePtr getTimewiseMeasure( const TimewiseMeasureSetIterConst& setIterator ) const;
            
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            ScoreTimewiseAttributesPtr myAttributes;
            ScoreHeaderGroupPtr myScoreHeaderGroup;
            TimewiseMeasureSet myTimewiseMeasureSet;
        };
    }
}
