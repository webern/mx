// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// self
#include "mx/core/Enums.h"

// std
#include <sstream>

namespace mx
{
    namespace core
    {
        /// AboveBelow /////////////////////////////////////////////////////////////////////////////

        AboveBelow parseAboveBelow( const std::string& value )
        {
            const auto opt = tryParseAboveBelow( value );
            return opt.value_or( AboveBelow::above );
        }

        std::optional<AboveBelow> tryParseAboveBelow( const std::string& value )
        {
            if( value == "above" ) { return AboveBelow::above; }
            else if( value == "below" ) { return AboveBelow::below; }
            return std::optional<AboveBelow>{};
        }

        std::string toString( const AboveBelow value )
        {
            switch ( value )
            {
                case AboveBelow::above: { return "above"; }
                case AboveBelow::below: { return "below"; }
                default: break;
            }
            return "above";
        }

        std::ostream& toStream( std::ostream& os, const AboveBelow value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const AboveBelow value )
        {
            return toStream( os, value );
        }

        /// AccidentalValue ////////////////////////////////////////////////////////////////////////

        AccidentalValue parseAccidentalValue( const std::string& value )
        {
            const auto opt = tryParseAccidentalValue( value );
            return opt.value_or( AccidentalValue::sharp );
        }

        std::optional<AccidentalValue> tryParseAccidentalValue( const std::string& value )
        {
            if( value == "sharp" ) { return AccidentalValue::sharp; }
            else if( value == "natural" ) { return AccidentalValue::natural; }
            else if( value == "flat" ) { return AccidentalValue::flat; }
            else if( value == "double-sharp" ) { return AccidentalValue::doubleSharp; }
            else if( value == "sharp-sharp" ) { return AccidentalValue::sharpSharp; }
            else if( value == "flat-flat" ) { return AccidentalValue::flatFlat; }
            else if( value == "natural-sharp" ) { return AccidentalValue::naturalSharp; }
            else if( value == "natural-flat" ) { return AccidentalValue::naturalFlat; }
            else if( value == "quarter-flat" ) { return AccidentalValue::quarterFlat; }
            else if( value == "quarter-sharp" ) { return AccidentalValue::quarterSharp; }
            else if( value == "three-quarters-flat" ) { return AccidentalValue::threeQuartersFlat; }
            else if( value == "three-quarters-sharp" ) { return AccidentalValue::threeQuartersSharp; }
            else if( value == "sharp-down" ) { return AccidentalValue::sharpDown; }
            else if( value == "sharp-up" ) { return AccidentalValue::sharpUp; }
            else if( value == "natural-down" ) { return AccidentalValue::naturalDown; }
            else if( value == "natural-up" ) { return AccidentalValue::naturalUp; }
            else if( value == "flat-down" ) { return AccidentalValue::flatDown; }
            else if( value == "flat-up" ) { return AccidentalValue::flatUp; }
            else if( value == "triple-sharp" ) { return AccidentalValue::tripleSharp; }
            else if( value == "triple-flat" ) { return AccidentalValue::tripleFlat; }
            else if( value == "slash-quarter-sharp" ) { return AccidentalValue::slashQuarterSharp; }
            else if( value == "slash-sharp" ) { return AccidentalValue::slashSharp; }
            else if( value == "slash-flat" ) { return AccidentalValue::slashFlat; }
            else if( value == "double-slash-flat" ) { return AccidentalValue::doubleSlashFlat; }
            else if( value == "sharp-1" ) { return AccidentalValue::sharp1; }
            else if( value == "sharp-2" ) { return AccidentalValue::sharp2; }
            else if( value == "sharp-3" ) { return AccidentalValue::sharp3; }
            else if( value == "sharp-5" ) { return AccidentalValue::sharp5; }
            else if( value == "flat-1" ) { return AccidentalValue::flat1; }
            else if( value == "flat-2" ) { return AccidentalValue::flat2; }
            else if( value == "flat-3" ) { return AccidentalValue::flat3; }
            else if( value == "flat-4" ) { return AccidentalValue::flat4; }
            else if( value == "sori" ) { return AccidentalValue::sori; }
            else if( value == "koron" ) { return AccidentalValue::koron; }
            return std::optional<AccidentalValue>{};
        }

        std::string toString( const AccidentalValue value )
        {
            switch ( value )
            {
                case AccidentalValue::sharp: { return "sharp"; }
                case AccidentalValue::natural: { return "natural"; }
                case AccidentalValue::flat: { return "flat"; }
                case AccidentalValue::doubleSharp: { return "double-sharp"; }
                case AccidentalValue::sharpSharp: { return "sharp-sharp"; }
                case AccidentalValue::flatFlat: { return "flat-flat"; }
                case AccidentalValue::naturalSharp: { return "natural-sharp"; }
                case AccidentalValue::naturalFlat: { return "natural-flat"; }
                case AccidentalValue::quarterFlat: { return "quarter-flat"; }
                case AccidentalValue::quarterSharp: { return "quarter-sharp"; }
                case AccidentalValue::threeQuartersFlat: { return "three-quarters-flat"; }
                case AccidentalValue::threeQuartersSharp: { return "three-quarters-sharp"; }
                case AccidentalValue::sharpDown: { return "sharp-down"; }
                case AccidentalValue::sharpUp: { return "sharp-up"; }
                case AccidentalValue::naturalDown: { return "natural-down"; }
                case AccidentalValue::naturalUp: { return "natural-up"; }
                case AccidentalValue::flatDown: { return "flat-down"; }
                case AccidentalValue::flatUp: { return "flat-up"; }
                case AccidentalValue::tripleSharp: { return "triple-sharp"; }
                case AccidentalValue::tripleFlat: { return "triple-flat"; }
                case AccidentalValue::slashQuarterSharp: { return "slash-quarter-sharp"; }
                case AccidentalValue::slashSharp: { return "slash-sharp"; }
                case AccidentalValue::slashFlat: { return "slash-flat"; }
                case AccidentalValue::doubleSlashFlat: { return "double-slash-flat"; }
                case AccidentalValue::sharp1: { return "sharp-1"; }
                case AccidentalValue::sharp2: { return "sharp-2"; }
                case AccidentalValue::sharp3: { return "sharp-3"; }
                case AccidentalValue::sharp5: { return "sharp-5"; }
                case AccidentalValue::flat1: { return "flat-1"; }
                case AccidentalValue::flat2: { return "flat-2"; }
                case AccidentalValue::flat3: { return "flat-3"; }
                case AccidentalValue::flat4: { return "flat-4"; }
                case AccidentalValue::sori: { return "sori"; }
                case AccidentalValue::koron: { return "koron"; }
                default: break;
            }
            return "sharp";
        }

        std::ostream& toStream( std::ostream& os, const AccidentalValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const AccidentalValue value )
        {
            return toStream( os, value );
        }

        /// ArrowDirectionEnum /////////////////////////////////////////////////////////////////////

        ArrowDirectionEnum parseArrowDirectionEnum( const std::string& value )
        {
            const auto opt = tryParseArrowDirectionEnum( value );
            return opt.value_or( ArrowDirectionEnum::left );
        }

        std::optional<ArrowDirectionEnum> tryParseArrowDirectionEnum( const std::string& value )
        {
            if( value == "left" ) { return ArrowDirectionEnum::left; }
            else if( value == "up" ) { return ArrowDirectionEnum::up; }
            else if( value == "right" ) { return ArrowDirectionEnum::right; }
            else if( value == "down" ) { return ArrowDirectionEnum::down; }
            else if( value == "northwest" ) { return ArrowDirectionEnum::northwest; }
            else if( value == "northeast" ) { return ArrowDirectionEnum::northeast; }
            else if( value == "southeast" ) { return ArrowDirectionEnum::southeast; }
            else if( value == "southwest" ) { return ArrowDirectionEnum::southwest; }
            else if( value == "left right" ) { return ArrowDirectionEnum::leftRight; }
            else if( value == "up down" ) { return ArrowDirectionEnum::upDown; }
            else if( value == "northwest southeast" ) { return ArrowDirectionEnum::northwestSoutheast; }
            else if( value == "northeast southwest" ) { return ArrowDirectionEnum::northeastSouthwest; }
            else if( value == "other" ) { return ArrowDirectionEnum::other; }
            return std::optional<ArrowDirectionEnum>{};
        }

        std::string toString( const ArrowDirectionEnum value )
        {
            switch ( value )
            {
                case ArrowDirectionEnum::left: { return "left"; }
                case ArrowDirectionEnum::up: { return "up"; }
                case ArrowDirectionEnum::right: { return "right"; }
                case ArrowDirectionEnum::down: { return "down"; }
                case ArrowDirectionEnum::northwest: { return "northwest"; }
                case ArrowDirectionEnum::northeast: { return "northeast"; }
                case ArrowDirectionEnum::southeast: { return "southeast"; }
                case ArrowDirectionEnum::southwest: { return "southwest"; }
                case ArrowDirectionEnum::leftRight: { return "left right"; }
                case ArrowDirectionEnum::upDown: { return "up down"; }
                case ArrowDirectionEnum::northwestSoutheast: { return "northwest southeast"; }
                case ArrowDirectionEnum::northeastSouthwest: { return "northeast southwest"; }
                case ArrowDirectionEnum::other: { return "other"; }
                default: break;
            }
            return "left";
        }

        std::ostream& toStream( std::ostream& os, const ArrowDirectionEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const ArrowDirectionEnum value )
        {
            return toStream( os, value );
        }

        /// ArrowStyleEnum /////////////////////////////////////////////////////////////////////////

        ArrowStyleEnum parseArrowStyleEnum( const std::string& value )
        {
            const auto opt = tryParseArrowStyleEnum( value );
            return opt.value_or( ArrowStyleEnum::single );
        }

        std::optional<ArrowStyleEnum> tryParseArrowStyleEnum( const std::string& value )
        {
            if( value == "single" ) { return ArrowStyleEnum::single; }
            else if( value == "double" ) { return ArrowStyleEnum::double_; }
            else if( value == "filled" ) { return ArrowStyleEnum::filled; }
            else if( value == "hollow" ) { return ArrowStyleEnum::hollow; }
            else if( value == "paired" ) { return ArrowStyleEnum::paired; }
            else if( value == "combined" ) { return ArrowStyleEnum::combined; }
            else if( value == "other" ) { return ArrowStyleEnum::other; }
            return std::optional<ArrowStyleEnum>{};
        }

        std::string toString( const ArrowStyleEnum value )
        {
            switch ( value )
            {
                case ArrowStyleEnum::single: { return "single"; }
                case ArrowStyleEnum::double_: { return "double"; }
                case ArrowStyleEnum::filled: { return "filled"; }
                case ArrowStyleEnum::hollow: { return "hollow"; }
                case ArrowStyleEnum::paired: { return "paired"; }
                case ArrowStyleEnum::combined: { return "combined"; }
                case ArrowStyleEnum::other: { return "other"; }
                default: break;
            }
            return "single";
        }

        std::ostream& toStream( std::ostream& os, const ArrowStyleEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const ArrowStyleEnum value )
        {
            return toStream( os, value );
        }

        /// BackwardForward ////////////////////////////////////////////////////////////////////////

        BackwardForward parseBackwardForward( const std::string& value )
        {
            const auto opt = tryParseBackwardForward( value );
            return opt.value_or( BackwardForward::backward );
        }

        std::optional<BackwardForward> tryParseBackwardForward( const std::string& value )
        {
            if( value == "backward" ) { return BackwardForward::backward; }
            else if( value == "forward" ) { return BackwardForward::forward; }
            return std::optional<BackwardForward>{};
        }

        std::string toString( const BackwardForward value )
        {
            switch ( value )
            {
                case BackwardForward::backward: { return "backward"; }
                case BackwardForward::forward: { return "forward"; }
                default: break;
            }
            return "backward";
        }

        std::ostream& toStream( std::ostream& os, const BackwardForward value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const BackwardForward value )
        {
            return toStream( os, value );
        }

        /// BarStyleEnum ///////////////////////////////////////////////////////////////////////////

        BarStyleEnum parseBarStyleEnum( const std::string& value )
        {
            const auto opt = tryParseBarStyleEnum( value );
            return opt.value_or( BarStyleEnum::regular );
        }

        std::optional<BarStyleEnum> tryParseBarStyleEnum( const std::string& value )
        {
            if( value == "regular" ) { return BarStyleEnum::regular; }
            else if( value == "dotted" ) { return BarStyleEnum::dotted; }
            else if( value == "dashed" ) { return BarStyleEnum::dashed; }
            else if( value == "heavy" ) { return BarStyleEnum::heavy; }
            else if( value == "light-light" ) { return BarStyleEnum::lightLight; }
            else if( value == "light-heavy" ) { return BarStyleEnum::lightHeavy; }
            else if( value == "heavy-light" ) { return BarStyleEnum::heavyLight; }
            else if( value == "heavy-heavy" ) { return BarStyleEnum::heavyHeavy; }
            else if( value == "tick" ) { return BarStyleEnum::tick; }
            else if( value == "short" ) { return BarStyleEnum::short_; }
            else if( value == "none" ) { return BarStyleEnum::none; }
            return std::optional<BarStyleEnum>{};
        }

        std::string toString( const BarStyleEnum value )
        {
            switch ( value )
            {
                case BarStyleEnum::regular: { return "regular"; }
                case BarStyleEnum::dotted: { return "dotted"; }
                case BarStyleEnum::dashed: { return "dashed"; }
                case BarStyleEnum::heavy: { return "heavy"; }
                case BarStyleEnum::lightLight: { return "light-light"; }
                case BarStyleEnum::lightHeavy: { return "light-heavy"; }
                case BarStyleEnum::heavyLight: { return "heavy-light"; }
                case BarStyleEnum::heavyHeavy: { return "heavy-heavy"; }
                case BarStyleEnum::tick: { return "tick"; }
                case BarStyleEnum::short_: { return "short"; }
                case BarStyleEnum::none: { return "none"; }
                default: break;
            }
            return "regular";
        }

        std::ostream& toStream( std::ostream& os, const BarStyleEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const BarStyleEnum value )
        {
            return toStream( os, value );
        }

        /// BeamValue //////////////////////////////////////////////////////////////////////////////

        BeamValue parseBeamValue( const std::string& value )
        {
            const auto opt = tryParseBeamValue( value );
            return opt.value_or( BeamValue::begin );
        }

        std::optional<BeamValue> tryParseBeamValue( const std::string& value )
        {
            if( value == "begin" ) { return BeamValue::begin; }
            else if( value == "continue" ) { return BeamValue::continue_; }
            else if( value == "end" ) { return BeamValue::end; }
            else if( value == "forward hook" ) { return BeamValue::forwardHook; }
            else if( value == "backward hook" ) { return BeamValue::backwardHook; }
            return std::optional<BeamValue>{};
        }

        std::string toString( const BeamValue value )
        {
            switch ( value )
            {
                case BeamValue::begin: { return "begin"; }
                case BeamValue::continue_: { return "continue"; }
                case BeamValue::end: { return "end"; }
                case BeamValue::forwardHook: { return "forward hook"; }
                case BeamValue::backwardHook: { return "backward hook"; }
                default: break;
            }
            return "begin";
        }

        std::ostream& toStream( std::ostream& os, const BeamValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const BeamValue value )
        {
            return toStream( os, value );
        }

        /// BeaterValue ////////////////////////////////////////////////////////////////////////////

        BeaterValue parseBeaterValue( const std::string& value )
        {
            const auto opt = tryParseBeaterValue( value );
            return opt.value_or( BeaterValue::bow );
        }

        std::optional<BeaterValue> tryParseBeaterValue( const std::string& value )
        {
            if( value == "bow" ) { return BeaterValue::bow; }
            else if( value == "chime hammer" ) { return BeaterValue::chimeHammer; }
            else if( value == "coin" ) { return BeaterValue::coin; }
            else if( value == "finger" ) { return BeaterValue::finger; }
            else if( value == "fingernail" ) { return BeaterValue::fingernail; }
            else if( value == "fist" ) { return BeaterValue::fist; }
            else if( value == "guiro scraper" ) { return BeaterValue::guiroScraper; }
            else if( value == "hammer" ) { return BeaterValue::hammer; }
            else if( value == "hand" ) { return BeaterValue::hand; }
            else if( value == "jazz stick" ) { return BeaterValue::jazzStick; }
            else if( value == "knitting needle" ) { return BeaterValue::knittingNeedle; }
            else if( value == "metal hammer" ) { return BeaterValue::metalHammer; }
            else if( value == "snare stick" ) { return BeaterValue::snareStick; }
            else if( value == "spoon mallet" ) { return BeaterValue::spoonMallet; }
            else if( value == "triangle beater" ) { return BeaterValue::triangleBeater; }
            else if( value == "triangle beater plain" ) { return BeaterValue::triangleBeaterPlain; }
            else if( value == "wire brush" ) { return BeaterValue::wireBrush; }
            return std::optional<BeaterValue>{};
        }

