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

        MX_FORWARD_DECLARE_ELEMENT( Alter )
        MX_FORWARD_DECLARE_ELEMENT( Octave )
        MX_FORWARD_DECLARE_ELEMENT( Step )
        MX_FORWARD_DECLARE_ELEMENT( Pitch )

        inline PitchPtr makePitch() { return std::make_shared<Pitch>(); }

        class Pitch : public ElementInterface
        {
        public:
            Pitch();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Step minOccurs = 1, maxOccurs = 1 _________ */
            StepPtr getStep() const;
            void setStep( const StepPtr& value );

            /* _________ Alter minOccurs = 0, maxOccurs = 1 _________ */
            AlterPtr getAlter() const;
            void setAlter( const AlterPtr& value );
            bool getHasAlter() const;
            void setHasAlter( const bool value );

            /* _________ Octave minOccurs = 1, maxOccurs = 1 _________ */
            OctavePtr getOctave() const;
            void setOctave( const OctavePtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            StepPtr myStep;
            AlterPtr myAlter;
            bool myHasAlter;
            OctavePtr myOctave;
        };
    }
}
