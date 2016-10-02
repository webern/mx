// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

//#define MX_USE_OLD_ELEMENTS_CPP

//#define MX_ELEMENTS_THE_OLD_WAY
#ifndef MX_ELEMENTS_THE_OLD_WAY

    #include "mx/core/IncludeAlot.h"

#endif


#ifdef MX_ELEMENTS_THE_OLD_WAY


#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/Strings.h"
#include "mx/core/FontSize.h"
#include "mx/core/Integers.h"
#include "mx/core/Decimals.h"
#include "mx/core/NumberOrNormal.h"
#include "mx/core/PositiveIntegerOrEmpty.h"
#include "mx/core/Color.h"
#include "mx/core/Date.h"
#include "mx/core/YesNoNumber.h"
#include "mx/core/EmptyPrintObjectStyleAlignAttributes.h"
#include <memory>

namespace mx
{
    namespace core
    {

        struct DynamicsAttributes;
        using DynamicsAttributesPtr = std::shared_ptr<DynamicsAttributes>;
        
        struct DynamicsAttributes : public AttributesInterface
        {
        public:
            DynamicsAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight halign;
            AboveBelow placement;
            NumberOfLines underline;
            NumberOfLines overline;
            NumberOfLines lineThrough;
            EnclosureShape enclosure;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
            bool hasPlacement;
            bool hasUnderline;
            bool hasOverline;
            bool hasLineThrough;
            bool hasEnclosure;
        };
        
        class Dynamics;
        using DynamicsPtr = std::shared_ptr<Dynamics>;
        using DynamicsUPtr = std::unique_ptr<Dynamics>;
        using DynamicsSet = std::vector<DynamicsPtr>;
        using DynamicsSetIter = DynamicsSet::iterator;
        using DynamicsSetIterConst = DynamicsSet::const_iterator;
        inline DynamicsPtr makeDynamics() { return std::make_shared<Dynamics>(); }
		inline DynamicsPtr makeDynamics( const DynamicsValue& value ) { return std::make_shared<Dynamics>( value ); }
		inline DynamicsPtr makeDynamics( DynamicsValue&& value ) { return std::make_shared<Dynamics>( std::move( value ) ); }

        class Dynamics : public ElementInterface
        {
        public:
            Dynamics( const DynamicsValue& value );
            Dynamics();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DynamicsAttributesPtr getAttributes() const;
            void setAttributes( const DynamicsAttributes& attributes );
            DynamicsValue getValue() const;
            void setValue( const DynamicsValue& value );
            

        private:
            DynamicsValue myValue;
            DynamicsAttributesPtr myAttributes;
        };

        struct AccidentalTextAttributes;
        using AccidentalTextAttributesPtr = std::shared_ptr<AccidentalTextAttributes>;
        
        struct AccidentalTextAttributes : public AttributesInterface
        {
        public:
            AccidentalTextAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            LeftCenterRight justify;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight halign;
            NumberOfLines underline;
            NumberOfLines overline;
            NumberOfLines lineThrough;
            RotationDegrees rotation;
            NumberOrNormal letterSpacing;
            NumberOrNormal lineHeight;
            XmlLang lang;
            XmlSpace space;
            EnclosureShape enclosure;
            bool hasJustify;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
            bool hasUnderline;
            bool hasOverline;
            bool hasLineThrough;
            bool hasRotation;
            bool hasLetterSpacing;
            bool hasLineHeight;
            bool hasLang;
            bool hasSpace;
            bool hasEnclosure;
        };
        
        class AccidentalText;
        using AccidentalTextPtr = std::shared_ptr<AccidentalText>;
        using AccidentalTextUPtr = std::unique_ptr<AccidentalText>;
        using AccidentalTextSet = std::vector<AccidentalTextPtr>;
        using AccidentalTextSetIter = AccidentalTextSet::iterator;
        using AccidentalTextSetIterConst = AccidentalTextSet::const_iterator;
        inline AccidentalTextPtr makeAccidentalText() { return std::make_shared<AccidentalText>(); }
		inline AccidentalTextPtr makeAccidentalText( const AccidentalValue& value ) { return std::make_shared<AccidentalText>( value ); }
		inline AccidentalTextPtr makeAccidentalText( AccidentalValue&& value ) { return std::make_shared<AccidentalText>( std::move( value ) ); }

        class AccidentalText : public ElementInterface
        {
        public:
            AccidentalText();
            AccidentalText( const AccidentalValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            AccidentalTextAttributesPtr getAttributes() const;
            void setAttributes( const AccidentalTextAttributesPtr& attributes );
            AccidentalValue getValue() const;
            void setValue( const AccidentalValue& value );

        private:
            AccidentalValue myValue;
            AccidentalTextAttributesPtr myAttributes;
        };

        struct DisplayTextAttributes;
        using DisplayTextAttributesPtr = std::shared_ptr<DisplayTextAttributes>;
        
        struct DisplayTextAttributes : public AttributesInterface
        {
        public:
            DisplayTextAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            LeftCenterRight justify;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight halign;
            NumberOfLines underline;
            NumberOfLines overline;
            NumberOfLines lineThrough;
            RotationDegrees rotation;
            NumberOrNormal letterSpacing;
            NumberOrNormal lineHeight;
            XmlLang lang;
            XmlSpace space;
            EnclosureShape enclosure;
            bool hasJustify;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
            bool hasUnderline;
            bool hasOverline;
            bool hasLineThrough;
            bool hasRotation;
            bool hasLetterSpacing;
            bool hasLineHeight;
            bool hasLang;
            bool hasSpace;
            bool hasEnclosure;
        };
        
        class DisplayText;
        using DisplayTextPtr = std::shared_ptr<DisplayText>;
        using DisplayTextUPtr = std::unique_ptr<DisplayText>;
        using DisplayTextSet = std::vector<DisplayTextPtr>;
        using DisplayTextSetIter = DisplayTextSet::iterator;
        using DisplayTextSetIterConst = DisplayTextSet::const_iterator;
        inline DisplayTextPtr makeDisplayText() { return std::make_shared<DisplayText>(); }
		inline DisplayTextPtr makeDisplayText( const XsString& value ) { return std::make_shared<DisplayText>( value ); }
		inline DisplayTextPtr makeDisplayText( XsString&& value ) { return std::make_shared<DisplayText>( std::move( value ) ); }

        class DisplayText : public ElementInterface
        {
        public:
            DisplayText();
            DisplayText( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DisplayTextAttributesPtr getAttributes() const;
            void setAttributes( const DisplayTextAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            DisplayTextAttributesPtr myAttributes;
        };
        
        
        
        struct DisplayTextAttributes;
        using DisplayTextAttributesPtr = std::shared_ptr<DisplayTextAttributes>;
        
        
        
 
        
        struct OtherPlayAttributes;
        using OtherPlayAttributesPtr = std::shared_ptr<OtherPlayAttributes>;
        
        struct OtherPlayAttributes : public AttributesInterface
        {
        public:
            OtherPlayAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken type;
            const bool hasType;
        };
        
        class OtherPlay;
        using OtherPlayPtr = std::shared_ptr<OtherPlay>;
        using OtherPlayUPtr = std::unique_ptr<OtherPlay>;
        using OtherPlaySet = std::vector<OtherPlayPtr>;
        using OtherPlaySetIter = OtherPlaySet::iterator;
        using OtherPlaySetIterConst = OtherPlaySet::const_iterator;
        inline OtherPlayPtr makeOtherPlay() { return std::make_shared<OtherPlay>(); }
		inline OtherPlayPtr makeOtherPlay( const XsString& value ) { return std::make_shared<OtherPlay>( value ); }
		inline OtherPlayPtr makeOtherPlay( XsString&& value ) { return std::make_shared<OtherPlay>( std::move( value ) ); }

        class OtherPlay : public ElementInterface
        {
        public:
            OtherPlay();
            OtherPlay( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OtherPlayAttributesPtr getAttributes() const;
            void setAttributes( const OtherPlayAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            OtherPlayAttributesPtr myAttributes;
        };
        
 
        struct PartSymbolAttributes;
        using PartSymbolAttributesPtr = std::shared_ptr<PartSymbolAttributes>;
        
        struct PartSymbolAttributes : public AttributesInterface
        {
        public:
            PartSymbolAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StaffNumber topStaff;
            StaffNumber bottomStaff;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            bool hasTopStaff;
            bool hasBottomStaff;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
        };


        class PartSymbol;
        using PartSymbolPtr = std::shared_ptr<PartSymbol>;
        using PartSymbolUPtr = std::unique_ptr<PartSymbol>;
        using PartSymbolSet = std::vector<PartSymbolPtr>;
        using PartSymbolSetIter = PartSymbolSet::iterator;
        using PartSymbolSetIterConst = PartSymbolSet::const_iterator;
        inline PartSymbolPtr makePartSymbol() { return std::make_shared<PartSymbol>(); }
		inline PartSymbolPtr makePartSymbol( const GroupSymbolValue& value ) { return std::make_shared<PartSymbol>( value ); }
		inline PartSymbolPtr makePartSymbol( GroupSymbolValue&& value ) { return std::make_shared<PartSymbol>( std::move( value ) ); }

        class PartSymbol : public ElementInterface
        {
        public:
            PartSymbol();
            PartSymbol( const GroupSymbolValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PartSymbolAttributesPtr getAttributes() const;
            void setAttributes( const PartSymbolAttributesPtr& attributes );
            GroupSymbolValue getValue() const;
            void setValue( const GroupSymbolValue& value );

        private:
            GroupSymbolValue myValue;
            PartSymbolAttributesPtr myAttributes;
        };
        

        struct KeyOctaveAttributes;
        using KeyOctaveAttributesPtr = std::shared_ptr<KeyOctaveAttributes>;
        
        struct KeyOctaveAttributes : public AttributesInterface
        {
        public:
            KeyOctaveAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            PositiveInteger number;
            YesNo cancel;
            const bool hasNumber;
            bool hasCancel;
        };
        
        class KeyOctave;
        using KeyOctavePtr = std::shared_ptr<KeyOctave>;
        using KeyOctaveUPtr = std::unique_ptr<KeyOctave>;
        using KeyOctaveSet = std::vector<KeyOctavePtr>;
        using KeyOctaveSetIter = KeyOctaveSet::iterator;
        using KeyOctaveSetIterConst = KeyOctaveSet::const_iterator;
        inline KeyOctavePtr makeKeyOctave() { return std::make_shared<KeyOctave>(); }
		inline KeyOctavePtr makeKeyOctave( const OctaveValue& value ) { return std::make_shared<KeyOctave>( value ); }
		inline KeyOctavePtr makeKeyOctave( OctaveValue&& value ) { return std::make_shared<KeyOctave>( std::move( value ) ); }

        class KeyOctave : public ElementInterface
        {
        public:
            KeyOctave();
            KeyOctave( const OctaveValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            KeyOctaveAttributesPtr getAttributes() const;
            void setAttributes( const KeyOctaveAttributesPtr& attributes );
            OctaveValue getValue() const;
            void setValue( const OctaveValue& value );

        private:
            OctaveValue myValue;
            KeyOctaveAttributesPtr myAttributes;
        };
        

        struct MultipleRestAttributes;
        using MultipleRestAttributesPtr = std::shared_ptr<MultipleRestAttributes>;
        
        struct MultipleRestAttributes : public AttributesInterface
        {
        public:
            MultipleRestAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo useSymbols;
            bool hasUseSymbols;
        };
        
        class MultipleRest;
        using MultipleRestPtr = std::shared_ptr<MultipleRest>;
        using MultipleRestUPtr = std::unique_ptr<MultipleRest>;
        using MultipleRestSet = std::vector<MultipleRestPtr>;
        using MultipleRestSetIter = MultipleRestSet::iterator;
        using MultipleRestSetIterConst = MultipleRestSet::const_iterator;
        inline MultipleRestPtr makeMultipleRest() { return std::make_shared<MultipleRest>(); }
		inline MultipleRestPtr makeMultipleRest( const PositiveIntegerOrEmpty& value ) { return std::make_shared<MultipleRest>( value ); }
		inline MultipleRestPtr makeMultipleRest( PositiveIntegerOrEmpty&& value ) { return std::make_shared<MultipleRest>( std::move( value ) ); }

        class MultipleRest : public ElementInterface
        {
        public:
            MultipleRest();
            MultipleRest( const PositiveIntegerOrEmpty& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MultipleRestAttributesPtr getAttributes() const;
            void setAttributes( const MultipleRestAttributesPtr& attributes );
            PositiveIntegerOrEmpty getValue() const;
            void setValue( const PositiveIntegerOrEmpty& value );

        private:
            PositiveIntegerOrEmpty myValue;
            MultipleRestAttributesPtr myAttributes;
        };
        

        struct MeasureRepeatAttributes;
        using MeasureRepeatAttributesPtr = std::shared_ptr<MeasureRepeatAttributes>;
        
        struct MeasureRepeatAttributes : public AttributesInterface
        {
        public:
            MeasureRepeatAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            PositiveInteger slashes;
            const bool hasType;
            bool hasSlashes;
        };
        
        class MeasureRepeat;
        using MeasureRepeatPtr = std::shared_ptr<MeasureRepeat>;
        using MeasureRepeatUPtr = std::unique_ptr<MeasureRepeat>;
        using MeasureRepeatSet = std::vector<MeasureRepeatPtr>;
        using MeasureRepeatSetIter = MeasureRepeatSet::iterator;
        using MeasureRepeatSetIterConst = MeasureRepeatSet::const_iterator;
        inline MeasureRepeatPtr makeMeasureRepeat() { return std::make_shared<MeasureRepeat>(); }
		inline MeasureRepeatPtr makeMeasureRepeat( const PositiveIntegerOrEmpty& value ) { return std::make_shared<MeasureRepeat>( value ); }
		inline MeasureRepeatPtr makeMeasureRepeat( PositiveIntegerOrEmpty&& value ) { return std::make_shared<MeasureRepeat>( std::move( value ) ); }

        class MeasureRepeat : public ElementInterface
        {
        public:
            MeasureRepeat();
            MeasureRepeat( const PositiveIntegerOrEmpty& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MeasureRepeatAttributesPtr getAttributes() const;
            void setAttributes( const MeasureRepeatAttributesPtr& attributes );
            PositiveIntegerOrEmpty getValue() const;
            void setValue( const PositiveIntegerOrEmpty& value );

        private:
            PositiveIntegerOrEmpty myValue;
            MeasureRepeatAttributesPtr myAttributes;
        };
        
        
        struct BarStyleAttributes;
        using BarStyleAttributesPtr = std::shared_ptr<BarStyleAttributes>;
        
        struct BarStyleAttributes : public AttributesInterface
        {
        public:
            BarStyleAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            Color color;
            bool hasColor;
        };
        
        class BarStyle;
        using BarStylePtr = std::shared_ptr<BarStyle>;
        using BarStyleUPtr = std::unique_ptr<BarStyle>;
        using BarStyleSet = std::vector<BarStylePtr>;
        using BarStyleSetIter = BarStyleSet::iterator;
        using BarStyleSetIterConst = BarStyleSet::const_iterator;
        inline BarStylePtr makeBarStyle() { return std::make_shared<BarStyle>(); }
		inline BarStylePtr makeBarStyle( const BarStyleEnum& value ) { return std::make_shared<BarStyle>( value ); }
		inline BarStylePtr makeBarStyle( BarStyleEnum&& value ) { return std::make_shared<BarStyle>( std::move( value ) ); }

        class BarStyle : public ElementInterface
        {
        public:
            BarStyle();
            BarStyle( const BarStyleEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BarStyleAttributesPtr getAttributes() const;
            void setAttributes( const BarStyleAttributesPtr& attributes );
            BarStyleEnum getValue() const;
            void setValue( const BarStyleEnum& value );

        private:
            BarStyleEnum myValue;
            BarStyleAttributesPtr myAttributes;
        };
        
        
        struct FermataAttributes;
        using FermataAttributesPtr = std::shared_ptr<FermataAttributes>;
        
        struct FermataAttributes : public AttributesInterface
        {
        public:
            FermataAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            UprightInverted type;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasType;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class Fermata;
        using FermataPtr = std::shared_ptr<Fermata>;
        using FermataUPtr = std::unique_ptr<Fermata>;
        using FermataSet = std::vector<FermataPtr>;
        using FermataSetIter = FermataSet::iterator;
        using FermataSetIterConst = FermataSet::const_iterator;
        inline FermataPtr makeFermata() { return std::make_shared<Fermata>(); }
		inline FermataPtr makeFermata( const FermataShape& value ) { return std::make_shared<Fermata>( value ); }
		inline FermataPtr makeFermata( FermataShape&& value ) { return std::make_shared<Fermata>( std::move( value ) ); }

        class Fermata : public ElementInterface
        {
        public:
            Fermata();
            Fermata( const FermataShape& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FermataAttributesPtr getAttributes() const;
            void setAttributes( const FermataAttributesPtr& attributes );
            FermataShape getValue() const;
            void setValue( const FermataShape& value );

        private:
            FermataShape myValue;
            FermataAttributesPtr myAttributes;
        };
        
        
        struct EndingAttributes;
        using EndingAttributesPtr = std::shared_ptr<EndingAttributes>;
        
        struct EndingAttributes : public AttributesInterface
        {
        public:
            EndingAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            EndingNumber number;
            StartStopDiscontinue type;
            YesNo printObject;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            TenthsValue endLength;
            TenthsValue textX;
            TenthsValue textY;
            const bool hasNumber;
            const bool hasType;
            bool hasPrintObject;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasEndLength;
            bool hasTextX;
            bool hasTextY;
        };
        
        class Ending;
        using EndingPtr = std::shared_ptr<Ending>;
        using EndingUPtr = std::unique_ptr<Ending>;
        using EndingSet = std::vector<EndingPtr>;
        using EndingSetIter = EndingSet::iterator;
        using EndingSetIterConst = EndingSet::const_iterator;
        inline EndingPtr makeEnding() { return std::make_shared<Ending>(); }
		inline EndingPtr makeEnding( const XsString& value ) { return std::make_shared<Ending>( value ); }
		inline EndingPtr makeEnding( XsString&& value ) { return std::make_shared<Ending>( std::move( value ) ); }

        class Ending : public ElementInterface
        {
        public:
            Ending();
            Ending( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EndingAttributesPtr getAttributes() const;
            void setAttributes( const EndingAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            EndingAttributesPtr myAttributes;
        };
        
        
        struct BassStepAttributes;
        using BassStepAttributesPtr = std::shared_ptr<BassStepAttributes>;
        
        struct BassStepAttributes : public AttributesInterface
        {
        public:
            BassStepAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken text;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasText;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class BassStep;
        using BassStepPtr = std::shared_ptr<BassStep>;
        using BassStepUPtr = std::unique_ptr<BassStep>;
        using BassStepSet = std::vector<BassStepPtr>;
        using BassStepSetIter = BassStepSet::iterator;
        using BassStepSetIterConst = BassStepSet::const_iterator;
        inline BassStepPtr makeBassStep() { return std::make_shared<BassStep>(); }
		inline BassStepPtr makeBassStep( const StepEnum& value ) { return std::make_shared<BassStep>( value ); }
		inline BassStepPtr makeBassStep( StepEnum&& value ) { return std::make_shared<BassStep>( std::move( value ) ); }

        class BassStep : public ElementInterface
        {
        public:
            BassStep();
            BassStep( const StepEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BassStepAttributesPtr getAttributes() const;
            void setAttributes( const BassStepAttributesPtr& attributes );
            StepEnum getValue() const;
            void setValue( const StepEnum& value );

        private:
            StepEnum myValue;
            BassStepAttributesPtr myAttributes;
        };
        
        
        struct BassAlterAttributes;
        using BassAlterAttributesPtr = std::shared_ptr<BassAlterAttributes>;
        
        struct BassAlterAttributes : public AttributesInterface
        {
        public:
            BassAlterAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo printObject;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftRight location;
            bool hasPrintObject;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasLocation;
        };
        
        class BassAlter;
        using BassAlterPtr = std::shared_ptr<BassAlter>;
        using BassAlterUPtr = std::unique_ptr<BassAlter>;
        using BassAlterSet = std::vector<BassAlterPtr>;
        using BassAlterSetIter = BassAlterSet::iterator;
        using BassAlterSetIterConst = BassAlterSet::const_iterator;
        inline BassAlterPtr makeBassAlter() { return std::make_shared<BassAlter>(); }
		inline BassAlterPtr makeBassAlter( const Semitones& value ) { return std::make_shared<BassAlter>( value ); }
		inline BassAlterPtr makeBassAlter( Semitones&& value ) { return std::make_shared<BassAlter>( std::move( value ) ); }

        class BassAlter : public ElementInterface
        {
        public:
            BassAlter();
            BassAlter( const Semitones& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BassAlterAttributesPtr getAttributes() const;
            void setAttributes( const BassAlterAttributesPtr& attributes );
            Semitones getValue() const;
            void setValue( const Semitones& value );

        private:
            Semitones myValue;
            BassAlterAttributesPtr myAttributes;
        };
        
        
        struct DegreeValueAttributes;
        using DegreeValueAttributesPtr = std::shared_ptr<DegreeValueAttributes>;
        
        struct DegreeValueAttributes : public AttributesInterface
        {
        public:
            DegreeValueAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            DegreeSymbolValue symbol;
            XsToken text;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasSymbol;
            bool hasText;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class DegreeValue;
        using DegreeValuePtr = std::shared_ptr<DegreeValue>;
        using DegreeValueUPtr = std::unique_ptr<DegreeValue>;
        using DegreeValueSet = std::vector<DegreeValuePtr>;
        using DegreeValueSetIter = DegreeValueSet::iterator;
        using DegreeValueSetIterConst = DegreeValueSet::const_iterator;
        inline DegreeValuePtr makeDegreeValue() { return std::make_shared<DegreeValue>(); }
		inline DegreeValuePtr makeDegreeValue( const PositiveInteger& value ) { return std::make_shared<DegreeValue>( value ); }
		inline DegreeValuePtr makeDegreeValue( PositiveInteger&& value ) { return std::make_shared<DegreeValue>( std::move( value ) ); }

        class DegreeValue : public ElementInterface
        {
        public:
            DegreeValue();
            DegreeValue( const PositiveInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DegreeValueAttributesPtr getAttributes() const;
            void setAttributes( const DegreeValueAttributesPtr& attributes );
            PositiveInteger getValue() const;
            void setValue( const PositiveInteger& value );

        private:
            PositiveInteger myValue;
            DegreeValueAttributesPtr myAttributes;
        };
        
        
        struct DegreeAlterAttributes;
        using DegreeAlterAttributesPtr = std::shared_ptr<DegreeAlterAttributes>;
        
        struct DegreeAlterAttributes : public AttributesInterface
        {
        public:
            DegreeAlterAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            YesNo plusMinus;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlusMinus;
        };
        
        class DegreeAlter;
        using DegreeAlterPtr = std::shared_ptr<DegreeAlter>;
        using DegreeAlterUPtr = std::unique_ptr<DegreeAlter>;
        using DegreeAlterSet = std::vector<DegreeAlterPtr>;
        using DegreeAlterSetIter = DegreeAlterSet::iterator;
        using DegreeAlterSetIterConst = DegreeAlterSet::const_iterator;
        inline DegreeAlterPtr makeDegreeAlter() { return std::make_shared<DegreeAlter>(); }
		inline DegreeAlterPtr makeDegreeAlter( const Semitones& value ) { return std::make_shared<DegreeAlter>( value ); }
		inline DegreeAlterPtr makeDegreeAlter( Semitones&& value ) { return std::make_shared<DegreeAlter>( std::move( value ) ); }

        class DegreeAlter : public ElementInterface
        {
        public:
            DegreeAlter();
            DegreeAlter( const Semitones& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DegreeAlterAttributesPtr getAttributes() const;
            void setAttributes( const DegreeAlterAttributesPtr& attributes );
            Semitones getValue() const;
            void setValue( const Semitones& value );

        private:
            Semitones myValue;
            DegreeAlterAttributesPtr myAttributes;
        };
        
        
        struct DegreeTypeAttributes;
        using DegreeTypeAttributesPtr = std::shared_ptr<DegreeTypeAttributes>;
        
        struct DegreeTypeAttributes : public AttributesInterface
        {
        public:
            DegreeTypeAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken text;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasText;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class DegreeType;
        using DegreeTypePtr = std::shared_ptr<DegreeType>;
        using DegreeTypeUPtr = std::unique_ptr<DegreeType>;
        using DegreeTypeSet = std::vector<DegreeTypePtr>;
        using DegreeTypeSetIter = DegreeTypeSet::iterator;
        using DegreeTypeSetIterConst = DegreeTypeSet::const_iterator;
        inline DegreeTypePtr makeDegreeType() { return std::make_shared<DegreeType>(); }
		inline DegreeTypePtr makeDegreeType( const DegreeTypeValue& value ) { return std::make_shared<DegreeType>( value ); }
		inline DegreeTypePtr makeDegreeType( DegreeTypeValue&& value ) { return std::make_shared<DegreeType>( std::move( value ) ); }

        class DegreeType : public ElementInterface
        {
        public:
            DegreeType();
            DegreeType( const DegreeTypeValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DegreeTypeAttributesPtr getAttributes() const;
            void setAttributes( const DegreeTypeAttributesPtr& attributes );
            DegreeTypeValue getValue() const;
            void setValue( const DegreeTypeValue& value );

        private:
            DegreeTypeValue myValue;
            DegreeTypeAttributesPtr myAttributes;
        };
        
        
        struct OffsetAttributes;
        using OffsetAttributesPtr = std::shared_ptr<OffsetAttributes>;
        
        struct OffsetAttributes : public AttributesInterface
        {
        public:
            OffsetAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo sound;
            bool hasSound;
        };
        
        class Offset;
        using OffsetPtr = std::shared_ptr<Offset>;
        using OffsetUPtr = std::unique_ptr<Offset>;
        using OffsetSet = std::vector<OffsetPtr>;
        using OffsetSetIter = OffsetSet::iterator;
        using OffsetSetIterConst = OffsetSet::const_iterator;
        inline OffsetPtr makeOffset() { return std::make_shared<Offset>(); }
		inline OffsetPtr makeOffset( const DivisionsValue& value ) { return std::make_shared<Offset>( value ); }
		inline OffsetPtr makeOffset( DivisionsValue&& value ) { return std::make_shared<Offset>( std::move( value ) ); }

        class Offset : public ElementInterface
        {
        public:
            Offset();
            Offset( const DivisionsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OffsetAttributesPtr getAttributes() const;
            void setAttributes( const OffsetAttributesPtr& attributes );
            DivisionsValue getValue() const;
            void setValue( const DivisionsValue& value );

        private:
            DivisionsValue myValue;
            OffsetAttributesPtr myAttributes;
        };
        
        
        struct RehearsalAttributes;
        using RehearsalAttributesPtr = std::shared_ptr<RehearsalAttributes>;
        
        struct RehearsalAttributes : public AttributesInterface
        {
        public:
            RehearsalAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            LeftCenterRight justify;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight halign;
            NumberOfLines underline;
            NumberOfLines overline;
            NumberOfLines lineThrough;
            RotationDegrees rotation;
            NumberOrNormal letterSpacing;
            NumberOrNormal lineHeight;
            XmlLang lang;
            XmlSpace space;
            EnclosureShape enclosure;
            bool hasJustify;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
            bool hasUnderline;
            bool hasOverline;
            bool hasLineThrough;
            bool hasRotation;
            bool hasLetterSpacing;
            bool hasLineHeight;
            bool hasLang;
            bool hasSpace;
            bool hasEnclosure;
        };
        
        class Rehearsal;
        using RehearsalPtr = std::shared_ptr<Rehearsal>;
        using RehearsalUPtr = std::unique_ptr<Rehearsal>;
        using RehearsalSet = std::vector<RehearsalPtr>;
        using RehearsalSetIter = RehearsalSet::iterator;
        using RehearsalSetIterConst = RehearsalSet::const_iterator;
        inline RehearsalPtr makeRehearsal() { return std::make_shared<Rehearsal>(); }
		inline RehearsalPtr makeRehearsal( const XsString& value ) { return std::make_shared<Rehearsal>( value ); }
		inline RehearsalPtr makeRehearsal( XsString&& value ) { return std::make_shared<Rehearsal>( std::move( value ) ); }

        class Rehearsal : public ElementInterface
        {
        public:
            Rehearsal();
            Rehearsal( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RehearsalAttributesPtr getAttributes() const;
            void setAttributes( const RehearsalAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            RehearsalAttributesPtr myAttributes;
        };
        
        
        struct WordsAttributes;
        using WordsAttributesPtr = std::shared_ptr<WordsAttributes>;
        
        struct WordsAttributes : public AttributesInterface
        {
        public:
            WordsAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            LeftCenterRight justify;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight halign;
            NumberOfLines underline;
            NumberOfLines overline;
            NumberOfLines lineThrough;
            RotationDegrees rotation;
            NumberOrNormal letterSpacing;
            NumberOrNormal lineHeight;
            XmlLang lang;
            XmlSpace space;
            EnclosureShape enclosure;
            bool hasJustify;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
            bool hasUnderline;
            bool hasOverline;
            bool hasLineThrough;
            bool hasRotation;
            bool hasLetterSpacing;
            bool hasLineHeight;
            bool hasLang;
            bool hasSpace;
            bool hasEnclosure;
        };
        
        class Words;
        using WordsPtr = std::shared_ptr<Words>;
        using WordsUPtr = std::unique_ptr<Words>;
        using WordsSet = std::vector<WordsPtr>;
        using WordsSetIter = WordsSet::iterator;
        using WordsSetIterConst = WordsSet::const_iterator;
        inline WordsPtr makeWords() { return std::make_shared<Words>(); }
		inline WordsPtr makeWords( const XsString& value ) { return std::make_shared<Words>( value ); }
		inline WordsPtr makeWords( XsString&& value ) { return std::make_shared<Words>( std::move( value ) ); }

        class Words : public ElementInterface
        {
        public:
            Words();
            Words( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            WordsAttributesPtr getAttributes() const;
            void setAttributes( const WordsAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            WordsAttributesPtr myAttributes;
        };
        
        
        struct PrincipalVoiceAttributes;
        using PrincipalVoiceAttributesPtr = std::shared_ptr<PrincipalVoiceAttributes>;
        
        struct PrincipalVoiceAttributes : public AttributesInterface
        {
        public:
            PrincipalVoiceAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            PrincipalVoiceSymbol symbol;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight halign;
            const bool hasType;
            const bool hasSymbol;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
        };
        
        class PrincipalVoice;
        using PrincipalVoicePtr = std::shared_ptr<PrincipalVoice>;
        using PrincipalVoiceUPtr = std::unique_ptr<PrincipalVoice>;
        using PrincipalVoiceSet = std::vector<PrincipalVoicePtr>;
        using PrincipalVoiceSetIter = PrincipalVoiceSet::iterator;
        using PrincipalVoiceSetIterConst = PrincipalVoiceSet::const_iterator;
        inline PrincipalVoicePtr makePrincipalVoice() { return std::make_shared<PrincipalVoice>(); }
		inline PrincipalVoicePtr makePrincipalVoice( const XsString& value ) { return std::make_shared<PrincipalVoice>( value ); }
		inline PrincipalVoicePtr makePrincipalVoice( XsString&& value ) { return std::make_shared<PrincipalVoice>( std::move( value ) ); }

        class PrincipalVoice : public ElementInterface
        {
        public:
            PrincipalVoice();
            PrincipalVoice( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PrincipalVoiceAttributesPtr getAttributes() const;
            void setAttributes( const PrincipalVoiceAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            PrincipalVoiceAttributesPtr myAttributes;
        };
        
        
        struct OtherDirectionAttributes;
        using OtherDirectionAttributesPtr = std::shared_ptr<OtherDirectionAttributes>;
        
        struct OtherDirectionAttributes : public AttributesInterface
        {
        public:
            OtherDirectionAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo printObject;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight halign;
            bool hasPrintObject;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
        };
        
        class OtherDirection;
        using OtherDirectionPtr = std::shared_ptr<OtherDirection>;
        using OtherDirectionUPtr = std::unique_ptr<OtherDirection>;
        using OtherDirectionSet = std::vector<OtherDirectionPtr>;
        using OtherDirectionSetIter = OtherDirectionSet::iterator;
        using OtherDirectionSetIterConst = OtherDirectionSet::const_iterator;
        inline OtherDirectionPtr makeOtherDirection() { return std::make_shared<OtherDirection>(); }
		inline OtherDirectionPtr makeOtherDirection( const XsString& value ) { return std::make_shared<OtherDirection>( value ); }
		inline OtherDirectionPtr makeOtherDirection( XsString&& value ) { return std::make_shared<OtherDirection>( std::move( value ) ); }

        class OtherDirection : public ElementInterface
        {
        public:
            OtherDirection();
            OtherDirection( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OtherDirectionAttributesPtr getAttributes() const;
            void setAttributes( const OtherDirectionAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            OtherDirectionAttributesPtr myAttributes;
        };
        
        
        struct FirstFretAttributes;
        using FirstFretAttributesPtr = std::shared_ptr<FirstFretAttributes>;
        
        struct FirstFretAttributes : public AttributesInterface
        {
        public:
            FirstFretAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken text;
            LeftRight location;
            bool hasText;
            bool hasLocation;
        };
        
        class FirstFret;
        using FirstFretPtr = std::shared_ptr<FirstFret>;
        using FirstFretUPtr = std::unique_ptr<FirstFret>;
        using FirstFretSet = std::vector<FirstFretPtr>;
        using FirstFretSetIter = FirstFretSet::iterator;
        using FirstFretSetIterConst = FirstFretSet::const_iterator;
        inline FirstFretPtr makeFirstFret() { return std::make_shared<FirstFret>(); }
		inline FirstFretPtr makeFirstFret( const PositiveInteger& value ) { return std::make_shared<FirstFret>( value ); }
		inline FirstFretPtr makeFirstFret( PositiveInteger&& value ) { return std::make_shared<FirstFret>( std::move( value ) ); }

        class FirstFret : public ElementInterface
        {
        public:
            FirstFret();
            FirstFret( const PositiveInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FirstFretAttributesPtr getAttributes() const;
            void setAttributes( const FirstFretAttributesPtr& attributes );
            PositiveInteger getValue() const;
            void setValue( const PositiveInteger& value );

        private:
            PositiveInteger myValue;
            FirstFretAttributesPtr myAttributes;
        };
        
        
        struct FretAttributes;
        using FretAttributesPtr = std::shared_ptr<FretAttributes>;
        
        struct FretAttributes : public AttributesInterface
        {
        public:
            FretAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class Fret;
        using FretPtr = std::shared_ptr<Fret>;
        using FretUPtr = std::unique_ptr<Fret>;
        using FretSet = std::vector<FretPtr>;
        using FretSetIter = FretSet::iterator;
        using FretSetIterConst = FretSet::const_iterator;
        inline FretPtr makeFret() { return std::make_shared<Fret>(); }
		inline FretPtr makeFret( const NonNegativeInteger& value ) { return std::make_shared<Fret>( value ); }
		inline FretPtr makeFret( NonNegativeInteger&& value ) { return std::make_shared<Fret>( std::move( value ) ); }

        class Fret : public ElementInterface
        {
        public:
            Fret();
            Fret( const NonNegativeInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FretAttributesPtr getAttributes() const;
            void setAttributes( const FretAttributesPtr& attributes );
            NonNegativeInteger getValue() const;
            void setValue( const NonNegativeInteger& value );

        private:
            NonNegativeInteger myValue;
            FretAttributesPtr myAttributes;
        };
        
        
        struct FingeringAttributes;
        using FingeringAttributesPtr = std::shared_ptr<FingeringAttributes>;
        
        struct FingeringAttributes : public AttributesInterface
        {
        public:
            FingeringAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo substitution;
            YesNo alternate;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            bool hasSubstitution;
            bool hasAlternate;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class Fingering;
        using FingeringPtr = std::shared_ptr<Fingering>;
        using FingeringUPtr = std::unique_ptr<Fingering>;
        using FingeringSet = std::vector<FingeringPtr>;
        using FingeringSetIter = FingeringSet::iterator;
        using FingeringSetIterConst = FingeringSet::const_iterator;
        inline FingeringPtr makeFingering() { return std::make_shared<Fingering>(); }
		inline FingeringPtr makeFingering( const XsString& value ) { return std::make_shared<Fingering>( value ); }
		inline FingeringPtr makeFingering( XsString&& value ) { return std::make_shared<Fingering>( std::move( value ) ); }

        class Fingering : public ElementInterface
        {
        public:
            Fingering();
            Fingering( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FingeringAttributesPtr getAttributes() const;
            void setAttributes( const FingeringAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            FingeringAttributesPtr myAttributes;
        };
        
        
        struct FeatureAttributes;
        using FeatureAttributesPtr = std::shared_ptr<FeatureAttributes>;
        
        struct FeatureAttributes : public AttributesInterface
        {
        public:
            FeatureAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken type;
            bool hasType;
        };
        
        class Feature;
        using FeaturePtr = std::shared_ptr<Feature>;
        using FeatureUPtr = std::unique_ptr<Feature>;
        using FeatureSet = std::vector<FeaturePtr>;
        using FeatureSetIter = FeatureSet::iterator;
        using FeatureSetIterConst = FeatureSet::const_iterator;
        inline FeaturePtr makeFeature() { return std::make_shared<Feature>(); }
		inline FeaturePtr makeFeature( const XsString& value ) { return std::make_shared<Feature>( value ); }
		inline FeaturePtr makeFeature( XsString&& value ) { return std::make_shared<Feature>( std::move( value ) ); }

        class Feature : public ElementInterface
        {
        public:
            Feature();
            Feature( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FeatureAttributesPtr getAttributes() const;
            void setAttributes( const FeatureAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            FeatureAttributesPtr myAttributes;
        };
        
        
        
        
        
        struct PerMinuteAttributes;
        using PerMinuteAttributesPtr = std::shared_ptr<PerMinuteAttributes>;
        
        struct PerMinuteAttributes : public AttributesInterface
        {
        public:
            PerMinuteAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class PerMinute;
        using PerMinutePtr = std::shared_ptr<PerMinute>;
        using PerMinuteUPtr = std::unique_ptr<PerMinute>;
        using PerMinuteSet = std::vector<PerMinutePtr>;
        using PerMinuteSetIter = PerMinuteSet::iterator;
        using PerMinuteSetIterConst = PerMinuteSet::const_iterator;
        inline PerMinutePtr makePerMinute() { return std::make_shared<PerMinute>(); }
		inline PerMinutePtr makePerMinute( const XsString& value ) { return std::make_shared<PerMinute>( value ); }
		inline PerMinutePtr makePerMinute( XsString&& value ) { return std::make_shared<PerMinute>( std::move( value ) ); }

        class PerMinute : public ElementInterface
        {
        public:
            PerMinute();
            PerMinute( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PerMinuteAttributesPtr getAttributes() const;
            void setAttributes( const PerMinuteAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            PerMinuteAttributesPtr myAttributes;
        };
        
        
        struct MetronomeBeamAttributes;
        using MetronomeBeamAttributesPtr = std::shared_ptr<MetronomeBeamAttributes>;
        
        struct MetronomeBeamAttributes : public AttributesInterface
        {
        public:
            MetronomeBeamAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            BeamLevel number;
            bool hasNumber;
        };
        
        class MetronomeBeam;
        using MetronomeBeamPtr = std::shared_ptr<MetronomeBeam>;
        using MetronomeBeamUPtr = std::unique_ptr<MetronomeBeam>;
        using MetronomeBeamSet = std::vector<MetronomeBeamPtr>;
        using MetronomeBeamSetIter = MetronomeBeamSet::iterator;
        using MetronomeBeamSetIterConst = MetronomeBeamSet::const_iterator;
        inline MetronomeBeamPtr makeMetronomeBeam() { return std::make_shared<MetronomeBeam>(); }
		inline MetronomeBeamPtr makeMetronomeBeam( const BeamValue& value ) { return std::make_shared<MetronomeBeam>( value ); }
		inline MetronomeBeamPtr makeMetronomeBeam( BeamValue&& value ) { return std::make_shared<MetronomeBeam>( std::move( value ) ); }

        class MetronomeBeam : public ElementInterface
        {
        public:
            MetronomeBeam();
            MetronomeBeam( const BeamValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MetronomeBeamAttributesPtr getAttributes() const;
            void setAttributes( const MetronomeBeamAttributesPtr& attributes );
            BeamValue getValue() const;
            void setValue( const BeamValue& value );

        private:
            BeamValue myValue;
            MetronomeBeamAttributesPtr myAttributes;
        };
        
        
        struct BeaterAttributes;
        using BeaterAttributesPtr = std::shared_ptr<BeaterAttributes>;
        
        struct BeaterAttributes : public AttributesInterface
        {
        public:
            BeaterAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TipDirection tip;
            bool hasTip;
        };
        
        class Beater;
        using BeaterPtr = std::shared_ptr<Beater>;
        using BeaterUPtr = std::unique_ptr<Beater>;
        using BeaterSet = std::vector<BeaterPtr>;
        using BeaterSetIter = BeaterSet::iterator;
        using BeaterSetIterConst = BeaterSet::const_iterator;
        inline BeaterPtr makeBeater() { return std::make_shared<Beater>(); }
		inline BeaterPtr makeBeater( const BeaterValue& value ) { return std::make_shared<Beater>( value ); }
		inline BeaterPtr makeBeater( BeaterValue&& value ) { return std::make_shared<Beater>( std::move( value ) ); }

        class Beater : public ElementInterface
        {
        public:
            Beater();
            Beater( const BeaterValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BeaterAttributesPtr getAttributes() const;
            void setAttributes( const BeaterAttributesPtr& attributes );
            BeaterValue getValue() const;
            void setValue( const BeaterValue& value );

        private:
            BeaterValue myValue;
            BeaterAttributesPtr myAttributes;
        };
        
        
        struct MeasureNumberingAttributes;
        using MeasureNumberingAttributesPtr = std::shared_ptr<MeasureNumberingAttributes>;
        
        struct MeasureNumberingAttributes : public AttributesInterface
        {
        public:
            MeasureNumberingAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight halign;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
        };
        
        class MeasureNumbering;
        using MeasureNumberingPtr = std::shared_ptr<MeasureNumbering>;
        using MeasureNumberingUPtr = std::unique_ptr<MeasureNumbering>;
        using MeasureNumberingSet = std::vector<MeasureNumberingPtr>;
        using MeasureNumberingSetIter = MeasureNumberingSet::iterator;
        using MeasureNumberingSetIterConst = MeasureNumberingSet::const_iterator;
        inline MeasureNumberingPtr makeMeasureNumbering() { return std::make_shared<MeasureNumbering>(); }
		inline MeasureNumberingPtr makeMeasureNumbering( const MeasureNumberingValue& value ) { return std::make_shared<MeasureNumbering>( value ); }
		inline MeasureNumberingPtr makeMeasureNumbering( MeasureNumberingValue&& value ) { return std::make_shared<MeasureNumbering>( std::move( value ) ); }

        class MeasureNumbering : public ElementInterface
        {
        public:
            MeasureNumbering();
            MeasureNumbering( const MeasureNumberingValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MeasureNumberingAttributesPtr getAttributes() const;
            void setAttributes( const MeasureNumberingAttributesPtr& attributes );
            MeasureNumberingValue getValue() const;
            void setValue( const MeasureNumberingValue& value );

        private:
            MeasureNumberingValue myValue;
            MeasureNumberingAttributesPtr myAttributes;
        };
        
        
        struct RootStepAttributes;
        using RootStepAttributesPtr = std::shared_ptr<RootStepAttributes>;
        
        struct RootStepAttributes : public AttributesInterface
        {
        public:
            RootStepAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken text;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasText;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class RootStep;
        using RootStepPtr = std::shared_ptr<RootStep>;
        using RootStepUPtr = std::unique_ptr<RootStep>;
        using RootStepSet = std::vector<RootStepPtr>;
        using RootStepSetIter = RootStepSet::iterator;
        using RootStepSetIterConst = RootStepSet::const_iterator;
        inline RootStepPtr makeRootStep() { return std::make_shared<RootStep>(); }
		inline RootStepPtr makeRootStep( const StepEnum& value ) { return std::make_shared<RootStep>( value ); }
		inline RootStepPtr makeRootStep( StepEnum&& value ) { return std::make_shared<RootStep>( std::move( value ) ); }

        class RootStep : public ElementInterface
        {
        public:
            RootStep();
            RootStep( const StepEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RootStepAttributesPtr getAttributes() const;
            void setAttributes( const RootStepAttributesPtr& attributes );
            StepEnum getValue() const;
            void setValue( const StepEnum& value );

        private:
            StepEnum myValue;
            RootStepAttributesPtr myAttributes;
        };
        
        
        struct RootAlterAttributes;
        using RootAlterAttributesPtr = std::shared_ptr<RootAlterAttributes>;
        
        struct RootAlterAttributes : public AttributesInterface
        {
        public:
            RootAlterAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo printObject;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftRight location;
            bool hasPrintObject;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasLocation;
        };
        
        class RootAlter;
        using RootAlterPtr = std::shared_ptr<RootAlter>;
        using RootAlterUPtr = std::unique_ptr<RootAlter>;
        using RootAlterSet = std::vector<RootAlterPtr>;
        using RootAlterSetIter = RootAlterSet::iterator;
        using RootAlterSetIterConst = RootAlterSet::const_iterator;
        inline RootAlterPtr makeRootAlter() { return std::make_shared<RootAlter>(); }
		inline RootAlterPtr makeRootAlter( const Semitones& value ) { return std::make_shared<RootAlter>( value ); }
		inline RootAlterPtr makeRootAlter( Semitones&& value ) { return std::make_shared<RootAlter>( std::move( value ) ); }

        class RootAlter : public ElementInterface
        {
        public:
            RootAlter();
            RootAlter( const Semitones& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RootAlterAttributesPtr getAttributes() const;
            void setAttributes( const RootAlterAttributesPtr& attributes );
            Semitones getValue() const;
            void setValue( const Semitones& value );

        private:
            Semitones myValue;
            RootAlterAttributesPtr myAttributes;
        };
        
        
        struct MidiDeviceAttributes;
        using MidiDeviceAttributesPtr = std::shared_ptr<MidiDeviceAttributes>;
        
        struct MidiDeviceAttributes : public AttributesInterface
        {
        public:
            MidiDeviceAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            Midi16 port;
            XsIDREF id;
            bool hasPort;
            bool hasId;
        };
        
        class MidiDevice;
        using MidiDevicePtr = std::shared_ptr<MidiDevice>;
        using MidiDeviceUPtr = std::unique_ptr<MidiDevice>;
        using MidiDeviceSet = std::vector<MidiDevicePtr>;
        using MidiDeviceSetIter = MidiDeviceSet::iterator;
        using MidiDeviceSetIterConst = MidiDeviceSet::const_iterator;
        inline MidiDevicePtr makeMidiDevice() { return std::make_shared<MidiDevice>(); }
		inline MidiDevicePtr makeMidiDevice( const XsString& value ) { return std::make_shared<MidiDevice>( value ); }
		inline MidiDevicePtr makeMidiDevice( XsString&& value ) { return std::make_shared<MidiDevice>( std::move( value ) ); }

        class MidiDevice : public ElementInterface
        {
        public:
            MidiDevice();
            MidiDevice( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MidiDeviceAttributesPtr getAttributes() const;
            void setAttributes( const MidiDeviceAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            MidiDeviceAttributesPtr myAttributes;
        };
        
        
        
        struct EncoderAttributes;
        using EncoderAttributesPtr = std::shared_ptr<EncoderAttributes>;
        
        struct EncoderAttributes : public AttributesInterface
        {
        public:
            EncoderAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken type;
            bool hasType;
        };
        
        class Encoder;
        using EncoderPtr = std::shared_ptr<Encoder>;
        using EncoderUPtr = std::unique_ptr<Encoder>;
        using EncoderSet = std::vector<EncoderPtr>;
        using EncoderSetIter = EncoderSet::iterator;
        using EncoderSetIterConst = EncoderSet::const_iterator;
        inline EncoderPtr makeEncoder() { return std::make_shared<Encoder>(); }
		inline EncoderPtr makeEncoder( const XsString& value ) { return std::make_shared<Encoder>( value ); }
		inline EncoderPtr makeEncoder( XsString&& value ) { return std::make_shared<Encoder>( std::move( value ) ); }

        class Encoder : public ElementInterface
        {
        public:
            Encoder();
            Encoder( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EncoderAttributesPtr getAttributes() const;
            void setAttributes( const EncoderAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            EncoderAttributesPtr myAttributes;
        };
        
        
        struct CreatorAttributes;
        using CreatorAttributesPtr = std::shared_ptr<CreatorAttributes>;
        
        struct CreatorAttributes : public AttributesInterface
        {
        public:
            CreatorAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken type;
            bool hasType;
        };
        
        class Creator;
        using CreatorPtr = std::shared_ptr<Creator>;
        using CreatorUPtr = std::unique_ptr<Creator>;
        using CreatorSet = std::vector<CreatorPtr>;
        using CreatorSetIter = CreatorSet::iterator;
        using CreatorSetIterConst = CreatorSet::const_iterator;
        inline CreatorPtr makeCreator() { return std::make_shared<Creator>(); }
		inline CreatorPtr makeCreator( const XsString& value ) { return std::make_shared<Creator>( value ); }
		inline CreatorPtr makeCreator( XsString&& value ) { return std::make_shared<Creator>( std::move( value ) ); }

        class Creator : public ElementInterface
        {
        public:
            Creator();
            Creator( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            CreatorAttributesPtr getAttributes() const;
            void setAttributes( const CreatorAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            CreatorAttributesPtr myAttributes;
        };
        
        
        struct RightsAttributes;
        using RightsAttributesPtr = std::shared_ptr<RightsAttributes>;
        
        struct RightsAttributes : public AttributesInterface
        {
        public:
            RightsAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken type;
            bool hasType;
        };
        
        class Rights;
        using RightsPtr = std::shared_ptr<Rights>;
        using RightsUPtr = std::unique_ptr<Rights>;
        using RightsSet = std::vector<RightsPtr>;
        using RightsSetIter = RightsSet::iterator;
        using RightsSetIterConst = RightsSet::const_iterator;
        inline RightsPtr makeRights() { return std::make_shared<Rights>(); }
		inline RightsPtr makeRights( const XsString& value ) { return std::make_shared<Rights>( value ); }
		inline RightsPtr makeRights( XsString&& value ) { return std::make_shared<Rights>( std::move( value ) ); }

        class Rights : public ElementInterface
        {
        public:
            Rights();
            Rights( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RightsAttributesPtr getAttributes() const;
            void setAttributes( const RightsAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            RightsAttributesPtr myAttributes;
        };
        
        
        struct RelationAttributes;
        using RelationAttributesPtr = std::shared_ptr<RelationAttributes>;
        
        struct RelationAttributes : public AttributesInterface
        {
        public:
            RelationAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken type;
            bool hasType;
        };
        
        class Relation;
        using RelationPtr = std::shared_ptr<Relation>;
        using RelationUPtr = std::unique_ptr<Relation>;
        using RelationSet = std::vector<RelationPtr>;
        using RelationSetIter = RelationSet::iterator;
        using RelationSetIterConst = RelationSet::const_iterator;
        inline RelationPtr makeRelation() { return std::make_shared<Relation>(); }
		inline RelationPtr makeRelation( const XsString& value ) { return std::make_shared<Relation>( value ); }
		inline RelationPtr makeRelation( XsString&& value ) { return std::make_shared<Relation>( std::move( value ) ); }

        class Relation : public ElementInterface
        {
        public:
            Relation();
            Relation( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RelationAttributesPtr getAttributes() const;
            void setAttributes( const RelationAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            RelationAttributesPtr myAttributes;
        };
        
        
        struct MiscellaneousFieldAttributes;
        using MiscellaneousFieldAttributesPtr = std::shared_ptr<MiscellaneousFieldAttributes>;
        
        struct MiscellaneousFieldAttributes : public AttributesInterface
        {
        public:
            MiscellaneousFieldAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken name;
            const bool hasName;
        };
        
        class MiscellaneousField;
        using MiscellaneousFieldPtr = std::shared_ptr<MiscellaneousField>;
        using MiscellaneousFieldUPtr = std::unique_ptr<MiscellaneousField>;
        using MiscellaneousFieldSet = std::vector<MiscellaneousFieldPtr>;
        using MiscellaneousFieldSetIter = MiscellaneousFieldSet::iterator;
        using MiscellaneousFieldSetIterConst = MiscellaneousFieldSet::const_iterator;
        inline MiscellaneousFieldPtr makeMiscellaneousField() { return std::make_shared<MiscellaneousField>(); }
		inline MiscellaneousFieldPtr makeMiscellaneousField( const XsString& value ) { return std::make_shared<MiscellaneousField>( value ); }
		inline MiscellaneousFieldPtr makeMiscellaneousField( XsString&& value ) { return std::make_shared<MiscellaneousField>( std::move( value ) ); }

        class MiscellaneousField : public ElementInterface
        {
        public:
            MiscellaneousField();
            MiscellaneousField( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MiscellaneousFieldAttributesPtr getAttributes() const;
            void setAttributes( const MiscellaneousFieldAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            MiscellaneousFieldAttributesPtr myAttributes;
        };
        
        
        struct LineWidthAttributes;
        using LineWidthAttributesPtr = std::shared_ptr<LineWidthAttributes>;
        
        struct LineWidthAttributes : public AttributesInterface
        {
        public:
            LineWidthAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            LineWidthType type;
            const bool hasType;
        };
        
        class LineWidth;
        using LineWidthPtr = std::shared_ptr<LineWidth>;
        using LineWidthUPtr = std::unique_ptr<LineWidth>;
        using LineWidthSet = std::vector<LineWidthPtr>;
        using LineWidthSetIter = LineWidthSet::iterator;
        using LineWidthSetIterConst = LineWidthSet::const_iterator;
        inline LineWidthPtr makeLineWidth() { return std::make_shared<LineWidth>(); }
		inline LineWidthPtr makeLineWidth( const TenthsValue& value ) { return std::make_shared<LineWidth>( value ); }
		inline LineWidthPtr makeLineWidth( TenthsValue&& value ) { return std::make_shared<LineWidth>( std::move( value ) ); }

        class LineWidth : public ElementInterface
        {
        public:
            LineWidth();
            LineWidth( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            LineWidthAttributesPtr getAttributes() const;
            void setAttributes( const LineWidthAttributesPtr& attributes );
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

        private:
            TenthsValue myValue;
            LineWidthAttributesPtr myAttributes;
        };
        
        
        struct NoteSizeAttributes;
        using NoteSizeAttributesPtr = std::shared_ptr<NoteSizeAttributes>;
        
        struct NoteSizeAttributes : public AttributesInterface
        {
        public:
            NoteSizeAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            NoteSizeType type;
            const bool hasType;
        };
        
        class NoteSize;
        using NoteSizePtr = std::shared_ptr<NoteSize>;
        using NoteSizeUPtr = std::unique_ptr<NoteSize>;
        using NoteSizeSet = std::vector<NoteSizePtr>;
        using NoteSizeSetIter = NoteSizeSet::iterator;
        using NoteSizeSetIterConst = NoteSizeSet::const_iterator;
        inline NoteSizePtr makeNoteSize() { return std::make_shared<NoteSize>(); }
		inline NoteSizePtr makeNoteSize( const NonNegativeDecimal& value ) { return std::make_shared<NoteSize>( value ); }
		inline NoteSizePtr makeNoteSize( NonNegativeDecimal&& value ) { return std::make_shared<NoteSize>( std::move( value ) ); }

        class NoteSize : public ElementInterface
        {
        public:
            NoteSize();
            NoteSize( const NonNegativeDecimal& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NoteSizeAttributesPtr getAttributes() const;
            void setAttributes( const NoteSizeAttributesPtr& attributes );
            NonNegativeDecimal getValue() const;
            void setValue( const NonNegativeDecimal& value );

        private:
            NonNegativeDecimal myValue;
            NoteSizeAttributesPtr myAttributes;
        };
        
        
        struct DistanceAttributes;
        using DistanceAttributesPtr = std::shared_ptr<DistanceAttributes>;
        
        struct DistanceAttributes : public AttributesInterface
        {
        public:
            DistanceAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            DistanceType type;
            const bool hasType;
        };
        
        class Distance;
        using DistancePtr = std::shared_ptr<Distance>;
        using DistanceUPtr = std::unique_ptr<Distance>;
        using DistanceSet = std::vector<DistancePtr>;
        using DistanceSetIter = DistanceSet::iterator;
        using DistanceSetIterConst = DistanceSet::const_iterator;
        inline DistancePtr makeDistance() { return std::make_shared<Distance>(); }
		inline DistancePtr makeDistance( const TenthsValue& value ) { return std::make_shared<Distance>( value ); }
		inline DistancePtr makeDistance( TenthsValue&& value ) { return std::make_shared<Distance>( std::move( value ) ); }

        class Distance : public ElementInterface
        {
        public:
            Distance();
            Distance( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DistanceAttributesPtr getAttributes() const;
            void setAttributes( const DistanceAttributesPtr& attributes );
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

        private:
            TenthsValue myValue;
            DistanceAttributesPtr myAttributes;
        };
        
        
        struct OtherAppearanceAttributes;
        using OtherAppearanceAttributesPtr = std::shared_ptr<OtherAppearanceAttributes>;
        
        struct OtherAppearanceAttributes : public AttributesInterface
        {
        public:
            OtherAppearanceAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken type;
            const bool hasType;
        };
        
        class OtherAppearance;
        using OtherAppearancePtr = std::shared_ptr<OtherAppearance>;
        using OtherAppearanceUPtr = std::unique_ptr<OtherAppearance>;
        using OtherAppearanceSet = std::vector<OtherAppearancePtr>;
        using OtherAppearanceSetIter = OtherAppearanceSet::iterator;
        using OtherAppearanceSetIterConst = OtherAppearanceSet::const_iterator;
        inline OtherAppearancePtr makeOtherAppearance() { return std::make_shared<OtherAppearance>(); }
		inline OtherAppearancePtr makeOtherAppearance( const XsString& value ) { return std::make_shared<OtherAppearance>( value ); }
		inline OtherAppearancePtr makeOtherAppearance( XsString&& value ) { return std::make_shared<OtherAppearance>( std::move( value ) ); }

        class OtherAppearance : public ElementInterface
        {
        public:
            OtherAppearance();
            OtherAppearance( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OtherAppearanceAttributesPtr getAttributes() const;
            void setAttributes( const OtherAppearanceAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            OtherAppearanceAttributesPtr myAttributes;
        };
        
        
        struct BreathMarkAttributes;
        using BreathMarkAttributesPtr = std::shared_ptr<BreathMarkAttributes>;
        
        struct BreathMarkAttributes : public AttributesInterface
        {
        public:
            BreathMarkAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class BreathMark;
        using BreathMarkPtr = std::shared_ptr<BreathMark>;
        using BreathMarkUPtr = std::unique_ptr<BreathMark>;
        using BreathMarkSet = std::vector<BreathMarkPtr>;
        using BreathMarkSetIter = BreathMarkSet::iterator;
        using BreathMarkSetIterConst = BreathMarkSet::const_iterator;
        inline BreathMarkPtr makeBreathMark() { return std::make_shared<BreathMark>(); }
		inline BreathMarkPtr makeBreathMark( const BreathMarkValue& value ) { return std::make_shared<BreathMark>( value ); }
		inline BreathMarkPtr makeBreathMark( BreathMarkValue&& value ) { return std::make_shared<BreathMark>( std::move( value ) ); }

        class BreathMark : public ElementInterface
        {
        public:
            BreathMark();
            BreathMark( const BreathMarkValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BreathMarkAttributesPtr getAttributes() const;
            void setAttributes( const BreathMarkAttributesPtr& attributes );
            BreathMarkValue getValue() const;
            void setValue( const BreathMarkValue& value );

        private:
            BreathMarkValue myValue;
            BreathMarkAttributesPtr myAttributes;
        };
        
        
        struct OtherArticulationAttributes;
        using OtherArticulationAttributesPtr = std::shared_ptr<OtherArticulationAttributes>;
        
        struct OtherArticulationAttributes : public AttributesInterface
        {
        public:
            OtherArticulationAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class OtherArticulation;
        using OtherArticulationPtr = std::shared_ptr<OtherArticulation>;
        using OtherArticulationUPtr = std::unique_ptr<OtherArticulation>;
        using OtherArticulationSet = std::vector<OtherArticulationPtr>;
        using OtherArticulationSetIter = OtherArticulationSet::iterator;
        using OtherArticulationSetIterConst = OtherArticulationSet::const_iterator;
        inline OtherArticulationPtr makeOtherArticulation() { return std::make_shared<OtherArticulation>(); }
		inline OtherArticulationPtr makeOtherArticulation( const XsString& value ) { return std::make_shared<OtherArticulation>( value ); }
		inline OtherArticulationPtr makeOtherArticulation( XsString&& value ) { return std::make_shared<OtherArticulation>( std::move( value ) ); }

        class OtherArticulation : public ElementInterface
        {
        public:
            OtherArticulation();
            OtherArticulation( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OtherArticulationAttributesPtr getAttributes() const;
            void setAttributes( const OtherArticulationAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            OtherArticulationAttributesPtr myAttributes;
        };
        
        
        struct WithBarAttributes;
        using WithBarAttributesPtr = std::shared_ptr<WithBarAttributes>;
        
        struct WithBarAttributes : public AttributesInterface
        {
        public:
            WithBarAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class WithBar;
        using WithBarPtr = std::shared_ptr<WithBar>;
        using WithBarUPtr = std::unique_ptr<WithBar>;
        using WithBarSet = std::vector<WithBarPtr>;
        using WithBarSetIter = WithBarSet::iterator;
        using WithBarSetIterConst = WithBarSet::const_iterator;
        inline WithBarPtr makeWithBar() { return std::make_shared<WithBar>(); }
		inline WithBarPtr makeWithBar( const XsString& value ) { return std::make_shared<WithBar>( value ); }
		inline WithBarPtr makeWithBar( XsString&& value ) { return std::make_shared<WithBar>( std::move( value ) ); }

        class WithBar : public ElementInterface
        {
        public:
            WithBar();
            WithBar( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            WithBarAttributesPtr getAttributes() const;
            void setAttributes( const WithBarAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            WithBarAttributesPtr myAttributes;
        };
        
        
        struct PrefixAttributes;
        using PrefixAttributesPtr = std::shared_ptr<PrefixAttributes>;
        
        struct PrefixAttributes : public AttributesInterface
        {
        public:
            PrefixAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class Prefix;
        using PrefixPtr = std::shared_ptr<Prefix>;
        using PrefixUPtr = std::unique_ptr<Prefix>;
        using PrefixSet = std::vector<PrefixPtr>;
        using PrefixSetIter = PrefixSet::iterator;
        using PrefixSetIterConst = PrefixSet::const_iterator;
        inline PrefixPtr makePrefix() { return std::make_shared<Prefix>(); }
		inline PrefixPtr makePrefix( const XsString& value ) { return std::make_shared<Prefix>( value ); }
		inline PrefixPtr makePrefix( XsString&& value ) { return std::make_shared<Prefix>( std::move( value ) ); }

        class Prefix : public ElementInterface
        {
        public:
            Prefix();
            Prefix( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PrefixAttributesPtr getAttributes() const;
            void setAttributes( const PrefixAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            PrefixAttributesPtr myAttributes;
        };
        
        
        struct FigureNumberAttributes;
        using FigureNumberAttributesPtr = std::shared_ptr<FigureNumberAttributes>;
        
        struct FigureNumberAttributes : public AttributesInterface
        {
        public:
            FigureNumberAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class FigureNumber;
        using FigureNumberPtr = std::shared_ptr<FigureNumber>;
        using FigureNumberUPtr = std::unique_ptr<FigureNumber>;
        using FigureNumberSet = std::vector<FigureNumberPtr>;
        using FigureNumberSetIter = FigureNumberSet::iterator;
        using FigureNumberSetIterConst = FigureNumberSet::const_iterator;
        inline FigureNumberPtr makeFigureNumber() { return std::make_shared<FigureNumber>(); }
		inline FigureNumberPtr makeFigureNumber( const XsString& value ) { return std::make_shared<FigureNumber>( value ); }
		inline FigureNumberPtr makeFigureNumber( XsString&& value ) { return std::make_shared<FigureNumber>( std::move( value ) ); }

        class FigureNumber : public ElementInterface
        {
        public:
            FigureNumber();
            FigureNumber( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FigureNumberAttributesPtr getAttributes() const;
            void setAttributes( const FigureNumberAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            FigureNumberAttributesPtr myAttributes;
        };
        
        
        struct SuffixAttributes;
        using SuffixAttributesPtr = std::shared_ptr<SuffixAttributes>;
        
        struct SuffixAttributes : public AttributesInterface
        {
        public:
            SuffixAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class Suffix;
        using SuffixPtr = std::shared_ptr<Suffix>;
        using SuffixUPtr = std::unique_ptr<Suffix>;
        using SuffixSet = std::vector<SuffixPtr>;
        using SuffixSetIter = SuffixSet::iterator;
        using SuffixSetIterConst = SuffixSet::const_iterator;
        inline SuffixPtr makeSuffix() { return std::make_shared<Suffix>(); }
		inline SuffixPtr makeSuffix( const XsString& value ) { return std::make_shared<Suffix>( value ); }
		inline SuffixPtr makeSuffix( XsString&& value ) { return std::make_shared<Suffix>( std::move( value ) ); }

        class Suffix : public ElementInterface
        {
        public:
            Suffix();
            Suffix( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            SuffixAttributesPtr getAttributes() const;
            void setAttributes( const SuffixAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            SuffixAttributesPtr myAttributes;
        };
        
        
        struct HoleClosedAttributes;
        using HoleClosedAttributesPtr = std::shared_ptr<HoleClosedAttributes>;
        
        struct HoleClosedAttributes : public AttributesInterface
        {
        public:
            HoleClosedAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            HoleClosedLocation location;
            bool hasLocation;
        };
        
        class HoleClosed;
        using HoleClosedPtr = std::shared_ptr<HoleClosed>;
        using HoleClosedUPtr = std::unique_ptr<HoleClosed>;
        using HoleClosedSet = std::vector<HoleClosedPtr>;
        using HoleClosedSetIter = HoleClosedSet::iterator;
        using HoleClosedSetIterConst = HoleClosedSet::const_iterator;
        inline HoleClosedPtr makeHoleClosed() { return std::make_shared<HoleClosed>(); }
		inline HoleClosedPtr makeHoleClosed( const HoleClosedValue& value ) { return std::make_shared<HoleClosed>( value ); }
		inline HoleClosedPtr makeHoleClosed( HoleClosedValue&& value ) { return std::make_shared<HoleClosed>( std::move( value ) ); }

        class HoleClosed : public ElementInterface
        {
        public:
            HoleClosed();
            HoleClosed( const HoleClosedValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HoleClosedAttributesPtr getAttributes() const;
            void setAttributes( const HoleClosedAttributesPtr& attributes );
            HoleClosedValue getValue() const;
            void setValue( const HoleClosedValue& value );

        private:
            HoleClosedValue myValue;
            HoleClosedAttributesPtr myAttributes;
        };
        
        
        struct TextAttributes;
        using TextAttributesPtr = std::shared_ptr<TextAttributes>;
        
        struct TextAttributes : public AttributesInterface
        {
        public:
            TextAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            NumberOfLines underline;
            NumberOfLines overline;
            NumberOfLines lineThrough;
            RotationDegrees rotation;
            NumberOrNormal letterSpacing;
            XmlLang lang;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasUnderline;
            bool hasOverline;
            bool hasLineThrough;
            bool hasRotation;
            bool hasLetterSpacing;
            bool hasLang;
        };
        
        class Text;
        using TextPtr = std::shared_ptr<Text>;
        using TextUPtr = std::unique_ptr<Text>;
        using TextSet = std::vector<TextPtr>;
        using TextSetIter = TextSet::iterator;
        using TextSetIterConst = TextSet::const_iterator;
        inline TextPtr makeText() { return std::make_shared<Text>(); }
		inline TextPtr makeText( const XsString& value ) { return std::make_shared<Text>( value ); }
		inline TextPtr makeText( XsString&& value ) { return std::make_shared<Text>( std::move( value ) ); }

        class Text : public ElementInterface
        {
        public:
            Text();
            Text( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TextAttributesPtr getAttributes() const;
            void setAttributes( const TextAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            TextAttributesPtr myAttributes;
        };
        
        
        struct ElisionAttributes;
        using ElisionAttributesPtr = std::shared_ptr<ElisionAttributes>;
        
        struct ElisionAttributes : public AttributesInterface
        {
        public:
            ElisionAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            NumberOfLines underline;
            NumberOfLines overline;
            NumberOfLines lineThrough;
            RotationDegrees rotation;
            NumberOrNormal letterSpacing;
            XmlLang lang;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasUnderline;
            bool hasOverline;
            bool hasLineThrough;
            bool hasRotation;
            bool hasLetterSpacing;
            bool hasLang;
        };
        
        class Elision;
        using ElisionPtr = std::shared_ptr<Elision>;
        using ElisionUPtr = std::unique_ptr<Elision>;
        using ElisionSet = std::vector<ElisionPtr>;
        using ElisionSetIter = ElisionSet::iterator;
        using ElisionSetIterConst = ElisionSet::const_iterator;
        inline ElisionPtr makeElision() { return std::make_shared<Elision>(); }
		inline ElisionPtr makeElision( const XsString& value ) { return std::make_shared<Elision>( value ); }
		inline ElisionPtr makeElision( XsString&& value ) { return std::make_shared<Elision>( std::move( value ) ); }

        class Elision : public ElementInterface
        {
        public:
            Elision();
            Elision( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ElisionAttributesPtr getAttributes() const;
            void setAttributes( const ElisionAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            ElisionAttributesPtr myAttributes;
        };
        
        
        
        
        
        struct GlissandoAttributes;
        using GlissandoAttributesPtr = std::shared_ptr<GlissandoAttributes>;
        
        struct GlissandoAttributes : public AttributesInterface
        {
        public:
            GlissandoAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            NumberLevel number;
            TenthsValue dashLength;
            TenthsValue spaceLength;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            const bool hasType;
            bool hasNumber;
            bool hasDashLength;
            bool hasSpaceLength;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class Glissando;
        using GlissandoPtr = std::shared_ptr<Glissando>;
        using GlissandoUPtr = std::unique_ptr<Glissando>;
        using GlissandoSet = std::vector<GlissandoPtr>;
        using GlissandoSetIter = GlissandoSet::iterator;
        using GlissandoSetIterConst = GlissandoSet::const_iterator;
        inline GlissandoPtr makeGlissando() { return std::make_shared<Glissando>(); }
		inline GlissandoPtr makeGlissando( const XsString& value ) { return std::make_shared<Glissando>( value ); }
		inline GlissandoPtr makeGlissando( XsString&& value ) { return std::make_shared<Glissando>( std::move( value ) ); }

        class Glissando : public ElementInterface
        {
        public:
            Glissando();
            Glissando( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            GlissandoAttributesPtr getAttributes() const;
            void setAttributes( const GlissandoAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            GlissandoAttributesPtr myAttributes;
        };
        
        
        struct SlideAttributes;
        using SlideAttributesPtr = std::shared_ptr<SlideAttributes>;
        
        struct SlideAttributes : public AttributesInterface
        {
        public:
            SlideAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            NumberLevel number;
            TenthsValue dashLength;
            TenthsValue spaceLength;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            YesNo accelerate;
            TrillBeats beats;
            Percent firstBeat;
            Percent lastBeat;
            const bool hasType;
            bool hasNumber;
            bool hasDashLength;
            bool hasSpaceLength;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasAccelerate;
            bool hasBeats;
            bool hasFirstBeat;
            bool hasLastBeat;
        };
        
        class Slide;
        using SlidePtr = std::shared_ptr<Slide>;
        using SlideUPtr = std::unique_ptr<Slide>;
        using SlideSet = std::vector<SlidePtr>;
        using SlideSetIter = SlideSet::iterator;
        using SlideSetIterConst = SlideSet::const_iterator;
        inline SlidePtr makeSlide() { return std::make_shared<Slide>(); }
		inline SlidePtr makeSlide( const XsString& value ) { return std::make_shared<Slide>( value ); }
		inline SlidePtr makeSlide( XsString&& value ) { return std::make_shared<Slide>( std::move( value ) ); }

        class Slide : public ElementInterface
        {
        public:
            Slide();
            Slide( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            SlideAttributesPtr getAttributes() const;
            void setAttributes( const SlideAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            SlideAttributesPtr myAttributes;
        };
        
        
        struct AccidentalMarkAttributes;
        using AccidentalMarkAttributesPtr = std::shared_ptr<AccidentalMarkAttributes>;
        
        struct AccidentalMarkAttributes : public AttributesInterface
        {
        public:
            AccidentalMarkAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class AccidentalMark;
        using AccidentalMarkPtr = std::shared_ptr<AccidentalMark>;
        using AccidentalMarkUPtr = std::unique_ptr<AccidentalMark>;
        using AccidentalMarkSet = std::vector<AccidentalMarkPtr>;
        using AccidentalMarkSetIter = AccidentalMarkSet::iterator;
        using AccidentalMarkSetIterConst = AccidentalMarkSet::const_iterator;
        inline AccidentalMarkPtr makeAccidentalMark() { return std::make_shared<AccidentalMark>(); }
		inline AccidentalMarkPtr makeAccidentalMark( const AccidentalValue& value ) { return std::make_shared<AccidentalMark>( value ); }
		inline AccidentalMarkPtr makeAccidentalMark( AccidentalValue&& value ) { return std::make_shared<AccidentalMark>( std::move( value ) ); }

        class AccidentalMark : public ElementInterface
        {
        public:
            AccidentalMark();
            AccidentalMark( const AccidentalValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            AccidentalMarkAttributesPtr getAttributes() const;
            void setAttributes( const AccidentalMarkAttributesPtr& attributes );
            AccidentalValue getValue() const;
            void setValue( const AccidentalValue& value );

        private:
            AccidentalValue myValue;
            AccidentalMarkAttributesPtr myAttributes;
        };
        
        
        struct OtherNotationAttributes;
        using OtherNotationAttributesPtr = std::shared_ptr<OtherNotationAttributes>;
        
        struct OtherNotationAttributes : public AttributesInterface
        {
        public:
            OtherNotationAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStopSingle type;
            NumberLevel number;
            YesNo printObject;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            const bool hasType;
            bool hasNumber;
            bool hasPrintObject;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class OtherNotation;
        using OtherNotationPtr = std::shared_ptr<OtherNotation>;
        using OtherNotationUPtr = std::unique_ptr<OtherNotation>;
        using OtherNotationSet = std::vector<OtherNotationPtr>;
        using OtherNotationSetIter = OtherNotationSet::iterator;
        using OtherNotationSetIterConst = OtherNotationSet::const_iterator;
        inline OtherNotationPtr makeOtherNotation() { return std::make_shared<OtherNotation>(); }
		inline OtherNotationPtr makeOtherNotation( const XsString& value ) { return std::make_shared<OtherNotation>( value ); }
		inline OtherNotationPtr makeOtherNotation( XsString&& value ) { return std::make_shared<OtherNotation>( std::move( value ) ); }

        class OtherNotation : public ElementInterface
        {
        public:
            OtherNotation();
            OtherNotation( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OtherNotationAttributesPtr getAttributes() const;
            void setAttributes( const OtherNotationAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            OtherNotationAttributesPtr myAttributes;
        };
        
        
        struct TypeAttributes;
        using TypeAttributesPtr = std::shared_ptr<TypeAttributes>;
        
        struct TypeAttributes : public AttributesInterface
        {
        public:
            TypeAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            SymbolSize size;
            bool hasSize;
        };
        
        class Type;
        using TypePtr = std::shared_ptr<Type>;
        using TypeUPtr = std::unique_ptr<Type>;
        using TypeSet = std::vector<TypePtr>;
        using TypeSetIter = TypeSet::iterator;
        using TypeSetIterConst = TypeSet::const_iterator;
        inline TypePtr makeType() { return std::make_shared<Type>(); }
		inline TypePtr makeType( const NoteTypeValue& value ) { return std::make_shared<Type>( value ); }
		inline TypePtr makeType( NoteTypeValue&& value ) { return std::make_shared<Type>( std::move( value ) ); }

        class Type : public ElementInterface
        {
        public:
            Type();
            Type( const NoteTypeValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TypeAttributesPtr getAttributes() const;
            void setAttributes( const TypeAttributesPtr& attributes );
            NoteTypeValue getValue() const;
            void setValue( const NoteTypeValue& value );

        private:
            NoteTypeValue myValue;
            TypeAttributesPtr myAttributes;
        };
        
        
        struct AccidentalAttributes;
        using AccidentalAttributesPtr = std::shared_ptr<AccidentalAttributes>;
        
        struct AccidentalAttributes : public AttributesInterface
        {
        public:
            AccidentalAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo cautionary;
            YesNo editorial;
            YesNo parentheses;
            YesNo bracket;
            SymbolSize size;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasCautionary;
            bool hasEditorial;
            bool hasParentheses;
            bool hasBracket;
            bool hasSize;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class Accidental;
        using AccidentalPtr = std::shared_ptr<Accidental>;
        using AccidentalUPtr = std::unique_ptr<Accidental>;
        using AccidentalSet = std::vector<AccidentalPtr>;
        using AccidentalSetIter = AccidentalSet::iterator;
        using AccidentalSetIterConst = AccidentalSet::const_iterator;
        inline AccidentalPtr makeAccidental() { return std::make_shared<Accidental>(); }
		inline AccidentalPtr makeAccidental( const AccidentalValue& value ) { return std::make_shared<Accidental>( value ); }
		inline AccidentalPtr makeAccidental( AccidentalValue&& value ) { return std::make_shared<Accidental>( std::move( value ) ); }

        class Accidental : public ElementInterface
        {
        public:
            Accidental();
            Accidental( const AccidentalValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            AccidentalAttributesPtr getAttributes() const;
            void setAttributes( const AccidentalAttributesPtr& attributes );
            AccidentalValue getValue() const;
            void setValue( const AccidentalValue& value );

        private:
            AccidentalValue myValue;
            AccidentalAttributesPtr myAttributes;
        };
        
        
        struct StemAttributes;
        using StemAttributesPtr = std::shared_ptr<StemAttributes>;
        
        struct StemAttributes : public AttributesInterface
        {
        public:
            StemAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
        };
        
        class Stem;
        using StemPtr = std::shared_ptr<Stem>;
        using StemUPtr = std::unique_ptr<Stem>;
        using StemSet = std::vector<StemPtr>;
        using StemSetIter = StemSet::iterator;
        using StemSetIterConst = StemSet::const_iterator;
        inline StemPtr makeStem() { return std::make_shared<Stem>(); }
		inline StemPtr makeStem( const StemValue& value ) { return std::make_shared<Stem>( value ); }
		inline StemPtr makeStem( StemValue&& value ) { return std::make_shared<Stem>( std::move( value ) ); }

        class Stem : public ElementInterface
        {
        public:
            Stem();
            Stem( const StemValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StemAttributesPtr getAttributes() const;
            void setAttributes( const StemAttributesPtr& attributes );
            StemValue getValue() const;
            void setValue( const StemValue& value );

        private:
            StemValue myValue;
            StemAttributesPtr myAttributes;
        };
        
        
        struct NoteheadAttributes;
        using NoteheadAttributesPtr = std::shared_ptr<NoteheadAttributes>;
        
        struct NoteheadAttributes : public AttributesInterface
        {
        public:
            NoteheadAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo filled;
            YesNo parentheses;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasFilled;
            bool hasParentheses;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class Notehead;
        using NoteheadPtr = std::shared_ptr<Notehead>;
        using NoteheadUPtr = std::unique_ptr<Notehead>;
        using NoteheadSet = std::vector<NoteheadPtr>;
        using NoteheadSetIter = NoteheadSet::iterator;
        using NoteheadSetIterConst = NoteheadSet::const_iterator;
        inline NoteheadPtr makeNotehead() { return std::make_shared<Notehead>(); }
		inline NoteheadPtr makeNotehead( const NoteheadValue& value ) { return std::make_shared<Notehead>( value ); }
		inline NoteheadPtr makeNotehead( NoteheadValue&& value ) { return std::make_shared<Notehead>( std::move( value ) ); }

        class Notehead : public ElementInterface
        {
        public:
            Notehead();
            Notehead( const NoteheadValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NoteheadAttributesPtr getAttributes() const;
            void setAttributes( const NoteheadAttributesPtr& attributes );
            NoteheadValue getValue() const;
            void setValue( const NoteheadValue& value );

        private:
            NoteheadValue myValue;
            NoteheadAttributesPtr myAttributes;
        };
        
        
        struct BeamAttributes;
        using BeamAttributesPtr = std::shared_ptr<BeamAttributes>;
        
        struct BeamAttributes : public AttributesInterface
        {
        public:
            BeamAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            BeamLevel number;
            YesNo repeater;
            Fan fan;
            bool hasNumber;
            bool hasRepeater;
            bool hasFan;
        };
        
        class Beam;
        using BeamPtr = std::shared_ptr<Beam>;
        using BeamUPtr = std::unique_ptr<Beam>;
        using BeamSet = std::vector<BeamPtr>;
        using BeamSetIter = BeamSet::iterator;
        using BeamSetIterConst = BeamSet::const_iterator;
        inline BeamPtr makeBeam() { return std::make_shared<Beam>(); }
		inline BeamPtr makeBeam( const BeamValue& value ) { return std::make_shared<Beam>( value ); }
		inline BeamPtr makeBeam( BeamValue&& value ) { return std::make_shared<Beam>( std::move( value ) ); }

        class Beam : public ElementInterface
        {
        public:
            Beam();
            Beam( const BeamValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BeamAttributesPtr getAttributes() const;
            void setAttributes( const BeamAttributesPtr& attributes );
            BeamValue getValue() const;
            void setValue( const BeamValue& value );

        private:
            BeamValue myValue;
            BeamAttributesPtr myAttributes;
        };
        
        
        struct TremoloAttributes;
        using TremoloAttributesPtr = std::shared_ptr<TremoloAttributes>;
        
        struct TremoloAttributes : public AttributesInterface
        {
        public:
            TremoloAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStopSingle type;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            bool hasType;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class Tremolo;
        using TremoloPtr = std::shared_ptr<Tremolo>;
        using TremoloUPtr = std::unique_ptr<Tremolo>;
        using TremoloSet = std::vector<TremoloPtr>;
        using TremoloSetIter = TremoloSet::iterator;
        using TremoloSetIterConst = TremoloSet::const_iterator;
        inline TremoloPtr makeTremolo() { return std::make_shared<Tremolo>(); }
		inline TremoloPtr makeTremolo( const TremoloMarks& value ) { return std::make_shared<Tremolo>( value ); }
		inline TremoloPtr makeTremolo( TremoloMarks&& value ) { return std::make_shared<Tremolo>( std::move( value ) ); }

        class Tremolo : public ElementInterface
        {
        public:
            Tremolo();
            Tremolo( const TremoloMarks& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TremoloAttributesPtr getAttributes() const;
            void setAttributes( const TremoloAttributesPtr& attributes );
            TremoloMarks getValue() const;
            void setValue( const TremoloMarks& value );

        private:
            TremoloMarks myValue;
            TremoloAttributesPtr myAttributes;
        };
        
        
        struct OtherOrnamentAttributes;
        using OtherOrnamentAttributesPtr = std::shared_ptr<OtherOrnamentAttributes>;
        
        struct OtherOrnamentAttributes : public AttributesInterface
        {
        public:
            OtherOrnamentAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class OtherOrnament;
        using OtherOrnamentPtr = std::shared_ptr<OtherOrnament>;
        using OtherOrnamentUPtr = std::unique_ptr<OtherOrnament>;
        using OtherOrnamentSet = std::vector<OtherOrnamentPtr>;
        using OtherOrnamentSetIter = OtherOrnamentSet::iterator;
        using OtherOrnamentSetIterConst = OtherOrnamentSet::const_iterator;
        inline OtherOrnamentPtr makeOtherOrnament() { return std::make_shared<OtherOrnament>(); }
		inline OtherOrnamentPtr makeOtherOrnament( const XsString& value ) { return std::make_shared<OtherOrnament>( value ); }
		inline OtherOrnamentPtr makeOtherOrnament( XsString&& value ) { return std::make_shared<OtherOrnament>( std::move( value ) ); }

        class OtherOrnament : public ElementInterface
        {
        public:
            OtherOrnament();
            OtherOrnament( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OtherOrnamentAttributesPtr getAttributes() const;
            void setAttributes( const OtherOrnamentAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            OtherOrnamentAttributesPtr myAttributes;
        };
        
        
        struct PluckAttributes;
        using PluckAttributesPtr = std::shared_ptr<PluckAttributes>;
        
        struct PluckAttributes : public AttributesInterface
        {
        public:
            PluckAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class Pluck;
        using PluckPtr = std::shared_ptr<Pluck>;
        using PluckUPtr = std::unique_ptr<Pluck>;
        using PluckSet = std::vector<PluckPtr>;
        using PluckSetIter = PluckSet::iterator;
        using PluckSetIterConst = PluckSet::const_iterator;
        inline PluckPtr makePluck() { return std::make_shared<Pluck>(); }
		inline PluckPtr makePluck( const XsString& value ) { return std::make_shared<Pluck>( value ); }
		inline PluckPtr makePluck( XsString&& value ) { return std::make_shared<Pluck>( std::move( value ) ); }

        class Pluck : public ElementInterface
        {
        public:
            Pluck();
            Pluck( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PluckAttributesPtr getAttributes() const;
            void setAttributes( const PluckAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            PluckAttributesPtr myAttributes;
        };
        
        
        struct HammerOnAttributes;
        using HammerOnAttributesPtr = std::shared_ptr<HammerOnAttributes>;
        
        struct HammerOnAttributes : public AttributesInterface
        {
        public:
            HammerOnAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            NumberLevel number;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            const bool hasType;
            bool hasNumber;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class HammerOn;
        using HammerOnPtr = std::shared_ptr<HammerOn>;
        using HammerOnUPtr = std::unique_ptr<HammerOn>;
        using HammerOnSet = std::vector<HammerOnPtr>;
        using HammerOnSetIter = HammerOnSet::iterator;
        using HammerOnSetIterConst = HammerOnSet::const_iterator;
        inline HammerOnPtr makeHammerOn() { return std::make_shared<HammerOn>(); }
		inline HammerOnPtr makeHammerOn( const XsString& value ) { return std::make_shared<HammerOn>( value ); }
		inline HammerOnPtr makeHammerOn( XsString&& value ) { return std::make_shared<HammerOn>( std::move( value ) ); }

        class HammerOn : public ElementInterface
        {
        public:
            HammerOn();
            HammerOn( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HammerOnAttributesPtr getAttributes() const;
            void setAttributes( const HammerOnAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            HammerOnAttributesPtr myAttributes;
        };
        
        
        struct PullOffAttributes;
        using PullOffAttributesPtr = std::shared_ptr<PullOffAttributes>;
        
        struct PullOffAttributes : public AttributesInterface
        {
        public:
            PullOffAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            NumberLevel number;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            const bool hasType;
            bool hasNumber;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class PullOff;
        using PullOffPtr = std::shared_ptr<PullOff>;
        using PullOffUPtr = std::unique_ptr<PullOff>;
        using PullOffSet = std::vector<PullOffPtr>;
        using PullOffSetIter = PullOffSet::iterator;
        using PullOffSetIterConst = PullOffSet::const_iterator;
        inline PullOffPtr makePullOff() { return std::make_shared<PullOff>(); }
		inline PullOffPtr makePullOff( const XsString& value ) { return std::make_shared<PullOff>( value ); }
		inline PullOffPtr makePullOff( XsString&& value ) { return std::make_shared<PullOff>( std::move( value ) ); }

        class PullOff : public ElementInterface
        {
        public:
            PullOff();
            PullOff( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PullOffAttributesPtr getAttributes() const;
            void setAttributes( const PullOffAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            PullOffAttributesPtr myAttributes;
        };
        
        
        struct TapAttributes;
        using TapAttributesPtr = std::shared_ptr<TapAttributes>;
        
        struct TapAttributes : public AttributesInterface
        {
        public:
            TapAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class Tap;
        using TapPtr = std::shared_ptr<Tap>;
        using TapUPtr = std::unique_ptr<Tap>;
        using TapSet = std::vector<TapPtr>;
        using TapSetIter = TapSet::iterator;
        using TapSetIterConst = TapSet::const_iterator;
        inline TapPtr makeTap() { return std::make_shared<Tap>(); }
		inline TapPtr makeTap( const XsString& value ) { return std::make_shared<Tap>( value ); }
		inline TapPtr makeTap( XsString&& value ) { return std::make_shared<Tap>( std::move( value ) ); }

        class Tap : public ElementInterface
        {
        public:
            Tap();
            Tap( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TapAttributesPtr getAttributes() const;
            void setAttributes( const TapAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            TapAttributesPtr myAttributes;
        };
        
        
        struct HandbellAttributes;
        using HandbellAttributesPtr = std::shared_ptr<HandbellAttributes>;
        
        struct HandbellAttributes : public AttributesInterface
        {
        public:
            HandbellAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class Handbell;
        using HandbellPtr = std::shared_ptr<Handbell>;
        using HandbellUPtr = std::unique_ptr<Handbell>;
        using HandbellSet = std::vector<HandbellPtr>;
        using HandbellSetIter = HandbellSet::iterator;
        using HandbellSetIterConst = HandbellSet::const_iterator;
        inline HandbellPtr makeHandbell() { return std::make_shared<Handbell>(); }
		inline HandbellPtr makeHandbell( const HandbellValue& value ) { return std::make_shared<Handbell>( value ); }
		inline HandbellPtr makeHandbell( HandbellValue&& value ) { return std::make_shared<Handbell>( std::move( value ) ); }

        class Handbell : public ElementInterface
        {
        public:
            Handbell();
            Handbell( const HandbellValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HandbellAttributesPtr getAttributes() const;
            void setAttributes( const HandbellAttributesPtr& attributes );
            HandbellValue getValue() const;
            void setValue( const HandbellValue& value );

        private:
            HandbellValue myValue;
            HandbellAttributesPtr myAttributes;
        };
        
        
        struct OtherTechnicalAttributes;
        using OtherTechnicalAttributesPtr = std::shared_ptr<OtherTechnicalAttributes>;
        
        struct OtherTechnicalAttributes : public AttributesInterface
        {
        public:
            OtherTechnicalAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class OtherTechnical;
        using OtherTechnicalPtr = std::shared_ptr<OtherTechnical>;
        using OtherTechnicalUPtr = std::unique_ptr<OtherTechnical>;
        using OtherTechnicalSet = std::vector<OtherTechnicalPtr>;
        using OtherTechnicalSetIter = OtherTechnicalSet::iterator;
        using OtherTechnicalSetIterConst = OtherTechnicalSet::const_iterator;
        inline OtherTechnicalPtr makeOtherTechnical() { return std::make_shared<OtherTechnical>(); }
		inline OtherTechnicalPtr makeOtherTechnical( const XsString& value ) { return std::make_shared<OtherTechnical>( value ); }
		inline OtherTechnicalPtr makeOtherTechnical( XsString&& value ) { return std::make_shared<OtherTechnical>( std::move( value ) ); }

        class OtherTechnical : public ElementInterface
        {
        public:
            OtherTechnical();
            OtherTechnical( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OtherTechnicalAttributesPtr getAttributes() const;
            void setAttributes( const OtherTechnicalAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            OtherTechnicalAttributesPtr myAttributes;
        };
        
        
        struct TupletNumberAttributes;
        using TupletNumberAttributesPtr = std::shared_ptr<TupletNumberAttributes>;
        
        struct TupletNumberAttributes : public AttributesInterface
        {
        public:
            TupletNumberAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class TupletNumber;
        using TupletNumberPtr = std::shared_ptr<TupletNumber>;
        using TupletNumberUPtr = std::unique_ptr<TupletNumber>;
        using TupletNumberSet = std::vector<TupletNumberPtr>;
        using TupletNumberSetIter = TupletNumberSet::iterator;
        using TupletNumberSetIterConst = TupletNumberSet::const_iterator;
        inline TupletNumberPtr makeTupletNumber() { return std::make_shared<TupletNumber>(); }
		inline TupletNumberPtr makeTupletNumber( const NonNegativeInteger& value ) { return std::make_shared<TupletNumber>( value ); }
		inline TupletNumberPtr makeTupletNumber( NonNegativeInteger&& value ) { return std::make_shared<TupletNumber>( std::move( value ) ); }

        class TupletNumber : public ElementInterface
        {
        public:
            TupletNumber();
            TupletNumber( const NonNegativeInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TupletNumberAttributesPtr getAttributes() const;
            void setAttributes( const TupletNumberAttributesPtr& attributes );
            NonNegativeInteger getValue() const;
            void setValue( const NonNegativeInteger& value );

        private:
            NonNegativeInteger myValue;
            TupletNumberAttributesPtr myAttributes;
        };
        
        
        struct TupletTypeAttributes;
        using TupletTypeAttributesPtr = std::shared_ptr<TupletTypeAttributes>;
        
        struct TupletTypeAttributes : public AttributesInterface
        {
        public:
            TupletTypeAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class TupletType;
        using TupletTypePtr = std::shared_ptr<TupletType>;
        using TupletTypeUPtr = std::unique_ptr<TupletType>;
        using TupletTypeSet = std::vector<TupletTypePtr>;
        using TupletTypeSetIter = TupletTypeSet::iterator;
        using TupletTypeSetIterConst = TupletTypeSet::const_iterator;
        inline TupletTypePtr makeTupletType() { return std::make_shared<TupletType>(); }
		inline TupletTypePtr makeTupletType( const NoteTypeValue& value ) { return std::make_shared<TupletType>( value ); }
		inline TupletTypePtr makeTupletType( NoteTypeValue&& value ) { return std::make_shared<TupletType>( std::move( value ) ); }

        class TupletType : public ElementInterface
        {
        public:
            TupletType();
            TupletType( const NoteTypeValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TupletTypeAttributesPtr getAttributes() const;
            void setAttributes( const TupletTypeAttributesPtr& attributes );
            NoteTypeValue getValue() const;
            void setValue( const NoteTypeValue& value );

        private:
            NoteTypeValue myValue;
            TupletTypeAttributesPtr myAttributes;
        };
        
        
        struct CreditWordsAttributes;
        using CreditWordsAttributesPtr = std::shared_ptr<CreditWordsAttributes>;
        
        struct CreditWordsAttributes : public AttributesInterface
        {
        public:
            CreditWordsAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            LeftCenterRight justify;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight halign;
            NumberOfLines underline;
            NumberOfLines overline;
            NumberOfLines lineThrough;
            RotationDegrees rotation;
            NumberOrNormal letterSpacing;
            NumberOrNormal lineHeight;
            XmlLang lang;
            XmlSpace space;
            EnclosureShape enclosure;
            bool hasJustify;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
            bool hasUnderline;
            bool hasOverline;
            bool hasLineThrough;
            bool hasRotation;
            bool hasLetterSpacing;
            bool hasLineHeight;
            bool hasLang;
            bool hasSpace;
            bool hasEnclosure;
        };
        
        class CreditWords;
        using CreditWordsPtr = std::shared_ptr<CreditWords>;
        using CreditWordsUPtr = std::unique_ptr<CreditWords>;
        using CreditWordsSet = std::vector<CreditWordsPtr>;
        using CreditWordsSetIter = CreditWordsSet::iterator;
        using CreditWordsSetIterConst = CreditWordsSet::const_iterator;
        inline CreditWordsPtr makeCreditWords() { return std::make_shared<CreditWords>(); }
		inline CreditWordsPtr makeCreditWords( const XsString& value ) { return std::make_shared<CreditWords>( value ); }
		inline CreditWordsPtr makeCreditWords( XsString&& value ) { return std::make_shared<CreditWords>( std::move( value ) ); }

        class CreditWords : public ElementInterface
        {
        public:
            CreditWords();
            CreditWords( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            CreditWordsAttributesPtr getAttributes() const;
            void setAttributes( const CreditWordsAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            CreditWordsAttributesPtr myAttributes;
        };
        

        struct GroupNameAttributes;
        using GroupNameAttributesPtr = std::shared_ptr<GroupNameAttributes>;
        
        struct GroupNameAttributes : public AttributesInterface
        {
        public:
            GroupNameAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight justify;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasJustify;
        };
        
        class GroupName;
        using GroupNamePtr = std::shared_ptr<GroupName>;
        using GroupNameUPtr = std::unique_ptr<GroupName>;
        using GroupNameSet = std::vector<GroupNamePtr>;
        using GroupNameSetIter = GroupNameSet::iterator;
        using GroupNameSetIterConst = GroupNameSet::const_iterator;
        inline GroupNamePtr makeGroupName() { return std::make_shared<GroupName>(); }
		inline GroupNamePtr makeGroupName( const XsString& value ) { return std::make_shared<GroupName>( value ); }
		inline GroupNamePtr makeGroupName( XsString&& value ) { return std::make_shared<GroupName>( std::move( value ) ); }

        class GroupName : public ElementInterface
        {
        public:
            GroupName();
            GroupName( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            GroupNameAttributesPtr getAttributes() const;
            void setAttributes( const GroupNameAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            GroupNameAttributesPtr myAttributes;
        };
        
        
        struct GroupAbbreviationAttributes;
        using GroupAbbreviationAttributesPtr = std::shared_ptr<GroupAbbreviationAttributes>;
        
        struct GroupAbbreviationAttributes : public AttributesInterface
        {
        public:
            GroupAbbreviationAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight justify;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasJustify;
        };
        
        class GroupAbbreviation;
        using GroupAbbreviationPtr = std::shared_ptr<GroupAbbreviation>;
        using GroupAbbreviationUPtr = std::unique_ptr<GroupAbbreviation>;
        using GroupAbbreviationSet = std::vector<GroupAbbreviationPtr>;
        using GroupAbbreviationSetIter = GroupAbbreviationSet::iterator;
        using GroupAbbreviationSetIterConst = GroupAbbreviationSet::const_iterator;
        inline GroupAbbreviationPtr makeGroupAbbreviation() { return std::make_shared<GroupAbbreviation>(); }
		inline GroupAbbreviationPtr makeGroupAbbreviation( const XsString& value ) { return std::make_shared<GroupAbbreviation>( value ); }
		inline GroupAbbreviationPtr makeGroupAbbreviation( XsString&& value ) { return std::make_shared<GroupAbbreviation>( std::move( value ) ); }

        class GroupAbbreviation : public ElementInterface
        {
        public:
            GroupAbbreviation();
            GroupAbbreviation( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            GroupAbbreviationAttributesPtr getAttributes() const;
            void setAttributes( const GroupAbbreviationAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            GroupAbbreviationAttributesPtr myAttributes;
        };
        
        
        struct GroupSymbolAttributes;
        using GroupSymbolAttributesPtr = std::shared_ptr<GroupSymbolAttributes>;
        
        struct GroupSymbolAttributes : public AttributesInterface
        {
        public:
            GroupSymbolAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
        };
        
        class GroupSymbol;
        using GroupSymbolPtr = std::shared_ptr<GroupSymbol>;
        using GroupSymbolUPtr = std::unique_ptr<GroupSymbol>;
        using GroupSymbolSet = std::vector<GroupSymbolPtr>;
        using GroupSymbolSetIter = GroupSymbolSet::iterator;
        using GroupSymbolSetIterConst = GroupSymbolSet::const_iterator;
        inline GroupSymbolPtr makeGroupSymbol() { return std::make_shared<GroupSymbol>(); }
		inline GroupSymbolPtr makeGroupSymbol( const GroupSymbolValue& value ) { return std::make_shared<GroupSymbol>( value ); }
		inline GroupSymbolPtr makeGroupSymbol( GroupSymbolValue&& value ) { return std::make_shared<GroupSymbol>( std::move( value ) ); }

        class GroupSymbol : public ElementInterface
        {
        public:
            GroupSymbol();
            GroupSymbol( const GroupSymbolValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            GroupSymbolAttributesPtr getAttributes() const;
            void setAttributes( const GroupSymbolAttributesPtr& attributes );
            GroupSymbolValue getValue() const;
            void setValue( const GroupSymbolValue& value );

        private:
            GroupSymbolValue myValue;
            GroupSymbolAttributesPtr myAttributes;
        };
        
        
        struct GroupBarlineAttributes;
        using GroupBarlineAttributesPtr = std::shared_ptr<GroupBarlineAttributes>;
        
        struct GroupBarlineAttributes : public AttributesInterface
        {
        public:
            GroupBarlineAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            Color color;
            bool hasColor;
        };
        
        class GroupBarline;
        using GroupBarlinePtr = std::shared_ptr<GroupBarline>;
        using GroupBarlineUPtr = std::unique_ptr<GroupBarline>;
        using GroupBarlineSet = std::vector<GroupBarlinePtr>;
        using GroupBarlineSetIter = GroupBarlineSet::iterator;
        using GroupBarlineSetIterConst = GroupBarlineSet::const_iterator;
        inline GroupBarlinePtr makeGroupBarline() { return std::make_shared<GroupBarline>(); }
		inline GroupBarlinePtr makeGroupBarline( const GroupBarlineValue& value ) { return std::make_shared<GroupBarline>( value ); }
		inline GroupBarlinePtr makeGroupBarline( GroupBarlineValue&& value ) { return std::make_shared<GroupBarline>( std::move( value ) ); }

        class GroupBarline : public ElementInterface
        {
        public:
            GroupBarline();
            GroupBarline( const GroupBarlineValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            GroupBarlineAttributesPtr getAttributes() const;
            void setAttributes( const GroupBarlineAttributesPtr& attributes );
            GroupBarlineValue getValue() const;
            void setValue( const GroupBarlineValue& value );

        private:
            GroupBarlineValue myValue;
            GroupBarlineAttributesPtr myAttributes;
        };
        
        
        struct PartNameAttributes;
        using PartNameAttributesPtr = std::shared_ptr<PartNameAttributes>;
        
        struct PartNameAttributes : public AttributesInterface
        {
        public:
            PartNameAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            YesNo printObject;
            LeftCenterRight justify;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPrintObject;
            bool hasJustify;
        };
        
        class PartName;
        using PartNamePtr = std::shared_ptr<PartName>;
        using PartNameUPtr = std::unique_ptr<PartName>;
        using PartNameSet = std::vector<PartNamePtr>;
        using PartNameSetIter = PartNameSet::iterator;
        using PartNameSetIterConst = PartNameSet::const_iterator;
        inline PartNamePtr makePartName() { return std::make_shared<PartName>(); }
		inline PartNamePtr makePartName( const XsString& value ) { return std::make_shared<PartName>( value ); }
		inline PartNamePtr makePartName( XsString&& value ) { return std::make_shared<PartName>( std::move( value ) ); }

        class PartName : public ElementInterface
        {
        public:
            PartName();
            PartName( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PartNameAttributesPtr getAttributes() const;
            void setAttributes( const PartNameAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            PartNameAttributesPtr myAttributes;
        };
        
        
        struct PartAbbreviationAttributes;
        using PartAbbreviationAttributesPtr = std::shared_ptr<PartAbbreviationAttributes>;
        
        struct PartAbbreviationAttributes : public AttributesInterface
        {
        public:
            PartAbbreviationAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            YesNo printObject;
            LeftCenterRight justify;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPrintObject;
            bool hasJustify;
        };
        
        class PartAbbreviation;
        using PartAbbreviationPtr = std::shared_ptr<PartAbbreviation>;
        using PartAbbreviationUPtr = std::unique_ptr<PartAbbreviation>;
        using PartAbbreviationSet = std::vector<PartAbbreviationPtr>;
        using PartAbbreviationSetIter = PartAbbreviationSet::iterator;
        using PartAbbreviationSetIterConst = PartAbbreviationSet::const_iterator;
        inline PartAbbreviationPtr makePartAbbreviation() { return std::make_shared<PartAbbreviation>(); }
		inline PartAbbreviationPtr makePartAbbreviation( const XsString& value ) { return std::make_shared<PartAbbreviation>( value ); }
		inline PartAbbreviationPtr makePartAbbreviation( XsString&& value ) { return std::make_shared<PartAbbreviation>( std::move( value ) ); }

        class PartAbbreviation : public ElementInterface
        {
        public:
            PartAbbreviation();
            PartAbbreviation( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PartAbbreviationAttributesPtr getAttributes() const;
            void setAttributes( const PartAbbreviationAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            PartAbbreviationAttributesPtr myAttributes;
        };

        
        struct FootnoteAttributes;
        using FootnoteAttributesPtr = std::shared_ptr<FootnoteAttributes>;
        
        struct FootnoteAttributes : public AttributesInterface
        {
        public:
            FootnoteAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            LeftCenterRight justify;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight halign;
            NumberOfLines underline;
            NumberOfLines overline;
            NumberOfLines lineThrough;
            RotationDegrees rotation;
            NumberOrNormal letterSpacing;
            NumberOrNormal lineHeight;
            XmlLang lang;
            XmlSpace space;
            EnclosureShape enclosure;
            bool hasJustify;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
            bool hasUnderline;
            bool hasOverline;
            bool hasLineThrough;
            bool hasRotation;
            bool hasLetterSpacing;
            bool hasLineHeight;
            bool hasLang;
            bool hasSpace;
            bool hasEnclosure;
        };
        
        class Footnote;
        using FootnotePtr = std::shared_ptr<Footnote>;
        using FootnoteUPtr = std::unique_ptr<Footnote>;
        using FootnoteSet = std::vector<FootnotePtr>;
        using FootnoteSetIter = FootnoteSet::iterator;
        using FootnoteSetIterConst = FootnoteSet::const_iterator;
        inline FootnotePtr makeFootnote() { return std::make_shared<Footnote>(); }
		inline FootnotePtr makeFootnote( const XsString& value ) { return std::make_shared<Footnote>( value ); }
		inline FootnotePtr makeFootnote( XsString&& value ) { return std::make_shared<Footnote>( std::move( value ) ); }

        class Footnote : public ElementInterface
        {
        public:
            Footnote();
            Footnote( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FootnoteAttributesPtr getAttributes() const;
            void setAttributes( const FootnoteAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            FootnoteAttributesPtr myAttributes;
        };
        
        
        struct LevelAttributes;
        using LevelAttributesPtr = std::shared_ptr<LevelAttributes>;
        
        struct LevelAttributes : public AttributesInterface
        {
        public:
            LevelAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo reference;
            YesNo parentheses;
            YesNo bracket;
            SymbolSize size;
            bool hasReference;
            bool hasParentheses;
            bool hasBracket;
            bool hasSize;
        };
        
        class Level;
        using LevelPtr = std::shared_ptr<Level>;
        using LevelUPtr = std::unique_ptr<Level>;
        using LevelSet = std::vector<LevelPtr>;
        using LevelSetIter = LevelSet::iterator;
        using LevelSetIterConst = LevelSet::const_iterator;
        inline LevelPtr makeLevel() { return std::make_shared<Level>(); }
		inline LevelPtr makeLevel( const XsString& value ) { return std::make_shared<Level>( value ); }
		inline LevelPtr makeLevel( XsString&& value ) { return std::make_shared<Level>( std::move( value ) ); }

        class Level : public ElementInterface
        {
        public:
            Level();
            Level( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            LevelAttributesPtr getAttributes() const;
            void setAttributes( const LevelAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            LevelAttributesPtr myAttributes;
        };
        
        
        struct CancelAttributes;
        using CancelAttributesPtr = std::shared_ptr<CancelAttributes>;
        
        struct CancelAttributes : public AttributesInterface
        {
        public:
            CancelAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            CancelLocation location;
            bool hasLocation;
        };
        
        class Cancel;
        using CancelPtr = std::shared_ptr<Cancel>;
        using CancelUPtr = std::unique_ptr<Cancel>;
        using CancelSet = std::vector<CancelPtr>;
        using CancelSetIter = CancelSet::iterator;
        using CancelSetIterConst = CancelSet::const_iterator;
        inline CancelPtr makeCancel() { return std::make_shared<Cancel>(); }
		inline CancelPtr makeCancel( const FifthsValue& value ) { return std::make_shared<Cancel>( value ); }
		inline CancelPtr makeCancel( FifthsValue&& value ) { return std::make_shared<Cancel>( std::move( value ) ); }

        class Cancel : public ElementInterface
        {
        public:
            Cancel();
            Cancel( const FifthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            CancelAttributesPtr getAttributes() const;
            void setAttributes( const CancelAttributesPtr& attributes );
            FifthsValue getValue() const;
            void setValue( const FifthsValue& value );

        private:
            FifthsValue myValue;
            CancelAttributesPtr myAttributes;
        };
        
        
        struct FunctionAttributes;
        using FunctionAttributesPtr = std::shared_ptr<FunctionAttributes>;
        
        struct FunctionAttributes : public AttributesInterface
        {
        public:
            FunctionAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class Function;
        using FunctionPtr = std::shared_ptr<Function>;
        using FunctionUPtr = std::unique_ptr<Function>;
        using FunctionSet = std::vector<FunctionPtr>;
        using FunctionSetIter = FunctionSet::iterator;
        using FunctionSetIterConst = FunctionSet::const_iterator;
        inline FunctionPtr makeFunction() { return std::make_shared<Function>(); }
		inline FunctionPtr makeFunction( const XsString& value ) { return std::make_shared<Function>( value ); }
		inline FunctionPtr makeFunction( XsString&& value ) { return std::make_shared<Function>( std::move( value ) ); }

        class Function : public ElementInterface
        {
        public:
            Function();
            Function( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FunctionAttributesPtr getAttributes() const;
            void setAttributes( const FunctionAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
            FunctionAttributesPtr myAttributes;
        };
        
        
        struct KindAttributes;
        using KindAttributesPtr = std::shared_ptr<KindAttributes>;
        
        struct KindAttributes : public AttributesInterface
        {
        public:
            KindAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo useSymbols;
            XsToken text;
            YesNo stackDegrees;
            YesNo parenthesesDegrees;
            YesNo bracketDegrees;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight halign;
            bool hasUseSymbols;
            bool hasText;
            bool hasStackDegrees;
            bool hasParenthesesDegrees;
            bool hasBracketDegrees;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
        };
        
        class Kind;
        using KindPtr = std::shared_ptr<Kind>;
        using KindUPtr = std::unique_ptr<Kind>;
        using KindSet = std::vector<KindPtr>;
        using KindSetIter = KindSet::iterator;
        using KindSetIterConst = KindSet::const_iterator;
        inline KindPtr makeKind() { return std::make_shared<Kind>(); }
		inline KindPtr makeKind( const KindValue& value ) { return std::make_shared<Kind>( value ); }
		inline KindPtr makeKind( KindValue&& value ) { return std::make_shared<Kind>( std::move( value ) ); }

        class Kind : public ElementInterface
        {
        public:
            Kind();
            Kind( const KindValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            KindAttributesPtr getAttributes() const;
            void setAttributes( const KindAttributesPtr& attributes );
            KindValue getValue() const;
            void setValue( const KindValue& value );

        private:
            KindValue myValue;
            KindAttributesPtr myAttributes;
        };
        
        
        struct InversionAttributes;
        using InversionAttributesPtr = std::shared_ptr<InversionAttributes>;
        
        struct InversionAttributes : public AttributesInterface
        {
        public:
            InversionAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class Inversion;
        using InversionPtr = std::shared_ptr<Inversion>;
        using InversionUPtr = std::unique_ptr<Inversion>;
        using InversionSet = std::vector<InversionPtr>;
        using InversionSetIter = InversionSet::iterator;
        using InversionSetIterConst = InversionSet::const_iterator;
        inline InversionPtr makeInversion() { return std::make_shared<Inversion>(); }
		inline InversionPtr makeInversion( const NonNegativeInteger& value ) { return std::make_shared<Inversion>( value ); }
		inline InversionPtr makeInversion( NonNegativeInteger&& value ) { return std::make_shared<Inversion>( std::move( value ) ); }

        class Inversion : public ElementInterface
        {
        public:
            Inversion();
            Inversion( const NonNegativeInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            InversionAttributesPtr getAttributes() const;
            void setAttributes( const InversionAttributesPtr& attributes );
            NonNegativeInteger getValue() const;
            void setValue( const NonNegativeInteger& value );

        private:
            NonNegativeInteger myValue;
            InversionAttributesPtr myAttributes;
        };
        
        
        class MidiChannel;
        using MidiChannelPtr = std::shared_ptr<MidiChannel>;
        using MidiChannelUPtr = std::unique_ptr<MidiChannel>;
        using MidiChannelSet = std::vector<MidiChannelPtr>;
        using MidiChannelSetIter = MidiChannelSet::iterator;
        using MidiChannelSetIterConst = MidiChannelSet::const_iterator;
        inline MidiChannelPtr makeMidiChannel() { return std::make_shared<MidiChannel>(); }
		inline MidiChannelPtr makeMidiChannel( const Midi16& value ) { return std::make_shared<MidiChannel>( value ); }
		inline MidiChannelPtr makeMidiChannel( Midi16&& value ) { return std::make_shared<MidiChannel>( std::move( value ) ); }

        class MidiChannel : public ElementInterface
        {
        public:
            MidiChannel();
            MidiChannel( const Midi16& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Midi16 getValue() const;
            void setValue( const Midi16& value );

        private:
            Midi16 myValue;
        };
        
        
        class MidiName;
        using MidiNamePtr = std::shared_ptr<MidiName>;
        using MidiNameUPtr = std::unique_ptr<MidiName>;
        using MidiNameSet = std::vector<MidiNamePtr>;
        using MidiNameSetIter = MidiNameSet::iterator;
        using MidiNameSetIterConst = MidiNameSet::const_iterator;
        inline MidiNamePtr makeMidiName() { return std::make_shared<MidiName>(); }
		inline MidiNamePtr makeMidiName( const XsString& value ) { return std::make_shared<MidiName>( value ); }
		inline MidiNamePtr makeMidiName( XsString&& value ) { return std::make_shared<MidiName>( std::move( value ) ); }

        class MidiName : public ElementInterface
        {
        public:
            MidiName();
            MidiName( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class MidiBank;
        using MidiBankPtr = std::shared_ptr<MidiBank>;
        using MidiBankUPtr = std::unique_ptr<MidiBank>;
        using MidiBankSet = std::vector<MidiBankPtr>;
        using MidiBankSetIter = MidiBankSet::iterator;
        using MidiBankSetIterConst = MidiBankSet::const_iterator;
        inline MidiBankPtr makeMidiBank() { return std::make_shared<MidiBank>(); }
		inline MidiBankPtr makeMidiBank( const Midi16384& value ) { return std::make_shared<MidiBank>( value ); }
		inline MidiBankPtr makeMidiBank( Midi16384&& value ) { return std::make_shared<MidiBank>( std::move( value ) ); }

        class MidiBank : public ElementInterface
        {
        public:
            MidiBank();
            MidiBank( const Midi16384& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Midi16384 getValue() const;
            void setValue( const Midi16384& value );

        private:
            Midi16384 myValue;
        };
        
        
        class MidiProgram;
        using MidiProgramPtr = std::shared_ptr<MidiProgram>;
        using MidiProgramUPtr = std::unique_ptr<MidiProgram>;
        using MidiProgramSet = std::vector<MidiProgramPtr>;
        using MidiProgramSetIter = MidiProgramSet::iterator;
        using MidiProgramSetIterConst = MidiProgramSet::const_iterator;
        inline MidiProgramPtr makeMidiProgram() { return std::make_shared<MidiProgram>(); }
		inline MidiProgramPtr makeMidiProgram( const Midi128& value ) { return std::make_shared<MidiProgram>( value ); }
		inline MidiProgramPtr makeMidiProgram( Midi128&& value ) { return std::make_shared<MidiProgram>( std::move( value ) ); }

        class MidiProgram : public ElementInterface
        {
        public:
            MidiProgram();
            MidiProgram( const Midi128& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Midi128 getValue() const;
            void setValue( const Midi128& value );

        private:
            Midi128 myValue;
        };
        
        
        class MidiUnpitched;
        using MidiUnpitchedPtr = std::shared_ptr<MidiUnpitched>;
        using MidiUnpitchedUPtr = std::unique_ptr<MidiUnpitched>;
        using MidiUnpitchedSet = std::vector<MidiUnpitchedPtr>;
        using MidiUnpitchedSetIter = MidiUnpitchedSet::iterator;
        using MidiUnpitchedSetIterConst = MidiUnpitchedSet::const_iterator;
        inline MidiUnpitchedPtr makeMidiUnpitched() { return std::make_shared<MidiUnpitched>(); }
		inline MidiUnpitchedPtr makeMidiUnpitched( const Midi128& value ) { return std::make_shared<MidiUnpitched>( value ); }
		inline MidiUnpitchedPtr makeMidiUnpitched( Midi128&& value ) { return std::make_shared<MidiUnpitched>( std::move( value ) ); }

        class MidiUnpitched : public ElementInterface
        {
        public:
            MidiUnpitched();
            MidiUnpitched( const Midi128& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Midi128 getValue() const;
            void setValue( const Midi128& value );

        private:
            Midi128 myValue;
        };
        
        
        class Volume;
        using VolumePtr = std::shared_ptr<Volume>;
        using VolumeUPtr = std::unique_ptr<Volume>;
        using VolumeSet = std::vector<VolumePtr>;
        using VolumeSetIter = VolumeSet::iterator;
        using VolumeSetIterConst = VolumeSet::const_iterator;
        inline VolumePtr makeVolume() { return std::make_shared<Volume>(); }
		inline VolumePtr makeVolume( const Percent& value ) { return std::make_shared<Volume>( value ); }
		inline VolumePtr makeVolume( Percent&& value ) { return std::make_shared<Volume>( std::move( value ) ); }

        class Volume : public ElementInterface
        {
        public:
            Volume();
            Volume( const Percent& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Percent getValue() const;
            void setValue( const Percent& value );

        private:
            Percent myValue;
        };
        
        
        class Pan;
        using PanPtr = std::shared_ptr<Pan>;
        using PanUPtr = std::unique_ptr<Pan>;
        using PanSet = std::vector<PanPtr>;
        using PanSetIter = PanSet::iterator;
        using PanSetIterConst = PanSet::const_iterator;
        inline PanPtr makePan() { return std::make_shared<Pan>(); }
		inline PanPtr makePan( const RotationDegrees& value ) { return std::make_shared<Pan>( value ); }
		inline PanPtr makePan( RotationDegrees&& value ) { return std::make_shared<Pan>( std::move( value ) ); }

        class Pan : public ElementInterface
        {
        public:
            Pan();
            Pan( const RotationDegrees& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RotationDegrees getValue() const;
            void setValue( const RotationDegrees& value );

        private:
            RotationDegrees myValue;
        };
        
        
        class Elevation;
        using ElevationPtr = std::shared_ptr<Elevation>;
        using ElevationUPtr = std::unique_ptr<Elevation>;
        using ElevationSet = std::vector<ElevationPtr>;
        using ElevationSetIter = ElevationSet::iterator;
        using ElevationSetIterConst = ElevationSet::const_iterator;
        inline ElevationPtr makeElevation() { return std::make_shared<Elevation>(); }
		inline ElevationPtr makeElevation( const RotationDegrees& value ) { return std::make_shared<Elevation>( value ); }
		inline ElevationPtr makeElevation( RotationDegrees&& value ) { return std::make_shared<Elevation>( std::move( value ) ); }

        class Elevation : public ElementInterface
        {
        public:
            Elevation();
            Elevation( const RotationDegrees& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RotationDegrees getValue() const;
            void setValue( const RotationDegrees& value );

        private:
            RotationDegrees myValue;
        };
        
        
        class Ipa;
        using IpaPtr = std::shared_ptr<Ipa>;
        using IpaUPtr = std::unique_ptr<Ipa>;
        using IpaSet = std::vector<IpaPtr>;
        using IpaSetIter = IpaSet::iterator;
        using IpaSetIterConst = IpaSet::const_iterator;
        inline IpaPtr makeIpa() { return std::make_shared<Ipa>(); }
		inline IpaPtr makeIpa( const XsString& value ) { return std::make_shared<Ipa>( value ); }
		inline IpaPtr makeIpa( XsString&& value ) { return std::make_shared<Ipa>( std::move( value ) ); }

        class Ipa : public ElementInterface
        {
        public:
            Ipa();
            Ipa( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class Mute;
        using MutePtr = std::shared_ptr<Mute>;
        using MuteUPtr = std::unique_ptr<Mute>;
        using MuteSet = std::vector<MutePtr>;
        using MuteSetIter = MuteSet::iterator;
        using MuteSetIterConst = MuteSet::const_iterator;
        inline MutePtr makeMute() { return std::make_shared<Mute>(); }
		inline MutePtr makeMute( const MuteEnum& value ) { return std::make_shared<Mute>( value ); }
		inline MutePtr makeMute( MuteEnum&& value ) { return std::make_shared<Mute>( std::move( value ) ); }

        class Mute : public ElementInterface
        {
        public:
            Mute();
            Mute( const MuteEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MuteEnum getValue() const;
            void setValue( const MuteEnum& value );

        private:
            MuteEnum myValue;
        };
        
        
        class SemiPitched;
        using SemiPitchedPtr = std::shared_ptr<SemiPitched>;
        using SemiPitchedUPtr = std::unique_ptr<SemiPitched>;
        using SemiPitchedSet = std::vector<SemiPitchedPtr>;
        using SemiPitchedSetIter = SemiPitchedSet::iterator;
        using SemiPitchedSetIterConst = SemiPitchedSet::const_iterator;
        inline SemiPitchedPtr makeSemiPitched() { return std::make_shared<SemiPitched>(); }
		inline SemiPitchedPtr makeSemiPitched( const SemiPitchedEnum& value ) { return std::make_shared<SemiPitched>( value ); }
		inline SemiPitchedPtr makeSemiPitched( SemiPitchedEnum&& value ) { return std::make_shared<SemiPitched>( std::move( value ) ); }

        class SemiPitched : public ElementInterface
        {
        public:
            SemiPitched();
            SemiPitched( const SemiPitchedEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            SemiPitchedEnum getValue() const;
            void setValue( const SemiPitchedEnum& value );

        private:
            SemiPitchedEnum myValue;
        };
        
        
        class Divisions;
        using DivisionsPtr = std::shared_ptr<Divisions>;
        using DivisionsUPtr = std::unique_ptr<Divisions>;
        using DivisionsSet = std::vector<DivisionsPtr>;
        using DivisionsSetIter = DivisionsSet::iterator;
        using DivisionsSetIterConst = DivisionsSet::const_iterator;
        inline DivisionsPtr makeDivisions() { return std::make_shared<Divisions>(); }
		inline DivisionsPtr makeDivisions( const PositiveDivisionsValue& value ) { return std::make_shared<Divisions>( value ); }
		inline DivisionsPtr makeDivisions( PositiveDivisionsValue&& value ) { return std::make_shared<Divisions>( std::move( value ) ); }

        class Divisions : public ElementInterface
        {
        public:
            Divisions();
            Divisions( const PositiveDivisionsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PositiveDivisionsValue getValue() const;
            void setValue( const PositiveDivisionsValue& value );

        private:
            PositiveDivisionsValue myValue;
        };
        
        
        class Staves;
        using StavesPtr = std::shared_ptr<Staves>;
        using StavesUPtr = std::unique_ptr<Staves>;
        using StavesSet = std::vector<StavesPtr>;
        using StavesSetIter = StavesSet::iterator;
        using StavesSetIterConst = StavesSet::const_iterator;
        inline StavesPtr makeStaves() { return std::make_shared<Staves>(); }
		inline StavesPtr makeStaves( const NonNegativeInteger& value ) { return std::make_shared<Staves>( value ); }
		inline StavesPtr makeStaves( NonNegativeInteger&& value ) { return std::make_shared<Staves>( std::move( value ) ); }

        class Staves : public ElementInterface
        {
        public:
            Staves();
            Staves( const NonNegativeInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NonNegativeInteger getValue() const;
            void setValue( const NonNegativeInteger& value );

        private:
            NonNegativeInteger myValue;
        };
        
        
        class Instruments;
        using InstrumentsPtr = std::shared_ptr<Instruments>;
        using InstrumentsUPtr = std::unique_ptr<Instruments>;
        using InstrumentsSet = std::vector<InstrumentsPtr>;
        using InstrumentsSetIter = InstrumentsSet::iterator;
        using InstrumentsSetIterConst = InstrumentsSet::const_iterator;
        inline InstrumentsPtr makeInstruments() { return std::make_shared<Instruments>(); }
		inline InstrumentsPtr makeInstruments( const NonNegativeInteger& value ) { return std::make_shared<Instruments>( value ); }
		inline InstrumentsPtr makeInstruments( NonNegativeInteger&& value ) { return std::make_shared<Instruments>( std::move( value ) ); }

        class Instruments : public ElementInterface
        {
        public:
            Instruments();
            Instruments( const NonNegativeInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NonNegativeInteger getValue() const;
            void setValue( const NonNegativeInteger& value );

        private:
            NonNegativeInteger myValue;
        };
        
        
        class Sign;
        using SignPtr = std::shared_ptr<Sign>;
        using SignUPtr = std::unique_ptr<Sign>;
        using SignSet = std::vector<SignPtr>;
        using SignSetIter = SignSet::iterator;
        using SignSetIterConst = SignSet::const_iterator;
        inline SignPtr makeSign() { return std::make_shared<Sign>(); }
		inline SignPtr makeSign( const ClefSign& value ) { return std::make_shared<Sign>( value ); }
		inline SignPtr makeSign( ClefSign&& value ) { return std::make_shared<Sign>( std::move( value ) ); }

        class Sign : public ElementInterface
        {
        public:
            Sign();
            Sign( const ClefSign& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ClefSign getValue() const;
            void setValue( const ClefSign& value );

        private:
            ClefSign myValue;
        };
        
        
        class Line;
        using LinePtr = std::shared_ptr<Line>;
        using LineUPtr = std::unique_ptr<Line>;
        using LineSet = std::vector<LinePtr>;
        using LineSetIter = LineSet::iterator;
        using LineSetIterConst = LineSet::const_iterator;
        inline LinePtr makeLine() { return std::make_shared<Line>(); }
		inline LinePtr makeLine( const StaffLine& value ) { return std::make_shared<Line>( value ); }
		inline LinePtr makeLine( StaffLine&& value ) { return std::make_shared<Line>( std::move( value ) ); }

        class Line : public ElementInterface
        {
        public:
            Line();
            Line( const StaffLine& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StaffLine getValue() const;
            void setValue( const StaffLine& value );

        private:
            StaffLine myValue;
        };
        
        
        class ClefOctaveChange;
        using ClefOctaveChangePtr = std::shared_ptr<ClefOctaveChange>;
        using ClefOctaveChangeUPtr = std::unique_ptr<ClefOctaveChange>;
        using ClefOctaveChangeSet = std::vector<ClefOctaveChangePtr>;
        using ClefOctaveChangeSetIter = ClefOctaveChangeSet::iterator;
        using ClefOctaveChangeSetIterConst = ClefOctaveChangeSet::const_iterator;
        inline ClefOctaveChangePtr makeClefOctaveChange() { return std::make_shared<ClefOctaveChange>(); }
		inline ClefOctaveChangePtr makeClefOctaveChange( const Integer& value ) { return std::make_shared<ClefOctaveChange>( value ); }
		inline ClefOctaveChangePtr makeClefOctaveChange( Integer&& value ) { return std::make_shared<ClefOctaveChange>( std::move( value ) ); }

        class ClefOctaveChange : public ElementInterface
        {
        public:
            ClefOctaveChange();
            ClefOctaveChange( const Integer& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Integer getValue() const;
            void setValue( const Integer& value );

        private:
            Integer myValue;
        };
        
        
        class TimeRelation;
        using TimeRelationPtr = std::shared_ptr<TimeRelation>;
        using TimeRelationUPtr = std::unique_ptr<TimeRelation>;
        using TimeRelationSet = std::vector<TimeRelationPtr>;
        using TimeRelationSetIter = TimeRelationSet::iterator;
        using TimeRelationSetIterConst = TimeRelationSet::const_iterator;
        inline TimeRelationPtr makeTimeRelation() { return std::make_shared<TimeRelation>(); }
		inline TimeRelationPtr makeTimeRelation( const TimeRelationEnum& value ) { return std::make_shared<TimeRelation>( value ); }
		inline TimeRelationPtr makeTimeRelation( TimeRelationEnum&& value ) { return std::make_shared<TimeRelation>( std::move( value ) ); }

        class TimeRelation : public ElementInterface
        {
        public:
            TimeRelation();
            TimeRelation( const TimeRelationEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TimeRelationEnum getValue() const;
            void setValue( const TimeRelationEnum& value );

        private:
            TimeRelationEnum myValue;
        };
        
        
        class StaffType;
        using StaffTypePtr = std::shared_ptr<StaffType>;
        using StaffTypeUPtr = std::unique_ptr<StaffType>;
        using StaffTypeSet = std::vector<StaffTypePtr>;
        using StaffTypeSetIter = StaffTypeSet::iterator;
        using StaffTypeSetIterConst = StaffTypeSet::const_iterator;
        inline StaffTypePtr makeStaffType() { return std::make_shared<StaffType>(); }
		inline StaffTypePtr makeStaffType( const StaffTypeEnum& value ) { return std::make_shared<StaffType>( value ); }
		inline StaffTypePtr makeStaffType( StaffTypeEnum&& value ) { return std::make_shared<StaffType>( std::move( value ) ); }

        class StaffType : public ElementInterface
        {
        public:
            StaffType();
            StaffType( const StaffTypeEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StaffTypeEnum getValue() const;
            void setValue( const StaffTypeEnum& value );

        private:
            StaffTypeEnum myValue;
        };
        
        
        class StaffLines;
        using StaffLinesPtr = std::shared_ptr<StaffLines>;
        using StaffLinesUPtr = std::unique_ptr<StaffLines>;
        using StaffLinesSet = std::vector<StaffLinesPtr>;
        using StaffLinesSetIter = StaffLinesSet::iterator;
        using StaffLinesSetIterConst = StaffLinesSet::const_iterator;
        inline StaffLinesPtr makeStaffLines() { return std::make_shared<StaffLines>(); }
		inline StaffLinesPtr makeStaffLines( const NonNegativeInteger& value ) { return std::make_shared<StaffLines>( value ); }
		inline StaffLinesPtr makeStaffLines( NonNegativeInteger&& value ) { return std::make_shared<StaffLines>( std::move( value ) ); }

        class StaffLines : public ElementInterface
        {
        public:
            StaffLines();
            StaffLines( const NonNegativeInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NonNegativeInteger getValue() const;
            void setValue( const NonNegativeInteger& value );

        private:
            NonNegativeInteger myValue;
        };
        
        
        class Capo;
        using CapoPtr = std::shared_ptr<Capo>;
        using CapoUPtr = std::unique_ptr<Capo>;
        using CapoSet = std::vector<CapoPtr>;
        using CapoSetIter = CapoSet::iterator;
        using CapoSetIterConst = CapoSet::const_iterator;
        inline CapoPtr makeCapo() { return std::make_shared<Capo>(); }
		inline CapoPtr makeCapo( const NonNegativeInteger& value ) { return std::make_shared<Capo>( value ); }
		inline CapoPtr makeCapo( NonNegativeInteger&& value ) { return std::make_shared<Capo>( std::move( value ) ); }

        class Capo : public ElementInterface
        {
        public:
            Capo();
            Capo( const NonNegativeInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NonNegativeInteger getValue() const;
            void setValue( const NonNegativeInteger& value );

        private:
            NonNegativeInteger myValue;
        };
        
        
        class StaffSize;
        using StaffSizePtr = std::shared_ptr<StaffSize>;
        using StaffSizeUPtr = std::unique_ptr<StaffSize>;
        using StaffSizeSet = std::vector<StaffSizePtr>;
        using StaffSizeSetIter = StaffSizeSet::iterator;
        using StaffSizeSetIterConst = StaffSizeSet::const_iterator;
        inline StaffSizePtr makeStaffSize() { return std::make_shared<StaffSize>(); }
		inline StaffSizePtr makeStaffSize( const NonNegativeDecimal& value ) { return std::make_shared<StaffSize>( value ); }
		inline StaffSizePtr makeStaffSize( NonNegativeDecimal&& value ) { return std::make_shared<StaffSize>( std::move( value ) ); }

        class StaffSize : public ElementInterface
        {
        public:
            StaffSize();
            StaffSize( const NonNegativeDecimal& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NonNegativeDecimal getValue() const;
            void setValue( const NonNegativeDecimal& value );

        private:
            NonNegativeDecimal myValue;
        };
        
        
        class SenzaMisura;
        using SenzaMisuraPtr = std::shared_ptr<SenzaMisura>;
        using SenzaMisuraUPtr = std::unique_ptr<SenzaMisura>;
        using SenzaMisuraSet = std::vector<SenzaMisuraPtr>;
        using SenzaMisuraSetIter = SenzaMisuraSet::iterator;
        using SenzaMisuraSetIterConst = SenzaMisuraSet::const_iterator;
        inline SenzaMisuraPtr makeSenzaMisura() { return std::make_shared<SenzaMisura>(); }
		inline SenzaMisuraPtr makeSenzaMisura( const XsString& value ) { return std::make_shared<SenzaMisura>( value ); }
		inline SenzaMisuraPtr makeSenzaMisura( XsString&& value ) { return std::make_shared<SenzaMisura>( std::move( value ) ); }

        class SenzaMisura : public ElementInterface
        {
        public:
            SenzaMisura();
            SenzaMisura( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class Diatonic;
        using DiatonicPtr = std::shared_ptr<Diatonic>;
        using DiatonicUPtr = std::unique_ptr<Diatonic>;
        using DiatonicSet = std::vector<DiatonicPtr>;
        using DiatonicSetIter = DiatonicSet::iterator;
        using DiatonicSetIterConst = DiatonicSet::const_iterator;
        inline DiatonicPtr makeDiatonic() { return std::make_shared<Diatonic>(); }
		inline DiatonicPtr makeDiatonic( const Integer& value ) { return std::make_shared<Diatonic>( value ); }
		inline DiatonicPtr makeDiatonic( Integer&& value ) { return std::make_shared<Diatonic>( std::move( value ) ); }

        class Diatonic : public ElementInterface
        {
        public:
            Diatonic();
            Diatonic( const Integer& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Integer getValue() const;
            void setValue( const Integer& value );

        private:
            Integer myValue;
        };
        
        
        class Chromatic;
        using ChromaticPtr = std::shared_ptr<Chromatic>;
        using ChromaticUPtr = std::unique_ptr<Chromatic>;
        using ChromaticSet = std::vector<ChromaticPtr>;
        using ChromaticSetIter = ChromaticSet::iterator;
        using ChromaticSetIterConst = ChromaticSet::const_iterator;
        inline ChromaticPtr makeChromatic() { return std::make_shared<Chromatic>(); }
		inline ChromaticPtr makeChromatic( const Semitones& value ) { return std::make_shared<Chromatic>( value ); }
		inline ChromaticPtr makeChromatic( Semitones&& value ) { return std::make_shared<Chromatic>( std::move( value ) ); }

        class Chromatic : public ElementInterface
        {
        public:
            Chromatic();
            Chromatic( const Semitones& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Semitones getValue() const;
            void setValue( const Semitones& value );

        private:
            Semitones myValue;
        };
        
        
        class OctaveChange;
        using OctaveChangePtr = std::shared_ptr<OctaveChange>;
        using OctaveChangeUPtr = std::unique_ptr<OctaveChange>;
        using OctaveChangeSet = std::vector<OctaveChangePtr>;
        using OctaveChangeSetIter = OctaveChangeSet::iterator;
        using OctaveChangeSetIterConst = OctaveChangeSet::const_iterator;
        inline OctaveChangePtr makeOctaveChange() { return std::make_shared<OctaveChange>(); }
		inline OctaveChangePtr makeOctaveChange( const Integer& value ) { return std::make_shared<OctaveChange>( value ); }
		inline OctaveChangePtr makeOctaveChange( Integer&& value ) { return std::make_shared<OctaveChange>( std::move( value ) ); }

        class OctaveChange : public ElementInterface
        {
        public:
            OctaveChange();
            OctaveChange( const Integer& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Integer getValue() const;
            void setValue( const Integer& value );

        private:
            Integer myValue;
        };
        
        
        class AccordionMiddle;
        using AccordionMiddlePtr = std::shared_ptr<AccordionMiddle>;
        using AccordionMiddleUPtr = std::unique_ptr<AccordionMiddle>;
        using AccordionMiddleSet = std::vector<AccordionMiddlePtr>;
        using AccordionMiddleSetIter = AccordionMiddleSet::iterator;
        using AccordionMiddleSetIterConst = AccordionMiddleSet::const_iterator;
        inline AccordionMiddlePtr makeAccordionMiddle() { return std::make_shared<AccordionMiddle>(); }
		inline AccordionMiddlePtr makeAccordionMiddle( const AccordionMiddleValue& value ) { return std::make_shared<AccordionMiddle>( value ); }
		inline AccordionMiddlePtr makeAccordionMiddle( AccordionMiddleValue&& value ) { return std::make_shared<AccordionMiddle>( std::move( value ) ); }

        class AccordionMiddle : public ElementInterface
        {
        public:
            AccordionMiddle();
            AccordionMiddle( const AccordionMiddleValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            AccordionMiddleValue getValue() const;
            void setValue( const AccordionMiddleValue& value );

        private:
            AccordionMiddleValue myValue;
        };
        
        
        class FrameStrings;
        using FrameStringsPtr = std::shared_ptr<FrameStrings>;
        using FrameStringsUPtr = std::unique_ptr<FrameStrings>;
        using FrameStringsSet = std::vector<FrameStringsPtr>;
        using FrameStringsSetIter = FrameStringsSet::iterator;
        using FrameStringsSetIterConst = FrameStringsSet::const_iterator;
        inline FrameStringsPtr makeFrameStrings() { return std::make_shared<FrameStrings>(); }
		inline FrameStringsPtr makeFrameStrings( const PositiveInteger& value ) { return std::make_shared<FrameStrings>( value ); }
		inline FrameStringsPtr makeFrameStrings( PositiveInteger&& value ) { return std::make_shared<FrameStrings>( std::move( value ) ); }

        class FrameStrings : public ElementInterface
        {
        public:
            FrameStrings();
            FrameStrings( const PositiveInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PositiveInteger getValue() const;
            void setValue( const PositiveInteger& value );

        private:
            PositiveInteger myValue;
        };
        
        
        class FrameFrets;
        using FrameFretsPtr = std::shared_ptr<FrameFrets>;
        using FrameFretsUPtr = std::unique_ptr<FrameFrets>;
        using FrameFretsSet = std::vector<FrameFretsPtr>;
        using FrameFretsSetIter = FrameFretsSet::iterator;
        using FrameFretsSetIterConst = FrameFretsSet::const_iterator;
        inline FrameFretsPtr makeFrameFrets() { return std::make_shared<FrameFrets>(); }
		inline FrameFretsPtr makeFrameFrets( const PositiveInteger& value ) { return std::make_shared<FrameFrets>( value ); }
		inline FrameFretsPtr makeFrameFrets( PositiveInteger&& value ) { return std::make_shared<FrameFrets>( std::move( value ) ); }

        class FrameFrets : public ElementInterface
        {
        public:
            FrameFrets();
            FrameFrets( const PositiveInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PositiveInteger getValue() const;
            void setValue( const PositiveInteger& value );

        private:
            PositiveInteger myValue;
        };
        
        
        class MetronomeRelation;
        using MetronomeRelationPtr = std::shared_ptr<MetronomeRelation>;
        using MetronomeRelationUPtr = std::unique_ptr<MetronomeRelation>;
        using MetronomeRelationSet = std::vector<MetronomeRelationPtr>;
        using MetronomeRelationSetIter = MetronomeRelationSet::iterator;
        using MetronomeRelationSetIterConst = MetronomeRelationSet::const_iterator;
        inline MetronomeRelationPtr makeMetronomeRelation() { return std::make_shared<MetronomeRelation>(); }

        class MetronomeRelation : public ElementInterface
        {
        public:
            MetronomeRelation();
            MetronomeRelation( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class MetronomeType;
        using MetronomeTypePtr = std::shared_ptr<MetronomeType>;
        using MetronomeTypeUPtr = std::unique_ptr<MetronomeType>;
        using MetronomeTypeSet = std::vector<MetronomeTypePtr>;
        using MetronomeTypeSetIter = MetronomeTypeSet::iterator;
        using MetronomeTypeSetIterConst = MetronomeTypeSet::const_iterator;
        inline MetronomeTypePtr makeMetronomeType() { return std::make_shared<MetronomeType>(); }
		inline MetronomeTypePtr makeMetronomeType( const NoteTypeValue& value ) { return std::make_shared<MetronomeType>( value ); }
		inline MetronomeTypePtr makeMetronomeType( NoteTypeValue&& value ) { return std::make_shared<MetronomeType>( std::move( value ) ); }

        class MetronomeType : public ElementInterface
        {
        public:
            MetronomeType();
            MetronomeType( const NoteTypeValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NoteTypeValue getValue() const;
            void setValue( const NoteTypeValue& value );

        private:
            NoteTypeValue myValue;
        };
        
        
        class PedalStep;
        using PedalStepPtr = std::shared_ptr<PedalStep>;
        using PedalStepUPtr = std::unique_ptr<PedalStep>;
        using PedalStepSet = std::vector<PedalStepPtr>;
        using PedalStepSetIter = PedalStepSet::iterator;
        using PedalStepSetIterConst = PedalStepSet::const_iterator;
        inline PedalStepPtr makePedalStep() { return std::make_shared<PedalStep>(); }
		inline PedalStepPtr makePedalStep( const StepEnum& value ) { return std::make_shared<PedalStep>( value ); }
		inline PedalStepPtr makePedalStep( StepEnum&& value ) { return std::make_shared<PedalStep>( std::move( value ) ); }

        class PedalStep : public ElementInterface
        {
        public:
            PedalStep();
            PedalStep( const StepEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StepEnum getValue() const;
            void setValue( const StepEnum& value );

        private:
            StepEnum myValue;
        };
        
        
        class PedalAlter;
        using PedalAlterPtr = std::shared_ptr<PedalAlter>;
        using PedalAlterUPtr = std::unique_ptr<PedalAlter>;
        using PedalAlterSet = std::vector<PedalAlterPtr>;
        using PedalAlterSetIter = PedalAlterSet::iterator;
        using PedalAlterSetIterConst = PedalAlterSet::const_iterator;
        inline PedalAlterPtr makePedalAlter() { return std::make_shared<PedalAlter>(); }
		inline PedalAlterPtr makePedalAlter( const Semitones& value ) { return std::make_shared<PedalAlter>( value ); }
		inline PedalAlterPtr makePedalAlter( Semitones&& value ) { return std::make_shared<PedalAlter>( std::move( value ) ); }

        class PedalAlter : public ElementInterface
        {
        public:
            PedalAlter();
            PedalAlter( const Semitones& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Semitones getValue() const;
            void setValue( const Semitones& value );

        private:
            Semitones myValue;
        };
        
        
        class Glass;
        using GlassPtr = std::shared_ptr<Glass>;
        using GlassUPtr = std::unique_ptr<Glass>;
        using GlassSet = std::vector<GlassPtr>;
        using GlassSetIter = GlassSet::iterator;
        using GlassSetIterConst = GlassSet::const_iterator;
        inline GlassPtr makeGlass() { return std::make_shared<Glass>(); }
		inline GlassPtr makeGlass( const GlassEnum& value ) { return std::make_shared<Glass>( value ); }
		inline GlassPtr makeGlass( GlassEnum&& value ) { return std::make_shared<Glass>( std::move( value ) ); }

        class Glass : public ElementInterface
        {
        public:
            Glass();
            Glass( const GlassEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            GlassEnum getValue() const;
            void setValue( const GlassEnum& value );

        private:
            GlassEnum myValue;
        };
        
        
        class Metal;
        using MetalPtr = std::shared_ptr<Metal>;
        using MetalUPtr = std::unique_ptr<Metal>;
        using MetalSet = std::vector<MetalPtr>;
        using MetalSetIter = MetalSet::iterator;
        using MetalSetIterConst = MetalSet::const_iterator;
        inline MetalPtr makeMetal() { return std::make_shared<Metal>(); }
		inline MetalPtr makeMetal( const MetalEnum& value ) { return std::make_shared<Metal>( value ); }
		inline MetalPtr makeMetal( MetalEnum&& value ) { return std::make_shared<Metal>( std::move( value ) ); }

        class Metal : public ElementInterface
        {
        public:
            Metal();
            Metal( const MetalEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MetalEnum getValue() const;
            void setValue( const MetalEnum& value );

        private:
            MetalEnum myValue;
        };
        
        
        class Wood;
        using WoodPtr = std::shared_ptr<Wood>;
        using WoodUPtr = std::unique_ptr<Wood>;
        using WoodSet = std::vector<WoodPtr>;
        using WoodSetIter = WoodSet::iterator;
        using WoodSetIterConst = WoodSet::const_iterator;
        inline WoodPtr makeWood() { return std::make_shared<Wood>(); }
		inline WoodPtr makeWood( const WoodEnum& value ) { return std::make_shared<Wood>( value ); }
		inline WoodPtr makeWood( WoodEnum&& value ) { return std::make_shared<Wood>( std::move( value ) ); }

        class Wood : public ElementInterface
        {
        public:
            Wood();
            Wood( const WoodEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            WoodEnum getValue() const;
            void setValue( const WoodEnum& value );

        private:
            WoodEnum myValue;
        };
        
        
        class Pitched;
        using PitchedPtr = std::shared_ptr<Pitched>;
        using PitchedUPtr = std::unique_ptr<Pitched>;
        using PitchedSet = std::vector<PitchedPtr>;
        using PitchedSetIter = PitchedSet::iterator;
        using PitchedSetIterConst = PitchedSet::const_iterator;
        inline PitchedPtr makePitched() { return std::make_shared<Pitched>(); }
		inline PitchedPtr makePitched( const PitchedEnum& value ) { return std::make_shared<Pitched>( value ); }
		inline PitchedPtr makePitched( PitchedEnum&& value ) { return std::make_shared<Pitched>( std::move( value ) ); }

        class Pitched : public ElementInterface
        {
        public:
            Pitched();
            Pitched( const PitchedEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PitchedEnum getValue() const;
            void setValue( const PitchedEnum& value );

        private:
            PitchedEnum myValue;
        };
        
        
        class Membrane;
        using MembranePtr = std::shared_ptr<Membrane>;
        using MembraneUPtr = std::unique_ptr<Membrane>;
        using MembraneSet = std::vector<MembranePtr>;
        using MembraneSetIter = MembraneSet::iterator;
        using MembraneSetIterConst = MembraneSet::const_iterator;
        inline MembranePtr makeMembrane() { return std::make_shared<Membrane>(); }
		inline MembranePtr makeMembrane( const MembraneEnum& value ) { return std::make_shared<Membrane>( value ); }
		inline MembranePtr makeMembrane( MembraneEnum&& value ) { return std::make_shared<Membrane>( std::move( value ) ); }

        class Membrane : public ElementInterface
        {
        public:
            Membrane();
            Membrane( const MembraneEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MembraneEnum getValue() const;
            void setValue( const MembraneEnum& value );

        private:
            MembraneEnum myValue;
        };
        
        
        class Effect;
        using EffectPtr = std::shared_ptr<Effect>;
        using EffectUPtr = std::unique_ptr<Effect>;
        using EffectSet = std::vector<EffectPtr>;
        using EffectSetIter = EffectSet::iterator;
        using EffectSetIterConst = EffectSet::const_iterator;
        inline EffectPtr makeEffect() { return std::make_shared<Effect>(); }
		inline EffectPtr makeEffect( const EffectEnum& value ) { return std::make_shared<Effect>( value ); }
		inline EffectPtr makeEffect( EffectEnum&& value ) { return std::make_shared<Effect>( std::move( value ) ); }

        class Effect : public ElementInterface
        {
        public:
            Effect();
            Effect( const EffectEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EffectEnum getValue() const;
            void setValue( const EffectEnum& value );

        private:
            EffectEnum myValue;
        };
        
        
        class StickLocation;
        using StickLocationPtr = std::shared_ptr<StickLocation>;
        using StickLocationUPtr = std::unique_ptr<StickLocation>;
        using StickLocationSet = std::vector<StickLocationPtr>;
        using StickLocationSetIter = StickLocationSet::iterator;
        using StickLocationSetIterConst = StickLocationSet::const_iterator;
        inline StickLocationPtr makeStickLocation() { return std::make_shared<StickLocation>(); }
		inline StickLocationPtr makeStickLocation( const StickLocationEnum& value ) { return std::make_shared<StickLocation>( value ); }
		inline StickLocationPtr makeStickLocation( StickLocationEnum&& value ) { return std::make_shared<StickLocation>( std::move( value ) ); }

        class StickLocation : public ElementInterface
        {
        public:
            StickLocation();
            StickLocation( const StickLocationEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StickLocationEnum getValue() const;
            void setValue( const StickLocationEnum& value );

        private:
            StickLocationEnum myValue;
        };
        
        
        class OtherPercussion;
        using OtherPercussionPtr = std::shared_ptr<OtherPercussion>;
        using OtherPercussionUPtr = std::unique_ptr<OtherPercussion>;
        using OtherPercussionSet = std::vector<OtherPercussionPtr>;
        using OtherPercussionSetIter = OtherPercussionSet::iterator;
        using OtherPercussionSetIterConst = OtherPercussionSet::const_iterator;
        inline OtherPercussionPtr makeOtherPercussion() { return std::make_shared<OtherPercussion>(); }
		inline OtherPercussionPtr makeOtherPercussion( const XsString& value ) { return std::make_shared<OtherPercussion>( value ); }
		inline OtherPercussionPtr makeOtherPercussion( XsString&& value ) { return std::make_shared<OtherPercussion>( std::move( value ) ); }

        class OtherPercussion : public ElementInterface
        {
        public:
            OtherPercussion();
            OtherPercussion( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class StickType;
        using StickTypePtr = std::shared_ptr<StickType>;
        using StickTypeUPtr = std::unique_ptr<StickType>;
        using StickTypeSet = std::vector<StickTypePtr>;
        using StickTypeSetIter = StickTypeSet::iterator;
        using StickTypeSetIterConst = StickTypeSet::const_iterator;
        inline StickTypePtr makeStickType() { return std::make_shared<StickType>(); }
		inline StickTypePtr makeStickType( const StickTypeEnum& value ) { return std::make_shared<StickType>( value ); }
		inline StickTypePtr makeStickType( StickTypeEnum&& value ) { return std::make_shared<StickType>( std::move( value ) ); }

        class StickType : public ElementInterface
        {
        public:
            StickType();
            StickType( const StickTypeEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StickTypeEnum getValue() const;
            void setValue( const StickTypeEnum& value );

        private:
            StickTypeEnum myValue;
        };
        
        
        class StickMaterial;
        using StickMaterialPtr = std::shared_ptr<StickMaterial>;
        using StickMaterialUPtr = std::unique_ptr<StickMaterial>;
        using StickMaterialSet = std::vector<StickMaterialPtr>;
        using StickMaterialSetIter = StickMaterialSet::iterator;
        using StickMaterialSetIterConst = StickMaterialSet::const_iterator;
        inline StickMaterialPtr makeStickMaterial() { return std::make_shared<StickMaterial>(); }
		inline StickMaterialPtr makeStickMaterial( const StickMaterialEnum& value ) { return std::make_shared<StickMaterial>( value ); }
		inline StickMaterialPtr makeStickMaterial( StickMaterialEnum&& value ) { return std::make_shared<StickMaterial>( std::move( value ) ); }

        class StickMaterial : public ElementInterface
        {
        public:
            StickMaterial();
            StickMaterial( const StickMaterialEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StickMaterialEnum getValue() const;
            void setValue( const StickMaterialEnum& value );

        private:
            StickMaterialEnum myValue;
        };
        
        
        class EncodingDate;
        using EncodingDatePtr = std::shared_ptr<EncodingDate>;
        using EncodingDateUPtr = std::unique_ptr<EncodingDate>;
        using EncodingDateSet = std::vector<EncodingDatePtr>;
        using EncodingDateSetIter = EncodingDateSet::iterator;
        using EncodingDateSetIterConst = EncodingDateSet::const_iterator;
        inline EncodingDatePtr makeEncodingDate() { return std::make_shared<EncodingDate>(); }
		inline EncodingDatePtr makeEncodingDate( const Date& value ) { return std::make_shared<EncodingDate>( value ); }
		inline EncodingDatePtr makeEncodingDate( Date&& value ) { return std::make_shared<EncodingDate>( std::move( value ) ); }

        class EncodingDate : public ElementInterface
        {
        public:
            EncodingDate();
            EncodingDate( const Date& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Date getValue() const;
            void setValue( const Date& value );

        private:
            Date myValue;
        };
        
        
        class Software;
        using SoftwarePtr = std::shared_ptr<Software>;
        using SoftwareUPtr = std::unique_ptr<Software>;
        using SoftwareSet = std::vector<SoftwarePtr>;
        using SoftwareSetIter = SoftwareSet::iterator;
        using SoftwareSetIterConst = SoftwareSet::const_iterator;
        inline SoftwarePtr makeSoftware() { return std::make_shared<Software>(); }
		inline SoftwarePtr makeSoftware( const XsString& value ) { return std::make_shared<Software>( value ); }
		inline SoftwarePtr makeSoftware( XsString&& value ) { return std::make_shared<Software>( std::move( value ) ); }

        class Software : public ElementInterface
        {
        public:
            Software();
            Software( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class EncodingDescription;
        using EncodingDescriptionPtr = std::shared_ptr<EncodingDescription>;
        using EncodingDescriptionUPtr = std::unique_ptr<EncodingDescription>;
        using EncodingDescriptionSet = std::vector<EncodingDescriptionPtr>;
        using EncodingDescriptionSetIter = EncodingDescriptionSet::iterator;
        using EncodingDescriptionSetIterConst = EncodingDescriptionSet::const_iterator;
        inline EncodingDescriptionPtr makeEncodingDescription() { return std::make_shared<EncodingDescription>(); }
		inline EncodingDescriptionPtr makeEncodingDescription( const XsString& value ) { return std::make_shared<EncodingDescription>( value ); }
		inline EncodingDescriptionPtr makeEncodingDescription( XsString&& value ) { return std::make_shared<EncodingDescription>( std::move( value ) ); }

        class EncodingDescription : public ElementInterface
        {
        public:
            EncodingDescription();
            EncodingDescription( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class Source;
        using SourcePtr = std::shared_ptr<Source>;
        using SourceUPtr = std::unique_ptr<Source>;
        using SourceSet = std::vector<SourcePtr>;
        using SourceSetIter = SourceSet::iterator;
        using SourceSetIterConst = SourceSet::const_iterator;
        inline SourcePtr makeSource() { return std::make_shared<Source>(); }
		inline SourcePtr makeSource( const XsString& value ) { return std::make_shared<Source>( value ); }
		inline SourcePtr makeSource( XsString&& value ) { return std::make_shared<Source>( std::move( value ) ); }

        class Source : public ElementInterface
        {
        public:
            Source();
            Source( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class MeasureDistance;
        using MeasureDistancePtr = std::shared_ptr<MeasureDistance>;
        using MeasureDistanceUPtr = std::unique_ptr<MeasureDistance>;
        using MeasureDistanceSet = std::vector<MeasureDistancePtr>;
        using MeasureDistanceSetIter = MeasureDistanceSet::iterator;
        using MeasureDistanceSetIterConst = MeasureDistanceSet::const_iterator;
        inline MeasureDistancePtr makeMeasureDistance() { return std::make_shared<MeasureDistance>(); }
		inline MeasureDistancePtr makeMeasureDistance( const TenthsValue& value ) { return std::make_shared<MeasureDistance>( value ); }
		inline MeasureDistancePtr makeMeasureDistance( TenthsValue&& value ) { return std::make_shared<MeasureDistance>( std::move( value ) ); }

        class MeasureDistance : public ElementInterface
        {
        public:
            MeasureDistance();
            MeasureDistance( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

        private:
            TenthsValue myValue;
        };
        
        
        class PageHeight;
        using PageHeightPtr = std::shared_ptr<PageHeight>;
        using PageHeightUPtr = std::unique_ptr<PageHeight>;
        using PageHeightSet = std::vector<PageHeightPtr>;
        using PageHeightSetIter = PageHeightSet::iterator;
        using PageHeightSetIterConst = PageHeightSet::const_iterator;
        inline PageHeightPtr makePageHeight() { return std::make_shared<PageHeight>(); }
		inline PageHeightPtr makePageHeight( const TenthsValue& value ) { return std::make_shared<PageHeight>( value ); }
		inline PageHeightPtr makePageHeight( TenthsValue&& value ) { return std::make_shared<PageHeight>( std::move( value ) ); }

        class PageHeight : public ElementInterface
        {
        public:
            PageHeight();
            PageHeight( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

        private:
            TenthsValue myValue;
        };
        
        
        class PageWidth;
        using PageWidthPtr = std::shared_ptr<PageWidth>;
        using PageWidthUPtr = std::unique_ptr<PageWidth>;
        using PageWidthSet = std::vector<PageWidthPtr>;
        using PageWidthSetIter = PageWidthSet::iterator;
        using PageWidthSetIterConst = PageWidthSet::const_iterator;
        inline PageWidthPtr makePageWidth() { return std::make_shared<PageWidth>(); }
		inline PageWidthPtr makePageWidth( const TenthsValue& value ) { return std::make_shared<PageWidth>( value ); }
		inline PageWidthPtr makePageWidth( TenthsValue&& value ) { return std::make_shared<PageWidth>( std::move( value ) ); }

        class PageWidth : public ElementInterface
        {
        public:
            PageWidth();
            PageWidth( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

        private:
            TenthsValue myValue;
        };
        
        
        class Millimeters;
        using MillimetersPtr = std::shared_ptr<Millimeters>;
        using MillimetersUPtr = std::unique_ptr<Millimeters>;
        using MillimetersSet = std::vector<MillimetersPtr>;
        using MillimetersSetIter = MillimetersSet::iterator;
        using MillimetersSetIterConst = MillimetersSet::const_iterator;
        inline MillimetersPtr makeMillimeters() { return std::make_shared<Millimeters>(); }
		inline MillimetersPtr makeMillimeters( const MillimetersValue& value ) { return std::make_shared<Millimeters>( value ); }
		inline MillimetersPtr makeMillimeters( MillimetersValue&& value ) { return std::make_shared<Millimeters>( std::move( value ) ); }

        class Millimeters : public ElementInterface
        {
        public:
            Millimeters();
            Millimeters( const MillimetersValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MillimetersValue getValue() const;
            void setValue( const MillimetersValue& value );

        private:
            MillimetersValue myValue;
        };
        
        
        class Tenths;
        using TenthsPtr = std::shared_ptr<Tenths>;
        using TenthsUPtr = std::unique_ptr<Tenths>;
        using TenthsSet = std::vector<TenthsPtr>;
        using TenthsSetIter = TenthsSet::iterator;
        using TenthsSetIterConst = TenthsSet::const_iterator;
        inline TenthsPtr makeTenths() { return std::make_shared<Tenths>(); }
		inline TenthsPtr makeTenths( const TenthsValue& value ) { return std::make_shared<Tenths>( value ); }
		inline TenthsPtr makeTenths( TenthsValue&& value ) { return std::make_shared<Tenths>( std::move( value ) ); }

        class Tenths : public ElementInterface
        {
        public:
            Tenths();
            Tenths( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

        private:
            TenthsValue myValue;
        };
        
        
        class StaffDistance;
        using StaffDistancePtr = std::shared_ptr<StaffDistance>;
        using StaffDistanceUPtr = std::unique_ptr<StaffDistance>;
        using StaffDistanceSet = std::vector<StaffDistancePtr>;
        using StaffDistanceSetIter = StaffDistanceSet::iterator;
        using StaffDistanceSetIterConst = StaffDistanceSet::const_iterator;
        inline StaffDistancePtr makeStaffDistance() { return std::make_shared<StaffDistance>(); }
		inline StaffDistancePtr makeStaffDistance( const TenthsValue& value ) { return std::make_shared<StaffDistance>( value ); }
		inline StaffDistancePtr makeStaffDistance( TenthsValue&& value ) { return std::make_shared<StaffDistance>( std::move( value ) ); }

        class StaffDistance : public ElementInterface
        {
        public:
            StaffDistance();
            StaffDistance( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

        private:
            TenthsValue myValue;
        };
        
        
        class SystemDistance;
        using SystemDistancePtr = std::shared_ptr<SystemDistance>;
        using SystemDistanceUPtr = std::unique_ptr<SystemDistance>;
        using SystemDistanceSet = std::vector<SystemDistancePtr>;
        using SystemDistanceSetIter = SystemDistanceSet::iterator;
        using SystemDistanceSetIterConst = SystemDistanceSet::const_iterator;
        inline SystemDistancePtr makeSystemDistance() { return std::make_shared<SystemDistance>(); }
		inline SystemDistancePtr makeSystemDistance( const TenthsValue& value ) { return std::make_shared<SystemDistance>( value ); }
		inline SystemDistancePtr makeSystemDistance( TenthsValue&& value ) { return std::make_shared<SystemDistance>( std::move( value ) ); }

        class SystemDistance : public ElementInterface
        {
        public:
            SystemDistance();
            SystemDistance( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

        private:
            TenthsValue myValue;
        };
        
        
        class TopSystemDistance;
        using TopSystemDistancePtr = std::shared_ptr<TopSystemDistance>;
        using TopSystemDistanceUPtr = std::unique_ptr<TopSystemDistance>;
        using TopSystemDistanceSet = std::vector<TopSystemDistancePtr>;
        using TopSystemDistanceSetIter = TopSystemDistanceSet::iterator;
        using TopSystemDistanceSetIterConst = TopSystemDistanceSet::const_iterator;
        inline TopSystemDistancePtr makeTopSystemDistance() { return std::make_shared<TopSystemDistance>(); }
		inline TopSystemDistancePtr makeTopSystemDistance( const TenthsValue& value ) { return std::make_shared<TopSystemDistance>( value ); }
		inline TopSystemDistancePtr makeTopSystemDistance( TenthsValue&& value ) { return std::make_shared<TopSystemDistance>( std::move( value ) ); }

        class TopSystemDistance : public ElementInterface
        {
        public:
            TopSystemDistance();
            TopSystemDistance( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

        private:
            TenthsValue myValue;
        };
        
        
        class ArrowDirection;
        using ArrowDirectionPtr = std::shared_ptr<ArrowDirection>;
        using ArrowDirectionUPtr = std::unique_ptr<ArrowDirection>;
        using ArrowDirectionSet = std::vector<ArrowDirectionPtr>;
        using ArrowDirectionSetIter = ArrowDirectionSet::iterator;
        using ArrowDirectionSetIterConst = ArrowDirectionSet::const_iterator;
        inline ArrowDirectionPtr makeArrowDirection() { return std::make_shared<ArrowDirection>(); }
		inline ArrowDirectionPtr makeArrowDirection( const ArrowDirectionEnum& value ) { return std::make_shared<ArrowDirection>( value ); }
		inline ArrowDirectionPtr makeArrowDirection( ArrowDirectionEnum&& value ) { return std::make_shared<ArrowDirection>( std::move( value ) ); }

        class ArrowDirection : public ElementInterface
        {
        public:
            ArrowDirection();
            ArrowDirection( const ArrowDirectionEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ArrowDirectionEnum getValue() const;
            void setValue( const ArrowDirectionEnum& value );

        private:
            ArrowDirectionEnum myValue;
        };
        
        
        class ArrowStyle;
        using ArrowStylePtr = std::shared_ptr<ArrowStyle>;
        using ArrowStyleUPtr = std::unique_ptr<ArrowStyle>;
        using ArrowStyleSet = std::vector<ArrowStylePtr>;
        using ArrowStyleSetIter = ArrowStyleSet::iterator;
        using ArrowStyleSetIterConst = ArrowStyleSet::const_iterator;
        inline ArrowStylePtr makeArrowStyle() { return std::make_shared<ArrowStyle>(); }
		inline ArrowStylePtr makeArrowStyle( const ArrowStyleEnum& value ) { return std::make_shared<ArrowStyle>( value ); }
		inline ArrowStylePtr makeArrowStyle( ArrowStyleEnum&& value ) { return std::make_shared<ArrowStyle>( std::move( value ) ); }

        class ArrowStyle : public ElementInterface
        {
        public:
            ArrowStyle();
            ArrowStyle( const ArrowStyleEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ArrowStyleEnum getValue() const;
            void setValue( const ArrowStyleEnum& value );

        private:
            ArrowStyleEnum myValue;
        };
        
        
        class CircularArrow;
        using CircularArrowPtr = std::shared_ptr<CircularArrow>;
        using CircularArrowUPtr = std::unique_ptr<CircularArrow>;
        using CircularArrowSet = std::vector<CircularArrowPtr>;
        using CircularArrowSetIter = CircularArrowSet::iterator;
        using CircularArrowSetIterConst = CircularArrowSet::const_iterator;
        inline CircularArrowPtr makeCircularArrow() { return std::make_shared<CircularArrow>(); }
		inline CircularArrowPtr makeCircularArrow( const CircularArrowEnum& value ) { return std::make_shared<CircularArrow>( value ); }
		inline CircularArrowPtr makeCircularArrow( CircularArrowEnum&& value ) { return std::make_shared<CircularArrow>( std::move( value ) ); }

        class CircularArrow : public ElementInterface
        {
        public:
            CircularArrow();
            CircularArrow( const CircularArrowEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            CircularArrowEnum getValue() const;
            void setValue( const CircularArrowEnum& value );

        private:
            CircularArrowEnum myValue;
        };
        
        
        class BendAlter;
        using BendAlterPtr = std::shared_ptr<BendAlter>;
        using BendAlterUPtr = std::unique_ptr<BendAlter>;
        using BendAlterSet = std::vector<BendAlterPtr>;
        using BendAlterSetIter = BendAlterSet::iterator;
        using BendAlterSetIterConst = BendAlterSet::const_iterator;
        inline BendAlterPtr makeBendAlter() { return std::make_shared<BendAlter>(); }
		inline BendAlterPtr makeBendAlter( const Semitones& value ) { return std::make_shared<BendAlter>( value ); }
		inline BendAlterPtr makeBendAlter( Semitones&& value ) { return std::make_shared<BendAlter>( std::move( value ) ); }

        class BendAlter : public ElementInterface
        {
        public:
            BendAlter();
            BendAlter( const Semitones& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Semitones getValue() const;
            void setValue( const Semitones& value );

        private:
            Semitones myValue;
        };
        
        
        class HoleType;
        using HoleTypePtr = std::shared_ptr<HoleType>;
        using HoleTypeUPtr = std::unique_ptr<HoleType>;
        using HoleTypeSet = std::vector<HoleTypePtr>;
        using HoleTypeSetIter = HoleTypeSet::iterator;
        using HoleTypeSetIterConst = HoleTypeSet::const_iterator;
        inline HoleTypePtr makeHoleType() { return std::make_shared<HoleType>(); }
		inline HoleTypePtr makeHoleType( const XsString& value ) { return std::make_shared<HoleType>( value ); }
		inline HoleTypePtr makeHoleType( XsString&& value ) { return std::make_shared<HoleType>( std::move( value ) ); }

        class HoleType : public ElementInterface
        {
        public:
            HoleType();
            HoleType( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class HoleShape;
        using HoleShapePtr = std::shared_ptr<HoleShape>;
        using HoleShapeUPtr = std::unique_ptr<HoleShape>;
        using HoleShapeSet = std::vector<HoleShapePtr>;
        using HoleShapeSetIter = HoleShapeSet::iterator;
        using HoleShapeSetIterConst = HoleShapeSet::const_iterator;
        inline HoleShapePtr makeHoleShape() { return std::make_shared<HoleShape>(); }
		inline HoleShapePtr makeHoleShape( const XsString& value ) { return std::make_shared<HoleShape>( value ); }
		inline HoleShapePtr makeHoleShape( XsString&& value ) { return std::make_shared<HoleShape>( std::move( value ) ); }

        class HoleShape : public ElementInterface
        {
        public:
            HoleShape();
            HoleShape( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class Syllabic;
        using SyllabicPtr = std::shared_ptr<Syllabic>;
        using SyllabicUPtr = std::unique_ptr<Syllabic>;
        using SyllabicSet = std::vector<SyllabicPtr>;
        using SyllabicSetIter = SyllabicSet::iterator;
        using SyllabicSetIterConst = SyllabicSet::const_iterator;
        inline SyllabicPtr makeSyllabic() { return std::make_shared<Syllabic>(); }
		inline SyllabicPtr makeSyllabic( const SyllabicEnum& value ) { return std::make_shared<Syllabic>( value ); }
		inline SyllabicPtr makeSyllabic( SyllabicEnum&& value ) { return std::make_shared<Syllabic>( std::move( value ) ); }

        class Syllabic : public ElementInterface
        {
        public:
            Syllabic();
            Syllabic( const SyllabicEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            SyllabicEnum getValue() const;
            void setValue( const SyllabicEnum& value );

        private:
            SyllabicEnum myValue;
        };
        
        
        
        
        
        class Step;
        using StepPtr = std::shared_ptr<Step>;
        using StepUPtr = std::unique_ptr<Step>;
        using StepSet = std::vector<StepPtr>;
        using StepSetIter = StepSet::iterator;
        using StepSetIterConst = StepSet::const_iterator;
        inline StepPtr makeStep() { return std::make_shared<Step>(); }
		inline StepPtr makeStep( const StepEnum& value ) { return std::make_shared<Step>( value ); }
		inline StepPtr makeStep( StepEnum&& value ) { return std::make_shared<Step>( std::move( value ) ); }

        class Step : public ElementInterface
        {
        public:
            Step();
            Step( const StepEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StepEnum getValue() const;
            void setValue( const StepEnum& value );

        private:
            StepEnum myValue;
        };
        
        
        class Alter;
        using AlterPtr = std::shared_ptr<Alter>;
        using AlterUPtr = std::unique_ptr<Alter>;
        using AlterSet = std::vector<AlterPtr>;
        using AlterSetIter = AlterSet::iterator;
        using AlterSetIterConst = AlterSet::const_iterator;
        inline AlterPtr makeAlter() { return std::make_shared<Alter>(); }
		inline AlterPtr makeAlter( const Semitones& value ) { return std::make_shared<Alter>( value ); }
		inline AlterPtr makeAlter( Semitones&& value ) { return std::make_shared<Alter>( std::move( value ) ); }

        class Alter : public ElementInterface
        {
        public:
            Alter();
            Alter( const Semitones& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Semitones getValue() const;
            void setValue( const Semitones& value );

        private:
            Semitones myValue;
        };
        
        
        class Octave;
        using OctavePtr = std::shared_ptr<Octave>;
        using OctaveUPtr = std::unique_ptr<Octave>;
        using OctaveSet = std::vector<OctavePtr>;
        using OctaveSetIter = OctaveSet::iterator;
        using OctaveSetIterConst = OctaveSet::const_iterator;
        inline OctavePtr makeOctave() { return std::make_shared<Octave>(); }
		inline OctavePtr makeOctave( const OctaveValue& value ) { return std::make_shared<Octave>( value ); }
		inline OctavePtr makeOctave( OctaveValue&& value ) { return std::make_shared<Octave>( std::move( value ) ); }

        class Octave : public ElementInterface
        {
        public:
            Octave();
            Octave( const OctaveValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OctaveValue getValue() const;
            void setValue( const OctaveValue& value );

        private:
            OctaveValue myValue;
        };
        
        
        class ActualNotes;
        using ActualNotesPtr = std::shared_ptr<ActualNotes>;
        using ActualNotesUPtr = std::unique_ptr<ActualNotes>;
        using ActualNotesSet = std::vector<ActualNotesPtr>;
        using ActualNotesSetIter = ActualNotesSet::iterator;
        using ActualNotesSetIterConst = ActualNotesSet::const_iterator;
        inline ActualNotesPtr makeActualNotes() { return std::make_shared<ActualNotes>(); }
		inline ActualNotesPtr makeActualNotes( const NonNegativeInteger& value ) { return std::make_shared<ActualNotes>( value ); }
		inline ActualNotesPtr makeActualNotes( NonNegativeInteger&& value ) { return std::make_shared<ActualNotes>( std::move( value ) ); }

        class ActualNotes : public ElementInterface
        {
        public:
            ActualNotes();
            ActualNotes( const NonNegativeInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NonNegativeInteger getValue() const;
            void setValue( const NonNegativeInteger& value );

        private:
            NonNegativeInteger myValue;
        };
        
        
        class NormalNotes;
        using NormalNotesPtr = std::shared_ptr<NormalNotes>;
        using NormalNotesUPtr = std::unique_ptr<NormalNotes>;
        using NormalNotesSet = std::vector<NormalNotesPtr>;
        using NormalNotesSetIter = NormalNotesSet::iterator;
        using NormalNotesSetIterConst = NormalNotesSet::const_iterator;
        inline NormalNotesPtr makeNormalNotes() { return std::make_shared<NormalNotes>(); }
		inline NormalNotesPtr makeNormalNotes( const NonNegativeInteger& value ) { return std::make_shared<NormalNotes>( value ); }
		inline NormalNotesPtr makeNormalNotes( NonNegativeInteger&& value ) { return std::make_shared<NormalNotes>( std::move( value ) ); }

        class NormalNotes : public ElementInterface
        {
        public:
            NormalNotes();
            NormalNotes( const NonNegativeInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NonNegativeInteger getValue() const;
            void setValue( const NonNegativeInteger& value );

        private:
            NonNegativeInteger myValue;
        };
        
        
        class NormalType;
        using NormalTypePtr = std::shared_ptr<NormalType>;
        using NormalTypeUPtr = std::unique_ptr<NormalType>;
        using NormalTypeSet = std::vector<NormalTypePtr>;
        using NormalTypeSetIter = NormalTypeSet::iterator;
        using NormalTypeSetIterConst = NormalTypeSet::const_iterator;
        inline NormalTypePtr makeNormalType() { return std::make_shared<NormalType>(); }
		inline NormalTypePtr makeNormalType( const NoteTypeValue& value ) { return std::make_shared<NormalType>( value ); }
		inline NormalTypePtr makeNormalType( NoteTypeValue&& value ) { return std::make_shared<NormalType>( std::move( value ) ); }

        class NormalType : public ElementInterface
        {
        public:
            NormalType();
            NormalType( const NoteTypeValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NoteTypeValue getValue() const;
            void setValue( const NoteTypeValue& value );

        private:
            NoteTypeValue myValue;
        };
        
        
        class CreditType;
        using CreditTypePtr = std::shared_ptr<CreditType>;
        using CreditTypeUPtr = std::unique_ptr<CreditType>;
        using CreditTypeSet = std::vector<CreditTypePtr>;
        using CreditTypeSetIter = CreditTypeSet::iterator;
        using CreditTypeSetIterConst = CreditTypeSet::const_iterator;
        inline CreditTypePtr makeCreditType() { return std::make_shared<CreditType>(); }
		inline CreditTypePtr makeCreditType( const XsString& value ) { return std::make_shared<CreditType>( value ); }
		inline CreditTypePtr makeCreditType( XsString&& value ) { return std::make_shared<CreditType>( std::move( value ) ); }

        class CreditType : public ElementInterface
        {
        public:
            CreditType();
            CreditType( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class InstrumentName;
        using InstrumentNamePtr = std::shared_ptr<InstrumentName>;
        using InstrumentNameUPtr = std::unique_ptr<InstrumentName>;
        using InstrumentNameSet = std::vector<InstrumentNamePtr>;
        using InstrumentNameSetIter = InstrumentNameSet::iterator;
        using InstrumentNameSetIterConst = InstrumentNameSet::const_iterator;
        inline InstrumentNamePtr makeInstrumentName() { return std::make_shared<InstrumentName>(); }
		inline InstrumentNamePtr makeInstrumentName( const XsString& value ) { return std::make_shared<InstrumentName>( value ); }
		inline InstrumentNamePtr makeInstrumentName( XsString&& value ) { return std::make_shared<InstrumentName>( std::move( value ) ); }

        class InstrumentName : public ElementInterface
        {
        public:
            InstrumentName();
            InstrumentName( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class InstrumentAbbreviation;
        using InstrumentAbbreviationPtr = std::shared_ptr<InstrumentAbbreviation>;
        using InstrumentAbbreviationUPtr = std::unique_ptr<InstrumentAbbreviation>;
        using InstrumentAbbreviationSet = std::vector<InstrumentAbbreviationPtr>;
        using InstrumentAbbreviationSetIter = InstrumentAbbreviationSet::iterator;
        using InstrumentAbbreviationSetIterConst = InstrumentAbbreviationSet::const_iterator;
        inline InstrumentAbbreviationPtr makeInstrumentAbbreviation() { return std::make_shared<InstrumentAbbreviation>(); }
		inline InstrumentAbbreviationPtr makeInstrumentAbbreviation( const XsString& value ) { return std::make_shared<InstrumentAbbreviation>( value ); }
		inline InstrumentAbbreviationPtr makeInstrumentAbbreviation( XsString&& value ) { return std::make_shared<InstrumentAbbreviation>( std::move( value ) ); }

        class InstrumentAbbreviation : public ElementInterface
        {
        public:
            InstrumentAbbreviation();
            InstrumentAbbreviation( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class InstrumentSound;
        using InstrumentSoundPtr = std::shared_ptr<InstrumentSound>;
        using InstrumentSoundUPtr = std::unique_ptr<InstrumentSound>;
        using InstrumentSoundSet = std::vector<InstrumentSoundPtr>;
        using InstrumentSoundSetIter = InstrumentSoundSet::iterator;
        using InstrumentSoundSetIterConst = InstrumentSoundSet::const_iterator;
        inline InstrumentSoundPtr makeInstrumentSound() { return std::make_shared<InstrumentSound>(); }
		inline InstrumentSoundPtr makeInstrumentSound( const XsString& value ) { return std::make_shared<InstrumentSound>( value ); }
		inline InstrumentSoundPtr makeInstrumentSound( XsString&& value ) { return std::make_shared<InstrumentSound>( std::move( value ) ); }

        class InstrumentSound : public ElementInterface
        {
        public:
            InstrumentSound();
            InstrumentSound( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class Ensemble;
        using EnsemblePtr = std::shared_ptr<Ensemble>;
        using EnsembleUPtr = std::unique_ptr<Ensemble>;
        using EnsembleSet = std::vector<EnsemblePtr>;
        using EnsembleSetIter = EnsembleSet::iterator;
        using EnsembleSetIterConst = EnsembleSet::const_iterator;
        inline EnsemblePtr makeEnsemble() { return std::make_shared<Ensemble>(); }
		inline EnsemblePtr makeEnsemble( const PositiveIntegerOrEmpty& value ) { return std::make_shared<Ensemble>( value ); }
		inline EnsemblePtr makeEnsemble( PositiveIntegerOrEmpty&& value ) { return std::make_shared<Ensemble>( std::move( value ) ); }

        class Ensemble : public ElementInterface
        {
        public:
            Ensemble();
            Ensemble( const PositiveIntegerOrEmpty& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PositiveIntegerOrEmpty getValue() const;
            void setValue( const PositiveIntegerOrEmpty& value );

        private:
            PositiveIntegerOrEmpty myValue;
        };
        
        
        class Group;
        using GroupPtr = std::shared_ptr<Group>;
        using GroupUPtr = std::unique_ptr<Group>;
        using GroupSet = std::vector<GroupPtr>;
        using GroupSetIter = GroupSet::iterator;
        using GroupSetIterConst = GroupSet::const_iterator;
        inline GroupPtr makeGroup() { return std::make_shared<Group>(); }
		inline GroupPtr makeGroup( const XsString& value ) { return std::make_shared<Group>( value ); }
		inline GroupPtr makeGroup( XsString&& value ) { return std::make_shared<Group>( std::move( value ) ); }

        class Group : public ElementInterface
        {
        public:
            Group();
            Group( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class VirtualLibrary;
        using VirtualLibraryPtr = std::shared_ptr<VirtualLibrary>;
        using VirtualLibraryUPtr = std::unique_ptr<VirtualLibrary>;
        using VirtualLibrarySet = std::vector<VirtualLibraryPtr>;
        using VirtualLibrarySetIter = VirtualLibrarySet::iterator;
        using VirtualLibrarySetIterConst = VirtualLibrarySet::const_iterator;
        inline VirtualLibraryPtr makeVirtualLibrary() { return std::make_shared<VirtualLibrary>(); }
		inline VirtualLibraryPtr makeVirtualLibrary( const XsString& value ) { return std::make_shared<VirtualLibrary>( value ); }
		inline VirtualLibraryPtr makeVirtualLibrary( XsString&& value ) { return std::make_shared<VirtualLibrary>( std::move( value ) ); }

        class VirtualLibrary : public ElementInterface
        {
        public:
            VirtualLibrary();
            VirtualLibrary( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class VirtualName;
        using VirtualNamePtr = std::shared_ptr<VirtualName>;
        using VirtualNameUPtr = std::unique_ptr<VirtualName>;
        using VirtualNameSet = std::vector<VirtualNamePtr>;
        using VirtualNameSetIter = VirtualNameSet::iterator;
        using VirtualNameSetIterConst = VirtualNameSet::const_iterator;
        inline VirtualNamePtr makeVirtualName() { return std::make_shared<VirtualName>(); }
		inline VirtualNamePtr makeVirtualName( const XsString& value ) { return std::make_shared<VirtualName>( value ); }
		inline VirtualNamePtr makeVirtualName( XsString&& value ) { return std::make_shared<VirtualName>( std::move( value ) ); }

        class VirtualName : public ElementInterface
        {
        public:
            VirtualName();
            VirtualName( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class WorkNumber;
        using WorkNumberPtr = std::shared_ptr<WorkNumber>;
        using WorkNumberUPtr = std::unique_ptr<WorkNumber>;
        using WorkNumberSet = std::vector<WorkNumberPtr>;
        using WorkNumberSetIter = WorkNumberSet::iterator;
        using WorkNumberSetIterConst = WorkNumberSet::const_iterator;
        inline WorkNumberPtr makeWorkNumber() { return std::make_shared<WorkNumber>(); }
		inline WorkNumberPtr makeWorkNumber( const XsString& value ) { return std::make_shared<WorkNumber>( value ); }
		inline WorkNumberPtr makeWorkNumber( XsString&& value ) { return std::make_shared<WorkNumber>( std::move( value ) ); }

        class WorkNumber : public ElementInterface
        {
        public:
            WorkNumber();
            WorkNumber( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class WorkTitle;
        using WorkTitlePtr = std::shared_ptr<WorkTitle>;
        using WorkTitleUPtr = std::unique_ptr<WorkTitle>;
        using WorkTitleSet = std::vector<WorkTitlePtr>;
        using WorkTitleSetIter = WorkTitleSet::iterator;
        using WorkTitleSetIterConst = WorkTitleSet::const_iterator;
        inline WorkTitlePtr makeWorkTitle() { return std::make_shared<WorkTitle>(); }
		inline WorkTitlePtr makeWorkTitle( const XsString& value ) { return std::make_shared<WorkTitle>( value ); }
		inline WorkTitlePtr makeWorkTitle( XsString&& value ) { return std::make_shared<WorkTitle>( std::move( value ) ); }

        class WorkTitle : public ElementInterface
        {
        public:
            WorkTitle();
            WorkTitle( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class Staff;
        using StaffPtr = std::shared_ptr<Staff>;
        using StaffUPtr = std::unique_ptr<Staff>;
        using StaffSet = std::vector<StaffPtr>;
        using StaffSetIter = StaffSet::iterator;
        using StaffSetIterConst = StaffSet::const_iterator;
        inline StaffPtr makeStaff() { return std::make_shared<Staff>(); }
		inline StaffPtr makeStaff( const PositiveInteger& value ) { return std::make_shared<Staff>( value ); }
		inline StaffPtr makeStaff( PositiveInteger&& value ) { return std::make_shared<Staff>( std::move( value ) ); }

        class Staff : public ElementInterface
        {
        public:
            Staff();
            Staff( const PositiveInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PositiveInteger getValue() const;
            void setValue( const PositiveInteger& value );

        private:
            PositiveInteger myValue;
        };
        
        
        class TuningStep;
        using TuningStepPtr = std::shared_ptr<TuningStep>;
        using TuningStepUPtr = std::unique_ptr<TuningStep>;
        using TuningStepSet = std::vector<TuningStepPtr>;
        using TuningStepSetIter = TuningStepSet::iterator;
        using TuningStepSetIterConst = TuningStepSet::const_iterator;
        inline TuningStepPtr makeTuningStep() { return std::make_shared<TuningStep>(); }
		inline TuningStepPtr makeTuningStep( const StepEnum& value ) { return std::make_shared<TuningStep>( value ); }
		inline TuningStepPtr makeTuningStep( StepEnum&& value ) { return std::make_shared<TuningStep>( std::move( value ) ); }

        class TuningStep : public ElementInterface
        {
        public:
            TuningStep();
            TuningStep( const StepEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StepEnum getValue() const;
            void setValue( const StepEnum& value );

        private:
            StepEnum myValue;
        };
        
        
        class TuningAlter;
        using TuningAlterPtr = std::shared_ptr<TuningAlter>;
        using TuningAlterUPtr = std::unique_ptr<TuningAlter>;
        using TuningAlterSet = std::vector<TuningAlterPtr>;
        using TuningAlterSetIter = TuningAlterSet::iterator;
        using TuningAlterSetIterConst = TuningAlterSet::const_iterator;
        inline TuningAlterPtr makeTuningAlter() { return std::make_shared<TuningAlter>(); }
		inline TuningAlterPtr makeTuningAlter( const Semitones& value ) { return std::make_shared<TuningAlter>( value ); }
		inline TuningAlterPtr makeTuningAlter( Semitones&& value ) { return std::make_shared<TuningAlter>( std::move( value ) ); }

        class TuningAlter : public ElementInterface
        {
        public:
            TuningAlter();
            TuningAlter( const Semitones& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Semitones getValue() const;
            void setValue( const Semitones& value );

        private:
            Semitones myValue;
        };
        
        
        class TuningOctave;
        using TuningOctavePtr = std::shared_ptr<TuningOctave>;
        using TuningOctaveUPtr = std::unique_ptr<TuningOctave>;
        using TuningOctaveSet = std::vector<TuningOctavePtr>;
        using TuningOctaveSetIter = TuningOctaveSet::iterator;
        using TuningOctaveSetIterConst = TuningOctaveSet::const_iterator;
        inline TuningOctavePtr makeTuningOctave() { return std::make_shared<TuningOctave>(); }
		inline TuningOctavePtr makeTuningOctave( const OctaveValue& value ) { return std::make_shared<TuningOctave>( value ); }
		inline TuningOctavePtr makeTuningOctave( OctaveValue&& value ) { return std::make_shared<TuningOctave>( std::move( value ) ); }

        class TuningOctave : public ElementInterface
        {
        public:
            TuningOctave();
            TuningOctave( const OctaveValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OctaveValue getValue() const;
            void setValue( const OctaveValue& value );

        private:
            OctaveValue myValue;
        };
        
        
        class Voice;
        using VoicePtr = std::shared_ptr<Voice>;
        using VoiceUPtr = std::unique_ptr<Voice>;
        using VoiceSet = std::vector<VoicePtr>;
        using VoiceSetIter = VoiceSet::iterator;
        using VoiceSetIterConst = VoiceSet::const_iterator;
        inline VoicePtr makeVoice() { return std::make_shared<Voice>(); }
		inline VoicePtr makeVoice( const XsString& value ) { return std::make_shared<Voice>( value ); }
		inline VoicePtr makeVoice( XsString&& value ) { return std::make_shared<Voice>( std::move( value ) ); }

        class Voice : public ElementInterface
        {
        public:
            Voice();
            Voice( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class KeyStep;
        using KeyStepPtr = std::shared_ptr<KeyStep>;
        using KeyStepUPtr = std::unique_ptr<KeyStep>;
        using KeyStepSet = std::vector<KeyStepPtr>;
        using KeyStepSetIter = KeyStepSet::iterator;
        using KeyStepSetIterConst = KeyStepSet::const_iterator;
        inline KeyStepPtr makeKeyStep() { return std::make_shared<KeyStep>(); }
		inline KeyStepPtr makeKeyStep( const StepEnum& value ) { return std::make_shared<KeyStep>( value ); }
		inline KeyStepPtr makeKeyStep( StepEnum&& value ) { return std::make_shared<KeyStep>( std::move( value ) ); }

        class KeyStep : public ElementInterface
        {
        public:
            KeyStep();
            KeyStep( const StepEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StepEnum getValue() const;
            void setValue( const StepEnum& value );

        private:
            StepEnum myValue;
        };
        
        
        class KeyAlter;
        using KeyAlterPtr = std::shared_ptr<KeyAlter>;
        using KeyAlterUPtr = std::unique_ptr<KeyAlter>;
        using KeyAlterSet = std::vector<KeyAlterPtr>;
        using KeyAlterSetIter = KeyAlterSet::iterator;
        using KeyAlterSetIterConst = KeyAlterSet::const_iterator;
        inline KeyAlterPtr makeKeyAlter() { return std::make_shared<KeyAlter>(); }
		inline KeyAlterPtr makeKeyAlter( const Semitones& value ) { return std::make_shared<KeyAlter>( value ); }
		inline KeyAlterPtr makeKeyAlter( Semitones&& value ) { return std::make_shared<KeyAlter>( std::move( value ) ); }

        class KeyAlter : public ElementInterface
        {
        public:
            KeyAlter();
            KeyAlter( const Semitones& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Semitones getValue() const;
            void setValue( const Semitones& value );

        private:
            Semitones myValue;
        };
        
        
        class KeyAccidental;
        using KeyAccidentalPtr = std::shared_ptr<KeyAccidental>;
        using KeyAccidentalUPtr = std::unique_ptr<KeyAccidental>;
        using KeyAccidentalSet = std::vector<KeyAccidentalPtr>;
        using KeyAccidentalSetIter = KeyAccidentalSet::iterator;
        using KeyAccidentalSetIterConst = KeyAccidentalSet::const_iterator;
        inline KeyAccidentalPtr makeKeyAccidental() { return std::make_shared<KeyAccidental>(); }
		inline KeyAccidentalPtr makeKeyAccidental( const AccidentalValue& value ) { return std::make_shared<KeyAccidental>( value ); }
		inline KeyAccidentalPtr makeKeyAccidental( AccidentalValue&& value ) { return std::make_shared<KeyAccidental>( std::move( value ) ); }

        class KeyAccidental : public ElementInterface
        {
        public:
            KeyAccidental();
            KeyAccidental( const AccidentalValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            AccidentalValue getValue() const;
            void setValue( const AccidentalValue& value );

        private:
            AccidentalValue myValue;
        };
        
        
        class SlashType;
        using SlashTypePtr = std::shared_ptr<SlashType>;
        using SlashTypeUPtr = std::unique_ptr<SlashType>;
        using SlashTypeSet = std::vector<SlashTypePtr>;
        using SlashTypeSetIter = SlashTypeSet::iterator;
        using SlashTypeSetIterConst = SlashTypeSet::const_iterator;
        inline SlashTypePtr makeSlashType() { return std::make_shared<SlashType>(); }
		inline SlashTypePtr makeSlashType( const NoteTypeValue& value ) { return std::make_shared<SlashType>( value ); }
		inline SlashTypePtr makeSlashType( NoteTypeValue&& value ) { return std::make_shared<SlashType>( std::move( value ) ); }

        class SlashType : public ElementInterface
        {
        public:
            SlashType();
            SlashType( const NoteTypeValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NoteTypeValue getValue() const;
            void setValue( const NoteTypeValue& value );

        private:
            NoteTypeValue myValue;
        };
        
        
        class Beats;
        using BeatsPtr = std::shared_ptr<Beats>;
        using BeatsUPtr = std::unique_ptr<Beats>;
        using BeatsSet = std::vector<BeatsPtr>;
        using BeatsSetIter = BeatsSet::iterator;
        using BeatsSetIterConst = BeatsSet::const_iterator;
        inline BeatsPtr makeBeats() { return std::make_shared<Beats>(); }
		inline BeatsPtr makeBeats( const XsString& value ) { return std::make_shared<Beats>( value ); }
		inline BeatsPtr makeBeats( XsString&& value ) { return std::make_shared<Beats>( std::move( value ) ); }

        class Beats : public ElementInterface
        {
        public:
            Beats();
            Beats( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class BeatType;
        using BeatTypePtr = std::shared_ptr<BeatType>;
        using BeatTypeUPtr = std::unique_ptr<BeatType>;
        using BeatTypeSet = std::vector<BeatTypePtr>;
        using BeatTypeSetIter = BeatTypeSet::iterator;
        using BeatTypeSetIterConst = BeatTypeSet::const_iterator;
        inline BeatTypePtr makeBeatType() { return std::make_shared<BeatType>(); }
		inline BeatTypePtr makeBeatType( const XsString& value ) { return std::make_shared<BeatType>( value ); }
		inline BeatTypePtr makeBeatType( XsString&& value ) { return std::make_shared<BeatType>( std::move( value ) ); }

        class BeatType : public ElementInterface
        {
        public:
            BeatType();
            BeatType( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class Fifths;
        using FifthsPtr = std::shared_ptr<Fifths>;
        using FifthsUPtr = std::unique_ptr<Fifths>;
        using FifthsSet = std::vector<FifthsPtr>;
        using FifthsSetIter = FifthsSet::iterator;
        using FifthsSetIterConst = FifthsSet::const_iterator;
        inline FifthsPtr makeFifths() { return std::make_shared<Fifths>(); }
		inline FifthsPtr makeFifths( const FifthsValue& value ) { return std::make_shared<Fifths>( value ); }
		inline FifthsPtr makeFifths( FifthsValue&& value ) { return std::make_shared<Fifths>( std::move( value ) ); }

        class Fifths : public ElementInterface
        {
        public:
            Fifths();
            Fifths( const FifthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FifthsValue getValue() const;
            void setValue( const FifthsValue& value );

        private:
            FifthsValue myValue;
        };
        
        
        class Mode;
        using ModePtr = std::shared_ptr<Mode>;
        using ModeUPtr = std::unique_ptr<Mode>;
        using ModeSet = std::vector<ModePtr>;
        using ModeSetIter = ModeSet::iterator;
        using ModeSetIterConst = ModeSet::const_iterator;
        inline ModePtr makeMode() { return std::make_shared<Mode>(); }
		inline ModePtr makeMode( const ModeValue& value ) { return std::make_shared<Mode>( value ); }
		inline ModePtr makeMode( ModeValue&& value ) { return std::make_shared<Mode>( std::move( value ) ); }

        class Mode : public ElementInterface
        {
        public:
            Mode();
            Mode( const ModeValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ModeValue getValue() const;
            void setValue( const ModeValue& value );

        private:
            ModeValue myValue;
        };
        
        
        class BeatUnit;
        using BeatUnitPtr = std::shared_ptr<BeatUnit>;
        using BeatUnitUPtr = std::unique_ptr<BeatUnit>;
        using BeatUnitSet = std::vector<BeatUnitPtr>;
        using BeatUnitSetIter = BeatUnitSet::iterator;
        using BeatUnitSetIterConst = BeatUnitSet::const_iterator;
        inline BeatUnitPtr makeBeatUnit() { return std::make_shared<BeatUnit>(); }
		inline BeatUnitPtr makeBeatUnit( const NoteTypeValue& value ) { return std::make_shared<BeatUnit>( value ); }
		inline BeatUnitPtr makeBeatUnit( NoteTypeValue&& value ) { return std::make_shared<BeatUnit>( std::move( value ) ); }

        class BeatUnit : public ElementInterface
        {
        public:
            BeatUnit();
            BeatUnit( const NoteTypeValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NoteTypeValue getValue() const;
            void setValue( const NoteTypeValue& value );

        private:
            NoteTypeValue myValue;
        };
        
        
        class TopMargin;
        using TopMarginPtr = std::shared_ptr<TopMargin>;
        using TopMarginUPtr = std::unique_ptr<TopMargin>;
        using TopMarginSet = std::vector<TopMarginPtr>;
        using TopMarginSetIter = TopMarginSet::iterator;
        using TopMarginSetIterConst = TopMarginSet::const_iterator;
        inline TopMarginPtr makeTopMargin() { return std::make_shared<TopMargin>(); }
		inline TopMarginPtr makeTopMargin( const TenthsValue& value ) { return std::make_shared<TopMargin>( value ); }
		inline TopMarginPtr makeTopMargin( TenthsValue&& value ) { return std::make_shared<TopMargin>( std::move( value ) ); }

        class TopMargin : public ElementInterface
        {
        public:
            TopMargin();
            TopMargin( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

        private:
            TenthsValue myValue;
        };
        
        
        class BottomMargin;
        using BottomMarginPtr = std::shared_ptr<BottomMargin>;
        using BottomMarginUPtr = std::unique_ptr<BottomMargin>;
        using BottomMarginSet = std::vector<BottomMarginPtr>;
        using BottomMarginSetIter = BottomMarginSet::iterator;
        using BottomMarginSetIterConst = BottomMarginSet::const_iterator;
        inline BottomMarginPtr makeBottomMargin() { return std::make_shared<BottomMargin>(); }
		inline BottomMarginPtr makeBottomMargin( const TenthsValue& value ) { return std::make_shared<BottomMargin>( value ); }
		inline BottomMarginPtr makeBottomMargin( TenthsValue&& value ) { return std::make_shared<BottomMargin>( std::move( value ) ); }

        class BottomMargin : public ElementInterface
        {
        public:
            BottomMargin();
            BottomMargin( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

        private:
            TenthsValue myValue;
        };
        
        
        class LeftMargin;
        using LeftMarginPtr = std::shared_ptr<LeftMargin>;
        using LeftMarginUPtr = std::unique_ptr<LeftMargin>;
        using LeftMarginSet = std::vector<LeftMarginPtr>;
        using LeftMarginSetIter = LeftMarginSet::iterator;
        using LeftMarginSetIterConst = LeftMarginSet::const_iterator;
        inline LeftMarginPtr makeLeftMargin() { return std::make_shared<LeftMargin>(); }
		inline LeftMarginPtr makeLeftMargin( const TenthsValue& value ) { return std::make_shared<LeftMargin>( value ); }
		inline LeftMarginPtr makeLeftMargin( TenthsValue&& value ) { return std::make_shared<LeftMargin>( std::move( value ) ); }

        class LeftMargin : public ElementInterface
        {
        public:
            LeftMargin();
            LeftMargin( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

        private:
            TenthsValue myValue;
        };
        
        
        class RightMargin;
        using RightMarginPtr = std::shared_ptr<RightMargin>;
        using RightMarginUPtr = std::unique_ptr<RightMargin>;
        using RightMarginSet = std::vector<RightMarginPtr>;
        using RightMarginSetIter = RightMarginSet::iterator;
        using RightMarginSetIterConst = RightMarginSet::const_iterator;
        inline RightMarginPtr makeRightMargin() { return std::make_shared<RightMargin>(); }
		inline RightMarginPtr makeRightMargin( const TenthsValue& value ) { return std::make_shared<RightMargin>( value ); }
		inline RightMarginPtr makeRightMargin( TenthsValue&& value ) { return std::make_shared<RightMargin>( std::move( value ) ); }

        class RightMargin : public ElementInterface
        {
        public:
            RightMargin();
            RightMargin( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

        private:
            TenthsValue myValue;
        };
        
        
        class Duration;
        using DurationPtr = std::shared_ptr<Duration>;
        using DurationUPtr = std::unique_ptr<Duration>;
        using DurationSet = std::vector<DurationPtr>;
        using DurationSetIter = DurationSet::iterator;
        using DurationSetIterConst = DurationSet::const_iterator;
        inline DurationPtr makeDuration() { return std::make_shared<Duration>(); }
		inline DurationPtr makeDuration( const PositiveDivisionsValue& value ) { return std::make_shared<Duration>( value ); }
		inline DurationPtr makeDuration( PositiveDivisionsValue&& value ) { return std::make_shared<Duration>( std::move( value ) ); }

        class Duration : public ElementInterface
        {
        public:
            Duration();
            Duration( const PositiveDivisionsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PositiveDivisionsValue getValue() const;
            void setValue( const PositiveDivisionsValue& value );

        private:
            PositiveDivisionsValue myValue;
        };
        
        
        class DisplayStep;
        using DisplayStepPtr = std::shared_ptr<DisplayStep>;
        using DisplayStepUPtr = std::unique_ptr<DisplayStep>;
        using DisplayStepSet = std::vector<DisplayStepPtr>;
        using DisplayStepSetIter = DisplayStepSet::iterator;
        using DisplayStepSetIterConst = DisplayStepSet::const_iterator;
        inline DisplayStepPtr makeDisplayStep() { return std::make_shared<DisplayStep>(); }
		inline DisplayStepPtr makeDisplayStep( const StepEnum& value ) { return std::make_shared<DisplayStep>( value ); }
		inline DisplayStepPtr makeDisplayStep( StepEnum&& value ) { return std::make_shared<DisplayStep>( std::move( value ) ); }

        class DisplayStep : public ElementInterface
        {
        public:
            DisplayStep();
            DisplayStep( const StepEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StepEnum getValue() const;
            void setValue( const StepEnum& value );

        private:
            StepEnum myValue;
        };
        
        
        class DisplayOctave;
        using DisplayOctavePtr = std::shared_ptr<DisplayOctave>;
        using DisplayOctaveUPtr = std::unique_ptr<DisplayOctave>;
        using DisplayOctaveSet = std::vector<DisplayOctavePtr>;
        using DisplayOctaveSetIter = DisplayOctaveSet::iterator;
        using DisplayOctaveSetIterConst = DisplayOctaveSet::const_iterator;
        inline DisplayOctavePtr makeDisplayOctave() { return std::make_shared<DisplayOctave>(); }
		inline DisplayOctavePtr makeDisplayOctave( const OctaveValue& value ) { return std::make_shared<DisplayOctave>( value ); }
		inline DisplayOctavePtr makeDisplayOctave( OctaveValue&& value ) { return std::make_shared<DisplayOctave>( std::move( value ) ); }

        class DisplayOctave : public ElementInterface
        {
        public:
            DisplayOctave();
            DisplayOctave( const OctaveValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OctaveValue getValue() const;
            void setValue( const OctaveValue& value );

        private:
            OctaveValue myValue;
        };
        
        
        class MovementNumber;
        using MovementNumberPtr = std::shared_ptr<MovementNumber>;
        using MovementNumberUPtr = std::unique_ptr<MovementNumber>;
        using MovementNumberSet = std::vector<MovementNumberPtr>;
        using MovementNumberSetIter = MovementNumberSet::iterator;
        using MovementNumberSetIterConst = MovementNumberSet::const_iterator;
        inline MovementNumberPtr makeMovementNumber() { return std::make_shared<MovementNumber>(); }
		inline MovementNumberPtr makeMovementNumber( const XsString& value ) { return std::make_shared<MovementNumber>( value ); }
		inline MovementNumberPtr makeMovementNumber( XsString&& value ) { return std::make_shared<MovementNumber>( std::move( value ) ); }

        class MovementNumber : public ElementInterface
        {
        public:
            MovementNumber();
            MovementNumber( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class MovementTitle;
        using MovementTitlePtr = std::shared_ptr<MovementTitle>;
        using MovementTitleUPtr = std::unique_ptr<MovementTitle>;
        using MovementTitleSet = std::vector<MovementTitlePtr>;
        using MovementTitleSetIter = MovementTitleSet::iterator;
        using MovementTitleSetIterConst = MovementTitleSet::const_iterator;
        inline MovementTitlePtr makeMovementTitle() { return std::make_shared<MovementTitle>(); }
		inline MovementTitlePtr makeMovementTitle( const XsString& value ) { return std::make_shared<MovementTitle>( value ); }
		inline MovementTitlePtr makeMovementTitle( XsString&& value ) { return std::make_shared<MovementTitle>( std::move( value ) ); }

        class MovementTitle : public ElementInterface
        {
        public:
            MovementTitle();
            MovementTitle( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            XsString myValue;
        };
        
        
        class Double;
        using DoublePtr = std::shared_ptr<Double>;
        using DoubleUPtr = std::unique_ptr<Double>;
        using DoubleSet = std::vector<DoublePtr>;
        using DoubleSetIter = DoubleSet::iterator;
        using DoubleSetIterConst = DoubleSet::const_iterator;
        inline DoublePtr makeDouble() { return std::make_shared<Double>(); }

        class Double : public ElementInterface
        {
        public:
            Double();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class AccordionHigh;
        using AccordionHighPtr = std::shared_ptr<AccordionHigh>;
        using AccordionHighUPtr = std::unique_ptr<AccordionHigh>;
        using AccordionHighSet = std::vector<AccordionHighPtr>;
        using AccordionHighSetIter = AccordionHighSet::iterator;
        using AccordionHighSetIterConst = AccordionHighSet::const_iterator;
        inline AccordionHighPtr makeAccordionHigh() { return std::make_shared<AccordionHigh>(); }

        class AccordionHigh : public ElementInterface
        {
        public:
            AccordionHigh();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class AccordionLow;
        using AccordionLowPtr = std::shared_ptr<AccordionLow>;
        using AccordionLowUPtr = std::unique_ptr<AccordionLow>;
        using AccordionLowSet = std::vector<AccordionLowPtr>;
        using AccordionLowSetIter = AccordionLowSet::iterator;
        using AccordionLowSetIterConst = AccordionLowSet::const_iterator;
        inline AccordionLowPtr makeAccordionLow() { return std::make_shared<AccordionLow>(); }

        class AccordionLow : public ElementInterface
        {
        public:
            AccordionLow();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class MetronomeDot;
        using MetronomeDotPtr = std::shared_ptr<MetronomeDot>;
        using MetronomeDotUPtr = std::unique_ptr<MetronomeDot>;
        using MetronomeDotSet = std::vector<MetronomeDotPtr>;
        using MetronomeDotSetIter = MetronomeDotSet::iterator;
        using MetronomeDotSetIterConst = MetronomeDotSet::const_iterator;
        inline MetronomeDotPtr makeMetronomeDot() { return std::make_shared<MetronomeDot>(); }

        class MetronomeDot : public ElementInterface
        {
        public:
            MetronomeDot();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class Timpani;
        using TimpaniPtr = std::shared_ptr<Timpani>;
        using TimpaniUPtr = std::unique_ptr<Timpani>;
        using TimpaniSet = std::vector<TimpaniPtr>;
        using TimpaniSetIter = TimpaniSet::iterator;
        using TimpaniSetIterConst = TimpaniSet::const_iterator;
        inline TimpaniPtr makeTimpani() { return std::make_shared<Timpani>(); }

        class Timpani : public ElementInterface
        {
        public:
            Timpani();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class PreBend;
        using PreBendPtr = std::shared_ptr<PreBend>;
        using PreBendUPtr = std::unique_ptr<PreBend>;
        using PreBendSet = std::vector<PreBendPtr>;
        using PreBendSetIter = PreBendSet::iterator;
        using PreBendSetIterConst = PreBendSet::const_iterator;
        inline PreBendPtr makePreBend() { return std::make_shared<PreBend>(); }

        class PreBend : public ElementInterface
        {
        public:
            PreBend();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class Release;
        using ReleasePtr = std::shared_ptr<Release>;
        using ReleaseUPtr = std::unique_ptr<Release>;
        using ReleaseSet = std::vector<ReleasePtr>;
        using ReleaseSetIter = ReleaseSet::iterator;
        using ReleaseSetIterConst = ReleaseSet::const_iterator;
        inline ReleasePtr makeRelease() { return std::make_shared<Release>(); }

        class Release : public ElementInterface
        {
        public:
            Release();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class Natural;
        using NaturalPtr = std::shared_ptr<Natural>;
        using NaturalUPtr = std::unique_ptr<Natural>;
        using NaturalSet = std::vector<NaturalPtr>;
        using NaturalSetIter = NaturalSet::iterator;
        using NaturalSetIterConst = NaturalSet::const_iterator;
        inline NaturalPtr makeNatural() { return std::make_shared<Natural>(); }

        class Natural : public ElementInterface
        {
        public:
            Natural();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class Artificial;
        using ArtificialPtr = std::shared_ptr<Artificial>;
        using ArtificialUPtr = std::unique_ptr<Artificial>;
        using ArtificialSet = std::vector<ArtificialPtr>;
        using ArtificialSetIter = ArtificialSet::iterator;
        using ArtificialSetIterConst = ArtificialSet::const_iterator;
        inline ArtificialPtr makeArtificial() { return std::make_shared<Artificial>(); }

        class Artificial : public ElementInterface
        {
        public:
            Artificial();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class BasePitch;
        using BasePitchPtr = std::shared_ptr<BasePitch>;
        using BasePitchUPtr = std::unique_ptr<BasePitch>;
        using BasePitchSet = std::vector<BasePitchPtr>;
        using BasePitchSetIter = BasePitchSet::iterator;
        using BasePitchSetIterConst = BasePitchSet::const_iterator;
        inline BasePitchPtr makeBasePitch() { return std::make_shared<BasePitch>(); }

        class BasePitch : public ElementInterface
        {
        public:
            BasePitch();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class TouchingPitch;
        using TouchingPitchPtr = std::shared_ptr<TouchingPitch>;
        using TouchingPitchUPtr = std::unique_ptr<TouchingPitch>;
        using TouchingPitchSet = std::vector<TouchingPitchPtr>;
        using TouchingPitchSetIter = TouchingPitchSet::iterator;
        using TouchingPitchSetIterConst = TouchingPitchSet::const_iterator;
        inline TouchingPitchPtr makeTouchingPitch() { return std::make_shared<TouchingPitch>(); }

        class TouchingPitch : public ElementInterface
        {
        public:
            TouchingPitch();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class SoundingPitch;
        using SoundingPitchPtr = std::shared_ptr<SoundingPitch>;
        using SoundingPitchUPtr = std::unique_ptr<SoundingPitch>;
        using SoundingPitchSet = std::vector<SoundingPitchPtr>;
        using SoundingPitchSetIter = SoundingPitchSet::iterator;
        using SoundingPitchSetIterConst = SoundingPitchSet::const_iterator;
        inline SoundingPitchPtr makeSoundingPitch() { return std::make_shared<SoundingPitch>(); }

        class SoundingPitch : public ElementInterface
        {
        public:
            SoundingPitch();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class Laughing;
        using LaughingPtr = std::shared_ptr<Laughing>;
        using LaughingUPtr = std::unique_ptr<Laughing>;
        using LaughingSet = std::vector<LaughingPtr>;
        using LaughingSetIter = LaughingSet::iterator;
        using LaughingSetIterConst = LaughingSet::const_iterator;
        inline LaughingPtr makeLaughing() { return std::make_shared<Laughing>(); }

        class Laughing : public ElementInterface
        {
        public:
            Laughing();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class Humming;
        using HummingPtr = std::shared_ptr<Humming>;
        using HummingUPtr = std::unique_ptr<Humming>;
        using HummingSet = std::vector<HummingPtr>;
        using HummingSetIter = HummingSet::iterator;
        using HummingSetIterConst = HummingSet::const_iterator;
        inline HummingPtr makeHumming() { return std::make_shared<Humming>(); }

        class Humming : public ElementInterface
        {
        public:
            Humming();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class EndLine;
        using EndLinePtr = std::shared_ptr<EndLine>;
        using EndLineUPtr = std::unique_ptr<EndLine>;
        using EndLineSet = std::vector<EndLinePtr>;
        using EndLineSetIter = EndLineSet::iterator;
        using EndLineSetIterConst = EndLineSet::const_iterator;
        inline EndLinePtr makeEndLine() { return std::make_shared<EndLine>(); }

        class EndLine : public ElementInterface
        {
        public:
            EndLine();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class EndParagraph;
        using EndParagraphPtr = std::shared_ptr<EndParagraph>;
        using EndParagraphUPtr = std::unique_ptr<EndParagraph>;
        using EndParagraphSet = std::vector<EndParagraphPtr>;
        using EndParagraphSetIter = EndParagraphSet::iterator;
        using EndParagraphSetIterConst = EndParagraphSet::const_iterator;
        inline EndParagraphPtr makeEndParagraph() { return std::make_shared<EndParagraph>(); }

        class EndParagraph : public ElementInterface
        {
        public:
            EndParagraph();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class Cue;
        using CuePtr = std::shared_ptr<Cue>;
        using CueUPtr = std::unique_ptr<Cue>;
        using CueSet = std::vector<CuePtr>;
        using CueSetIter = CueSet::iterator;
        using CueSetIterConst = CueSet::const_iterator;
        inline CuePtr makeCue() { return std::make_shared<Cue>(); }

        class Cue : public ElementInterface
        {
        public:
            Cue();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class NormalDot;
        using NormalDotPtr = std::shared_ptr<NormalDot>;
        using NormalDotUPtr = std::unique_ptr<NormalDot>;
        using NormalDotSet = std::vector<NormalDotPtr>;
        using NormalDotSetIter = NormalDotSet::iterator;
        using NormalDotSetIterConst = NormalDotSet::const_iterator;
        inline NormalDotPtr makeNormalDot() { return std::make_shared<NormalDot>(); }

        class NormalDot : public ElementInterface
        {
        public:
            NormalDot();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class GroupTime;
        using GroupTimePtr = std::shared_ptr<GroupTime>;
        using GroupTimeUPtr = std::unique_ptr<GroupTime>;
        using GroupTimeSet = std::vector<GroupTimePtr>;
        using GroupTimeSetIter = GroupTimeSet::iterator;
        using GroupTimeSetIterConst = GroupTimeSet::const_iterator;
        inline GroupTimePtr makeGroupTime() { return std::make_shared<GroupTime>(); }

        class GroupTime : public ElementInterface
        {
        public:
            GroupTime();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class Solo;
        using SoloPtr = std::shared_ptr<Solo>;
        using SoloUPtr = std::unique_ptr<Solo>;
        using SoloSet = std::vector<SoloPtr>;
        using SoloSetIter = SoloSet::iterator;
        using SoloSetIterConst = SoloSet::const_iterator;
        inline SoloPtr makeSolo() { return std::make_shared<Solo>(); }

        class Solo : public ElementInterface
        {
        public:
            Solo();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class SlashDot;
        using SlashDotPtr = std::shared_ptr<SlashDot>;
        using SlashDotUPtr = std::unique_ptr<SlashDot>;
        using SlashDotSet = std::vector<SlashDotPtr>;
        using SlashDotSetIter = SlashDotSet::iterator;
        using SlashDotSetIterConst = SlashDotSet::const_iterator;
        inline SlashDotPtr makeSlashDot() { return std::make_shared<SlashDot>(); }

        class SlashDot : public ElementInterface
        {
        public:
            SlashDot();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class BeatUnitDot;
        using BeatUnitDotPtr = std::shared_ptr<BeatUnitDot>;
        using BeatUnitDotUPtr = std::unique_ptr<BeatUnitDot>;
        using BeatUnitDotSet = std::vector<BeatUnitDotPtr>;
        using BeatUnitDotSetIter = BeatUnitDotSet::iterator;
        using BeatUnitDotSetIterConst = BeatUnitDotSet::const_iterator;
        inline BeatUnitDotPtr makeBeatUnitDot() { return std::make_shared<BeatUnitDot>(); }

        class BeatUnitDot : public ElementInterface
        {
        public:
            BeatUnitDot();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class Chord;
        using ChordPtr = std::shared_ptr<Chord>;
        using ChordUPtr = std::unique_ptr<Chord>;
        using ChordSet = std::vector<ChordPtr>;
        using ChordSetIter = ChordSet::iterator;
        using ChordSetIterConst = ChordSet::const_iterator;
        inline ChordPtr makeChord() { return std::make_shared<Chord>(); }

        class Chord : public ElementInterface
        {
        public:
            Chord();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
        };
        
        class Segno;
        using SegnoPtr = std::shared_ptr<Segno>;
        using SegnoUPtr = std::unique_ptr<Segno>;
        using SegnoSet = std::vector<SegnoPtr>;
        using SegnoSetIter = SegnoSet::iterator;
        using SegnoSetIterConst = SegnoSet::const_iterator;
        inline SegnoPtr makeSegno() { return std::make_shared<Segno>(); }

        class Segno : public ElementInterface
        {
        public:
            Segno();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPrintObjectStyleAlignAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPrintObjectStyleAlignAttributesPtr& attributes );

        private:
            EmptyPrintObjectStyleAlignAttributesPtr myAttributes;
        };
        
        class Coda;
        using CodaPtr = std::shared_ptr<Coda>;
        using CodaUPtr = std::unique_ptr<Coda>;
        using CodaSet = std::vector<CodaPtr>;
        using CodaSetIter = CodaSet::iterator;
        using CodaSetIterConst = CodaSet::const_iterator;
        inline CodaPtr makeCoda() { return std::make_shared<Coda>(); }

        class Coda : public ElementInterface
        {
        public:
            Coda();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPrintObjectStyleAlignAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPrintObjectStyleAlignAttributesPtr& attributes );

        private:
            EmptyPrintObjectStyleAlignAttributesPtr myAttributes;
        };

        class Damp;
        using DampPtr = std::shared_ptr<Damp>;
        using DampUPtr = std::unique_ptr<Damp>;
        using DampSet = std::vector<DampPtr>;
        using DampSetIter = DampSet::iterator;
        using DampSetIterConst = DampSet::const_iterator;
        inline DampPtr makeDamp() { return std::make_shared<Damp>(); }

        class Damp : public ElementInterface
        {
        public:
            Damp();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPrintObjectStyleAlignAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPrintObjectStyleAlignAttributesPtr& attributes );

        private:
            EmptyPrintObjectStyleAlignAttributesPtr myAttributes;
        };
        

        class DampAll;
        using DampAllPtr = std::shared_ptr<DampAll>;
        using DampAllUPtr = std::unique_ptr<DampAll>;
        using DampAllSet = std::vector<DampAllPtr>;
        using DampAllSetIter = DampAllSet::iterator;
        using DampAllSetIterConst = DampAllSet::const_iterator;
        inline DampAllPtr makeDampAll() { return std::make_shared<DampAll>(); }

        class DampAll : public ElementInterface
        {
        public:
            DampAll();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPrintObjectStyleAlignAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPrintObjectStyleAlignAttributesPtr& attributes );

        private:
            EmptyPrintObjectStyleAlignAttributesPtr myAttributes;
        };
        
        class Eyeglasses;
        using EyeglassesPtr = std::shared_ptr<Eyeglasses>;
        using EyeglassesUPtr = std::unique_ptr<Eyeglasses>;
        using EyeglassesSet = std::vector<EyeglassesPtr>;
        using EyeglassesSetIter = EyeglassesSet::iterator;
        using EyeglassesSetIterConst = EyeglassesSet::const_iterator;
        inline EyeglassesPtr makeEyeglasses() { return std::make_shared<Eyeglasses>(); }

        class Eyeglasses : public ElementInterface
        {
        public:
            Eyeglasses();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPrintObjectStyleAlignAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPrintObjectStyleAlignAttributesPtr& attributes );

        private:
            EmptyPrintObjectStyleAlignAttributesPtr myAttributes;
        };
        
        class LeftDivider;
        using LeftDividerPtr = std::shared_ptr<LeftDivider>;
        using LeftDividerUPtr = std::unique_ptr<LeftDivider>;
        using LeftDividerSet = std::vector<LeftDividerPtr>;
        using LeftDividerSetIter = LeftDividerSet::iterator;
        using LeftDividerSetIterConst = LeftDividerSet::const_iterator;
        inline LeftDividerPtr makeLeftDivider() { return std::make_shared<LeftDivider>(); }

        class LeftDivider : public ElementInterface
        {
        public:
            LeftDivider();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPrintObjectStyleAlignAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPrintObjectStyleAlignAttributesPtr& attributes );

        private:
            EmptyPrintObjectStyleAlignAttributesPtr myAttributes;
        };
        
        class RightDivider;
        using RightDividerPtr = std::shared_ptr<RightDivider>;
        using RightDividerUPtr = std::unique_ptr<RightDivider>;
        using RightDividerSet = std::vector<RightDividerPtr>;
        using RightDividerSetIter = RightDividerSet::iterator;
        using RightDividerSetIterConst = RightDividerSet::const_iterator;
        inline RightDividerPtr makeRightDivider() { return std::make_shared<RightDivider>(); }

        class RightDivider : public ElementInterface
        {
        public:
            RightDivider();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPrintObjectStyleAlignAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPrintObjectStyleAlignAttributesPtr& attributes );

        private:
            EmptyPrintObjectStyleAlignAttributesPtr myAttributes;
        };
        
        struct EmptyPlacementAttributes;
        using EmptyPlacementAttributesPtr = std::shared_ptr<EmptyPlacementAttributes>;
        
        struct EmptyPlacementAttributes : public AttributesInterface
        {
        public:
            EmptyPlacementAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class Accent;
        using AccentPtr = std::shared_ptr<Accent>;
        using AccentUPtr = std::unique_ptr<Accent>;
        using AccentSet = std::vector<AccentPtr>;
        using AccentSetIter = AccentSet::iterator;
        using AccentSetIterConst = AccentSet::const_iterator;
        inline AccentPtr makeAccent() { return std::make_shared<Accent>(); }

        class Accent : public ElementInterface
        {
        public:
            Accent();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class Staccato;
        using StaccatoPtr = std::shared_ptr<Staccato>;
        using StaccatoUPtr = std::unique_ptr<Staccato>;
        using StaccatoSet = std::vector<StaccatoPtr>;
        using StaccatoSetIter = StaccatoSet::iterator;
        using StaccatoSetIterConst = StaccatoSet::const_iterator;
        inline StaccatoPtr makeStaccato() { return std::make_shared<Staccato>(); }

        class Staccato : public ElementInterface
        {
        public:
            Staccato();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class Tenuto;
        using TenutoPtr = std::shared_ptr<Tenuto>;
        using TenutoUPtr = std::unique_ptr<Tenuto>;
        using TenutoSet = std::vector<TenutoPtr>;
        using TenutoSetIter = TenutoSet::iterator;
        using TenutoSetIterConst = TenutoSet::const_iterator;
        inline TenutoPtr makeTenuto() { return std::make_shared<Tenuto>(); }

        class Tenuto : public ElementInterface
        {
        public:
            Tenuto();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class DetachedLegato;
        using DetachedLegatoPtr = std::shared_ptr<DetachedLegato>;
        using DetachedLegatoUPtr = std::unique_ptr<DetachedLegato>;
        using DetachedLegatoSet = std::vector<DetachedLegatoPtr>;
        using DetachedLegatoSetIter = DetachedLegatoSet::iterator;
        using DetachedLegatoSetIterConst = DetachedLegatoSet::const_iterator;
        inline DetachedLegatoPtr makeDetachedLegato() { return std::make_shared<DetachedLegato>(); }

        class DetachedLegato : public ElementInterface
        {
        public:
            DetachedLegato();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class Staccatissimo;
        using StaccatissimoPtr = std::shared_ptr<Staccatissimo>;
        using StaccatissimoUPtr = std::unique_ptr<Staccatissimo>;
        using StaccatissimoSet = std::vector<StaccatissimoPtr>;
        using StaccatissimoSetIter = StaccatissimoSet::iterator;
        using StaccatissimoSetIterConst = StaccatissimoSet::const_iterator;
        inline StaccatissimoPtr makeStaccatissimo() { return std::make_shared<Staccatissimo>(); }

        class Staccatissimo : public ElementInterface
        {
        public:
            Staccatissimo();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class Spiccato;
        using SpiccatoPtr = std::shared_ptr<Spiccato>;
        using SpiccatoUPtr = std::unique_ptr<Spiccato>;
        using SpiccatoSet = std::vector<SpiccatoPtr>;
        using SpiccatoSetIter = SpiccatoSet::iterator;
        using SpiccatoSetIterConst = SpiccatoSet::const_iterator;
        inline SpiccatoPtr makeSpiccato() { return std::make_shared<Spiccato>(); }

        class Spiccato : public ElementInterface
        {
        public:
            Spiccato();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        struct EmptyLineAttributes;
        using EmptyLineAttributesPtr = std::shared_ptr<EmptyLineAttributes>;
        
        struct EmptyLineAttributes : public AttributesInterface
        {
        public:
            EmptyLineAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue dashLength;
            TenthsValue spaceLength;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            bool hasDashLength;
            bool hasSpaceLength;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
        };
        
        class Scoop;
        using ScoopPtr = std::shared_ptr<Scoop>;
        using ScoopUPtr = std::unique_ptr<Scoop>;
        using ScoopSet = std::vector<ScoopPtr>;
        using ScoopSetIter = ScoopSet::iterator;
        using ScoopSetIterConst = ScoopSet::const_iterator;
        inline ScoopPtr makeScoop() { return std::make_shared<Scoop>(); }

        class Scoop : public ElementInterface
        {
        public:
            Scoop();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyLineAttributesPtr getAttributes() const;
            void setAttributes( const EmptyLineAttributesPtr& attributes );

        private:
            EmptyLineAttributesPtr myAttributes;
        };
        
        class Plop;
        using PlopPtr = std::shared_ptr<Plop>;
        using PlopUPtr = std::unique_ptr<Plop>;
        using PlopSet = std::vector<PlopPtr>;
        using PlopSetIter = PlopSet::iterator;
        using PlopSetIterConst = PlopSet::const_iterator;
        inline PlopPtr makePlop() { return std::make_shared<Plop>(); }

        class Plop : public ElementInterface
        {
        public:
            Plop();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyLineAttributesPtr getAttributes() const;
            void setAttributes( const EmptyLineAttributesPtr& attributes );

        private:
            EmptyLineAttributesPtr myAttributes;
        };
        
        class Doit;
        using DoitPtr = std::shared_ptr<Doit>;
        using DoitUPtr = std::unique_ptr<Doit>;
        using DoitSet = std::vector<DoitPtr>;
        using DoitSetIter = DoitSet::iterator;
        using DoitSetIterConst = DoitSet::const_iterator;
        inline DoitPtr makeDoit() { return std::make_shared<Doit>(); }

        class Doit : public ElementInterface
        {
        public:
            Doit();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyLineAttributesPtr getAttributes() const;
            void setAttributes( const EmptyLineAttributesPtr& attributes );

        private:
            EmptyLineAttributesPtr myAttributes;
        };
        
        class Falloff;
        using FalloffPtr = std::shared_ptr<Falloff>;
        using FalloffUPtr = std::unique_ptr<Falloff>;
        using FalloffSet = std::vector<FalloffPtr>;
        using FalloffSetIter = FalloffSet::iterator;
        using FalloffSetIterConst = FalloffSet::const_iterator;
        inline FalloffPtr makeFalloff() { return std::make_shared<Falloff>(); }

        class Falloff : public ElementInterface
        {
        public:
            Falloff();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyLineAttributesPtr getAttributes() const;
            void setAttributes( const EmptyLineAttributesPtr& attributes );

        private:
            EmptyLineAttributesPtr myAttributes;
        };
        
        class Caesura;
        using CaesuraPtr = std::shared_ptr<Caesura>;
        using CaesuraUPtr = std::unique_ptr<Caesura>;
        using CaesuraSet = std::vector<CaesuraPtr>;
        using CaesuraSetIter = CaesuraSet::iterator;
        using CaesuraSetIterConst = CaesuraSet::const_iterator;
        inline CaesuraPtr makeCaesura() { return std::make_shared<Caesura>(); }

        class Caesura : public ElementInterface
        {
        public:
            Caesura();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class Stress;
        using StressPtr = std::shared_ptr<Stress>;
        using StressUPtr = std::unique_ptr<Stress>;
        using StressSet = std::vector<StressPtr>;
        using StressSetIter = StressSet::iterator;
        using StressSetIterConst = StressSet::const_iterator;
        inline StressPtr makeStress() { return std::make_shared<Stress>(); }

        class Stress : public ElementInterface
        {
        public:
            Stress();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class Unstress;
        using UnstressPtr = std::shared_ptr<Unstress>;
        using UnstressUPtr = std::unique_ptr<Unstress>;
        using UnstressSet = std::vector<UnstressPtr>;
        using UnstressSetIter = UnstressSet::iterator;
        using UnstressSetIterConst = UnstressSet::const_iterator;
        inline UnstressPtr makeUnstress() { return std::make_shared<Unstress>(); }

        class Unstress : public ElementInterface
        {
        public:
            Unstress();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class Dot;
        using DotPtr = std::shared_ptr<Dot>;
        using DotUPtr = std::unique_ptr<Dot>;
        using DotSet = std::vector<DotPtr>;
        using DotSetIter = DotSet::iterator;
        using DotSetIterConst = DotSet::const_iterator;
        inline DotPtr makeDot() { return std::make_shared<Dot>(); }

        class Dot : public ElementInterface
        {
        public:
            Dot();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        struct EmptyTrillSoundAttributes;
        using EmptyTrillSoundAttributesPtr = std::shared_ptr<EmptyTrillSoundAttributes>;
        
        struct EmptyTrillSoundAttributes : public AttributesInterface
        {
        public:
            EmptyTrillSoundAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            StartNote startNote;
            TrillStep trillStep;
            TwoNoteTurn twoNoteTurn;
            YesNo accelerate;
            TrillBeats beats;
            Percent secondBeat;
            Percent lastBeat;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
            bool hasStartNote;
            bool hasTrillStep;
            bool hasTwoNoteTurn;
            bool hasAccelerate;
            bool hasBeats;
            bool hasSecondBeat;
            bool hasLastBeat;
        };
        
        class TrillMark;
        using TrillMarkPtr = std::shared_ptr<TrillMark>;
        using TrillMarkUPtr = std::unique_ptr<TrillMark>;
        using TrillMarkSet = std::vector<TrillMarkPtr>;
        using TrillMarkSetIter = TrillMarkSet::iterator;
        using TrillMarkSetIterConst = TrillMarkSet::const_iterator;
        inline TrillMarkPtr makeTrillMark() { return std::make_shared<TrillMark>(); }

        class TrillMark : public ElementInterface
        {
        public:
            TrillMark();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyTrillSoundAttributesPtr getAttributes() const;
            void setAttributes( const EmptyTrillSoundAttributesPtr& attributes );

        private:
            EmptyTrillSoundAttributesPtr myAttributes;
        };
        
        class VerticalTurn;
        using VerticalTurnPtr = std::shared_ptr<VerticalTurn>;
        using VerticalTurnUPtr = std::unique_ptr<VerticalTurn>;
        using VerticalTurnSet = std::vector<VerticalTurnPtr>;
        using VerticalTurnSetIter = VerticalTurnSet::iterator;
        using VerticalTurnSetIterConst = VerticalTurnSet::const_iterator;
        inline VerticalTurnPtr makeVerticalTurn() { return std::make_shared<VerticalTurn>(); }

        class VerticalTurn : public ElementInterface
        {
        public:
            VerticalTurn();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyTrillSoundAttributesPtr getAttributes() const;
            void setAttributes( const EmptyTrillSoundAttributesPtr& attributes );

        private:
            EmptyTrillSoundAttributesPtr myAttributes;
        };
        
        class Shake;
        using ShakePtr = std::shared_ptr<Shake>;
        using ShakeUPtr = std::unique_ptr<Shake>;
        using ShakeSet = std::vector<ShakePtr>;
        using ShakeSetIter = ShakeSet::iterator;
        using ShakeSetIterConst = ShakeSet::const_iterator;
        inline ShakePtr makeShake() { return std::make_shared<Shake>(); }

        class Shake : public ElementInterface
        {
        public:
            Shake();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyTrillSoundAttributesPtr getAttributes() const;
            void setAttributes( const EmptyTrillSoundAttributesPtr& attributes );

        private:
            EmptyTrillSoundAttributesPtr myAttributes;
        };
        
        class Schleifer;
        using SchleiferPtr = std::shared_ptr<Schleifer>;
        using SchleiferUPtr = std::unique_ptr<Schleifer>;
        using SchleiferSet = std::vector<SchleiferPtr>;
        using SchleiferSetIter = SchleiferSet::iterator;
        using SchleiferSetIterConst = SchleiferSet::const_iterator;
        inline SchleiferPtr makeSchleifer() { return std::make_shared<Schleifer>(); }

        class Schleifer : public ElementInterface
        {
        public:
            Schleifer();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class UpBow;
        using UpBowPtr = std::shared_ptr<UpBow>;
        using UpBowUPtr = std::unique_ptr<UpBow>;
        using UpBowSet = std::vector<UpBowPtr>;
        using UpBowSetIter = UpBowSet::iterator;
        using UpBowSetIterConst = UpBowSet::const_iterator;
        inline UpBowPtr makeUpBow() { return std::make_shared<UpBow>(); }

        class UpBow : public ElementInterface
        {
        public:
            UpBow();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class DownBow;
        using DownBowPtr = std::shared_ptr<DownBow>;
        using DownBowUPtr = std::unique_ptr<DownBow>;
        using DownBowSet = std::vector<DownBowPtr>;
        using DownBowSetIter = DownBowSet::iterator;
        using DownBowSetIterConst = DownBowSet::const_iterator;
        inline DownBowPtr makeDownBow() { return std::make_shared<DownBow>(); }

        class DownBow : public ElementInterface
        {
        public:
            DownBow();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class OpenString;
        using OpenStringPtr = std::shared_ptr<OpenString>;
        using OpenStringUPtr = std::unique_ptr<OpenString>;
        using OpenStringSet = std::vector<OpenStringPtr>;
        using OpenStringSetIter = OpenStringSet::iterator;
        using OpenStringSetIterConst = OpenStringSet::const_iterator;
        inline OpenStringPtr makeOpenString() { return std::make_shared<OpenString>(); }

        class OpenString : public ElementInterface
        {
        public:
            OpenString();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class ThumbPosition;
        using ThumbPositionPtr = std::shared_ptr<ThumbPosition>;
        using ThumbPositionUPtr = std::unique_ptr<ThumbPosition>;
        using ThumbPositionSet = std::vector<ThumbPositionPtr>;
        using ThumbPositionSetIter = ThumbPositionSet::iterator;
        using ThumbPositionSetIterConst = ThumbPositionSet::const_iterator;
        inline ThumbPositionPtr makeThumbPosition() { return std::make_shared<ThumbPosition>(); }

        class ThumbPosition : public ElementInterface
        {
        public:
            ThumbPosition();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class DoubleTongue;
        using DoubleTonguePtr = std::shared_ptr<DoubleTongue>;
        using DoubleTongueUPtr = std::unique_ptr<DoubleTongue>;
        using DoubleTongueSet = std::vector<DoubleTonguePtr>;
        using DoubleTongueSetIter = DoubleTongueSet::iterator;
        using DoubleTongueSetIterConst = DoubleTongueSet::const_iterator;
        inline DoubleTonguePtr makeDoubleTongue() { return std::make_shared<DoubleTongue>(); }

        class DoubleTongue : public ElementInterface
        {
        public:
            DoubleTongue();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class TripleTongue;
        using TripleTonguePtr = std::shared_ptr<TripleTongue>;
        using TripleTongueUPtr = std::unique_ptr<TripleTongue>;
        using TripleTongueSet = std::vector<TripleTonguePtr>;
        using TripleTongueSetIter = TripleTongueSet::iterator;
        using TripleTongueSetIterConst = TripleTongueSet::const_iterator;
        inline TripleTonguePtr makeTripleTongue() { return std::make_shared<TripleTongue>(); }

        class TripleTongue : public ElementInterface
        {
        public:
            TripleTongue();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class Stopped;
        using StoppedPtr = std::shared_ptr<Stopped>;
        using StoppedUPtr = std::unique_ptr<Stopped>;
        using StoppedSet = std::vector<StoppedPtr>;
        using StoppedSetIter = StoppedSet::iterator;
        using StoppedSetIterConst = StoppedSet::const_iterator;
        inline StoppedPtr makeStopped() { return std::make_shared<Stopped>(); }

        class Stopped : public ElementInterface
        {
        public:
            Stopped();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class SnapPizzicato;
        using SnapPizzicatoPtr = std::shared_ptr<SnapPizzicato>;
        using SnapPizzicatoUPtr = std::unique_ptr<SnapPizzicato>;
        using SnapPizzicatoSet = std::vector<SnapPizzicatoPtr>;
        using SnapPizzicatoSetIter = SnapPizzicatoSet::iterator;
        using SnapPizzicatoSetIterConst = SnapPizzicatoSet::const_iterator;
        inline SnapPizzicatoPtr makeSnapPizzicato() { return std::make_shared<SnapPizzicato>(); }

        class SnapPizzicato : public ElementInterface
        {
        public:
            SnapPizzicato();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        class Fingernails;
        using FingernailsPtr = std::shared_ptr<Fingernails>;
        using FingernailsUPtr = std::unique_ptr<Fingernails>;
        using FingernailsSet = std::vector<FingernailsPtr>;
        using FingernailsSetIter = FingernailsSet::iterator;
        using FingernailsSetIterConst = FingernailsSet::const_iterator;
        inline FingernailsPtr makeFingernails() { return std::make_shared<Fingernails>(); }

        class Fingernails : public ElementInterface
        {
        public:
            Fingernails();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
        
        struct EmptyFontAttributes;
        using EmptyFontAttributesPtr = std::shared_ptr<EmptyFontAttributes>;
        
        struct EmptyFontAttributes : public AttributesInterface
        {
        public:
            EmptyFontAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class MusicFont;
        using MusicFontPtr = std::shared_ptr<MusicFont>;
        using MusicFontUPtr = std::unique_ptr<MusicFont>;
        using MusicFontSet = std::vector<MusicFontPtr>;
        using MusicFontSetIter = MusicFontSet::iterator;
        using MusicFontSetIterConst = MusicFontSet::const_iterator;
        inline MusicFontPtr makeMusicFont() { return std::make_shared<MusicFont>(); }

        class MusicFont : public ElementInterface
        {
        public:
            MusicFont();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyFontAttributesPtr getAttributes() const;
            void setAttributes( const EmptyFontAttributesPtr& attributes );

        private:
            EmptyFontAttributesPtr myAttributes;
        };
        
        class WordFont;
        using WordFontPtr = std::shared_ptr<WordFont>;
        using WordFontUPtr = std::unique_ptr<WordFont>;
        using WordFontSet = std::vector<WordFontPtr>;
        using WordFontSetIter = WordFontSet::iterator;
        using WordFontSetIterConst = WordFontSet::const_iterator;
        inline WordFontPtr makeWordFont() { return std::make_shared<WordFont>(); }

        class WordFont : public ElementInterface
        {
        public:
            WordFont();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyFontAttributesPtr getAttributes() const;
            void setAttributes( const EmptyFontAttributesPtr& attributes );

        private:
            EmptyFontAttributesPtr myAttributes;
        };
        
        
        struct WavyLineAttributes;
        using WavyLineAttributesPtr = std::shared_ptr<WavyLineAttributes>;
        
        struct WavyLineAttributes : public AttributesInterface
        {
        public:
            WavyLineAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStopContinue type;
            NumberLevel number;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            AboveBelow placement;
            StartNote startNote;
            TrillStep trillStep;
            TwoNoteTurn twoNoteTurn;
            YesNo accelerate;
            TrillBeats beats;
            Percent secondBeat;
            Percent lastBeat;
            const bool hasType;
            bool hasNumber;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasPlacement;
            bool hasStartNote;
            bool hasTrillStep;
            bool hasTwoNoteTurn;
            bool hasAccelerate;
            bool hasBeats;
            bool hasSecondBeat;
            bool hasLastBeat;
        };
        
        class WavyLine;
        using WavyLinePtr = std::shared_ptr<WavyLine>;
        using WavyLineUPtr = std::unique_ptr<WavyLine>;
        using WavyLineSet = std::vector<WavyLinePtr>;
        using WavyLineSetIter = WavyLineSet::iterator;
        using WavyLineSetIterConst = WavyLineSet::const_iterator;
        inline WavyLinePtr makeWavyLine() { return std::make_shared<WavyLine>(); }

        class WavyLine : public ElementInterface
        {
        public:
            WavyLine();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            WavyLineAttributesPtr getAttributes() const;
            void setAttributes( const WavyLineAttributesPtr& attributes );

        private:
            WavyLineAttributesPtr myAttributes;
        };
        
        
        struct RepeatAttributes;
        using RepeatAttributesPtr = std::shared_ptr<RepeatAttributes>;
        
        struct RepeatAttributes : public AttributesInterface
        {
        public:
            RepeatAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            BackwardForward direction;
            NonNegativeInteger times;
            Winged winged;
            const bool hasDirection;
            bool hasTimes;
            bool hasWinged;
        };
        
        class Repeat;
        using RepeatPtr = std::shared_ptr<Repeat>;
        using RepeatUPtr = std::unique_ptr<Repeat>;
        using RepeatSet = std::vector<RepeatPtr>;
        using RepeatSetIter = RepeatSet::iterator;
        using RepeatSetIterConst = RepeatSet::const_iterator;
        inline RepeatPtr makeRepeat() { return std::make_shared<Repeat>(); }

        class Repeat : public ElementInterface
        {
        public:
            Repeat();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RepeatAttributesPtr getAttributes() const;
            void setAttributes( const RepeatAttributesPtr& attributes );

        private:
            RepeatAttributesPtr myAttributes;
        };
        
        
        struct WedgeAttributes;
        using WedgeAttributesPtr = std::shared_ptr<WedgeAttributes>;
        
        struct WedgeAttributes : public AttributesInterface
        {
        public:
            WedgeAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            WedgeType type;
            NumberLevel number;
            TenthsValue spread;
            YesNo niente;
            TenthsValue dashLength;
            TenthsValue spaceLength;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            const bool hasType;
            bool hasNumber;
            bool hasSpread;
            bool hasNiente;
            bool hasDashLength;
            bool hasSpaceLength;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
        };
        
        class Wedge;
        using WedgePtr = std::shared_ptr<Wedge>;
        using WedgeUPtr = std::unique_ptr<Wedge>;
        using WedgeSet = std::vector<WedgePtr>;
        using WedgeSetIter = WedgeSet::iterator;
        using WedgeSetIterConst = WedgeSet::const_iterator;
        inline WedgePtr makeWedge() { return std::make_shared<Wedge>(); }

        class Wedge : public ElementInterface
        {
        public:
            Wedge();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            WedgeAttributesPtr getAttributes() const;
            void setAttributes( const WedgeAttributesPtr& attributes );

        private:
            WedgeAttributesPtr myAttributes;
        };
        
        
        struct DashesAttributes;
        using DashesAttributesPtr = std::shared_ptr<DashesAttributes>;
        
        struct DashesAttributes : public AttributesInterface
        {
        public:
            DashesAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStopContinue type;
            NumberLevel number;
            TenthsValue dashLength;
            TenthsValue spaceLength;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            const bool hasType;
            bool hasNumber;
            bool hasDashLength;
            bool hasSpaceLength;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
        };
        
        class Dashes;
        using DashesPtr = std::shared_ptr<Dashes>;
        using DashesUPtr = std::unique_ptr<Dashes>;
        using DashesSet = std::vector<DashesPtr>;
        using DashesSetIter = DashesSet::iterator;
        using DashesSetIterConst = DashesSet::const_iterator;
        inline DashesPtr makeDashes() { return std::make_shared<Dashes>(); }

        class Dashes : public ElementInterface
        {
        public:
            Dashes();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DashesAttributesPtr getAttributes() const;
            void setAttributes( const DashesAttributesPtr& attributes );

        private:
            DashesAttributesPtr myAttributes;
        };
        
        
        struct BracketAttributes;
        using BracketAttributesPtr = std::shared_ptr<BracketAttributes>;
        
        struct BracketAttributes : public AttributesInterface
        {
        public:
            BracketAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo bracket;
            bool hasBracket;
        };
        
        class Bracket;
        using BracketPtr = std::shared_ptr<Bracket>;
        using BracketUPtr = std::unique_ptr<Bracket>;
        using BracketSet = std::vector<BracketPtr>;
        using BracketSetIter = BracketSet::iterator;
        using BracketSetIterConst = BracketSet::const_iterator;
        inline BracketPtr makeBracket() { return std::make_shared<Bracket>(); }

        class Bracket : public ElementInterface
        {
        public:
            Bracket();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BracketAttributesPtr getAttributes() const;
            void setAttributes( const BracketAttributesPtr& attributes );

        private:
            BracketAttributesPtr myAttributes;
        };
        
        
        struct PedalAttributes;
        using PedalAttributesPtr = std::shared_ptr<PedalAttributes>;
        
        struct PedalAttributes : public AttributesInterface
        {
        public:
            PedalAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStopChangeContinue type;
            YesNo line;
            YesNo sign;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight halign;
            const bool hasType;
            bool hasLine;
            bool hasSign;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
        };
        
        class Pedal;
        using PedalPtr = std::shared_ptr<Pedal>;
        using PedalUPtr = std::unique_ptr<Pedal>;
        using PedalSet = std::vector<PedalPtr>;
        using PedalSetIter = PedalSet::iterator;
        using PedalSetIterConst = PedalSet::const_iterator;
        inline PedalPtr makePedal() { return std::make_shared<Pedal>(); }

        class Pedal : public ElementInterface
        {
        public:
            Pedal();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PedalAttributesPtr getAttributes() const;
            void setAttributes( const PedalAttributesPtr& attributes );

        private:
            PedalAttributesPtr myAttributes;
        };
        
        
        struct OctaveShiftAttributes;
        using OctaveShiftAttributesPtr = std::shared_ptr<OctaveShiftAttributes>;
        
        struct OctaveShiftAttributes : public AttributesInterface
        {
        public:
            OctaveShiftAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            UpDownStopContinue type;
            NumberLevel number;
            PositiveInteger size;
            TenthsValue dashLength;
            TenthsValue spaceLength;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            const bool hasType;
            bool hasNumber;
            bool hasSize;
            bool hasDashLength;
            bool hasSpaceLength;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class OctaveShift;
        using OctaveShiftPtr = std::shared_ptr<OctaveShift>;
        using OctaveShiftUPtr = std::unique_ptr<OctaveShift>;
        using OctaveShiftSet = std::vector<OctaveShiftPtr>;
        using OctaveShiftSetIter = OctaveShiftSet::iterator;
        using OctaveShiftSetIterConst = OctaveShiftSet::const_iterator;
        inline OctaveShiftPtr makeOctaveShift() { return std::make_shared<OctaveShift>(); }

        class OctaveShift : public ElementInterface
        {
        public:
            OctaveShift();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OctaveShiftAttributesPtr getAttributes() const;
            void setAttributes( const OctaveShiftAttributesPtr& attributes );

        private:
            OctaveShiftAttributesPtr myAttributes;
        };
        
        
        struct StringMuteAttributes;
        using StringMuteAttributesPtr = std::shared_ptr<StringMuteAttributes>;
        
        struct StringMuteAttributes : public AttributesInterface
        {
        public:
            StringMuteAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            OnOff type;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            LeftCenterRight halign;
            const bool hasType;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
        };
        
        class StringMute;
        using StringMutePtr = std::shared_ptr<StringMute>;
        using StringMuteUPtr = std::unique_ptr<StringMute>;
        using StringMuteSet = std::vector<StringMutePtr>;
        using StringMuteSetIter = StringMuteSet::iterator;
        using StringMuteSetIterConst = StringMuteSet::const_iterator;
        inline StringMutePtr makeStringMute() { return std::make_shared<StringMute>(); }

        class StringMute : public ElementInterface
        {
        public:
            StringMute();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StringMuteAttributesPtr getAttributes() const;
            void setAttributes( const StringMuteAttributesPtr& attributes );

        private:
            StringMuteAttributesPtr myAttributes;
        };
        
        
        struct ImageAttributes;
        using ImageAttributesPtr = std::shared_ptr<ImageAttributes>;
        
        struct ImageAttributes : public AttributesInterface
        {
        public:
            ImageAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsAnyUri source;
            XsToken type;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            LeftCenterRight halign;
            const bool hasSource;
            const bool hasType;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasHalign;
        };
        
        class Image;
        using ImagePtr = std::shared_ptr<Image>;
        using ImageUPtr = std::unique_ptr<Image>;
        using ImageSet = std::vector<ImagePtr>;
        using ImageSetIter = ImageSet::iterator;
        using ImageSetIterConst = ImageSet::const_iterator;
        inline ImagePtr makeImage() { return std::make_shared<Image>(); }

        class Image : public ElementInterface
        {
        public:
            Image();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ImageAttributesPtr getAttributes() const;
            void setAttributes( const ImageAttributesPtr& attributes );

        private:
            ImageAttributesPtr myAttributes;
        };
        
        
        struct BarreAttributes;
        using BarreAttributesPtr = std::shared_ptr<BarreAttributes>;
        
        struct BarreAttributes : public AttributesInterface
        {
        public:
            BarreAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            const bool hasType;
        };
        
        class Barre;
        using BarrePtr = std::shared_ptr<Barre>;
        using BarreUPtr = std::unique_ptr<Barre>;
        using BarreSet = std::vector<BarrePtr>;
        using BarreSetIter = BarreSet::iterator;
        using BarreSetIterConst = BarreSet::const_iterator;
        inline BarrePtr makeBarre() { return std::make_shared<Barre>(); }

        class Barre : public ElementInterface
        {
        public:
            Barre();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BarreAttributesPtr getAttributes() const;
            void setAttributes( const BarreAttributesPtr& attributes );

        private:
            BarreAttributesPtr myAttributes;
        };
        
        
        struct SupportsAttributes;
        using SupportsAttributesPtr = std::shared_ptr<SupportsAttributes>;
        
        struct SupportsAttributes : public AttributesInterface
        {
        public:
            SupportsAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo type;
            XsNMToken element;
            XsNMToken attribute;
            XsToken value;
            const bool hasType;
            const bool hasElement;
            bool hasAttribute;
            bool hasValue;
        };
        
        class Supports;
        using SupportsPtr = std::shared_ptr<Supports>;
        using SupportsUPtr = std::unique_ptr<Supports>;
        using SupportsSet = std::vector<SupportsPtr>;
        using SupportsSetIter = SupportsSet::iterator;
        using SupportsSetIterConst = SupportsSet::const_iterator;
        inline SupportsPtr makeSupports() { return std::make_shared<Supports>(); }

        class Supports : public ElementInterface
        {
        public:
            Supports();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            SupportsAttributesPtr getAttributes() const;
            void setAttributes( const SupportsAttributesPtr& attributes );

        private:
            SupportsAttributesPtr myAttributes;
        };
        
        
        struct ExtendAttributes;
        using ExtendAttributesPtr = std::shared_ptr<ExtendAttributes>;
        
        struct ExtendAttributes : public AttributesInterface
        {
        public:
            ExtendAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStopContinue type;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasType;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class Extend;
        using ExtendPtr = std::shared_ptr<Extend>;
        using ExtendUPtr = std::unique_ptr<Extend>;
        using ExtendSet = std::vector<ExtendPtr>;
        using ExtendSetIter = ExtendSet::iterator;
        using ExtendSetIterConst = ExtendSet::const_iterator;
        inline ExtendPtr makeExtend() { return std::make_shared<Extend>(); }

        class Extend : public ElementInterface
        {
        public:
            Extend();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ExtendAttributesPtr getAttributes() const;
            void setAttributes( const ExtendAttributesPtr& attributes );

        private:
            ExtendAttributesPtr myAttributes;
        };
        
        
        struct TiedAttributes;
        using TiedAttributesPtr = std::shared_ptr<TiedAttributes>;
        
        struct TiedAttributes : public AttributesInterface
        {
        public:
            TiedAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStopContinue type;
            NumberLevel number;
            TenthsValue dashLength;
            TenthsValue spaceLength;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            AboveBelow placement;
            OverUnder orientation;
            DivisionsValue bezierOffset;
            DivisionsValue bezierOffset2;
            TenthsValue bezierX;
            TenthsValue bezierY;
            TenthsValue bezierX2;
            TenthsValue bezierY2;
            const bool hasType;
            bool hasNumber;
            bool hasDashLength;
            bool hasSpaceLength;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasPlacement;
            bool hasOrientation;
            bool hasBezierOffset;
            bool hasBezierOffset2;
            bool hasBezierX;
            bool hasBezierY;
            bool hasBezierX2;
            bool hasBezierY2;
        };
        
        class Tied;
        using TiedPtr = std::shared_ptr<Tied>;
        using TiedUPtr = std::unique_ptr<Tied>;
        using TiedSet = std::vector<TiedPtr>;
        using TiedSetIter = TiedSet::iterator;
        using TiedSetIterConst = TiedSet::const_iterator;
        inline TiedPtr makeTied() { return std::make_shared<Tied>(); }

        class Tied : public ElementInterface
        {
        public:
            Tied();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TiedAttributesPtr getAttributes() const;
            void setAttributes( const TiedAttributesPtr& attributes );

        private:
            TiedAttributesPtr myAttributes;
        };
        
        
        struct SlurAttributes;
        using SlurAttributesPtr = std::shared_ptr<SlurAttributes>;
        
        struct SlurAttributes : public AttributesInterface
        {
        public:
            SlurAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStopContinue type;
            NumberLevel number;
            TenthsValue dashLength;
            TenthsValue spaceLength;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            AboveBelow placement;
            OverUnder orientation;
            DivisionsValue bezierOffset;
            DivisionsValue bezierOffset2;
            TenthsValue bezierX;
            TenthsValue bezierY;
            TenthsValue bezierX2;
            TenthsValue bezierY2;
            const bool hasType;
            bool hasNumber;
            bool hasDashLength;
            bool hasSpaceLength;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasPlacement;
            bool hasOrientation;
            bool hasBezierOffset;
            bool hasBezierOffset2;
            bool hasBezierX;
            bool hasBezierY;
            bool hasBezierX2;
            bool hasBezierY2;
        };
        
        class Slur;
        using SlurPtr = std::shared_ptr<Slur>;
        using SlurUPtr = std::unique_ptr<Slur>;
        using SlurSet = std::vector<SlurPtr>;
        using SlurSetIter = SlurSet::iterator;
        using SlurSetIterConst = SlurSet::const_iterator;
        inline SlurPtr makeSlur() { return std::make_shared<Slur>(); }

        class Slur : public ElementInterface
        {
        public:
            Slur();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            SlurAttributesPtr getAttributes() const;
            void setAttributes( const SlurAttributesPtr& attributes );

        private:
            SlurAttributesPtr myAttributes;
        };
        
        
        struct ArpeggiateAttributes;
        using ArpeggiateAttributesPtr = std::shared_ptr<ArpeggiateAttributes>;
        
        struct ArpeggiateAttributes : public AttributesInterface
        {
        public:
            ArpeggiateAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            NumberLevel number;
            UpDown direction;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            AboveBelow placement;
            bool hasNumber;
            bool hasDirection;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasPlacement;
        };
        
        class Arpeggiate;
        using ArpeggiatePtr = std::shared_ptr<Arpeggiate>;
        using ArpeggiateUPtr = std::unique_ptr<Arpeggiate>;
        using ArpeggiateSet = std::vector<ArpeggiatePtr>;
        using ArpeggiateSetIter = ArpeggiateSet::iterator;
        using ArpeggiateSetIterConst = ArpeggiateSet::const_iterator;
        inline ArpeggiatePtr makeArpeggiate() { return std::make_shared<Arpeggiate>(); }

        class Arpeggiate : public ElementInterface
        {
        public:
            Arpeggiate();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ArpeggiateAttributesPtr getAttributes() const;
            void setAttributes( const ArpeggiateAttributesPtr& attributes );

        private:
            ArpeggiateAttributesPtr myAttributes;
        };
        
        
        struct NonArpeggiateAttributes;
        using NonArpeggiateAttributesPtr = std::shared_ptr<NonArpeggiateAttributes>;
        
        struct NonArpeggiateAttributes : public AttributesInterface
        {
        public:
            NonArpeggiateAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TopBottom type;
            NumberLevel number;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            AboveBelow placement;
            const bool hasType;
            bool hasNumber;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasPlacement;
        };
        
        class NonArpeggiate;
        using NonArpeggiatePtr = std::shared_ptr<NonArpeggiate>;
        using NonArpeggiateUPtr = std::unique_ptr<NonArpeggiate>;
        using NonArpeggiateSet = std::vector<NonArpeggiatePtr>;
        using NonArpeggiateSetIter = NonArpeggiateSet::iterator;
        using NonArpeggiateSetIterConst = NonArpeggiateSet::const_iterator;
        inline NonArpeggiatePtr makeNonArpeggiate() { return std::make_shared<NonArpeggiate>(); }

        class NonArpeggiate : public ElementInterface
        {
        public:
            NonArpeggiate();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NonArpeggiateAttributesPtr getAttributes() const;
            void setAttributes( const NonArpeggiateAttributesPtr& attributes );

        private:
            NonArpeggiateAttributesPtr myAttributes;
        };
        
        
        struct GraceAttributes;
        using GraceAttributesPtr = std::shared_ptr<GraceAttributes>;
        
        struct GraceAttributes : public AttributesInterface
        {
        public:
            GraceAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            Percent stealTimePrevious;
            Percent stealTimeFollowing;
            DivisionsValue makeTime;
            YesNo slash;
            bool hasStealTimePrevious;
            bool hasStealTimeFollowing;
            bool hasMakeTime;
            bool hasSlash;
        };
        
        class Grace;
        using GracePtr = std::shared_ptr<Grace>;
        using GraceUPtr = std::unique_ptr<Grace>;
        using GraceSet = std::vector<GracePtr>;
        using GraceSetIter = GraceSet::iterator;
        using GraceSetIterConst = GraceSet::const_iterator;
        inline GracePtr makeGrace() { return std::make_shared<Grace>(); }

        class Grace : public ElementInterface
        {
        public:
            Grace();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            GraceAttributesPtr getAttributes() const;
            void setAttributes( const GraceAttributesPtr& attributes );

        private:
            GraceAttributesPtr myAttributes;
        };
        
        
        struct TieAttributes;
        using TieAttributesPtr = std::shared_ptr<TieAttributes>;
        
        struct TieAttributes : public AttributesInterface
        {
        public:
            TieAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            TimeOnly timeOnly;
            const bool hasType;
            bool hasTimeOnly;
        };
        
        class Tie;
        using TiePtr = std::shared_ptr<Tie>;
        using TieUPtr = std::unique_ptr<Tie>;
        using TieSet = std::vector<TiePtr>;
        using TieSetIter = TieSet::iterator;
        using TieSetIterConst = TieSet::const_iterator;
        inline TiePtr makeTie() { return std::make_shared<Tie>(); }

        class Tie : public ElementInterface
        {
        public:
            Tie();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TieAttributesPtr getAttributes() const;
            void setAttributes( const TieAttributesPtr& attributes );

        private:
            TieAttributesPtr myAttributes;
        };
        
        
        
        struct InstrumentAttributes;
        using InstrumentAttributesPtr = std::shared_ptr<InstrumentAttributes>;
        
        struct InstrumentAttributes : public AttributesInterface
        {
        public:
            InstrumentAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsIDREF id;
            const bool hasId;
        };
        
        class Instrument;
        using InstrumentPtr = std::shared_ptr<Instrument>;
        using InstrumentUPtr = std::unique_ptr<Instrument>;
        using InstrumentSet = std::vector<InstrumentPtr>;
        using InstrumentSetIter = InstrumentSet::iterator;
        using InstrumentSetIterConst = InstrumentSet::const_iterator;
        inline InstrumentPtr makeInstrument() { return std::make_shared<Instrument>(); }

        class Instrument : public ElementInterface
        {
        public:
            Instrument();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            InstrumentAttributesPtr getAttributes() const;
            void setAttributes( const InstrumentAttributesPtr& attributes );

        private:
            InstrumentAttributesPtr myAttributes;
        };
        
        
        struct TurnAttributes;
        using TurnAttributesPtr = std::shared_ptr<TurnAttributes>;
        
        struct TurnAttributes : public AttributesInterface
        {
        public:
            TurnAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            StartNote startNote;
            TrillStep trillStep;
            TwoNoteTurn twoNoteTurn;
            YesNo accelerate;
            TrillBeats beats;
            Percent secondBeat;
            Percent lastBeat;
            YesNo slash;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
            bool hasStartNote;
            bool hasTrillStep;
            bool hasTwoNoteTurn;
            bool hasAccelerate;
            bool hasBeats;
            bool hasSecondBeat;
            bool hasLastBeat;
            bool hasSlash;
        };
        
        class Turn;
        using TurnPtr = std::shared_ptr<Turn>;
        using TurnUPtr = std::unique_ptr<Turn>;
        using TurnSet = std::vector<TurnPtr>;
        using TurnSetIter = TurnSet::iterator;
        using TurnSetIterConst = TurnSet::const_iterator;
        inline TurnPtr makeTurn() { return std::make_shared<Turn>(); }

        class Turn : public ElementInterface
        {
        public:
            Turn();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TurnAttributesPtr getAttributes() const;
            void setAttributes( const TurnAttributesPtr& attributes );

        private:
            TurnAttributesPtr myAttributes;
        };
        
        
        struct DelayedTurnAttributes;
        using DelayedTurnAttributesPtr = std::shared_ptr<DelayedTurnAttributes>;
        
        struct DelayedTurnAttributes : public AttributesInterface
        {
        public:
            DelayedTurnAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            StartNote startNote;
            TrillStep trillStep;
            TwoNoteTurn twoNoteTurn;
            YesNo accelerate;
            TrillBeats beats;
            Percent secondBeat;
            Percent lastBeat;
            YesNo slash;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
            bool hasStartNote;
            bool hasTrillStep;
            bool hasTwoNoteTurn;
            bool hasAccelerate;
            bool hasBeats;
            bool hasSecondBeat;
            bool hasLastBeat;
            bool hasSlash;
        };
        
        class DelayedTurn;
        using DelayedTurnPtr = std::shared_ptr<DelayedTurn>;
        using DelayedTurnUPtr = std::unique_ptr<DelayedTurn>;
        using DelayedTurnSet = std::vector<DelayedTurnPtr>;
        using DelayedTurnSetIter = DelayedTurnSet::iterator;
        using DelayedTurnSetIterConst = DelayedTurnSet::const_iterator;
        inline DelayedTurnPtr makeDelayedTurn() { return std::make_shared<DelayedTurn>(); }

        class DelayedTurn : public ElementInterface
        {
        public:
            DelayedTurn();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DelayedTurnAttributesPtr getAttributes() const;
            void setAttributes( const DelayedTurnAttributesPtr& attributes );

        private:
            DelayedTurnAttributesPtr myAttributes;
        };
        
        
        struct InvertedTurnAttributes;
        using InvertedTurnAttributesPtr = std::shared_ptr<InvertedTurnAttributes>;
        
        struct InvertedTurnAttributes : public AttributesInterface
        {
        public:
            InvertedTurnAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            StartNote startNote;
            TrillStep trillStep;
            TwoNoteTurn twoNoteTurn;
            YesNo accelerate;
            TrillBeats beats;
            Percent secondBeat;
            Percent lastBeat;
            YesNo slash;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
            bool hasStartNote;
            bool hasTrillStep;
            bool hasTwoNoteTurn;
            bool hasAccelerate;
            bool hasBeats;
            bool hasSecondBeat;
            bool hasLastBeat;
            bool hasSlash;
        };
        
        class InvertedTurn;
        using InvertedTurnPtr = std::shared_ptr<InvertedTurn>;
        using InvertedTurnUPtr = std::unique_ptr<InvertedTurn>;
        using InvertedTurnSet = std::vector<InvertedTurnPtr>;
        using InvertedTurnSetIter = InvertedTurnSet::iterator;
        using InvertedTurnSetIterConst = InvertedTurnSet::const_iterator;
        inline InvertedTurnPtr makeInvertedTurn() { return std::make_shared<InvertedTurn>(); }

        class InvertedTurn : public ElementInterface
        {
        public:
            InvertedTurn();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            InvertedTurnAttributesPtr getAttributes() const;
            void setAttributes( const InvertedTurnAttributesPtr& attributes );

        private:
            InvertedTurnAttributesPtr myAttributes;
        };
        
        
        struct DelayedInvertedTurnAttributes;
        using DelayedInvertedTurnAttributesPtr = std::shared_ptr<DelayedInvertedTurnAttributes>;
        
        struct DelayedInvertedTurnAttributes : public AttributesInterface
        {
        public:
            DelayedInvertedTurnAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            AboveBelow placement;
            StartNote startNote;
            TrillStep trillStep;
            TwoNoteTurn twoNoteTurn;
            YesNo accelerate;
            TrillBeats beats;
            Percent secondBeat;
            Percent lastBeat;
            YesNo slash;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPlacement;
            bool hasStartNote;
            bool hasTrillStep;
            bool hasTwoNoteTurn;
            bool hasAccelerate;
            bool hasBeats;
            bool hasSecondBeat;
            bool hasLastBeat;
            bool hasSlash;
        };
        
        class DelayedInvertedTurn;
        using DelayedInvertedTurnPtr = std::shared_ptr<DelayedInvertedTurn>;
        using DelayedInvertedTurnUPtr = std::unique_ptr<DelayedInvertedTurn>;
        using DelayedInvertedTurnSet = std::vector<DelayedInvertedTurnPtr>;
        using DelayedInvertedTurnSetIter = DelayedInvertedTurnSet::iterator;
        using DelayedInvertedTurnSetIterConst = DelayedInvertedTurnSet::const_iterator;
        inline DelayedInvertedTurnPtr makeDelayedInvertedTurn() { return std::make_shared<DelayedInvertedTurn>(); }

        class DelayedInvertedTurn : public ElementInterface
        {
        public:
            DelayedInvertedTurn();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DelayedInvertedTurnAttributesPtr getAttributes() const;
            void setAttributes( const DelayedInvertedTurnAttributesPtr& attributes );

        private:
            DelayedInvertedTurnAttributesPtr myAttributes;
        };
        
        
        struct TupletDotAttributes;
        using TupletDotAttributesPtr = std::shared_ptr<TupletDotAttributes>;
        
        struct TupletDotAttributes : public AttributesInterface
        {
        public:
            TupletDotAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class TupletDot;
        using TupletDotPtr = std::shared_ptr<TupletDot>;
        using TupletDotUPtr = std::unique_ptr<TupletDot>;
        using TupletDotSet = std::vector<TupletDotPtr>;
        using TupletDotSetIter = TupletDotSet::iterator;
        using TupletDotSetIterConst = TupletDotSet::const_iterator;
        inline TupletDotPtr makeTupletDot() { return std::make_shared<TupletDot>(); }

        class TupletDot : public ElementInterface
        {
        public:
            TupletDot();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TupletDotAttributesPtr getAttributes() const;
            void setAttributes( const TupletDotAttributesPtr& attributes );

        private:
            TupletDotAttributesPtr myAttributes;
        };
        
        
        struct LinkAttributes;
        using LinkAttributesPtr = std::shared_ptr<LinkAttributes>;
        
        struct LinkAttributes : public AttributesInterface
        {
        public:
            LinkAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XlinkHref href;
            const XlinkType type;
            XlinkRole role;
            XlinkTitle title;
            XlinkShow show;
            XlinkActuate actuate;
            XsToken name;
            XsNMToken element;
            PositiveInteger position;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            const bool hasHref;
            bool hasType;
            bool hasRole;
            bool hasTitle;
            bool hasShow;
            bool hasActuate;
            bool hasName;
            bool hasElement;
            bool hasPosition;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
        };
        
        class Link;
        using LinkPtr = std::shared_ptr<Link>;
        using LinkUPtr = std::unique_ptr<Link>;
        using LinkSet = std::vector<LinkPtr>;
        using LinkSetIter = LinkSet::iterator;
        using LinkSetIterConst = LinkSet::const_iterator;
        inline LinkPtr makeLink() { return std::make_shared<Link>(); }

        class Link : public ElementInterface
        {
        public:
            Link();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            LinkAttributesPtr getAttributes() const;
            void setAttributes( const LinkAttributesPtr& attributes );

        private:
            LinkAttributesPtr myAttributes;
        };
        
        
        struct BookmarkAttributes;
        using BookmarkAttributesPtr = std::shared_ptr<BookmarkAttributes>;
        
        struct BookmarkAttributes : public AttributesInterface
        {
        public:
            BookmarkAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsID id;
            XsToken name;
            XsNMToken element;
            PositiveInteger position;
            const bool hasId;
            bool hasName;
            bool hasElement;
            bool hasPosition;
        };
        
        class Bookmark;
        using BookmarkPtr = std::shared_ptr<Bookmark>;
        using BookmarkUPtr = std::unique_ptr<Bookmark>;
        using BookmarkSet = std::vector<BookmarkPtr>;
        using BookmarkSetIter = BookmarkSet::iterator;
        using BookmarkSetIterConst = BookmarkSet::const_iterator;
        inline BookmarkPtr makeBookmark() { return std::make_shared<Bookmark>(); }

        class Bookmark : public ElementInterface
        {
        public:
            Bookmark();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BookmarkAttributesPtr getAttributes() const;
            void setAttributes( const BookmarkAttributesPtr& attributes );

        private:
            BookmarkAttributesPtr myAttributes;
        };
        
        
        struct CreditImageAttributes;
        using CreditImageAttributesPtr = std::shared_ptr<CreditImageAttributes>;
        
        struct CreditImageAttributes : public AttributesInterface
        {
        public:
            CreditImageAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsAnyUri source;
            XsToken type;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            LeftCenterRight halign;
            const bool hasSource;
            const bool hasType;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasHalign;
        };
        
        class CreditImage;
        using CreditImagePtr = std::shared_ptr<CreditImage>;
        using CreditImageUPtr = std::unique_ptr<CreditImage>;
        using CreditImageSet = std::vector<CreditImagePtr>;
        using CreditImageSetIter = CreditImageSet::iterator;
        using CreditImageSetIterConst = CreditImageSet::const_iterator;
        inline CreditImagePtr makeCreditImage() { return std::make_shared<CreditImage>(); }

        class CreditImage : public ElementInterface
        {
        public:
            CreditImage();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            CreditImageAttributesPtr getAttributes() const;
            void setAttributes( const CreditImageAttributesPtr& attributes );

        private:
            CreditImageAttributesPtr myAttributes;
        };
        
        
        struct LyricFontAttributes;
        using LyricFontAttributesPtr = std::shared_ptr<LyricFontAttributes>;
        
        struct LyricFontAttributes : public AttributesInterface
        {
        public:
            LyricFontAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsNMToken number;
            XsToken name;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            bool hasNumber;
            bool hasName;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
        };
        
        class LyricFont;
        using LyricFontPtr = std::shared_ptr<LyricFont>;
        using LyricFontUPtr = std::unique_ptr<LyricFont>;
        using LyricFontSet = std::vector<LyricFontPtr>;
        using LyricFontSetIter = LyricFontSet::iterator;
        using LyricFontSetIterConst = LyricFontSet::const_iterator;
        inline LyricFontPtr makeLyricFont() { return std::make_shared<LyricFont>(); }

        class LyricFont : public ElementInterface
        {
        public:
            LyricFont();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            LyricFontAttributesPtr getAttributes() const;
            void setAttributes( const LyricFontAttributesPtr& attributes );

        private:
            LyricFontAttributesPtr myAttributes;
        };
        
        
        struct LyricLanguageAttributes;
        using LyricLanguageAttributesPtr = std::shared_ptr<LyricLanguageAttributes>;
        
        struct LyricLanguageAttributes : public AttributesInterface
        {
        public:
            LyricLanguageAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsNMToken number;
            XsToken name;
            XmlLang lang;
            bool hasNumber;
            bool hasName;
            const bool hasLang;
        };
        
        class LyricLanguage;
        using LyricLanguagePtr = std::shared_ptr<LyricLanguage>;
        using LyricLanguageUPtr = std::unique_ptr<LyricLanguage>;
        using LyricLanguageSet = std::vector<LyricLanguagePtr>;
        using LyricLanguageSetIter = LyricLanguageSet::iterator;
        using LyricLanguageSetIterConst = LyricLanguageSet::const_iterator;
        inline LyricLanguagePtr makeLyricLanguage() { return std::make_shared<LyricLanguage>(); }

        class LyricLanguage : public ElementInterface
        {
        public:
            LyricLanguage();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            LyricLanguageAttributesPtr getAttributes() const;
            void setAttributes( const LyricLanguageAttributesPtr& attributes );

        private:
            LyricLanguageAttributesPtr myAttributes;
        };
        
        
        struct OpusAttributes;
        using OpusAttributesPtr = std::shared_ptr<OpusAttributes>;
        
        struct OpusAttributes : public AttributesInterface
        {
        public:
            OpusAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XlinkHref href;
            const XlinkType type;
            XlinkRole role;
            XlinkTitle title;
            XlinkShow show;
            XlinkActuate actuate;
            const bool hasHref;
            bool hasType;
            bool hasRole;
            bool hasTitle;
            bool hasShow;
            bool hasActuate;
        };
        
        class Opus;
        using OpusPtr = std::shared_ptr<Opus>;
        using OpusUPtr = std::unique_ptr<Opus>;
        using OpusSet = std::vector<OpusPtr>;
        using OpusSetIter = OpusSet::iterator;
        using OpusSetIterConst = OpusSet::const_iterator;
        inline OpusPtr makeOpus() { return std::make_shared<Opus>(); }

        class Opus : public ElementInterface
        {
        public:
            Opus();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OpusAttributesPtr getAttributes() const;
            void setAttributes( const OpusAttributesPtr& attributes );

        private:
            OpusAttributesPtr myAttributes;
        };
        
        class TraditionalKey;
        using TraditionalKeyPtr = std::shared_ptr<TraditionalKey>;
        using TraditionalKeyUPtr = std::unique_ptr<TraditionalKey>;
        using TraditionalKeySet = std::vector<TraditionalKeyPtr>;
        using TraditionalKeySetIter = TraditionalKeySet::iterator;
        using TraditionalKeySetIterConst = TraditionalKeySet::const_iterator;
        inline TraditionalKeyPtr makeTraditionalKey() { return std::make_shared<TraditionalKey>(); }

        class TraditionalKey : public ElementInterface
        {
        public:
            TraditionalKey();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            CancelPtr getCancel() const;
            void setCancel( const CancelPtr& value );
            bool getHasCancel() const;
            void setHasCancel( const bool value );
            FifthsPtr getFifths() const;
            void setFifths( const FifthsPtr& value );
            ModePtr getMode() const;
            void setMode( const ModePtr& value );
            bool getHasMode() const;
            void setHasMode( const bool value );

        private:
            CancelPtr myCancel;
            FifthsPtr myFifths;
            ModePtr myMode;
            bool myHasCancel;
            bool myHasMode;
        };
        
        class NonTraditionalKey;
        using NonTraditionalKeyPtr = std::shared_ptr<NonTraditionalKey>;
        using NonTraditionalKeyUPtr = std::shared_ptr<NonTraditionalKey>;
        using NonTraditionalKeySet = std::vector<NonTraditionalKeyPtr>;
        using NonTraditionalKeySetIter = NonTraditionalKeySet::iterator;
        using NonTraditionalKeySetIterConst = NonTraditionalKeySet::const_iterator;
        inline NonTraditionalKeyPtr makeNonTraditionalKey() { return std::make_shared<NonTraditionalKey>(); }

        class NonTraditionalKey : public ElementInterface
        {
        public:
            NonTraditionalKey();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            KeyStepPtr getKeyStep() const;
            void setKeyStep( const KeyStepPtr& value );
            KeyAlterPtr getKeyAlter() const;
            void setKeyAlter( const KeyAlterPtr& value );
            KeyAccidentalPtr getKeyAccidental() const;
            void setKeyAccidental( const KeyAccidentalPtr& value );
            bool getHasKeyAccidental() const;
            void setHasKeyAccidental( const bool value );

        private:
            KeyStepPtr myKeyStep;
            KeyAlterPtr myKeyAlter;
            KeyAccidentalPtr myKeyAccidental;
            bool myHasKeyAccidental;
        };
        
        
        class KeyChoice;
        using KeyChoicePtr = std::shared_ptr<KeyChoice>;
        using KeyChoiceUPtr = std::unique_ptr<KeyChoice>;
        using KeyChoiceSet = std::vector<KeyChoicePtr>;
        using KeyChoiceSetIter = KeyChoiceSet::iterator;
        using KeyChoiceSetIterConst = KeyChoiceSet::const_iterator;
        inline KeyChoicePtr makeKeyChoice() { return std::make_shared<KeyChoice>(); }

        class KeyChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                traditionalKey = 0,
                nonTraditionalKey = 1
            };
            KeyChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Choice getChoice() const;
            void setChoice( const Choice value );
            TraditionalKeyPtr getTraditionalKey() const;
            void setTraditionalKey( const TraditionalKeyPtr& value );
            
            const NonTraditionalKeySet& getNonTraditionalKeySet() const;
            void removeNonTraditionalKey( const NonTraditionalKeySetIterConst& value );
            void addNonTraditionalKey( const NonTraditionalKeyPtr& value );
            void clearNonTraditionalKeySet();
            

        private:
            Choice myChoice;
            TraditionalKeyPtr myTraditionalKey;
            NonTraditionalKeySet myNonTraditionalKeySet;
        };
        
        
        struct KeyAttributes;
        using KeyAttributesPtr = std::shared_ptr<KeyAttributes>;
        struct KeyAttributes : public AttributesInterface
        {
        public:
            KeyAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StaffNumber number;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            YesNo printObject;
            bool hasNumber;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPrintObject;
        };
        
        class Key;
        using KeyPtr = std::shared_ptr<Key>;
        using KeyUPtr = std::unique_ptr<Key>;
        using KeySet = std::vector<KeyPtr>;
        using KeySetIter = KeySet::iterator;
        using KeySetIterConst = KeySet::const_iterator;
        inline KeyPtr makeKey() { return std::make_shared<Key>(); }

        class Key : public ElementInterface
        {
        public:
            Key();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            KeyAttributesPtr getAttributes() const;
            void setAttributes( const KeyAttributesPtr& value );
            KeyChoicePtr getKeyChoice() const;
            void setKeyChoice( const KeyChoicePtr& value );
            const KeyOctaveSet& getKeyOctaveSet() const;
            void removeKeyOctave( const KeyOctaveSetIterConst& value );
            void addKeyOctave( const KeyOctavePtr& value );
            void clearKeyOctaveSet();

        private:
            KeyAttributesPtr myAttributes;
            KeyChoicePtr myKeyChoice;
            KeyOctaveSet myKeyOctaveSet;
        };
        
        
        struct ClefAttributes;
        using ClefAttributesPtr = std::shared_ptr<ClefAttributes>;
        
        struct ClefAttributes : public AttributesInterface
        {
        public:
            ClefAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StaffNumber number;
            YesNo additional;
            SymbolSize size;
            YesNo afterBarline;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            YesNo printObject;
            bool hasNumber;
            bool hasAdditional;
            bool hasSize;
            bool hasAfterBarline;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasPrintObject;
        };
        
        class Clef;
        using ClefPtr = std::shared_ptr<Clef>;
        using ClefUPtr = std::unique_ptr<Clef>;
        using ClefSet = std::vector<ClefPtr>;
        using ClefSetIter = ClefSet::iterator;
        using ClefSetIterConst = ClefSet::const_iterator;
        inline ClefPtr makeClef() { return std::make_shared<Clef>(); }

        class Clef : public ElementInterface
        {
        public:
            Clef();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ClefAttributesPtr getAttributes() const;
            void setAttributes( const ClefAttributesPtr& value );

            /* _________ Sign minOccurs = 1, maxOccurs = 1 _________ */
            SignPtr getSign() const;
            void setSign( const SignPtr& value );

            /* _________ Line minOccurs = 0, maxOccurs = 1 _________ */
            LinePtr getLine() const;
            void setLine( const LinePtr& value );
            bool getHasLine() const;
            void setHasLine( const bool value );

            /* _________ ClefOctaveChange minOccurs = 0, maxOccurs = 1 _________ */
            ClefOctaveChangePtr getClefOctaveChange() const;
            void setClefOctaveChange( const ClefOctaveChangePtr& value );
            bool getHasClefOctaveChange() const;
            void setHasClefOctaveChange( const bool value );

        private:
            ClefAttributesPtr myAttributes;
            SignPtr mySign;
            LinePtr myLine;
            bool myHasLine;
            ClefOctaveChangePtr myClefOctaveChange;
            bool myHasClefOctaveChange;
        };
        
        struct TransposeAttributes;
        using TransposeAttributesPtr = std::shared_ptr<TransposeAttributes>;
        
        struct TransposeAttributes : public AttributesInterface
        {
        public:
            TransposeAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StaffNumber number;
            bool hasNumber;
        };
        
        class Transpose;
        using TransposePtr = std::shared_ptr<Transpose>;
        using TransposeUPtr = std::unique_ptr<Transpose>;
        using TransposeSet = std::vector<TransposePtr>;
        using TransposeSetIter = TransposeSet::iterator;
        using TransposeSetIterConst = TransposeSet::const_iterator;
        inline TransposePtr makeTranspose() { return std::make_shared<Transpose>(); }

        class Transpose : public ElementInterface
        {
        public:
            Transpose();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TransposeAttributesPtr getAttributes() const;
            void setAttributes( const TransposeAttributesPtr& value );

            /* _________ Diatonic minOccurs = 0, maxOccurs = 1 _________ */
            DiatonicPtr getDiatonic() const;
            void setDiatonic( const DiatonicPtr& value );
            bool getHasDiatonic() const;
            void setHasDiatonic( const bool value );

            /* _________ Chromatic minOccurs = 1, maxOccurs = 1 _________ */
            ChromaticPtr getChromatic() const;
            void setChromatic( const ChromaticPtr& value );

            /* _________ OctaveChange minOccurs = 0, maxOccurs = 1 _________ */
            OctaveChangePtr getOctaveChange() const;
            void setOctaveChange( const OctaveChangePtr& value );
            bool getHasOctaveChange() const;
            void setHasOctaveChange( const bool value );

            /* _________ Double minOccurs = 0, maxOccurs = 1 _________ */
            DoublePtr getDouble() const;
            void setDouble( const DoublePtr& value );
            bool getHasDouble() const;
            void setHasDouble( const bool value );

        private:
            TransposeAttributesPtr myAttributes;
            DiatonicPtr myDiatonic;
            bool myHasDiatonic;
            ChromaticPtr myChromatic;
            OctaveChangePtr myOctaveChange;
            bool myHasOctaveChange;
            DoublePtr myDouble;
            bool myHasDouble;
        };
        
        
        struct DirectiveAttributes;
        using DirectiveAttributesPtr = std::shared_ptr<DirectiveAttributes>;
        
        struct DirectiveAttributes : public AttributesInterface
        {
        public:
            DirectiveAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            XmlLang lang;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasLang;
        };
        
        class Directive;
        using DirectivePtr = std::shared_ptr<Directive>;
        using DirectiveUPtr = std::unique_ptr<Directive>;
        using DirectiveSet = std::vector<DirectivePtr>;
        using DirectiveSetIter = DirectiveSet::iterator;
        using DirectiveSetIterConst = DirectiveSet::const_iterator;
        inline DirectivePtr makeDirective() { return std::make_shared<Directive>(); }
        inline DirectivePtr makeDirective( const XsString& value ) { return std::make_shared<Directive>( value ); }
        inline DirectivePtr makeDirective( XsString&& value ) { return std::make_shared<Directive>( std::move( value ) ); }

        class Directive : public ElementInterface
        {
        public:
            Directive();
            Directive( const XsString& value );

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DirectiveAttributesPtr getAttributes() const;
            void setAttributes( const DirectiveAttributesPtr& value );
            XsString getValue() const;
            void setValue( const XsString& value );

        private:
            DirectiveAttributesPtr myAttributes;
            XsString myValue;
        };
        
        struct BeatRepeatAttributes;
        using BeatRepeatAttributesPtr = std::shared_ptr<BeatRepeatAttributes>;
        
        struct BeatRepeatAttributes : public AttributesInterface
        {
        public:
            BeatRepeatAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            PositiveInteger slashes;
            YesNo useDots;
            const bool hasType;
            bool hasSlashes;
            bool hasUseDots;
        };
        
        class BeatRepeat;
        using BeatRepeatPtr = std::shared_ptr<BeatRepeat>;
        using BeatRepeatUPtr = std::unique_ptr<BeatRepeat>;
        using BeatRepeatSet = std::vector<BeatRepeatPtr>;
        using BeatRepeatSetIter = BeatRepeatSet::iterator;
        using BeatRepeatSetIterConst = BeatRepeatSet::const_iterator;
        inline BeatRepeatPtr makeBeatRepeat() { return std::make_shared<BeatRepeat>(); }

        class BeatRepeat : public ElementInterface
        {
        public:
            BeatRepeat();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BeatRepeatAttributesPtr getAttributes() const;
            void setAttributes( const BeatRepeatAttributesPtr& value );

            /* _________ SlashType minOccurs = 1, maxOccurs = 1 _________ */
            SlashTypePtr getSlashType() const;
            void setSlashType( const SlashTypePtr& value );

            /* _________ SlashDot minOccurs = 0, maxOccurs = unbounded _________ */
            const SlashDotSet& getSlashDotSet() const;
            void addSlashDot( const SlashDotPtr& value );
            void removeSlashDot( const SlashDotSetIterConst& value );
            void clearSlashDotSet();

        private:
            BeatRepeatAttributesPtr myAttributes;
            SlashTypePtr mySlashType;
            SlashDotSet mySlashDotSet;
        };
        
        struct SlashAttributes;
        using SlashAttributesPtr = std::shared_ptr<SlashAttributes>;
        
        struct SlashAttributes : public AttributesInterface
        {
        public:
            SlashAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            YesNo useDots;
            YesNo useStems;
            const 	bool hasType;
            bool hasUseDots;
            bool hasUseStems;
        };
        
        class Slash;
        using SlashPtr = std::shared_ptr<Slash>;
        using SlashUPtr = std::unique_ptr<Slash>;
        using SlashSet = std::vector<SlashPtr>;
        using SlashSetIter = SlashSet::iterator;
        using SlashSetIterConst = SlashSet::const_iterator;
        inline SlashPtr makeSlash() { return std::make_shared<Slash>(); }

        class Slash : public ElementInterface
        {
        public:
            Slash();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            SlashAttributesPtr getAttributes() const;
            void setAttributes( const SlashAttributesPtr& value );

            /* _________ SlashType minOccurs = 1, maxOccurs = 1 _________ */
            SlashTypePtr getSlashType() const;
            void setSlashType( const SlashTypePtr& value );

            /* _________ SlashDot minOccurs = 0, maxOccurs = unbounded _________ */
            const SlashDotSet& getSlashDotSet() const;
            void addSlashDot( const SlashDotPtr& value );
            void removeSlashDot( const SlashDotSetIterConst& value );
            void clearSlashDotSet();

        private:
            SlashAttributesPtr myAttributes;
            SlashTypePtr mySlashType;
            SlashDotSet mySlashDotSet;
        };
        
        
        struct StaffTuningAttributes;
        using StaffTuningAttributesPtr = std::shared_ptr<StaffTuningAttributes>;
        
        struct StaffTuningAttributes : public AttributesInterface
        {
        public:
            StaffTuningAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StaffLine line;
            bool hasLine;
        };
        
        class StaffTuning;
        using StaffTuningPtr = std::shared_ptr<StaffTuning>;
        using StaffTuningUPtr = std::unique_ptr<StaffTuning>;
        using StaffTuningSet = std::vector<StaffTuningPtr>;
        using StaffTuningSetIter = StaffTuningSet::iterator;
        using StaffTuningSetIterConst = StaffTuningSet::const_iterator;
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

        private:
            StaffTuningAttributesPtr myAttributes;
            TuningStepPtr myTuningStep;
            TuningAlterPtr myTuningAlter;
            bool myHasTuningAlter;
            TuningOctavePtr myTuningOctave;
        };
        
        struct StaffDetailsAttributes;
        using StaffDetailsAttributesPtr = std::shared_ptr<StaffDetailsAttributes>;
        
        struct StaffDetailsAttributes : public AttributesInterface
        {
        public:
            StaffDetailsAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StaffNumber number;
            ShowFrets showFrets;
            YesNo printObject;
            YesNo printSpacing;
            bool hasNumber;
            bool hasShowFrets;
            bool hasPrintObject;
            bool hasPrintSpacing;
        };
        
        class StaffDetails;
        using StaffDetailsPtr = std::shared_ptr<StaffDetails>;
        using StaffDetailsUPtr = std::unique_ptr<StaffDetails>;
        using StaffDetailsSet = std::vector<StaffDetailsPtr>;
        using StaffDetailsSetIter = StaffDetailsSet::iterator;
        using StaffDetailsSetIterConst = StaffDetailsSet::const_iterator;
        inline StaffDetailsPtr makeStaffDetails() { return std::make_shared<StaffDetails>(); }

        class StaffDetails : public ElementInterface
        {
        public:
            StaffDetails();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StaffDetailsAttributesPtr getAttributes() const;
            void setAttributes( const StaffDetailsAttributesPtr& value );

            /* _________ StaffType minOccurs = 0, maxOccurs = 1 _________ */
            StaffTypePtr getStaffType() const;
            void setStaffType( const StaffTypePtr& value );
            bool getHasStaffType() const;
            void setHasStaffType( const bool value );

            /* _________ StaffLines minOccurs = 0, maxOccurs = 1 _________ */
            StaffLinesPtr getStaffLines() const;
            void setStaffLines( const StaffLinesPtr& value );
            bool getHasStaffLines() const;
            void setHasStaffLines( const bool value );

            /* _________ StaffTuning minOccurs = 0, maxOccurs = unbounded _________ */
            const StaffTuningSet& getStaffTuningSet() const;
            void addStaffTuning( const StaffTuningPtr& value );
            void removeStaffTuning( const StaffTuningSetIterConst& value );
            bool getHasStaffTuning() const;
            void setHasStaffTuning( const bool value );
            void clearStaffTuningSet();

            /* _________ Capo minOccurs = 0, maxOccurs = 1 _________ */
            CapoPtr getCapo() const;
            void setCapo( const CapoPtr& value );
            bool getHasCapo() const;
            void setHasCapo( const bool value );

            /* _________ StaffSize minOccurs = 0, maxOccurs = 1 _________ */
            StaffSizePtr getStaffSize() const;
            void setStaffSize( const StaffSizePtr& value );
            bool getHasStaffSize() const;
            void setHasStaffSize( const bool value );

        private:
            StaffDetailsAttributesPtr myAttributes;
            StaffTypePtr myStaffType;
            bool myHasStaffType;
            StaffLinesPtr myStaffLines;
            bool myHasStaffLines;
            StaffTuningSet myStaffTuningSet;
            CapoPtr myCapo;
            bool myHasCapo;
            StaffSizePtr myStaffSize;
            bool myHasStaffSize;
        };
        
        class MeasureStyleChoice;
        using MeasureStyleChoicePtr = std::shared_ptr<MeasureStyleChoice>;
        using MeasureStyleChoiceUPtr = std::unique_ptr<MeasureStyleChoice>;
        using MeasureStyleChoiceSet = std::vector<MeasureStyleChoicePtr>;
        using MeasureStyleChoiceSetIter = MeasureStyleChoiceSet::iterator;
        using MeasureStyleChoiceSetIterConst = MeasureStyleChoiceSet::const_iterator;
        inline MeasureStyleChoicePtr makeMeasureStyleChoice() { return std::make_shared<MeasureStyleChoice>(); }

        class MeasureStyleChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                multipleRest = 0,
                measureRepeat = 1,
                beatRepeat = 2,
                slash = 3
            };
            MeasureStyleChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Choice getChoice() const;
            void setChoice( const Choice value );
            MultipleRestPtr getMultipleRest() const;
            void setMultipleRest( const MultipleRestPtr& value );
            MeasureRepeatPtr getMeasureRepeat() const;
            void setMeasureRepeat( const MeasureRepeatPtr& value );
            BeatRepeatPtr getBeatRepeat() const;
            void setBeatRepeat( const BeatRepeatPtr& value );
            SlashPtr getSlash() const;
            void setSlash( const SlashPtr& value );

        private:
            Choice myChoice;
            MultipleRestPtr myMultipleRest;
            MeasureRepeatPtr myMeasureRepeat;
            BeatRepeatPtr myBeatRepeat;
            SlashPtr mySlash;
        };
        
        struct MeasureStyleAttributes;
        using MeasureStyleAttributesPtr = std::shared_ptr<MeasureStyleAttributes>;
        
        struct MeasureStyleAttributes : public AttributesInterface
        {
        public:
            MeasureStyleAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StaffNumber number;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            bool hasNumber;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
        };
        
        class MeasureStyle;
        using MeasureStylePtr = std::shared_ptr<MeasureStyle>;
        using MeasureStyleUPtr = std::unique_ptr<MeasureStyle>;
        using MeasureStyleSet = std::vector<MeasureStylePtr>;
        using MeasureStyleSetIter = MeasureStyleSet::iterator;
        using MeasureStyleSetIterConst = MeasureStyleSet::const_iterator;
        inline MeasureStylePtr makeMeasureStyle() { return std::make_shared<MeasureStyle>(); }

        class MeasureStyle : public ElementInterface
        {
        public:
            MeasureStyle();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MeasureStyleAttributesPtr getAttributes() const;
            void setAttributes( const MeasureStyleAttributesPtr& value );
            MeasureStyleChoicePtr getMeasureStyleChoice() const;
            void setMeasureStyleChoice( const MeasureStyleChoicePtr& value );

        private:
            MeasureStyleAttributesPtr myAttributes;
            MeasureStyleChoicePtr myChoice;
        };
        
        
        struct InterchangeableAttributes;
        using InterchangeableAttributesPtr = std::shared_ptr<InterchangeableAttributes>;
        
        struct InterchangeableAttributes : public AttributesInterface
        {
        public:
            InterchangeableAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TimeSymbol symbol;
            TimeSeparator separator;
            bool hasSymbol;
            bool hasSeparator;
        };
        
        class Interchangeable;
        using InterchangeablePtr = std::shared_ptr<Interchangeable>;
        using InterchangeableUPtr = std::unique_ptr<Interchangeable>;
        using InterchangeableSet = std::vector<InterchangeablePtr>;
        using InterchangeableSetIter = InterchangeableSet::iterator;
        using InterchangeableSetIterConst = InterchangeableSet::const_iterator;
        inline InterchangeablePtr makeInterchangeable() { return std::make_shared<Interchangeable>(); }

        class Interchangeable : public ElementInterface
        {
        public:
            Interchangeable();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            InterchangeableAttributesPtr getAttributes() const;
            void setAttributes( const InterchangeableAttributesPtr& value );

            /* _________ TimeRelation minOccurs = 0, maxOccurs = 1 _________ */
            TimeRelationPtr getTimeRelation() const;
            void setTimeRelation( const TimeRelationPtr& value );
            bool getHasTimeRelation() const;
            void setHasTimeRelation( const bool value );

            /* _________ Beats minOccurs = 1, maxOccurs = 1 _________ */
            BeatsPtr getBeats() const;
            void setBeats( const BeatsPtr& value );

            /* _________ BeatType minOccurs = 1, maxOccurs = 1 _________ */
            BeatTypePtr getBeatType() const;
            void setBeatType( const BeatTypePtr& value );

        private:
            InterchangeableAttributesPtr myAttributes;
            TimeRelationPtr myTimeRelation;
            bool myHasTimeRelation;
            BeatsPtr myBeats;
            BeatTypePtr myBeatType;
        };
        
        struct AccordionRegistrationAttributes;
        using AccordionRegistrationAttributesPtr = std::shared_ptr<AccordionRegistrationAttributes>;
        
        struct AccordionRegistrationAttributes : public AttributesInterface
        {
        public:
            AccordionRegistrationAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            LeftCenterRight halign;
            Valign valign;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasHalign;
            bool hasValign;
        };
        
        class AccordionRegistration;
        using AccordionRegistrationPtr = std::shared_ptr<AccordionRegistration>;
        using AccordionRegistrationUPtr = std::unique_ptr<AccordionRegistration>;
        using AccordionRegistrationSet = std::vector<AccordionRegistrationPtr>;
        using AccordionRegistrationSetIter = AccordionRegistrationSet::iterator;
        using AccordionRegistrationSetIterConst = AccordionRegistrationSet::const_iterator;
        inline AccordionRegistrationPtr makeAccordionRegistration() { return std::make_shared<AccordionRegistration>(); }

        class AccordionRegistration : public ElementInterface
        {
        public:
            AccordionRegistration();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            AccordionRegistrationAttributesPtr getAttributes() const;
            void setAttributes( const AccordionRegistrationAttributesPtr& value );

            /* _________ AccordionHigh minOccurs = 0, maxOccurs = 1 _________ */
            AccordionHighPtr getAccordionHigh() const;
            void setAccordionHigh( const AccordionHighPtr& value );
            bool getHasAccordionHigh() const;
            void setHasAccordionHigh( const bool value );

            /* _________ AccordionMiddle minOccurs = 0, maxOccurs = 1 _________ */
            AccordionMiddlePtr getAccordionMiddle() const;
            void setAccordionMiddle( const AccordionMiddlePtr& value );
            bool getHasAccordionMiddle() const;
            void setHasAccordionMiddle( const bool value );

            /* _________ AccordionLow minOccurs = 0, maxOccurs = 1 _________ */
            AccordionLowPtr getAccordionLow() const;
            void setAccordionLow( const AccordionLowPtr& value );
            bool getHasAccordionLow() const;
            void setHasAccordionLow( const bool value );

        private:
            AccordionRegistrationAttributesPtr myAttributes;
            AccordionHighPtr myAccordionHigh;
            bool myHasAccordionHigh;
            AccordionMiddlePtr myAccordionMiddle;
            bool myHasAccordionMiddle;
            AccordionLowPtr myAccordionLow;
            bool myHasAccordionLow;
        };
        
        class TimeSignature;
        using TimeSignatureGroupPtr = std::shared_ptr<TimeSignature>;
        using TimeSignatureUPtr = std::unique_ptr<TimeSignature>;
        using TimeSignatureSet = std::vector<TimeSignatureGroupPtr>;
        using TimeSignatureSetIter = TimeSignatureSet::iterator;
        using TimeSignatureSetIterConst = TimeSignatureSet::const_iterator;
        inline TimeSignatureGroupPtr makeTimeSignature() { return std::make_shared<TimeSignature>(); }

        class TimeSignature : public ElementInterface
        {
        public:
            TimeSignature();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BeatsPtr getBeats() const;
            void setBeats( const BeatsPtr& value );
            BeatTypePtr getBeatType() const;
            void setBeatType( const BeatTypePtr& value );
            InterchangeablePtr getInterchangeable() const;
            void setInterchangeable( const InterchangeablePtr& value );
            bool getHasInterchangeable() const;
            void setHasInterchangeable( const bool value );

        private:
            BeatsPtr myBeats;
            BeatTypePtr myBeatType;
            InterchangeablePtr myInterchangeable;
            bool myHasInterchangeable;
        };
        
        class TimeChoice;
        using TimeChoicePtr = std::shared_ptr<TimeChoice>;
        using TimeChoiceUPtr = std::unique_ptr<TimeChoice>;
        using TimeChoiceSet = std::vector<TimeChoicePtr>;
        using TimeChoiceSetIter = TimeChoiceSet::iterator;
        using TimeChoiceSetIterConst = TimeChoiceSet::const_iterator;
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
            TimeSignatureGroupPtr getTimeSignature() const;
            void setTimeSignature( const TimeSignatureGroupPtr& value );
            SenzaMisuraPtr getSenzaMisura() const;
            void setSenzaMisura( const SenzaMisuraPtr& value );

        private:
            Choice myChoice;
            TimeSignatureGroupPtr myTimeSignature;
            SenzaMisuraPtr mySenzaMisura;
        };
        
        struct TimeAttributes;
        using TimeAttributesPtr = std::shared_ptr<TimeAttributes>;
        
        struct TimeAttributes : public AttributesInterface
        {
        public:
            TimeAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StaffNumber number;
            TimeSymbol symbol;
            TimeSeparator separator;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            LeftCenterRight halign;
            Valign valign;
            YesNo printObject;
            bool hasNumber;
            bool hasSymbol;
            bool hasSeparator;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasHalign;
            bool hasValign;
            bool hasPrintObject;
        };
        
        class Time;
        using TimePtr = std::shared_ptr<Time>;
        using TimeUPtr = std::unique_ptr<Time>;
        using TimeSet = std::vector<TimePtr>;
        using TimeSetIter = TimeSet::iterator;
        using TimeSetIterConst = TimeSet::const_iterator;
        inline TimePtr makeTime() { return std::make_shared<Time>(); }

        class Time : public ElementInterface
        {
        public:
            Time();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TimeAttributesPtr getAttributes() const;
            void setAttributes( const TimeAttributesPtr& value );
            TimeChoicePtr getTimeChoice() const;
            void setTimeChoice( const TimeChoicePtr& value );

        private:
            TimeAttributesPtr myAttributes;
            TimeChoicePtr myChoice;
        };
        
        struct StringAttributes;
        using StringAttributesPtr = std::shared_ptr<StringAttributes>;
        
        struct StringAttributes : public AttributesInterface
        {
        public:
            StringAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            AboveBelow placement;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasPlacement;
        };
        
        class String;
        using StringPtr = std::shared_ptr<String>;
        using StringUPtr = std::unique_ptr<String>;
        using StringSet = std::vector<StringPtr>;
        using StringSetIter = StringSet::iterator;
        using StringSetIterConst = StringSet::const_iterator;
        inline StringPtr makeString() { return std::make_shared<String>(); }
        inline StringPtr makeString( const StringNumber& value ) { return std::make_shared<String>( value ); }
		inline StringPtr makeString( StringNumber&& value ) { return std::make_shared<String>( std::move( value ) ); }
        

        class String : public ElementInterface
        {
        public:
            String();
            String( const StringNumber& value );

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StringAttributesPtr getAttributes() const;
            void setAttributes( const StringAttributesPtr& value );
            StringNumber getValue() const;
            void setValue( const StringNumber& value );

        private:
            StringAttributesPtr myAttributes;
            StringNumber myValue;
        };
        
        class FrameNote;
        using FrameNotePtr = std::shared_ptr<FrameNote>;
        using FrameNoteUPtr = std::unique_ptr<FrameNote>;
        using FrameNoteSet = std::vector<FrameNotePtr>;
        using FrameNoteSetIter = FrameNoteSet::iterator;
        using FrameNoteSetIterConst = FrameNoteSet::const_iterator;
        inline FrameNotePtr makeFrameNote() { return std::make_shared<FrameNote>(); }

        class FrameNote : public ElementInterface
        {
        public:
            FrameNote();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ String minOccurs = 1, maxOccurs = 1 _________ */
            StringPtr getString() const;
            void setString( const StringPtr& value );

            /* _________ Fret minOccurs = 1, maxOccurs = 1 _________ */
            FretPtr getFret() const;
            void setFret( const FretPtr& value );

            /* _________ Fingering minOccurs = 0, maxOccurs = 1 _________ */
            FingeringPtr getFingering() const;
            void setFingering( const FingeringPtr& value );
            bool getHasFingering() const;
            void setHasFingering( const bool value );

            /* _________ Barre minOccurs = 0, maxOccurs = 1 _________ */
            BarrePtr getBarre() const;
            void setBarre( const BarrePtr& value );
            bool getHasBarre() const;
            void setHasBarre( const bool value );

        private:
            StringPtr myString;
            FretPtr myFret;
            FingeringPtr myFingering;
            bool myHasFingering;
            BarrePtr myBarre;
            bool myHasBarre;
        };
        
        struct FrameAttributes;
        using FrameAttributesPtr = std::shared_ptr<FrameAttributes>;
        
        struct FrameAttributes : public AttributesInterface
        {
        public:
            FrameAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            Color color;
            LeftCenterRight halign;
            ValignImage valign;
            TenthsValue height;
            TenthsValue width;
            XsToken unplayed;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasColor;
            bool hasHalign;
            bool hasValign;
            bool hasHeight;
            bool hasWidth;
            bool hasUnplayed;
        };
        
        class Frame;
        using FramePtr = std::shared_ptr<Frame>;
        using FrameUPtr = std::unique_ptr<Frame>;
        using FrameSet = std::vector<FramePtr>;
        using FrameSetIter = FrameSet::iterator;
        using FrameSetIterConst = FrameSet::const_iterator;
        inline FramePtr makeFrame() { return std::make_shared<Frame>(); }

        class Frame : public ElementInterface
        {
        public:
            Frame();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FrameAttributesPtr getAttributes() const;
            void setAttributes( const FrameAttributesPtr& value );

            /* _________ FrameStrings minOccurs = 1, maxOccurs = 1 _________ */
            FrameStringsPtr getFrameStrings() const;
            void setFrameStrings( const FrameStringsPtr& value );

            /* _________ FrameFrets minOccurs = 1, maxOccurs = 1 _________ */
            FrameFretsPtr getFrameFrets() const;
            void setFrameFrets( const FrameFretsPtr& value );

            /* _________ FirstFret minOccurs = 0, maxOccurs = 1 _________ */
            FirstFretPtr getFirstFret() const;
            void setFirstFret( const FirstFretPtr& value );
            bool getHasFirstFret() const;
            void setHasFirstFret( const bool value );

            /* _________ FrameNote minOccurs = 1, maxOccurs = unbounded _________ */
            const FrameNoteSet& getFrameNoteSet() const;
            void addFrameNote( const FrameNotePtr& value );
            void removeFrameNote( const FrameNoteSetIterConst& value );
            void clearFrameNoteSet();
            FrameNotePtr getFrameNote( const FrameNoteSetIterConst& setIterator ) const;

        private:
            FrameAttributesPtr myAttributes;
            FrameStringsPtr myFrameStrings;
            FrameFretsPtr myFrameFrets;
            FirstFretPtr myFirstFret;
            bool myHasFirstFret;
            FrameNoteSet myFrameNoteSet;
        };
        
        class PedalTuning;
        using PedalTuningPtr = std::shared_ptr<PedalTuning>;
        using PedalTuningUPtr = std::unique_ptr<PedalTuning>;
        using PedalTuningSet = std::vector<PedalTuningPtr>;
        using PedalTuningSetIter = PedalTuningSet::iterator;
        using PedalTuningSetIterConst = PedalTuningSet::const_iterator;
        inline PedalTuningPtr makePedalTuning() { return std::make_shared<PedalTuning>(); }

        class PedalTuning : public ElementInterface
        {
        public:
            PedalTuning();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ PedalStep minOccurs = 1, maxOccurs = 1 _________ */
            PedalStepPtr getPedalStep() const;
            void setPedalStep( const PedalStepPtr& value );

            /* _________ PedalAlter minOccurs = 1, maxOccurs = 1 _________ */
            PedalAlterPtr getPedalAlter() const;
            void setPedalAlter( const PedalAlterPtr& value );

        private:
            PedalStepPtr myPedalStep;
            PedalAlterPtr myPedalAlter;
        };
        struct HarpPedalsAttributes;
        using HarpPedalsAttributesPtr = std::shared_ptr<HarpPedalsAttributes>;
        
        struct HarpPedalsAttributes : public AttributesInterface
        {
        public:
            HarpPedalsAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            LeftCenterRight halign;
            Valign valign;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasHalign;
            bool hasValign;
        };
        
        class HarpPedals;
        using HarpPedalsPtr = std::shared_ptr<HarpPedals>;
        using HarpPedalsUPtr = std::unique_ptr<HarpPedals>;
        using HarpPedalsSet = std::vector<HarpPedalsPtr>;
        using HarpPedalsSetIter = HarpPedalsSet::iterator;
        using HarpPedalsSetIterConst = HarpPedalsSet::const_iterator;
        inline HarpPedalsPtr makeHarpPedals() { return std::make_shared<HarpPedals>(); }

        class HarpPedals : public ElementInterface
        {
        public:
            HarpPedals();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HarpPedalsAttributesPtr getAttributes() const;
            void setAttributes( const HarpPedalsAttributesPtr& value );

            /* _________ PedalTuning minOccurs = 1, maxOccurs = unbounded _________ */
            const PedalTuningSet& getPedalTuningSet() const;
            void addPedalTuning( const PedalTuningPtr& value );
            void removePedalTuning( const PedalTuningSetIterConst& value );
            void clearPedalTuningSet();
            PedalTuningPtr getPedalTuning( const PedalTuningSetIterConst& setIterator ) const;

        private:
            HarpPedalsAttributesPtr myAttributes;
            PedalTuningSet myPedalTuningSet;
        };
        
        
        
        class TimeModificationNormalTypeNormalDot;
        using TimeModificationNormalTypeNormalDotPtr = std::shared_ptr<TimeModificationNormalTypeNormalDot>;
        using TimeModificationNormalTypeNormalDotUPtr = std::unique_ptr<TimeModificationNormalTypeNormalDot>;
        using TimeModificationNormalTypeNormalDotSet = std::vector<TimeModificationNormalTypeNormalDotPtr>;
        using TimeModificationNormalTypeNormalDotSetIter = TimeModificationNormalTypeNormalDotSet::iterator;
        using TimeModificationNormalTypeNormalDotSetIterConst = TimeModificationNormalTypeNormalDotSet::const_iterator;
        inline TimeModificationNormalTypeNormalDotPtr makeTimeModificationNormalTypeNormalDot() { return std::make_shared<TimeModificationNormalTypeNormalDot>(); }

        class TimeModificationNormalTypeNormalDot : public ElementInterface
        {
        public:
            TimeModificationNormalTypeNormalDot();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ NormalType minOccurs = 1, maxOccurs = 1 _________ */
            NormalTypePtr getNormalType() const;
            void setNormalType( const NormalTypePtr& value );

            /* _________ NormalDot minOccurs = 0, maxOccurs = unbounded _________ */
            const NormalDotSet& getNormalDotSet() const;
            void addNormalDot( const NormalDotPtr& value );
            void removeNormalDot( const NormalDotSetIterConst& value );
            void clearNormalDotSet();
            NormalDotPtr getNormalDot( const NormalDotSetIterConst& setIterator ) const;

        private:
            NormalTypePtr myNormalType;
            NormalDotSet myNormalDotSet;
        };
        
        struct MetronomeTupletAttributes;
        using MetronomeTupletAttributesPtr = std::shared_ptr<MetronomeTupletAttributes>;
        
        struct MetronomeTupletAttributes : public AttributesInterface
        {
        public:
            MetronomeTupletAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            YesNo bracket;
            ShowTuplet showNumber;
            const 	bool hasType;
            bool hasBracket;
            bool hasShowNumber;
        };
        
        class MetronomeTuplet;
        using MetronomeTupletPtr = std::shared_ptr<MetronomeTuplet>;
        using MetronomeTupletUPtr = std::unique_ptr<MetronomeTuplet>;
        using MetronomeTupletSet = std::vector<MetronomeTupletPtr>;
        using MetronomeTupletSetIter = MetronomeTupletSet::iterator;
        using MetronomeTupletSetIterConst = MetronomeTupletSet::const_iterator;
        inline MetronomeTupletPtr makeMetronomeTuplet() { return std::make_shared<MetronomeTuplet>(); }
        

        class MetronomeTuplet : public ElementInterface
        {
        public:
            MetronomeTuplet();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MetronomeTupletAttributesPtr getAttributes() const;
            void setAttributes( const MetronomeTupletAttributesPtr& value );

            /* _________ ActualNotes minOccurs = 1, maxOccurs = 1 _________ */
            ActualNotesPtr getActualNotes() const;
            void setActualNotes( const ActualNotesPtr& value );

            /* _________ NormalNotes minOccurs = 1, maxOccurs = 1 _________ */
            NormalNotesPtr getNormalNotes() const;
            void setNormalNotes( const NormalNotesPtr& value );

            /* _________ TimeModificationNormalTypeNormalDot minOccurs = 0, maxOccurs = 1 _________ */
            TimeModificationNormalTypeNormalDotPtr getTimeModificationNormalTypeNormalDot() const;
            void setTimeModificationNormalTypeNormalDot( const TimeModificationNormalTypeNormalDotPtr& value );
            bool getHasTimeModificationNormalTypeNormalDot() const;
            void setHasTimeModificationNormalTypeNormalDot( const bool value );

        private:
            MetronomeTupletAttributesPtr myAttributes;
            ActualNotesPtr myActualNotes;
            NormalNotesPtr myNormalNotes;
            TimeModificationNormalTypeNormalDotPtr myTimeModificationNormalTypeNormalDot;
            bool myHasTimeModificationNormalTypeNormalDot;
        };
        
        class MetronomeNote;
        using MetronomeNotePtr = std::shared_ptr<MetronomeNote>;
        using MetronomeNoteUPtr = std::unique_ptr<MetronomeNote>;
        using MetronomeNoteSet = std::vector<MetronomeNotePtr>;
        using MetronomeNoteSetIter = MetronomeNoteSet::iterator;
        using MetronomeNoteSetIterConst = MetronomeNoteSet::const_iterator;
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
            MetronomeTypePtr myMetronomeType;
            MetronomeDotSet myMetronomeDotSet;
            MetronomeBeamSet myMetronomeBeamSet;
            MetronomeTupletPtr myMetronomeTuplet;
            bool myHasMetronomeTuplet;
        };
        
        class BeatUnitGroup;
        using BeatUnitGroupPtr = std::shared_ptr<BeatUnitGroup>;
        using BeatUnitGroupUPtr = std::unique_ptr<BeatUnitGroup>;
        using BeatUnitGroupSet = std::vector<BeatUnitGroupPtr>;
        using BeatUnitGroupSetIter = BeatUnitGroupSet::iterator;
        using BeatUnitGroupSetIterConst = BeatUnitGroupSet::const_iterator;
        inline BeatUnitGroupPtr makeBeatUnitGroup() { return std::make_shared<BeatUnitGroup>(); }

        class BeatUnitGroup : public ElementInterface
        {
        public:
            BeatUnitGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ BeatUnit minOccurs = 1, maxOccurs = 1 _________ */
            BeatUnitPtr getBeatUnit() const;
            void setBeatUnit( const BeatUnitPtr& value );

            /* _________ BeatUnitDot minOccurs = 0, maxOccurs = unbounded _________ */
            const BeatUnitDotSet& getBeatUnitDotSet() const;
            void addBeatUnitDot( const BeatUnitDotPtr& value );
            void removeBeatUnitDot( const BeatUnitDotSetIterConst& setIterator );
            void clearBeatUnitDotSet();

        private:
            BeatUnitPtr myBeatUnit;
            BeatUnitDotSet myBeatUnitDotSet;
        };
        
        class MetronomeRelationGroup;
        using MetronomeRelationGroupPtr = std::shared_ptr<MetronomeRelationGroup>;
        using MetronomeRelationGroupUPtr = std::unique_ptr<MetronomeRelationGroup>;
        using MetronomeRelationGroupSet = std::vector<MetronomeRelationGroupPtr>;
        using MetronomeRelationGroupSetIter = MetronomeRelationGroupSet::iterator;
        using MetronomeRelationGroupSetIterConst = MetronomeRelationGroupSet::const_iterator;
        inline MetronomeRelationGroupPtr makeMetronomeRelationGroup() { return std::make_shared<MetronomeRelationGroup>(); }

        class MetronomeRelationGroup : public ElementInterface
        {
        public:
            MetronomeRelationGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ MetronomeRelation minOccurs = 1, maxOccurs = 1 _________ */
            MetronomeRelationPtr getMetronomeRelation() const;
            void setMetronomeRelation( const MetronomeRelationPtr& value );

            /* _________ MetronomeNote minOccurs = 1, maxOccurs = 1 _________ */
            MetronomeNotePtr getMetronomeNote() const;
            void setMetronomeNote( const MetronomeNotePtr& value );

        private:
            MetronomeRelationPtr myMetronomeRelation;
            MetronomeNotePtr myMetronomeNote;
        };
        
        class NoteRelationNote;
        using NoteRelationNotePtr = std::shared_ptr<NoteRelationNote>;
        using NoteRelationNoteUPtr = std::unique_ptr<NoteRelationNote>;
        using NoteRelationNoteSet = std::vector<NoteRelationNotePtr>;
        using NoteRelationNoteSetIter = NoteRelationNoteSet::iterator;
        using NoteRelationNoteSetIterConst = NoteRelationNoteSet::const_iterator;
        inline NoteRelationNotePtr makeNoteRelationNote() { return std::make_shared<NoteRelationNote>(); }

        class NoteRelationNote : public ElementInterface
        {
        public:
            NoteRelationNote();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ MetronomeNote minOccurs = 1, maxOccurs = unbounded _________ */
            const MetronomeNoteSet& getMetronomeNoteSet() const;
            void addMetronomeNote( const MetronomeNotePtr& value );
            void removeMetronomeNote( const MetronomeNoteSetIterConst& setIterator );
            void clearMetronomeNoteSet();

            /* _________ MetronomeRelationGroup minOccurs = 0, maxOccurs = 1 _________ */
            MetronomeRelationGroupPtr getMetronomeRelationGroup() const;
            void setMetronomeRelationGroup( const MetronomeRelationGroupPtr& value );
            bool getHasMetronomeRelationGroup() const;
            void setHasMetronomeRelationGroup( const bool value );

        private:
            MetronomeNoteSet myMetronomeNoteSet;
            MetronomeRelationGroupPtr myMetronomeRelationGroup;
            bool myHasMetronomeRelationGroup;
        };
        
        class PerMinuteOrBeatUnitChoice;
        using PerMinuteOrBeatUnitChoicePtr = std::shared_ptr<PerMinuteOrBeatUnitChoice>;
        using PerMinuteOrBeatUnitChoiceUPtr = std::unique_ptr<PerMinuteOrBeatUnitChoice>;
        using PerMinuteOrBeatUnitChoiceSet = std::vector<PerMinuteOrBeatUnitChoicePtr>;
        using PerMinuteOrBeatUnitChoiceSetIter = PerMinuteOrBeatUnitChoiceSet::iterator;
        using PerMinuteOrBeatUnitChoiceSetIterConst = PerMinuteOrBeatUnitChoiceSet::const_iterator;
        inline PerMinuteOrBeatUnitChoicePtr makePerMinuteOrBeatUnitChoice() { return std::make_shared<PerMinuteOrBeatUnitChoice>(); }

        class PerMinuteOrBeatUnitChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                perMinute = 0,
                beatUnitGroup = 1
            };
            PerMinuteOrBeatUnitChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PerMinuteOrBeatUnitChoice::Choice getChoice() const;
            void setChoice( const PerMinuteOrBeatUnitChoice::Choice value );
            PerMinutePtr getPerMinute() const;
            void setPerMinute( const PerMinutePtr& value );
            BeatUnitGroupPtr getBeatUnitGroup() const;
            void setBeatUnitGroup( const BeatUnitGroupPtr& value );

        private:
            Choice myChoice;
            PerMinutePtr myPerMinute;
            BeatUnitGroupPtr myBeatUnitGroup;
        };
        
        class BeatUnitPer;
        using BeatUnitPerPtr = std::shared_ptr<BeatUnitPer>;
        using BeatUnitPerUPtr = std::unique_ptr<BeatUnitPer>;
        using BeatUnitPerSet = std::vector<BeatUnitPerPtr>;
        using BeatUnitPerSetIter = BeatUnitPerSet::iterator;
        using BeatUnitPerSetIterConst = BeatUnitPerSet::const_iterator;
        inline BeatUnitPerPtr makeBeatUnitPer() { return std::make_shared<BeatUnitPer>(); }

        class BeatUnitPer : public ElementInterface
        {
        public:
            BeatUnitPer();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BeatUnitGroupPtr getBeatUnitGroup() const;
            void setBeatUnitGroup( const BeatUnitGroupPtr& value );
            PerMinuteOrBeatUnitChoicePtr getPerMinuteOrBeatUnitChoice() const;
            void setPerMinuteOtBeatUnitChoice( const PerMinuteOrBeatUnitChoicePtr& value );

        private:
            BeatUnitGroupPtr myBeatUnitGroup;
            PerMinuteOrBeatUnitChoicePtr myPerMinuteOrBeatUnitChoice;
        };
        
        
        
        class BeatUnitPerOrNoteRelationNoteChoice;
        using BeatUnitPerOrNoteRelationNoteChoicePtr = std::shared_ptr<BeatUnitPerOrNoteRelationNoteChoice>;
        using BeatUnitPerOrNoteRelationNoteChoiceUPtr = std::unique_ptr<BeatUnitPerOrNoteRelationNoteChoice>;
        using BeatUnitPerOrNoteRelationNoteChoiceSet = std::vector<BeatUnitPerOrNoteRelationNoteChoicePtr>;
        using BeatUnitPerOrNoteRelationNoteChoiceSetIter = BeatUnitPerOrNoteRelationNoteChoiceSet::iterator;
        using BeatUnitPerOrNoteRelationNoteChoiceSetIterConst = BeatUnitPerOrNoteRelationNoteChoiceSet::const_iterator;
        inline BeatUnitPerOrNoteRelationNoteChoicePtr makeBeatUnitPerOrNoteRelationNoteChoice() { return std::make_shared<BeatUnitPerOrNoteRelationNoteChoice>(); }

        class BeatUnitPerOrNoteRelationNoteChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                beatUnitPer = 0,
                noteRelationNote = 1
            };
            BeatUnitPerOrNoteRelationNoteChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BeatUnitPerOrNoteRelationNoteChoice::Choice getChoice() const;
            void setChoice( const BeatUnitPerOrNoteRelationNoteChoice::Choice value );
            BeatUnitPerPtr getBeatUnitPer() const;
            void setBeatUnitPer( const BeatUnitPerPtr& value );
            NoteRelationNotePtr getNoteRelationNote() const;
            void setNoteRelationNote( const NoteRelationNotePtr& value );

        private:
            Choice myChoice;
            BeatUnitPerPtr myBeatUnitPer;
            NoteRelationNotePtr myNoteRelationNote;
        };
        
        struct MetronomeAttributes;
        using MetronomeAttributesPtr = std::shared_ptr<MetronomeAttributes>;
        
        struct MetronomeAttributes : public AttributesInterface
        {
        public:
            MetronomeAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            LeftCenterRight halign;
            Valign valign;
            LeftCenterRight justify;
            YesNo parentheses;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasHalign;
            bool hasValign;
            bool hasJustify;
            bool hasParentheses;
        };
        
        class Metronome;
        using MetronomePtr = std::shared_ptr<Metronome>;
        using MetronomeUPtr = std::unique_ptr<Metronome>;
        using MetronomeSet = std::vector<MetronomePtr>;
        using MetronomeSetIter = MetronomeSet::iterator;
        using MetronomeSetIterConst = MetronomeSet::const_iterator;
        inline MetronomePtr makeMetronome() { return std::make_shared<Metronome>(); }

        class Metronome : public ElementInterface
        {
        public:
            Metronome();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MetronomeAttributesPtr getAttributes() const;
            void setAttributes( const MetronomeAttributesPtr& value );
            BeatUnitPerOrNoteRelationNoteChoicePtr getBeatUnitPerOrNoteRelationNoteChoice() const;
            void setBeatUnitPerOrNoteRelationNoteChoice( const BeatUnitPerOrNoteRelationNoteChoicePtr& value );

        private:
            MetronomeAttributesPtr myAttributes;
            BeatUnitPerOrNoteRelationNoteChoicePtr myBeatUnitPerOrNoteRelationNoteChoice;
        };
        
        struct StickAttributes;
        using StickAttributesPtr = std::shared_ptr<StickAttributes>;
        
        struct StickAttributes : public AttributesInterface
        {
        public:
            StickAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TipDirection tip;
            bool hasTip;
        };
        
        class Stick;
        using StickPtr = std::shared_ptr<Stick>;
        using StickUPtr = std::unique_ptr<Stick>;
        using StickSet = std::vector<StickPtr>;
        using StickSetIter = StickSet::iterator;
        using StickSetIterConst = StickSet::const_iterator;
        inline StickPtr makeStick() { return std::make_shared<Stick>(); }

        class Stick : public ElementInterface
        {
        public:
            Stick();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StickAttributesPtr getAttributes() const;
            void setAttributes( const StickAttributesPtr& value );

            /* _________ StickType minOccurs = 1, maxOccurs = 1 _________ */
            StickTypePtr getStickType() const;
            void setStickType( const StickTypePtr& value );

            /* _________ StickMaterial minOccurs = 1, maxOccurs = 1 _________ */
            StickMaterialPtr getStickMaterial() const;
            void setStickMaterial( const StickMaterialPtr& value );

        private:
            StickAttributesPtr myAttributes;
            StickTypePtr myStickType;
            StickMaterialPtr myStickMaterial;
        };
        
        class MeasureLayout;
        using MeasureLayoutPtr = std::shared_ptr<MeasureLayout>;
        using MeasureLayoutUPtr = std::unique_ptr<MeasureLayout>;
        using MeasureLayoutSet = std::vector<MeasureLayoutPtr>;
        using MeasureLayoutSetIter = MeasureLayoutSet::iterator;
        using MeasureLayoutSetIterConst = MeasureLayoutSet::const_iterator;
        inline MeasureLayoutPtr makeMeasureLayout() { return std::make_shared<MeasureLayout>(); }

        class MeasureLayout : public ElementInterface
        {
        public:
            MeasureLayout();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ MeasureDistance minOccurs = 0, maxOccurs = 1 _________ */
            MeasureDistancePtr getMeasureDistance() const;
            void setMeasureDistance( const MeasureDistancePtr& value );
            bool getHasMeasureDistance() const;
            void setHasMeasureDistance( const bool value );

        private:
            MeasureDistancePtr myMeasureDistance;
            bool myHasMeasureDistance;
        };
        
        class DisplayTextOrAccidentalText;
        using DisplayTextOrAccidentalTextPtr = std::shared_ptr<DisplayTextOrAccidentalText>;
        using DisplayTextOrAccidentalTextUPtr = std::unique_ptr<DisplayTextOrAccidentalText>;
        using DisplayTextOrAccidentalTextSet = std::vector<DisplayTextOrAccidentalTextPtr>;
        using DisplayTextOrAccidentalTextSetIter = DisplayTextOrAccidentalTextSet::iterator;
        using DisplayTextOrAccidentalTextSetIterConst = DisplayTextOrAccidentalTextSet::const_iterator;
        inline DisplayTextOrAccidentalTextPtr makeDisplayTextOrAccidentalText() { return std::make_shared<DisplayTextOrAccidentalText>(); }

        class DisplayTextOrAccidentalText : public ElementInterface
        {
        public:
            enum class Choice
            {
                displayText = 1,
                accidentalText = 2
            };
            DisplayTextOrAccidentalText();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DisplayTextOrAccidentalText::Choice getChoice() const;
            void setChoice( const DisplayTextOrAccidentalText::Choice value );
            DisplayTextPtr getDisplayText() const;
            void setDisplayText( const DisplayTextPtr& value );
            AccidentalTextPtr getAccidentalText() const;
            void setAccidentalText( const AccidentalTextPtr& value );

        private:
            Choice myChoice;
            DisplayTextPtr myDisplayText;
            AccidentalTextPtr myAccidentalText;
        };
        
        struct PartNameDisplayAttributes;
        using PartNameDisplayAttributesPtr = std::shared_ptr<PartNameDisplayAttributes>;
        
        struct PartNameDisplayAttributes : public AttributesInterface
        {
        public:
            PartNameDisplayAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo printObject;
            bool hasPrintObject;
        };
        
        class PartNameDisplay;
        using PartNameDisplayPtr = std::shared_ptr<PartNameDisplay>;
        using PartNameDisplayUPtr = std::unique_ptr<PartNameDisplay>;
        using PartNameDisplaySet = std::vector<PartNameDisplayPtr>;
        using PartNameDisplaySetIter = PartNameDisplaySet::iterator;
        using PartNameDisplaySetIterConst = PartNameDisplaySet::const_iterator;
        inline PartNameDisplayPtr makePartNameDisplay() { return std::make_shared<PartNameDisplay>(); }

        class PartNameDisplay : public ElementInterface
        {
        public:
            PartNameDisplay();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PartNameDisplayAttributesPtr getAttributes() const;
            void setAttributes( const PartNameDisplayAttributesPtr& value );

            /* _________ DisplayTextOrAccidentalText minOccurs = 0, maxOccurs = unbounded _________ */
            const DisplayTextOrAccidentalTextSet& getDisplayTextOrAccidentalText() const;
            void addDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextPtr& value );
            void removeDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextSetIterConst& setIterator );
            void clearDisplayTextOrAccidentalTextSet();
            DisplayTextOrAccidentalTextPtr getDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextSetIterConst& setIterator ) const;

        private:
            PartNameDisplayAttributesPtr myAttributes;
            DisplayTextOrAccidentalTextSet myDisplayTextOrAccidentalTextSet;
        };
        
        class PercussionChoice;
        using PercussionChoicePtr = std::shared_ptr<PercussionChoice>;
        using PercussionChoiceUPtr = std::unique_ptr<PercussionChoice>;
        using PercussionChoiceSet = std::vector<PercussionChoicePtr>;
        using PercussionChoiceSetIter = PercussionChoiceSet::iterator;
        using PercussionChoiceSetIterConst = PercussionChoiceSet::const_iterator;
        inline PercussionChoicePtr makePercussionChoice() { return std::make_shared<PercussionChoice>(); }

        class PercussionChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                glass = 1,
                metal = 2,
                wood = 3,
                pitched = 4,
                membrane = 5,
                effect = 6,
                timpani = 7,
                beater = 8,
                stick = 9,
                stickType = 10,
                stickMaterial = 11,
                stickLocation = 12,
                otherPercussion = 13
            };
            PercussionChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PercussionChoice::Choice getChoice() const;
            void setChoice( const PercussionChoice::Choice value );

            /* _________ Glass minOccurs = 1, maxOccurs = 1 _________ */
            GlassPtr getGlass() const;
            void setGlass( const GlassPtr& value );

            /* _________ Metal minOccurs = 1, maxOccurs = 1 _________ */
            MetalPtr getMetal() const;
            void setMetal( const MetalPtr& value );

            /* _________ Wood minOccurs = 1, maxOccurs = 1 _________ */
            WoodPtr getWood() const;
            void setWood( const WoodPtr& value );

            /* _________ Pitched minOccurs = 1, maxOccurs = 1 _________ */
            PitchedPtr getPitched() const;
            void setPitched( const PitchedPtr& value );

            /* _________ Membrane minOccurs = 1, maxOccurs = 1 _________ */
            MembranePtr getMembrane() const;
            void setMembrane( const MembranePtr& value );

            /* _________ Effect minOccurs = 1, maxOccurs = 1 _________ */
            EffectPtr getEffect() const;
            void setEffect( const EffectPtr& value );

            /* _________ Timpani minOccurs = 1, maxOccurs = 1 _________ */
            TimpaniPtr getTimpani() const;
            void setTimpani( const TimpaniPtr& value );

            /* _________ Beater minOccurs = 1, maxOccurs = 1 _________ */
            BeaterPtr getBeater() const;
            void setBeater( const BeaterPtr& value );

            /* _________ Stick minOccurs = 1, maxOccurs = 1 _________ */
            StickPtr getStick() const;
            void setStick( const StickPtr& value );

            /* _________ StickType minOccurs = 1, maxOccurs = 1 _________ */
            StickTypePtr getStickType() const;
            void setStickType( const StickTypePtr& value );

            /* _________ StickMaterial minOccurs = 1, maxOccurs = 1 _________ */
            StickMaterialPtr getStickMaterial() const;
            void setStickMaterial( const StickMaterialPtr& value );

            /* _________ StickLocation minOccurs = 1, maxOccurs = 1 _________ */
            StickLocationPtr getStickLocation() const;
            void setStickLocation( const StickLocationPtr& value );

            /* _________ OtherPercussion minOccurs = 1, maxOccurs = 1 _________ */
            OtherPercussionPtr getOtherPercussion() const;
            void setOtherPercussion( const OtherPercussionPtr& value );

        private:
            Choice myChoice;
            GlassPtr myGlass;
            MetalPtr myMetal;
            WoodPtr myWood;
            PitchedPtr myPitched;
            MembranePtr myMembrane;
            EffectPtr myEffect;
            TimpaniPtr myTimpani;
            BeaterPtr myBeater;
            StickPtr myStick;
            StickTypePtr myStickType;
            StickMaterialPtr myStickMaterial;
            StickLocationPtr myStickLocation;
            OtherPercussionPtr myOtherPercussion;
        };
        
        
        struct PercussionAttributes;
        using PercussionAttributesPtr = std::shared_ptr<PercussionAttributes>;
        
        struct PercussionAttributes : public AttributesInterface
        {
        public:
            PercussionAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            LeftCenterRight halign;
            Valign valign;
            EnclosureShape enclosure;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasHalign;
            bool hasValign;
            bool hasEnclosure;
        };
        
        class Percussion;
        using PercussionPtr = std::shared_ptr<Percussion>;
        using PercussionUPtr = std::unique_ptr<Percussion>;
        using PercussionSet = std::vector<PercussionPtr>;
        using PercussionSetIter = PercussionSet::iterator;
        using PercussionSetIterConst = PercussionSet::const_iterator;
        inline PercussionPtr makePercussion() { return std::make_shared<Percussion>(); }

        class Percussion : public ElementInterface
        {
        public:
            Percussion();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PercussionAttributesPtr getAttributes() const;
            void setAttributes( const PercussionAttributesPtr& value );
            PercussionChoicePtr getPercussionChoice() const;
            void setPercussionChoice( const PercussionChoicePtr& value );

        private:
            PercussionAttributesPtr myAttributes;
            PercussionChoicePtr myChoice;
        };
        
        
        struct AccordAttributes;
        using AccordAttributesPtr = std::shared_ptr<AccordAttributes>;
        
        struct AccordAttributes : public AttributesInterface
        {
        public:
            AccordAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StringNumber string;
            bool hasString;
        };
        
        class Accord;
        using AccordPtr = std::shared_ptr<Accord>;
        using AccordUPtr = std::unique_ptr<Accord>;
        using AccordSet = std::vector<AccordPtr>;
        using AccordSetIter = AccordSet::iterator;
        using AccordSetIterConst = AccordSet::const_iterator;
        inline AccordPtr makeAccord() { return std::make_shared<Accord>(); }

        class Accord : public ElementInterface
        {
        public:
            Accord();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            AccordAttributesPtr getAttributes() const;
            void setAttributes( const AccordAttributesPtr& value );

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

        private:
            AccordAttributesPtr myAttributes;
            TuningStepPtr myTuningStep;
            TuningAlterPtr myTuningAlter;
            bool myHasTuningAlter;
            TuningOctavePtr myTuningOctave;
        };
        
        class Scordatura;
        using ScordaturaPtr = std::shared_ptr<Scordatura>;
        using ScordaturaUPtr = std::unique_ptr<Scordatura>;
        using ScordaturaSet = std::vector<ScordaturaPtr>;
        using ScordaturaSetIter = ScordaturaSet::iterator;
        using ScordaturaSetIterConst = ScordaturaSet::const_iterator;
        inline ScordaturaPtr makeScordatura() { return std::make_shared<Scordatura>(); }

        class Scordatura : public ElementInterface
        {
        public:
            Scordatura();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Accord minOccurs = 1, maxOccurs = unbounded _________ */
            const AccordSet& getAccordSet() const;
            void addAccord( const AccordPtr& value );
            void removeAccord( const AccordSetIterConst& value );
            void clearAccordSet();
            AccordPtr getAccord( const AccordSetIterConst& setIterator ) const;

        private:
            AccordSet myAccordSet;
        };
        
        struct AttributesIterface;
        using AttributesIterfacePtr = std::shared_ptr<AttributesIterface>;
        
        struct AttributesIterface : public AttributesInterface
        {
        public:
            AttributesIterface();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
        };
        
        class DirectionType;
        using DirectionTypePtr = std::shared_ptr<DirectionType>;
        using DirectionTypeUPtr = std::unique_ptr<DirectionType>;
        using DirectionTypeSet = std::vector<DirectionTypePtr>;
        using DirectionTypeSetIter = DirectionTypeSet::iterator;
        using DirectionTypeSetIterConst = DirectionTypeSet::const_iterator;
        inline DirectionTypePtr makeDirectionType() { return std::make_shared<DirectionType>(); }

        class DirectionType : public ElementInterface
        {
        public:
            enum class Choice
            {
                rehearsal = 1,
                segno = 2,
                words = 3,
                coda = 4,
                wedge = 5,
                dynamics = 6,
                dashes = 7,
                bracket = 8,
                pedal = 9,
                metronome = 10,
                octaveShift = 11,
                harpPedals = 12,
                damp = 13,
                dampAll = 14,
                eyeglasses = 15,
                stringMute = 16,
                scordatura = 17,
                image = 18,
                principalVoice = 19,
                accordionRegistration = 20,
                percussion = 21,
                otherDirection = 22
            };
            DirectionType();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DirectionType::Choice getChoice() const;
            void setChoice( const DirectionType::Choice value );

            /* _________ Rehearsal minOccurs = 1, maxOccurs = unbounded _________ */
            const RehearsalSet& getRehearsalSet() const;
            void addRehearsal( const RehearsalPtr& value );
            void removeRehearsal( const RehearsalSetIterConst& value );
            void clearRehearsalSet();
            RehearsalPtr getRehearsal( const RehearsalSetIterConst& setIterator ) const;

            /* _________ Segno minOccurs = 1, maxOccurs = unbounded _________ */
            const SegnoSet& getSegnoSet() const;
            void addSegno( const SegnoPtr& value );
            void removeSegno( const SegnoSetIterConst& value );
            void clearSegnoSet();
            SegnoPtr getSegno( const SegnoSetIterConst& setIterator ) const;

            /* _________ Words minOccurs = 1, maxOccurs = unbounded _________ */
            const WordsSet& getWordsSet() const;
            void addWords( const WordsPtr& value );
            void removeWords( const WordsSetIterConst& value );
            void clearWordsSet();
            WordsPtr getWords( const WordsSetIterConst& setIterator ) const;

            /* _________ Coda minOccurs = 1, maxOccurs = unbounded _________ */
            const CodaSet& getCodaSet() const;
            void addCoda( const CodaPtr& value );
            void removeCoda( const CodaSetIterConst& value );
            void clearCodaSet();
            CodaPtr getCoda( const CodaSetIterConst& setIterator ) const;

            /* _________ Wedge minOccurs = 1, maxOccurs = 1 _________ */
            WedgePtr getWedge() const;
            void setWedge( const WedgePtr& value );

            /* _________ Dynamics minOccurs = 1, maxOccurs = unbounded _________ */
            const DynamicsSet& getDynamicsSet() const;
            void addDynamics( const DynamicsPtr& value );
            void removeDynamics( const DynamicsSetIterConst& value );
            void clearDynamicsSet();
            DynamicsPtr getDynamics( const DynamicsSetIterConst& setIterator ) const;

            /* _________ Dashes minOccurs = 1, maxOccurs = 1 _________ */
            DashesPtr getDashes() const;
            void setDashes( const DashesPtr& value );

            /* _________ Bracket minOccurs = 1, maxOccurs = 1 _________ */
            BracketPtr getBracket() const;
            void setBracket( const BracketPtr& value );

            /* _________ Pedal minOccurs = 1, maxOccurs = 1 _________ */
            PedalPtr getPedal() const;
            void setPedal( const PedalPtr& value );

            /* _________ Metronome minOccurs = 1, maxOccurs = 1 _________ */
            MetronomePtr getMetronome() const;
            void setMetronome( const MetronomePtr& value );

            /* _________ OctaveShift minOccurs = 1, maxOccurs = 1 _________ */
            OctaveShiftPtr getOctaveShift() const;
            void setOctaveShift( const OctaveShiftPtr& value );

            /* _________ HarpPedals minOccurs = 1, maxOccurs = 1 _________ */
            HarpPedalsPtr getHarpPedals() const;
            void setHarpPedals( const HarpPedalsPtr& value );

            /* _________ Damp minOccurs = 1, maxOccurs = 1 _________ */
            DampPtr getDamp() const;
            void setDamp( const DampPtr& value );

            /* _________ DampAll minOccurs = 1, maxOccurs = 1 _________ */
            DampAllPtr getDampAll() const;
            void setDampAll( const DampAllPtr& value );

            /* _________ Eyeglasses minOccurs = 1, maxOccurs = 1 _________ */
            EyeglassesPtr getEyeglasses() const;
            void setEyeglasses( const EyeglassesPtr& value );

            /* _________ StringMute minOccurs = 1, maxOccurs = 1 _________ */
            StringMutePtr getStringMute() const;
            void setStringMute( const StringMutePtr& value );

            /* _________ Scordatura minOccurs = 1, maxOccurs = 1 _________ */
            ScordaturaPtr getScordatura() const;
            void setScordatura( const ScordaturaPtr& value );

            /* _________ Image minOccurs = 1, maxOccurs = 1 _________ */
            ImagePtr getImage() const;
            void setImage( const ImagePtr& value );

            /* _________ PrincipalVoice minOccurs = 1, maxOccurs = 1 _________ */
            PrincipalVoicePtr getPrincipalVoice() const;
            void setPrincipalVoice( const PrincipalVoicePtr& value );

            /* _________ AccordionRegistration minOccurs = 1, maxOccurs = 1 _________ */
            AccordionRegistrationPtr getAccordionRegistration() const;
            void setAccordionRegistration( const AccordionRegistrationPtr& value );

            /* _________ Percussion minOccurs = 1, maxOccurs = unbounded _________ */
            const PercussionSet& getPercussionSet() const;
            void addPercussion( const PercussionPtr& value );
            void removePercussion( const PercussionSetIterConst& value );
            void clearPercussionSet();
            PercussionPtr getPercussion( const PercussionSetIterConst& setIterator ) const;

            /* _________ OtherDirection minOccurs = 1, maxOccurs = 1 _________ */
            OtherDirectionPtr getOtherDirection() const;
            void setOtherDirection( const OtherDirectionPtr& value );

        private:
            Choice myChoice;
            RehearsalSet myRehearsalSet;
            SegnoSet mySegnoSet;
            WordsSet myWordsSet;
            CodaSet myCodaSet;
            WedgePtr myWedge;
            DynamicsSet myDynamicsSet;
            DashesPtr myDashes;
            BracketPtr myBracket;
            PedalPtr myPedal;
            MetronomePtr myMetronome;
            OctaveShiftPtr myOctaveShift;
            HarpPedalsPtr myHarpPedals;
            DampPtr myDamp;
            DampAllPtr myDampAll;
            EyeglassesPtr myEyeglasses;
            StringMutePtr myStringMute;
            ScordaturaPtr myScordatura;
            ImagePtr myImage;
            PrincipalVoicePtr myPrincipalVoice;
            AccordionRegistrationPtr myAccordionRegistration;
            PercussionSet myPercussionSet;
            OtherDirectionPtr myOtherDirection;
        };
        
        
        struct MidiInstrumentAttributes;
        using MidiInstrumentAttributesPtr = std::shared_ptr<MidiInstrumentAttributes>;
        
        struct MidiInstrumentAttributes : public AttributesInterface
        {
        public:
            MidiInstrumentAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsIDREF id;
            const 	bool hasId;
        };
        
        class MidiInstrument;
        using MidiInstrumentPtr = std::shared_ptr<MidiInstrument>;
        using MidiInstrumentUPtr = std::unique_ptr<MidiInstrument>;
        using MidiInstrumentSet = std::vector<MidiInstrumentPtr>;
        using MidiInstrumentSetIter = MidiInstrumentSet::iterator;
        using MidiInstrumentSetIterConst = MidiInstrumentSet::const_iterator;
        inline MidiInstrumentPtr makeMidiInstrument() { return std::make_shared<MidiInstrument>(); }

        class MidiInstrument : public ElementInterface
        {
        public:
            MidiInstrument();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MidiInstrumentAttributesPtr getAttributes() const;
            void setAttributes( const MidiInstrumentAttributesPtr& value );

            /* _________ MidiChannel minOccurs = 0, maxOccurs = 1 _________ */
            MidiChannelPtr getMidiChannel() const;
            void setMidiChannel( const MidiChannelPtr& value );
            bool getHasMidiChannel() const;
            void setHasMidiChannel( const bool value );

            /* _________ MidiName minOccurs = 0, maxOccurs = 1 _________ */
            MidiNamePtr getMidiName() const;
            void setMidiName( const MidiNamePtr& value );
            bool getHasMidiName() const;
            void setHasMidiName( const bool value );

            /* _________ MidiBank minOccurs = 0, maxOccurs = 1 _________ */
            MidiBankPtr getMidiBank() const;
            void setMidiBank( const MidiBankPtr& value );
            bool getHasMidiBank() const;
            void setHasMidiBank( const bool value );

            /* _________ MidiProgram minOccurs = 0, maxOccurs = 1 _________ */
            MidiProgramPtr getMidiProgram() const;
            void setMidiProgram( const MidiProgramPtr& value );
            bool getHasMidiProgram() const;
            void setHasMidiProgram( const bool value );

            /* _________ MidiUnpitched minOccurs = 0, maxOccurs = 1 _________ */
            MidiUnpitchedPtr getMidiUnpitched() const;
            void setMidiUnpitched( const MidiUnpitchedPtr& value );
            bool getHasMidiUnpitched() const;
            void setHasMidiUnpitched( const bool value );

            /* _________ Volume minOccurs = 0, maxOccurs = 1 _________ */
            VolumePtr getVolume() const;
            void setVolume( const VolumePtr& value );
            bool getHasVolume() const;
            void setHasVolume( const bool value );

            /* _________ Pan minOccurs = 0, maxOccurs = 1 _________ */
            PanPtr getPan() const;
            void setPan( const PanPtr& value );
            bool getHasPan() const;
            void setHasPan( const bool value );

            /* _________ Elevation minOccurs = 0, maxOccurs = 1 _________ */
            ElevationPtr getElevation() const;
            void setElevation( const ElevationPtr& value );
            bool getHasElevation() const;
            void setHasElevation( const bool value );

        private:
            MidiInstrumentAttributesPtr myAttributes;
            MidiChannelPtr myMidiChannel;
            bool myHasMidiChannel;
            MidiNamePtr myMidiName;
            bool myHasMidiName;
            MidiBankPtr myMidiBank;
            bool myHasMidiBank;
            MidiProgramPtr myMidiProgram;
            bool myHasMidiProgram;
            MidiUnpitchedPtr myMidiUnpitched;
            bool myHasMidiUnpitched;
            VolumePtr myVolume;
            bool myHasVolume;
            PanPtr myPan;
            bool myHasPan;
            ElevationPtr myElevation;
            bool myHasElevation;
        };
        
        
        struct PlayAttributes;
        using PlayAttributesPtr = std::shared_ptr<PlayAttributes>;
        
        struct PlayAttributes : public AttributesInterface
        {
        public:
            PlayAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsIDREF id;
            bool hasId;
        };
        
        class Play;
        using PlayPtr = std::shared_ptr<Play>;
        using PlayUPtr = std::unique_ptr<Play>;
        using PlaySet = std::vector<PlayPtr>;
        using PlaySetIter = PlaySet::iterator;
        using PlaySetIterConst = PlaySet::const_iterator;
        inline PlayPtr makePlay() { return std::make_shared<Play>(); }

        class Play : public ElementInterface
        {
        public:
            enum class Choice
            {
                ipa = 1,
                mute = 2,
                semiPitched = 3,
                otherPlay = 4
            };
            Play();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PlayAttributesPtr getAttributes() const;
            void setAttributes( const PlayAttributesPtr& value );

            /* _________ Choice __________ */
            Play::Choice getChoice() const;
            void setChoice( const Play::Choice value );

            /* _________ Ipa minOccurs = 1, maxOccurs = 1 _________ */
            IpaPtr getIpa() const;
            void setIpa( const IpaPtr& value );

            /* _________ Mute minOccurs = 1, maxOccurs = 1 _________ */
            MutePtr getMute() const;
            void setMute( const MutePtr& value );

            /* _________ SemiPitched minOccurs = 1, maxOccurs = 1 _________ */
            SemiPitchedPtr getSemiPitched() const;
            void setSemiPitched( const SemiPitchedPtr& value );

            /* _________ OtherPlay minOccurs = 1, maxOccurs = 1 _________ */
            OtherPlayPtr getOtherPlay() const;
            void setOtherPlay( const OtherPlayPtr& value );
            

        private:
            PlayAttributesPtr myAttributes;
            Choice myChoice;
            IpaPtr myIpa;
            MutePtr myMute;
            SemiPitchedPtr mySemiPitched;
            OtherPlayPtr myOtherPlay;
        };
        
        class Encoding;
        using EncodingPtr = std::shared_ptr<Encoding>;
        using EncodingUPtr = std::unique_ptr<Encoding>;
        using EncodingSet = std::vector<EncodingPtr>;
        using EncodingSetIter = EncodingSet::iterator;
        using EncodingSetIterConst = EncodingSet::const_iterator;
        inline EncodingPtr makeEncoding() { return std::make_shared<Encoding>(); }

        class Encoding : public ElementInterface
        {
        public:
            enum class Choice
            {
                encodingDate = 1,
                encoder = 2,
                software = 3,
                encodingDescription = 4,
                supports = 5
            };
            Encoding();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice _________ */
            Encoding::Choice getChoice() const;
            void setChoice( const Encoding::Choice value );

            /* _________ EncodingDate minOccurs = 1, maxOccurs = 1 _________ */
            EncodingDatePtr getEncodingDate() const;
            void setEncodingDate( const EncodingDatePtr& value );

            /* _________ Encoder minOccurs = 1, maxOccurs = 1 _________ */
            EncoderPtr getEncoder() const;
            void setEncoder( const EncoderPtr& value );

            /* _________ Software minOccurs = 1, maxOccurs = 1 _________ */
            SoftwarePtr getSoftware() const;
            void setSoftware( const SoftwarePtr& value );

            /* _________ EncodingDescription minOccurs = 1, maxOccurs = 1 _________ */
            EncodingDescriptionPtr getEncodingDescription() const;
            void setEncodingDescription( const EncodingDescriptionPtr& value );

            /* _________ Supports minOccurs = 1, maxOccurs = 1 _________ */
            SupportsPtr getSupports() const;
            void setSupports( const SupportsPtr& value );

        private:
            Choice myChoice;
            EncodingDatePtr myEncodingDate;
            EncoderPtr myEncoder;
            SoftwarePtr mySoftware;
            EncodingDescriptionPtr myEncodingDescription;
            SupportsPtr mySupports;
        };
        
        class Miscellaneous;
        using MiscellaneousPtr = std::shared_ptr<Miscellaneous>;
        using MiscellaneousUPtr = std::unique_ptr<Miscellaneous>;
        using MiscellaneousSet = std::vector<MiscellaneousPtr>;
        using MiscellaneousSetIter = MiscellaneousSet::iterator;
        using MiscellaneousSetIterConst = MiscellaneousSet::const_iterator;
        inline MiscellaneousPtr makeMiscellaneous() { return std::make_shared<Miscellaneous>(); }

        class Miscellaneous : public ElementInterface
        {
        public:
            Miscellaneous();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ MiscellaneousField minOccurs = 0, maxOccurs = unbounded _________ */
            const MiscellaneousFieldSet& getMiscellaneousFieldSet() const;
            void addMiscellaneousField( const MiscellaneousFieldPtr& value );
            void removeMiscellaneousField( const MiscellaneousFieldSetIterConst& value );
            void clearMiscellaneousFieldSet();
            MiscellaneousFieldPtr getMiscellaneousField( const MiscellaneousFieldSetIterConst& setIterator ) const;

        private:
            MiscellaneousFieldSet myMiscellaneousFieldSet;
        };
        
        struct SoundAttributes;
        using SoundAttributesPtr = std::shared_ptr<SoundAttributes>;
        
        struct SoundAttributes : public AttributesInterface
        {
        public:
            SoundAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            NonNegativeDecimal tempo;
            NonNegativeDecimal dynamics;
            YesNo dacapo;
            XsToken segno;
            XsToken dalsegno;
            XsToken coda;
            XsToken tocoda;
            DivisionsValue divisions;
            YesNo forwardRepeat;
            XsToken fine;
            TimeOnly timeOnly;
            YesNo pizzicato;
            RotationDegrees pan;
            RotationDegrees elevation;
            YesNoNumber damperPedal;
            YesNoNumber softPedal;
            YesNoNumber sostenutoPedal;
            bool hasTempo;
            bool hasDynamics;
            bool hasDacapo;
            bool hasSegno;
            bool hasDalsegno;
            bool hasCoda;
            bool hasTocoda;
            bool hasDivisions;
            bool hasForwardRepeat;
            bool hasFine;
            bool hasTimeOnly;
            bool hasPizzicato;
            bool hasPan;
            bool hasElevation;
            bool hasDamperPedal;
            bool hasSoftPedal;
            bool hasSostenutoPedal;
        };
        
        class Sound;
        using SoundPtr = std::shared_ptr<Sound>;
        using SoundUPtr = std::unique_ptr<Sound>;
        using SoundSet = std::vector<SoundPtr>;
        using SoundSetIter = SoundSet::iterator;
        using SoundSetIterConst = SoundSet::const_iterator;
        inline SoundPtr makeSound() { return std::make_shared<Sound>(); }

        class Sound : public ElementInterface
        {
        public:
            Sound();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            SoundAttributesPtr getAttributes() const;
            void setAttributes( const SoundAttributesPtr& value );

            /* _________ MidiDevice minOccurs = 0, maxOccurs = 1 _________ */
            MidiDevicePtr getMidiDevice() const;
            void setMidiDevice( const MidiDevicePtr& value );
            bool getHasMidiDevice() const;
            void setHasMidiDevice( const bool value );

            /* _________ MidiInstrument minOccurs = 0, maxOccurs = 1 _________ */
            MidiInstrumentPtr getMidiInstrument() const;
            void setMidiInstrument( const MidiInstrumentPtr& value );
            bool getHasMidiInstrument() const;
            void setHasMidiInstrument( const bool value );

            /* _________ Play minOccurs = 0, maxOccurs = 1 _________ */
            PlayPtr getPlay() const;
            void setPlay( const PlayPtr& value );
            bool getHasPlay() const;
            void setHasPlay( const bool value );

        private:
            SoundAttributesPtr myAttributes;
            MidiDevicePtr myMidiDevice;
            bool myHasMidiDevice;
            MidiInstrumentPtr myMidiInstrument;
            bool myHasMidiInstrument;
            PlayPtr myPlay;
            bool myHasPlay;
        };
        
        struct PageMarginsAttributes;
        using PageMarginsAttributesPtr = std::shared_ptr<PageMarginsAttributes>;
        
        struct PageMarginsAttributes : public AttributesInterface
        {
        public:
            PageMarginsAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            MarginType type;
            bool hasType;
        };
        
        class PageMargins;
        using PageMarginsPtr = std::shared_ptr<PageMargins>;
        using PageMarginsUPtr = std::unique_ptr<PageMargins>;
        using PageMarginsSet = std::vector<PageMarginsPtr>;
        using PageMarginsSetIter = PageMarginsSet::iterator;
        using PageMarginsSetIterConst = PageMarginsSet::const_iterator;
        inline PageMarginsPtr makePageMargins() { return std::make_shared<PageMargins>(); }

        class PageMargins : public ElementInterface
        {
        public:
            PageMargins();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PageMarginsAttributesPtr getAttributes() const;
            void setAttributes( const PageMarginsAttributesPtr& value );

            /* _________ LeftMargin minOccurs = 1, maxOccurs = 1 _________ */
            LeftMarginPtr getLeftMargin() const;
            void setLeftMargin( const LeftMarginPtr& value );

            /* _________ RightMargin minOccurs = 1, maxOccurs = 1 _________ */
            RightMarginPtr getRightMargin() const;
            void setRightMargin( const RightMarginPtr& value );

            /* _________ TopMargin minOccurs = 1, maxOccurs = 1 _________ */
            TopMarginPtr getTopMargin() const;
            void setTopMargin( const TopMarginPtr& value );

            /* _________ BottomMargin minOccurs = 1, maxOccurs = 1 _________ */
            BottomMarginPtr getBottomMargin() const;
            void setBottomMargin( const BottomMarginPtr& value );

        private:
            PageMarginsAttributesPtr myAttributes;
            LeftMarginPtr myLeftMargin;
            RightMarginPtr myRightMargin;
            TopMarginPtr myTopMargin;
            BottomMarginPtr myBottomMargin;
        };
        
        class SystemMargins;
        using SystemMarginsPtr = std::shared_ptr<SystemMargins>;
        using SystemMarginsUPtr = std::unique_ptr<SystemMargins>;
        using SystemMarginsSet = std::vector<SystemMarginsPtr>;
        using SystemMarginsSetIter = SystemMarginsSet::iterator;
        using SystemMarginsSetIterConst = SystemMarginsSet::const_iterator;
        inline SystemMarginsPtr makeSystemMargins() { return std::make_shared<SystemMargins>(); }

        class SystemMargins : public ElementInterface
        {
        public:
            SystemMargins();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ LeftMargin minOccurs = 1, maxOccurs = 1 _________ */
            LeftMarginPtr getLeftMargin() const;
            void setLeftMargin( const LeftMarginPtr& value );

            /* _________ RightMargin minOccurs = 1, maxOccurs = 1 _________ */
            RightMarginPtr getRightMargin() const;
            void setRightMargin( const RightMarginPtr& value );

        private:
            LeftMarginPtr myLeftMargin;
            RightMarginPtr myRightMargin;
        };
        
        class SystemDividers;
        using SystemDividersPtr = std::shared_ptr<SystemDividers>;
        using SystemDividersUPtr = std::unique_ptr<SystemDividers>;
        using SystemDividersSet = std::vector<SystemDividersPtr>;
        using SystemDividersSetIter = SystemDividersSet::iterator;
        using SystemDividersSetIterConst = SystemDividersSet::const_iterator;
        inline SystemDividersPtr makeSystemDividers() { return std::make_shared<SystemDividers>(); }

        class SystemDividers : public ElementInterface
        {
        public:
            SystemDividers();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ LeftDivider minOccurs = 1, maxOccurs = 1 _________ */
            LeftDividerPtr getLeftDivider() const;
            void setLeftDivider( const LeftDividerPtr& value );

            /* _________ RightDivider minOccurs = 1, maxOccurs = 1 _________ */
            RightDividerPtr getRightDivider() const;
            void setRightDivider( const RightDividerPtr& value );

        private:
            LeftDividerPtr myLeftDivider;
            RightDividerPtr myRightDivider;
        };
        
        struct StrongAccentAttributes;
        using StrongAccentAttributesPtr = std::shared_ptr<StrongAccentAttributes>;
        
        struct StrongAccentAttributes : public AttributesInterface
        {
        public:
            StrongAccentAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            AboveBelow placement;
            UpDown type;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasPlacement;
            bool hasType;
        };
        
        class StrongAccent;
        using StrongAccentPtr = std::shared_ptr<StrongAccent>;
        using StrongAccentUPtr = std::unique_ptr<StrongAccent>;
        using StrongAccentSet = std::vector<StrongAccentPtr>;
        using StrongAccentSetIter = StrongAccentSet::iterator;
        using StrongAccentSetIterConst = StrongAccentSet::const_iterator;
        inline StrongAccentPtr makeStrongAccent() { return std::make_shared<StrongAccent>(); }

        class StrongAccent : public ElementInterface
        {
        public:
            StrongAccent();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StrongAccentAttributesPtr getAttributes() const;
            void setAttributes( const StrongAccentAttributesPtr& value );

        private:
            StrongAccentAttributesPtr myAttributes;
        };
        
        class Figure;
        using FigurePtr = std::shared_ptr<Figure>;
        using FigureUPtr = std::unique_ptr<Figure>;
        using FigureSet = std::vector<FigurePtr>;
        using FigureSetIter = FigureSet::iterator;
        using FigureSetIterConst = FigureSet::const_iterator;
        inline FigurePtr makeFigure() { return std::make_shared<Figure>(); }

        class Figure : public ElementInterface
        {
        public:
            Figure();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Prefix minOccurs = 0, maxOccurs = 1 _________ */
            PrefixPtr getPrefix() const;
            void setPrefix( const PrefixPtr& value );
            bool getHasPrefix() const;
            void setHasPrefix( const bool value );

            /* _________ FigureNumber minOccurs = 0, maxOccurs = 1 _________ */
            FigureNumberPtr getFigureNumber() const;
            void setFigureNumber( const FigureNumberPtr& value );
            bool getHasFigureNumber() const;
            void setHasFigureNumber( const bool value );

            /* _________ Suffix minOccurs = 0, maxOccurs = 1 _________ */
            SuffixPtr getSuffix() const;
            void setSuffix( const SuffixPtr& value );
            bool getHasSuffix() const;
            void setHasSuffix( const bool value );

            /* _________ Extend minOccurs = 0, maxOccurs = 1 _________ */
            ExtendPtr getExtend() const;
            void setExtend( const ExtendPtr& value );
            bool getHasExtend() const;
            void setHasExtend( const bool value );

        private:
            PrefixPtr myPrefix;
            bool myHasPrefix;
            FigureNumberPtr myFigureNumber;
            bool myHasFigureNumber;
            SuffixPtr mySuffix;
            bool myHasSuffix;
            ExtendPtr myExtend;
            bool myHasExtend;
        };
        class NormalTypeNormalDotGroup;
        using NormalTypeNormalDotGroupPtr = std::shared_ptr<NormalTypeNormalDotGroup>;
        using NormalTypeNormalDotGroupUPtr = std::unique_ptr<NormalTypeNormalDotGroup>;
        using NormalTypeNormalDotGroupSet = std::vector<NormalTypeNormalDotGroupPtr>;
        using NormalTypeNormalDotGroupSetIter = NormalTypeNormalDotGroupSet::iterator;
        using NormalTypeNormalDotGroupSetIterConst = NormalTypeNormalDotGroupSet::const_iterator;
        inline NormalTypeNormalDotGroupPtr makeNormalTypeNormalDotGroup() { return std::make_shared<NormalTypeNormalDotGroup>(); }

        class NormalTypeNormalDotGroup : public ElementInterface
        {
        public:
            NormalTypeNormalDotGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ NormalType minOccurs = 1, maxOccurs = 1 _________ */
            NormalTypePtr getNormalType() const;
            void setNormalType( const NormalTypePtr& value );

            /* _________ NormalDot minOccurs = 0, maxOccurs = unbounded _________ */
            const NormalDotSet& getNormalDotSet() const;
            void addNormalDot( const NormalDotPtr& value );
            void removeNormalDot( const NormalDotSetIterConst& value );
            void clearNormalDotSet();
            NormalDotPtr getNormalDot( const NormalDotSetIterConst& setIterator ) const;

        private:
            NormalTypePtr myNormalType;
            NormalDotSet myNormalDotSet;
        };
        
        class TimeModification;
        using TimeModificationPtr = std::shared_ptr<TimeModification>;
        using TimeModificationUPtr = std::unique_ptr<TimeModification>;
        using TimeModificationSet = std::vector<TimeModificationPtr>;
        using TimeModificationSetIter = TimeModificationSet::iterator;
        using TimeModificationSetIterConst = TimeModificationSet::const_iterator;
        inline TimeModificationPtr makeTimeModification() { return std::make_shared<TimeModification>(); }

        class TimeModification : public ElementInterface
        {
        public:
            TimeModification();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ ActualNotes minOccurs = 1, maxOccurs = 1 _________ */
            ActualNotesPtr getActualNotes() const;
            void setActualNotes( const ActualNotesPtr& value );

            /* _________ NormalNotes minOccurs = 1, maxOccurs = 1 _________ */
            NormalNotesPtr getNormalNotes() const;
            void setNormalNotes( const NormalNotesPtr& value );

            /* _________ NormalTypeNormalDotGroup minOccurs = 0, maxOccurs = 1 _________ */
            NormalTypeNormalDotGroupPtr getNormalTypeNormalDotGroup() const;
            void setNormalTypeNormalDotGroup( const NormalTypeNormalDotGroupPtr& value );
            bool getHasNormalTypeNormalDotGroup() const;
            void setHasNormalTypeNormalDotGroup( const bool value );

        private:
            ActualNotesPtr myActualNotes;
            NormalNotesPtr myNormalNotes;
            NormalTypeNormalDotGroupPtr myNormalTypeNormalDotGroup;
            bool myHasNormalTypeNormalDotGroup;
        };
        
        class NoteheadText;
        using NoteheadTextPtr = std::shared_ptr<NoteheadText>;
        using NoteheadTextUPtr = std::unique_ptr<NoteheadText>;
        using NoteheadTextSet = std::vector<NoteheadTextPtr>;
        using NoteheadTextSetIter = NoteheadTextSet::iterator;
        using NoteheadTextSetIterConst = NoteheadTextSet::const_iterator;
        inline NoteheadTextPtr makeNoteheadText() { return std::make_shared<NoteheadText>(); }

        class NoteheadText : public ElementInterface
        {
        public:
            NoteheadText();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ DisplayText minOccurs = 1, maxOccurs = 1 _________ */
            DisplayTextPtr getDisplayText() const;
            void setDisplayText( const DisplayTextPtr& value );

            /* _________ AccidentalText minOccurs = 1, maxOccurs = 1 _________ */
            AccidentalTextPtr getAccidentalText() const;
            void setAccidentalText( const AccidentalTextPtr& value );

        private:
            DisplayTextPtr myDisplayText;
            AccidentalTextPtr myAccidentalText;
        };
        
        class ArticulationsChoice;
        using ArticulationsChoicePtr = std::shared_ptr<ArticulationsChoice>;
        using ArticulationsChoiceUPtr = std::unique_ptr<ArticulationsChoice>;
        using ArticulationsChoiceSet = std::vector<ArticulationsChoicePtr>;
        using ArticulationsChoiceSetIter = ArticulationsChoiceSet::iterator;
        using ArticulationsChoiceSetIterConst = ArticulationsChoiceSet::const_iterator;
        inline ArticulationsChoicePtr makeArticulationsChoice() { return std::make_shared<ArticulationsChoice>(); }

        class ArticulationsChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                accent = 1,
                strongAccent = 2,
                staccato = 3,
                tenuto = 4,
                detachedLegato = 5,
                staccatissimo = 6,
                spiccato = 7,
                scoop = 8,
                plop = 9,
                doit = 10,
                falloff = 11,
                breathMark = 12,
                caesura = 13,
                stress = 14,
                unstress = 15,
                otherArticulation = 16
            };
            ArticulationsChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            ArticulationsChoice::Choice getChoice() const;
            void setChoice( const ArticulationsChoice::Choice value );

            /* _________ Accent minOccurs = 1, maxOccurs = 1 _________ */
            AccentPtr getAccent() const;
            void setAccent( const AccentPtr& value );

            /* _________ StrongAccent minOccurs = 1, maxOccurs = 1 _________ */
            StrongAccentPtr getStrongAccent() const;
            void setStrongAccent( const StrongAccentPtr& value );

            /* _________ Staccato minOccurs = 1, maxOccurs = 1 _________ */
            StaccatoPtr getStaccato() const;
            void setStaccato( const StaccatoPtr& value );

            /* _________ Tenuto minOccurs = 1, maxOccurs = 1 _________ */
            TenutoPtr getTenuto() const;
            void setTenuto( const TenutoPtr& value );

            /* _________ DetachedLegato minOccurs = 1, maxOccurs = 1 _________ */
            DetachedLegatoPtr getDetachedLegato() const;
            void setDetachedLegato( const DetachedLegatoPtr& value );

            /* _________ Staccatissimo minOccurs = 1, maxOccurs = 1 _________ */
            StaccatissimoPtr getStaccatissimo() const;
            void setStaccatissimo( const StaccatissimoPtr& value );

            /* _________ Spiccato minOccurs = 1, maxOccurs = 1 _________ */
            SpiccatoPtr getSpiccato() const;
            void setSpiccato( const SpiccatoPtr& value );

            /* _________ Scoop minOccurs = 1, maxOccurs = 1 _________ */
            ScoopPtr getScoop() const;
            void setScoop( const ScoopPtr& value );

            /* _________ Plop minOccurs = 1, maxOccurs = 1 _________ */
            PlopPtr getPlop() const;
            void setPlop( const PlopPtr& value );

            /* _________ Doit minOccurs = 1, maxOccurs = 1 _________ */
            DoitPtr getDoit() const;
            void setDoit( const DoitPtr& value );

            /* _________ Falloff minOccurs = 1, maxOccurs = 1 _________ */
            FalloffPtr getFalloff() const;
            void setFalloff( const FalloffPtr& value );

            /* _________ BreathMark minOccurs = 1, maxOccurs = 1 _________ */
            BreathMarkPtr getBreathMark() const;
            void setBreathMark( const BreathMarkPtr& value );

            /* _________ Caesura minOccurs = 1, maxOccurs = 1 _________ */
            CaesuraPtr getCaesura() const;
            void setCaesura( const CaesuraPtr& value );

            /* _________ Stress minOccurs = 1, maxOccurs = 1 _________ */
            StressPtr getStress() const;
            void setStress( const StressPtr& value );

            /* _________ Unstress minOccurs = 1, maxOccurs = 1 _________ */
            UnstressPtr getUnstress() const;
            void setUnstress( const UnstressPtr& value );

            /* _________ OtherArticulation minOccurs = 1, maxOccurs = 1 _________ */
            OtherArticulationPtr getOtherArticulation() const;
            void setOtherArticulation( const OtherArticulationPtr& value );

        private:
            Choice myChoice;
            AccentPtr myAccent;
            StrongAccentPtr myStrongAccent;
            StaccatoPtr myStaccato;
            TenutoPtr myTenuto;
            DetachedLegatoPtr myDetachedLegato;
            StaccatissimoPtr myStaccatissimo;
            SpiccatoPtr mySpiccato;
            ScoopPtr myScoop;
            PlopPtr myPlop;
            DoitPtr myDoit;
            FalloffPtr myFalloff;
            BreathMarkPtr myBreathMark;
            CaesuraPtr myCaesura;
            StressPtr myStress;
            UnstressPtr myUnstress;
            OtherArticulationPtr myOtherArticulation;
        };
        class Articulations;
        using ArticulationsPtr = std::shared_ptr<Articulations>;
        using ArticulationsUPtr = std::unique_ptr<Articulations>;
        using ArticulationsSet = std::vector<ArticulationsPtr>;
        using ArticulationsSetIter = ArticulationsSet::iterator;
        using ArticulationsSetIterConst = ArticulationsSet::const_iterator;
        inline ArticulationsPtr makeArticulations() { return std::make_shared<Articulations>(); }

        class Articulations : public ElementInterface
        {
        public:
            Articulations();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ ArticulationsChoice minOccurs = 0, maxOccurs = unbounded _________ */
            const ArticulationsChoiceSet& getArticulationsChoiceSet() const;
            void addArticulationsChoice( const ArticulationsChoicePtr& value );
            void removeArticulationsChoice( const ArticulationsChoiceSetIterConst& value );
            void clearArticulationsChoiceSet();
            ArticulationsChoicePtr getArticulationsChoice( const ArticulationsChoiceSetIterConst& setIterator ) const;

        private:
            ArticulationsChoiceSet myArticulationsChoiceSet;
        };
        
        
        class ElisionSyllabicGroup;
        using ElisionSyllabicGroupPtr = std::shared_ptr<ElisionSyllabicGroup>;
        using ElisionSyllabicGroupUPtr = std::unique_ptr<ElisionSyllabicGroup>;
        using ElisionSyllabicGroupSet = std::vector<ElisionSyllabicGroupPtr>;
        using ElisionSyllabicGroupSetIter = ElisionSyllabicGroupSet::iterator;
        using ElisionSyllabicGroupSetIterConst = ElisionSyllabicGroupSet::const_iterator;
        inline ElisionSyllabicGroupPtr makeElisionSyllabicGroup() { return std::make_shared<ElisionSyllabicGroup>(); }

        class ElisionSyllabicGroup : public ElementInterface
        {
        public:
            ElisionSyllabicGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Elision minOccurs = 1, maxOccurs = 1 _________ */
            ElisionPtr getElision() const;
            void setElision( const ElisionPtr& value );

            /* _________ Syllabic minOccurs = 0, maxOccurs = 1 _________ */
            SyllabicPtr getSyllabic() const;
            void setSyllabic( const SyllabicPtr& value );
            bool getHasSyllabic() const;
            void setHasSyllabic( const bool value );

        private:
            ElisionPtr myElision;
            SyllabicPtr mySyllabic;
            bool myHasSyllabic;
        };
        
        class ElisionSyllabicTextGroup;
        using ElisionSyllabicTextGroupPtr = std::shared_ptr<ElisionSyllabicTextGroup>;
        using ElisionSyllabicTextGroupUPtr = std::unique_ptr<ElisionSyllabicTextGroup>;
        using ElisionSyllabicTextGroupSet = std::vector<ElisionSyllabicTextGroupPtr>;
        using ElisionSyllabicTextGroupSetIter = ElisionSyllabicTextGroupSet::iterator;
        using ElisionSyllabicTextGroupSetIterConst = ElisionSyllabicTextGroupSet::const_iterator;
        inline ElisionSyllabicTextGroupPtr makeElisionSyllabicTextGroup() { return std::make_shared<ElisionSyllabicTextGroup>(); }

        class ElisionSyllabicTextGroup : public ElementInterface
        {
        public:
            ElisionSyllabicTextGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ ElisionSyllabicGroup minOccurs = 0, maxOccurs = 1 _________ */
            ElisionSyllabicGroupPtr getElisionSyllabicGroup() const;
            void setElisionSyllabicGroup( const ElisionSyllabicGroupPtr& value );
            bool getHasElisionSyllabicGroup() const;
            void setHasElisionSyllabicGroup( const bool value );

            /* _________ Text minOccurs = 1, maxOccurs = 1 _________ */
            TextPtr getText() const;
            void setText( const TextPtr& value );

        private:
            ElisionSyllabicGroupPtr myElisionSyllabicGroup;
            bool myHasElisionSyllabicGroup;
            TextPtr myText;
        };
        
        
        class SyllabicTextGroup;
        using SyllabicTextGroupPtr = std::shared_ptr<SyllabicTextGroup>;
        using SyllabicTextGroupUPtr = std::unique_ptr<SyllabicTextGroup>;
        using SyllabicTextGroupSet = std::vector<SyllabicTextGroupPtr>;
        using SyllabicTextGroupSetIter = SyllabicTextGroupSet::iterator;
        using SyllabicTextGroupSetIterConst = SyllabicTextGroupSet::const_iterator;
        inline SyllabicTextGroupPtr makeSyllabicTextGroup() { return std::make_shared<SyllabicTextGroup>(); }

        class SyllabicTextGroup : public ElementInterface
        {
        public:
            SyllabicTextGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Syllabic minOccurs = 0, maxOccurs = 1 _________ */
            SyllabicPtr getSyllabic() const;
            void setSyllabic( const SyllabicPtr& value );
            bool getHasSyllabic() const;
            void setHasSyllabic( const bool value );

            /* _________ Text minOccurs = 1, maxOccurs = 1 _________ */
            TextPtr getText() const;
            void setText( const TextPtr& value );

            /* _________ ElisionSyllabicTextGroup minOccurs = 0, maxOccurs = unbounded _________ */
            const ElisionSyllabicTextGroupSet& getElisionSyllabicTextGroupSet() const;
            void addElisionSyllabicTextGroup( const ElisionSyllabicTextGroupPtr& value );
            void removeElisionSyllabicTextGroup( const ElisionSyllabicTextGroupSetIterConst& value );
            void clearElisionSyllabicTextGroupSet();
            ElisionSyllabicTextGroupPtr getElisionSyllabicTextGroup( const ElisionSyllabicTextGroupSetIterConst& setIterator ) const;

            /* _________ Extend minOccurs = 0, maxOccurs = 1 _________ */
            ExtendPtr getExtend() const;
            void setExtend( const ExtendPtr& value );
            bool getHasExtend() const;
            void setHasExtend( const bool value );

        private:
            SyllabicPtr mySyllabic;
            bool myHasSyllabic;
            TextPtr myText;
            ElisionSyllabicTextGroupSet myElisionSyllabicTextGroupSet;
            ExtendPtr myExtend;
            bool myHasExtend;
        };
        
        
        class LyricTextChoice;
        using LyricTextChoicePtr = std::shared_ptr<LyricTextChoice>;
        using LyricTextChoiceUPtr = std::unique_ptr<LyricTextChoice>;
        using LyricTextChoiceSet = std::vector<LyricTextChoicePtr>;
        using LyricTextChoiceSetIter = LyricTextChoiceSet::iterator;
        using LyricTextChoiceSetIterConst = LyricTextChoiceSet::const_iterator;
        inline LyricTextChoicePtr makeLyricTextChoice() { return std::make_shared<LyricTextChoice>(); }

        class LyricTextChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                syllabicTextGroup = 1,
                extend = 2,
                laughing = 3,
                humming = 4
            };
            LyricTextChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            LyricTextChoice::Choice getChoice() const;
            void setChoice( const LyricTextChoice::Choice value );

            /* _________ SyllabicTextGroup minOccurs = 1, maxOccurs = 1 _________ */
            SyllabicTextGroupPtr getSyllabicTextGroup() const;
            void setSyllabicTextGroup( const SyllabicTextGroupPtr& value );

            /* _________ Extend minOccurs = 1, maxOccurs = 1 _________ */
            ExtendPtr getExtend() const;
            void setExtend( const ExtendPtr& value );

            /* _________ Laughing minOccurs = 1, maxOccurs = 1 _________ */
            LaughingPtr getLaughing() const;
            void setLaughing( const LaughingPtr& value );

            /* _________ Humming minOccurs = 1, maxOccurs = 1 _________ */
            HummingPtr getHumming() const;
            void setHumming( const HummingPtr& value );

        private:
            Choice myChoice;
            SyllabicTextGroupPtr mySyllabicTextGroup;
            ExtendPtr myExtend;
            LaughingPtr myLaughing;
            HummingPtr myHumming;
        };
        
        
        class EditorialGroup;
        using EditorialGroupPtr = std::shared_ptr<EditorialGroup>;
        using EditorialGroupUPtr = std::unique_ptr<EditorialGroup>;
        using EditorialGroupSet = std::vector<EditorialGroupPtr>;
        using EditorialGroupSetIter = EditorialGroupSet::iterator;
        using EditorialGroupSetIterConst = EditorialGroupSet::const_iterator;
        inline EditorialGroupPtr makeEditorialGroup() { return std::make_shared<EditorialGroup>(); }

        class EditorialGroup : public ElementInterface
        {
        public:
            EditorialGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Footnote minOccurs = 0, maxOccurs = 1 _________ */
            FootnotePtr getFootnote() const;
            void setFootnote( const FootnotePtr& value );
            bool getHasFootnote() const;
            void setHasFootnote( const bool value );

            /* _________ Level minOccurs = 0, maxOccurs = 1 _________ */
            LevelPtr getLevel() const;
            void setLevel( const LevelPtr& value );
            bool getHasLevel() const;
            void setHasLevel( const bool value );

        private:
            FootnotePtr myFootnote;
            bool myHasFootnote;
            LevelPtr myLevel;
            bool myHasLevel;
        };
        
        class EditorialVoiceGroup;
        using EditorialVoiceGroupPtr = std::shared_ptr<EditorialVoiceGroup>;
        using EditorialVoiceGroupUPtr = std::unique_ptr<EditorialVoiceGroup>;
        using EditorialVoiceGroupSet = std::vector<EditorialVoiceGroupPtr>;
        using EditorialVoiceGroupSetIter = EditorialVoiceGroupSet::iterator;
        using EditorialVoiceGroupSetIterConst = EditorialVoiceGroupSet::const_iterator;
        inline EditorialVoiceGroupPtr makeEditorialVoiceGroup() { return std::make_shared<EditorialVoiceGroup>(); }

        class EditorialVoiceGroup : public ElementInterface
        {
        public:
            EditorialVoiceGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Footnote minOccurs = 0, maxOccurs = 1 _________ */
            FootnotePtr getFootnote() const;
            void setFootnote( const FootnotePtr& value );
            bool getHasFootnote() const;
            void setHasFootnote( const bool value );

            /* _________ Level minOccurs = 0, maxOccurs = 1 _________ */
            LevelPtr getLevel() const;
            void setLevel( const LevelPtr& value );
            bool getHasLevel() const;
            void setHasLevel( const bool value );

            /* _________ Voice minOccurs = 0, maxOccurs = 1 _________ */
            VoicePtr getVoice() const;
            void setVoice( const VoicePtr& value );
            bool getHasVoice() const;
            void setHasVoice( const bool value );

        private:
            FootnotePtr myFootnote;
            bool myHasFootnote;
            LevelPtr myLevel;
            bool myHasLevel;
            VoicePtr myVoice;
            bool myHasVoice;
        };
        
        struct LyricAttributes;
        using LyricAttributesPtr = std::shared_ptr<LyricAttributes>;
        
        struct LyricAttributes : public AttributesInterface
        {
        public:
            LyricAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsNMToken number;
            XsToken name;
            LeftCenterRight justify;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            AboveBelow placement;
            Color color;
            YesNo printObject;
            bool hasNumber;
            bool hasName;
            bool hasJustify;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasPlacement;
            bool hasColor;
            bool hasPrintObject;
        };
        
        class Lyric;
        using LyricPtr = std::shared_ptr<Lyric>;
        using LyricUPtr = std::unique_ptr<Lyric>;
        using LyricSet = std::vector<LyricPtr>;
        using LyricSetIter = LyricSet::iterator;
        using LyricSetIterConst = LyricSet::const_iterator;
        inline LyricPtr makeLyric() { return std::make_shared<Lyric>(); }

        class Lyric : public ElementInterface
        {
        public:
            Lyric();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            LyricAttributesPtr getAttributes() const;
            void setAttributes( const LyricAttributesPtr& value );

            /* _________ LyricTextChoice minOccurs = 1, maxOccurs = 1 _________ */
            LyricTextChoicePtr getLyricTextChoice() const;
            void setLyricTextChoice( const LyricTextChoicePtr& value );

            /* _________ EndLine minOccurs = 0, maxOccurs = 1 _________ */
            EndLinePtr getEndLine() const;
            void setEndLine( const EndLinePtr& value );
            bool getHasEndLine() const;
            void setHasEndLine( const bool value );

            /* _________ EndParagraph minOccurs = 0, maxOccurs = 1 _________ */
            EndParagraphPtr getEndParagraph() const;
            void setEndParagraph( const EndParagraphPtr& value );
            bool getHasEndParagraph() const;
            void setHasEndParagraph( const bool value );

            /* _________ EditorialGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialGroupPtr getEditorialGroup() const;
            void setEditorialGroup( const EditorialGroupPtr& value );

        private:
            LyricAttributesPtr myAttributes;
            LyricTextChoicePtr myLyricTextChoice;
            EndLinePtr myEndLine;
            bool myHasEndLine;
            EndParagraphPtr myEndParagraph;
            bool myHasEndParagraph;
            EditorialGroupPtr myEditorialGroup;
        };
        
        
        
        struct MordentAttributes;
        using MordentAttributesPtr = std::shared_ptr<MordentAttributes>;
        
        struct MordentAttributes : public AttributesInterface
        {
        public:
            MordentAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            AboveBelow placement;
            StartNote startNote;
            TrillStep trillStep;
            TwoNoteTurn twoNoteTurn;
            YesNo accelerate;
            TrillBeats beats;
            Percent secondBeat;
            Percent lastBeat;
            YesNo long_;
            AboveBelow approach;
            AboveBelow departure;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasPlacement;
            bool hasStartNote;
            bool hasTrillStep;
            bool hasTwoNoteTurn;
            bool hasAccelerate;
            bool hasBeats;
            bool hasSecondBeat;
            bool hasLastBeat;
            bool hasLong;
            bool hasApproach;
            bool hasDeparture;
        };
        
        
        
        class Mordent;
        using MordentPtr = std::shared_ptr<Mordent>;
        using MordentUPtr = std::unique_ptr<Mordent>;
        using MordentSet = std::vector<MordentPtr>;
        using MordentSetIter = MordentSet::iterator;
        using MordentSetIterConst = MordentSet::const_iterator;
        inline MordentPtr makeMordent() { return std::make_shared<Mordent>(); }

        class Mordent : public ElementInterface
        {
        public:
            Mordent();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MordentAttributesPtr getAttributes() const;
            void setAttributes( const MordentAttributesPtr& value );

        private:
            MordentAttributesPtr myAttributes;
        };
        
        
        struct InvertedMordentAttributes;
        using InvertedMordentAttributesPtr = std::shared_ptr<InvertedMordentAttributes>;
        
        struct InvertedMordentAttributes : public AttributesInterface
        {
        public:
            InvertedMordentAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            AboveBelow placement;
            StartNote startNote;
            TrillStep trillStep;
            TwoNoteTurn twoNoteTurn;
            YesNo accelerate;
            TrillBeats beats;
            Percent secondBeat;
            Percent lastBeat;
            YesNo long_;
            AboveBelow approach;
            AboveBelow departure;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasPlacement;
            bool hasStartNote;
            bool hasTrillStep;
            bool hasTwoNoteTurn;
            bool hasAccelerate;
            bool hasBeats;
            bool hasSecondBeat;
            bool hasLastBeat;
            bool hasLong;
            bool hasApproach;
            bool hasDeparture;
        };
        
        class InvertedMordent;
        using InvertedMordentPtr = std::shared_ptr<InvertedMordent>;
        using InvertedMordentUPtr = std::unique_ptr<InvertedMordent>;
        using InvertedMordentSet = std::vector<InvertedMordentPtr>;
        using InvertedMordentSetIter = InvertedMordentSet::iterator;
        using InvertedMordentSetIterConst = InvertedMordentSet::const_iterator;
        inline InvertedMordentPtr makeInvertedMordent() { return std::make_shared<InvertedMordent>(); }

        class InvertedMordent : public ElementInterface
        {
        public:
            InvertedMordent();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            InvertedMordentAttributesPtr getAttributes() const;
            void setAttributes( const InvertedMordentAttributesPtr& value );

        private:
            InvertedMordentAttributesPtr myAttributes;
        };
        
        
        struct HeelAttributes;
        using HeelAttributesPtr = std::shared_ptr<HeelAttributes>;
        
        struct HeelAttributes : public AttributesInterface
        {
        public:
            HeelAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            AboveBelow placement;
            YesNo substitution;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasPlacement;
            bool hasSubstitution;
        };
        
        class Heel;
        using HeelPtr = std::shared_ptr<Heel>;
        using HeelUPtr = std::unique_ptr<Heel>;
        using HeelSet = std::vector<HeelPtr>;
        using HeelSetIter = HeelSet::iterator;
        using HeelSetIterConst = HeelSet::const_iterator;
        inline HeelPtr makeHeel() { return std::make_shared<Heel>(); }

        class Heel : public ElementInterface
        {
        public:
            Heel();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HeelAttributesPtr getAttributes() const;
            void setAttributes( const HeelAttributesPtr& value );

        private:
            HeelAttributesPtr myAttributes;
        };
        
        
        struct ToeAttributes;
        using ToeAttributesPtr = std::shared_ptr<ToeAttributes>;
        
        struct ToeAttributes : public AttributesInterface
        {
        public:
            ToeAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            AboveBelow placement;
            YesNo substitution;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasPlacement;
            bool hasSubstitution;
        };
        
        class Toe;
        using ToePtr = std::shared_ptr<Toe>;
        using ToeUPtr = std::unique_ptr<Toe>;
        using ToeSet = std::vector<ToePtr>;
        using ToeSetIter = ToeSet::iterator;
        using ToeSetIterConst = ToeSet::const_iterator;
        inline ToePtr makeToe() { return std::make_shared<Toe>(); }

        class Toe : public ElementInterface
        {
        public:
            Toe();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ToeAttributesPtr getAttributes() const;
            void setAttributes( const ToeAttributesPtr& value );

        private:
            ToeAttributesPtr myAttributes;
        };
        
        
        struct HoleAttributes;
        using HoleAttributesPtr = std::shared_ptr<HoleAttributes>;
        
        struct HoleAttributes : public AttributesInterface
        {
        public:
            HoleAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            AboveBelow placement;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasPlacement;
        };
        
        class Hole;
        using HolePtr = std::shared_ptr<Hole>;
        using HoleUPtr = std::unique_ptr<Hole>;
        using HoleSet = std::vector<HolePtr>;
        using HoleSetIter = HoleSet::iterator;
        using HoleSetIterConst = HoleSet::const_iterator;
        inline HolePtr makeHole() { return std::make_shared<Hole>(); }

        class Hole : public ElementInterface
        {
        public:
            Hole();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HoleAttributesPtr getAttributes() const;
            void setAttributes( const HoleAttributesPtr& value );

            /* _________ HoleType minOccurs = 0, maxOccurs = 1 _________ */
            HoleTypePtr getHoleType() const;
            void setHoleType( const HoleTypePtr& value );
            bool getHasHoleType() const;
            void setHasHoleType( const bool value );

            /* _________ HoleClosed minOccurs = 1, maxOccurs = 1 _________ */
            HoleClosedPtr getHoleClosed() const;
            void setHoleClosed( const HoleClosedPtr& value );

            /* _________ HoleShape minOccurs = 0, maxOccurs = 1 _________ */
            HoleShapePtr getHoleShape() const;
            void setHoleShape( const HoleShapePtr& value );
            bool getHasHoleShape() const;
            void setHasHoleShape( const bool value );

        private:
            HoleAttributesPtr myAttributes;
            HoleTypePtr myHoleType;
            bool myHasHoleType;
            HoleClosedPtr myHoleClosed;
            HoleShapePtr myHoleShape;
            bool myHasHoleShape;
        };
        
        class TupletActual;
        using TupletActualPtr = std::shared_ptr<TupletActual>;
        using TupletActualUPtr = std::unique_ptr<TupletActual>;
        using TupletActualSet = std::vector<TupletActualPtr>;
        using TupletActualSetIter = TupletActualSet::iterator;
        using TupletActualSetIterConst = TupletActualSet::const_iterator;
        inline TupletActualPtr makeTupletActual() { return std::make_shared<TupletActual>(); }

        class TupletActual : public ElementInterface
        {
        public:
            TupletActual();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ TupletNumber minOccurs = 0, maxOccurs = 1 _________ */
            TupletNumberPtr getTupletNumber() const;
            void setTupletNumber( const TupletNumberPtr& value );
            bool getHasTupletNumber() const;
            void setHasTupletNumber( const bool value );

            /* _________ TupletType minOccurs = 0, maxOccurs = 1 _________ */
            TupletTypePtr getTupletType() const;
            void setTupletType( const TupletTypePtr& value );
            bool getHasTupletType() const;
            void setHasTupletType( const bool value );

            /* _________ TupletDot minOccurs = 0, maxOccurs = unbounded _________ */
            const TupletDotSet& getTupletDotSet() const;
            void addTupletDot( const TupletDotPtr& value );
            void removeTupletDot( const TupletDotSetIterConst& value );
            void clearTupletDotSet();
            TupletDotPtr getTupletDot( const TupletDotSetIterConst& setIterator ) const;

        private:
            TupletNumberPtr myTupletNumber;
            bool myHasTupletNumber;
            TupletTypePtr myTupletType;
            bool myHasTupletType;
            TupletDotSet myTupletDotSet;
        };
        
        
        class TupletNormal;
        using TupletNormalPtr = std::shared_ptr<TupletNormal>;
        using TupletNormalUPtr = std::unique_ptr<TupletNormal>;
        using TupletNormalSet = std::vector<TupletNormalPtr>;
        using TupletNormalSetIter = TupletNormalSet::iterator;
        using TupletNormalSetIterConst = TupletNormalSet::const_iterator;
        inline TupletNormalPtr makeTupletNormal() { return std::make_shared<TupletNormal>(); }

        class TupletNormal : public ElementInterface
        {
        public:
            TupletNormal();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ TupletNumber minOccurs = 0, maxOccurs = 1 _________ */
            TupletNumberPtr getTupletNumber() const;
            void setTupletNumber( const TupletNumberPtr& value );
            bool getHasTupletNumber() const;
            void setHasTupletNumber( const bool value );

            /* _________ TupletType minOccurs = 0, maxOccurs = 1 _________ */
            TupletTypePtr getTupletType() const;
            void setTupletType( const TupletTypePtr& value );
            bool getHasTupletType() const;
            void setHasTupletType( const bool value );

            /* _________ TupletDot minOccurs = 0, maxOccurs = unbounded _________ */
            const TupletDotSet& getTupletDotSet() const;
            void addTupletDot( const TupletDotPtr& value );
            void removeTupletDot( const TupletDotSetIterConst& value );
            void clearTupletDotSet();
            TupletDotPtr getTupletDot( const TupletDotSetIterConst& setIterator ) const;

        private:
            TupletNumberPtr myTupletNumber;
            bool myHasTupletNumber;
            TupletTypePtr myTupletType;
            bool myHasTupletType;
            TupletDotSet myTupletDotSet;
        };
        
        
        struct TupletAttributes;
        using TupletAttributesPtr = std::shared_ptr<TupletAttributes>;
        
        struct TupletAttributes : public AttributesInterface
        {
        public:
            TupletAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            NumberLevel number;
            YesNo bracket;
            ShowTuplet showNumber;
            ShowTuplet showType;
            LineShape lineShape;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            AboveBelow placement;
            const 	bool hasType;
            bool hasNumber;
            bool hasBracket;
            bool hasShowNumber;
            bool hasShowType;
            bool hasLineShape;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasPlacement;
        };
        
        class Tuplet;
        using TupletPtr = std::shared_ptr<Tuplet>;
        using TupletUPtr = std::unique_ptr<Tuplet>;
        using TupletSet = std::vector<TupletPtr>;
        using TupletSetIter = TupletSet::iterator;
        using TupletSetIterConst = TupletSet::const_iterator;
        inline TupletPtr makeTuplet() { return std::make_shared<Tuplet>(); }

        class Tuplet : public ElementInterface
        {
        public:
            Tuplet();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TupletAttributesPtr getAttributes() const;
            void setAttributes( const TupletAttributesPtr& value );

            /* _________ TupletActual minOccurs = 0, maxOccurs = 1 _________ */
            TupletActualPtr getTupletActual() const;
            void setTupletActual( const TupletActualPtr& value );
            bool getHasTupletActual() const;
            void setHasTupletActual( const bool value );

            /* _________ TupletNormal minOccurs = 0, maxOccurs = 1 _________ */
            TupletNormalPtr getTupletNormal() const;
            void setTupletNormal( const TupletNormalPtr& value );
            bool getHasTupletNormal() const;
            void setHasTupletNormal( const bool value );

        private:
            TupletAttributesPtr myAttributes;
            TupletActualPtr myTupletActual;
            bool myHasTupletActual;
            TupletNormalPtr myTupletNormal;
            bool myHasTupletNormal;
        };
        
        class Scaling;
        using ScalingPtr = std::shared_ptr<Scaling>;
        using ScalingUPtr = std::unique_ptr<Scaling>;
        using ScalingSet = std::vector<ScalingPtr>;
        using ScalingSetIter = ScalingSet::iterator;
        using ScalingSetIterConst = ScalingSet::const_iterator;
        inline ScalingPtr makeScaling() { return std::make_shared<Scaling>(); }

        class Scaling : public ElementInterface
        {
        public:
            Scaling();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Millimeters minOccurs = 1, maxOccurs = 1 _________ */
            MillimetersPtr getMillimeters() const;
            void setMillimeters( const MillimetersPtr& value );

            /* _________ Tenths minOccurs = 1, maxOccurs = 1 _________ */
            TenthsPtr getTenths() const;
            void setTenths( const TenthsPtr& value );

        private:
            MillimetersPtr myMillimeters;
            TenthsPtr myTenths;
        };
        
        
        class Appearance;
        using AppearancePtr = std::shared_ptr<Appearance>;
        using AppearanceUPtr = std::unique_ptr<Appearance>;
        using AppearanceSet = std::vector<AppearancePtr>;
        using AppearanceSetIter = AppearanceSet::iterator;
        using AppearanceSetIterConst = AppearanceSet::const_iterator;
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

        private:
            LineWidthSet myLineWidthSet;
            NoteSizeSet myNoteSizeSet;
            DistanceSet myDistanceSet;
            OtherAppearanceSet myOtherAppearanceSet;
        };
        
        class VirtualInstrument;
        using VirtualInstrumentPtr = std::shared_ptr<VirtualInstrument>;
        using VirtualInstrumentUPtr = std::unique_ptr<VirtualInstrument>;
        using VirtualInstrumentSet = std::vector<VirtualInstrumentPtr>;
        using VirtualInstrumentSetIter = VirtualInstrumentSet::iterator;
        using VirtualInstrumentSetIterConst = VirtualInstrumentSet::const_iterator;
        inline VirtualInstrumentPtr makeVirtualInstrument() { return std::make_shared<VirtualInstrument>(); }

        class VirtualInstrument : public ElementInterface
        {
        public:
            VirtualInstrument();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ VirtualLibrary minOccurs = 0, maxOccurs = 1 _________ */
            VirtualLibraryPtr getVirtualLibrary() const;
            void setVirtualLibrary( const VirtualLibraryPtr& value );
            bool getHasVirtualLibrary() const;
            void setHasVirtualLibrary( const bool value );

            /* _________ VirtualName minOccurs = 0, maxOccurs = 1 _________ */
            VirtualNamePtr getVirtualName() const;
            void setVirtualName( const VirtualNamePtr& value );
            bool getHasVirtualName() const;
            void setHasVirtualName( const bool value );

        private:
            VirtualLibraryPtr myVirtualLibrary;
            bool myHasVirtualLibrary;
            VirtualNamePtr myVirtualName;
            bool myHasVirtualName;
        };
        
        
        class Root;
        using RootPtr = std::shared_ptr<Root>;
        using RootUPtr = std::unique_ptr<Root>;
        using RootSet = std::vector<RootPtr>;
        using RootSetIter = RootSet::iterator;
        using RootSetIterConst = RootSet::const_iterator;
        inline RootPtr makeRoot() { return std::make_shared<Root>(); }

        class Root : public ElementInterface
        {
        public:
            Root();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ RootStep minOccurs = 1, maxOccurs = 1 _________ */
            RootStepPtr getRootStep() const;
            void setRootStep( const RootStepPtr& value );

            /* _________ RootAlter minOccurs = 0, maxOccurs = 1 _________ */
            RootAlterPtr getRootAlter() const;
            void setRootAlter( const RootAlterPtr& value );
            bool getHasRootAlter() const;
            void setHasRootAlter( const bool value );

        private:
            RootStepPtr myRootStep;
            RootAlterPtr myRootAlter;
            bool myHasRootAlter;
        };
        
        
        class Bass;
        using BassPtr = std::shared_ptr<Bass>;
        using BassUPtr = std::unique_ptr<Bass>;
        using BassSet = std::vector<BassPtr>;
        using BassSetIter = BassSet::iterator;
        using BassSetIterConst = BassSet::const_iterator;
        inline BassPtr makeBass() { return std::make_shared<Bass>(); }

        class Bass : public ElementInterface
        {
        public:
            Bass();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ BassStep minOccurs = 1, maxOccurs = 1 _________ */
            BassStepPtr getBassStep() const;
            void setBassStep( const BassStepPtr& value );

            /* _________ BassAlter minOccurs = 0, maxOccurs = 1 _________ */
            BassAlterPtr getBassAlter() const;
            void setBassAlter( const BassAlterPtr& value );
            bool getHasBassAlter() const;
            void setHasBassAlter( const bool value );

        private:
            BassStepPtr myBassStep;
            BassAlterPtr myBassAlter;
            bool myHasBassAlter;
        };
        
        
        struct DegreeAttributes;
        using DegreeAttributesPtr = std::shared_ptr<DegreeAttributes>;
        
        struct DegreeAttributes : public AttributesInterface
        {
        public:
            DegreeAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo printObject;
            bool hasPrintObject;
        };
        
        class Degree;
        using DegreePtr = std::shared_ptr<Degree>;
        using DegreeUPtr = std::unique_ptr<Degree>;
        using DegreeSet = std::vector<DegreePtr>;
        using DegreeSetIter = DegreeSet::iterator;
        using DegreeSetIterConst = DegreeSet::const_iterator;
        inline DegreePtr makeDegree() { return std::make_shared<Degree>(); }

        class Degree : public ElementInterface
        {
        public:
            Degree();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DegreeAttributesPtr getAttributes() const;
            void setAttributes( const DegreeAttributesPtr& value );

            /* _________ DegreeValue minOccurs = 1, maxOccurs = 1 _________ */
            DegreeValuePtr getDegreeValue() const;
            void setDegreeValue( const DegreeValuePtr& value );

            /* _________ DegreeAlter minOccurs = 1, maxOccurs = 1 _________ */
            DegreeAlterPtr getDegreeAlter() const;
            void setDegreeAlter( const DegreeAlterPtr& value );

            /* _________ DegreeType minOccurs = 1, maxOccurs = 1 _________ */
            DegreeTypePtr getDegreeType() const;
            void setDegreeType( const DegreeTypePtr& value );

        private:
            DegreeAttributesPtr myAttributes;
            DegreeValuePtr myDegreeValue;
            DegreeAlterPtr myDegreeAlter;
            DegreeTypePtr myDegreeType;
        };
        
        class PageLayout;
        using PageLayoutPtr = std::shared_ptr<PageLayout>;
        using PageLayoutUPtr = std::unique_ptr<PageLayout>;
        using PageLayoutSet = std::vector<PageLayoutPtr>;
        using PageLayoutSetIter = PageLayoutSet::iterator;
        using PageLayoutSetIterConst = PageLayoutSet::const_iterator;
        inline PageLayoutPtr makePageLayout() { return std::make_shared<PageLayout>(); }

        class PageLayout : public ElementInterface
        {
        public:
            PageLayout();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ PageHeight minOccurs = 1, maxOccurs = 1 _________ */
            PageHeightPtr getPageHeight() const;
            void setPageHeight( const PageHeightPtr& value );

            /* _________ PageWidth minOccurs = 1, maxOccurs = 1 _________ */
            PageWidthPtr getPageWidth() const;
            void setPageWidth( const PageWidthPtr& value );

            /* _________ PageMargins minOccurs = 0, maxOccurs = 2 _________ */
            const PageMarginsSet& getPageMarginsSet() const;
            void addPageMargins( const PageMarginsPtr& value );
            void removePageMargins( const PageMarginsSetIterConst& value );
            void clearPageMarginsSet();
            PageMarginsPtr getPageMargins( const PageMarginsSetIterConst& setIterator ) const;

        private:
            PageHeightPtr myPageHeight;
            PageWidthPtr myPageWidth;
            PageMarginsSet myPageMarginsSet;
        };
        
        class SystemLayout;
        using SystemLayoutPtr = std::shared_ptr<SystemLayout>;
        using SystemLayoutUPtr = std::unique_ptr<SystemLayout>;
        using SystemLayoutSet = std::vector<SystemLayoutPtr>;
        using SystemLayoutSetIter = SystemLayoutSet::iterator;
        using SystemLayoutSetIterConst = SystemLayoutSet::const_iterator;
        inline SystemLayoutPtr makeSystemLayout() { return std::make_shared<SystemLayout>(); }

        class SystemLayout : public ElementInterface
        {
        public:
            SystemLayout();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ SystemMargins minOccurs = 0, maxOccurs = 1 _________ */
            SystemMarginsPtr getSystemMargins() const;
            void setSystemMargins( const SystemMarginsPtr& value );
            bool getHasSystemMargins() const;
            void setHasSystemMargins( const bool value );
            SystemDistancePtr getSystemDistance() const;
            void setSystemDistance( const SystemDistancePtr& value );
            bool getHasSystemDistance() const;
            void setHasSystemDistance( const bool value );

            /* _________ TopSystemDistance minOccurs = 0, maxOccurs = 1 _________ */
            TopSystemDistancePtr getTopSystemDistance() const;
            void setTopSystemDistance( const TopSystemDistancePtr& value );
            bool getHasTopSystemDistance() const;
            void setHasTopSystemDistance( const bool value );

            /* _________ SystemDividers minOccurs = 0, maxOccurs = 1 _________ */
            SystemDividersPtr getSystemDividers() const;
            void setSystemDividers( const SystemDividersPtr& value );
            bool getHasSystemDividers() const;
            void setHasSystemDividers( const bool value );

        private:
            SystemMarginsPtr mySystemMargins;
            bool myHasSystemMargins;
            SystemDistancePtr mySystemDistance;
            bool myHasSystemDistance;
            TopSystemDistancePtr myTopSystemDistance;
            bool myHasTopSystemDistance;
            SystemDividersPtr mySystemDividers;
            bool myHasSystemDividers;
        };
        
        struct StaffLayoutAttributes;
        using StaffLayoutAttributesPtr = std::shared_ptr<StaffLayoutAttributes>;
        
        struct StaffLayoutAttributes : public AttributesInterface
        {
        public:
            StaffLayoutAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StaffNumber number;
            bool hasNumber;
        };
        
        class StaffLayout;
        using StaffLayoutPtr = std::shared_ptr<StaffLayout>;
        using StaffLayoutUPtr = std::unique_ptr<StaffLayout>;
        using StaffLayoutSet = std::vector<StaffLayoutPtr>;
        using StaffLayoutSetIter = StaffLayoutSet::iterator;
        using StaffLayoutSetIterConst = StaffLayoutSet::const_iterator;
        inline StaffLayoutPtr makeStaffLayout() { return std::make_shared<StaffLayout>(); }

        class StaffLayout : public ElementInterface
        {
        public:
            StaffLayout();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StaffLayoutAttributesPtr getAttributes() const;
            void setAttributes( const StaffLayoutAttributesPtr& value );

            /* _________ StaffDistance minOccurs = 0, maxOccurs = 1 _________ */
            StaffDistancePtr getStaffDistance() const;
            void setStaffDistance( const StaffDistancePtr& value );
            bool getHasStaffDistance() const;
            void setHasStaffDistance( const bool value );

        private:
            StaffLayoutAttributesPtr myAttributes;
            StaffDistancePtr myStaffDistance;
            bool myHasStaffDistance;
        };
        
        class Pitch;
        using PitchPtr = std::shared_ptr<Pitch>;
        using PitchUPtr = std::unique_ptr<Pitch>;
        using PitchSet = std::vector<PitchPtr>;
        using PitchSetIter = PitchSet::iterator;
        using PitchSetIterConst = PitchSet::const_iterator;
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

        private:
            StepPtr myStep;
            AlterPtr myAlter;
            bool myHasAlter;
            OctavePtr myOctave;
        };
        
        class DisplayStepOctaveGroup;
        using DisplayStepOctaveGroupPtr = std::shared_ptr<DisplayStepOctaveGroup>;
        using DisplayStepOctaveGroupUPtr = std::unique_ptr<DisplayStepOctaveGroup>;
        using DisplayStepOctaveGroupSet = std::vector<DisplayStepOctaveGroupPtr>;
        using DisplayStepOctaveGroupSetIter = DisplayStepOctaveGroupSet::iterator;
        using DisplayStepOctaveGroupSetIterConst = DisplayStepOctaveGroupSet::const_iterator;
        inline DisplayStepOctaveGroupPtr makeDisplayStepOctaveGroup() { return std::make_shared<DisplayStepOctaveGroup>(); }

        class DisplayStepOctaveGroup : public ElementInterface
        {
        public:
            DisplayStepOctaveGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ DisplayStep minOccurs = 1, maxOccurs = 1 _________ */
            DisplayStepPtr getDisplayStep() const;
            void setDisplayStep( const DisplayStepPtr& value );

            /* _________ DisplayOctave minOccurs = 1, maxOccurs = 1 _________ */
            DisplayOctavePtr getDisplayOctave() const;
            void setDisplayOctave( const DisplayOctavePtr& value );

        private:
            DisplayStepPtr myDisplayStep;
            DisplayOctavePtr myDisplayOctave;
        };
        
        
        class Unpitched;
        using UnpitchedPtr = std::shared_ptr<Unpitched>;
        using UnpitchedUPtr = std::unique_ptr<Unpitched>;
        using UnpitchedSet = std::vector<UnpitchedPtr>;
        using UnpitchedSetIter = UnpitchedSet::iterator;
        using UnpitchedSetIterConst = UnpitchedSet::const_iterator;
        inline UnpitchedPtr makeUnpitched() { return std::make_shared<Unpitched>(); }

        class Unpitched : public ElementInterface
        {
        public:
            Unpitched();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ DisplayStepOctaveGroup minOccurs = 0, maxOccurs = 1 _________ */
            DisplayStepOctaveGroupPtr getDisplayStepOctaveGroup() const;
            void setDisplayStepOctaveGroup( const DisplayStepOctaveGroupPtr& value );
            bool getHasDisplayStepOctaveGroup() const;
            void setHasDisplayStepOctaveGroup( const bool value );

        private:
            DisplayStepOctaveGroupPtr myDisplayStepOctaveGroup;
            bool myHasDisplayStepOctaveGroup;
        };
        
        
        struct RestAttributes;
        using RestAttributesPtr = std::shared_ptr<RestAttributes>;
        
        struct RestAttributes : public AttributesInterface
        {
        public:
            RestAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo measure;
            bool hasMeasure;
        };
        
        class Rest;
        using RestPtr = std::shared_ptr<Rest>;
        using RestUPtr = std::unique_ptr<Rest>;
        using RestSet = std::vector<RestPtr>;
        using RestSetIter = RestSet::iterator;
        using RestSetIterConst = RestSet::const_iterator;
        inline RestPtr makeRest() { return std::make_shared<Rest>(); }

        class Rest : public ElementInterface
        {
        public:
            Rest();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RestAttributesPtr getAttributes() const;
            void setAttributes( const RestAttributesPtr& value );

            /* _________ DisplayStepOctaveGroup minOccurs = 0, maxOccurs = 1 _________ */
            DisplayStepOctaveGroupPtr getDisplayStepOctaveGroup() const;
            void setDisplayStepOctaveGroup( const DisplayStepOctaveGroupPtr& value );
            bool getHasDisplayStepOctaveGroup() const;
            void setHasDisplayStepOctaveGroup( const bool value );

        private:
            RestAttributesPtr myAttributes;
            DisplayStepOctaveGroupPtr myDisplayStepOctaveGroup;
            bool myHasDisplayStepOctaveGroup;
        };
        
        
        class Backup;
        using BackupPtr = std::shared_ptr<Backup>;
        using BackupUPtr = std::unique_ptr<Backup>;
        using BackupSet = std::vector<BackupPtr>;
        using BackupSetIter = BackupSet::iterator;
        using BackupSetIterConst = BackupSet::const_iterator;
        inline BackupPtr makeBackup() { return std::make_shared<Backup>(); }

        class Backup : public ElementInterface
        {
        public:
            Backup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Duration minOccurs = 1, maxOccurs = 1 _________ */
            DurationPtr getDuration() const;
            void setDuration( const DurationPtr& value );

            /* _________ EditorialGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialGroupPtr getEditorialGroup() const;
            void setEditorialGroup( const EditorialGroupPtr& value );

        private:
            DurationPtr myDuration;
            EditorialGroupPtr myEditorialGroup;
        };
        
        
        class Forward;
        using ForwardPtr = std::shared_ptr<Forward>;
        using ForwardUPtr = std::unique_ptr<Forward>;
        using ForwardSet = std::vector<ForwardPtr>;
        using ForwardSetIter = ForwardSet::iterator;
        using ForwardSetIterConst = ForwardSet::const_iterator;
        inline ForwardPtr makeForward() { return std::make_shared<Forward>(); }

        class Forward : public ElementInterface
        {
        public:
            Forward();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Duration minOccurs = 1, maxOccurs = 1 _________ */
            DurationPtr getDuration() const;
            void setDuration( const DurationPtr& value );

            /* _________ Footnote minOccurs = 1, maxOccurs = 1 _________ */
            EditorialVoiceGroupPtr getEditorialVoiceGroup() const;
            void setEditorialVoiceGroup( const EditorialVoiceGroupPtr& value );

            /* _________ Staff minOccurs = 0, maxOccurs = 1 _________ */
            StaffPtr getStaff() const;
            void setStaff( const StaffPtr& value );
            bool getHasStaff() const;
            void setHasStaff( const bool value );

        private:
            DurationPtr myDuration;
            EditorialVoiceGroupPtr myEditorialVoiceGroup;
            StaffPtr myStaff;
            bool myHasStaff;
        };
        
        struct FiguredBassAttributes;
        using FiguredBassAttributesPtr = std::shared_ptr<FiguredBassAttributes>;
        
        struct FiguredBassAttributes : public AttributesInterface
        {
        public:
            FiguredBassAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            YesNo printObject;
            YesNo printDot;
            YesNo printSpacing;
            YesNo printLyric;
            YesNo parentheses;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasPrintObject;
            bool hasPrintDot;
            bool hasPrintSpacing;
            bool hasPrintLyric;
            bool hasParentheses;
        };
        
        class FiguredBass;
        using FiguredBassPtr = std::shared_ptr<FiguredBass>;
        using FiguredBassUPtr = std::unique_ptr<FiguredBass>;
        using FiguredBassSet = std::vector<FiguredBassPtr>;
        using FiguredBassSetIter = FiguredBassSet::iterator;
        using FiguredBassSetIterConst = FiguredBassSet::const_iterator;
        inline FiguredBassPtr makeFiguredBass() { return std::make_shared<FiguredBass>(); }

        class FiguredBass : public ElementInterface
        {
        public:
            FiguredBass();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FiguredBassAttributesPtr getAttributes() const;
            void setAttributes( const FiguredBassAttributesPtr& value );

            /* _________ Figure minOccurs = 1, maxOccurs = unbounded _________ */
            const FigureSet& getFigureSet() const;
            void addFigure( const FigurePtr& value );
            void removeFigure( const FigureSetIterConst& value );
            void clearFigureSet();
            FigurePtr getFigure( const FigureSetIterConst& setIterator ) const;

            /* _________ Duration minOccurs = 0, maxOccurs = 1 _________ */
            DurationPtr getDuration() const;
            void setDuration( const DurationPtr& value );
            bool getHasDuration() const;
            void setHasDuration( const bool value );

            /* _________ EditorialGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialGroupPtr getEditorialGroup() const;
            void setEditorialGroup( const EditorialGroupPtr& value );

        private:
            FiguredBassAttributesPtr myAttributes;
            FigureSet myFigureSet;
            DurationPtr myDuration;
            bool myHasDuration;
            EditorialGroupPtr myEditorialGroup;
        };
        
        
        struct BarlineAttributes;
        using BarlineAttributesPtr = std::shared_ptr<BarlineAttributes>;
        
        struct BarlineAttributes : public AttributesInterface
        {
        public:
            BarlineAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            RightLeftMiddle location;
            XsToken segno;
            XsToken coda;
            DivisionsValue divisions;
            bool hasLocation;
            bool hasSegno;
            bool hasCoda;
            bool hasDivisions;
        };
        
        class Barline;
        using BarlinePtr = std::shared_ptr<Barline>;
        using BarlineUPtr = std::unique_ptr<Barline>;
        using BarlineSet = std::vector<BarlinePtr>;
        using BarlineSetIter = BarlineSet::iterator;
        using BarlineSetIterConst = BarlineSet::const_iterator;
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
        
        
        struct GroupingAttributes;
        using GroupingAttributesPtr = std::shared_ptr<GroupingAttributes>;
        
        struct GroupingAttributes : public AttributesInterface
        {
        public:
            GroupingAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStopSingle type;
            XsToken number;
            XsToken memberOf;
            const 	bool hasType;
            bool hasNumber;
            bool hasMemberOf;
        };
        
        class Grouping;
        using GroupingPtr = std::shared_ptr<Grouping>;
        using GroupingUPtr = std::unique_ptr<Grouping>;
        using GroupingSet = std::vector<GroupingPtr>;
        using GroupingSetIter = GroupingSet::iterator;
        using GroupingSetIterConst = GroupingSet::const_iterator;
        inline GroupingPtr makeGrouping() { return std::make_shared<Grouping>(); }

        class Grouping : public ElementInterface
        {
        public:
            Grouping();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            GroupingAttributesPtr getAttributes() const;
            void setAttributes( const GroupingAttributesPtr& value );

            /* _________ Feature minOccurs = 0, maxOccurs = unbounded _________ */
            const FeatureSet& getFeatureSet() const;
            void addFeature( const FeaturePtr& value );
            void removeFeature( const FeatureSetIterConst& value );
            void clearFeatureSet();
            FeaturePtr getFeature( const FeatureSetIterConst& setIterator ) const;

        private:
            GroupingAttributesPtr myAttributes;
            FeatureSet myFeatureSet;
        };
        
        class Work;
        using WorkPtr = std::shared_ptr<Work>;
        using WorkUPtr = std::unique_ptr<Work>;
        using WorkSet = std::vector<WorkPtr>;
        using WorkSetIter = WorkSet::iterator;
        using WorkSetIterConst = WorkSet::const_iterator;
        inline WorkPtr makeWork() { return std::make_shared<Work>(); }

        class Work : public ElementInterface
        {
        public:
            Work();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ WorkNumber minOccurs = 0, maxOccurs = 1 _________ */
            WorkNumberPtr getWorkNumber() const;
            void setWorkNumber( const WorkNumberPtr& value );
            bool getHasWorkNumber() const;
            void setHasWorkNumber( const bool value );

            /* _________ WorkTitle minOccurs = 0, maxOccurs = 1 _________ */
            WorkTitlePtr getWorkTitle() const;
            void setWorkTitle( const WorkTitlePtr& value );
            bool getHasWorkTitle() const;
            void setHasWorkTitle( const bool value );

            /* _________ Opus minOccurs = 0, maxOccurs = 1 _________ */
            OpusPtr getOpus() const;
            void setOpus( const OpusPtr& value );
            bool getHasOpus() const;
            void setHasOpus( const bool value );

        private:
            WorkNumberPtr myWorkNumber;
            bool myHasWorkNumber;
            WorkTitlePtr myWorkTitle;
            bool myHasWorkTitle;
            OpusPtr myOpus;
            bool myHasOpus;
        };
        
        class LayoutGroup;
        using LayoutGroupPtr = std::shared_ptr<LayoutGroup>;
        using LayoutGroupUPtr = std::unique_ptr<LayoutGroup>;
        using LayoutGroupSet = std::vector<LayoutGroupPtr>;
        using LayoutGroupSetIter = LayoutGroupSet::iterator;
        using LayoutGroupSetIterConst = LayoutGroupSet::const_iterator;
        inline LayoutGroupPtr makeLayoutGroup() { return std::make_shared<LayoutGroup>(); }

        class LayoutGroup : public ElementInterface
        {
        public:
            LayoutGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ PageLayout minOccurs = 0, maxOccurs = 1 _________ */
            PageLayoutPtr getPageLayout() const;
            void setPageLayout( const PageLayoutPtr& value );
            bool getHasPageLayout() const;
            void setHasPageLayout( const bool value );

            /* _________ SystemLayout minOccurs = 0, maxOccurs = 1 _________ */
            SystemLayoutPtr getSystemLayout() const;
            void setSystemLayout( const SystemLayoutPtr& value );
            bool getHasSystemLayout() const;
            void setHasSystemLayout( const bool value );

            /* _________ StaffLayout minOccurs = 0, maxOccurs = unbounded _________ */
            const StaffLayoutSet& getStaffLayoutSet() const;
            void addStaffLayout( const StaffLayoutPtr& value );
            void removeStaffLayout( const StaffLayoutSetIterConst& value );
            void clearStaffLayoutSet();
            StaffLayoutPtr getStaffLayout( const StaffLayoutSetIterConst& setIterator ) const;

        private:
            PageLayoutPtr myPageLayout;
            bool myHasPageLayout;
            SystemLayoutPtr mySystemLayout;
            bool myHasSystemLayout;
            StaffLayoutSet myStaffLayoutSet;
        };
        
        
        class Defaults;
        using DefaultsPtr = std::shared_ptr<Defaults>;
        using DefaultsUPtr = std::unique_ptr<Defaults>;
        using DefaultsSet = std::vector<DefaultsPtr>;
        using DefaultsSetIter = DefaultsSet::iterator;
        using DefaultsSetIterConst = DefaultsSet::const_iterator;
        inline DefaultsPtr makeDefaults() { return std::make_shared<Defaults>(); }

        class Defaults : public ElementInterface
        {
        public:
            Defaults();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Scaling minOccurs = 0, maxOccurs = 1 _________ */
            ScalingPtr getScaling() const;
            void setScaling( const ScalingPtr& value );
            bool getHasScaling() const;
            void setHasScaling( const bool value );

            /* _________ LayoutGroup minOccurs = 1, maxOccurs = 1 _________ */
            LayoutGroupPtr getLayoutGroup() const;
            void setLayoutGroup( const LayoutGroupPtr& value );

            /* _________ Appearance minOccurs = 0, maxOccurs = 1 _________ */
            AppearancePtr getAppearance() const;
            void setAppearance( const AppearancePtr& value );
            bool getHasAppearance() const;
            void setHasAppearance( const bool value );

            /* _________ MusicFont minOccurs = 0, maxOccurs = 1 _________ */
            MusicFontPtr getMusicFont() const;
            void setMusicFont( const MusicFontPtr& value );
            bool getHasMusicFont() const;
            void setHasMusicFont( const bool value );

            /* _________ WordFont minOccurs = 0, maxOccurs = 1 _________ */
            WordFontPtr getWordFont() const;
            void setWordFont( const WordFontPtr& value );
            bool getHasWordFont() const;
            void setHasWordFont( const bool value );

            /* _________ LyricFont minOccurs = 0, maxOccurs = unbounded _________ */
            const LyricFontSet& getLyricFontSet() const;
            void addLyricFont( const LyricFontPtr& value );
            void removeLyricFont( const LyricFontSetIterConst& value );
            void clearLyricFontSet();
            LyricFontPtr getLyricFont( const LyricFontSetIterConst& setIterator ) const;

            /* _________ LyricLanguage minOccurs = 0, maxOccurs = unbounded _________ */
            const LyricLanguageSet& getLyricLanguageSet() const;
            void addLyricLanguage( const LyricLanguagePtr& value );
            void removeLyricLanguage( const LyricLanguageSetIterConst& value );
            void clearLyricLanguageSet();
            LyricLanguagePtr getLyricLanguage( const LyricLanguageSetIterConst& setIterator ) const;

        private:
            ScalingPtr myScaling;
            bool myHasScaling;
            LayoutGroupPtr myLayoutGroup;
            AppearancePtr myAppearance;
            bool myHasAppearance;
            MusicFontPtr myMusicFont;
            bool myHasMusicFont;
            WordFontPtr myWordFont;
            bool myHasWordFont;
            LyricFontSet myLyricFontSet;
            LyricLanguageSet myLyricLanguageSet;
        };
        
        
        class OrnamentsChoice;
        using OrnamentsChoicePtr = std::shared_ptr<OrnamentsChoice>;
        using OrnamentsChoiceUPtr = std::unique_ptr<OrnamentsChoice>;
        using OrnamentsChoiceSet = std::vector<OrnamentsChoicePtr>;
        using OrnamentsChoiceSetIter = OrnamentsChoiceSet::iterator;
        using OrnamentsChoiceSetIterConst = OrnamentsChoiceSet::const_iterator;
        inline OrnamentsChoicePtr makeOrnamentsChoice() { return std::make_shared<OrnamentsChoice>(); }

        class OrnamentsChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                trillMark = 1,
                turn = 2,
                delayedTurn = 3,
                invertedTurn = 4,
                delayedInvertedTurn = 5,
                verticalTurn = 6,
                shake = 7,
                wavyLine = 8,
                mordent = 9,
                invertedMordent = 10,
                schleifer = 11,
                tremolo = 12,
                otherOrnament = 13
            };
            OrnamentsChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice enum _________ */
            OrnamentsChoice::Choice getChoice() const;
            void setChoice( const OrnamentsChoice::Choice value );

            /* _________ TrillMark minOccurs = 1, maxOccurs = 1 _________ */
            TrillMarkPtr getTrillMark() const;
            void setTrillMark( const TrillMarkPtr& value );

            /* _________ Turn minOccurs = 1, maxOccurs = 1 _________ */
            TurnPtr getTurn() const;
            void setTurn( const TurnPtr& value );

            /* _________ DelayedTurn minOccurs = 1, maxOccurs = 1 _________ */
            DelayedTurnPtr getDelayedTurn() const;
            void setDelayedTurn( const DelayedTurnPtr& value );

            /* _________ InvertedTurn minOccurs = 1, maxOccurs = 1 _________ */
            InvertedTurnPtr getInvertedTurn() const;
            void setInvertedTurn( const InvertedTurnPtr& value );

            /* _________ DelayedInvertedTurn minOccurs = 1, maxOccurs = 1 _________ */
            DelayedInvertedTurnPtr getDelayedInvertedTurn() const;
            void setDelayedInvertedTurn( const DelayedInvertedTurnPtr& value );

            /* _________ VerticalTurn minOccurs = 1, maxOccurs = 1 _________ */
            VerticalTurnPtr getVerticalTurn() const;
            void setVerticalTurn( const VerticalTurnPtr& value );

            /* _________ Shake minOccurs = 1, maxOccurs = 1 _________ */
            ShakePtr getShake() const;
            void setShake( const ShakePtr& value );

            /* _________ WavyLine minOccurs = 1, maxOccurs = 1 _________ */
            WavyLinePtr getWavyLine() const;
            void setWavyLine( const WavyLinePtr& value );

            /* _________ Mordent minOccurs = 1, maxOccurs = 1 _________ */
            MordentPtr getMordent() const;
            void setMordent( const MordentPtr& value );

            /* _________ InvertedMordent minOccurs = 1, maxOccurs = 1 _________ */
            InvertedMordentPtr getInvertedMordent() const;
            void setInvertedMordent( const InvertedMordentPtr& value );

            /* _________ Schleifer minOccurs = 1, maxOccurs = 1 _________ */
            SchleiferPtr getSchleifer() const;
            void setSchleifer( const SchleiferPtr& value );

            /* _________ Tremolo minOccurs = 1, maxOccurs = 1 _________ */
            TremoloPtr getTremolo() const;
            void setTremolo( const TremoloPtr& value );

            /* _________ OtherOrnament minOccurs = 1, maxOccurs = 1 _________ */
            OtherOrnamentPtr getOtherOrnament() const;
            void setOtherOrnament( const OtherOrnamentPtr& value );

        private:
            Choice myChoice;
            TrillMarkPtr myTrillMark;
            TurnPtr myTurn;
            DelayedTurnPtr myDelayedTurn;
            InvertedTurnPtr myInvertedTurn;
            DelayedInvertedTurnPtr myDelayedInvertedTurn;
            VerticalTurnPtr myVerticalTurn;
            ShakePtr myShake;
            WavyLinePtr myWavyLine;
            MordentPtr myMordent;
            InvertedMordentPtr myInvertedMordent;
            SchleiferPtr mySchleifer;
            TremoloPtr myTremolo;
            OtherOrnamentPtr myOtherOrnament;
        };
        class Ornaments;
        using OrnamentsPtr = std::shared_ptr<Ornaments>;
        using OrnamentsUPtr = std::unique_ptr<Ornaments>;
        using OrnamentsSet = std::vector<OrnamentsPtr>;
        using OrnamentsSetIter = OrnamentsSet::iterator;
        using OrnamentsSetIterConst = OrnamentsSet::const_iterator;
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

            /* _________ OrnamentsChoice minOccurs = 1, maxOccurs = 1 _________ */
            OrnamentsChoicePtr getOrnamentsChoice() const;
            void setOrnamentsChoice( const OrnamentsChoicePtr& value );

            /* _________ AccidentalMark minOccurs = 0, maxOccurs = unbounded _________ */
            const AccidentalMarkSet& getAccidentalMarkSet() const;
            void addAccidentalMark( const AccidentalMarkPtr& value );
            void removeAccidentalMark( const AccidentalMarkSetIterConst& value );
            void clearAccidentalMarkSet();
            AccidentalMarkPtr getAccidentalMark( const AccidentalMarkSetIterConst& setIterator ) const;

        private:
            OrnamentsChoicePtr myOrnamentsChoice;
            AccidentalMarkSet myAccidentalMarkSet;
        };
        
        
        class BendChoice;
        using BendChoicePtr = std::shared_ptr<BendChoice>;
        using BendChoiceUPtr = std::unique_ptr<BendChoice>;
        using BendChoiceSet = std::vector<BendChoicePtr>;
        using BendChoiceSetIter = BendChoiceSet::iterator;
        using BendChoiceSetIterConst = BendChoiceSet::const_iterator;
        inline BendChoicePtr makeBendChoice() { return std::make_shared<BendChoice>(); }

        class BendChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                preBend = 1,
                release = 2
            };
            BendChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BendChoice::Choice getChoice() const;
            void setChoice( BendChoice::Choice value );

            /* _________ PreBend minOccurs = 1, maxOccurs = 1 _________ */
            PreBendPtr getPreBend() const;
            void setPreBend( const PreBendPtr& value );

            /* _________ Release minOccurs = 1, maxOccurs = 1 _________ */
            ReleasePtr getRelease() const;
            void setRelease( const ReleasePtr& value );

        private:
            Choice myChoice;
            PreBendPtr myPreBend;
            ReleasePtr myRelease;
        };
        
        
        struct BendAttributes;
        using BendAttributesPtr = std::shared_ptr<BendAttributes>;
        
        struct BendAttributes : public AttributesInterface
        {
        public:
            BendAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            YesNo accelerate;
            TrillBeats beats;
            Percent firstBeat;
            Percent lastBeat;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasAccelerate;
            bool hasBeats;
            bool hasFirstBeat;
            bool hasLastBeat;
        };
        
        class Bend;
        using BendPtr = std::shared_ptr<Bend>;
        using BendUPtr = std::unique_ptr<Bend>;
        using BendSet = std::vector<BendPtr>;
        using BendSetIter = BendSet::iterator;
        using BendSetIterConst = BendSet::const_iterator;
        inline BendPtr makeBend() { return std::make_shared<Bend>(); }

        class Bend : public ElementInterface
        {
        public:
            Bend();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BendAttributesPtr getAttributes() const;
            void setAttributes( const BendAttributesPtr& value );

            /* _________ BendAlter minOccurs = 1, maxOccurs = 1 _________ */
            BendAlterPtr getBendAlter() const;
            void setBendAlter( const BendAlterPtr& value );

            /* _________ BendChoice minOccurs = 1, maxOccurs = 1 _________ */
            BendChoicePtr getBendChoice() const;
            void setBendChoice( const BendChoicePtr& value );

            /* _________ WithBar minOccurs = 0, maxOccurs = 1 _________ */
            WithBarPtr getWithBar() const;
            void setWithBar( const WithBarPtr& value );
            bool getHasWithBar() const;
            void setHasWithBar( const bool value );

        private:
            BendAttributesPtr myAttributes;
            BendAlterPtr myBendAlter;
            BendChoicePtr myBendChoice;
            WithBarPtr myWithBar;
            bool myHasWithBar;
        };
        
        class ArrowGroup;
        using ArrowGroupPtr = std::shared_ptr<ArrowGroup>;
        using ArrowGroupUPtr = std::unique_ptr<ArrowGroup>;
        using ArrowGroupSet = std::vector<ArrowGroupPtr>;
        using ArrowGroupSetIter = ArrowGroupSet::iterator;
        using ArrowGroupSetIterConst = ArrowGroupSet::const_iterator;
        inline ArrowGroupPtr makeArrowGroup() { return std::make_shared<ArrowGroup>(); }

        class ArrowGroup : public ElementInterface
        {
        public:
            ArrowGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ ArrowDirection minOccurs = 1, maxOccurs = 1 _________ */
            ArrowDirectionPtr getArrowDirection() const;
            void setArrowDirection( const ArrowDirectionPtr& value );

            /* _________ ArrowStyle minOccurs = 0, maxOccurs = 1 _________ */
            ArrowStylePtr getArrowStyle() const;
            void setArrowStyle( const ArrowStylePtr& value );
            bool getHasArrowStyle() const;
            void setHasArrowStyle( const bool value );

        private:
            ArrowDirectionPtr myArrowDirection;
            ArrowStylePtr myArrowStyle;
            bool myHasArrowStyle;
        };
        
        struct ArrowAttributes;
        using ArrowAttributesPtr = std::shared_ptr<ArrowAttributes>;
        
        struct ArrowAttributes : public AttributesInterface
        {
        public:
            ArrowAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            AboveBelow placement;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasPlacement;
        };
        
        class Arrow;
        using ArrowPtr = std::shared_ptr<Arrow>;
        using ArrowUPtr = std::unique_ptr<Arrow>;
        using ArrowSet = std::vector<ArrowPtr>;
        using ArrowSetIter = ArrowSet::iterator;
        using ArrowSetIterConst = ArrowSet::const_iterator;
        inline ArrowPtr makeArrow() { return std::make_shared<Arrow>(); }

        class Arrow : public ElementInterface
        {
        public:
            enum class Choice
            {
                arrowGroup = 1,
                circularArrow = 2
            };
            Arrow();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ArrowAttributesPtr getAttributes() const;
            void setAttributes( const ArrowAttributesPtr& value );

            /* _________ Choice _________ */
            Arrow::Choice getChoice() const;
            void setChoice( const Arrow::Choice value );

            /* _________ ArrowGroup minOccurs = 1, maxOccurs = 1 _________ */
            ArrowGroupPtr getArrowGroup() const;
            void setArrowGroup( const ArrowGroupPtr& value );

            /* _________ CircularArrow minOccurs = 1, maxOccurs = 1 _________ */
            CircularArrowPtr getCircularArrow() const;
            void setCircularArrow( const CircularArrowPtr& value );

        private:
            Choice myChoice;
            ArrowAttributesPtr myAttributes;
            ArrowGroupPtr myArrowGroup;
            CircularArrowPtr myCircularArrow;
        };
        
        struct GroupNameDisplayAttributes;
        using GroupNameDisplayAttributesPtr = std::shared_ptr<GroupNameDisplayAttributes>;
        
        struct GroupNameDisplayAttributes : public AttributesInterface
        {
        public:
            GroupNameDisplayAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo printObject;
            bool hasPrintObject;
        };
        
        class GroupNameDisplay;
        using GroupNameDisplayPtr = std::shared_ptr<GroupNameDisplay>;
        using GroupNameDisplayUPtr = std::unique_ptr<GroupNameDisplay>;
        using GroupNameDisplaySet = std::vector<GroupNameDisplayPtr>;
        using GroupNameDisplaySetIter = GroupNameDisplaySet::iterator;
        using GroupNameDisplaySetIterConst = GroupNameDisplaySet::const_iterator;
        inline GroupNameDisplayPtr makeGroupNameDisplay() { return std::make_shared<GroupNameDisplay>(); }

        class GroupNameDisplay : public ElementInterface
        {
        public:
            enum class Choice
            {
                displayText = 1,
                accidentalText = 2
            };
            GroupNameDisplay();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            GroupNameDisplayAttributesPtr getAttributes() const;
            void setAttributes( const GroupNameDisplayAttributesPtr& value );

            /* _________ Choice _________ */
            GroupNameDisplay::Choice getChoice() const;
            void setChoice( const GroupNameDisplay::Choice value );

            /* _________ DisplayText minOccurs = 1, maxOccurs = 1 _________ */
            DisplayTextPtr getDisplayText() const;
            void setDisplayText( const DisplayTextPtr& value );

            /* _________ AccidentalText minOccurs = 1, maxOccurs = 1 _________ */
            AccidentalTextPtr getAccidentalText() const;
            void setAccidentalText( const AccidentalTextPtr& value );

        private:
            GroupNameDisplayAttributesPtr myAttributes;
            Choice myChoice;
            DisplayTextPtr myDisplayText;
            AccidentalTextPtr myAccidentalText;
        };
        
        
        struct GroupAbbreviationDisplayAttributes;
        using GroupAbbreviationDisplayAttributesPtr = std::shared_ptr<GroupAbbreviationDisplayAttributes>;
        
        struct GroupAbbreviationDisplayAttributes : public AttributesInterface
        {
        public:
            GroupAbbreviationDisplayAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo printObject;
            bool hasPrintObject;
        };
        
        class GroupAbbreviationDisplay;
        using GroupAbbreviationDisplayPtr = std::shared_ptr<GroupAbbreviationDisplay>;
        using GroupAbbreviationDisplayUPtr = std::unique_ptr<GroupAbbreviationDisplay>;
        using GroupAbbreviationDisplaySet = std::vector<GroupAbbreviationDisplayPtr>;
        using GroupAbbreviationDisplaySetIter = GroupAbbreviationDisplaySet::iterator;
        using GroupAbbreviationDisplaySetIterConst = GroupAbbreviationDisplaySet::const_iterator;
        inline GroupAbbreviationDisplayPtr makeGroupAbbreviationDisplay() { return std::make_shared<GroupAbbreviationDisplay>(); }

        class GroupAbbreviationDisplay : public ElementInterface
        {
        public:
            enum class Choice
            {
                displayText = 1,
                accidentalText = 2
            };
            GroupAbbreviationDisplay();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            GroupAbbreviationDisplayAttributesPtr getAttributes() const;
            void setAttributes( const GroupAbbreviationDisplayAttributesPtr& value );

            /* _________ Choice _________ */
            GroupAbbreviationDisplay::Choice getChoice() const;
            void setChoice( const GroupAbbreviationDisplay::Choice value );

            /* _________ DisplayText minOccurs = 1, maxOccurs = 1 _________ */
            DisplayTextPtr getDisplayText() const;
            void setDisplayText( const DisplayTextPtr& value );

            /* _________ AccidentalText minOccurs = 1, maxOccurs = 1 _________ */
            AccidentalTextPtr getAccidentalText() const;
            void setAccidentalText( const AccidentalTextPtr& value );

        private:
            GroupAbbreviationDisplayAttributesPtr myAttributes;
            Choice myChoice;
            DisplayTextPtr myDisplayText;
            AccidentalTextPtr myAccidentalText;
        };
        
        class Identification;
        using IdentificationPtr = std::shared_ptr<Identification>;
        using IdentificationUPtr = std::unique_ptr<Identification>;
        using IdentificationSet = std::vector<IdentificationPtr>;
        using IdentificationSetIter = IdentificationSet::iterator;
        using IdentificationSetIterConst = IdentificationSet::const_iterator;
        inline IdentificationPtr makeIdentification() { return std::make_shared<Identification>(); }

        class Identification : public ElementInterface
        {
        public:
            Identification();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Creator minOccurs = 0, maxOccurs = unbounded _________ */
            const CreatorSet& getCreatorSet() const;
            void addCreator( const CreatorPtr& value );
            void removeCreator( const CreatorSetIterConst& value );
            void clearCreatorSet();
            CreatorPtr getCreator( const CreatorSetIterConst& setIterator ) const;

            /* _________ Rights minOccurs = 0, maxOccurs = unbounded _________ */
            const RightsSet& getRightsSet() const;
            void addRights( const RightsPtr& value );
            void removeRights( const RightsSetIterConst& value );
            void clearRightsSet();
            RightsPtr getRights( const RightsSetIterConst& setIterator ) const;

            /* _________ Encoding minOccurs = 0, maxOccurs = 1 _________ */
            EncodingPtr getEncoding() const;
            void setEncoding( const EncodingPtr& value );
            bool getHasEncoding() const;
            void setHasEncoding( const bool value );

            /* _________ Source minOccurs = 0, maxOccurs = 1 _________ */
            SourcePtr getSource() const;
            void setSource( const SourcePtr& value );
            bool getHasSource() const;
            void setHasSource( const bool value );

            /* _________ Relation minOccurs = 0, maxOccurs = unbounded _________ */
            const RelationSet& getRelationSet() const;
            void addRelation( const RelationPtr& value );
            void removeRelation( const RelationSetIterConst& value );
            void clearRelationSet();
            RelationPtr getRelation( const RelationSetIterConst& setIterator ) const;

            /* _________ Miscellaneous minOccurs = 0, maxOccurs = 1 _________ */
            MiscellaneousPtr getMiscellaneous() const;
            void setMiscellaneous( const MiscellaneousPtr& value );
            bool getHasMiscellaneous() const;
            void setHasMiscellaneous( const bool value );

        private:
            CreatorSet myCreatorSet;
            RightsSet myRightsSet;
            EncodingPtr myEncoding;
            bool myHasEncoding;
            SourcePtr mySource;
            bool myHasSource;
            RelationSet myRelationSet;
            MiscellaneousPtr myMiscellaneous;
            bool myHasMiscellaneous;
        };
        
        struct PartAbbreviationDisplayAttributes;
        using PartAbbreviationDisplayAttributesPtr = std::shared_ptr<PartAbbreviationDisplayAttributes>;
        
        struct PartAbbreviationDisplayAttributes : public AttributesInterface
        {
        public:
            PartAbbreviationDisplayAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo printObject;
            bool hasPrintObject;
        };
        
        class PartAbbreviationDisplay;
        using PartAbbreviationDisplayPtr = std::shared_ptr<PartAbbreviationDisplay>;
        using PartAbbreviationDisplayUPtr = std::unique_ptr<PartAbbreviationDisplay>;
        using PartAbbreviationDisplaySet = std::vector<PartAbbreviationDisplayPtr>;
        using PartAbbreviationDisplaySetIter = PartAbbreviationDisplaySet::iterator;
        using PartAbbreviationDisplaySetIterConst = PartAbbreviationDisplaySet::const_iterator;
        inline PartAbbreviationDisplayPtr makePartAbbreviationDisplay() { return std::make_shared<PartAbbreviationDisplay>(); }

        class PartAbbreviationDisplay : public ElementInterface
        {
        public:
            enum class Choice
            {
                displayText = 1,
                accidentalText = 2
            };
            PartAbbreviationDisplay();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PartAbbreviationDisplayAttributesPtr getAttributes() const;
            void setAttributes( const PartAbbreviationDisplayAttributesPtr& value );

            /* _________ Choice _________ */
            PartAbbreviationDisplay::Choice getChoice() const;
            void setChoice( const PartAbbreviationDisplay::Choice value );

            /* _________ DisplayText minOccurs = 1, maxOccurs = 1 _________ */
            DisplayTextPtr getDisplayText() const;
            void setDisplayText( const DisplayTextPtr& value );

            /* _________ AccidentalText minOccurs = 1, maxOccurs = 1 _________ */
            AccidentalTextPtr getAccidentalText() const;
            void setAccidentalText( const AccidentalTextPtr& value );

        private:
            PartAbbreviationDisplayAttributesPtr myAttributes;
            Choice myChoice;
            DisplayTextPtr myDisplayText;
            AccidentalTextPtr myAccidentalText;
        };
        
        
        class SoloOrEnsembleChoice;
        using SoloOrEnsembleChoicePtr = std::shared_ptr<SoloOrEnsembleChoice>;
        using SoloOrEnsembleChoiceUPtr = std::unique_ptr<SoloOrEnsembleChoice>;
        using SoloOrEnsembleChoiceSet = std::vector<SoloOrEnsembleChoicePtr>;
        using SoloOrEnsembleChoiceSetIter = SoloOrEnsembleChoiceSet::iterator;
        using SoloOrEnsembleChoiceSetIterConst = SoloOrEnsembleChoiceSet::const_iterator;
        inline SoloOrEnsembleChoicePtr makeSoloOrEnsembleChoice() { return std::make_shared<SoloOrEnsembleChoice>(); }

        class SoloOrEnsembleChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                solo = 1,
                ensemble = 2
            };
            SoloOrEnsembleChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice  _________ */
            SoloOrEnsembleChoice::Choice getChoice() const;
            void setChoice( const SoloOrEnsembleChoice::Choice value );

            /* _________ Solo minOccurs = 1, maxOccurs = 1 _________ */
            SoloPtr getSolo() const;
            void setSolo( const SoloPtr& value );

            /* _________ Ensemble minOccurs = 1, maxOccurs = 1 _________ */
            EnsemblePtr getEnsemble() const;
            void setEnsemble( const EnsemblePtr& value );

        private:
            Choice myChoice;
            SoloPtr mySolo;
            EnsemblePtr myEnsemble;
        };
        
        struct ScoreInstrumentAttributes;
        using ScoreInstrumentAttributesPtr = std::shared_ptr<ScoreInstrumentAttributes>;
        
        struct ScoreInstrumentAttributes : public AttributesInterface
        {
        public:
            ScoreInstrumentAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsID id;
            const 	bool hasId;
        };
        
        class ScoreInstrument;
        using ScoreInstrumentPtr = std::shared_ptr<ScoreInstrument>;
        using ScoreInstrumentUPtr = std::unique_ptr<ScoreInstrument>;
        using ScoreInstrumentSet = std::vector<ScoreInstrumentPtr>;
        using ScoreInstrumentSetIter = ScoreInstrumentSet::iterator;
        using ScoreInstrumentSetIterConst = ScoreInstrumentSet::const_iterator;
        inline ScoreInstrumentPtr makeScoreInstrument() { return std::make_shared<ScoreInstrument>(); }

        class ScoreInstrument : public ElementInterface
        {
        public:
            ScoreInstrument();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ScoreInstrumentAttributesPtr getAttributes() const;
            void setAttributes( const ScoreInstrumentAttributesPtr& value );

            /* _________ InstrumentName minOccurs = 1, maxOccurs = 1 _________ */
            InstrumentNamePtr getInstrumentName() const;
            void setInstrumentName( const InstrumentNamePtr& value );

            /* _________ InstrumentAbbreviation minOccurs = 0, maxOccurs = 1 _________ */
            InstrumentAbbreviationPtr getInstrumentAbbreviation() const;
            void setInstrumentAbbreviation( const InstrumentAbbreviationPtr& value );
            bool getHasInstrumentAbbreviation() const;
            void setHasInstrumentAbbreviation( const bool value );

            /* _________ SoloOrEnsembleChoice minOccurs = 0, maxOccurs = 1 _________ */
            SoloOrEnsembleChoicePtr getSoloOrEnsembleChoice() const;
            void setSoloOrEnsembleChoice( const SoloOrEnsembleChoicePtr& value );
            bool getHasSoloOrEnsembleChoice() const;
            void setHasSoloOrEnsembleChoice( const bool value );

            /* _________ InstrumentSound minOccurs = 0, maxOccurs = 1 _________ */
            InstrumentSoundPtr getInstrumentSound() const;
            void setInstrumentSound( const InstrumentSoundPtr& value );
            bool getHasInstrumentSound() const;
            void setHasInstrumentSound( const bool value );

            /* _________ VirtualInstrument minOccurs = 0, maxOccurs = 1 _________ */
            VirtualInstrumentPtr getVirtualInstrument() const;
            void setVirtualInstrument( const VirtualInstrumentPtr& value );
            bool getHasVirtualInstrument() const;
            void setHasVirtualInstrument( const bool value );

        private:
            ScoreInstrumentAttributesPtr myAttributes;
            InstrumentNamePtr myInstrumentName;
            InstrumentAbbreviationPtr myInstrumentAbbreviation;
            bool myHasInstrumentAbbreviation;
            InstrumentSoundPtr myInstrumentSound;
            bool myHasInstrumentSound;
            SoloOrEnsembleChoicePtr mySoloOrEnsembleChoice;
            bool myHasSoloOrEnsembleChoice;
            VirtualInstrumentPtr myVirtualInstrument;
            bool myHasVirtualInstrument;
        };
        
        
        class HarmonyChordGroup;
        using HarmonyChordGroupPtr = std::shared_ptr<HarmonyChordGroup>;
        using HarmonyChordGroupUPtr = std::unique_ptr<HarmonyChordGroup>;
        using HarmonyChordGroupSet = std::vector<HarmonyChordGroupPtr>;
        using HarmonyChordGroupSetIter = HarmonyChordGroupSet::iterator;
        using HarmonyChordGroupSetIterConst = HarmonyChordGroupSet::const_iterator;
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
        
        struct HarmonyAttributes;
        using HarmonyAttributesPtr = std::shared_ptr<HarmonyAttributes>;
        
        struct HarmonyAttributes : public AttributesInterface
        {
        public:
            HarmonyAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            HarmonyType type;
            YesNo printObject;
            YesNo printFrame;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            AboveBelow placement;
            bool hasType;
            bool hasPrintObject;
            bool hasPrintFrame;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasPlacement;
        };
        
        class Harmony;
        using HarmonyPtr = std::shared_ptr<Harmony>;
        using HarmonyUPtr = std::unique_ptr<Harmony>;
        using HarmonySet = std::vector<HarmonyPtr>;
        using HarmonySetIter = HarmonySet::iterator;
        using HarmonySetIterConst = HarmonySet::const_iterator;
        inline HarmonyPtr makeHarmony() { return std::make_shared<Harmony>(); }

        class Harmony : public ElementInterface
        {
        public:
            Harmony();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HarmonyAttributesPtr getAttributes() const;
            void setAttributes( const HarmonyAttributesPtr& value );

            /* _________ HarmonyChordGroup minOccurs = 1, maxOccurs = unbounded _________ */
            const HarmonyChordGroupSet& getHarmonyChordGroupSet() const;
            void addHarmonyChordGroup( const HarmonyChordGroupPtr& value );
            void removeHarmonyChordGroup( const HarmonyChordGroupSetIterConst& value );
            void clearHarmonyChordGroupSet();
            HarmonyChordGroupPtr getHarmonyChordGroup( const HarmonyChordGroupSetIterConst& setIterator ) const;

            /* _________ Frame minOccurs = 0, maxOccurs = 1 _________ */
            FramePtr getFrame() const;
            void setFrame( const FramePtr& value );
            bool getHasFrame() const;
            void setHasFrame( const bool value );

            /* _________ Offset minOccurs = 0, maxOccurs = 1 _________ */
            OffsetPtr getOffset() const;
            void setOffset( const OffsetPtr& value );
            bool getHasOffset() const;
            void setHasOffset( const bool value );

            /* _________ EditorialGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialGroupPtr getEditorialGroup() const;
            void setEditorialGroup( const EditorialGroupPtr& value );

            /* _________ Staff minOccurs = 0, maxOccurs = 1 _________ */
            StaffPtr getStaff() const;
            void setStaff( const StaffPtr& value );
            bool getHasStaff() const;
            void setHasStaff( const bool value );

        private:
            HarmonyAttributesPtr myAttributes;
            HarmonyChordGroupSet myHarmonyChordGroupSet;
            FramePtr myFrame;
            bool myHasFrame;
            OffsetPtr myOffset;
            bool myHasOffset;
            EditorialGroupPtr myEditorialGroup;
            StaffPtr myStaff;
            bool myHasStaff;
        };
        
        class CreditWordsGroup;
        using CreditWordsGroupPtr = std::shared_ptr<CreditWordsGroup>;
        using CreditWordsGroupUPtr = std::unique_ptr<CreditWordsGroup>;
        using CreditWordsGroupSet = std::vector<CreditWordsGroupPtr>;
        using CreditWordsGroupSetIter = CreditWordsGroupSet::iterator;
        using CreditWordsGroupSetIterConst = CreditWordsGroupSet::const_iterator;
        inline CreditWordsGroupPtr makeCreditWordsGroup() { return std::make_shared<CreditWordsGroup>(); }

        class CreditWordsGroup : public ElementInterface
        {
        public:
            CreditWordsGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ LinkSet minOccurs = 0, maxOccurs = unbounded _________ */
            const LinkSet& getLinkSet() const;
            void addLink( const LinkPtr& value );
            void removeLink( const LinkSetIterConst& value );
            void clearLinkSet();
            LinkPtr getLink( const LinkSetIterConst& setIterator ) const;

            /* _________ Bookmark minOccurs = 0, maxOccurs = unbounded _________ */
            const BookmarkSet& getBookmarkSet() const;
            void addBookmark( const BookmarkPtr& value );
            void removeBookmark( const BookmarkSetIterConst& value );
            void clearBookmarkSet();
            BookmarkPtr getBookmark( const BookmarkSetIterConst& setIterator ) const;

            /* _________ CreditWords minOccurs = 1, maxOccurs = 1 _________ */
            CreditWordsPtr getCreditWords() const;
            void setCreditWords( const CreditWordsPtr& value );

        private:
            LinkSet myLinkSet;
            BookmarkSet myBookmarkSet;
            CreditWordsPtr myCreditWords;
        };
        
        class CreditChoice;
        using CreditChoicePtr = std::shared_ptr<CreditChoice>;
        using CreditChoiceUPtr = std::unique_ptr<CreditChoice>;
        using CreditChoiceSet = std::vector<CreditChoicePtr>;
        using CreditChoiceSetIter = CreditChoiceSet::iterator;
        using CreditChoiceSetIterConst = CreditChoiceSet::const_iterator;
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

            /* _________ CreditWordsGroup minOccurs = 1, maxOccurs = 1 _________ */
            CreditWordsGroupPtr getCreditWordsGroup() const;
            void setCreditWordsGroup( const CreditWordsGroupPtr& value );

        private:
            Choice myChoice;
            CreditImagePtr myCreditImage;
            CreditWordsGroupPtr myCreditWordsGroup;
        };
        
        
        struct CreditAttributes;
        using CreditAttributesPtr = std::shared_ptr<CreditAttributes>;
        
        struct CreditAttributes : public AttributesInterface
        {
        public:
            CreditAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            PositiveInteger page;
            bool hasPage;
        };
        
        class Credit;
        using CreditPtr = std::shared_ptr<Credit>;
        using CreditUPtr = std::unique_ptr<Credit>;
        using CreditSet = std::vector<CreditPtr>;
        using CreditSetIter = CreditSet::iterator;
        using CreditSetIterConst = CreditSet::const_iterator;
        inline CreditPtr makeCredit() { return std::make_shared<Credit>(); }

        class Credit : public ElementInterface
        {
        public:
            Credit();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            CreditAttributesPtr getAttributes() const;
            void setAttributes( const CreditAttributesPtr& value );

            /* _________ CreditType minOccurs = 0, maxOccurs = unbounded _________ */
            const CreditTypeSet& getCreditTypeSet() const;
            void addCreditType( const CreditTypePtr& value );
            void removeCreditType( const CreditTypeSetIterConst& value );
            void clearCreditTypeSet();
            CreditTypePtr getCreditType( const CreditTypeSetIterConst& setIterator ) const;

            /* _________ Link minOccurs = 0, maxOccurs = unbounded _________ */
            const LinkSet& getLinkSet() const;
            void addLink( const LinkPtr& value );
            void removeLink( const LinkSetIterConst& value );
            void clearLinkSet();
            LinkPtr getLink( const LinkSetIterConst& setIterator ) const;

            /* _________ Bookmark minOccurs = 0, maxOccurs = unbounded _________ */
            const BookmarkSet& getBookmarkSet() const;
            void addBookmark( const BookmarkPtr& value );
            void removeBookmark( const BookmarkSetIterConst& value );
            void clearBookmarkSet();
            BookmarkPtr getBookmark( const BookmarkSetIterConst& setIterator ) const;

            /* _________ CreditChoice minOccurs = 1, maxOccurs = 1 _________ */
            CreditChoicePtr getCreditChoice() const;
            void setCreditChoice( const CreditChoicePtr& value );

        private:
            CreditAttributesPtr myAttributes;
            CreditTypeSet myCreditTypeSet;
            LinkSet myLinkSet;
            BookmarkSet myBookmarkSet;
            CreditChoicePtr myCreditChoice;
        };
        
        class HarmonicTypeChoice;
        using HarmonicTypeChoicePtr = std::shared_ptr<HarmonicTypeChoice>;
        using HarmonicTypeChoiceUPtr = std::unique_ptr<HarmonicTypeChoice>;
        using HarmonicTypeChoiceSet = std::vector<HarmonicTypeChoicePtr>;
        using HarmonicTypeChoiceSetIter = HarmonicTypeChoiceSet::iterator;
        using HarmonicTypeChoiceSetIterConst = HarmonicTypeChoiceSet::const_iterator;
        inline HarmonicTypeChoicePtr makeHarmonicTypeChoice() { return std::make_shared<HarmonicTypeChoice>(); }

        class HarmonicTypeChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                natural = 1,
                artificial = 2
            };
            HarmonicTypeChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            HarmonicTypeChoice::Choice getChoice() const;
            void setChoice( const HarmonicTypeChoice::Choice value );

            /* _________ Natural minOccurs = 1, maxOccurs = 1 _________ */
            NaturalPtr getNatural() const;
            void setNatural( const NaturalPtr& value );

            /* _________ Artificial minOccurs = 1, maxOccurs = 1 _________ */
            ArtificialPtr getArtificial() const;
            void setArtificial( const ArtificialPtr& value );

        private:
            Choice myChoice;
            NaturalPtr myNatural;
            ArtificialPtr myArtificial;
        };
        
        class HarmonicInfoChoice;
        using HarmonicInfoChoicePtr = std::shared_ptr<HarmonicInfoChoice>;
        using HarmonicInfoChoiceUPtr = std::unique_ptr<HarmonicInfoChoice>;
        using HarmonicInfoChoiceSet = std::vector<HarmonicInfoChoicePtr>;
        using HarmonicInfoChoiceSetIter = HarmonicInfoChoiceSet::iterator;
        using HarmonicInfoChoiceSetIterConst = HarmonicInfoChoiceSet::const_iterator;
        inline HarmonicInfoChoicePtr makeHarmonicInfoChoice() { return std::make_shared<HarmonicInfoChoice>(); }

        class HarmonicInfoChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                basePitch = 1,
                touchingPitch = 2,
                soundingPitch = 3
            };
            HarmonicInfoChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            HarmonicInfoChoice::Choice getChoice() const;
            void setChoice( const HarmonicInfoChoice::Choice value );

            /* _________ BasePitch minOccurs = 1, maxOccurs = 1 _________ */
            BasePitchPtr getBasePitch() const;
            void setBasePitch( const BasePitchPtr& value );

            /* _________ TouchingPitch minOccurs = 1, maxOccurs = 1 _________ */
            TouchingPitchPtr getTouchingPitch() const;
            void setTouchingPitch( const TouchingPitchPtr& value );

            /* _________ SoundingPitch minOccurs = 1, maxOccurs = 1 _________ */
            SoundingPitchPtr getSoundingPitch() const;
            void setSoundingPitch( const SoundingPitchPtr& value );

        private:
            Choice myChoice;
            BasePitchPtr myBasePitch;
            TouchingPitchPtr myTouchingPitch;
            SoundingPitchPtr mySoundingPitch;
        };
        
        
        struct HarmonicAttributes;
        using HarmonicAttributesPtr = std::shared_ptr<HarmonicAttributes>;
        
        struct HarmonicAttributes : public AttributesInterface
        {
        public:
            HarmonicAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo printObject;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            AboveBelow placement;
            bool hasPrintObject;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasPlacement;
        };
        
        class Harmonic;
        using HarmonicPtr = std::shared_ptr<Harmonic>;
        using HarmonicUPtr = std::unique_ptr<Harmonic>;
        using HarmonicSet = std::vector<HarmonicPtr>;
        using HarmonicSetIter = HarmonicSet::iterator;
        using HarmonicSetIterConst = HarmonicSet::const_iterator;
        inline HarmonicPtr makeHarmonic() { return std::make_shared<Harmonic>(); }

        class Harmonic : public ElementInterface
        {
        public:
            Harmonic();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HarmonicAttributesPtr getAttributes() const;
            void setAttributes( const HarmonicAttributesPtr& value );

            /* _________ HarmonicTypeChoice minOccurs = 0, maxOccurs = 1 _________ */
            HarmonicTypeChoicePtr getHarmonicTypeChoice() const;
            void setHarmonicTypeChoice( const HarmonicTypeChoicePtr& value );
            bool getHasHarmonicTypeChoice() const;
            void setHasHarmonicTypeChoice( const bool value );

            /* _________ HarmonicInfoChoice minOccurs = 0, maxOccurs = 1 _________ */
            HarmonicInfoChoicePtr getHarmonicInfoChoice() const;
            void setHarmonicInfoChoice( const HarmonicInfoChoicePtr& value );
            bool getHasHarmonicInfoChoice() const;
            void setHasHarmonicInfoChoice( const bool value );

        private:
            HarmonicAttributesPtr myAttributes;
            HarmonicTypeChoicePtr myHarmonicTypeChoice;
            bool myHasHarmonicTypeChoice;
            HarmonicInfoChoicePtr myHarmonicInfoChoice;
            bool myHasHarmonicInfoChoice;
        };
        
        
        struct PrintAttributes;
        using PrintAttributesPtr = std::shared_ptr<PrintAttributes>;
        
        struct PrintAttributes : public AttributesInterface
        {
        public:
            PrintAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue staffSpacing;
            YesNo newSystem;
            YesNo newPage;
            PositiveInteger blankPage;
            XsToken pageNumber;
            bool hasStaffSpacing;
            bool hasNewSystem;
            bool hasNewPage;
            bool hasBlankPage;
            bool hasPageNumber;
        };
        
        class Print;
        using PrintPtr = std::shared_ptr<Print>;
        using PrintUPtr = std::unique_ptr<Print>;
        using PrintSet = std::vector<PrintPtr>;
        using PrintSetIter = PrintSet::iterator;
        using PrintSetIterConst = PrintSet::const_iterator;
        inline PrintPtr makePrint() { return std::make_shared<Print>(); }

        class Print : public ElementInterface
        {
        public:
            Print();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PrintAttributesPtr getAttributes() const;
            void setAttributes( const PrintAttributesPtr& value );

            /* _________ LayoutGroup minOccurs = 1, maxOccurs = 1 _________ */
            LayoutGroupPtr getLayoutGroup() const;
            void setLayoutGroup( const LayoutGroupPtr& value );

            /* _________ MeasureLayout minOccurs = 0, maxOccurs = 1 _________ */
            MeasureLayoutPtr getMeasureLayout() const;
            void setMeasureLayout( const MeasureLayoutPtr& value );
            bool getHasMeasureLayout() const;
            void setHasMeasureLayout( const bool value );

            /* _________ MeasureNumbering minOccurs = 0, maxOccurs = 1 _________ */
            MeasureNumberingPtr getMeasureNumbering() const;
            void setMeasureNumbering( const MeasureNumberingPtr& value );
            bool getHasMeasureNumbering() const;
            void setHasMeasureNumbering( const bool value );

            /* _________ PartNameDisplay minOccurs = 0, maxOccurs = 1 _________ */
            PartNameDisplayPtr getPartNameDisplay() const;
            void setPartNameDisplay( const PartNameDisplayPtr& value );
            bool getHasPartNameDisplay() const;
            void setHasPartNameDisplay( const bool value );

            /* _________ PartAbbreviationDisplay minOccurs = 0, maxOccurs = 1 _________ */
            PartAbbreviationDisplayPtr getPartAbbreviationDisplay() const;
            void setPartAbbreviationDisplay( const PartAbbreviationDisplayPtr& value );
            bool getHasPartAbbreviationDisplay() const;
            void setHasPartAbbreviationDisplay( const bool value );

        private:
            PrintAttributesPtr myAttributes;
            LayoutGroupPtr myLayoutGroup;
            MeasureLayoutPtr myMeasureLayout;
            bool myHasMeasureLayout;
            MeasureNumberingPtr myMeasureNumbering;
            bool myHasMeasureNumbering;
            PartNameDisplayPtr myPartNameDisplay;
            bool myHasPartNameDisplay;
            PartAbbreviationDisplayPtr myPartAbbreviationDisplay;
            bool myHasPartAbbreviationDisplay;
        };
        
        struct PartGroupAttributes;
        using PartGroupAttributesPtr = std::shared_ptr<PartGroupAttributes>;
        
        struct PartGroupAttributes : public AttributesInterface
        {
        public:
            PartGroupAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            XsToken number;
            const 	bool hasType;
            bool hasNumber;
        };
        
        class PartGroup;
        using PartGroupPtr = std::shared_ptr<PartGroup>;
        using PartGroupUPtr = std::unique_ptr<PartGroup>;
        using PartGroupSet = std::vector<PartGroupPtr>;
        using PartGroupSetIter = PartGroupSet::iterator;
        using PartGroupSetIterConst = PartGroupSet::const_iterator;
        inline PartGroupPtr makePartGroup() { return std::make_shared<PartGroup>(); }

        class PartGroup : public ElementInterface
        {
        public:
            PartGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PartGroupAttributesPtr getAttributes() const;
            void setAttributes( const PartGroupAttributesPtr& value );

            /* _________ GroupName minOccurs = 0, maxOccurs = 1 _________ */
            GroupNamePtr getGroupName() const;
            void setGroupName( const GroupNamePtr& value );
            bool getHasGroupName() const;
            void setHasGroupName( const bool value );

            /* _________ GroupNameDisplay minOccurs = 0, maxOccurs = 1 _________ */
            GroupNameDisplayPtr getGroupNameDisplay() const;
            void setGroupNameDisplay( const GroupNameDisplayPtr& value );
            bool getHasGroupNameDisplay() const;
            void setHasGroupNameDisplay( const bool value );

            /* _________ GroupAbbreviation minOccurs = 0, maxOccurs = 1 _________ */
            GroupAbbreviationPtr getGroupAbbreviation() const;
            void setGroupAbbreviation( const GroupAbbreviationPtr& value );
            bool getHasGroupAbbreviation() const;
            void setHasGroupAbbreviation( const bool value );

            /* _________ GroupAbbreviationDisplay minOccurs = 0, maxOccurs = 1 _________ */
            GroupAbbreviationDisplayPtr getGroupAbbreviationDisplay() const;
            void setGroupAbbreviationDisplay( const GroupAbbreviationDisplayPtr& value );
            bool getHasGroupAbbreviationDisplay() const;
            void setHasGroupAbbreviationDisplay( const bool value );

            /* _________ GroupSymbol minOccurs = 0, maxOccurs = 1 _________ */
            GroupSymbolPtr getGroupSymbol() const;
            void setGroupSymbol( const GroupSymbolPtr& value );
            bool getHasGroupSymbol() const;
            void setHasGroupSymbol( const bool value );

            /* _________ GroupBarline minOccurs = 0, maxOccurs = 1 _________ */
            GroupBarlinePtr getGroupBarline() const;
            void setGroupBarline( const GroupBarlinePtr& value );
            bool getHasGroupBarline() const;
            void setHasGroupBarline( const bool value );

            /* _________ GroupTime minOccurs = 0, maxOccurs = 1 _________ */
            GroupTimePtr getGroupTime() const;
            void setGroupTime( const GroupTimePtr& value );
            bool getHasGroupTime() const;
            void setHasGroupTime( const bool value );

            /* _________ EditorialGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialGroupPtr getEditorialGroup() const;
            void setEditorialGroup( const EditorialGroupPtr& value );

        private:
            PartGroupAttributesPtr myAttributes;
            GroupNamePtr myGroupName;
            bool myHasGroupName;
            GroupNameDisplayPtr myGroupNameDisplay;
            bool myHasGroupNameDisplay;
            GroupAbbreviationPtr myGroupAbbreviation;
            bool myHasGroupAbbreviation;
            GroupAbbreviationDisplayPtr myGroupAbbreviationDisplay;
            bool myHasGroupAbbreviationDisplay;
            GroupSymbolPtr myGroupSymbol;
            bool myHasGroupSymbol;
            GroupBarlinePtr myGroupBarline;
            bool myHasGroupBarline;
            GroupTimePtr myGroupTime;
            bool myHasGroupTime;
            EditorialGroupPtr myEditorialGroup;
        };
        
        class TechnicalChoice;
        using TechnicalChoicePtr = std::shared_ptr<TechnicalChoice>;
        using TechnicalChoiceUPtr = std::unique_ptr<TechnicalChoice>;
        using TechnicalChoiceSet = std::vector<TechnicalChoicePtr>;
        using TechnicalChoiceSetIter = TechnicalChoiceSet::iterator;
        using TechnicalChoiceSetIterConst = TechnicalChoiceSet::const_iterator;
        inline TechnicalChoicePtr makeTechnicalChoice() { return std::make_shared<TechnicalChoice>(); }

        class TechnicalChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                upBow = 1,
                downBow = 2,
                harmonic = 3,
                openString = 4,
                thumbPosition = 5,
                fingering = 6,
                pluck = 7,
                doubleTongue = 8,
                tripleTongue = 9,
                stopped = 10,
                snapPizzicato = 11,
                fret = 12,
                string_ = 13,
                hammerOn = 14,
                pullOff = 15,
                bend = 16,
                tap = 17,
                heel = 18,
                toe = 19,
                fingernails = 20,
                hole = 21,
                arrow = 22,
                handbell = 23,
                otherTechnical = 24
            };
            TechnicalChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            TechnicalChoice::Choice getChoice() const;
            void setChoice( const TechnicalChoice::Choice value );

            /* _________ UpBow minOccurs = 1, maxOccurs = 1 _________ */
            UpBowPtr getUpBow() const;
            void setUpBow( const UpBowPtr& value );

            /* _________ DownBow minOccurs = 1, maxOccurs = 1 _________ */
            DownBowPtr getDownBow() const;
            void setDownBow( const DownBowPtr& value );

            /* _________ Harmonic minOccurs = 1, maxOccurs = 1 _________ */
            HarmonicPtr getHarmonic() const;
            void setHarmonic( const HarmonicPtr& value );

            /* _________ OpenString minOccurs = 1, maxOccurs = 1 _________ */
            OpenStringPtr getOpenString() const;
            void setOpenString( const OpenStringPtr& value );

            /* _________ ThumbPosition minOccurs = 1, maxOccurs = 1 _________ */
            ThumbPositionPtr getThumbPosition() const;
            void setThumbPosition( const ThumbPositionPtr& value );

            /* _________ Fingering minOccurs = 1, maxOccurs = 1 _________ */
            FingeringPtr getFingering() const;
            void setFingering( const FingeringPtr& value );

            /* _________ Pluck minOccurs = 1, maxOccurs = 1 _________ */
            PluckPtr getPluck() const;
            void setPluck( const PluckPtr& value );

            /* _________ DoubleTongue minOccurs = 1, maxOccurs = 1 _________ */
            DoubleTonguePtr getDoubleTongue() const;
            void setDoubleTongue( const DoubleTonguePtr& value );

            /* _________ TripleTongue minOccurs = 1, maxOccurs = 1 _________ */
            TripleTonguePtr getTripleTongue() const;
            void setTripleTongue( const TripleTonguePtr& value );

            /* _________ Stopped minOccurs = 1, maxOccurs = 1 _________ */
            StoppedPtr getStopped() const;
            void setStopped( const StoppedPtr& value );

            /* _________ SnapPizzicato minOccurs = 1, maxOccurs = 1 _________ */
            SnapPizzicatoPtr getSnapPizzicato() const;
            void setSnapPizzicato( const SnapPizzicatoPtr& value );

            /* _________ Fret minOccurs = 1, maxOccurs = 1 _________ */
            FretPtr getFret() const;
            void setFret( const FretPtr& value );

            /* _________ String minOccurs = 1, maxOccurs = 1 _________ */
            StringPtr getString() const;
            void setString( const StringPtr& value );

            /* _________ HammerOn minOccurs = 1, maxOccurs = 1 _________ */
            HammerOnPtr getHammerOn() const;
            void setHammerOn( const HammerOnPtr& value );

            /* _________ PullOff minOccurs = 1, maxOccurs = 1 _________ */
            PullOffPtr getPullOff() const;
            void setPullOff( const PullOffPtr& value );

            /* _________ Bend minOccurs = 1, maxOccurs = 1 _________ */
            BendPtr getBend() const;
            void setBend( const BendPtr& value );

            /* _________ Tap minOccurs = 1, maxOccurs = 1 _________ */
            TapPtr getTap() const;
            void setTap( const TapPtr& value );

            /* _________ Heel minOccurs = 1, maxOccurs = 1 _________ */
            HeelPtr getHeel() const;
            void setHeel( const HeelPtr& value );

            /* _________ Toe minOccurs = 1, maxOccurs = 1 _________ */
            ToePtr getToe() const;
            void setToe( const ToePtr& value );

            /* _________ Fingernails minOccurs = 1, maxOccurs = 1 _________ */
            FingernailsPtr getFingernails() const;
            void setFingernails( const FingernailsPtr& value );

            /* _________ Hole minOccurs = 1, maxOccurs = 1 _________ */
            HolePtr getHole() const;
            void setHole( const HolePtr& value );

            /* _________ Arrow minOccurs = 1, maxOccurs = 1 _________ */
            ArrowPtr getArrow() const;
            void setArrow( const ArrowPtr& value );

            /* _________ Handbell minOccurs = 1, maxOccurs = 1 _________ */
            HandbellPtr getHandbell() const;
            void setHandbell( const HandbellPtr& value );

            /* _________ OtherTechnical minOccurs = 1, maxOccurs = 1 _________ */
            OtherTechnicalPtr getOtherTechnical() const;
            void setOtherTechnical( const OtherTechnicalPtr& value );

        private:
            Choice myChoice;
            UpBowPtr myUpBow;
            DownBowPtr myDownBow;
            HarmonicPtr myHarmonic;
            OpenStringPtr myOpenString;
            ThumbPositionPtr myThumbPosition;
            FingeringPtr myFingering;
            PluckPtr myPluck;
            DoubleTonguePtr myDoubleTongue;
            TripleTonguePtr myTripleTongue;
            StoppedPtr myStopped;
            SnapPizzicatoPtr mySnapPizzicato;
            FretPtr myFret;
            StringPtr myString;
            HammerOnPtr myHammerOn;
            PullOffPtr myPullOff;
            BendPtr myBend;
            TapPtr myTap;
            HeelPtr myHeel;
            ToePtr myToe;
            FingernailsPtr myFingernails;
            HolePtr myHole;
            ArrowPtr myArrow;
            HandbellPtr myHandbell;
            OtherTechnicalPtr myOtherTechnical;
        };
        
        class Technical;
        using TechnicalPtr = std::shared_ptr<Technical>;
        using TechnicalUPtr = std::unique_ptr<Technical>;
        using TechnicalSet = std::vector<TechnicalPtr>;
        using TechnicalSetIter = TechnicalSet::iterator;
        using TechnicalSetIterConst = TechnicalSet::const_iterator;
        inline TechnicalPtr makeTechnical() { return std::make_shared<Technical>(); }

        class Technical : public ElementInterface
        {
        public:
            Technical();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ TechnicalChoice minOccurs = 0, maxOccurs = unbounded _________ */
            const TechnicalChoiceSet& getTechnicalChoiceSet() const;
            void addTechnicalChoice( const TechnicalChoicePtr& value );
            void removeTechnicalChoice( const TechnicalChoiceSetIterConst& value );
            void clearTechnicalChoiceSet();
            TechnicalChoicePtr getTechnicalChoice( const TechnicalChoiceSetIterConst& setIterator ) const;

        private:
            TechnicalChoiceSet myTechnicalChoiceSet;
        };
        
        
        
        
        class EditorialVoiceDirectionGroup;
        using EditorialVoiceDirectionGroupPtr = std::shared_ptr<EditorialVoiceDirectionGroup>;
        using EditorialVoiceDirectionGroupUPtr = std::unique_ptr<EditorialVoiceDirectionGroup>;
        using EditorialVoiceDirectionGroupSet = std::vector<EditorialVoiceDirectionGroupPtr>;
        using EditorialVoiceDirectionGroupSetIter = EditorialVoiceDirectionGroupSet::iterator;
        using EditorialVoiceDirectionGroupSetIterConst = EditorialVoiceDirectionGroupSet::const_iterator;
        inline EditorialVoiceDirectionGroupPtr makeEditorialVoiceDirectionGroup() { return std::make_shared<EditorialVoiceDirectionGroup>(); }
        

        class EditorialVoiceDirectionGroup : public ElementInterface
        {
        public:
            EditorialVoiceDirectionGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Footnote minOccurs = 0, maxOccurs = 1 _________ */
            FootnotePtr getFootnote() const;
            void setFootnote( const FootnotePtr& value );
            bool getHasFootnote() const;
            void setHasFootnote( const bool value );

            /* _________ Level minOccurs = 0, maxOccurs = 1 _________ */
            LevelPtr getLevel() const;
            void setLevel( const LevelPtr& value );
            bool getHasLevel() const;
            void setHasLevel( const bool value );

            /* _________ Voice minOccurs = 0, maxOccurs = 1 _________ */
            VoicePtr getVoice() const;
            void setVoice( const VoicePtr& value );
            bool getHasVoice() const;
            void setHasVoice( const bool value );

        private:
            FootnotePtr myFootnote;
            bool myHasFootnote;
            LevelPtr myLevel;
            bool myHasLevel;
            VoicePtr myVoice;
            bool myHasVoice;
        };
        
        
        
        struct DirectionAttributes;
        using DirectionAttributesPtr = std::shared_ptr<DirectionAttributes>;
        
        struct DirectionAttributes : public AttributesInterface
        {
        public:
            DirectionAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            AboveBelow placement;
            YesNo directive;
            bool hasPlacement;
            bool hasDirective;
        };
        
        class Direction;
        using DirectionPtr = std::shared_ptr<Direction>;
        using DirectionUPtr = std::unique_ptr<Direction>;
        using DirectionSet = std::vector<DirectionPtr>;
        using DirectionSetIter = DirectionSet::iterator;
        using DirectionSetIterConst = DirectionSet::const_iterator;
        inline DirectionPtr makeDirection() { return std::make_shared<Direction>(); }

        class Direction : public ElementInterface
        {
        public:
            Direction();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DirectionAttributesPtr getAttributes() const;
            void setAttributes( const DirectionAttributesPtr& value );

            /* _________ DirectionType minOccurs = 1, maxOccurs = unbounded _________ */
            const DirectionTypeSet& getDirectionTypeSet() const;
            void addDirectionType( const DirectionTypePtr& value );
            void removeDirectionType( const DirectionTypeSetIterConst& value );
            void clearDirectionTypeSet();
            DirectionTypePtr getDirectionType( const DirectionTypeSetIterConst& setIterator ) const;

            /* _________ Offset minOccurs = 0, maxOccurs = 1 _________ */
            OffsetPtr getOffset() const;
            void setOffset( const OffsetPtr& value );
            bool getHasOffset() const;
            void setHasOffset( const bool value );

            /* _________ EditorialVoiceDirectionGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialVoiceDirectionGroupPtr getEditorialVoiceDirectionGroup() const;
            void setEditorialVoiceDirectionGroup( const EditorialVoiceDirectionGroupPtr& value );

            /* _________ Staff minOccurs = 0, maxOccurs = 1 _________ */
            StaffPtr getStaff() const;
            void setStaff( const StaffPtr& value );
            bool getHasStaff() const;
            void setHasStaff( const bool value );

            /* _________ Sound minOccurs = 0, maxOccurs = 1 _________ */
            SoundPtr getSound() const;
            void setSound( const SoundPtr& value );
            bool getHasSound() const;
            void setHasSound( const bool value );

        private:
            DirectionAttributesPtr myAttributes;
            DirectionTypeSet myDirectionTypeSet;
            OffsetPtr myOffset;
            bool myHasOffset;
            EditorialVoiceDirectionGroupPtr myEditorialVoiceDirectionGroup;
            StaffPtr myStaff;
            bool myHasStaff;
            SoundPtr mySound;
            bool myHasSound;
        };
        
        
        
        class Properties;
        using PropertiesPtr = std::shared_ptr<Properties>;
        using PropertiesUPtr = std::unique_ptr<Properties>;
        using PropertiesSet = std::vector<PropertiesPtr>;
        using PropertiesSetIter = PropertiesSet::iterator;
        using PropertiesSetIterConst = PropertiesSet::const_iterator;
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
        
        
        
        
        struct ScorePartAttributes;
        using ScorePartAttributesPtr = std::shared_ptr<ScorePartAttributes>;
        
        struct ScorePartAttributes : public AttributesInterface
        {
        public:
            ScorePartAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsID id;
            const 	bool hasId;
        };
        
        class MidiDeviceInstrumentGroup;
        using MidiDeviceInstrumentGroupPtr = std::shared_ptr<MidiDeviceInstrumentGroup>;
        using MidiDeviceInstrumentGroupUPtr = std::unique_ptr<MidiDeviceInstrumentGroup>;
        using MidiDeviceInstrumentGroupSet = std::vector<MidiDeviceInstrumentGroupPtr>;
        using MidiDeviceInstrumentGroupSetIter = MidiDeviceInstrumentGroupSet::iterator;
        using MidiDeviceInstrumentGroupSetIterConst = MidiDeviceInstrumentGroupSet::const_iterator;
        inline MidiDeviceInstrumentGroupPtr makeMidiDeviceInstrumentGroup() { return std::make_shared<MidiDeviceInstrumentGroup>(); }

        class MidiDeviceInstrumentGroup : public ElementInterface
        {
        public:
            MidiDeviceInstrumentGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ MidiDevice minOccurs = 0, maxOccurs = 1 _________ */
            MidiDevicePtr getMidiDevice() const;
            void setMidiDevice( const MidiDevicePtr& value );
            bool getHasMidiDevice() const;
            void setHasMidiDevice( const bool value );

            /* _________ MidiInstrument minOccurs = 0, maxOccurs = 1 _________ */
            MidiInstrumentPtr getMidiInstrument() const;
            void setMidiInstrument( const MidiInstrumentPtr& value );
            bool getHasMidiInstrument() const;
            void setHasMidiInstrument( const bool value );

        private:
            MidiDevicePtr myMidiDevice;
            bool myHasMidiDevice;
            MidiInstrumentPtr myMidiInstrument;
            bool myHasMidiInstrument;
        };
        
        class ScorePart;
        using ScorePartPtr = std::shared_ptr<ScorePart>;
        using ScorePartUPtr = std::unique_ptr<ScorePart>;
        using ScorePartSet = std::vector<ScorePartPtr>;
        using ScorePartSetIter = ScorePartSet::iterator;
        using ScorePartSetIterConst = ScorePartSet::const_iterator;
        inline ScorePartPtr makeScorePart() { return std::make_shared<ScorePart>(); }

        class ScorePart : public ElementInterface
        {
        public:
            ScorePart();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ScorePartAttributesPtr getAttributes() const;
            void setAttributes( const ScorePartAttributesPtr& value );

            /* _________ Identification minOccurs = 0, maxOccurs = 1 _________ */
            IdentificationPtr getIdentification() const;
            void setIdentification( const IdentificationPtr& value );
            bool getHasIdentification() const;
            void setHasIdentification( const bool value );

            /* _________ PartName minOccurs = 1, maxOccurs = 1 _________ */
            PartNamePtr getPartName() const;
            void setPartName( const PartNamePtr& value );

            /* _________ PartNameDisplay minOccurs = 0, maxOccurs = 1 _________ */
            PartNameDisplayPtr getPartNameDisplay() const;
            void setPartNameDisplay( const PartNameDisplayPtr& value );
            bool getHasPartNameDisplay() const;
            void setHasPartNameDisplay( const bool value );

            /* _________ PartAbbreviation minOccurs = 0, maxOccurs = 1 _________ */
            PartAbbreviationPtr getPartAbbreviation() const;
            void setPartAbbreviation( const PartAbbreviationPtr& value );
            bool getHasPartAbbreviation() const;
            void setHasPartAbbreviation( const bool value );

            /* _________ PartAbbreviationDisplay minOccurs = 0, maxOccurs = 1 _________ */
            PartAbbreviationDisplayPtr getPartAbbreviationDisplay() const;
            void setPartAbbreviationDisplay( const PartAbbreviationDisplayPtr& value );
            bool getHasPartAbbreviationDisplay() const;
            void setHasPartAbbreviationDisplay( const bool value );

            /* _________ Group minOccurs = 0, maxOccurs = unbounded _________ */
            const GroupSet& getGroupSet() const;
            void addGroup( const GroupPtr& value );
            void removeGroup( const GroupSetIterConst& value );
            void clearGroupSet();
            GroupPtr getGroup( const GroupSetIterConst& setIterator ) const;

            /* _________ ScoreInstrument minOccurs = 0, maxOccurs = unbounded _________ */
            const ScoreInstrumentSet& getScoreInstrumentSet() const;
            void addScoreInstrument( const ScoreInstrumentPtr& value );
            void removeScoreInstrument( const ScoreInstrumentSetIterConst& value );
            void clearScoreInstrumentSet();
            ScoreInstrumentPtr getScoreInstrument( const ScoreInstrumentSetIterConst& setIterator ) const;

            /* _________ MidiDeviceInstrumentGroup minOccurs = 0, maxOccurs = unbounded _________ */
            const MidiDeviceInstrumentGroupSet& getMidiDeviceInstrumentGroupSet() const;
            void addMidiDeviceInstrumentGroup( const MidiDeviceInstrumentGroupPtr& value );
            void removeMidiDeviceInstrumentGroup( const MidiDeviceInstrumentGroupSetIterConst& value );
            void clearMidiDeviceInstrumentGroupSet();
            MidiDeviceInstrumentGroupPtr getMidiDeviceInstrumentGroup( const MidiDeviceInstrumentGroupSetIterConst& setIterator ) const;

        private:
            ScorePartAttributesPtr myAttributes;
            IdentificationPtr myIdentification;
            bool myHasIdentification;
            PartNamePtr myPartName;
            PartNameDisplayPtr myPartNameDisplay;
            bool myHasPartNameDisplay;
            PartAbbreviationPtr myPartAbbreviation;
            bool myHasPartAbbreviation;
            PartAbbreviationDisplayPtr myPartAbbreviationDisplay;
            bool myHasPartAbbreviationDisplay;
            GroupSet myGroupSet;
            ScoreInstrumentSet myScoreInstrumentSet;
            MidiDeviceInstrumentGroupSet myMidiDeviceInstrumentGroupSet;
        };
        
        
        class NotationsChoice;
        using NotationsChoicePtr = std::shared_ptr<NotationsChoice>;
        using NotationsChoiceUPtr = std::unique_ptr<NotationsChoice>;
        using NotationsChoiceSet = std::vector<NotationsChoicePtr>;
        using NotationsChoiceSetIter = NotationsChoiceSet::iterator;
        using NotationsChoiceSetIterConst = NotationsChoiceSet::const_iterator;
        inline NotationsChoicePtr makeNotationsChoice() { return std::make_shared<NotationsChoice>(); }

        class NotationsChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                tied = 1,
                slur = 2,
                tuplet = 3,
                glissando = 4,
                slide = 5,
                ornaments = 6,
                technical = 7,
                articulations = 8,
                dynamics = 9,
                fermata = 10,
                arpeggiate = 11,
                nonArpeggiate = 12,
                accidentalMark = 13,
                otherNotation = 14
            };
            NotationsChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            NotationsChoice::Choice getChoice() const;
            void setChoice( const NotationsChoice::Choice value );

            /* _________ Tied minOccurs = 1, maxOccurs = 1 _________ */
            TiedPtr getTied() const;
            void setTied( const TiedPtr& value );

            /* _________ Slur minOccurs = 1, maxOccurs = 1 _________ */
            SlurPtr getSlur() const;
            void setSlur( const SlurPtr& value );

            /* _________ Tuplet minOccurs = 1, maxOccurs = 1 _________ */
            TupletPtr getTuplet() const;
            void setTuplet( const TupletPtr& value );

            /* _________ Glissando minOccurs = 1, maxOccurs = 1 _________ */
            GlissandoPtr getGlissando() const;
            void setGlissando( const GlissandoPtr& value );

            /* _________ Slide minOccurs = 1, maxOccurs = 1 _________ */
            SlidePtr getSlide() const;
            void setSlide( const SlidePtr& value );

            /* _________ Ornaments minOccurs = 1, maxOccurs = 1 _________ */
            OrnamentsPtr getOrnaments() const;
            void setOrnaments( const OrnamentsPtr& value );

            /* _________ Technical minOccurs = 1, maxOccurs = 1 _________ */
            TechnicalPtr getTechnical() const;
            void setTechnical( const TechnicalPtr& value );

            /* _________ Articulations minOccurs = 1, maxOccurs = 1 _________ */
            ArticulationsPtr getArticulations() const;
            void setArticulations( const ArticulationsPtr& value );

            /* _________ Dynamics minOccurs = 1, maxOccurs = 1 _________ */
            DynamicsPtr getDynamics() const;
            void setDynamics( const DynamicsPtr& value );

            /* _________ Fermata minOccurs = 1, maxOccurs = 1 _________ */
            FermataPtr getFermata() const;
            void setFermata( const FermataPtr& value );

            /* _________ Arpeggiate minOccurs = 1, maxOccurs = 1 _________ */
            ArpeggiatePtr getArpeggiate() const;
            void setArpeggiate( const ArpeggiatePtr& value );

            /* _________ NonArpeggiate minOccurs = 1, maxOccurs = 1 _________ */
            NonArpeggiatePtr getNonArpeggiate() const;
            void setNonArpeggiate( const NonArpeggiatePtr& value );

            /* _________ AccidentalMark minOccurs = 1, maxOccurs = 1 _________ */
            AccidentalMarkPtr getAccidentalMark() const;
            void setAccidentalMark( const AccidentalMarkPtr& value );

            /* _________ OtherNotation minOccurs = 1, maxOccurs = 1 _________ */
            OtherNotationPtr getOtherNotation() const;
            void setOtherNotation( const OtherNotationPtr& value );

        private:
            Choice myChoice;
            TiedPtr myTied;
            SlurPtr mySlur;
            TupletPtr myTuplet;
            GlissandoPtr myGlissando;
            SlidePtr mySlide;
            OrnamentsPtr myOrnaments;
            TechnicalPtr myTechnical;
            ArticulationsPtr myArticulations;
            DynamicsPtr myDynamics;
            FermataPtr myFermata;
            ArpeggiatePtr myArpeggiate;
            NonArpeggiatePtr myNonArpeggiate;
            AccidentalMarkPtr myAccidentalMark;
            OtherNotationPtr myOtherNotation;
        };
        
        struct NotationsAttributes;
        using NotationsAttributesPtr = std::shared_ptr<NotationsAttributes>;
        
        struct NotationsAttributes : public AttributesInterface
        {
        public:
            NotationsAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo printObject;
            bool hasPrintObject;
        };
        
        class Notations;
        using NotationsPtr = std::shared_ptr<Notations>;
        using NotationsUPtr = std::unique_ptr<Notations>;
        using NotationsSet = std::vector<NotationsPtr>;
        using NotationsSetIter = NotationsSet::iterator;
        using NotationsSetIterConst = NotationsSet::const_iterator;
        inline NotationsPtr makeNotations() { return std::make_shared<Notations>(); }

        class Notations : public ElementInterface
        {
        public:
            Notations();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NotationsAttributesPtr getAttributes() const;
            void setAttributes( const NotationsAttributesPtr& value );

            /* _________ EditorialGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialGroupPtr getEditorialGroup() const;
            void setEditorialGroup( const EditorialGroupPtr& value );

            /* _________ NotationsChoice minOccurs = 0, maxOccurs = unbounded _________ */
            const NotationsChoiceSet& getNotationsChoiceSet() const;
            void addNotationsChoice( const NotationsChoicePtr& value );
            void removeNotationsChoice( const NotationsChoiceSetIterConst& value );
            void clearNotationsChoiceSet();
            NotationsChoicePtr getNotationsChoice( const NotationsChoiceSetIterConst& setIterator ) const;

        private:
            NotationsAttributesPtr myAttributes;
            EditorialGroupPtr myEditorialGroup;
            NotationsChoiceSet myNotationsChoiceSet;
        };
        
        
        class FullNoteTypeChoice;
        using FullNoteTypeChoicePtr = std::shared_ptr<FullNoteTypeChoice>;
        using FullNoteTypeChoiceUPtr = std::unique_ptr<FullNoteTypeChoice>;
        using FullNoteTypeChoiceSet = std::vector<FullNoteTypeChoicePtr>;
        using FullNoteTypeChoiceSetIter = FullNoteTypeChoiceSet::iterator;
        using FullNoteTypeChoiceSetIterConst = FullNoteTypeChoiceSet::const_iterator;
        inline FullNoteTypeChoicePtr makeFullNoteTypeChoice() { return std::make_shared<FullNoteTypeChoice>(); }

        class FullNoteTypeChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                pitch = 1,
                unpitched = 2,
                rest = 3
            };
            FullNoteTypeChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            FullNoteTypeChoice::Choice getChoice() const;
            void setChoice( const FullNoteTypeChoice::Choice value );

            /* _________ Pitch minOccurs = 1, maxOccurs = 1 _________ */
            PitchPtr getPitch() const;
            void setPitch( const PitchPtr& value );

            /* _________ Unpitched minOccurs = 1, maxOccurs = 1 _________ */
            UnpitchedPtr getUnpitched() const;
            void setUnpitched( const UnpitchedPtr& value );

            /* _________ Rest minOccurs = 1, maxOccurs = 1 _________ */
            RestPtr getRest() const;
            void setRest( const RestPtr& value );

        private:
            Choice myChoice;
            PitchPtr myPitch;
            UnpitchedPtr myUnpitched;
            RestPtr myRest;
        };
        
        class FullNoteGroup;
        using FullNoteGroupPtr = std::shared_ptr<FullNoteGroup>;
        using FullNoteGroupUPtr = std::unique_ptr<FullNoteGroup>;
        using FullNoteGroupSet = std::vector<FullNoteGroupPtr>;
        using FullNoteGroupSetIter = FullNoteGroupSet::iterator;
        using FullNoteGroupSetIterConst = FullNoteGroupSet::const_iterator;
        inline FullNoteGroupPtr makeFullNoteGroup() { return std::make_shared<FullNoteGroup>(); }

        class FullNoteGroup : public ElementInterface
        {
        public:
            FullNoteGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Chord minOccurs = 0, maxOccurs = 1 _________ */
            ChordPtr getChord() const;
            void setChord( const ChordPtr& value );
            bool getHasChord() const;
            void setHasChord( const bool value );

            /* _________ FullNoteTypeChoice minOccurs = 1, maxOccurs = 1 _________ */
            FullNoteTypeChoicePtr getFullNoteTypeChoice() const;
            void setFullNoteTypeChoice( const FullNoteTypeChoicePtr& value );

        private:
            ChordPtr myChord;
            bool myHasChord;
            FullNoteTypeChoicePtr myFullNoteTypeChoice;
        };
        
        class GraceNoteGroup;
        using GraceNoteGroupPtr = std::shared_ptr<GraceNoteGroup>;
        using GraceNoteGroupUPtr = std::unique_ptr<GraceNoteGroup>;
        using GraceNoteGroupSet = std::vector<GraceNoteGroupPtr>;
        using GraceNoteGroupSetIter = GraceNoteGroupSet::iterator;
        using GraceNoteGroupSetIterConst = GraceNoteGroupSet::const_iterator;
        inline GraceNoteGroupPtr makeGraceNoteGroup() { return std::make_shared<GraceNoteGroup>(); }

        class GraceNoteGroup : public ElementInterface
        {
        public:
            GraceNoteGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Grace minOccurs = 1, maxOccurs = 1 _________ */
            GracePtr getGrace() const;
            void setGrace( const GracePtr& value );

            /* _________ FullNoteGroup minOccurs = 1, maxOccurs = 1 _________ */
            FullNoteGroupPtr getFullNoteGroup() const;
            void setFullNoteGroup( const FullNoteGroupPtr& value );

            /* _________ Tie minOccurs = 0, maxOccurs = 2 _________ */
            const TieSet& getTieSet() const;
            void addTie( const TiePtr& value );
            void removeTie( const TieSetIterConst& value );
            void clearTieSet();
            TiePtr getTie( const TieSetIterConst& setIterator ) const;

        private:
            GracePtr myGrace;
            FullNoteGroupPtr myFullNoteGroup;
            TieSet myTieSet;
        };
        
        class CueNoteGroup;
        using CueNoteGroupPtr = std::shared_ptr<CueNoteGroup>;
        using CueNoteGroupUPtr = std::unique_ptr<CueNoteGroup>;
        using CueNoteGroupSet = std::vector<CueNoteGroupPtr>;
        using CueNoteGroupSetIter = CueNoteGroupSet::iterator;
        using CueNoteGroupSetIterConst = CueNoteGroupSet::const_iterator;
        inline CueNoteGroupPtr makeCueNoteGroup() { return std::make_shared<CueNoteGroup>(); }

        class CueNoteGroup : public ElementInterface
        {
        public:
            CueNoteGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Cue minOccurs = 1, maxOccurs = 1 _________ */
            CuePtr getCue() const;
            void setCue( const CuePtr& value );

            /* _________ FullNoteGroup minOccurs = 1, maxOccurs = 1 _________ */
            FullNoteGroupPtr getFullNoteGroup() const;
            void setFullNoteGroup( const FullNoteGroupPtr& value );

            /* _________ Duration minOccurs = 1, maxOccurs = 1 _________ */
            DurationPtr getDuration() const;
            void setDuration( const DurationPtr& value );

        private:
            CuePtr myCue;
            FullNoteGroupPtr myFullNoteGroup;
            DurationPtr myDuration;
        };
        
        class NormalNoteGroup;
        using NormalNoteGroupPtr = std::shared_ptr<NormalNoteGroup>;
        using NormalNoteGroupUPtr = std::unique_ptr<NormalNoteGroup>;
        using NormalNoteGroupSet = std::vector<NormalNoteGroupPtr>;
        using NormalNoteGroupSetIter = NormalNoteGroupSet::iterator;
        using NormalNoteGroupSetIterConst = NormalNoteGroupSet::const_iterator;
        inline NormalNoteGroupPtr makeNormalNoteGroup() { return std::make_shared<NormalNoteGroup>(); }

        class NormalNoteGroup : public ElementInterface
        {
        public:
            NormalNoteGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ FullNoteGroup minOccurs = 1, maxOccurs = 1 _________ */
            FullNoteGroupPtr getFullNoteGroup() const;
            void setFullNoteGroup( const FullNoteGroupPtr& value );

            /* _________ Duration minOccurs = 1, maxOccurs = 1 _________ */
            DurationPtr getDuration() const;
            void setDuration( const DurationPtr& value );

            /* _________ Tie minOccurs = 0, maxOccurs = 2 _________ */
            const TieSet& getTieSet() const;
            void addTie( const TiePtr& value );
            void removeTie( const TieSetIterConst& value );
            void clearTieSet();
            TiePtr getTie( const TieSetIterConst& setIterator ) const;

        private:
            FullNoteGroupPtr myFullNoteGroup;
            DurationPtr myDuration;
            TieSet myTieSet;
        };
        
        class NoteChoice;
        using NoteChoicePtr = std::shared_ptr<NoteChoice>;
        using NoteChoiceUPtr = std::unique_ptr<NoteChoice>;
        using NoteChoiceSet = std::vector<NoteChoicePtr>;
        using NoteChoiceSetIter = NoteChoiceSet::iterator;
        using NoteChoiceSetIterConst = NoteChoiceSet::const_iterator;
        inline NoteChoicePtr makeNoteChoice() { return std::make_shared<NoteChoice>(); }

        class NoteChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                grace = 1,
                cue = 2,
                normal = 3
            };
            NoteChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            NoteChoice::Choice getChoice() const;
            void setChoice( const NoteChoice::Choice value );

            /* _________ GraceNoteGroup minOccurs = 1, maxOccurs = 1 _________ */
            GraceNoteGroupPtr getGraceNoteGroup() const;
            void setGraceNoteGroup( const GraceNoteGroupPtr& value );

            /* _________ CueNoteGroup minOccurs = 1, maxOccurs = 1 _________ */
            CueNoteGroupPtr getCueNoteGroup() const;
            void setCueNoteGroup( const CueNoteGroupPtr& value );

            /* _________ NormalNoteGroup minOccurs = 1, maxOccurs = 1 _________ */
            NormalNoteGroupPtr getNormalNoteGroup() const;
            void setNormalNoteGroup( const NormalNoteGroupPtr& value );

        private:
            Choice myChoice;
            GraceNoteGroupPtr myGraceNoteGroup;
            CueNoteGroupPtr myCueNoteGroup;
            NormalNoteGroupPtr myNormalNoteGroup;
        };
        
        struct NoteAttributes;
        using NoteAttributesPtr = std::shared_ptr<NoteAttributes>;
        
        struct NoteAttributes : public AttributesInterface
        {
        public:
            NoteAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            YesNo printObject;
            YesNo printDot;
            YesNo printSpacing;
            YesNo printLyric;
            NonNegativeDecimal dynamics;
            NonNegativeDecimal endDynamics;
            DivisionsValue attack;
            DivisionsValue release;
            TimeOnly timeOnly;
            YesNo pizzicato;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasPrintObject;
            bool hasPrintDot;
            bool hasPrintSpacing;
            bool hasPrintLyric;
            bool hasDynamics;
            bool hasEndDynamics;
            bool hasAttack;
            bool hasRelease;
            bool hasTimeOnly;
            bool hasPizzicato;
        };
        
        class Note;
        using NotePtr = std::shared_ptr<Note>;
        using NoteUPtr = std::unique_ptr<Note>;
        using NoteSet = std::vector<NotePtr>;
        using NoteSetIter = NoteSet::iterator;
        using NoteSetIterConst = NoteSet::const_iterator;
        inline NotePtr makeNote() { return std::make_shared<Note>(); }

        class Note : public ElementInterface
        {
        public:
            Note();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NoteAttributesPtr getAttributes() const;
            void setAttributes( const NoteAttributesPtr& value );

            /* _________ NoteChoice minOccurs = 1, maxOccurs = 1 _________ */
            NoteChoicePtr getNoteChoice() const;
            void setNoteChoice( const NoteChoicePtr& value );

            /* _________ Instrument minOccurs = 0, maxOccurs = 1 _________ */
            InstrumentPtr getInstrument() const;
            void setInstrument( const InstrumentPtr& value );
            bool getHasInstrument() const;
            void setHasInstrument( const bool value );

            /* _________ EditorialVoiceGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialVoiceGroupPtr getEditorialVoiceGroup() const;
            void setEditorialVoiceGroup( const EditorialVoiceGroupPtr& value );

            /* _________ Type minOccurs = 0, maxOccurs = 1 _________ */
            TypePtr getType() const;
            void setType( const TypePtr& value );
            bool getHasType() const;
            void setHasType( const bool value );

            /* _________ Dot minOccurs = 0, maxOccurs = unbounded _________ */
            const DotSet& getDotSet() const;
            void addDot( const DotPtr& value );
            void removeDot( const DotSetIterConst& value );
            void clearDotSet();
            DotPtr getDot( const DotSetIterConst& setIterator ) const;

            /* _________ Accidental minOccurs = 0, maxOccurs = 1 _________ */
            AccidentalPtr getAccidental() const;
            void setAccidental( const AccidentalPtr& value );
            bool getHasAccidental() const;
            void setHasAccidental( const bool value );

            /* _________ TimeModification minOccurs = 0, maxOccurs = 1 _________ */
            TimeModificationPtr getTimeModification() const;
            void setTimeModification( const TimeModificationPtr& value );
            bool getHasTimeModification() const;
            void setHasTimeModification( const bool value );

            /* _________ Stem minOccurs = 0, maxOccurs = 1 _________ */
            StemPtr getStem() const;
            void setStem( const StemPtr& value );
            bool getHasStem() const;
            void setHasStem( const bool value );

            /* _________ Notehead minOccurs = 0, maxOccurs = 1 _________ */
            NoteheadPtr getNotehead() const;
            void setNotehead( const NoteheadPtr& value );
            bool getHasNotehead() const;
            void setHasNotehead( const bool value );

            /* _________ NoteheadText minOccurs = 0, maxOccurs = 1 _________ */
            NoteheadTextPtr getNoteheadText() const;
            void setNoteheadText( const NoteheadTextPtr& value );
            bool getHasNoteheadText() const;
            void setHasNoteheadText( const bool value );

            /* _________ Staff minOccurs = 0, maxOccurs = 1 _________ */
            StaffPtr getStaff() const;
            void setStaff( const StaffPtr& value );
            bool getHasStaff() const;
            void setHasStaff( const bool value );

            /* _________ Beam minOccurs = 0, maxOccurs = 8 _________ */
            const BeamSet& getBeamSet() const;
            void addBeam( const BeamPtr& value );
            void removeBeam( const BeamSetIterConst& value );
            void clearBeamSet();
            BeamPtr getBeam( const BeamSetIterConst& setIterator ) const;

            /* _________ Notations minOccurs = 0, maxOccurs = unbounded _________ */
            const NotationsSet& getNotationsSet() const;
            void addNotations( const NotationsPtr& value );
            void removeNotations( const NotationsSetIterConst& value );
            void clearNotationsSet();
            NotationsPtr getNotations( const NotationsSetIterConst& setIterator ) const;

            /* _________ Lyric minOccurs = 0, maxOccurs = unbounded _________ */
            const LyricSet& getLyricSet() const;
            void addLyric( const LyricPtr& value );
            void removeLyric( const LyricSetIterConst& value );
            void clearLyricSet();
            LyricPtr getLyric( const LyricSetIterConst& setIterator ) const;

            /* _________ Play minOccurs = 0, maxOccurs = 1 _________ */
            PlayPtr getPlay() const;
            void setPlay( const PlayPtr& value );
            bool getHasPlay() const;
            void setHasPlay( const bool value );

        private:
            NoteAttributesPtr myAttributes;
            NoteChoicePtr myNoteChoice;
            InstrumentPtr myInstrument;
            bool myHasInstrument;
            EditorialVoiceGroupPtr myEditorialVoiceGroup;
            TypePtr myType;
            bool myHasType;
            DotSet myDotSet;
            AccidentalPtr myAccidental;
            bool myHasAccidental;
            TimeModificationPtr myTimeModification;
            bool myHasTimeModification;
            StemPtr myStem;
            bool myHasStem;
            NoteheadPtr myNotehead;
            bool myHasNotehead;
            NoteheadTextPtr myNoteheadText;
            bool myHasNoteheadText;
            StaffPtr myStaff;
            bool myHasStaff;
            BeamSet myBeamSet;
            NotationsSet myNotationsSet;
            LyricSet myLyricSet;
            PlayPtr myPlay;
            bool myHasPlay;
        };
        
        
        class PartGroupOrScorePart;
        using PartGroupOrScorePartPtr = std::shared_ptr<PartGroupOrScorePart>;
        using PartGroupOrScorePartUPtr = std::unique_ptr<PartGroupOrScorePart>;
        using PartGroupOrScorePartSet = std::vector<PartGroupOrScorePartPtr>;
        using PartGroupOrScorePartSetIter = PartGroupOrScorePartSet::iterator;
        using PartGroupOrScorePartSetIterConst = PartGroupOrScorePartSet::const_iterator;
        inline PartGroupOrScorePartPtr makePartGroupOrScorePart() { return std::make_shared<PartGroupOrScorePart>(); }

        class PartGroupOrScorePart : public ElementInterface
        {
        public:
            enum class Choice
            {
                partGroup = 1,
                scorePart = 2
            };
            PartGroupOrScorePart();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            PartGroupOrScorePart::Choice getChoice() const;
            void setChoice( const PartGroupOrScorePart::Choice value );

            /* _________ PartGroup minOccurs = 1, maxPartGroupOccurs = 1 _________ */
            PartGroupPtr getPartGroup() const;
            void setPartGroup( const PartGroupPtr& value );

            /* _________ ScorePart minOccurs = 1, maxOccurs = 1 _________ */
            ScorePartPtr getScorePart() const;
            void setScorePart( const ScorePartPtr& value );

        private:
            Choice myChoice;
            PartGroupPtr myPartGroup;
            ScorePartPtr myScorePart;
        };
        
        class PartList;
        using PartListPtr = std::shared_ptr<PartList>;
        using PartListUPtr = std::unique_ptr<PartList>;
        using PartListSet = std::vector<PartListPtr>;
        using PartListSetIter = PartListSet::iterator;
        using PartListSetIterConst = PartListSet::const_iterator;
        inline PartListPtr makePartList() { return std::make_shared<PartList>(); }

        class PartList : public ElementInterface
        {
        public:
            PartList();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ PartGroup minOccurs = 0, maxOccurs = unbounded _________ */
            const PartGroupSet& getPartGroupSet() const;
            void addPartGroup( const PartGroupPtr& value );
            void removePartGroup( const PartGroupSetIterConst& value );
            void clearPartGroupSet();
            PartGroupPtr getPartGroup( const PartGroupSetIterConst& setIterator ) const;

            /* _________ ScorePart minOccurs = 1, maxOccurs = 1 _________ */
            ScorePartPtr getScorePart() const;
            void setScorePart( const ScorePartPtr& value );

            /* _________ PartGroupOrScorePart minOccurs = 0, maxOccurs = unbounded _________ */
            const PartGroupOrScorePartSet& getPartGroupOrScorePartSet() const;
            void addPartGroupOrScorePart( const PartGroupOrScorePartPtr& value );
            void removePartGroupOrScorePart( const PartGroupOrScorePartSetIterConst& value );
            void clearPartGroupOrScorePartSet();
            PartGroupOrScorePartPtr getPartGroupOrScorePart( const PartGroupOrScorePartSetIterConst& setIterator ) const;

        private:
            PartGroupSet myPartGroupSet;
            ScorePartPtr myScorePart;
            PartGroupOrScorePartSet myPartGroupOrScorePartSet;
        };
        
        
        class MusicDataChoice;
        using MusicDataChoicePtr = std::shared_ptr<MusicDataChoice>;
        using MusicDataChoiceUPtr = std::unique_ptr<MusicDataChoice>;
        using MusicDataChoiceSet = std::vector<MusicDataChoicePtr>;
        using MusicDataChoiceSetIter = MusicDataChoiceSet::iterator;
        using MusicDataChoiceSetIterConst = MusicDataChoiceSet::const_iterator;
        inline MusicDataChoicePtr makeMusicDataChoice() { return std::make_shared<MusicDataChoice>(); }

        class MusicDataChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                note = 1,
                backup = 2,
                forward = 3,
                direction = 4,
                properties = 5,
                harmony = 6,
                figuredBass = 7,
                print = 8,
                sound = 9,
                barline = 10,
                grouping = 11,
                link = 12,
                bookmark = 13
            };
            MusicDataChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            MusicDataChoice::Choice getChoice() const;
            void setChoice( const MusicDataChoice::Choice value );

            /* _________ Note minOccurs = 1, maxOccurs = 1 _________ */
            NotePtr getNote() const;
            void setNote( const NotePtr& value );

            /* _________ Backup minOccurs = 1, maxOccurs = 1 _________ */
            BackupPtr getBackup() const;
            void setBackup( const BackupPtr& value );

            /* _________ Forward minOccurs = 1, maxOccurs = 1 _________ */
            ForwardPtr getForward() const;
            void setForward( const ForwardPtr& value );

            /* _________ Direction minOccurs = 1, maxOccurs = 1 _________ */
            DirectionPtr getDirection() const;
            void setDirection( const DirectionPtr& value );

            /* _________ Properties minOccurs = 1, maxOccurs = 1 _________ */
            PropertiesPtr getProperties() const;
            void setProperties( const PropertiesPtr& value );

            /* _________ Harmony minOccurs = 1, maxOccurs = 1 _________ */
            HarmonyPtr getHarmony() const;
            void setHarmony( const HarmonyPtr& value );

            /* _________ FiguredBass minOccurs = 1, maxOccurs = 1 _________ */
            FiguredBassPtr getFiguredBass() const;
            void setFiguredBass( const FiguredBassPtr& value );

            /* _________ Print minOccurs = 1, maxOccurs = 1 _________ */
            PrintPtr getPrint() const;
            void setPrint( const PrintPtr& value );

            /* _________ Sound minOccurs = 1, maxOccurs = 1 _________ */
            SoundPtr getSound() const;
            void setSound( const SoundPtr& value );

            /* _________ Barline minOccurs = 1, maxOccurs = 1 _________ */
            BarlinePtr getBarline() const;
            void setBarline( const BarlinePtr& value );

            /* _________ Grouping minOccurs = 1, maxOccurs = 1 _________ */
            GroupingPtr getGrouping() const;
            void setGrouping( const GroupingPtr& value );

            /* _________ Link minOccurs = 1, maxOccurs = 1 _________ */
            LinkPtr getLink() const;
            void setLink( const LinkPtr& value );

            /* _________ Bookmark minOccurs = 1, maxOccurs = 1 _________ */
            BookmarkPtr getBookmark() const;
            void setBookmark( const BookmarkPtr& value );

        private:
            Choice myChoice;
            NotePtr myNote;
            BackupPtr myBackup;
            ForwardPtr myForward;
            DirectionPtr myDirection;
            PropertiesPtr myProperties;
            HarmonyPtr myHarmony;
            FiguredBassPtr myFiguredBass;
            PrintPtr myPrint;
            SoundPtr mySound;
            BarlinePtr myBarline;
            GroupingPtr myGrouping;
            LinkPtr myLink;
            BookmarkPtr myBookmark;
        };
        
        class MusicDataGroup;
        using MusicDataGroupPtr = std::shared_ptr<MusicDataGroup>;
        using MusicDataGroupUPtr = std::unique_ptr<MusicDataGroup>;
        using MusicDataGroupSet = std::vector<MusicDataGroupPtr>;
        using MusicDataGroupSetIter = MusicDataGroupSet::iterator;
        using MusicDataGroupSetIterConst = MusicDataGroupSet::const_iterator;
        inline MusicDataGroupPtr makeMusicDataGroup() { return std::make_shared<MusicDataGroup>(); }

        class MusicDataGroup : public ElementInterface
        {
        public:
            MusicDataGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ MusicDataChoice minOccurs = 0, maxOccurs = unbounded _________ */
            const MusicDataChoiceSet& getMusicDataChoiceSet() const;
            void addMusicDataChoice( const MusicDataChoicePtr& value );
            void removeMusicDataChoice( const MusicDataChoiceSetIterConst& value );
            void clearMusicDataChoiceSet();
            MusicDataChoicePtr getMusicDataChoice( const MusicDataChoiceSetIterConst& setIterator ) const;

        private:
            MusicDataChoiceSet myMusicDataChoiceSet;
        };
        
        struct MeasureAttributes;
        using MeasureAttributesPtr = std::shared_ptr<MeasureAttributes>;
        
        struct MeasureAttributes : public AttributesInterface
        {
        public:
            MeasureAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken number;
            YesNo implicit;
            YesNo nonControlling;
            TenthsValue width;
            const 	bool hasNumber;
            bool hasImplicit;
            bool hasNonControlling;
            bool hasWidth;
        };
        
        class PartwiseMeasure;
        using PartwiseMeasurePtr = std::shared_ptr<PartwiseMeasure>;
        using PartwiseMeasureUPtr = std::unique_ptr<PartwiseMeasure>;
        using PartwiseMeasureSet = std::vector<PartwiseMeasurePtr>;
        using PartwiseMeasureSetIter = PartwiseMeasureSet::iterator;
        using PartwiseMeasureSetIterConst = PartwiseMeasureSet::const_iterator;
        inline PartwiseMeasurePtr makePartwiseMeasure() { return std::make_shared<PartwiseMeasure>(); }

        class PartwiseMeasure : public ElementInterface
        {
        public:
            PartwiseMeasure();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MeasureAttributesPtr getAttributes() const;
            void setAttributes( const MeasureAttributesPtr& value );

            /* _________ MusicDataGroup minOccurs = 1, maxOccurs = 1 _________ */
            MusicDataGroupPtr getMusicDataGroup() const;
            void setMusicDataGroup( const MusicDataGroupPtr& value );

        private:
            MeasureAttributesPtr myAttributes;
            MusicDataGroupPtr myMusicDataGroup;
        };
        
        struct PartAttributes;
        using PartAttributesPtr = std::shared_ptr<PartAttributes>;
        
        struct PartAttributes : public AttributesInterface
        {
        public:
            PartAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsIDREF id;
            const bool hasId;
        };
        
        class PartwisePart;
        using PartwisePartPtr = std::shared_ptr<PartwisePart>;
        using PartwisePartUPtr = std::unique_ptr<PartwisePart>;
        using PartwisePartSet = std::vector<PartwisePartPtr>;
        using PartwisePartSetIter = PartwisePartSet::iterator;
        using PartwisePartSetIterConst = PartwisePartSet::const_iterator;
        inline PartwisePartPtr makePartwisePart() { return std::make_shared<PartwisePart>(); }

        class PartwisePart : public ElementInterface
        {
        public:
            PartwisePart();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PartAttributesPtr getAttributes() const;
            void setAttributes( const PartAttributesPtr& value );

            /* _________ PartwiseMeasure minOccurs = 0, maxOccurs = unbounded _________ */
            const PartwiseMeasureSet& getPartwiseMeasureSet() const;
            void addPartwiseMeasure( const PartwiseMeasurePtr& value );
            void removePartwiseMeasure( const PartwiseMeasureSetIterConst& value );
            void clearPartwiseMeasureSet();
            PartwiseMeasurePtr getPartwiseMeasure( const PartwiseMeasureSetIterConst& setIterator ) const;

        private:
            PartAttributesPtr myAttributes;
            PartwiseMeasureSet myPartwiseMeasureSet;
        };
        
        class ScoreHeaderGroup;
        using ScoreHeaderGroupPtr = std::shared_ptr<ScoreHeaderGroup>;
        using ScoreHeaderGroupUPtr = std::unique_ptr<ScoreHeaderGroup>;
        using ScoreHeaderGroupSet = std::vector<ScoreHeaderGroupPtr>;
        using ScoreHeaderGroupSetIter = ScoreHeaderGroupSet::iterator;
        using ScoreHeaderGroupSetIterConst = ScoreHeaderGroupSet::const_iterator;
        inline ScoreHeaderGroupPtr makeScoreHeaderGroup() { return std::make_shared<ScoreHeaderGroup>(); }

        class ScoreHeaderGroup : public ElementInterface
        {
        public:
            ScoreHeaderGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Work minOccurs = 0, maxOccurs = 1 _________ */
            WorkPtr getWork() const;
            void setWork( const WorkPtr& value );
            bool getHasWork() const;
            void setHasWork( const bool value );

            /* _________ MovementNumber minOccurs = 0, maxOccurs = 1 _________ */
            MovementNumberPtr getMovementNumber() const;
            void setMovementNumber( const MovementNumberPtr& value );
            bool getHasMovementNumber() const;
            void setHasMovementNumber( const bool value );

            /* _________ MovementTitle minOccurs = 0, maxOccurs = 1 _________ */
            MovementTitlePtr getMovementTitle() const;
            void setMovementTitle( const MovementTitlePtr& value );
            bool getHasMovementTitle() const;
            void setHasMovementTitle( const bool value );

            /* _________ Identification minOccurs = 0, maxOccurs = 1 _________ */
            IdentificationPtr getIdentification() const;
            void setIdentification( const IdentificationPtr& value );
            bool getHasIdentification() const;
            void setHasIdentification( const bool value );

            /* _________ Defaults minOccurs = 0, maxOccurs = 1 _________ */
            DefaultsPtr getDefaults() const;
            void setDefaults( const DefaultsPtr& value );
            bool getHasDefaults() const;
            void setHasDefaults( const bool value );

            /* _________ Credit minOccurs = 0, maxOccurs = unbounded _________ */
            const CreditSet& getCreditSet() const;
            void addCredit( const CreditPtr& value );
            void removeCredit( const CreditSetIterConst& value );
            void clearCreditSet();
            CreditPtr getCredit( const CreditSetIterConst& setIterator ) const;

            /* _________ PartList minOccurs = 1, maxOccurs = 1 _________ */
            PartListPtr getPartList() const;
            void setPartList( const PartListPtr& value );

        private:
            WorkPtr myWork;
            bool myHasWork;
            MovementNumberPtr myMovementNumber;
            bool myHasMovementNumber;
            MovementTitlePtr myMovementTitle;
            bool myHasMovementTitle;
            IdentificationPtr myIdentification;
            bool myHasIdentification;
            DefaultsPtr myDefaults;
            bool myHasDefaults;
            CreditSet myCreditSet;
            PartListPtr myPartList;
        };
        
        
        struct ScorePartwiseAttributes;
        using ScorePartwiseAttributesPtr = std::shared_ptr<ScorePartwiseAttributes>;
        
        struct ScorePartwiseAttributes : public AttributesInterface
        {
        public:
            ScorePartwiseAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken version;
            bool hasVersion;
        };
        
        class ScorePartwise;
        using ScorePartwisePtr = std::shared_ptr<ScorePartwise>;
        using ScorePartwiseUPtr = std::unique_ptr<ScorePartwise>;
        using ScorePartwiseSet = std::vector<ScorePartwisePtr>;
        using ScorePartwiseSetIter = ScorePartwiseSet::iterator;
        using ScorePartwiseSetIterConst = ScorePartwiseSet::const_iterator;
        inline ScorePartwisePtr makeScorePartwise() { return std::make_shared<ScorePartwise>(); }

        class ScorePartwise : public ElementInterface
        {
        public:
            ScorePartwise();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ScorePartwiseAttributesPtr getAttributes() const;
            void setAttributes( const ScorePartwiseAttributesPtr& value );

            /* _________ ScoreHeaderGroup minOccurs = 1, maxOccurs = 1 _________ */
            ScoreHeaderGroupPtr getScoreHeaderGroup() const;
            void setScoreHeaderGroup( const ScoreHeaderGroupPtr& value );

            /* _________ PartwisePart minOccurs = 1, maxOccurs = unbounded _________ */
            const PartwisePartSet& getPartwisePartSet() const;
            void addPartwisePart( const PartwisePartPtr& value );
            void removePartwisePart( const PartwisePartSetIterConst& value );
            void clearPartwisePartSet();
            PartwisePartPtr getPartwisePart( const PartwisePartSetIterConst& setIterator ) const;

        private:
            ScorePartwiseAttributesPtr myAttributes;
            ScoreHeaderGroupPtr myScoreHeaderGroup;
            PartwisePartSet myPartwisePartSet;
        };
        
        
        
        
        class TimewisePart;
        using TimewisePartPtr = std::shared_ptr<TimewisePart>;
        using TimewisePartUPtr = std::unique_ptr<TimewisePart>;
        using TimewisePartSet = std::vector<TimewisePartPtr>;
        using TimewisePartSetIter = TimewisePartSet::iterator;
        using TimewisePartSetIterConst = TimewisePartSet::const_iterator;
        inline TimewisePartPtr makeTimewisePart() { return std::make_shared<TimewisePart>(); }

        class TimewisePart : public ElementInterface
        {
        public:
            TimewisePart();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PartAttributesPtr getAttributes() const;
            void setAttributes( const PartAttributesPtr& value );

            /* _________ MusicDataGroup minOccurs = 1, maxOccurs = 1 _________ */
            MusicDataGroupPtr getMusicDataGroup() const;
            void setMusicDataGroup( const MusicDataGroupPtr& value );

        private:
            PartAttributesPtr myAttributes;
            MusicDataGroupPtr myMusicDataGroup;
        };
        
        struct MeasureAttributes;
        using MeasureAttributesPtr = std::shared_ptr<MeasureAttributes>;
        
        class TimewiseMeasure;
        using TimewiseMeasurePtr = std::shared_ptr<TimewiseMeasure>;
        using TimewiseMeasureUPtr = std::unique_ptr<TimewiseMeasure>;
        using TimewiseMeasureSet = std::vector<TimewiseMeasurePtr>;
        using TimewiseMeasureSetIter = TimewiseMeasureSet::iterator;
        using TimewiseMeasureSetIterConst = TimewiseMeasureSet::const_iterator;
        inline TimewiseMeasurePtr makeTimewiseMeasure() { return std::make_shared<TimewiseMeasure>(); }

        class TimewiseMeasure : public ElementInterface
        {
        public:
            TimewiseMeasure();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MeasureAttributesPtr getAttributes() const;
            void setAttributes( const MeasureAttributesPtr& value );

            /* _________ TimewisePart minOccurs = 1, maxOccurs = unbounded _________ */
            const TimewisePartSet& getTimewisePartSet() const;
            void addTimewisePart( const TimewisePartPtr& value );
            void removeTimewisePart( const TimewisePartSetIterConst& value );
            void clearTimewisePartSet();
            TimewisePartPtr getTimewisePart( const TimewisePartSetIterConst& setIterator ) const;

        private:
            MeasureAttributesPtr myAttributes;
            TimewisePartSet myTimewisePartSet;
        };
        
        
        struct ScoreTimewiseAttributes;
        using ScoreTimewiseAttributesPtr = std::shared_ptr<ScoreTimewiseAttributes>;
        
        struct ScoreTimewiseAttributes : public AttributesInterface
        {
        public:
            ScoreTimewiseAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken version;
            bool hasVersion;
        };
        
        class ScoreTimewise;
        using ScoreTimewisePtr = std::shared_ptr<ScoreTimewise>;
        using ScoreTimewiseUPtr = std::unique_ptr<ScoreTimewise>;
        using ScoreTimewiseSet = std::vector<ScoreTimewisePtr>;
        using ScoreTimewiseSetIter = ScoreTimewiseSet::iterator;
        using ScoreTimewiseSetIterConst = ScoreTimewiseSet::const_iterator;
        inline ScoreTimewisePtr makeScoreTimewise() { return std::make_shared<ScoreTimewise>(); }

        class ScoreTimewise : public ElementInterface
        {
        public:
            ScoreTimewise();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ScoreTimewiseAttributesPtr getAttributes() const;
            void setAttributes( const ScoreTimewiseAttributesPtr& value );

            /* _________ ScoreHeaderGroup minOccurs = 1, maxOccurs = 1 _________ */
            ScoreHeaderGroupPtr getScoreHeaderGroup() const;
            void setScoreHeaderGroup( const ScoreHeaderGroupPtr& value );

            /* _________ TimewiseMeasure minOccurs = 1, maxOccurs = unbounded _________ */
            const TimewiseMeasureSet& getTimewiseMeasureSet() const;
            void addTimewiseMeasure( const TimewiseMeasurePtr& value );
            void removeTimewiseMeasure( const TimewiseMeasureSetIterConst& value );
            void clearTimewiseMeasureSet();
            TimewiseMeasurePtr getTimewiseMeasure( const TimewiseMeasureSetIterConst& setIterator ) const;

        private:
            ScoreTimewiseAttributesPtr myAttributes;
            ScoreHeaderGroupPtr myScoreHeaderGroup;
            TimewiseMeasureSet myTimewiseMeasureSet;
        };
    }
}

#endif