        std::string toString( const BeaterValue value )
        {
            switch ( value )
            {
                case BeaterValue::bow: { return "bow"; }
                case BeaterValue::chimeHammer: { return "chime hammer"; }
                case BeaterValue::coin: { return "coin"; }
                case BeaterValue::finger: { return "finger"; }
                case BeaterValue::fingernail: { return "fingernail"; }
                case BeaterValue::fist: { return "fist"; }
                case BeaterValue::guiroScraper: { return "guiro scraper"; }
                case BeaterValue::hammer: { return "hammer"; }
                case BeaterValue::hand: { return "hand"; }
                case BeaterValue::jazzStick: { return "jazz stick"; }
                case BeaterValue::knittingNeedle: { return "knitting needle"; }
                case BeaterValue::metalHammer: { return "metal hammer"; }
                case BeaterValue::snareStick: { return "snare stick"; }
                case BeaterValue::spoonMallet: { return "spoon mallet"; }
                case BeaterValue::triangleBeater: { return "triangle beater"; }
                case BeaterValue::triangleBeaterPlain: { return "triangle beater plain"; }
                case BeaterValue::wireBrush: { return "wire brush"; }
                default: break;
            }
            return "bow";
        }

        std::ostream& toStream( std::ostream& os, const BeaterValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const BeaterValue value )
        {
            return toStream( os, value );
        }

        /// BreathMarkValue ////////////////////////////////////////////////////////////////////////

        BreathMarkValue parseBreathMarkValue( const std::string& value )
        {
            const auto opt = tryParseBreathMarkValue( value );
            return opt.value_or( BreathMarkValue::emptystring );
        }

        std::optional<BreathMarkValue> tryParseBreathMarkValue( const std::string& value )
        {
            if( value == "" ) { return BreathMarkValue::emptystring; }
            else if( value == "comma" ) { return BreathMarkValue::comma; }
            else if( value == "tick" ) { return BreathMarkValue::tick; }
            return std::optional<BreathMarkValue>{};
        }

        std::string toString( const BreathMarkValue value )
        {
            switch ( value )
            {
                case BreathMarkValue::emptystring: { return ""; }
                case BreathMarkValue::comma: { return "comma"; }
                case BreathMarkValue::tick: { return "tick"; }
                default: break;
            }
            return "";
        }

        std::ostream& toStream( std::ostream& os, const BreathMarkValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const BreathMarkValue value )
        {
            return toStream( os, value );
        }

        /// CancelLocation /////////////////////////////////////////////////////////////////////////

        CancelLocation parseCancelLocation( const std::string& value )
        {
            const auto opt = tryParseCancelLocation( value );
            return opt.value_or( CancelLocation::left );
        }

        std::optional<CancelLocation> tryParseCancelLocation( const std::string& value )
        {
            if( value == "left" ) { return CancelLocation::left; }
            else if( value == "right" ) { return CancelLocation::right; }
            else if( value == "before-barline" ) { return CancelLocation::beforeBarline; }
            return std::optional<CancelLocation>{};
        }

        std::string toString( const CancelLocation value )
        {
            switch ( value )
            {
                case CancelLocation::left: { return "left"; }
                case CancelLocation::right: { return "right"; }
                case CancelLocation::beforeBarline: { return "before-barline"; }
                default: break;
            }
            return "left";
        }

        std::ostream& toStream( std::ostream& os, const CancelLocation value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const CancelLocation value )
        {
            return toStream( os, value );
        }

        /// CircularArrowEnum //////////////////////////////////////////////////////////////////////

        CircularArrowEnum parseCircularArrowEnum( const std::string& value )
        {
            const auto opt = tryParseCircularArrowEnum( value );
            return opt.value_or( CircularArrowEnum::clockwise );
        }

        std::optional<CircularArrowEnum> tryParseCircularArrowEnum( const std::string& value )
        {
            if( value == "clockwise" ) { return CircularArrowEnum::clockwise; }
            else if( value == "anticlockwise" ) { return CircularArrowEnum::anticlockwise; }
            return std::optional<CircularArrowEnum>{};
        }

        std::string toString( const CircularArrowEnum value )
        {
            switch ( value )
            {
                case CircularArrowEnum::clockwise: { return "clockwise"; }
                case CircularArrowEnum::anticlockwise: { return "anticlockwise"; }
                default: break;
            }
            return "clockwise";
        }

        std::ostream& toStream( std::ostream& os, const CircularArrowEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const CircularArrowEnum value )
        {
            return toStream( os, value );
        }

        /// ClefSign ///////////////////////////////////////////////////////////////////////////////

        ClefSign parseClefSign( const std::string& value )
        {
            const auto opt = tryParseClefSign( value );
            return opt.value_or( ClefSign::g );
        }

        std::optional<ClefSign> tryParseClefSign( const std::string& value )
        {
            if( value == "G" ) { return ClefSign::g; }
            else if( value == "F" ) { return ClefSign::f; }
            else if( value == "C" ) { return ClefSign::c; }
            else if( value == "percussion" ) { return ClefSign::percussion; }
            else if( value == "TAB" ) { return ClefSign::tab; }
            else if( value == "jianpu" ) { return ClefSign::jianpu; }
            else if( value == "none" ) { return ClefSign::none; }
            return std::optional<ClefSign>{};
        }

        std::string toString( const ClefSign value )
        {
            switch ( value )
            {
                case ClefSign::g: { return "G"; }
                case ClefSign::f: { return "F"; }
                case ClefSign::c: { return "C"; }
                case ClefSign::percussion: { return "percussion"; }
                case ClefSign::tab: { return "TAB"; }
                case ClefSign::jianpu: { return "jianpu"; }
                case ClefSign::none: { return "none"; }
                default: break;
            }
            return "G";
        }

        std::ostream& toStream( std::ostream& os, const ClefSign value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const ClefSign value )
        {
            return toStream( os, value );
        }

        /// CssFontSize ////////////////////////////////////////////////////////////////////////////

        CssFontSize parseCssFontSize( const std::string& value )
        {
            const auto opt = tryParseCssFontSize( value );
            return opt.value_or( CssFontSize::xxSmall );
        }

        std::optional<CssFontSize> tryParseCssFontSize( const std::string& value )
        {
            if( value == "xx-small" ) { return CssFontSize::xxSmall; }
            else if( value == "x-small" ) { return CssFontSize::xSmall; }
            else if( value == "small" ) { return CssFontSize::small; }
            else if( value == "medium" ) { return CssFontSize::medium; }
            else if( value == "large" ) { return CssFontSize::large; }
            else if( value == "x-large" ) { return CssFontSize::xLarge; }
            else if( value == "xx-large" ) { return CssFontSize::xxLarge; }
            return std::optional<CssFontSize>{};
        }

        std::string toString( const CssFontSize value )
        {
            switch ( value )
            {
                case CssFontSize::xxSmall: { return "xx-small"; }
                case CssFontSize::xSmall: { return "x-small"; }
                case CssFontSize::small: { return "small"; }
                case CssFontSize::medium: { return "medium"; }
                case CssFontSize::large: { return "large"; }
                case CssFontSize::xLarge: { return "x-large"; }
                case CssFontSize::xxLarge: { return "xx-large"; }
                default: break;
            }
            return "xx-small";
        }

        std::ostream& toStream( std::ostream& os, const CssFontSize value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const CssFontSize value )
        {
            return toStream( os, value );
        }

        /// DegreeSymbolValue //////////////////////////////////////////////////////////////////////

        DegreeSymbolValue parseDegreeSymbolValue( const std::string& value )
        {
            const auto opt = tryParseDegreeSymbolValue( value );
            return opt.value_or( DegreeSymbolValue::major );
        }

        std::optional<DegreeSymbolValue> tryParseDegreeSymbolValue( const std::string& value )
        {
            if( value == "major" ) { return DegreeSymbolValue::major; }
            else if( value == "minor" ) { return DegreeSymbolValue::minor; }
            else if( value == "augmented" ) { return DegreeSymbolValue::augmented; }
            else if( value == "diminished" ) { return DegreeSymbolValue::diminished; }
            else if( value == "half-diminished" ) { return DegreeSymbolValue::halfDiminished; }
            return std::optional<DegreeSymbolValue>{};
        }

        std::string toString( const DegreeSymbolValue value )
        {
            switch ( value )
            {
                case DegreeSymbolValue::major: { return "major"; }
                case DegreeSymbolValue::minor: { return "minor"; }
                case DegreeSymbolValue::augmented: { return "augmented"; }
                case DegreeSymbolValue::diminished: { return "diminished"; }
                case DegreeSymbolValue::halfDiminished: { return "half-diminished"; }
                default: break;
            }
            return "major";
        }

        std::ostream& toStream( std::ostream& os, const DegreeSymbolValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const DegreeSymbolValue value )
        {
            return toStream( os, value );
        }

        /// DegreeTypeValue ////////////////////////////////////////////////////////////////////////

        DegreeTypeValue parseDegreeTypeValue( const std::string& value )
        {
            const auto opt = tryParseDegreeTypeValue( value );
            return opt.value_or( DegreeTypeValue::add );
        }

        std::optional<DegreeTypeValue> tryParseDegreeTypeValue( const std::string& value )
        {
            if( value == "add" ) { return DegreeTypeValue::add; }
            else if( value == "alter" ) { return DegreeTypeValue::alter; }
            else if( value == "subtract" ) { return DegreeTypeValue::subtract; }
            return std::optional<DegreeTypeValue>{};
        }

        std::string toString( const DegreeTypeValue value )
        {
            switch ( value )
            {
                case DegreeTypeValue::add: { return "add"; }
                case DegreeTypeValue::alter: { return "alter"; }
                case DegreeTypeValue::subtract: { return "subtract"; }
                default: break;
            }
            return "add";
        }

        std::ostream& toStream( std::ostream& os, const DegreeTypeValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const DegreeTypeValue value )
        {
            return toStream( os, value );
        }

        /// DistanceTypeEnum ///////////////////////////////////////////////////////////////////////

        DistanceTypeEnum parseDistanceTypeEnum( const std::string& value )
        {
            const auto opt = tryParseDistanceTypeEnum( value );
            return opt.value_or( DistanceTypeEnum::beam );
        }

        std::optional<DistanceTypeEnum> tryParseDistanceTypeEnum( const std::string& value )
        {
            if( value == "beam" ) { return DistanceTypeEnum::beam; }
            else if( value == "hyphen" ) { return DistanceTypeEnum::hyphen; }
            else if( value == "other" ) { return DistanceTypeEnum::other; }
            return std::optional<DistanceTypeEnum>{};
        }

        std::string toString( const DistanceTypeEnum value )
        {
            switch ( value )
            {
                case DistanceTypeEnum::beam: { return "beam"; }
                case DistanceTypeEnum::hyphen: { return "hyphen"; }
                default: break;
            }
            return "other";
        }

        std::ostream& toStream( std::ostream& os, const DistanceTypeEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const DistanceTypeEnum value )
        {
            return toStream( os, value );
        }

        DistanceType::DistanceType( const DistanceTypeEnum value )
        : myEnum( value )
        , myCustomValue( "" )
        {
            setValue( value );
        }

        DistanceType::DistanceType( const std::string& value )
        : myEnum( DistanceTypeEnum::other )
        , myCustomValue( value )
        {
            setValue( value );
        }

        DistanceType::DistanceType()
        : myEnum( DistanceTypeEnum::beam )
        , myCustomValue( "" )
        {
            setValue( DistanceTypeEnum::beam );
        }

        DistanceTypeEnum DistanceType::getValue() const
        {
            return myEnum;
        }

        std::string DistanceType::getValueString() const
        {
            if ( myEnum != DistanceTypeEnum::other )
            {
                return toString( myEnum );
            }

            else
            {
                return myCustomValue;
            }
        }

        void DistanceType::setValue( const DistanceTypeEnum value )
        {
            myEnum = value;
            if( value != DistanceTypeEnum::other && !myCustomValue.empty() )
            {
                myCustomValue = "";
            }
        }

        void DistanceType::setValue( const std::string& value )
        {
            const auto temp = tryParseDistanceTypeEnum( value );
            if ( temp )
            {
                myEnum = temp.value();
                myCustomValue = "";
            }
            else
            {
                setValue( DistanceTypeEnum::other );
                myCustomValue = value;
            }
        }

        DistanceType parseDistanceType( const std::string& value )
        {
            return DistanceType( value );
        }

        std::string toString( const DistanceType& value )
        {
            return value.getValueString();
        }

        std::ostream& toStream( std::ostream& os, const DistanceType& value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const DistanceType& value )
        {
            return toStream( os, value );
        }

        /// DynamicsEnum ///////////////////////////////////////////////////////////////////////////

        DynamicsEnum parseDynamicsEnum( const std::string& value )
        {
            const auto opt = tryParseDynamicsEnum( value );
            return opt.value_or( DynamicsEnum::mf );
        }

        std::optional<DynamicsEnum> tryParseDynamicsEnum( const std::string& value )
        {
            if( value == "p" ) { return DynamicsEnum::p; }
            else if( value == "pp" ) { return DynamicsEnum::pp; }
            else if( value == "ppp" ) { return DynamicsEnum::ppp; }
            else if( value == "pppp" ) { return DynamicsEnum::pppp; }
            else if( value == "ppppp" ) { return DynamicsEnum::ppppp; }
            else if( value == "pppppp" ) { return DynamicsEnum::pppppp; }
            else if( value == "f" ) { return DynamicsEnum::f; }
            else if( value == "ff" ) { return DynamicsEnum::ff; }
            else if( value == "fff" ) { return DynamicsEnum::fff; }
            else if( value == "ffff" ) { return DynamicsEnum::ffff; }
            else if( value == "fffff" ) { return DynamicsEnum::fffff; }
            else if( value == "ffffff" ) { return DynamicsEnum::ffffff; }
            else if( value == "mp" ) { return DynamicsEnum::mp; }
            else if( value == "mf" ) { return DynamicsEnum::mf; }
            else if( value == "sf" ) { return DynamicsEnum::sf; }
            else if( value == "sfp" ) { return DynamicsEnum::sfp; }
            else if( value == "sfpp" ) { return DynamicsEnum::sfpp; }
            else if( value == "fp" ) { return DynamicsEnum::fp; }
            else if( value == "rf" ) { return DynamicsEnum::rf; }
            else if( value == "rfz" ) { return DynamicsEnum::rfz; }
            else if( value == "sfz" ) { return DynamicsEnum::sfz; }
            else if( value == "sffz" ) { return DynamicsEnum::sffz; }
            else if( value == "fz" ) { return DynamicsEnum::fz; }
            else if( value == "other-dynamics" ) { return DynamicsEnum::otherDynamics; }
            return std::optional<DynamicsEnum>{};
        }

        std::string toString( const DynamicsEnum value )
        {
            switch ( value )
            {
                case DynamicsEnum::p: { return "p"; }
                case DynamicsEnum::pp: { return "pp"; }
                case DynamicsEnum::ppp: { return "ppp"; }
                case DynamicsEnum::pppp: { return "pppp"; }
                case DynamicsEnum::ppppp: { return "ppppp"; }
                case DynamicsEnum::pppppp: { return "pppppp"; }
                case DynamicsEnum::f: { return "f"; }
                case DynamicsEnum::ff: { return "ff"; }
                case DynamicsEnum::fff: { return "fff"; }
                case DynamicsEnum::ffff: { return "ffff"; }
                case DynamicsEnum::fffff: { return "fffff"; }
                case DynamicsEnum::ffffff: { return "ffffff"; }
                case DynamicsEnum::mp: { return "mp"; }
                case DynamicsEnum::mf: { return "mf"; }
                case DynamicsEnum::sf: { return "sf"; }
                case DynamicsEnum::sfp: { return "sfp"; }
                case DynamicsEnum::sfpp: { return "sfpp"; }
                case DynamicsEnum::fp: { return "fp"; }
                case DynamicsEnum::rf: { return "rf"; }
                case DynamicsEnum::rfz: { return "rfz"; }
                case DynamicsEnum::sfz: { return "sfz"; }
                case DynamicsEnum::sffz: { return "sffz"; }
                case DynamicsEnum::fz: { return "fz"; }
                default: break;
            }
            return "other-dynamics";
        }

        std::ostream& toStream( std::ostream& os, const DynamicsEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const DynamicsEnum value )
        {
            return toStream( os, value );
        }

        DynamicsValue::DynamicsValue( const DynamicsEnum value )
        : myEnum( value )
        , myCustomValue( "" )
        {
            setValue( value );
        }

        DynamicsValue::DynamicsValue( const std::string& value )
        : myEnum( DynamicsEnum::otherDynamics )
        , myCustomValue( value )
        {
            setValue( value );
        }

