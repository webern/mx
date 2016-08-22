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

        MX_FORWARD_DECLARE_ELEMENT( Clef )
        MX_FORWARD_DECLARE_ELEMENT( Directive )
        MX_FORWARD_DECLARE_ELEMENT( Divisions )
        MX_FORWARD_DECLARE_ELEMENT( EditorialGroup )
        MX_FORWARD_DECLARE_ELEMENT( Instruments )
        MX_FORWARD_DECLARE_ELEMENT( Key )
        MX_FORWARD_DECLARE_ELEMENT( MeasureStyle )
        MX_FORWARD_DECLARE_ELEMENT( PartSymbol )
        MX_FORWARD_DECLARE_ELEMENT( StaffDetails )
        MX_FORWARD_DECLARE_ELEMENT( Staves )
        MX_FORWARD_DECLARE_ELEMENT( Time )
        MX_FORWARD_DECLARE_ELEMENT( Transpose )
        MX_FORWARD_DECLARE_ELEMENT( Properties )

        inline PropertiesPtr makeProperties() { return std::make_shared<Properties>(); }

        class Properties : public ElementInterface
        {
        public:
            Properties();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ EditorialGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialGroupPtr getEditorialGroup() const;
            void setEditorialGroup( const EditorialGroupPtr& value );

            /* _________ Divisions minOccurs = 0, maxOccurs = 1 _________ */
            DivisionsPtr getDivisions() const;
            void setDivisions( const DivisionsPtr& value );
            bool getHasDivisions() const;
            void setHasDivisions( const bool value );

            /* _________ Key minOccurs = 0, maxOccurs = unbounded _________ */
            const KeySet& getKeySet() const;
            void addKey( const KeyPtr& value );
            void removeKey( const KeySetIterConst& value );
            void clearKeySet();
            KeyPtr getKey( const KeySetIterConst& setIterator ) const;

            /* _________ Time minOccurs = 0, maxOccurs = unbounded _________ */
            const TimeSet& getTimeSet() const;
            void addTime( const TimePtr& value );
            void removeTime( const TimeSetIterConst& value );
            void clearTimeSet();
            TimePtr getTime( const TimeSetIterConst& setIterator ) const;

            /* _________ Staves minOccurs = 0, maxOccurs = 1 _________ */
            StavesPtr getStaves() const;
            void setStaves( const StavesPtr& value );
            bool getHasStaves() const;
            void setHasStaves( const bool value );

            /* _________ PartSymbol minOccurs = 0, maxOccurs = 1 _________ */
            PartSymbolPtr getPartSymbol() const;
            void setPartSymbol( const PartSymbolPtr& value );
            bool getHasPartSymbol() const;
            void setHasPartSymbol( const bool value );

            /* _________ Instruments minOccurs = 0, maxOccurs = 1 _________ */
            InstrumentsPtr getInstruments() const;
            void setInstruments( const InstrumentsPtr& value );
            bool getHasInstruments() const;
            void setHasInstruments( const bool value );

            /* _________ Clef minOccurs = 0, maxOccurs = unbounded _________ */
            const ClefSet& getClefSet() const;
            void addClef( const ClefPtr& value );
            void removeClef( const ClefSetIterConst& value );
            void clearClefSet();
            ClefPtr getClef( const ClefSetIterConst& setIterator ) const;

            /* _________ StaffDetails minOccurs = 0, maxOccurs = unbounded _________ */
            const StaffDetailsSet& getStaffDetailsSet() const;
            void addStaffDetails( const StaffDetailsPtr& value );
            void removeStaffDetails( const StaffDetailsSetIterConst& value );
            void clearStaffDetailsSet();
            StaffDetailsPtr getStaffDetails( const StaffDetailsSetIterConst& setIterator ) const;

            /* _________ Transpose minOccurs = 0, maxOccurs = unbounded _________ */
            const TransposeSet& getTransposeSet() const;
            void addTranspose( const TransposePtr& value );
            void removeTranspose( const TransposeSetIterConst& value );
            void clearTransposeSet();
            TransposePtr getTranspose( const TransposeSetIterConst& setIterator ) const;

            /* _________ Directive minOccurs = 0, maxOccurs = unbounded _________ */
            const DirectiveSet& getDirectiveSet() const;
            void addDirective( const DirectivePtr& value );
            void removeDirective( const DirectiveSetIterConst& value );
            void clearDirectiveSet();
            DirectivePtr getDirective( const DirectiveSetIterConst& setIterator ) const;

            /* _________ MeasureStyle minOccurs = 0, maxOccurs = unbounded _________ */
            const MeasureStyleSet& getMeasureStyleSet() const;
            void addMeasureStyle( const MeasureStylePtr& value );
            void removeMeasureStyle( const MeasureStyleSetIterConst& value );
            void clearMeasureStyleSet();
            MeasureStylePtr getMeasureStyle( const MeasureStyleSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
            
        private:
            EditorialGroupPtr myEditorialGroup;
            DivisionsPtr myDivisions;
            bool myHasDivisions;
            KeySet myKeySet;
            TimeSet myTimeSet;
            StavesPtr myStaves;
            bool myHasStaves;
            PartSymbolPtr myPartSymbol;
            bool myHasPartSymbol;
            InstrumentsPtr myInstruments;
            bool myHasInstruments;
            ClefSet myClefSet;
            StaffDetailsSet myStaffDetailsSet;
            TransposeSet myTransposeSet;
            DirectiveSet myDirectiveSet;
            MeasureStyleSet myMeasureStyleSet;
        };
    }
}
