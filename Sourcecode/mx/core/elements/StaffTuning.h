// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/StaffTuningAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( StaffTuningAttributes )
        MX_FORWARD_DECLARE_ELEMENT( TuningAlter )
        MX_FORWARD_DECLARE_ELEMENT( TuningOctave )
        MX_FORWARD_DECLARE_ELEMENT( TuningStep )
        MX_FORWARD_DECLARE_ELEMENT( StaffTuning )

        inline StaffTuningPtr makeStaffTuning() { return std::make_shared<StaffTuning>(); }

        class StaffTuning : public ElementInterface
        {
        public:
            StaffTuning();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StaffTuningAttributesPtr getAttributes() const;
            void setAttributes( const StaffTuningAttributesPtr& value );

            /* _________ TuningStep minOccurs = 1, maxOccurs = 1 _________ */
            TuningStepPtr getTuningStep() const;
            void setTuningStep( const TuningStepPtr& value );

            /* _________ TuningAlter minOccurs = 0, maxOccurs = 1 _________ */
            TuningAlterPtr getTuningAlter() const;
            void setTuningAlter( const TuningAlterPtr& value );
            bool getHasTuningAlter() const;
            void setHasTuningAlter( const bool value );

            /* _________ TuningOctave minOccurs = 1, maxOccurs = 1 _________ */
            TuningOctavePtr getTuningOctave() const;
            void setTuningOctave( const TuningOctavePtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            StaffTuningAttributesPtr myAttributes;
            TuningStepPtr myTuningStep;
            TuningAlterPtr myTuningAlter;
            bool myHasTuningAlter;
            TuningOctavePtr myTuningOctave;
        };
    }
}