        DynamicsValue::DynamicsValue()
        : myEnum( DynamicsEnum::mf )
        , myCustomValue( "" )
        {
            setValue( DynamicsEnum::mf );
        }

        DynamicsEnum DynamicsValue::getValue() const
        {
            return myEnum;
        }

        std::string DynamicsValue::getValueString() const
        {
            if ( myEnum != DynamicsEnum::otherDynamics )
            {
                return toString( myEnum );
            }

            else
            {
                return myCustomValue;
            }
        }

        void DynamicsValue::setValue( const DynamicsEnum value )
        {
            myEnum = value;
            if( value != DynamicsEnum::otherDynamics && !myCustomValue.empty() )
            {
                myCustomValue = "";
            }
        }

        void DynamicsValue::setValue( const std::string& value )
        {
            const auto temp = tryParseDynamicsEnum( value );
            if ( temp )
            {
                myEnum = temp.value();
                myCustomValue = "";
            }
            else
            {
                setValue( DynamicsEnum::otherDynamics );
                myCustomValue = value;
            }
        }

        DynamicsValue parseDynamicsValue( const std::string& value )
        {
            return DynamicsValue( value );
        }

        std::string toString( const DynamicsValue& value )
        {
            return value.getValueString();
        }

        std::ostream& toStream( std::ostream& os, const DynamicsValue& value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const DynamicsValue& value )
        {
            return toStream( os, value );
        }

        /// EffectEnum /////////////////////////////////////////////////////////////////////////////

        EffectEnum parseEffectEnum( const std::string& value )
        {
            const auto opt = tryParseEffectEnum( value );
            return opt.value_or( EffectEnum::anvil );
        }

        std::optional<EffectEnum> tryParseEffectEnum( const std::string& value )
        {
            if( value == "anvil" ) { return EffectEnum::anvil; }
            else if( value == "auto horn" ) { return EffectEnum::autoHorn; }
            else if( value == "bird whistle" ) { return EffectEnum::birdWhistle; }
            else if( value == "cannon" ) { return EffectEnum::cannon; }
            else if( value == "duck call" ) { return EffectEnum::duckCall; }
            else if( value == "gun shot" ) { return EffectEnum::gunShot; }
            else if( value == "klaxon horn" ) { return EffectEnum::klaxonHorn; }
            else if( value == "lions roar" ) { return EffectEnum::lionsRoar; }
            else if( value == "police whistle" ) { return EffectEnum::policeWhistle; }
            else if( value == "siren" ) { return EffectEnum::siren; }
            else if( value == "slide whistle" ) { return EffectEnum::slideWhistle; }
            else if( value == "thunder sheet" ) { return EffectEnum::thunderSheet; }
            else if( value == "wind machine" ) { return EffectEnum::windMachine; }
            else if( value == "wind whistle" ) { return EffectEnum::windWhistle; }
            return std::optional<EffectEnum>{};
        }

        std::string toString( const EffectEnum value )
        {
            switch ( value )
            {
                case EffectEnum::anvil: { return "anvil"; }
                case EffectEnum::autoHorn: { return "auto horn"; }
                case EffectEnum::birdWhistle: { return "bird whistle"; }
                case EffectEnum::cannon: { return "cannon"; }
                case EffectEnum::duckCall: { return "duck call"; }
                case EffectEnum::gunShot: { return "gun shot"; }
                case EffectEnum::klaxonHorn: { return "klaxon horn"; }
                case EffectEnum::lionsRoar: { return "lions roar"; }
                case EffectEnum::policeWhistle: { return "police whistle"; }
                case EffectEnum::siren: { return "siren"; }
                case EffectEnum::slideWhistle: { return "slide whistle"; }
                case EffectEnum::thunderSheet: { return "thunder sheet"; }
                case EffectEnum::windMachine: { return "wind machine"; }
                case EffectEnum::windWhistle: { return "wind whistle"; }
                default: break;
            }
            return "anvil";
        }

        std::ostream& toStream( std::ostream& os, const EffectEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const EffectEnum value )
        {
            return toStream( os, value );
        }

        /// EnclosureShape /////////////////////////////////////////////////////////////////////////

        EnclosureShape parseEnclosureShape( const std::string& value )
        {
            const auto opt = tryParseEnclosureShape( value );
            return opt.value_or( EnclosureShape::rectangle );
        }

        std::optional<EnclosureShape> tryParseEnclosureShape( const std::string& value )
        {
            if( value == "rectangle" ) { return EnclosureShape::rectangle; }
            else if( value == "square" ) { return EnclosureShape::square; }
            else if( value == "oval" ) { return EnclosureShape::oval; }
            else if( value == "circle" ) { return EnclosureShape::circle; }
            else if( value == "bracket" ) { return EnclosureShape::bracket; }
            else if( value == "triangle" ) { return EnclosureShape::triangle; }
            else if( value == "diamond" ) { return EnclosureShape::diamond; }
            else if( value == "none" ) { return EnclosureShape::none; }
            return std::optional<EnclosureShape>{};
        }

        std::string toString( const EnclosureShape value )
        {
            switch ( value )
            {
                case EnclosureShape::rectangle: { return "rectangle"; }
                case EnclosureShape::square: { return "square"; }
                case EnclosureShape::oval: { return "oval"; }
                case EnclosureShape::circle: { return "circle"; }
                case EnclosureShape::bracket: { return "bracket"; }
                case EnclosureShape::triangle: { return "triangle"; }
                case EnclosureShape::diamond: { return "diamond"; }
                case EnclosureShape::none: { return "none"; }
                default: break;
            }
            return "rectangle";
        }

        std::ostream& toStream( std::ostream& os, const EnclosureShape value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const EnclosureShape value )
        {
            return toStream( os, value );
        }

        /// Fan ////////////////////////////////////////////////////////////////////////////////////

        Fan parseFan( const std::string& value )
        {
            const auto opt = tryParseFan( value );
            return opt.value_or( Fan::accel );
        }

        std::optional<Fan> tryParseFan( const std::string& value )
        {
            if( value == "accel" ) { return Fan::accel; }
            else if( value == "rit" ) { return Fan::rit; }
            else if( value == "none" ) { return Fan::none; }
            return std::optional<Fan>{};
        }

        std::string toString( const Fan value )
        {
            switch ( value )
            {
                case Fan::accel: { return "accel"; }
                case Fan::rit: { return "rit"; }
                case Fan::none: { return "none"; }
                default: break;
            }
            return "accel";
        }

        std::ostream& toStream( std::ostream& os, const Fan value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const Fan value )
        {
            return toStream( os, value );
        }

        /// FermataShape ///////////////////////////////////////////////////////////////////////////

        FermataShape parseFermataShape( const std::string& value )
        {
            const auto opt = tryParseFermataShape( value );
            return opt.value_or( FermataShape::normal );
        }

        std::optional<FermataShape> tryParseFermataShape( const std::string& value )
        {
            if( value == "normal" ) { return FermataShape::normal; }
            else if( value == "angled" ) { return FermataShape::angled; }
            else if( value == "square" ) { return FermataShape::square; }
            else if( value == "" ) { return FermataShape::emptystring; }
            return std::optional<FermataShape>{};
        }

        std::string toString( const FermataShape value )
        {
            switch ( value )
            {
                case FermataShape::normal: { return "normal"; }
                case FermataShape::angled: { return "angled"; }
                case FermataShape::square: { return "square"; }
                case FermataShape::emptystring: { return ""; }
                default: break;
            }
            return "normal";
        }

        std::ostream& toStream( std::ostream& os, const FermataShape value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const FermataShape value )
        {
            return toStream( os, value );
        }

        /// FontStyle //////////////////////////////////////////////////////////////////////////////

        FontStyle parseFontStyle( const std::string& value )
        {
            const auto opt = tryParseFontStyle( value );
            return opt.value_or( FontStyle::normal );
        }

        std::optional<FontStyle> tryParseFontStyle( const std::string& value )
        {
            if( value == "normal" ) { return FontStyle::normal; }
            else if( value == "italic" ) { return FontStyle::italic; }
            return std::optional<FontStyle>{};
        }

        std::string toString( const FontStyle value )
        {
            switch ( value )
            {
                case FontStyle::normal: { return "normal"; }
                case FontStyle::italic: { return "italic"; }
                default: break;
            }
            return "normal";
        }

        std::ostream& toStream( std::ostream& os, const FontStyle value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const FontStyle value )
        {
            return toStream( os, value );
        }

        /// FontWeight /////////////////////////////////////////////////////////////////////////////

        FontWeight parseFontWeight( const std::string& value )
        {
            const auto opt = tryParseFontWeight( value );
            return opt.value_or( FontWeight::normal );
        }

        std::optional<FontWeight> tryParseFontWeight( const std::string& value )
        {
            if( value == "normal" ) { return FontWeight::normal; }
            else if( value == "bold" ) { return FontWeight::bold; }
            return std::optional<FontWeight>{};
        }

        std::string toString( const FontWeight value )
        {
            switch ( value )
            {
                case FontWeight::normal: { return "normal"; }
                case FontWeight::bold: { return "bold"; }
                default: break;
            }
            return "normal";
        }

        std::ostream& toStream( std::ostream& os, const FontWeight value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const FontWeight value )
        {
            return toStream( os, value );
        }

        /// GlassEnum //////////////////////////////////////////////////////////////////////////////

        GlassEnum parseGlassEnum( const std::string& value )
        {
            const auto opt = tryParseGlassEnum( value );
            return opt.value_or( GlassEnum::windChimes );
        }

        std::optional<GlassEnum> tryParseGlassEnum( const std::string& value )
        {
            if( value == "wind chimes" ) { return GlassEnum::windChimes; }
            return std::optional<GlassEnum>{};
        }

        std::string toString( const GlassEnum value )
        {
            switch ( value )
            {
                case GlassEnum::windChimes: { return "wind chimes"; }
                default: break;
            }
            return "wind chimes";
        }

        std::ostream& toStream( std::ostream& os, const GlassEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const GlassEnum value )
        {
            return toStream( os, value );
        }

        /// GroupBarlineValue //////////////////////////////////////////////////////////////////////

        GroupBarlineValue parseGroupBarlineValue( const std::string& value )
        {
            const auto opt = tryParseGroupBarlineValue( value );
            return opt.value_or( GroupBarlineValue::yes );
        }

        std::optional<GroupBarlineValue> tryParseGroupBarlineValue( const std::string& value )
        {
            if( value == "yes" ) { return GroupBarlineValue::yes; }
            else if( value == "no" ) { return GroupBarlineValue::no; }
            else if( value == "Mensurstrich" ) { return GroupBarlineValue::mensurstrich; }
            return std::optional<GroupBarlineValue>{};
        }

        std::string toString( const GroupBarlineValue value )
        {
            switch ( value )
            {
                case GroupBarlineValue::yes: { return "yes"; }
                case GroupBarlineValue::no: { return "no"; }
                case GroupBarlineValue::mensurstrich: { return "Mensurstrich"; }
                default: break;
            }
            return "yes";
        }

        std::ostream& toStream( std::ostream& os, const GroupBarlineValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const GroupBarlineValue value )
        {
            return toStream( os, value );
        }

        /// GroupSymbolValue ///////////////////////////////////////////////////////////////////////

        GroupSymbolValue parseGroupSymbolValue( const std::string& value )
        {
            const auto opt = tryParseGroupSymbolValue( value );
            return opt.value_or( GroupSymbolValue::none );
        }

        std::optional<GroupSymbolValue> tryParseGroupSymbolValue( const std::string& value )
        {
            if( value == "none" ) { return GroupSymbolValue::none; }
            else if( value == "brace" ) { return GroupSymbolValue::brace; }
            else if( value == "line" ) { return GroupSymbolValue::line; }
            else if( value == "bracket" ) { return GroupSymbolValue::bracket; }
            else if( value == "square" ) { return GroupSymbolValue::square; }
            return std::optional<GroupSymbolValue>{};
        }

        std::string toString( const GroupSymbolValue value )
        {
            switch ( value )
            {
                case GroupSymbolValue::none: { return "none"; }
                case GroupSymbolValue::brace: { return "brace"; }
                case GroupSymbolValue::line: { return "line"; }
                case GroupSymbolValue::bracket: { return "bracket"; }
                case GroupSymbolValue::square: { return "square"; }
                default: break;
            }
            return "none";
        }

        std::ostream& toStream( std::ostream& os, const GroupSymbolValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const GroupSymbolValue value )
        {
            return toStream( os, value );
        }

        /// HandbellValue //////////////////////////////////////////////////////////////////////////

        HandbellValue parseHandbellValue( const std::string& value )
        {
            const auto opt = tryParseHandbellValue( value );
            return opt.value_or( HandbellValue::damp );
        }

        std::optional<HandbellValue> tryParseHandbellValue( const std::string& value )
        {
            if( value == "damp" ) { return HandbellValue::damp; }
            else if( value == "echo" ) { return HandbellValue::echo; }
            else if( value == "gyro" ) { return HandbellValue::gyro; }
            else if( value == "hand martellato" ) { return HandbellValue::handMartellato; }
            else if( value == "mallet lift" ) { return HandbellValue::malletLift; }
            else if( value == "mallet table" ) { return HandbellValue::malletTable; }
            else if( value == "martellato" ) { return HandbellValue::martellato; }
            else if( value == "martellato lift" ) { return HandbellValue::martellatoLift; }
            else if( value == "muted martellato" ) { return HandbellValue::mutedMartellato; }
            else if( value == "pluck lift" ) { return HandbellValue::pluckLift; }
            else if( value == "swing" ) { return HandbellValue::swing; }
            return std::optional<HandbellValue>{};
        }

        std::string toString( const HandbellValue value )
        {
            switch ( value )
            {
                case HandbellValue::damp: { return "damp"; }
                case HandbellValue::echo: { return "echo"; }
                case HandbellValue::gyro: { return "gyro"; }
                case HandbellValue::handMartellato: { return "hand martellato"; }
                case HandbellValue::malletLift: { return "mallet lift"; }
                case HandbellValue::malletTable: { return "mallet table"; }
                case HandbellValue::martellato: { return "martellato"; }
                case HandbellValue::martellatoLift: { return "martellato lift"; }
                case HandbellValue::mutedMartellato: { return "muted martellato"; }
                case HandbellValue::pluckLift: { return "pluck lift"; }
                case HandbellValue::swing: { return "swing"; }
                default: break;
            }
            return "damp";
        }

        std::ostream& toStream( std::ostream& os, const HandbellValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const HandbellValue value )
        {
            return toStream( os, value );
        }

        /// HarmonyType ////////////////////////////////////////////////////////////////////////////

        HarmonyType parseHarmonyType( const std::string& value )
        {
            const auto opt = tryParseHarmonyType( value );
            return opt.value_or( HarmonyType::explicit_ );
        }

        std::optional<HarmonyType> tryParseHarmonyType( const std::string& value )
        {
            if( value == "explicit" ) { return HarmonyType::explicit_; }
            else if( value == "implied" ) { return HarmonyType::implied; }
            else if( value == "alternate" ) { return HarmonyType::alternate; }
            return std::optional<HarmonyType>{};
        }

        std::string toString( const HarmonyType value )
        {
            switch ( value )
            {
                case HarmonyType::explicit_: { return "explicit"; }
                case HarmonyType::implied: { return "implied"; }
                case HarmonyType::alternate: { return "alternate"; }
                default: break;
            }
            return "explicit";
        }

        std::ostream& toStream( std::ostream& os, const HarmonyType value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const HarmonyType value )
        {
            return toStream( os, value );
        }

        /// HoleClosedLocation /////////////////////////////////////////////////////////////////////

        HoleClosedLocation parseHoleClosedLocation( const std::string& value )
        {
            const auto opt = tryParseHoleClosedLocation( value );
            return opt.value_or( HoleClosedLocation::right );
        }

        std::optional<HoleClosedLocation> tryParseHoleClosedLocation( const std::string& value )
        {
            if( value == "right" ) { return HoleClosedLocation::right; }
            else if( value == "bottom" ) { return HoleClosedLocation::bottom; }
            else if( value == "left" ) { return HoleClosedLocation::left; }
            else if( value == "top" ) { return HoleClosedLocation::top; }
            return std::optional<HoleClosedLocation>{};
        }

        std::string toString( const HoleClosedLocation value )
        {
            switch ( value )
            {
                case HoleClosedLocation::right: { return "right"; }
                case HoleClosedLocation::bottom: { return "bottom"; }
                case HoleClosedLocation::left: { return "left"; }
                case HoleClosedLocation::top: { return "top"; }
                default: break;
            }
            return "right";
        }

        std::ostream& toStream( std::ostream& os, const HoleClosedLocation value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const HoleClosedLocation value )
        {
            return toStream( os, value );
        }

