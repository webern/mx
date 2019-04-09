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

        MX_FORWARD_DECLARE_ELEMENT( SenzaMisura )
        MX_FORWARD_DECLARE_ELEMENT( TimeSignatureGroup )
        MX_FORWARD_DECLARE_ELEMENT( TimeChoice )

        inline TimeChoicePtr makeTimeChoice() { return std::make_shared<TimeChoice>(); }

        class TimeChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                timeSignature = 0,
                senzaMisura = 1
            };
            TimeChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            
            Choice getChoice() const;
            void setChoice( const Choice value );
            
            /* _________ TimeSignature minOccurs = 1, maxOccurs = unbounded _________ */
            const TimeSignatureGroupSet& getTimeSignatureGroupSet() const;
            void addTimeSignatureGroup( const TimeSignatureGroupPtr& value );
            void removeTimeSignatureGroup( const TimeSignatureGroupSetIterConst& value );
            void clearTimeSignatureGroupSet();
            TimeSignatureGroupPtr getTimeSignatureGroup( const TimeSignatureGroupSetIterConst& setIterator ) const;
            
            SenzaMisuraPtr getSenzaMisura() const;
            void setSenzaMisura( const SenzaMisuraPtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            TimeSignatureGroupSet myTimeSignatureGroupSet;
            SenzaMisuraPtr mySenzaMisura;
        };
    }
}
