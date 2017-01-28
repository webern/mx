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

        MX_FORWARD_DECLARE_ELEMENT( Bass )
        MX_FORWARD_DECLARE_ELEMENT( Degree )
        MX_FORWARD_DECLARE_ELEMENT( Function )
        MX_FORWARD_DECLARE_ELEMENT( Inversion )
        MX_FORWARD_DECLARE_ELEMENT( Kind )
        MX_FORWARD_DECLARE_ELEMENT( Root )
        MX_FORWARD_DECLARE_ELEMENT( HarmonyChordGroup )

        inline HarmonyChordGroupPtr makeHarmonyChordGroup() { return std::make_shared<HarmonyChordGroup>(); }

        class HarmonyChordGroup : public ElementInterface
        {
        public:
            enum class Choice
            {
                root = 1,
                function = 2
            };
            HarmonyChordGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HarmonyChordGroup::Choice getChoice() const;
            void setChoice( const HarmonyChordGroup::Choice value );

            /* _________ Root minOccurs = 1, maxOccurs = 1 _________ */
            RootPtr getRoot() const;
            void setRoot( const RootPtr& value );

            /* _________ Function minOccurs = 1, maxOccurs = 1 _________ */
            FunctionPtr getFunction() const;
            void setFunction( const FunctionPtr& value );

            /* _________ Kind minOccurs = 1, maxOccurs = 1 _________ */
            KindPtr getKind() const;
            void setKind( const KindPtr& value );

            /* _________ Inversion minOccurs = 0, maxOccurs = 1 _________ */
            InversionPtr getInversion() const;
            void setInversion( const InversionPtr& value );
            bool getHasInversion() const;
            void setHasInversion( const bool value );

            /* _________ Bass minOccurs = 0, maxOccurs = 1 _________ */
            BassPtr getBass() const;
            void setBass( const BassPtr& value );
            bool getHasBass() const;
            void setHasBass( const bool value );

            /* _________ Degree minOccurs = 0, maxOccurs = unbounded _________ */
            const DegreeSet& getDegreeSet() const;
            void addDegree( const DegreePtr& value );
            void removeDegree( const DegreeSetIterConst& value );
            void clearDegreeSet();
            DegreePtr getDegree( const DegreeSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            RootPtr myRoot;
            FunctionPtr myFunction;
            KindPtr myKind;
            InversionPtr myInversion;
            bool myHasInversion;
            BassPtr myBass;
            bool myHasBass;
            DegreeSet myDegreeSet;
        };
    }
}
