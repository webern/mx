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

        MX_FORWARD_DECLARE_ELEMENT( Credit )
        MX_FORWARD_DECLARE_ELEMENT( Defaults )
        MX_FORWARD_DECLARE_ELEMENT( Identification )
        MX_FORWARD_DECLARE_ELEMENT( MovementNumber )
        MX_FORWARD_DECLARE_ELEMENT( MovementTitle )
        MX_FORWARD_DECLARE_ELEMENT( PartList )
        MX_FORWARD_DECLARE_ELEMENT( Work )
        MX_FORWARD_DECLARE_ELEMENT( ScoreHeaderGroup )

        inline ScoreHeaderGroupPtr makeScoreHeaderGroup() { return std::make_shared<ScoreHeaderGroup>(); }

        class ScoreHeaderGroup : public ElementInterface
        {
        public:
            ScoreHeaderGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Work minOccurs = 0, maxOccurs = 1 _________ */
            WorkPtr getWork() const;
            void setWork( const WorkPtr& value );
            bool getHasWork() const;
            void setHasWork( const bool value );

            /* _________ MovementNumber minOccurs = 0, maxOccurs = 1 _________ */
            MovementNumberPtr getMovementNumber() const;
            void setMovementNumber( const MovementNumberPtr& value );
            bool getHasMovementNumber() const;
            void setHasMovementNumber( const bool value );

            /* _________ MovementTitle minOccurs = 0, maxOccurs = 1 _________ */
            MovementTitlePtr getMovementTitle() const;
            void setMovementTitle( const MovementTitlePtr& value );
            bool getHasMovementTitle() const;
            void setHasMovementTitle( const bool value );

            /* _________ Identification minOccurs = 0, maxOccurs = 1 _________ */
            IdentificationPtr getIdentification() const;
            void setIdentification( const IdentificationPtr& value );
            bool getHasIdentification() const;
            void setHasIdentification( const bool value );

            /* _________ Defaults minOccurs = 0, maxOccurs = 1 _________ */
            DefaultsPtr getDefaults() const;
            void setDefaults( const DefaultsPtr& value );
            bool getHasDefaults() const;
            void setHasDefaults( const bool value );

            /* _________ Credit minOccurs = 0, maxOccurs = unbounded _________ */
            const CreditSet& getCreditSet() const;
            void addCredit( const CreditPtr& value );
            void removeCredit( const CreditSetIterConst& value );
            void clearCreditSet();
            CreditPtr getCredit( const CreditSetIterConst& setIterator ) const;

            /* _________ PartList minOccurs = 1, maxOccurs = 1 _________ */
            PartListPtr getPartList() const;
            void setPartList( const PartListPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            WorkPtr myWork;
            bool myHasWork;
            MovementNumberPtr myMovementNumber;
            bool myHasMovementNumber;
            MovementTitlePtr myMovementTitle;
            bool myHasMovementTitle;
            IdentificationPtr myIdentification;
            bool myHasIdentification;
            DefaultsPtr myDefaults;
            bool myHasDefaults;
            CreditSet myCreditSet;
            PartListPtr myPartList;
        };
    }
}
