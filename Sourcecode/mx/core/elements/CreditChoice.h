// MusicXML Class Library v0.2
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

        MX_FORWARD_DECLARE_ELEMENT( CreditImage )
        MX_FORWARD_DECLARE_ELEMENT( CreditWordsGroup )
        MX_FORWARD_DECLARE_ELEMENT( CreditWords )
        MX_FORWARD_DECLARE_ELEMENT( CreditChoice )

        inline CreditChoicePtr makeCreditChoice() { return std::make_shared<CreditChoice>(); }

        class CreditChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                creditImage = 1,
                creditWords = 2
            };
            CreditChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            CreditChoice::Choice getChoice() const;
            void setChoice( const CreditChoice::Choice value );

            /* _________ CreditImage minOccurs = 1, maxOccurs = 1 _________ */
            CreditImagePtr getCreditImage() const;
            void setCreditImage( const CreditImagePtr& value );

            /* _________ CreditWords minOccurs = 1, maxOccurs = 1 _________ */
            CreditWordsPtr getCreditWords() const;
            void setCreditWords( const CreditWordsPtr& value );

            /* _________ CreditWordsGroup minOccurs = 0, maxOccurs = unbounded _________ */
            const CreditWordsGroupSet& getCreditWordsGroupSet() const;
            void addCreditWordsGroup( const CreditWordsGroupPtr& value );
            void removeCreditWordsGroup( const CreditWordsGroupSetIterConst& value );
            void clearCreditWordsGroupSet();
            CreditWordsGroupPtr getCreditWordsGroup( const CreditWordsGroupSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            CreditImagePtr myCreditImage;
            CreditWordsPtr myCreditWords;
            CreditWordsGroupSet myCreditWordsGroupSet;
        };
    }
}
