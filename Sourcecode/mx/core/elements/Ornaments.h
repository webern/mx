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

        MX_FORWARD_DECLARE_ELEMENT( AccidentalMark )
        MX_FORWARD_DECLARE_ELEMENT( OrnamentsChoice )
        MX_FORWARD_DECLARE_ELEMENT( Ornaments )

        inline OrnamentsPtr makeOrnaments() { return std::make_shared<Ornaments>(); }

        class Ornaments : public ElementInterface
        {
        public:
            Ornaments();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ OrnamentsChoice minOccurs = 0, maxOccurs = unbounded _________ */
            const OrnamentsChoiceSet& getOrnamentsChoiceSet() const;
            void addOrnamentsChoice( const OrnamentsChoicePtr& value );
            void removeOrnamentsChoice( const OrnamentsChoiceSetIterConst& value );
            void clearOrnamentsChoiceSet();
            OrnamentsChoicePtr getOrnamentsChoice( const OrnamentsChoiceSetIterConst& setIterator ) const;


            /* _________ AccidentalMark minOccurs = 0, maxOccurs = unbounded _________ */
            const AccidentalMarkSet& getAccidentalMarkSet() const;
            void addAccidentalMark( const AccidentalMarkPtr& value );
            void removeAccidentalMark( const AccidentalMarkSetIterConst& value );
            void clearAccidentalMarkSet();
            AccidentalMarkPtr getAccidentalMark( const AccidentalMarkSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            OrnamentsChoiceSet myOrnamentsChoiceSet;
            AccidentalMarkSet myAccidentalMarkSet;
            
            bool isOrnamentsChoice( const char* const name ) const;
        };
    }
}
