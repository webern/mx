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

        MX_FORWARD_DECLARE_ELEMENT( Distance )
        MX_FORWARD_DECLARE_ELEMENT( LineWidth )
        MX_FORWARD_DECLARE_ELEMENT( NoteSize )
        MX_FORWARD_DECLARE_ELEMENT( OtherAppearance )
        MX_FORWARD_DECLARE_ELEMENT( Appearance )

        inline AppearancePtr makeAppearance() { return std::make_shared<Appearance>(); }

        class Appearance : public ElementInterface
        {
        public:
            Appearance();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ LineWidth minOccurs = 0, maxOccurs = unbounded _________ */
            const LineWidthSet& getLineWidthSet() const;
            void addLineWidth( const LineWidthPtr& value );
            void removeLineWidth( const LineWidthSetIterConst& value );
            void clearLineWidthSet();
            LineWidthPtr getLineWidth( const LineWidthSetIterConst& setIterator ) const;

            /* _________ NoteSize minOccurs = 0, maxOccurs = unbounded _________ */
            const NoteSizeSet& getNoteSizeSet() const;
            void addNoteSize( const NoteSizePtr& value );
            void removeNoteSize( const NoteSizeSetIterConst& value );
            void clearNoteSizeSet();
            NoteSizePtr getNoteSize( const NoteSizeSetIterConst& setIterator ) const;

            /* _________ Distance minOccurs = 0, maxOccurs = unbounded _________ */
            const DistanceSet& getDistanceSet() const;
            void addDistance( const DistancePtr& value );
            void removeDistance( const DistanceSetIterConst& value );
            void clearDistanceSet();
            DistancePtr getDistance( const DistanceSetIterConst& setIterator ) const;

            /* _________ OtherAppearance minOccurs = 0, maxOccurs = unbounded _________ */
            const OtherAppearanceSet& getOtherAppearanceSet() const;
            void addOtherAppearance( const OtherAppearancePtr& value );
            void removeOtherAppearance( const OtherAppearanceSetIterConst& value );
            void clearOtherAppearanceSet();
            OtherAppearancePtr getOtherAppearance( const OtherAppearanceSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            LineWidthSet myLineWidthSet;
            NoteSizeSet myNoteSizeSet;
            DistanceSet myDistanceSet;
            OtherAppearanceSet myOtherAppearanceSet;
        };
    }
}