        /// HoleClosedValue ////////////////////////////////////////////////////////////////////////

        HoleClosedValue parseHoleClosedValue( const std::string& value )
        {
            const auto opt = tryParseHoleClosedValue( value );
            return opt.value_or( HoleClosedValue::yes );
        }

        std::optional<HoleClosedValue> tryParseHoleClosedValue( const std::string& value )
        {
            if( value == "yes" ) { return HoleClosedValue::yes; }
            else if( value == "no" ) { return HoleClosedValue::no; }
            else if( value == "half" ) { return HoleClosedValue::half; }
            return std::optional<HoleClosedValue>{};
        }

        std::string toString( const HoleClosedValue value )
        {
            switch ( value )
            {
                case HoleClosedValue::yes: { return "yes"; }
                case HoleClosedValue::no: { return "no"; }
                case HoleClosedValue::half: { return "half"; }
                default: break;
            }
            return "yes";
        }

        std::ostream& toStream( std::ostream& os, const HoleClosedValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const HoleClosedValue value )
        {
            return toStream( os, value );
        }

        /// KindValue //////////////////////////////////////////////////////////////////////////////

        KindValue parseKindValue( const std::string& value )
        {
            const auto opt = tryParseKindValue( value );
            return opt.value_or( KindValue::major );
        }

        std::optional<KindValue> tryParseKindValue( const std::string& value )
        {
            if( value == "major" ) { return KindValue::major; }
            else if( value == "minor" ) { return KindValue::minor; }
            else if( value == "augmented" ) { return KindValue::augmented; }
            else if( value == "diminished" ) { return KindValue::diminished; }
            else if( value == "dominant" ) { return KindValue::dominant; }
            else if( value == "major-seventh" ) { return KindValue::majorSeventh; }
            else if( value == "minor-seventh" ) { return KindValue::minorSeventh; }
            else if( value == "diminished-seventh" ) { return KindValue::diminishedSeventh; }
            else if( value == "augmented-seventh" ) { return KindValue::augmentedSeventh; }
            else if( value == "half-diminished" ) { return KindValue::halfDiminished; }
            else if( value == "major-minor" ) { return KindValue::majorMinor; }
            else if( value == "major-sixth" ) { return KindValue::majorSixth; }
            else if( value == "minor-sixth" ) { return KindValue::minorSixth; }
            else if( value == "dominant-ninth" ) { return KindValue::dominantNinth; }
            else if( value == "major-ninth" ) { return KindValue::majorNinth; }
            else if( value == "minor-ninth" ) { return KindValue::minorNinth; }
            else if( value == "dominant-11th" ) { return KindValue::dominant11th; }
            else if( value == "major-11th" ) { return KindValue::major11th; }
            else if( value == "minor-11th" ) { return KindValue::minor11th; }
            else if( value == "dominant-13th" ) { return KindValue::dominant13th; }
            else if( value == "major-13th" ) { return KindValue::major13th; }
            else if( value == "minor-13th" ) { return KindValue::minor13th; }
            else if( value == "suspended-second" ) { return KindValue::suspendedSecond; }
            else if( value == "suspended-fourth" ) { return KindValue::suspendedFourth; }
            else if( value == "Neapolitan" ) { return KindValue::neapolitan; }
            else if( value == "Italian" ) { return KindValue::italian; }
            else if( value == "French" ) { return KindValue::french; }
            else if( value == "German" ) { return KindValue::german; }
            else if( value == "pedal" ) { return KindValue::pedal; }
            else if( value == "power" ) { return KindValue::power; }
            else if( value == "Tristan" ) { return KindValue::tristan; }
            else if( value == "other" ) { return KindValue::other; }
            else if( value == "none" ) { return KindValue::none; }
            return std::optional<KindValue>{};
        }

        std::string toString( const KindValue value )
        {
            switch ( value )
            {
                case KindValue::major: { return "major"; }
                case KindValue::minor: { return "minor"; }
                case KindValue::augmented: { return "augmented"; }
                case KindValue::diminished: { return "diminished"; }
                case KindValue::dominant: { return "dominant"; }
                case KindValue::majorSeventh: { return "major-seventh"; }
                case KindValue::minorSeventh: { return "minor-seventh"; }
                case KindValue::diminishedSeventh: { return "diminished-seventh"; }
                case KindValue::augmentedSeventh: { return "augmented-seventh"; }
                case KindValue::halfDiminished: { return "half-diminished"; }
                case KindValue::majorMinor: { return "major-minor"; }
                case KindValue::majorSixth: { return "major-sixth"; }
                case KindValue::minorSixth: { return "minor-sixth"; }
                case KindValue::dominantNinth: { return "dominant-ninth"; }
                case KindValue::majorNinth: { return "major-ninth"; }
                case KindValue::minorNinth: { return "minor-ninth"; }
                case KindValue::dominant11th: { return "dominant-11th"; }
                case KindValue::major11th: { return "major-11th"; }
                case KindValue::minor11th: { return "minor-11th"; }
                case KindValue::dominant13th: { return "dominant-13th"; }
                case KindValue::major13th: { return "major-13th"; }
                case KindValue::minor13th: { return "minor-13th"; }
                case KindValue::suspendedSecond: { return "suspended-second"; }
                case KindValue::suspendedFourth: { return "suspended-fourth"; }
                case KindValue::neapolitan: { return "Neapolitan"; }
                case KindValue::italian: { return "Italian"; }
                case KindValue::french: { return "French"; }
                case KindValue::german: { return "German"; }
                case KindValue::pedal: { return "pedal"; }
                case KindValue::power: { return "power"; }
                case KindValue::tristan: { return "Tristan"; }
                case KindValue::other: { return "other"; }
                case KindValue::none: { return "none"; }
                default: break;
            }
            return "major";
        }

        std::ostream& toStream( std::ostream& os, const KindValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const KindValue value )
        {
            return toStream( os, value );
        }

        /// LeftCenterRight ////////////////////////////////////////////////////////////////////////

        LeftCenterRight parseLeftCenterRight( const std::string& value )
        {
            const auto opt = tryParseLeftCenterRight( value );
            return opt.value_or( LeftCenterRight::left );
        }

        std::optional<LeftCenterRight> tryParseLeftCenterRight( const std::string& value )
        {
            if( value == "left" ) { return LeftCenterRight::left; }
            else if( value == "center" ) { return LeftCenterRight::center; }
            else if( value == "right" ) { return LeftCenterRight::right; }
            return std::optional<LeftCenterRight>{};
        }

        std::string toString( const LeftCenterRight value )
        {
            switch ( value )
            {
                case LeftCenterRight::left: { return "left"; }
                case LeftCenterRight::center: { return "center"; }
                case LeftCenterRight::right: { return "right"; }
                default: break;
            }
            return "left";
        }

        std::ostream& toStream( std::ostream& os, const LeftCenterRight value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const LeftCenterRight value )
        {
            return toStream( os, value );
        }

        /// LeftRight //////////////////////////////////////////////////////////////////////////////

        LeftRight parseLeftRight( const std::string& value )
        {
            const auto opt = tryParseLeftRight( value );
            return opt.value_or( LeftRight::left );
        }

        std::optional<LeftRight> tryParseLeftRight( const std::string& value )
        {
            if( value == "left" ) { return LeftRight::left; }
            else if( value == "right" ) { return LeftRight::right; }
            return std::optional<LeftRight>{};
        }

        std::string toString( const LeftRight value )
        {
            switch ( value )
            {
                case LeftRight::left: { return "left"; }
                case LeftRight::right: { return "right"; }
                default: break;
            }
            return "left";
        }

        std::ostream& toStream( std::ostream& os, const LeftRight value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const LeftRight value )
        {
            return toStream( os, value );
        }

        /// LineEnd ////////////////////////////////////////////////////////////////////////////////

        LineEnd parseLineEnd( const std::string& value )
        {
            const auto opt = tryParseLineEnd( value );
            return opt.value_or( LineEnd::up );
        }

        std::optional<LineEnd> tryParseLineEnd( const std::string& value )
        {
            if( value == "up" ) { return LineEnd::up; }
            else if( value == "down" ) { return LineEnd::down; }
            else if( value == "both" ) { return LineEnd::both; }
            else if( value == "arrow" ) { return LineEnd::arrow; }
            else if( value == "none" ) { return LineEnd::none; }
            return std::optional<LineEnd>{};
        }

        std::string toString( const LineEnd value )
        {
            switch ( value )
            {
                case LineEnd::up: { return "up"; }
                case LineEnd::down: { return "down"; }
                case LineEnd::both: { return "both"; }
                case LineEnd::arrow: { return "arrow"; }
                case LineEnd::none: { return "none"; }
                default: break;
            }
            return "up";
        }

        std::ostream& toStream( std::ostream& os, const LineEnd value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const LineEnd value )
        {
            return toStream( os, value );
        }

        /// LineShape //////////////////////////////////////////////////////////////////////////////

        LineShape parseLineShape( const std::string& value )
        {
            const auto opt = tryParseLineShape( value );
            return opt.value_or( LineShape::straight );
        }

        std::optional<LineShape> tryParseLineShape( const std::string& value )
        {
            if( value == "straight" ) { return LineShape::straight; }
            else if( value == "curved" ) { return LineShape::curved; }
            return std::optional<LineShape>{};
        }

        std::string toString( const LineShape value )
        {
            switch ( value )
            {
                case LineShape::straight: { return "straight"; }
                case LineShape::curved: { return "curved"; }
                default: break;
            }
            return "straight";
        }

        std::ostream& toStream( std::ostream& os, const LineShape value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const LineShape value )
        {
            return toStream( os, value );
        }

        /// LineType ///////////////////////////////////////////////////////////////////////////////

        LineType parseLineType( const std::string& value )
        {
            const auto opt = tryParseLineType( value );
            return opt.value_or( LineType::solid );
        }

        std::optional<LineType> tryParseLineType( const std::string& value )
        {
            if( value == "solid" ) { return LineType::solid; }
            else if( value == "dashed" ) { return LineType::dashed; }
            else if( value == "dotted" ) { return LineType::dotted; }
            else if( value == "wavy" ) { return LineType::wavy; }
            return std::optional<LineType>{};
        }

        std::string toString( const LineType value )
        {
            switch ( value )
            {
                case LineType::solid: { return "solid"; }
                case LineType::dashed: { return "dashed"; }
                case LineType::dotted: { return "dotted"; }
                case LineType::wavy: { return "wavy"; }
                default: break;
            }
            return "solid";
        }

        std::ostream& toStream( std::ostream& os, const LineType value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const LineType value )
        {
            return toStream( os, value );
        }

        /// LineWidthTypeEnum //////////////////////////////////////////////////////////////////////

        LineWidthTypeEnum parseLineWidthTypeEnum( const std::string& value )
        {
            const auto opt = tryParseLineWidthTypeEnum( value );
            return opt.value_or( LineWidthTypeEnum::beam );
        }

        std::optional<LineWidthTypeEnum> tryParseLineWidthTypeEnum( const std::string& value )
        {
            if( value == "beam" ) { return LineWidthTypeEnum::beam; }
            else if( value == "bracket" ) { return LineWidthTypeEnum::bracket; }
            else if( value == "dashes" ) { return LineWidthTypeEnum::dashes; }
            else if( value == "enclosure" ) { return LineWidthTypeEnum::enclosure; }
            else if( value == "ending" ) { return LineWidthTypeEnum::ending; }
            else if( value == "extend" ) { return LineWidthTypeEnum::extend; }
            else if( value == "heavy barline" ) { return LineWidthTypeEnum::heavyBarline; }
            else if( value == "leger" ) { return LineWidthTypeEnum::leger; }
            else if( value == "light barline" ) { return LineWidthTypeEnum::lightBarline; }
            else if( value == "octave shift" ) { return LineWidthTypeEnum::octaveShift; }
            else if( value == "pedal" ) { return LineWidthTypeEnum::pedal; }
            else if( value == "slur middle" ) { return LineWidthTypeEnum::slurMiddle; }
            else if( value == "slur tip" ) { return LineWidthTypeEnum::slurTip; }
            else if( value == "staff" ) { return LineWidthTypeEnum::staff; }
            else if( value == "stem" ) { return LineWidthTypeEnum::stem; }
            else if( value == "tie middle" ) { return LineWidthTypeEnum::tieMiddle; }
            else if( value == "tie tip" ) { return LineWidthTypeEnum::tieTip; }
            else if( value == "tuplet bracket" ) { return LineWidthTypeEnum::tupletBracket; }
            else if( value == "wedge" ) { return LineWidthTypeEnum::wedge; }
            else if( value == "other" ) { return LineWidthTypeEnum::other; }
            return std::optional<LineWidthTypeEnum>{};
        }

        std::string toString( const LineWidthTypeEnum value )
        {
            switch ( value )
            {
                case LineWidthTypeEnum::beam: { return "beam"; }
                case LineWidthTypeEnum::bracket: { return "bracket"; }
                case LineWidthTypeEnum::dashes: { return "dashes"; }
                case LineWidthTypeEnum::enclosure: { return "enclosure"; }
                case LineWidthTypeEnum::ending: { return "ending"; }
                case LineWidthTypeEnum::extend: { return "extend"; }
                case LineWidthTypeEnum::heavyBarline: { return "heavy barline"; }
                case LineWidthTypeEnum::leger: { return "leger"; }
                case LineWidthTypeEnum::lightBarline: { return "light barline"; }
                case LineWidthTypeEnum::octaveShift: { return "octave shift"; }
                case LineWidthTypeEnum::pedal: { return "pedal"; }
                case LineWidthTypeEnum::slurMiddle: { return "slur middle"; }
                case LineWidthTypeEnum::slurTip: { return "slur tip"; }
                case LineWidthTypeEnum::staff: { return "staff"; }
                case LineWidthTypeEnum::stem: { return "stem"; }
                case LineWidthTypeEnum::tieMiddle: { return "tie middle"; }
                case LineWidthTypeEnum::tieTip: { return "tie tip"; }
                case LineWidthTypeEnum::tupletBracket: { return "tuplet bracket"; }
                case LineWidthTypeEnum::wedge: { return "wedge"; }
                default: break;
            }
            return "other";
        }

        std::ostream& toStream( std::ostream& os, const LineWidthTypeEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const LineWidthTypeEnum value )
        {
            return toStream( os, value );
        }

        LineWidthType::LineWidthType( const LineWidthTypeEnum value )
        : myEnum( value )
        , myCustomValue( "" )
        {
            setValue( value );
        }

        LineWidthType::LineWidthType( const std::string& value )
        : myEnum( LineWidthTypeEnum::other )
        , myCustomValue( value )
        {
            setValue( value );
        }

        LineWidthType::LineWidthType()
        : myEnum( LineWidthTypeEnum::beam )
        , myCustomValue( "" )
        {
            setValue( LineWidthTypeEnum::beam );
        }

        LineWidthTypeEnum LineWidthType::getValue() const
        {
            return myEnum;
        }

        std::string LineWidthType::getValueString() const
        {
            if ( myEnum != LineWidthTypeEnum::other )
            {
                return toString( myEnum );
            }

            else
            {
                return myCustomValue;
            }
        }

        void LineWidthType::setValue( const LineWidthTypeEnum value )
        {
            myEnum = value;
            if( value != LineWidthTypeEnum::other && !myCustomValue.empty() )
            {
                myCustomValue = "";
            }
        }

        void LineWidthType::setValue( const std::string& value )
        {
            const auto temp = tryParseLineWidthTypeEnum( value );
            if ( temp )
            {
                myEnum = temp.value();
                myCustomValue = "";
            }
            else
            {
                setValue( LineWidthTypeEnum::other );
                myCustomValue = value;
            }
        }

        LineWidthType parseLineWidthType( const std::string& value )
        {
            return LineWidthType( value );
        }

        std::string toString( const LineWidthType& value )
        {
            return value.getValueString();
        }

        std::ostream& toStream( std::ostream& os, const LineWidthType& value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const LineWidthType& value )
        {
            return toStream( os, value );
        }

        /// MarginType /////////////////////////////////////////////////////////////////////////////

        MarginType parseMarginType( const std::string& value )
        {
            const auto opt = tryParseMarginType( value );
            return opt.value_or( MarginType::odd );
        }

        std::optional<MarginType> tryParseMarginType( const std::string& value )
        {
            if( value == "odd" ) { return MarginType::odd; }
            else if( value == "even" ) { return MarginType::even; }
            else if( value == "both" ) { return MarginType::both; }
            return std::optional<MarginType>{};
        }

        std::string toString( const MarginType value )
        {
            switch ( value )
            {
                case MarginType::odd: { return "odd"; }
                case MarginType::even: { return "even"; }
                case MarginType::both: { return "both"; }
                default: break;
            }
            return "odd";
        }

        std::ostream& toStream( std::ostream& os, const MarginType value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const MarginType value )
        {
            return toStream( os, value );
        }

