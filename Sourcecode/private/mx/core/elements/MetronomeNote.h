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

        MX_FORWARD_DECLARE_ELEMENT( MetronomeBeam )
        MX_FORWARD_DECLARE_ELEMENT( MetronomeDot )
        MX_FORWARD_DECLARE_ELEMENT( MetronomeTuplet )
        MX_FORWARD_DECLARE_ELEMENT( MetronomeType )
        MX_FORWARD_DECLARE_ELEMENT( MetronomeNote )

        inline MetronomeNotePtr makeMetronomeNote() { return std::make_shared<MetronomeNote>(); }

        class MetronomeNote : public ElementInterface
        {
        public:
            MetronomeNote();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ MetronomeType minOccurs = 1, maxOccurs = 1 _________ */
            MetronomeTypePtr getMetronomeType() const;
            void setMetronomeType( const MetronomeTypePtr& value );

            /* _________ MetronomeDot minOccurs = 0, maxOccurs = unbounded _________ */
            const MetronomeDotSet& getMetronomeDotSet() const;
            void addMetronomeDot( const MetronomeDotPtr& value );
            void removeMetronomeDot( const MetronomeDotSetIterConst& value );
            void clearMetronomeDotSet();
            MetronomeDotPtr getMetronomeDot( const MetronomeDotSetIterConst& setIterator ) const;

            /* _________ MetronomeBeam minOccurs = 0, maxOccurs = unbounded _________ */
            const MetronomeBeamSet& getMetronomeBeamSet() const;
            void addMetronomeBeam( const MetronomeBeamPtr& value );
            void removeMetronomeBeam( const MetronomeBeamSetIterConst& value );
            void clearMetronomeBeamSet();
            MetronomeBeamPtr getMetronomeBeam( const MetronomeBeamSetIterConst& setIterator ) const;

            /* _________ MetronomeTuplet minOccurs = 0, maxOccurs = 1 _________ */
            MetronomeTupletPtr getMetronomeTuplet() const;
            void setMetronomeTuplet( const MetronomeTupletPtr& value );
            bool getHasMetronomeTuplet() const;
            void setHasMetronomeTuplet( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            MetronomeTypePtr myMetronomeType;
            MetronomeDotSet myMetronomeDotSet;
            MetronomeBeamSet myMetronomeBeamSet;
            MetronomeTupletPtr myMetronomeTuplet;
            bool myHasMetronomeTuplet;
        };
    }
}
