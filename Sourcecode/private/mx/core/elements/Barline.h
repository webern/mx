// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/BarlineAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( BarlineAttributes )
        MX_FORWARD_DECLARE_ELEMENT( BarStyle )
        MX_FORWARD_DECLARE_ELEMENT( Coda )
        MX_FORWARD_DECLARE_ELEMENT( EditorialGroup )
        MX_FORWARD_DECLARE_ELEMENT( Ending )
        MX_FORWARD_DECLARE_ELEMENT( Fermata )
        MX_FORWARD_DECLARE_ELEMENT( Repeat )
        MX_FORWARD_DECLARE_ELEMENT( Segno )
        MX_FORWARD_DECLARE_ELEMENT( WavyLine )
        MX_FORWARD_DECLARE_ELEMENT( Barline )

        inline BarlinePtr makeBarline() { return std::make_shared<Barline>(); }

        class Barline : public ElementInterface
        {
        public:
            Barline();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BarlineAttributesPtr getAttributes() const;
            void setAttributes( const BarlineAttributesPtr& value );

            /* _________ BarStyle minOccurs = 0, maxOccurs = 1 _________ */
            BarStylePtr getBarStyle() const;
            void setBarStyle( const BarStylePtr& value );
            bool getHasBarStyle() const;
            void setHasBarStyle( const bool value );

            /* _________ EditorialGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialGroupPtr getEditorialGroup() const;
            void setEditorialGroup( const EditorialGroupPtr& value );

            /* _________ WavyLine minOccurs = 0, maxOccurs = 1 _________ */
            WavyLinePtr getWavyLine() const;
            void setWavyLine( const WavyLinePtr& value );
            bool getHasWavyLine() const;
            void setHasWavyLine( const bool value );

            /* _________ Segno minOccurs = 0, maxOccurs = 1 _________ */
            SegnoPtr getSegno() const;
            void setSegno( const SegnoPtr& value );
            bool getHasSegno() const;
            void setHasSegno( const bool value );

            /* _________ Coda minOccurs = 0, maxOccurs = 1 _________ */
            CodaPtr getCoda() const;
            void setCoda( const CodaPtr& value );
            bool getHasCoda() const;
            void setHasCoda( const bool value );

            /* _________ Fermata minOccurs = 0, maxOccurs = 2 _________ */
            const FermataSet& getFermataSet() const;
            void addFermata( const FermataPtr& value );
            void removeFermata( const FermataSetIterConst& value );
            void clearFermataSet();
            FermataPtr getFermata( const FermataSetIterConst& setIterator ) const;

            /* _________ Ending minOccurs = 0, maxOccurs = 1 _________ */
            EndingPtr getEnding() const;
            void setEnding( const EndingPtr& value );
            bool getHasEnding() const;
            void setHasEnding( const bool value );

            /* _________ Repeat minOccurs = 0, maxOccurs = 1 _________ */
            RepeatPtr getRepeat() const;
            void setRepeat( const RepeatPtr& value );
            bool getHasRepeat() const;
            void setHasRepeat( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            BarlineAttributesPtr myAttributes;
            BarStylePtr myBarStyle;
            bool myHasBarStyle;
            EditorialGroupPtr myEditorialGroup;
            WavyLinePtr myWavyLine;
            bool myHasWavyLine;
            SegnoPtr mySegno;
            bool myHasSegno;
            CodaPtr myCoda;
            bool myHasCoda;
            FermataSet myFermataSet;
            EndingPtr myEnding;
            bool myHasEnding;
            RepeatPtr myRepeat;
            bool myHasRepeat;
        };
    }
}