        /// MeasureNumberingValue //////////////////////////////////////////////////////////////////

        MeasureNumberingValue parseMeasureNumberingValue( const std::string& value )
        {
            const auto opt = tryParseMeasureNumberingValue( value );
            return opt.value_or( MeasureNumberingValue::none );
        }

        std::optional<MeasureNumberingValue> tryParseMeasureNumberingValue( const std::string& value )
        {
            if( value == "none" ) { return MeasureNumberingValue::none; }
            else if( value == "measure" ) { return MeasureNumberingValue::measure; }
            else if( value == "system" ) { return MeasureNumberingValue::system; }
            return std::optional<MeasureNumberingValue>{};
        }

        std::string toString( const MeasureNumberingValue value )
        {
            switch ( value )
            {
                case MeasureNumberingValue::none: { return "none"; }
                case MeasureNumberingValue::measure: { return "measure"; }
                case MeasureNumberingValue::system: { return "system"; }
                default: break;
            }
            return "none";
        }

        std::ostream& toStream( std::ostream& os, const MeasureNumberingValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const MeasureNumberingValue value )
        {
            return toStream( os, value );
        }

        /// MembraneEnum ///////////////////////////////////////////////////////////////////////////

        MembraneEnum parseMembraneEnum( const std::string& value )
        {
            const auto opt = tryParseMembraneEnum( value );
            return opt.value_or( MembraneEnum::bassDrum );
        }

        std::optional<MembraneEnum> tryParseMembraneEnum( const std::string& value )
        {
            if( value == "bass drum" ) { return MembraneEnum::bassDrum; }
            else if( value == "bass drum on side" ) { return MembraneEnum::bassDrumOnSide; }
            else if( value == "bongos" ) { return MembraneEnum::bongos; }
            else if( value == "conga drum" ) { return MembraneEnum::congaDrum; }
            else if( value == "goblet drum" ) { return MembraneEnum::gobletDrum; }
            else if( value == "military drum" ) { return MembraneEnum::militaryDrum; }
            else if( value == "snare drum" ) { return MembraneEnum::snareDrum; }
            else if( value == "snare drum snares off" ) { return MembraneEnum::snareDrumSnaresOff; }
            else if( value == "tambourine" ) { return MembraneEnum::tambourine; }
            else if( value == "tenor drum" ) { return MembraneEnum::tenorDrum; }
            else if( value == "timbales" ) { return MembraneEnum::timbales; }
            else if( value == "tomtom" ) { return MembraneEnum::tomtom; }
            return std::optional<MembraneEnum>{};
        }

        std::string toString( const MembraneEnum value )
        {
            switch ( value )
            {
                case MembraneEnum::bassDrum: { return "bass drum"; }
                case MembraneEnum::bassDrumOnSide: { return "bass drum on side"; }
                case MembraneEnum::bongos: { return "bongos"; }
                case MembraneEnum::congaDrum: { return "conga drum"; }
                case MembraneEnum::gobletDrum: { return "goblet drum"; }
                case MembraneEnum::militaryDrum: { return "military drum"; }
                case MembraneEnum::snareDrum: { return "snare drum"; }
                case MembraneEnum::snareDrumSnaresOff: { return "snare drum snares off"; }
                case MembraneEnum::tambourine: { return "tambourine"; }
                case MembraneEnum::tenorDrum: { return "tenor drum"; }
                case MembraneEnum::timbales: { return "timbales"; }
                case MembraneEnum::tomtom: { return "tomtom"; }
                default: break;
            }
            return "bass drum";
        }

        std::ostream& toStream( std::ostream& os, const MembraneEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const MembraneEnum value )
        {
            return toStream( os, value );
        }

        /// MetalEnum //////////////////////////////////////////////////////////////////////////////

        MetalEnum parseMetalEnum( const std::string& value )
        {
            const auto opt = tryParseMetalEnum( value );
            return opt.value_or( MetalEnum::almglocken );
        }

        std::optional<MetalEnum> tryParseMetalEnum( const std::string& value )
        {
            if( value == "almglocken" ) { return MetalEnum::almglocken; }
            else if( value == "bell" ) { return MetalEnum::bell; }
            else if( value == "bell plate" ) { return MetalEnum::bellPlate; }
            else if( value == "brake drum" ) { return MetalEnum::brakeDrum; }
            else if( value == "Chinese cymbal" ) { return MetalEnum::chineseCymbal; }
            else if( value == "cowbell" ) { return MetalEnum::cowbell; }
            else if( value == "crash cymbals" ) { return MetalEnum::crashCymbals; }
            else if( value == "crotale" ) { return MetalEnum::crotale; }
            else if( value == "cymbal tongs" ) { return MetalEnum::cymbalTongs; }
            else if( value == "domed gong" ) { return MetalEnum::domedGong; }
            else if( value == "finger cymbals" ) { return MetalEnum::fingerCymbals; }
            else if( value == "flexatone" ) { return MetalEnum::flexatone; }
            else if( value == "gong" ) { return MetalEnum::gong; }
            else if( value == "hi-hat" ) { return MetalEnum::hiHat; }
            else if( value == "high-hat cymbals" ) { return MetalEnum::highHatCymbals; }
            else if( value == "handbell" ) { return MetalEnum::handbell; }
            else if( value == "sistrum" ) { return MetalEnum::sistrum; }
            else if( value == "sizzle cymbal" ) { return MetalEnum::sizzleCymbal; }
            else if( value == "sleigh bells" ) { return MetalEnum::sleighBells; }
            else if( value == "suspended cymbal" ) { return MetalEnum::suspendedCymbal; }
            else if( value == "tam tam" ) { return MetalEnum::tamTam; }
            else if( value == "triangle" ) { return MetalEnum::triangle; }
            else if( value == "Vietnamese hat" ) { return MetalEnum::vietnameseHat; }
            return std::optional<MetalEnum>{};
        }

        std::string toString( const MetalEnum value )
        {
            switch ( value )
            {
                case MetalEnum::almglocken: { return "almglocken"; }
                case MetalEnum::bell: { return "bell"; }
                case MetalEnum::bellPlate: { return "bell plate"; }
                case MetalEnum::brakeDrum: { return "brake drum"; }
                case MetalEnum::chineseCymbal: { return "Chinese cymbal"; }
                case MetalEnum::cowbell: { return "cowbell"; }
                case MetalEnum::crashCymbals: { return "crash cymbals"; }
                case MetalEnum::crotale: { return "crotale"; }
                case MetalEnum::cymbalTongs: { return "cymbal tongs"; }
                case MetalEnum::domedGong: { return "domed gong"; }
                case MetalEnum::fingerCymbals: { return "finger cymbals"; }
                case MetalEnum::flexatone: { return "flexatone"; }
                case MetalEnum::gong: { return "gong"; }
                case MetalEnum::hiHat: { return "hi-hat"; }
                case MetalEnum::highHatCymbals: { return "high-hat cymbals"; }
                case MetalEnum::handbell: { return "handbell"; }
                case MetalEnum::sistrum: { return "sistrum"; }
                case MetalEnum::sizzleCymbal: { return "sizzle cymbal"; }
                case MetalEnum::sleighBells: { return "sleigh bells"; }
                case MetalEnum::suspendedCymbal: { return "suspended cymbal"; }
                case MetalEnum::tamTam: { return "tam tam"; }
                case MetalEnum::triangle: { return "triangle"; }
                case MetalEnum::vietnameseHat: { return "Vietnamese hat"; }
                default: break;
            }
            return "almglocken";
        }

        std::ostream& toStream( std::ostream& os, const MetalEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const MetalEnum value )
        {
            return toStream( os, value );
        }

        /// ModeEnum ///////////////////////////////////////////////////////////////////////////////

        ModeEnum parseModeEnum( const std::string& value )
        {
            const auto opt = tryParseModeEnum( value );
            return opt.value_or( ModeEnum::major );
        }

        std::optional<ModeEnum> tryParseModeEnum( const std::string& value )
        {
            if( value == "major" ) { return ModeEnum::major; }
            else if( value == "minor" ) { return ModeEnum::minor; }
            else if( value == "dorian" ) { return ModeEnum::dorian; }
            else if( value == "phrygian" ) { return ModeEnum::phrygian; }
            else if( value == "lydian" ) { return ModeEnum::lydian; }
            else if( value == "mixolydian" ) { return ModeEnum::mixolydian; }
            else if( value == "aeolian" ) { return ModeEnum::aeolian; }
            else if( value == "ionian" ) { return ModeEnum::ionian; }
            else if( value == "locrian" ) { return ModeEnum::locrian; }
            else if( value == "none" ) { return ModeEnum::none; }
            else if( value == "other" ) { return ModeEnum::other; }
            return std::optional<ModeEnum>{};
        }

        std::string toString( const ModeEnum value )
        {
            switch ( value )
            {
                case ModeEnum::major: { return "major"; }
                case ModeEnum::minor: { return "minor"; }
                case ModeEnum::dorian: { return "dorian"; }
                case ModeEnum::phrygian: { return "phrygian"; }
                case ModeEnum::lydian: { return "lydian"; }
                case ModeEnum::mixolydian: { return "mixolydian"; }
                case ModeEnum::aeolian: { return "aeolian"; }
                case ModeEnum::ionian: { return "ionian"; }
                case ModeEnum::locrian: { return "locrian"; }
                case ModeEnum::none: { return "none"; }
                default: break;
            }
            return "other";
        }

        std::ostream& toStream( std::ostream& os, const ModeEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const ModeEnum value )
        {
            return toStream( os, value );
        }

        ModeValue::ModeValue( const ModeEnum value )
        : myEnum( value )
        , myCustomValue( "" )
        {
            setValue( value );
        }

        ModeValue::ModeValue( const std::string& value )
        : myEnum( ModeEnum::other )
        , myCustomValue( value )
        {
            setValue( value );
        }

        ModeValue::ModeValue()
        : myEnum( ModeEnum::major )
        , myCustomValue( "" )
        {
            setValue( ModeEnum::major );
        }

        ModeEnum ModeValue::getValue() const
        {
            return myEnum;
        }

        std::string ModeValue::getValueString() const
        {
            if ( myEnum != ModeEnum::other )
            {
                return toString( myEnum );
            }

            else
            {
                return myCustomValue;
            }
        }

        void ModeValue::setValue( const ModeEnum value )
        {
            myEnum = value;
            if( value != ModeEnum::other && !myCustomValue.empty() )
            {
                myCustomValue = "";
            }
        }

        void ModeValue::setValue( const std::string& value )
        {
            const auto temp = tryParseModeEnum( value );
            if ( temp )
            {
                myEnum = temp.value();
                myCustomValue = "";
            }
            else
            {
                setValue( ModeEnum::other );
                myCustomValue = value;
            }
        }

        ModeValue parseModeValue( const std::string& value )
        {
            return ModeValue( value );
        }

        std::string toString( const ModeValue& value )
        {
            return value.getValueString();
        }

        std::ostream& toStream( std::ostream& os, const ModeValue& value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const ModeValue& value )
        {
            return toStream( os, value );
        }

        /// MuteEnum ///////////////////////////////////////////////////////////////////////////////

        MuteEnum parseMuteEnum( const std::string& value )
        {
            const auto opt = tryParseMuteEnum( value );
            return opt.value_or( MuteEnum::on );
        }

        std::optional<MuteEnum> tryParseMuteEnum( const std::string& value )
        {
            if( value == "on" ) { return MuteEnum::on; }
            else if( value == "off" ) { return MuteEnum::off; }
            else if( value == "straight" ) { return MuteEnum::straight; }
            else if( value == "cup" ) { return MuteEnum::cup; }
            else if( value == "harmon-no-stem" ) { return MuteEnum::harmonNoStem; }
            else if( value == "harmon-stem" ) { return MuteEnum::harmonStem; }
            else if( value == "bucket" ) { return MuteEnum::bucket; }
            else if( value == "plunger" ) { return MuteEnum::plunger; }
            else if( value == "hat" ) { return MuteEnum::hat; }
            else if( value == "solotone" ) { return MuteEnum::solotone; }
            else if( value == "practice" ) { return MuteEnum::practice; }
            else if( value == "stop-mute" ) { return MuteEnum::stopMute; }
            else if( value == "stop-hand" ) { return MuteEnum::stopHand; }
            else if( value == "echo" ) { return MuteEnum::echo; }
            else if( value == "palm" ) { return MuteEnum::palm; }
            return std::optional<MuteEnum>{};
        }

        std::string toString( const MuteEnum value )
        {
            switch ( value )
            {
                case MuteEnum::on: { return "on"; }
                case MuteEnum::off: { return "off"; }
                case MuteEnum::straight: { return "straight"; }
                case MuteEnum::cup: { return "cup"; }
                case MuteEnum::harmonNoStem: { return "harmon-no-stem"; }
                case MuteEnum::harmonStem: { return "harmon-stem"; }
                case MuteEnum::bucket: { return "bucket"; }
                case MuteEnum::plunger: { return "plunger"; }
                case MuteEnum::hat: { return "hat"; }
                case MuteEnum::solotone: { return "solotone"; }
                case MuteEnum::practice: { return "practice"; }
                case MuteEnum::stopMute: { return "stop-mute"; }
                case MuteEnum::stopHand: { return "stop-hand"; }
                case MuteEnum::echo: { return "echo"; }
                case MuteEnum::palm: { return "palm"; }
                default: break;
            }
            return "on";
        }

        std::ostream& toStream( std::ostream& os, const MuteEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const MuteEnum value )
        {
            return toStream( os, value );
        }

        /// NoteSizeType ///////////////////////////////////////////////////////////////////////////

        NoteSizeType parseNoteSizeType( const std::string& value )
        {
            const auto opt = tryParseNoteSizeType( value );
            return opt.value_or( NoteSizeType::cue );
        }

        std::optional<NoteSizeType> tryParseNoteSizeType( const std::string& value )
        {
            if( value == "cue" ) { return NoteSizeType::cue; }
            else if( value == "grace" ) { return NoteSizeType::grace; }
            else if( value == "large" ) { return NoteSizeType::large; }
            return std::optional<NoteSizeType>{};
        }

        std::string toString( const NoteSizeType value )
        {
            switch ( value )
            {
                case NoteSizeType::cue: { return "cue"; }
                case NoteSizeType::grace: { return "grace"; }
                case NoteSizeType::large: { return "large"; }
                default: break;
            }
            return "cue";
        }

        std::ostream& toStream( std::ostream& os, const NoteSizeType value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const NoteSizeType value )
        {
            return toStream( os, value );
        }

        /// NoteTypeValue //////////////////////////////////////////////////////////////////////////

        NoteTypeValue parseNoteTypeValue( const std::string& value )
        {
            const auto opt = tryParseNoteTypeValue( value );
            return opt.value_or( NoteTypeValue::oneThousandTwentyFourth );
        }

        std::optional<NoteTypeValue> tryParseNoteTypeValue( const std::string& value )
        {
            if( value == "1024th" ) { return NoteTypeValue::oneThousandTwentyFourth; }
            else if( value == "512th" ) { return NoteTypeValue::fiveHundredTwelfth; }
            else if( value == "256th" ) { return NoteTypeValue::twoHundredFifthySixth; }
            else if( value == "128th" ) { return NoteTypeValue::oneHundredTwentyEighth; }
            else if( value == "64th" ) { return NoteTypeValue::sixtyFourth; }
            else if( value == "32nd" ) { return NoteTypeValue::thirtySecond; }
            else if( value == "16th" ) { return NoteTypeValue::sixteenth; }
            else if( value == "eighth" ) { return NoteTypeValue::eighth; }
            else if( value == "quarter" ) { return NoteTypeValue::quarter; }
            else if( value == "half" ) { return NoteTypeValue::half; }
            else if( value == "whole" ) { return NoteTypeValue::whole; }
            else if( value == "breve" ) { return NoteTypeValue::breve; }
            else if( value == "long" ) { return NoteTypeValue::long_; }
            else if( value == "maxima" ) { return NoteTypeValue::maxima; }
            return std::optional<NoteTypeValue>{};
        }

        std::string toString( const NoteTypeValue value )
        {
            switch ( value )
            {
                case NoteTypeValue::oneThousandTwentyFourth: { return "1024th"; }
                case NoteTypeValue::fiveHundredTwelfth: { return "512th"; }
                case NoteTypeValue::twoHundredFifthySixth: { return "256th"; }
                case NoteTypeValue::oneHundredTwentyEighth: { return "128th"; }
                case NoteTypeValue::sixtyFourth: { return "64th"; }
                case NoteTypeValue::thirtySecond: { return "32nd"; }
                case NoteTypeValue::sixteenth: { return "16th"; }
                case NoteTypeValue::eighth: { return "eighth"; }
                case NoteTypeValue::quarter: { return "quarter"; }
                case NoteTypeValue::half: { return "half"; }
                case NoteTypeValue::whole: { return "whole"; }
                case NoteTypeValue::breve: { return "breve"; }
                case NoteTypeValue::long_: { return "long"; }
                case NoteTypeValue::maxima: { return "maxima"; }
                default: break;
            }
            return "1024th";
        }

