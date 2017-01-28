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

        MX_FORWARD_DECLARE_ELEMENT( PartGroup )
        MX_FORWARD_DECLARE_ELEMENT( PartGroupOrScorePart )
        MX_FORWARD_DECLARE_ELEMENT( ScorePart )
        MX_FORWARD_DECLARE_ELEMENT( PartList )

        inline PartListPtr makePartList() { return std::make_shared<PartList>(); }

        class PartList : public ElementInterface
        {
        public:
            PartList();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ PartGroup minOccurs = 0, maxOccurs = unbounded _________ */
            const PartGroupSet& getPartGroupSet() const;
            void addPartGroup( const PartGroupPtr& value );
            void removePartGroup( const PartGroupSetIterConst& value );
            void clearPartGroupSet();
            PartGroupPtr getPartGroup( const PartGroupSetIterConst& setIterator ) const;

            /* _________ ScorePart minOccurs = 1, maxOccurs = 1 _________ */
            ScorePartPtr getScorePart() const;
            void setScorePart( const ScorePartPtr& value );

            /* _________ PartGroupOrScorePart minOccurs = 0, maxOccurs = unbounded _________ */
            const PartGroupOrScorePartSet& getPartGroupOrScorePartSet() const;
            void addPartGroupOrScorePart( const PartGroupOrScorePartPtr& value );
            void removePartGroupOrScorePart( const PartGroupOrScorePartSetIterConst& value );
            void clearPartGroupOrScorePartSet();
            PartGroupOrScorePartPtr getPartGroupOrScorePart( const PartGroupOrScorePartSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
            
        private:
            PartGroupSet myPartGroupSet;
            ScorePartPtr myScorePart;
            PartGroupOrScorePartSet myPartGroupOrScorePartSet;
        };
    }
}