        std::ostream& toStream( std::ostream& os, const NoteTypeValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const NoteTypeValue value )
        {
            return toStream( os, value );
        }

        /// NoteheadValue //////////////////////////////////////////////////////////////////////////

        NoteheadValue parseNoteheadValue( const std::string& value )
        {
            const auto opt = tryParseNoteheadValue( value );
            return opt.value_or( NoteheadValue::slash );
        }

        std::optional<NoteheadValue> tryParseNoteheadValue( const std::string& value )
        {
            if( value == "slash" ) { return NoteheadValue::slash; }
            else if( value == "triangle" ) { return NoteheadValue::triangle; }
            else if( value == "diamond" ) { return NoteheadValue::diamond; }
            else if( value == "square" ) { return NoteheadValue::square; }
            else if( value == "cross" ) { return NoteheadValue::cross; }
            else if( value == "x" ) { return NoteheadValue::x; }
            else if( value == "circle-x" ) { return NoteheadValue::circleX; }
            else if( value == "inverted triangle" ) { return NoteheadValue::invertedTriangle; }
            else if( value == "arrow down" ) { return NoteheadValue::arrowDown; }
            else if( value == "arrow up" ) { return NoteheadValue::arrowUp; }
            else if( value == "slashed" ) { return NoteheadValue::slashed; }
            else if( value == "back slashed" ) { return NoteheadValue::backSlashed; }
            else if( value == "normal" ) { return NoteheadValue::normal; }
            else if( value == "cluster" ) { return NoteheadValue::cluster; }
            else if( value == "circle dot" ) { return NoteheadValue::circleDot; }
            else if( value == "left triangle" ) { return NoteheadValue::leftTriangle; }
            else if( value == "rectangle" ) { return NoteheadValue::rectangle; }
            else if( value == "none" ) { return NoteheadValue::none; }
            else if( value == "do" ) { return NoteheadValue::do_; }
            else if( value == "re" ) { return NoteheadValue::re; }
            else if( value == "mi" ) { return NoteheadValue::mi; }
            else if( value == "fa" ) { return NoteheadValue::fa; }
            else if( value == "fa up" ) { return NoteheadValue::faUp; }
            else if( value == "so" ) { return NoteheadValue::so; }
            else if( value == "la" ) { return NoteheadValue::la; }
            else if( value == "ti" ) { return NoteheadValue::ti; }
            return std::optional<NoteheadValue>{};
        }

        std::string toString( const NoteheadValue value )
        {
            switch ( value )
            {
                case NoteheadValue::slash: { return "slash"; }
                case NoteheadValue::triangle: { return "triangle"; }
                case NoteheadValue::diamond: { return "diamond"; }
                case NoteheadValue::square: { return "square"; }
                case NoteheadValue::cross: { return "cross"; }
                case NoteheadValue::x: { return "x"; }
                case NoteheadValue::circleX: { return "circle-x"; }
                case NoteheadValue::invertedTriangle: { return "inverted triangle"; }
                case NoteheadValue::arrowDown: { return "arrow down"; }
                case NoteheadValue::arrowUp: { return "arrow up"; }
                case NoteheadValue::slashed: { return "slashed"; }
                case NoteheadValue::backSlashed: { return "back slashed"; }
                case NoteheadValue::normal: { return "normal"; }
                case NoteheadValue::cluster: { return "cluster"; }
                case NoteheadValue::circleDot: { return "circle dot"; }
                case NoteheadValue::leftTriangle: { return "left triangle"; }
                case NoteheadValue::rectangle: { return "rectangle"; }
                case NoteheadValue::none: { return "none"; }
                case NoteheadValue::do_: { return "do"; }
                case NoteheadValue::re: { return "re"; }
                case NoteheadValue::mi: { return "mi"; }
                case NoteheadValue::fa: { return "fa"; }
                case NoteheadValue::faUp: { return "fa up"; }
                case NoteheadValue::so: { return "so"; }
                case NoteheadValue::la: { return "la"; }
                case NoteheadValue::ti: { return "ti"; }
                default: break;
            }
            return "slash";
        }

        std::ostream& toStream( std::ostream& os, const NoteheadValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const NoteheadValue value )
        {
            return toStream( os, value );
        }

        /// OnOff //////////////////////////////////////////////////////////////////////////////////

        OnOff parseOnOff( const std::string& value )
        {
            const auto opt = tryParseOnOff( value );
            return opt.value_or( OnOff::on );
        }

        std::optional<OnOff> tryParseOnOff( const std::string& value )
        {
            if( value == "on" ) { return OnOff::on; }
            else if( value == "off" ) { return OnOff::off; }
            return std::optional<OnOff>{};
        }

        std::string toString( const OnOff value )
        {
            switch ( value )
            {
                case OnOff::on: { return "on"; }
                case OnOff::off: { return "off"; }
                default: break;
            }
            return "on";
        }

        std::ostream& toStream( std::ostream& os, const OnOff value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const OnOff value )
        {
            return toStream( os, value );
        }

        /// OverUnder //////////////////////////////////////////////////////////////////////////////

        OverUnder parseOverUnder( const std::string& value )
        {
            const auto opt = tryParseOverUnder( value );
            return opt.value_or( OverUnder::over );
        }

        std::optional<OverUnder> tryParseOverUnder( const std::string& value )
        {
            if( value == "over" ) { return OverUnder::over; }
            else if( value == "under" ) { return OverUnder::under; }
            return std::optional<OverUnder>{};
        }

        std::string toString( const OverUnder value )
        {
            switch ( value )
            {
                case OverUnder::over: { return "over"; }
                case OverUnder::under: { return "under"; }
                default: break;
            }
            return "over";
        }

        std::ostream& toStream( std::ostream& os, const OverUnder value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const OverUnder value )
        {
            return toStream( os, value );
        }

        /// PitchedEnum ////////////////////////////////////////////////////////////////////////////

        PitchedEnum parsePitchedEnum( const std::string& value )
        {
            const auto opt = tryParsePitchedEnum( value );
            return opt.value_or( PitchedEnum::chimes );
        }

        std::optional<PitchedEnum> tryParsePitchedEnum( const std::string& value )
        {
            if( value == "chimes" ) { return PitchedEnum::chimes; }
            else if( value == "glockenspiel" ) { return PitchedEnum::glockenspiel; }
            else if( value == "mallet" ) { return PitchedEnum::mallet; }
            else if( value == "marimba" ) { return PitchedEnum::marimba; }
            else if( value == "tubular chimes" ) { return PitchedEnum::tubularChimes; }
            else if( value == "vibraphone" ) { return PitchedEnum::vibraphone; }
            else if( value == "xylophone" ) { return PitchedEnum::xylophone; }
            return std::optional<PitchedEnum>{};
        }

        std::string toString( const PitchedEnum value )
        {
            switch ( value )
            {
                case PitchedEnum::chimes: { return "chimes"; }
                case PitchedEnum::glockenspiel: { return "glockenspiel"; }
                case PitchedEnum::mallet: { return "mallet"; }
                case PitchedEnum::marimba: { return "marimba"; }
                case PitchedEnum::tubularChimes: { return "tubular chimes"; }
                case PitchedEnum::vibraphone: { return "vibraphone"; }
                case PitchedEnum::xylophone: { return "xylophone"; }
                default: break;
            }
            return "chimes";
        }

        std::ostream& toStream( std::ostream& os, const PitchedEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const PitchedEnum value )
        {
            return toStream( os, value );
        }

        /// PrincipalVoiceSymbol ///////////////////////////////////////////////////////////////////

        PrincipalVoiceSymbol parsePrincipalVoiceSymbol( const std::string& value )
        {
            const auto opt = tryParsePrincipalVoiceSymbol( value );
            return opt.value_or( PrincipalVoiceSymbol::hauptstimme );
        }

        std::optional<PrincipalVoiceSymbol> tryParsePrincipalVoiceSymbol( const std::string& value )
        {
            if( value == "Hauptstimme" ) { return PrincipalVoiceSymbol::hauptstimme; }
            else if( value == "Nebenstimme" ) { return PrincipalVoiceSymbol::nebenstimme; }
            else if( value == "plain" ) { return PrincipalVoiceSymbol::plain; }
            else if( value == "none" ) { return PrincipalVoiceSymbol::none; }
            return std::optional<PrincipalVoiceSymbol>{};
        }

        std::string toString( const PrincipalVoiceSymbol value )
        {
            switch ( value )
            {
                case PrincipalVoiceSymbol::hauptstimme: { return "Hauptstimme"; }
                case PrincipalVoiceSymbol::nebenstimme: { return "Nebenstimme"; }
                case PrincipalVoiceSymbol::plain: { return "plain"; }
                case PrincipalVoiceSymbol::none: { return "none"; }
                default: break;
            }
            return "Hauptstimme";
        }

        std::ostream& toStream( std::ostream& os, const PrincipalVoiceSymbol value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const PrincipalVoiceSymbol value )
        {
            return toStream( os, value );
        }

        /// RightLeftMiddle ////////////////////////////////////////////////////////////////////////

        RightLeftMiddle parseRightLeftMiddle( const std::string& value )
        {
            const auto opt = tryParseRightLeftMiddle( value );
            return opt.value_or( RightLeftMiddle::right );
        }

        std::optional<RightLeftMiddle> tryParseRightLeftMiddle( const std::string& value )
        {
            if( value == "right" ) { return RightLeftMiddle::right; }
            else if( value == "left" ) { return RightLeftMiddle::left; }
            else if( value == "middle" ) { return RightLeftMiddle::middle; }
            return std::optional<RightLeftMiddle>{};
        }

        std::string toString( const RightLeftMiddle value )
        {
            switch ( value )
            {
                case RightLeftMiddle::right: { return "right"; }
                case RightLeftMiddle::left: { return "left"; }
                case RightLeftMiddle::middle: { return "middle"; }
                default: break;
            }
            return "right";
        }

        std::ostream& toStream( std::ostream& os, const RightLeftMiddle value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const RightLeftMiddle value )
        {
            return toStream( os, value );
        }

        /// SemiPitchedEnum ////////////////////////////////////////////////////////////////////////

        SemiPitchedEnum parseSemiPitchedEnum( const std::string& value )
        {
            const auto opt = tryParseSemiPitchedEnum( value );
            return opt.value_or( SemiPitchedEnum::high );
        }

        std::optional<SemiPitchedEnum> tryParseSemiPitchedEnum( const std::string& value )
        {
            if( value == "high" ) { return SemiPitchedEnum::high; }
            else if( value == "medium-high" ) { return SemiPitchedEnum::mediumHigh; }
            else if( value == "medium" ) { return SemiPitchedEnum::medium; }
            else if( value == "medium-low" ) { return SemiPitchedEnum::mediumLow; }
            else if( value == "low" ) { return SemiPitchedEnum::low; }
            else if( value == "very-low" ) { return SemiPitchedEnum::veryLow; }
            return std::optional<SemiPitchedEnum>{};
        }

        std::string toString( const SemiPitchedEnum value )
        {
            switch ( value )
            {
                case SemiPitchedEnum::high: { return "high"; }
                case SemiPitchedEnum::mediumHigh: { return "medium-high"; }
                case SemiPitchedEnum::medium: { return "medium"; }
                case SemiPitchedEnum::mediumLow: { return "medium-low"; }
                case SemiPitchedEnum::low: { return "low"; }
                case SemiPitchedEnum::veryLow: { return "very-low"; }
                default: break;
            }
            return "high";
        }

        std::ostream& toStream( std::ostream& os, const SemiPitchedEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const SemiPitchedEnum value )
        {
            return toStream( os, value );
        }

        /// ShowFrets //////////////////////////////////////////////////////////////////////////////

        ShowFrets parseShowFrets( const std::string& value )
        {
            const auto opt = tryParseShowFrets( value );
            return opt.value_or( ShowFrets::numbers );
        }

        std::optional<ShowFrets> tryParseShowFrets( const std::string& value )
        {
            if( value == "numbers" ) { return ShowFrets::numbers; }
            else if( value == "letters" ) { return ShowFrets::letters; }
            return std::optional<ShowFrets>{};
        }

        std::string toString( const ShowFrets value )
        {
            switch ( value )
            {
                case ShowFrets::numbers: { return "numbers"; }
                case ShowFrets::letters: { return "letters"; }
                default: break;
            }
            return "numbers";
        }

        std::ostream& toStream( std::ostream& os, const ShowFrets value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const ShowFrets value )
        {
            return toStream( os, value );
        }

        /// ShowTuplet /////////////////////////////////////////////////////////////////////////////

        ShowTuplet parseShowTuplet( const std::string& value )
        {
            const auto opt = tryParseShowTuplet( value );
            return opt.value_or( ShowTuplet::actual );
        }

        std::optional<ShowTuplet> tryParseShowTuplet( const std::string& value )
        {
            if( value == "actual" ) { return ShowTuplet::actual; }
            else if( value == "both" ) { return ShowTuplet::both; }
            else if( value == "none" ) { return ShowTuplet::none; }
            return std::optional<ShowTuplet>{};
        }

        std::string toString( const ShowTuplet value )
        {
            switch ( value )
            {
                case ShowTuplet::actual: { return "actual"; }
                case ShowTuplet::both: { return "both"; }
                case ShowTuplet::none: { return "none"; }
                default: break;
            }
            return "actual";
        }

        std::ostream& toStream( std::ostream& os, const ShowTuplet value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const ShowTuplet value )
        {
            return toStream( os, value );
        }

        /// StaffTypeEnum //////////////////////////////////////////////////////////////////////////

        StaffTypeEnum parseStaffTypeEnum( const std::string& value )
        {
            const auto opt = tryParseStaffTypeEnum( value );
            return opt.value_or( StaffTypeEnum::ossia );
        }

        std::optional<StaffTypeEnum> tryParseStaffTypeEnum( const std::string& value )
        {
            if( value == "ossia" ) { return StaffTypeEnum::ossia; }
            else if( value == "cue" ) { return StaffTypeEnum::cue; }
            else if( value == "editorial" ) { return StaffTypeEnum::editorial; }
            else if( value == "regular" ) { return StaffTypeEnum::regular; }
            else if( value == "alternate" ) { return StaffTypeEnum::alternate; }
            return std::optional<StaffTypeEnum>{};
        }

        std::string toString( const StaffTypeEnum value )
        {
            switch ( value )
            {
                case StaffTypeEnum::ossia: { return "ossia"; }
                case StaffTypeEnum::cue: { return "cue"; }
                case StaffTypeEnum::editorial: { return "editorial"; }
                case StaffTypeEnum::regular: { return "regular"; }
                case StaffTypeEnum::alternate: { return "alternate"; }
                default: break;
            }
            return "ossia";
        }

        std::ostream& toStream( std::ostream& os, const StaffTypeEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const StaffTypeEnum value )
        {
            return toStream( os, value );
        }

        /// StartNote //////////////////////////////////////////////////////////////////////////////

        StartNote parseStartNote( const std::string& value )
        {
            const auto opt = tryParseStartNote( value );
            return opt.value_or( StartNote::upper );
        }

        std::optional<StartNote> tryParseStartNote( const std::string& value )
        {
            if( value == "upper" ) { return StartNote::upper; }
            else if( value == "main" ) { return StartNote::main; }
            else if( value == "below" ) { return StartNote::below; }
            return std::optional<StartNote>{};
        }

        std::string toString( const StartNote value )
        {
            switch ( value )
            {
                case StartNote::upper: { return "upper"; }
                case StartNote::main: { return "main"; }
                case StartNote::below: { return "below"; }
                default: break;
            }
            return "upper";
        }

        std::ostream& toStream( std::ostream& os, const StartNote value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const StartNote value )
        {
            return toStream( os, value );
        }

        /// StartStop //////////////////////////////////////////////////////////////////////////////

        StartStop parseStartStop( const std::string& value )
        {
            const auto opt = tryParseStartStop( value );
            return opt.value_or( StartStop::start );
        }

        std::optional<StartStop> tryParseStartStop( const std::string& value )
        {
            if( value == "start" ) { return StartStop::start; }
            else if( value == "stop" ) { return StartStop::stop; }
            return std::optional<StartStop>{};
        }

        std::string toString( const StartStop value )
        {
            switch ( value )
            {
                case StartStop::start: { return "start"; }
                case StartStop::stop: { return "stop"; }
                default: break;
            }
            return "start";
        }

        std::ostream& toStream( std::ostream& os, const StartStop value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const StartStop value )
        {
            return toStream( os, value );
        }

        /// StartStopChangeContinue ////////////////////////////////////////////////////////////////

        StartStopChangeContinue parseStartStopChangeContinue( const std::string& value )
        {
            const auto opt = tryParseStartStopChangeContinue( value );
            return opt.value_or( StartStopChangeContinue::start );
        }

        std::optional<StartStopChangeContinue> tryParseStartStopChangeContinue( const std::string& value )
        {
            if( value == "start" ) { return StartStopChangeContinue::start; }
            else if( value == "stop" ) { return StartStopChangeContinue::stop; }
            else if( value == "change" ) { return StartStopChangeContinue::change; }
            else if( value == "continue" ) { return StartStopChangeContinue::continue_; }
            return std::optional<StartStopChangeContinue>{};
        }

        std::string toString( const StartStopChangeContinue value )
        {
            switch ( value )
            {
                case StartStopChangeContinue::start: { return "start"; }
                case StartStopChangeContinue::stop: { return "stop"; }
                case StartStopChangeContinue::change: { return "change"; }
                case StartStopChangeContinue::continue_: { return "continue"; }
                default: break;
            }
            return "start";
        }

        std::ostream& toStream( std::ostream& os, const StartStopChangeContinue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const StartStopChangeContinue value )
        {
            return toStream( os, value );
        }

        /// StartStopContinue //////////////////////////////////////////////////////////////////////

        StartStopContinue parseStartStopContinue( const std::string& value )
        {
            const auto opt = tryParseStartStopContinue( value );
            return opt.value_or( StartStopContinue::start );
        }

        std::optional<StartStopContinue> tryParseStartStopContinue( const std::string& value )
        {
            if( value == "start" ) { return StartStopContinue::start; }
            else if( value == "stop" ) { return StartStopContinue::stop; }
            else if( value == "continue" ) { return StartStopContinue::continue_; }
            return std::optional<StartStopContinue>{};
        }

        std::string toString( const StartStopContinue value )
        {
            switch ( value )
            {
                case StartStopContinue::start: { return "start"; }
                case StartStopContinue::stop: { return "stop"; }
                case StartStopContinue::continue_: { return "continue"; }
                default: break;
            }
            return "start";
        }

        std::ostream& toStream( std::ostream& os, const StartStopContinue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const StartStopContinue value )
        {
            return toStream( os, value );
        }

        /// StartStopDiscontinue ///////////////////////////////////////////////////////////////////

        StartStopDiscontinue parseStartStopDiscontinue( const std::string& value )
        {
            const auto opt = tryParseStartStopDiscontinue( value );
            return opt.value_or( StartStopDiscontinue::start );
        }

        std::optional<StartStopDiscontinue> tryParseStartStopDiscontinue( const std::string& value )
        {
            if( value == "start" ) { return StartStopDiscontinue::start; }
            else if( value == "stop" ) { return StartStopDiscontinue::stop; }
            else if( value == "discontinue" ) { return StartStopDiscontinue::discontinue; }
            return std::optional<StartStopDiscontinue>{};
        }

        std::string toString( const StartStopDiscontinue value )
        {
            switch ( value )
            {
                case StartStopDiscontinue::start: { return "start"; }
                case StartStopDiscontinue::stop: { return "stop"; }
                case StartStopDiscontinue::discontinue: { return "discontinue"; }
                default: break;
            }
            return "start";
        }

        std::ostream& toStream( std::ostream& os, const StartStopDiscontinue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const StartStopDiscontinue value )
        {
            return toStream( os, value );
        }

        /// StartStopSingle ////////////////////////////////////////////////////////////////////////

        StartStopSingle parseStartStopSingle( const std::string& value )
        {
            const auto opt = tryParseStartStopSingle( value );
            return opt.value_or( StartStopSingle::start );
        }

        std::optional<StartStopSingle> tryParseStartStopSingle( const std::string& value )
        {
            if( value == "start" ) { return StartStopSingle::start; }
            else if( value == "stop" ) { return StartStopSingle::stop; }
            else if( value == "single" ) { return StartStopSingle::single; }
            return std::optional<StartStopSingle>{};
        }

        std::string toString( const StartStopSingle value )
        {
            switch ( value )
            {
                case StartStopSingle::start: { return "start"; }
                case StartStopSingle::stop: { return "stop"; }
                case StartStopSingle::single: { return "single"; }
                default: break;
            }
            return "start";
        }

        std::ostream& toStream( std::ostream& os, const StartStopSingle value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const StartStopSingle value )
        {
            return toStream( os, value );
        }

        /// StemValue //////////////////////////////////////////////////////////////////////////////

        StemValue parseStemValue( const std::string& value )
        {
            const auto opt = tryParseStemValue( value );
            return opt.value_or( StemValue::down );
        }

        std::optional<StemValue> tryParseStemValue( const std::string& value )
        {
            if( value == "down" ) { return StemValue::down; }
            else if( value == "up" ) { return StemValue::up; }
            else if( value == "double" ) { return StemValue::double_; }
            else if( value == "none" ) { return StemValue::none; }
            return std::optional<StemValue>{};
        }

        std::string toString( const StemValue value )
        {
            switch ( value )
            {
                case StemValue::down: { return "down"; }
                case StemValue::up: { return "up"; }
                case StemValue::double_: { return "double"; }
                case StemValue::none: { return "none"; }
                default: break;
            }
            return "down";
        }

        std::ostream& toStream( std::ostream& os, const StemValue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const StemValue value )
        {
            return toStream( os, value );
        }

        /// StepEnum ///////////////////////////////////////////////////////////////////////////////

        StepEnum parseStepEnum( const std::string& value )
        {
            const auto opt = tryParseStepEnum( value );
            return opt.value_or( StepEnum::a );
        }

        std::optional<StepEnum> tryParseStepEnum( const std::string& value )
        {
            if( value == "A" ) { return StepEnum::a; }
            else if( value == "B" ) { return StepEnum::b; }
            else if( value == "C" ) { return StepEnum::c; }
            else if( value == "D" ) { return StepEnum::d; }
            else if( value == "E" ) { return StepEnum::e; }
            else if( value == "F" ) { return StepEnum::f; }
            else if( value == "G" ) { return StepEnum::g; }
            return std::optional<StepEnum>{};
        }

        std::string toString( const StepEnum value )
        {
            switch ( value )
            {
                case StepEnum::a: { return "A"; }
                case StepEnum::b: { return "B"; }
                case StepEnum::c: { return "C"; }
                case StepEnum::d: { return "D"; }
                case StepEnum::e: { return "E"; }
                case StepEnum::f: { return "F"; }
                case StepEnum::g: { return "G"; }
                default: break;
            }
            return "A";
        }

        std::ostream& toStream( std::ostream& os, const StepEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const StepEnum value )
        {
            return toStream( os, value );
        }

        /// StickLocationEnum //////////////////////////////////////////////////////////////////////

        StickLocationEnum parseStickLocationEnum( const std::string& value )
        {
            const auto opt = tryParseStickLocationEnum( value );
            return opt.value_or( StickLocationEnum::center );
        }

        std::optional<StickLocationEnum> tryParseStickLocationEnum( const std::string& value )
        {
            if( value == "center" ) { return StickLocationEnum::center; }
            else if( value == "rim" ) { return StickLocationEnum::rim; }
            else if( value == "cymbal bell" ) { return StickLocationEnum::cymbalBell; }
            else if( value == "cymbal edge" ) { return StickLocationEnum::cymbalEdge; }
            return std::optional<StickLocationEnum>{};
        }

        std::string toString( const StickLocationEnum value )
        {
            switch ( value )
            {
                case StickLocationEnum::center: { return "center"; }
                case StickLocationEnum::rim: { return "rim"; }
                case StickLocationEnum::cymbalBell: { return "cymbal bell"; }
                case StickLocationEnum::cymbalEdge: { return "cymbal edge"; }
                default: break;
            }
            return "center";
        }

        std::ostream& toStream( std::ostream& os, const StickLocationEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const StickLocationEnum value )
        {
            return toStream( os, value );
        }

        /// StickMaterialEnum //////////////////////////////////////////////////////////////////////

        StickMaterialEnum parseStickMaterialEnum( const std::string& value )
        {
            const auto opt = tryParseStickMaterialEnum( value );
            return opt.value_or( StickMaterialEnum::soft );
        }

        std::optional<StickMaterialEnum> tryParseStickMaterialEnum( const std::string& value )
        {
            if( value == "soft" ) { return StickMaterialEnum::soft; }
            else if( value == "medium" ) { return StickMaterialEnum::medium; }
            else if( value == "hard" ) { return StickMaterialEnum::hard; }
            else if( value == "shaded" ) { return StickMaterialEnum::shaded; }
            else if( value == "x" ) { return StickMaterialEnum::x; }
            return std::optional<StickMaterialEnum>{};
        }

        std::string toString( const StickMaterialEnum value )
        {
            switch ( value )
            {
                case StickMaterialEnum::soft: { return "soft"; }
                case StickMaterialEnum::medium: { return "medium"; }
                case StickMaterialEnum::hard: { return "hard"; }
                case StickMaterialEnum::shaded: { return "shaded"; }
                case StickMaterialEnum::x: { return "x"; }
                default: break;
            }
            return "soft";
        }

        std::ostream& toStream( std::ostream& os, const StickMaterialEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const StickMaterialEnum value )
        {
            return toStream( os, value );
        }

        /// StickTypeEnum //////////////////////////////////////////////////////////////////////////

        StickTypeEnum parseStickTypeEnum( const std::string& value )
        {
            const auto opt = tryParseStickTypeEnum( value );
            return opt.value_or( StickTypeEnum::bassDrum );
        }

        std::optional<StickTypeEnum> tryParseStickTypeEnum( const std::string& value )
        {
            if( value == "bass drum" ) { return StickTypeEnum::bassDrum; }
            else if( value == "double bass drum" ) { return StickTypeEnum::doubleBassDrum; }
            else if( value == "timpani" ) { return StickTypeEnum::timpani; }
            else if( value == "xylophone" ) { return StickTypeEnum::xylophone; }
            else if( value == "yarn" ) { return StickTypeEnum::yarn; }
            return std::optional<StickTypeEnum>{};
        }

        std::string toString( const StickTypeEnum value )
        {
            switch ( value )
            {
                case StickTypeEnum::bassDrum: { return "bass drum"; }
                case StickTypeEnum::doubleBassDrum: { return "double bass drum"; }
                case StickTypeEnum::timpani: { return "timpani"; }
                case StickTypeEnum::xylophone: { return "xylophone"; }
                case StickTypeEnum::yarn: { return "yarn"; }
                default: break;
            }
            return "bass drum";
        }

        std::ostream& toStream( std::ostream& os, const StickTypeEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const StickTypeEnum value )
        {
            return toStream( os, value );
        }

        /// SyllabicEnum ///////////////////////////////////////////////////////////////////////////

        SyllabicEnum parseSyllabicEnum( const std::string& value )
        {
            const auto opt = tryParseSyllabicEnum( value );
            return opt.value_or( SyllabicEnum::single );
        }

        std::optional<SyllabicEnum> tryParseSyllabicEnum( const std::string& value )
        {
            if( value == "single" ) { return SyllabicEnum::single; }
            else if( value == "begin" ) { return SyllabicEnum::begin; }
            else if( value == "end" ) { return SyllabicEnum::end; }
            else if( value == "middle" ) { return SyllabicEnum::middle; }
            return std::optional<SyllabicEnum>{};
        }

        std::string toString( const SyllabicEnum value )
        {
            switch ( value )
            {
                case SyllabicEnum::single: { return "single"; }
                case SyllabicEnum::begin: { return "begin"; }
                case SyllabicEnum::end: { return "end"; }
                case SyllabicEnum::middle: { return "middle"; }
                default: break;
            }
            return "single";
        }

        std::ostream& toStream( std::ostream& os, const SyllabicEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const SyllabicEnum value )
        {
            return toStream( os, value );
        }

        /// SymbolSize /////////////////////////////////////////////////////////////////////////////

        SymbolSize parseSymbolSize( const std::string& value )
        {
            const auto opt = tryParseSymbolSize( value );
            return opt.value_or( SymbolSize::full );
        }

        std::optional<SymbolSize> tryParseSymbolSize( const std::string& value )
        {
            if( value == "full" ) { return SymbolSize::full; }
            else if( value == "cue" ) { return SymbolSize::cue; }
            else if( value == "large" ) { return SymbolSize::large; }
            return std::optional<SymbolSize>{};
        }

        std::string toString( const SymbolSize value )
        {
            switch ( value )
            {
                case SymbolSize::full: { return "full"; }
                case SymbolSize::cue: { return "cue"; }
                case SymbolSize::large: { return "large"; }
                default: break;
            }
            return "full";
        }

        std::ostream& toStream( std::ostream& os, const SymbolSize value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const SymbolSize value )
        {
            return toStream( os, value );
        }

        /// TextDirection //////////////////////////////////////////////////////////////////////////

        TextDirection parseTextDirection( const std::string& value )
        {
            const auto opt = tryParseTextDirection( value );
            return opt.value_or( TextDirection::ltr );
        }

        std::optional<TextDirection> tryParseTextDirection( const std::string& value )
        {
            if( value == "ltr" ) { return TextDirection::ltr; }
            else if( value == "rtl" ) { return TextDirection::rtl; }
            else if( value == "lro" ) { return TextDirection::lro; }
            else if( value == "rlo" ) { return TextDirection::rlo; }
            return std::optional<TextDirection>{};
        }

        std::string toString( const TextDirection value )
        {
            switch ( value )
            {
                case TextDirection::ltr: { return "ltr"; }
                case TextDirection::rtl: { return "rtl"; }
                case TextDirection::lro: { return "lro"; }
                case TextDirection::rlo: { return "rlo"; }
                default: break;
            }
            return "ltr";
        }

        std::ostream& toStream( std::ostream& os, const TextDirection value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const TextDirection value )
        {
            return toStream( os, value );
        }

        /// TimeRelationEnum ///////////////////////////////////////////////////////////////////////

        TimeRelationEnum parseTimeRelationEnum( const std::string& value )
        {
            const auto opt = tryParseTimeRelationEnum( value );
            return opt.value_or( TimeRelationEnum::parentheses );
        }

        std::optional<TimeRelationEnum> tryParseTimeRelationEnum( const std::string& value )
        {
            if( value == "parentheses" ) { return TimeRelationEnum::parentheses; }
            else if( value == "bracket" ) { return TimeRelationEnum::bracket; }
            else if( value == "equals" ) { return TimeRelationEnum::equals; }
            else if( value == "slash" ) { return TimeRelationEnum::slash; }
            else if( value == "space" ) { return TimeRelationEnum::space; }
            else if( value == "hyphen" ) { return TimeRelationEnum::hyphen; }
            return std::optional<TimeRelationEnum>{};
        }

        std::string toString( const TimeRelationEnum value )
        {
            switch ( value )
            {
                case TimeRelationEnum::parentheses: { return "parentheses"; }
                case TimeRelationEnum::bracket: { return "bracket"; }
                case TimeRelationEnum::equals: { return "equals"; }
                case TimeRelationEnum::slash: { return "slash"; }
                case TimeRelationEnum::space: { return "space"; }
                case TimeRelationEnum::hyphen: { return "hyphen"; }
                default: break;
            }
            return "parentheses";
        }

        std::ostream& toStream( std::ostream& os, const TimeRelationEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const TimeRelationEnum value )
        {
            return toStream( os, value );
        }

        /// TimeSeparator //////////////////////////////////////////////////////////////////////////

        TimeSeparator parseTimeSeparator( const std::string& value )
        {
            const auto opt = tryParseTimeSeparator( value );
            return opt.value_or( TimeSeparator::none );
        }

        std::optional<TimeSeparator> tryParseTimeSeparator( const std::string& value )
        {
            if( value == "none" ) { return TimeSeparator::none; }
            else if( value == "horizontal" ) { return TimeSeparator::horizontal; }
            else if( value == "diagonal" ) { return TimeSeparator::diagonal; }
            else if( value == "vertical" ) { return TimeSeparator::vertical; }
            else if( value == "adjacent" ) { return TimeSeparator::adjacent; }
            return std::optional<TimeSeparator>{};
        }

        std::string toString( const TimeSeparator value )
        {
            switch ( value )
            {
                case TimeSeparator::none: { return "none"; }
                case TimeSeparator::horizontal: { return "horizontal"; }
                case TimeSeparator::diagonal: { return "diagonal"; }
                case TimeSeparator::vertical: { return "vertical"; }
                case TimeSeparator::adjacent: { return "adjacent"; }
                default: break;
            }
            return "none";
        }

        std::ostream& toStream( std::ostream& os, const TimeSeparator value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const TimeSeparator value )
        {
            return toStream( os, value );
        }

        /// TimeSymbol /////////////////////////////////////////////////////////////////////////////

        TimeSymbol parseTimeSymbol( const std::string& value )
        {
            const auto opt = tryParseTimeSymbol( value );
            return opt.value_or( TimeSymbol::common );
        }

        std::optional<TimeSymbol> tryParseTimeSymbol( const std::string& value )
        {
            if( value == "common" ) { return TimeSymbol::common; }
            else if( value == "cut" ) { return TimeSymbol::cut; }
            else if( value == "single-number" ) { return TimeSymbol::singleNumber; }
            else if( value == "note" ) { return TimeSymbol::note; }
            else if( value == "dotted-note" ) { return TimeSymbol::dottedNote; }
            else if( value == "normal" ) { return TimeSymbol::normal; }
            return std::optional<TimeSymbol>{};
        }

        std::string toString( const TimeSymbol value )
        {
            switch ( value )
            {
                case TimeSymbol::common: { return "common"; }
                case TimeSymbol::cut: { return "cut"; }
                case TimeSymbol::singleNumber: { return "single-number"; }
                case TimeSymbol::note: { return "note"; }
                case TimeSymbol::dottedNote: { return "dotted-note"; }
                case TimeSymbol::normal: { return "normal"; }
                default: break;
            }
            return "common";
        }

        std::ostream& toStream( std::ostream& os, const TimeSymbol value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const TimeSymbol value )
        {
            return toStream( os, value );
        }

        /// TipDirection ///////////////////////////////////////////////////////////////////////////

        TipDirection parseTipDirection( const std::string& value )
        {
            const auto opt = tryParseTipDirection( value );
            return opt.value_or( TipDirection::up );
        }

        std::optional<TipDirection> tryParseTipDirection( const std::string& value )
        {
            if( value == "up" ) { return TipDirection::up; }
            else if( value == "down" ) { return TipDirection::down; }
            else if( value == "left" ) { return TipDirection::left; }
            else if( value == "right" ) { return TipDirection::right; }
            else if( value == "northwest" ) { return TipDirection::northwest; }
            else if( value == "northeast" ) { return TipDirection::northeast; }
            else if( value == "southeast" ) { return TipDirection::southeast; }
            else if( value == "southwest" ) { return TipDirection::southwest; }
            return std::optional<TipDirection>{};
        }

        std::string toString( const TipDirection value )
        {
            switch ( value )
            {
                case TipDirection::up: { return "up"; }
                case TipDirection::down: { return "down"; }
                case TipDirection::left: { return "left"; }
                case TipDirection::right: { return "right"; }
                case TipDirection::northwest: { return "northwest"; }
                case TipDirection::northeast: { return "northeast"; }
                case TipDirection::southeast: { return "southeast"; }
                case TipDirection::southwest: { return "southwest"; }
                default: break;
            }
            return "up";
        }

        std::ostream& toStream( std::ostream& os, const TipDirection value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const TipDirection value )
        {
            return toStream( os, value );
        }

        /// TopBottom //////////////////////////////////////////////////////////////////////////////

        TopBottom parseTopBottom( const std::string& value )
        {
            const auto opt = tryParseTopBottom( value );
            return opt.value_or( TopBottom::top );
        }

        std::optional<TopBottom> tryParseTopBottom( const std::string& value )
        {
            if( value == "top" ) { return TopBottom::top; }
            else if( value == "bottom" ) { return TopBottom::bottom; }
            return std::optional<TopBottom>{};
        }

        std::string toString( const TopBottom value )
        {
            switch ( value )
            {
                case TopBottom::top: { return "top"; }
                case TopBottom::bottom: { return "bottom"; }
                default: break;
            }
            return "top";
        }

        std::ostream& toStream( std::ostream& os, const TopBottom value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const TopBottom value )
        {
            return toStream( os, value );
        }

        /// TrillStep //////////////////////////////////////////////////////////////////////////////

        TrillStep parseTrillStep( const std::string& value )
        {
            const auto opt = tryParseTrillStep( value );
            return opt.value_or( TrillStep::whole );
        }

        std::optional<TrillStep> tryParseTrillStep( const std::string& value )
        {
            if( value == "whole" ) { return TrillStep::whole; }
            else if( value == "half" ) { return TrillStep::half; }
            else if( value == "unison" ) { return TrillStep::unison; }
            return std::optional<TrillStep>{};
        }

        std::string toString( const TrillStep value )
        {
            switch ( value )
            {
                case TrillStep::whole: { return "whole"; }
                case TrillStep::half: { return "half"; }
                case TrillStep::unison: { return "unison"; }
                default: break;
            }
            return "whole";
        }

        std::ostream& toStream( std::ostream& os, const TrillStep value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const TrillStep value )
        {
            return toStream( os, value );
        }

        /// TwoNoteTurn ////////////////////////////////////////////////////////////////////////////

        TwoNoteTurn parseTwoNoteTurn( const std::string& value )
        {
            const auto opt = tryParseTwoNoteTurn( value );
            return opt.value_or( TwoNoteTurn::whole );
        }

        std::optional<TwoNoteTurn> tryParseTwoNoteTurn( const std::string& value )
        {
            if( value == "whole" ) { return TwoNoteTurn::whole; }
            else if( value == "half" ) { return TwoNoteTurn::half; }
            else if( value == "none" ) { return TwoNoteTurn::none; }
            return std::optional<TwoNoteTurn>{};
        }

        std::string toString( const TwoNoteTurn value )
        {
            switch ( value )
            {
                case TwoNoteTurn::whole: { return "whole"; }
                case TwoNoteTurn::half: { return "half"; }
                case TwoNoteTurn::none: { return "none"; }
                default: break;
            }
            return "whole";
        }

        std::ostream& toStream( std::ostream& os, const TwoNoteTurn value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const TwoNoteTurn value )
        {
            return toStream( os, value );
        }

        /// UpDown /////////////////////////////////////////////////////////////////////////////////

        UpDown parseUpDown( const std::string& value )
        {
            const auto opt = tryParseUpDown( value );
            return opt.value_or( UpDown::up );
        }

        std::optional<UpDown> tryParseUpDown( const std::string& value )
        {
            if( value == "up" ) { return UpDown::up; }
            else if( value == "down" ) { return UpDown::down; }
            return std::optional<UpDown>{};
        }

        std::string toString( const UpDown value )
        {
            switch ( value )
            {
                case UpDown::up: { return "up"; }
                case UpDown::down: { return "down"; }
                default: break;
            }
            return "up";
        }

        std::ostream& toStream( std::ostream& os, const UpDown value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const UpDown value )
        {
            return toStream( os, value );
        }

        /// UpDownNone /////////////////////////////////////////////////////////////////////////////////

        UpDownNone parseUpDownNone( const std::string& value )
        {
            const auto opt = tryParseUpDownNone( value );
            return opt.value_or( UpDownNone::none );
        }

        std::optional<UpDownNone> tryParseUpDownNone( const std::string& value )
        {
            if( value == "up" ) { return UpDownNone::up; }
            else if( value == "down" ) { return UpDownNone::down; }
            return std::optional<UpDownNone>{};
        }

        std::string toString( const UpDownNone value )
        {
            switch ( value )
            {
                case UpDownNone::up: { return "up"; }
                case UpDownNone::down: { return "down"; }
                default: break;
            }
            return "up";
        }

        std::ostream& toStream( std::ostream& os, const UpDownNone value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const UpDownNone value )
        {
            return toStream( os, value );
        }
    
        /// UpDownStopContinue /////////////////////////////////////////////////////////////////////

        UpDownStopContinue parseUpDownStopContinue( const std::string& value )
        {
            const auto opt = tryParseUpDownStopContinue( value );
            return opt.value_or( UpDownStopContinue::up );
        }

        std::optional<UpDownStopContinue> tryParseUpDownStopContinue( const std::string& value )
        {
            if( value == "up" ) { return UpDownStopContinue::up; }
            else if( value == "down" ) { return UpDownStopContinue::down; }
            else if( value == "stop" ) { return UpDownStopContinue::stop; }
            else if( value == "continue" ) { return UpDownStopContinue::continue_; }
            return std::optional<UpDownStopContinue>{};
        }

        std::string toString( const UpDownStopContinue value )
        {
            switch ( value )
            {
                case UpDownStopContinue::up: { return "up"; }
                case UpDownStopContinue::down: { return "down"; }
                case UpDownStopContinue::stop: { return "stop"; }
                case UpDownStopContinue::continue_: { return "continue"; }
                default: break;
            }
            return "up";
        }

        std::ostream& toStream( std::ostream& os, const UpDownStopContinue value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const UpDownStopContinue value )
        {
            return toStream( os, value );
        }

        /// UprightInverted ////////////////////////////////////////////////////////////////////////

        UprightInverted parseUprightInverted( const std::string& value )
        {
            const auto opt = tryParseUprightInverted( value );
            return opt.value_or( UprightInverted::upright );
        }

        std::optional<UprightInverted> tryParseUprightInverted( const std::string& value )
        {
            if( value == "upright" ) { return UprightInverted::upright; }
            else if( value == "inverted" ) { return UprightInverted::inverted; }
            return std::optional<UprightInverted>{};
        }

        std::string toString( const UprightInverted value )
        {
            switch ( value )
            {
                case UprightInverted::upright: { return "upright"; }
                case UprightInverted::inverted: { return "inverted"; }
                default: break;
            }
            return "upright";
        }

        std::ostream& toStream( std::ostream& os, const UprightInverted value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const UprightInverted value )
        {
            return toStream( os, value );
        }

        /// Valign /////////////////////////////////////////////////////////////////////////////////

        Valign parseValign( const std::string& value )
        {
            const auto opt = tryParseValign( value );
            return opt.value_or( Valign::top );
        }

        std::optional<Valign> tryParseValign( const std::string& value )
        {
            if( value == "top" ) { return Valign::top; }
            else if( value == "middle" ) { return Valign::middle; }
            else if( value == "bottom" ) { return Valign::bottom; }
            else if( value == "baseline" ) { return Valign::baseline; }
            return std::optional<Valign>{};
        }

        std::string toString( const Valign value )
        {
            switch ( value )
            {
                case Valign::top: { return "top"; }
                case Valign::middle: { return "middle"; }
                case Valign::bottom: { return "bottom"; }
                case Valign::baseline: { return "baseline"; }
                default: break;
            }
            return "top";
        }

        std::ostream& toStream( std::ostream& os, const Valign value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const Valign value )
        {
            return toStream( os, value );
        }

        /// ValignImage ////////////////////////////////////////////////////////////////////////////

        ValignImage parseValignImage( const std::string& value )
        {
            const auto opt = tryParseValignImage( value );
            return opt.value_or( ValignImage::top );
        }

        std::optional<ValignImage> tryParseValignImage( const std::string& value )
        {
            if( value == "top" ) { return ValignImage::top; }
            else if( value == "middle" ) { return ValignImage::middle; }
            else if( value == "bottom" ) { return ValignImage::bottom; }
            return std::optional<ValignImage>{};
        }

        std::string toString( const ValignImage value )
        {
            switch ( value )
            {
                case ValignImage::top: { return "top"; }
                case ValignImage::middle: { return "middle"; }
                case ValignImage::bottom: { return "bottom"; }
                default: break;
            }
            return "top";
        }

        std::ostream& toStream( std::ostream& os, const ValignImage value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const ValignImage value )
        {
            return toStream( os, value );
        }

        /// WedgeType //////////////////////////////////////////////////////////////////////////////

        WedgeType parseWedgeType( const std::string& value )
        {
            const auto opt = tryParseWedgeType( value );
            return opt.value_or( WedgeType::crescendo );
        }

        std::optional<WedgeType> tryParseWedgeType( const std::string& value )
        {
            if( value == "crescendo" ) { return WedgeType::crescendo; }
            else if( value == "diminuendo" ) { return WedgeType::diminuendo; }
            else if( value == "stop" ) { return WedgeType::stop; }
            else if( value == "continue" ) { return WedgeType::continue_; }
            return std::optional<WedgeType>{};
        }

        std::string toString( const WedgeType value )
        {
            switch ( value )
            {
                case WedgeType::crescendo: { return "crescendo"; }
                case WedgeType::diminuendo: { return "diminuendo"; }
                case WedgeType::stop: { return "stop"; }
                case WedgeType::continue_: { return "continue"; }
                default: break;
            }
            return "crescendo";
        }

        std::ostream& toStream( std::ostream& os, const WedgeType value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const WedgeType value )
        {
            return toStream( os, value );
        }

        /// Winged /////////////////////////////////////////////////////////////////////////////////

        Winged parseWinged( const std::string& value )
        {
            const auto opt = tryParseWinged( value );
            return opt.value_or( Winged::none );
        }

        std::optional<Winged> tryParseWinged( const std::string& value )
        {
            if( value == "none" ) { return Winged::none; }
            else if( value == "straight" ) { return Winged::straight; }
            else if( value == "curved" ) { return Winged::curved; }
            else if( value == "double-straight" ) { return Winged::doubleStraight; }
            else if( value == "double-curved" ) { return Winged::doubleCurved; }
            return std::optional<Winged>{};
        }

        std::string toString( const Winged value )
        {
            switch ( value )
            {
                case Winged::none: { return "none"; }
                case Winged::straight: { return "straight"; }
                case Winged::curved: { return "curved"; }
                case Winged::doubleStraight: { return "double-straight"; }
                case Winged::doubleCurved: { return "double-curved"; }
                default: break;
            }
            return "none";
        }

        std::ostream& toStream( std::ostream& os, const Winged value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const Winged value )
        {
            return toStream( os, value );
        }

        /// WoodEnum ///////////////////////////////////////////////////////////////////////////////

        WoodEnum parseWoodEnum( const std::string& value )
        {
            const auto opt = tryParseWoodEnum( value );
            return opt.value_or( WoodEnum::boardClapper );
        }

        std::optional<WoodEnum> tryParseWoodEnum( const std::string& value )
        {
            if( value == "board clapper" ) { return WoodEnum::boardClapper; }
            else if( value == "cabasa" ) { return WoodEnum::cabasa; }
            else if( value == "castanets" ) { return WoodEnum::castanets; }
            else if( value == "claves" ) { return WoodEnum::claves; }
            else if( value == "guiro" ) { return WoodEnum::guiro; }
            else if( value == "log drum" ) { return WoodEnum::logDrum; }
            else if( value == "maraca" ) { return WoodEnum::maraca; }
            else if( value == "maracas" ) { return WoodEnum::maracas; }
            else if( value == "ratchet" ) { return WoodEnum::ratchet; }
            else if( value == "sandpaper blocks" ) { return WoodEnum::sandpaperBlocks; }
            else if( value == "slit drum" ) { return WoodEnum::slitDrum; }
            else if( value == "temple block" ) { return WoodEnum::templeBlock; }
            else if( value == "vibraslap" ) { return WoodEnum::vibraslap; }
            else if( value == "wood block" ) { return WoodEnum::woodBlock; }
            return std::optional<WoodEnum>{};
        }

        std::string toString( const WoodEnum value )
        {
            switch ( value )
            {
                case WoodEnum::boardClapper: { return "board clapper"; }
                case WoodEnum::cabasa: { return "cabasa"; }
                case WoodEnum::castanets: { return "castanets"; }
                case WoodEnum::claves: { return "claves"; }
                case WoodEnum::guiro: { return "guiro"; }
                case WoodEnum::logDrum: { return "log drum"; }
                case WoodEnum::maraca: { return "maraca"; }
                case WoodEnum::maracas: { return "maracas"; }
                case WoodEnum::ratchet: { return "ratchet"; }
                case WoodEnum::sandpaperBlocks: { return "sandpaper blocks"; }
                case WoodEnum::slitDrum: { return "slit drum"; }
                case WoodEnum::templeBlock: { return "temple block"; }
                case WoodEnum::vibraslap: { return "vibraslap"; }
                case WoodEnum::woodBlock: { return "wood block"; }
                default: break;
            }
            return "board clapper";
        }

        std::ostream& toStream( std::ostream& os, const WoodEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const WoodEnum value )
        {
            return toStream( os, value );
        }

        /// YesNo //////////////////////////////////////////////////////////////////////////////////

        YesNo parseYesNo( const std::string& value )
        {
            const auto opt = tryParseYesNo( value );
            return opt.value_or( YesNo::yes );
        }

        std::optional<YesNo> tryParseYesNo( const std::string& value )
        {
            if( value == "yes" ) { return YesNo::yes; }
            else if( value == "no" ) { return YesNo::no; }
            return std::optional<YesNo>{};
        }

        std::string toString( const YesNo value )
        {
            switch ( value )
            {
                case YesNo::yes: { return "yes"; }
                case YesNo::no: { return "no"; }
                default: break;
            }
            return "yes";
        }

        std::ostream& toStream( std::ostream& os, const YesNo value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const YesNo value )
        {
            return toStream( os, value );
        }
    }
}
