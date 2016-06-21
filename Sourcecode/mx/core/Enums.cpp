// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/Enums.h"

namespace mx
{
	namespace core
	{
		AboveBelow parseAboveBelow( const std::string& value )
		{
			if ( value == "above" ) { return AboveBelow::above; }
			else if ( value == "below" ) { return AboveBelow::below; }
			return AboveBelow::above;
		}
		std::string toString( const AboveBelow value )
		{
			switch ( value ) 
			{
				case AboveBelow::above: return "above"; break;
				case AboveBelow::below: return "below"; break;
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

		CssFontSize parseCssFontSize( const std::string& value )
		{
			if ( value == "xx-small" ) { return CssFontSize::xxSmall; }
			else if ( value == "x-small" ) { return CssFontSize::xSmall; }
			else if ( value == "small" ) { return CssFontSize::small; }
			else if ( value == "medium" ) { return CssFontSize::medium; }
			else if ( value == "large" ) { return CssFontSize::large; }
			else if ( value == "x-large" ) { return CssFontSize::xLarge; }
			else if ( value == "xx-large" ) { return CssFontSize::xxLarge; }
			return CssFontSize::xxSmall;
		}
		std::string toString( const CssFontSize value )
		{
			switch ( value ) 
			{
				case CssFontSize::xxSmall: return "xx-small"; break;
				case CssFontSize::xSmall: return "x-small"; break;
				case CssFontSize::small: return "small"; break;
				case CssFontSize::medium: return "medium"; break;
				case CssFontSize::large: return "large"; break;
				case CssFontSize::xLarge: return "x-large"; break;
				case CssFontSize::xxLarge: return "xx-large"; break;
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

		EnclosureShape parseEnclosureShape( const std::string& value )
		{
			if ( value == "rectangle" ) { return EnclosureShape::rectangle; }
			else if ( value == "square" ) { return EnclosureShape::square; }
			else if ( value == "oval" ) { return EnclosureShape::oval; }
			else if ( value == "circle" ) { return EnclosureShape::circle; }
			else if ( value == "bracket" ) { return EnclosureShape::bracket; }
			else if ( value == "triangle" ) { return EnclosureShape::triangle; }
			else if ( value == "diamond" ) { return EnclosureShape::diamond; }
			else if ( value == "none" ) { return EnclosureShape::none; }
			return EnclosureShape::rectangle;
		}
		std::string toString( const EnclosureShape value )
		{
			switch ( value ) 
			{
				case EnclosureShape::rectangle: return "rectangle"; break;
				case EnclosureShape::square: return "square"; break;
				case EnclosureShape::oval: return "oval"; break;
				case EnclosureShape::circle: return "circle"; break;
				case EnclosureShape::bracket: return "bracket"; break;
				case EnclosureShape::triangle: return "triangle"; break;
				case EnclosureShape::diamond: return "diamond"; break;
				case EnclosureShape::none: return "none"; break;
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

		FermataShape parseFermataShape( const std::string& value )
		{
			if ( value == "normal" ) { return FermataShape::normal; }
			else if ( value == "angled" ) { return FermataShape::angled; }
			else if ( value == "square" ) { return FermataShape::square; }
			else if ( value == "" ) { return FermataShape::emptystring; }
			return FermataShape::normal;
		}
		std::string toString( const FermataShape value )
		{
			switch ( value ) 
			{
				case FermataShape::normal: return "normal"; break;
				case FermataShape::angled: return "angled"; break;
				case FermataShape::square: return "square"; break;
				case FermataShape::emptystring: return ""; break;
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

		FontStyle parseFontStyle( const std::string& value )
		{
			if ( value == "normal" ) { return FontStyle::normal; }
			else if ( value == "italic" ) { return FontStyle::italic; }
			return FontStyle::normal;
		}
		std::string toString( const FontStyle value )
		{
			switch ( value ) 
			{
				case FontStyle::normal: return "normal"; break;
				case FontStyle::italic: return "italic"; break;
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

		FontWeight parseFontWeight( const std::string& value )
		{
			if ( value == "normal" ) { return FontWeight::normal; }
			else if ( value == "bold" ) { return FontWeight::bold; }
			return FontWeight::normal;
		}
		std::string toString( const FontWeight value )
		{
			switch ( value ) 
			{
				case FontWeight::normal: return "normal"; break;
				case FontWeight::bold: return "bold"; break;
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

		LeftCenterRight parseLeftCenterRight( const std::string& value )
		{
			if ( value == "left" ) { return LeftCenterRight::left; }
			else if ( value == "center" ) { return LeftCenterRight::center; }
			else if ( value == "right" ) { return LeftCenterRight::right; }
			return LeftCenterRight::left;
		}
		std::string toString( const LeftCenterRight value )
		{
			switch ( value ) 
			{
				case LeftCenterRight::left: return "left"; break;
				case LeftCenterRight::center: return "center"; break;
				case LeftCenterRight::right: return "right"; break;
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

		LeftRight parseLeftRight( const std::string& value )
		{
			if ( value == "left" ) { return LeftRight::left; }
			else if ( value == "right" ) { return LeftRight::right; }
			return LeftRight::left;
		}
		std::string toString( const LeftRight value )
		{
			switch ( value ) 
			{
				case LeftRight::left: return "left"; break;
				case LeftRight::right: return "right"; break;
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

		LineShape parseLineShape( const std::string& value )
		{
			if ( value == "straight" ) { return LineShape::straight; }
			else if ( value == "curved" ) { return LineShape::curved; }
			return LineShape::straight;
		}
		std::string toString( const LineShape value )
		{
			switch ( value ) 
			{
				case LineShape::straight: return "straight"; break;
				case LineShape::curved: return "curved"; break;
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

		LineType parseLineType( const std::string& value )
		{
			if ( value == "solid" ) { return LineType::solid; }
			else if ( value == "dashed" ) { return LineType::dashed; }
			else if ( value == "dotted" ) { return LineType::dotted; }
			else if ( value == "wavy" ) { return LineType::wavy; }
			return LineType::solid;
		}
		std::string toString( const LineType value )
		{
			switch ( value ) 
			{
				case LineType::solid: return "solid"; break;
				case LineType::dashed: return "dashed"; break;
				case LineType::dotted: return "dotted"; break;
				case LineType::wavy: return "wavy"; break;
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

		MuteEnum parseMuteEnum( const std::string& value )
		{
			if ( value == "on" ) { return MuteEnum::on; }
			else if ( value == "off" ) { return MuteEnum::off; }
			else if ( value == "straight" ) { return MuteEnum::straight; }
			else if ( value == "cup" ) { return MuteEnum::cup; }
			else if ( value == "harmon-no-stem" ) { return MuteEnum::harmonNoStem; }
			else if ( value == "harmon-stem" ) { return MuteEnum::harmonStem; }
			else if ( value == "bucket" ) { return MuteEnum::bucket; }
			else if ( value == "plunger" ) { return MuteEnum::plunger; }
			else if ( value == "hat" ) { return MuteEnum::hat; }
			else if ( value == "solotone" ) { return MuteEnum::solotone; }
			else if ( value == "practice" ) { return MuteEnum::practice; }
			else if ( value == "stop-mute" ) { return MuteEnum::stopMute; }
			else if ( value == "stop-hand" ) { return MuteEnum::stopHand; }
			else if ( value == "echo" ) { return MuteEnum::echo; }
			else if ( value == "palm" ) { return MuteEnum::palm; }
			return MuteEnum::on;
		}
		std::string toString( const MuteEnum value )
		{
			switch ( value ) 
			{
				case MuteEnum::on: return "on"; break;
				case MuteEnum::off: return "off"; break;
				case MuteEnum::straight: return "straight"; break;
				case MuteEnum::cup: return "cup"; break;
				case MuteEnum::harmonNoStem: return "harmon-no-stem"; break;
				case MuteEnum::harmonStem: return "harmon-stem"; break;
				case MuteEnum::bucket: return "bucket"; break;
				case MuteEnum::plunger: return "plunger"; break;
				case MuteEnum::hat: return "hat"; break;
				case MuteEnum::solotone: return "solotone"; break;
				case MuteEnum::practice: return "practice"; break;
				case MuteEnum::stopMute: return "stop-mute"; break;
				case MuteEnum::stopHand: return "stop-hand"; break;
				case MuteEnum::echo: return "echo"; break;
				case MuteEnum::palm: return "palm"; break;
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

		OverUnder parseOverUnder( const std::string& value )
		{
			if ( value == "over" ) { return OverUnder::over; }
			else if ( value == "under" ) { return OverUnder::under; }
			return OverUnder::over;
		}
		std::string toString( const OverUnder value )
		{
			switch ( value ) 
			{
				case OverUnder::over: return "over"; break;
				case OverUnder::under: return "under"; break;
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

		SemiPitchedEnum parseSemiPitchedEnum( const std::string& value )
		{
			if ( value == "high" ) { return SemiPitchedEnum::high; }
			else if ( value == "medium-high" ) { return SemiPitchedEnum::mediumHigh; }
			else if ( value == "medium" ) { return SemiPitchedEnum::medium; }
			else if ( value == "medium-low" ) { return SemiPitchedEnum::mediumLow; }
			else if ( value == "low" ) { return SemiPitchedEnum::low; }
			else if ( value == "very-low" ) { return SemiPitchedEnum::veryLow; }
			return SemiPitchedEnum::high;
		}
		std::string toString( const SemiPitchedEnum value )
		{
			switch ( value ) 
			{
				case SemiPitchedEnum::high: return "high"; break;
				case SemiPitchedEnum::mediumHigh: return "medium-high"; break;
				case SemiPitchedEnum::medium: return "medium"; break;
				case SemiPitchedEnum::mediumLow: return "medium-low"; break;
				case SemiPitchedEnum::low: return "low"; break;
				case SemiPitchedEnum::veryLow: return "very-low"; break;
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

		StartNote parseStartNote( const std::string& value )
		{
			if ( value == "upper" ) { return StartNote::upper; }
			else if ( value == "main" ) { return StartNote::main; }
			else if ( value == "below" ) { return StartNote::below; }
			return StartNote::upper;
		}
		std::string toString( const StartNote value )
		{
			switch ( value ) 
			{
				case StartNote::upper: return "upper"; break;
				case StartNote::main: return "main"; break;
				case StartNote::below: return "below"; break;
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

		StartStop parseStartStop( const std::string& value )
		{
			if ( value == "start" ) { return StartStop::start; }
			else if ( value == "stop" ) { return StartStop::stop; }
			return StartStop::start;
		}
		std::string toString( const StartStop value )
		{
			switch ( value ) 
			{
				case StartStop::start: return "start"; break;
				case StartStop::stop: return "stop"; break;
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

		StartStopContinue parseStartStopContinue( const std::string& value )
		{
			if ( value == "start" ) { return StartStopContinue::start; }
			else if ( value == "stop" ) { return StartStopContinue::stop; }
			else if ( value == "continue" ) { return StartStopContinue::continue_; }
			return StartStopContinue::start;
		}
		std::string toString( const StartStopContinue value )
		{
			switch ( value ) 
			{
				case StartStopContinue::start: return "start"; break;
				case StartStopContinue::stop: return "stop"; break;
				case StartStopContinue::continue_: return "continue"; break;
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

		StartStopSingle parseStartStopSingle( const std::string& value )
		{
			if ( value == "start" ) { return StartStopSingle::start; }
			else if ( value == "stop" ) { return StartStopSingle::stop; }
			else if ( value == "single" ) { return StartStopSingle::single; }
			return StartStopSingle::start;
		}
		std::string toString( const StartStopSingle value )
		{
			switch ( value ) 
			{
				case StartStopSingle::start: return "start"; break;
				case StartStopSingle::stop: return "stop"; break;
				case StartStopSingle::single: return "single"; break;
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

		SymbolSize parseSymbolSize( const std::string& value )
		{
			if ( value == "full" ) { return SymbolSize::full; }
			else if ( value == "cue" ) { return SymbolSize::cue; }
			else if ( value == "large" ) { return SymbolSize::large; }
			return SymbolSize::full;
		}
		std::string toString( const SymbolSize value )
		{
			switch ( value ) 
			{
				case SymbolSize::full: return "full"; break;
				case SymbolSize::cue: return "cue"; break;
				case SymbolSize::large: return "large"; break;
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

		TextDirection parseTextDirection( const std::string& value )
		{
			if ( value == "ltr" ) { return TextDirection::ltr; }
			else if ( value == "rtl" ) { return TextDirection::rtl; }
			else if ( value == "lro" ) { return TextDirection::lro; }
			else if ( value == "rlo" ) { return TextDirection::rlo; }
			return TextDirection::ltr;
		}
		std::string toString( const TextDirection value )
		{
			switch ( value ) 
			{
				case TextDirection::ltr: return "ltr"; break;
				case TextDirection::rtl: return "rtl"; break;
				case TextDirection::lro: return "lro"; break;
				case TextDirection::rlo: return "rlo"; break;
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

		TopBottom parseTopBottom( const std::string& value )
		{
			if ( value == "top" ) { return TopBottom::top; }
			else if ( value == "bottom" ) { return TopBottom::bottom; }
			return TopBottom::top;
		}
		std::string toString( const TopBottom value )
		{
			switch ( value ) 
			{
				case TopBottom::top: return "top"; break;
				case TopBottom::bottom: return "bottom"; break;
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

		TrillStep parseTrillStep( const std::string& value )
		{
			if ( value == "whole" ) { return TrillStep::whole; }
			else if ( value == "half" ) { return TrillStep::half; }
			else if ( value == "unison" ) { return TrillStep::unison; }
			return TrillStep::whole;
		}
		std::string toString( const TrillStep value )
		{
			switch ( value ) 
			{
				case TrillStep::whole: return "whole"; break;
				case TrillStep::half: return "half"; break;
				case TrillStep::unison: return "unison"; break;
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

		TwoNoteTurn parseTwoNoteTurn( const std::string& value )
		{
			if ( value == "whole" ) { return TwoNoteTurn::whole; }
			else if ( value == "half" ) { return TwoNoteTurn::half; }
			else if ( value == "none" ) { return TwoNoteTurn::none; }
			return TwoNoteTurn::whole;
		}
		std::string toString( const TwoNoteTurn value )
		{
			switch ( value ) 
			{
				case TwoNoteTurn::whole: return "whole"; break;
				case TwoNoteTurn::half: return "half"; break;
				case TwoNoteTurn::none: return "none"; break;
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

		UpDown parseUpDown( const std::string& value )
		{
			if ( value == "up" ) { return UpDown::up; }
			else if ( value == "down" ) { return UpDown::down; }
			return UpDown::up;
		}
		std::string toString( const UpDown value )
		{
			switch ( value ) 
			{
				case UpDown::up: return "up"; break;
				case UpDown::down: return "down"; break;
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

		UprightInverted parseUprightInverted( const std::string& value )
		{
			if ( value == "upright" ) { return UprightInverted::upright; }
			else if ( value == "inverted" ) { return UprightInverted::inverted; }
			return UprightInverted::upright;
		}
		std::string toString( const UprightInverted value )
		{
			switch ( value ) 
			{
				case UprightInverted::upright: return "upright"; break;
				case UprightInverted::inverted: return "inverted"; break;
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

		Valign parseValign( const std::string& value )
		{
			if ( value == "top" ) { return Valign::top; }
			else if ( value == "middle" ) { return Valign::middle; }
			else if ( value == "bottom" ) { return Valign::bottom; }
			else if ( value == "baseline" ) { return Valign::baseline; }
			return Valign::top;
		}
		std::string toString( const Valign value )
		{
			switch ( value ) 
			{
				case Valign::top: return "top"; break;
				case Valign::middle: return "middle"; break;
				case Valign::bottom: return "bottom"; break;
				case Valign::baseline: return "baseline"; break;
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

		ValignImage parseValignImage( const std::string& value )
		{
			if ( value == "top" ) { return ValignImage::top; }
			else if ( value == "middle" ) { return ValignImage::middle; }
			else if ( value == "bottom" ) { return ValignImage::bottom; }
			return ValignImage::top;
		}
		std::string toString( const ValignImage value )
		{
			switch ( value ) 
			{
				case ValignImage::top: return "top"; break;
				case ValignImage::middle: return "middle"; break;
				case ValignImage::bottom: return "bottom"; break;
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

		YesNo parseYesNo( const std::string& value )
		{
			if ( value == "yes" ) { return YesNo::yes; }
			else if ( value == "no" ) { return YesNo::no; }
			return YesNo::yes;
		}
		std::string toString( const YesNo value )
		{
			switch ( value ) 
			{
				case YesNo::yes: return "yes"; break;
				case YesNo::no: return "no"; break;
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

		CancelLocation parseCancelLocation( const std::string& value )
		{
			if ( value == "left" ) { return CancelLocation::left; }
			else if ( value == "right" ) { return CancelLocation::right; }
			else if ( value == "before-barline" ) { return CancelLocation::beforeBarline; }
			return CancelLocation::left;
		}
		std::string toString( const CancelLocation value )
		{
			switch ( value ) 
			{
				case CancelLocation::left: return "left"; break;
				case CancelLocation::right: return "right"; break;
				case CancelLocation::beforeBarline: return "before-barline"; break;
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

		ClefSign parseClefSign( const std::string& value )
		{
			if ( value == "G" ) { return ClefSign::g; }
			else if ( value == "F" ) { return ClefSign::f; }
			else if ( value == "C" ) { return ClefSign::c; }
			else if ( value == "percussion" ) { return ClefSign::percussion; }
			else if ( value == "TAB" ) { return ClefSign::tab; }
			else if ( value == "jianpu" ) { return ClefSign::jianpu; }
			else if ( value == "none" ) { return ClefSign::none; }
			return ClefSign::g;
		}
		std::string toString( const ClefSign value )
		{
			switch ( value ) 
			{
				case ClefSign::g: return "G"; break;
				case ClefSign::f: return "F"; break;
				case ClefSign::c: return "C"; break;
				case ClefSign::percussion: return "percussion"; break;
				case ClefSign::tab: return "TAB"; break;
				case ClefSign::jianpu: return "jianpu"; break;
				case ClefSign::none: return "none"; break;
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

		ShowFrets parseShowFrets( const std::string& value )
		{
			if ( value == "numbers" ) { return ShowFrets::numbers; }
			else if ( value == "letters" ) { return ShowFrets::letters; }
			return ShowFrets::numbers;
		}
		std::string toString( const ShowFrets value )
		{
			switch ( value ) 
			{
				case ShowFrets::numbers: return "numbers"; break;
				case ShowFrets::letters: return "letters"; break;
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

		StaffTypeEnum parseStaffTypeEnum( const std::string& value )
		{
			if ( value == "ossia" ) { return StaffTypeEnum::ossia; }
			else if ( value == "cue" ) { return StaffTypeEnum::cue; }
			else if ( value == "editorial" ) { return StaffTypeEnum::editorial; }
			else if ( value == "regular" ) { return StaffTypeEnum::regular; }
			else if ( value == "alternate" ) { return StaffTypeEnum::alternate; }
			return StaffTypeEnum::ossia;
		}
		std::string toString( const StaffTypeEnum value )
		{
			switch ( value ) 
			{
				case StaffTypeEnum::ossia: return "ossia"; break;
				case StaffTypeEnum::cue: return "cue"; break;
				case StaffTypeEnum::editorial: return "editorial"; break;
				case StaffTypeEnum::regular: return "regular"; break;
				case StaffTypeEnum::alternate: return "alternate"; break;
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

		TimeRelationEnum parseTimeRelationEnum( const std::string& value )
		{
			if ( value == "parentheses" ) { return TimeRelationEnum::parentheses; }
			else if ( value == "bracket" ) { return TimeRelationEnum::bracket; }
			else if ( value == "equals" ) { return TimeRelationEnum::equals; }
			else if ( value == "slash" ) { return TimeRelationEnum::slash; }
			else if ( value == "space" ) { return TimeRelationEnum::space; }
			else if ( value == "hyphen" ) { return TimeRelationEnum::hyphen; }
			return TimeRelationEnum::parentheses;
		}
		std::string toString( const TimeRelationEnum value )
		{
			switch ( value ) 
			{
				case TimeRelationEnum::parentheses: return "parentheses"; break;
				case TimeRelationEnum::bracket: return "bracket"; break;
				case TimeRelationEnum::equals: return "equals"; break;
				case TimeRelationEnum::slash: return "slash"; break;
				case TimeRelationEnum::space: return "space"; break;
				case TimeRelationEnum::hyphen: return "hyphen"; break;
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

		TimeSeparator parseTimeSeparator( const std::string& value )
		{
			if ( value == "none" ) { return TimeSeparator::none; }
			else if ( value == "horizontal" ) { return TimeSeparator::horizontal; }
			else if ( value == "diagonal" ) { return TimeSeparator::diagonal; }
			else if ( value == "vertical" ) { return TimeSeparator::vertical; }
			else if ( value == "adjacent" ) { return TimeSeparator::adjacent; }
			return TimeSeparator::none;
		}
		std::string toString( const TimeSeparator value )
		{
			switch ( value ) 
			{
				case TimeSeparator::none: return "none"; break;
				case TimeSeparator::horizontal: return "horizontal"; break;
				case TimeSeparator::diagonal: return "diagonal"; break;
				case TimeSeparator::vertical: return "vertical"; break;
				case TimeSeparator::adjacent: return "adjacent"; break;
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

		TimeSymbol parseTimeSymbol( const std::string& value )
		{
			if ( value == "common" ) { return TimeSymbol::common; }
			else if ( value == "cut" ) { return TimeSymbol::cut; }
			else if ( value == "single-number" ) { return TimeSymbol::singleNumber; }
			else if ( value == "note" ) { return TimeSymbol::note; }
			else if ( value == "dotted-note" ) { return TimeSymbol::dottedNote; }
			else if ( value == "normal" ) { return TimeSymbol::normal; }
			return TimeSymbol::common;
		}
		std::string toString( const TimeSymbol value )
		{
			switch ( value ) 
			{
				case TimeSymbol::common: return "common"; break;
				case TimeSymbol::cut: return "cut"; break;
				case TimeSymbol::singleNumber: return "single-number"; break;
				case TimeSymbol::note: return "note"; break;
				case TimeSymbol::dottedNote: return "dotted-note"; break;
				case TimeSymbol::normal: return "normal"; break;
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

		BackwardForward parseBackwardForward( const std::string& value )
		{
			if ( value == "backward" ) { return BackwardForward::backward; }
			else if ( value == "forward" ) { return BackwardForward::forward; }
			return BackwardForward::backward;
		}
		std::string toString( const BackwardForward value )
		{
			switch ( value ) 
			{
				case BackwardForward::backward: return "backward"; break;
				case BackwardForward::forward: return "forward"; break;
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

		BarStyleEnum parseBarStyleEnum( const std::string& value )
		{
			if ( value == "regular" ) { return BarStyleEnum::regular; }
			else if ( value == "dotted" ) { return BarStyleEnum::dotted; }
			else if ( value == "dashed" ) { return BarStyleEnum::dashed; }
			else if ( value == "heavy" ) { return BarStyleEnum::heavy; }
			else if ( value == "light-light" ) { return BarStyleEnum::lightLight; }
			else if ( value == "light-heavy" ) { return BarStyleEnum::lightHeavy; }
			else if ( value == "heavy-light" ) { return BarStyleEnum::heavyLight; }
			else if ( value == "heavy-heavy" ) { return BarStyleEnum::heavyHeavy; }
			else if ( value == "tick" ) { return BarStyleEnum::tick; }
			else if ( value == "short" ) { return BarStyleEnum::short_; }
			else if ( value == "none" ) { return BarStyleEnum::none; }
			return BarStyleEnum::regular;
		}
		std::string toString( const BarStyleEnum value )
		{
			switch ( value ) 
			{
				case BarStyleEnum::regular: return "regular"; break;
				case BarStyleEnum::dotted: return "dotted"; break;
				case BarStyleEnum::dashed: return "dashed"; break;
				case BarStyleEnum::heavy: return "heavy"; break;
				case BarStyleEnum::lightLight: return "light-light"; break;
				case BarStyleEnum::lightHeavy: return "light-heavy"; break;
				case BarStyleEnum::heavyLight: return "heavy-light"; break;
				case BarStyleEnum::heavyHeavy: return "heavy-heavy"; break;
				case BarStyleEnum::tick: return "tick"; break;
				case BarStyleEnum::short_: return "short"; break;
				case BarStyleEnum::none: return "none"; break;
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

		RightLeftMiddle parseRightLeftMiddle( const std::string& value )
		{
			if ( value == "right" ) { return RightLeftMiddle::right; }
			else if ( value == "left" ) { return RightLeftMiddle::left; }
			else if ( value == "middle" ) { return RightLeftMiddle::middle; }
			return RightLeftMiddle::right;
		}
		std::string toString( const RightLeftMiddle value )
		{
			switch ( value ) 
			{
				case RightLeftMiddle::right: return "right"; break;
				case RightLeftMiddle::left: return "left"; break;
				case RightLeftMiddle::middle: return "middle"; break;
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

		StartStopDiscontinue parseStartStopDiscontinue( const std::string& value )
		{
			if ( value == "start" ) { return StartStopDiscontinue::start; }
			else if ( value == "stop" ) { return StartStopDiscontinue::stop; }
			else if ( value == "discontinue" ) { return StartStopDiscontinue::discontinue; }
			return StartStopDiscontinue::start;
		}
		std::string toString( const StartStopDiscontinue value )
		{
			switch ( value ) 
			{
				case StartStopDiscontinue::start: return "start"; break;
				case StartStopDiscontinue::stop: return "stop"; break;
				case StartStopDiscontinue::discontinue: return "discontinue"; break;
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

		Winged parseWinged( const std::string& value )
		{
			if ( value == "none" ) { return Winged::none; }
			else if ( value == "straight" ) { return Winged::straight; }
			else if ( value == "curved" ) { return Winged::curved; }
			else if ( value == "double-straight" ) { return Winged::doubleStraight; }
			else if ( value == "double-curved" ) { return Winged::doubleCurved; }
			return Winged::none;
		}
		std::string toString( const Winged value )
		{
			switch ( value ) 
			{
				case Winged::none: return "none"; break;
				case Winged::straight: return "straight"; break;
				case Winged::curved: return "curved"; break;
				case Winged::doubleStraight: return "double-straight"; break;
				case Winged::doubleCurved: return "double-curved"; break;
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

		BeaterValue parseBeaterValue( const std::string& value )
		{
			if ( value == "bow" ) { return BeaterValue::bow; }
			else if ( value == "chime hammer" ) { return BeaterValue::chimeHammer; }
			else if ( value == "coin" ) { return BeaterValue::coin; }
			else if ( value == "finger" ) { return BeaterValue::finger; }
			else if ( value == "fingernail" ) { return BeaterValue::fingernail; }
			else if ( value == "fist" ) { return BeaterValue::fist; }
			else if ( value == "guiro scraper" ) { return BeaterValue::guiroScraper; }
			else if ( value == "hammer" ) { return BeaterValue::hammer; }
			else if ( value == "hand" ) { return BeaterValue::hand; }
			else if ( value == "jazz stick" ) { return BeaterValue::jazzStick; }
			else if ( value == "knitting needle" ) { return BeaterValue::knittingNeedle; }
			else if ( value == "metal hammer" ) { return BeaterValue::metalHammer; }
			else if ( value == "snare stick" ) { return BeaterValue::snareStick; }
			else if ( value == "spoon mallet" ) { return BeaterValue::spoonMallet; }
			else if ( value == "triangle beater" ) { return BeaterValue::triangleBeater; }
			else if ( value == "triangle beater plain" ) { return BeaterValue::triangleBeaterPlain; }
			else if ( value == "wire brush" ) { return BeaterValue::wireBrush; }
			return BeaterValue::bow;
		}
		std::string toString( const BeaterValue value )
		{
			switch ( value ) 
			{
				case BeaterValue::bow: return "bow"; break;
				case BeaterValue::chimeHammer: return "chime hammer"; break;
				case BeaterValue::coin: return "coin"; break;
				case BeaterValue::finger: return "finger"; break;
				case BeaterValue::fingernail: return "fingernail"; break;
				case BeaterValue::fist: return "fist"; break;
				case BeaterValue::guiroScraper: return "guiro scraper"; break;
				case BeaterValue::hammer: return "hammer"; break;
				case BeaterValue::hand: return "hand"; break;
				case BeaterValue::jazzStick: return "jazz stick"; break;
				case BeaterValue::knittingNeedle: return "knitting needle"; break;
				case BeaterValue::metalHammer: return "metal hammer"; break;
				case BeaterValue::snareStick: return "snare stick"; break;
				case BeaterValue::spoonMallet: return "spoon mallet"; break;
				case BeaterValue::triangleBeater: return "triangle beater"; break;
				case BeaterValue::triangleBeaterPlain: return "triangle beater plain"; break;
				case BeaterValue::wireBrush: return "wire brush"; break;
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

		DegreeSymbolValue parseDegreeSymbolValue( const std::string& value )
		{
			if ( value == "major" ) { return DegreeSymbolValue::major; }
			else if ( value == "minor" ) { return DegreeSymbolValue::minor; }
			else if ( value == "augmented" ) { return DegreeSymbolValue::augmented; }
			else if ( value == "diminished" ) { return DegreeSymbolValue::diminished; }
			else if ( value == "half-diminished" ) { return DegreeSymbolValue::halfDiminished; }
			return DegreeSymbolValue::major;
		}
		std::string toString( const DegreeSymbolValue value )
		{
			switch ( value ) 
			{
				case DegreeSymbolValue::major: return "major"; break;
				case DegreeSymbolValue::minor: return "minor"; break;
				case DegreeSymbolValue::augmented: return "augmented"; break;
				case DegreeSymbolValue::diminished: return "diminished"; break;
				case DegreeSymbolValue::halfDiminished: return "half-diminished"; break;
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

		DegreeTypeValue parseDegreeTypeValue( const std::string& value )
		{
			if ( value == "add" ) { return DegreeTypeValue::add; }
			else if ( value == "alter" ) { return DegreeTypeValue::alter; }
			else if ( value == "subtract" ) { return DegreeTypeValue::subtract; }
			return DegreeTypeValue::add;
		}
		std::string toString( const DegreeTypeValue value )
		{
			switch ( value ) 
			{
				case DegreeTypeValue::add: return "add"; break;
				case DegreeTypeValue::alter: return "alter"; break;
				case DegreeTypeValue::subtract: return "subtract"; break;
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

		EffectEnum parseEffectEnum( const std::string& value )
		{
			if ( value == "anvil" ) { return EffectEnum::anvil; }
			else if ( value == "auto horn" ) { return EffectEnum::autoHorn; }
			else if ( value == "bird whistle" ) { return EffectEnum::birdWhistle; }
			else if ( value == "cannon" ) { return EffectEnum::cannon; }
			else if ( value == "duck call" ) { return EffectEnum::duckCall; }
			else if ( value == "gun shot" ) { return EffectEnum::gunShot; }
			else if ( value == "klaxon horn" ) { return EffectEnum::klaxonHorn; }
			else if ( value == "lions roar" ) { return EffectEnum::lionsRoar; }
			else if ( value == "police whistle" ) { return EffectEnum::policeWhistle; }
			else if ( value == "siren" ) { return EffectEnum::siren; }
			else if ( value == "slide whistle" ) { return EffectEnum::slideWhistle; }
			else if ( value == "thunder sheet" ) { return EffectEnum::thunderSheet; }
			else if ( value == "wind machine" ) { return EffectEnum::windMachine; }
			else if ( value == "wind whistle" ) { return EffectEnum::windWhistle; }
			return EffectEnum::anvil;
		}
		std::string toString( const EffectEnum value )
		{
			switch ( value ) 
			{
				case EffectEnum::anvil: return "anvil"; break;
				case EffectEnum::autoHorn: return "auto horn"; break;
				case EffectEnum::birdWhistle: return "bird whistle"; break;
				case EffectEnum::cannon: return "cannon"; break;
				case EffectEnum::duckCall: return "duck call"; break;
				case EffectEnum::gunShot: return "gun shot"; break;
				case EffectEnum::klaxonHorn: return "klaxon horn"; break;
				case EffectEnum::lionsRoar: return "lions roar"; break;
				case EffectEnum::policeWhistle: return "police whistle"; break;
				case EffectEnum::siren: return "siren"; break;
				case EffectEnum::slideWhistle: return "slide whistle"; break;
				case EffectEnum::thunderSheet: return "thunder sheet"; break;
				case EffectEnum::windMachine: return "wind machine"; break;
				case EffectEnum::windWhistle: return "wind whistle"; break;
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

		GlassEnum parseGlassEnum( const std::string& value )
		{
			if ( value == "wind chimes" ) { return GlassEnum::windChimes; }
			return GlassEnum::windChimes;
		}
		std::string toString( const GlassEnum value )
		{
			switch ( value ) 
			{
				case GlassEnum::windChimes: return "wind chimes"; break;
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

		HarmonyType parseHarmonyType( const std::string& value )
		{
			if ( value == "explicit" ) { return HarmonyType::explicit_; }
			else if ( value == "implied" ) { return HarmonyType::implied; }
			else if ( value == "alternate" ) { return HarmonyType::alternate; }
			return HarmonyType::explicit_;
		}
		std::string toString( const HarmonyType value )
		{
			switch ( value ) 
			{
				case HarmonyType::explicit_: return "explicit"; break;
				case HarmonyType::implied: return "implied"; break;
				case HarmonyType::alternate: return "alternate"; break;
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

		KindValue parseKindValue( const std::string& value )
		{
			if ( value == "major" ) { return KindValue::major; }
			else if ( value == "minor" ) { return KindValue::minor; }
			else if ( value == "augmented" ) { return KindValue::augmented; }
			else if ( value == "diminished" ) { return KindValue::diminished; }
			else if ( value == "dominant" ) { return KindValue::dominant; }
			else if ( value == "major-seventh" ) { return KindValue::majorSeventh; }
			else if ( value == "minor-seventh" ) { return KindValue::minorSeventh; }
			else if ( value == "diminished-seventh" ) { return KindValue::diminishedSeventh; }
			else if ( value == "augmented-seventh" ) { return KindValue::augmentedSeventh; }
			else if ( value == "half-diminished" ) { return KindValue::halfDiminished; }
			else if ( value == "major-minor" ) { return KindValue::majorMinor; }
			else if ( value == "major-sixth" ) { return KindValue::majorSixth; }
			else if ( value == "minor-sixth" ) { return KindValue::minorSixth; }
			else if ( value == "dominant-ninth" ) { return KindValue::dominantNinth; }
			else if ( value == "major-ninth" ) { return KindValue::majorNinth; }
			else if ( value == "minor-ninth" ) { return KindValue::minorNinth; }
			else if ( value == "dominant-11th" ) { return KindValue::dominant11Th; }
			else if ( value == "major-11th" ) { return KindValue::major11Th; }
			else if ( value == "minor-11th" ) { return KindValue::minor11Th; }
			else if ( value == "dominant-13th" ) { return KindValue::dominant13Th; }
			else if ( value == "major-13th" ) { return KindValue::major13Th; }
			else if ( value == "minor-13th" ) { return KindValue::minor13Th; }
			else if ( value == "suspended-second" ) { return KindValue::suspendedSecond; }
			else if ( value == "suspended-fourth" ) { return KindValue::suspendedFourth; }
			else if ( value == "Neapolitan" ) { return KindValue::neapolitan; }
			else if ( value == "Italian" ) { return KindValue::italian; }
			else if ( value == "French" ) { return KindValue::french; }
			else if ( value == "German" ) { return KindValue::german; }
			else if ( value == "pedal" ) { return KindValue::pedal; }
			else if ( value == "power" ) { return KindValue::power; }
			else if ( value == "Tristan" ) { return KindValue::tristan; }
			else if ( value == "other" ) { return KindValue::other; }
			else if ( value == "none" ) { return KindValue::none; }
			return KindValue::major;
		}
		std::string toString( const KindValue value )
		{
			switch ( value ) 
			{
				case KindValue::major: return "major"; break;
				case KindValue::minor: return "minor"; break;
				case KindValue::augmented: return "augmented"; break;
				case KindValue::diminished: return "diminished"; break;
				case KindValue::dominant: return "dominant"; break;
				case KindValue::majorSeventh: return "major-seventh"; break;
				case KindValue::minorSeventh: return "minor-seventh"; break;
				case KindValue::diminishedSeventh: return "diminished-seventh"; break;
				case KindValue::augmentedSeventh: return "augmented-seventh"; break;
				case KindValue::halfDiminished: return "half-diminished"; break;
				case KindValue::majorMinor: return "major-minor"; break;
				case KindValue::majorSixth: return "major-sixth"; break;
				case KindValue::minorSixth: return "minor-sixth"; break;
				case KindValue::dominantNinth: return "dominant-ninth"; break;
				case KindValue::majorNinth: return "major-ninth"; break;
				case KindValue::minorNinth: return "minor-ninth"; break;
				case KindValue::dominant11Th: return "dominant-11th"; break;
				case KindValue::major11Th: return "major-11th"; break;
				case KindValue::minor11Th: return "minor-11th"; break;
				case KindValue::dominant13Th: return "dominant-13th"; break;
				case KindValue::major13Th: return "major-13th"; break;
				case KindValue::minor13Th: return "minor-13th"; break;
				case KindValue::suspendedSecond: return "suspended-second"; break;
				case KindValue::suspendedFourth: return "suspended-fourth"; break;
				case KindValue::neapolitan: return "Neapolitan"; break;
				case KindValue::italian: return "Italian"; break;
				case KindValue::french: return "French"; break;
				case KindValue::german: return "German"; break;
				case KindValue::pedal: return "pedal"; break;
				case KindValue::power: return "power"; break;
				case KindValue::tristan: return "Tristan"; break;
				case KindValue::other: return "other"; break;
				case KindValue::none: return "none"; break;
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

		LineEnd parseLineEnd( const std::string& value )
		{
			if ( value == "up" ) { return LineEnd::up; }
			else if ( value == "down" ) { return LineEnd::down; }
			else if ( value == "both" ) { return LineEnd::both; }
			else if ( value == "arrow" ) { return LineEnd::arrow; }
			else if ( value == "none" ) { return LineEnd::none; }
			return LineEnd::up;
		}
		std::string toString( const LineEnd value )
		{
			switch ( value ) 
			{
				case LineEnd::up: return "up"; break;
				case LineEnd::down: return "down"; break;
				case LineEnd::both: return "both"; break;
				case LineEnd::arrow: return "arrow"; break;
				case LineEnd::none: return "none"; break;
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

		MeasureNumberingValue parseMeasureNumberingValue( const std::string& value )
		{
			if ( value == "none" ) { return MeasureNumberingValue::none; }
			else if ( value == "measure" ) { return MeasureNumberingValue::measure; }
			else if ( value == "system" ) { return MeasureNumberingValue::system; }
			return MeasureNumberingValue::none;
		}
		std::string toString( const MeasureNumberingValue value )
		{
			switch ( value ) 
			{
				case MeasureNumberingValue::none: return "none"; break;
				case MeasureNumberingValue::measure: return "measure"; break;
				case MeasureNumberingValue::system: return "system"; break;
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

		MembraneEnum parseMembraneEnum( const std::string& value )
		{
			if ( value == "bass drum" ) { return MembraneEnum::bassDrum; }
			else if ( value == "bass drum on side" ) { return MembraneEnum::bassDrumOnSide; }
			else if ( value == "bongos" ) { return MembraneEnum::bongos; }
			else if ( value == "conga drum" ) { return MembraneEnum::congaDrum; }
			else if ( value == "goblet drum" ) { return MembraneEnum::gobletDrum; }
			else if ( value == "military drum" ) { return MembraneEnum::militaryDrum; }
			else if ( value == "snare drum" ) { return MembraneEnum::snareDrum; }
			else if ( value == "snare drum snares off" ) { return MembraneEnum::snareDrumSnaresOff; }
			else if ( value == "tambourine" ) { return MembraneEnum::tambourine; }
			else if ( value == "tenor drum" ) { return MembraneEnum::tenorDrum; }
			else if ( value == "timbales" ) { return MembraneEnum::timbales; }
			else if ( value == "tomtom" ) { return MembraneEnum::tomtom; }
			return MembraneEnum::bassDrum;
		}
		std::string toString( const MembraneEnum value )
		{
			switch ( value ) 
			{
				case MembraneEnum::bassDrum: return "bass drum"; break;
				case MembraneEnum::bassDrumOnSide: return "bass drum on side"; break;
				case MembraneEnum::bongos: return "bongos"; break;
				case MembraneEnum::congaDrum: return "conga drum"; break;
				case MembraneEnum::gobletDrum: return "goblet drum"; break;
				case MembraneEnum::militaryDrum: return "military drum"; break;
				case MembraneEnum::snareDrum: return "snare drum"; break;
				case MembraneEnum::snareDrumSnaresOff: return "snare drum snares off"; break;
				case MembraneEnum::tambourine: return "tambourine"; break;
				case MembraneEnum::tenorDrum: return "tenor drum"; break;
				case MembraneEnum::timbales: return "timbales"; break;
				case MembraneEnum::tomtom: return "tomtom"; break;
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

		MetalEnum parseMetalEnum( const std::string& value )
		{
			if ( value == "almglocken" ) { return MetalEnum::almglocken; }
			else if ( value == "bell" ) { return MetalEnum::bell; }
			else if ( value == "bell plate" ) { return MetalEnum::bellPlate; }
			else if ( value == "brake drum" ) { return MetalEnum::brakeDrum; }
			else if ( value == "Chinese cymbal" ) { return MetalEnum::chineseCymbal; }
			else if ( value == "cowbell" ) { return MetalEnum::cowbell; }
			else if ( value == "crash cymbals" ) { return MetalEnum::crashCymbals; }
			else if ( value == "crotale" ) { return MetalEnum::crotale; }
			else if ( value == "cymbal tongs" ) { return MetalEnum::cymbalTongs; }
			else if ( value == "domed gong" ) { return MetalEnum::domedGong; }
			else if ( value == "finger cymbals" ) { return MetalEnum::fingerCymbals; }
			else if ( value == "flexatone" ) { return MetalEnum::flexatone; }
			else if ( value == "gong" ) { return MetalEnum::gong; }
			else if ( value == "hi-hat" ) { return MetalEnum::hiHat; }
			else if ( value == "high-hat cymbals" ) { return MetalEnum::highHatCymbals; }
			else if ( value == "handbell" ) { return MetalEnum::handbell; }
			else if ( value == "sistrum" ) { return MetalEnum::sistrum; }
			else if ( value == "sizzle cymbal" ) { return MetalEnum::sizzleCymbal; }
			else if ( value == "sleigh bells" ) { return MetalEnum::sleighBells; }
			else if ( value == "suspended cymbal" ) { return MetalEnum::suspendedCymbal; }
			else if ( value == "tam tam" ) { return MetalEnum::tamTam; }
			else if ( value == "triangle" ) { return MetalEnum::triangle; }
			else if ( value == "Vietnamese hat" ) { return MetalEnum::vietnameseHat; }
			return MetalEnum::almglocken;
		}
		std::string toString( const MetalEnum value )
		{
			switch ( value ) 
			{
				case MetalEnum::almglocken: return "almglocken"; break;
				case MetalEnum::bell: return "bell"; break;
				case MetalEnum::bellPlate: return "bell plate"; break;
				case MetalEnum::brakeDrum: return "brake drum"; break;
				case MetalEnum::chineseCymbal: return "Chinese cymbal"; break;
				case MetalEnum::cowbell: return "cowbell"; break;
				case MetalEnum::crashCymbals: return "crash cymbals"; break;
				case MetalEnum::crotale: return "crotale"; break;
				case MetalEnum::cymbalTongs: return "cymbal tongs"; break;
				case MetalEnum::domedGong: return "domed gong"; break;
				case MetalEnum::fingerCymbals: return "finger cymbals"; break;
				case MetalEnum::flexatone: return "flexatone"; break;
				case MetalEnum::gong: return "gong"; break;
				case MetalEnum::hiHat: return "hi-hat"; break;
				case MetalEnum::highHatCymbals: return "high-hat cymbals"; break;
				case MetalEnum::handbell: return "handbell"; break;
				case MetalEnum::sistrum: return "sistrum"; break;
				case MetalEnum::sizzleCymbal: return "sizzle cymbal"; break;
				case MetalEnum::sleighBells: return "sleigh bells"; break;
				case MetalEnum::suspendedCymbal: return "suspended cymbal"; break;
				case MetalEnum::tamTam: return "tam tam"; break;
				case MetalEnum::triangle: return "triangle"; break;
				case MetalEnum::vietnameseHat: return "Vietnamese hat"; break;
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

		OnOff parseOnOff( const std::string& value )
		{
			if ( value == "on" ) { return OnOff::on; }
			else if ( value == "off" ) { return OnOff::off; }
			return OnOff::on;
		}
		std::string toString( const OnOff value )
		{
			switch ( value ) 
			{
				case OnOff::on: return "on"; break;
				case OnOff::off: return "off"; break;
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

		PitchedEnum parsePitchedEnum( const std::string& value )
		{
			if ( value == "chimes" ) { return PitchedEnum::chimes; }
			else if ( value == "glockenspiel" ) { return PitchedEnum::glockenspiel; }
			else if ( value == "mallet" ) { return PitchedEnum::mallet; }
			else if ( value == "marimba" ) { return PitchedEnum::marimba; }
			else if ( value == "tubular chimes" ) { return PitchedEnum::tubularChimes; }
			else if ( value == "vibraphone" ) { return PitchedEnum::vibraphone; }
			else if ( value == "xylophone" ) { return PitchedEnum::xylophone; }
			return PitchedEnum::chimes;
		}
		std::string toString( const PitchedEnum value )
		{
			switch ( value ) 
			{
				case PitchedEnum::chimes: return "chimes"; break;
				case PitchedEnum::glockenspiel: return "glockenspiel"; break;
				case PitchedEnum::mallet: return "mallet"; break;
				case PitchedEnum::marimba: return "marimba"; break;
				case PitchedEnum::tubularChimes: return "tubular chimes"; break;
				case PitchedEnum::vibraphone: return "vibraphone"; break;
				case PitchedEnum::xylophone: return "xylophone"; break;
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

		PrincipalVoiceSymbol parsePrincipalVoiceSymbol( const std::string& value )
		{
			if ( value == "Hauptstimme" ) { return PrincipalVoiceSymbol::hauptstimme; }
			else if ( value == "Nebenstimme" ) { return PrincipalVoiceSymbol::nebenstimme; }
			else if ( value == "plain" ) { return PrincipalVoiceSymbol::plain; }
			else if ( value == "none" ) { return PrincipalVoiceSymbol::none; }
			return PrincipalVoiceSymbol::hauptstimme;
		}
		std::string toString( const PrincipalVoiceSymbol value )
		{
			switch ( value ) 
			{
				case PrincipalVoiceSymbol::hauptstimme: return "Hauptstimme"; break;
				case PrincipalVoiceSymbol::nebenstimme: return "Nebenstimme"; break;
				case PrincipalVoiceSymbol::plain: return "plain"; break;
				case PrincipalVoiceSymbol::none: return "none"; break;
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

		StartStopChangeContinue parseStartStopChangeContinue( const std::string& value )
		{
			if ( value == "start" ) { return StartStopChangeContinue::start; }
			else if ( value == "stop" ) { return StartStopChangeContinue::stop; }
			else if ( value == "change" ) { return StartStopChangeContinue::change; }
			else if ( value == "continue" ) { return StartStopChangeContinue::continue_; }
			return StartStopChangeContinue::start;
		}
		std::string toString( const StartStopChangeContinue value )
		{
			switch ( value ) 
			{
				case StartStopChangeContinue::start: return "start"; break;
				case StartStopChangeContinue::stop: return "stop"; break;
				case StartStopChangeContinue::change: return "change"; break;
				case StartStopChangeContinue::continue_: return "continue"; break;
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

		TipDirection parseTipDirection( const std::string& value )
		{
			if ( value == "up" ) { return TipDirection::up; }
			else if ( value == "down" ) { return TipDirection::down; }
			else if ( value == "left" ) { return TipDirection::left; }
			else if ( value == "right" ) { return TipDirection::right; }
			else if ( value == "northwest" ) { return TipDirection::northwest; }
			else if ( value == "northeast" ) { return TipDirection::northeast; }
			else if ( value == "southeast" ) { return TipDirection::southeast; }
			else if ( value == "southwest" ) { return TipDirection::southwest; }
			return TipDirection::up;
		}
		std::string toString( const TipDirection value )
		{
			switch ( value ) 
			{
				case TipDirection::up: return "up"; break;
				case TipDirection::down: return "down"; break;
				case TipDirection::left: return "left"; break;
				case TipDirection::right: return "right"; break;
				case TipDirection::northwest: return "northwest"; break;
				case TipDirection::northeast: return "northeast"; break;
				case TipDirection::southeast: return "southeast"; break;
				case TipDirection::southwest: return "southwest"; break;
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

		StickLocationEnum parseStickLocationEnum( const std::string& value )
		{
			if ( value == "center" ) { return StickLocationEnum::center; }
			else if ( value == "rim" ) { return StickLocationEnum::rim; }
			else if ( value == "cymbal bell" ) { return StickLocationEnum::cymbalBell; }
			else if ( value == "cymbal edge" ) { return StickLocationEnum::cymbalEdge; }
			return StickLocationEnum::center;
		}
		std::string toString( const StickLocationEnum value )
		{
			switch ( value ) 
			{
				case StickLocationEnum::center: return "center"; break;
				case StickLocationEnum::rim: return "rim"; break;
				case StickLocationEnum::cymbalBell: return "cymbal bell"; break;
				case StickLocationEnum::cymbalEdge: return "cymbal edge"; break;
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

		StickMaterialEnum parseStickMaterialEnum( const std::string& value )
		{
			if ( value == "soft" ) { return StickMaterialEnum::soft; }
			else if ( value == "medium" ) { return StickMaterialEnum::medium; }
			else if ( value == "hard" ) { return StickMaterialEnum::hard; }
			else if ( value == "shaded" ) { return StickMaterialEnum::shaded; }
			else if ( value == "x" ) { return StickMaterialEnum::x; }
			return StickMaterialEnum::soft;
		}
		std::string toString( const StickMaterialEnum value )
		{
			switch ( value ) 
			{
				case StickMaterialEnum::soft: return "soft"; break;
				case StickMaterialEnum::medium: return "medium"; break;
				case StickMaterialEnum::hard: return "hard"; break;
				case StickMaterialEnum::shaded: return "shaded"; break;
				case StickMaterialEnum::x: return "x"; break;
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

		StickTypeEnum parseStickTypeEnum( const std::string& value )
		{
			if ( value == "bass drum" ) { return StickTypeEnum::bassDrum; }
			else if ( value == "double bass drum" ) { return StickTypeEnum::doubleBassDrum; }
			else if ( value == "timpani" ) { return StickTypeEnum::timpani; }
			else if ( value == "xylophone" ) { return StickTypeEnum::xylophone; }
			else if ( value == "yarn" ) { return StickTypeEnum::yarn; }
			return StickTypeEnum::bassDrum;
		}
		std::string toString( const StickTypeEnum value )
		{
			switch ( value ) 
			{
				case StickTypeEnum::bassDrum: return "bass drum"; break;
				case StickTypeEnum::doubleBassDrum: return "double bass drum"; break;
				case StickTypeEnum::timpani: return "timpani"; break;
				case StickTypeEnum::xylophone: return "xylophone"; break;
				case StickTypeEnum::yarn: return "yarn"; break;
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

		UpDownStopContinue parseUpDownStopContinue( const std::string& value )
		{
			if ( value == "up" ) { return UpDownStopContinue::up; }
			else if ( value == "down" ) { return UpDownStopContinue::down; }
			else if ( value == "stop" ) { return UpDownStopContinue::stop; }
			else if ( value == "continue" ) { return UpDownStopContinue::continue_; }
			return UpDownStopContinue::up;
		}
		std::string toString( const UpDownStopContinue value )
		{
			switch ( value ) 
			{
				case UpDownStopContinue::up: return "up"; break;
				case UpDownStopContinue::down: return "down"; break;
				case UpDownStopContinue::stop: return "stop"; break;
				case UpDownStopContinue::continue_: return "continue"; break;
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

		WedgeType parseWedgeType( const std::string& value )
		{
			if ( value == "crescendo" ) { return WedgeType::crescendo; }
			else if ( value == "diminuendo" ) { return WedgeType::diminuendo; }
			else if ( value == "stop" ) { return WedgeType::stop; }
			else if ( value == "continue" ) { return WedgeType::continue_; }
			return WedgeType::crescendo;
		}
		std::string toString( const WedgeType value )
		{
			switch ( value ) 
			{
				case WedgeType::crescendo: return "crescendo"; break;
				case WedgeType::diminuendo: return "diminuendo"; break;
				case WedgeType::stop: return "stop"; break;
				case WedgeType::continue_: return "continue"; break;
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

		WoodEnum parseWoodEnum( const std::string& value )
		{
			if ( value == "board clapper" ) { return WoodEnum::boardClapper; }
			else if ( value == "cabasa" ) { return WoodEnum::cabasa; }
			else if ( value == "castanets" ) { return WoodEnum::castanets; }
			else if ( value == "claves" ) { return WoodEnum::claves; }
			else if ( value == "guiro" ) { return WoodEnum::guiro; }
			else if ( value == "log drum" ) { return WoodEnum::logDrum; }
			else if ( value == "maraca" ) { return WoodEnum::maraca; }
			else if ( value == "maracas" ) { return WoodEnum::maracas; }
			else if ( value == "ratchet" ) { return WoodEnum::ratchet; }
			else if ( value == "sandpaper blocks" ) { return WoodEnum::sandpaperBlocks; }
			else if ( value == "slit drum" ) { return WoodEnum::slitDrum; }
			else if ( value == "temple block" ) { return WoodEnum::templeBlock; }
			else if ( value == "vibraslap" ) { return WoodEnum::vibraslap; }
			else if ( value == "wood block" ) { return WoodEnum::woodBlock; }
			return WoodEnum::boardClapper;
		}
		std::string toString( const WoodEnum value )
		{
			switch ( value ) 
			{
				case WoodEnum::boardClapper: return "board clapper"; break;
				case WoodEnum::cabasa: return "cabasa"; break;
				case WoodEnum::castanets: return "castanets"; break;
				case WoodEnum::claves: return "claves"; break;
				case WoodEnum::guiro: return "guiro"; break;
				case WoodEnum::logDrum: return "log drum"; break;
				case WoodEnum::maraca: return "maraca"; break;
				case WoodEnum::maracas: return "maracas"; break;
				case WoodEnum::ratchet: return "ratchet"; break;
				case WoodEnum::sandpaperBlocks: return "sandpaper blocks"; break;
				case WoodEnum::slitDrum: return "slit drum"; break;
				case WoodEnum::templeBlock: return "temple block"; break;
				case WoodEnum::vibraslap: return "vibraslap"; break;
				case WoodEnum::woodBlock: return "wood block"; break;
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

		MarginType parseMarginType( const std::string& value )
		{
			if ( value == "odd" ) { return MarginType::odd; }
			else if ( value == "even" ) { return MarginType::even; }
			else if ( value == "both" ) { return MarginType::both; }
			return MarginType::odd;
		}
		std::string toString( const MarginType value )
		{
			switch ( value ) 
			{
				case MarginType::odd: return "odd"; break;
				case MarginType::even: return "even"; break;
				case MarginType::both: return "both"; break;
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

		NoteSizeType parseNoteSizeType( const std::string& value )
		{
			if ( value == "cue" ) { return NoteSizeType::cue; }
			else if ( value == "grace" ) { return NoteSizeType::grace; }
			else if ( value == "large" ) { return NoteSizeType::large; }
			return NoteSizeType::cue;
		}
		std::string toString( const NoteSizeType value )
		{
			switch ( value ) 
			{
				case NoteSizeType::cue: return "cue"; break;
				case NoteSizeType::grace: return "grace"; break;
				case NoteSizeType::large: return "large"; break;
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

		AccidentalValue parseAccidentalValue( const std::string& value )
		{
			if ( value == "sharp" ) { return AccidentalValue::sharp; }
			else if ( value == "natural" ) { return AccidentalValue::natural; }
			else if ( value == "flat" ) { return AccidentalValue::flat; }
			else if ( value == "double-sharp" ) { return AccidentalValue::doubleSharp; }
			else if ( value == "sharp-sharp" ) { return AccidentalValue::sharpSharp; }
			else if ( value == "flat-flat" ) { return AccidentalValue::flatFlat; }
			else if ( value == "natural-sharp" ) { return AccidentalValue::naturalSharp; }
			else if ( value == "natural-flat" ) { return AccidentalValue::naturalFlat; }
			else if ( value == "quarter-flat" ) { return AccidentalValue::quarterFlat; }
			else if ( value == "quarter-sharp" ) { return AccidentalValue::quarterSharp; }
			else if ( value == "three-quarters-flat" ) { return AccidentalValue::threeQuartersFlat; }
			else if ( value == "three-quarters-sharp" ) { return AccidentalValue::threeQuartersSharp; }
			else if ( value == "sharp-down" ) { return AccidentalValue::sharpDown; }
			else if ( value == "sharp-up" ) { return AccidentalValue::sharpUp; }
			else if ( value == "natural-down" ) { return AccidentalValue::naturalDown; }
			else if ( value == "natural-up" ) { return AccidentalValue::naturalUp; }
			else if ( value == "flat-down" ) { return AccidentalValue::flatDown; }
			else if ( value == "flat-up" ) { return AccidentalValue::flatUp; }
			else if ( value == "triple-sharp" ) { return AccidentalValue::tripleSharp; }
			else if ( value == "triple-flat" ) { return AccidentalValue::tripleFlat; }
			else if ( value == "slash-quarter-sharp" ) { return AccidentalValue::slashQuarterSharp; }
			else if ( value == "slash-sharp" ) { return AccidentalValue::slashSharp; }
			else if ( value == "slash-flat" ) { return AccidentalValue::slashFlat; }
			else if ( value == "double-slash-flat" ) { return AccidentalValue::doubleSlashFlat; }
			else if ( value == "sharp-1" ) { return AccidentalValue::sharp1; }
			else if ( value == "sharp-2" ) { return AccidentalValue::sharp2; }
			else if ( value == "sharp-3" ) { return AccidentalValue::sharp3; }
			else if ( value == "sharp-5" ) { return AccidentalValue::sharp5; }
			else if ( value == "flat-1" ) { return AccidentalValue::flat1; }
			else if ( value == "flat-2" ) { return AccidentalValue::flat2; }
			else if ( value == "flat-3" ) { return AccidentalValue::flat3; }
			else if ( value == "flat-4" ) { return AccidentalValue::flat4; }
			else if ( value == "sori" ) { return AccidentalValue::sori; }
			else if ( value == "koron" ) { return AccidentalValue::koron; }
			return AccidentalValue::sharp;
		}
		std::string toString( const AccidentalValue value )
		{
			switch ( value ) 
			{
				case AccidentalValue::sharp: return "sharp"; break;
				case AccidentalValue::natural: return "natural"; break;
				case AccidentalValue::flat: return "flat"; break;
				case AccidentalValue::doubleSharp: return "double-sharp"; break;
				case AccidentalValue::sharpSharp: return "sharp-sharp"; break;
				case AccidentalValue::flatFlat: return "flat-flat"; break;
				case AccidentalValue::naturalSharp: return "natural-sharp"; break;
				case AccidentalValue::naturalFlat: return "natural-flat"; break;
				case AccidentalValue::quarterFlat: return "quarter-flat"; break;
				case AccidentalValue::quarterSharp: return "quarter-sharp"; break;
				case AccidentalValue::threeQuartersFlat: return "three-quarters-flat"; break;
				case AccidentalValue::threeQuartersSharp: return "three-quarters-sharp"; break;
				case AccidentalValue::sharpDown: return "sharp-down"; break;
				case AccidentalValue::sharpUp: return "sharp-up"; break;
				case AccidentalValue::naturalDown: return "natural-down"; break;
				case AccidentalValue::naturalUp: return "natural-up"; break;
				case AccidentalValue::flatDown: return "flat-down"; break;
				case AccidentalValue::flatUp: return "flat-up"; break;
				case AccidentalValue::tripleSharp: return "triple-sharp"; break;
				case AccidentalValue::tripleFlat: return "triple-flat"; break;
				case AccidentalValue::slashQuarterSharp: return "slash-quarter-sharp"; break;
				case AccidentalValue::slashSharp: return "slash-sharp"; break;
				case AccidentalValue::slashFlat: return "slash-flat"; break;
				case AccidentalValue::doubleSlashFlat: return "double-slash-flat"; break;
				case AccidentalValue::sharp1: return "sharp-1"; break;
				case AccidentalValue::sharp2: return "sharp-2"; break;
				case AccidentalValue::sharp3: return "sharp-3"; break;
				case AccidentalValue::sharp5: return "sharp-5"; break;
				case AccidentalValue::flat1: return "flat-1"; break;
				case AccidentalValue::flat2: return "flat-2"; break;
				case AccidentalValue::flat3: return "flat-3"; break;
				case AccidentalValue::flat4: return "flat-4"; break;
				case AccidentalValue::sori: return "sori"; break;
				case AccidentalValue::koron: return "koron"; break;
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

		ArrowDirectionEnum parseArrowDirectionEnum( const std::string& value )
		{
			if ( value == "left" ) { return ArrowDirectionEnum::left; }
			else if ( value == "up" ) { return ArrowDirectionEnum::up; }
			else if ( value == "right" ) { return ArrowDirectionEnum::right; }
			else if ( value == "down" ) { return ArrowDirectionEnum::down; }
			else if ( value == "northwest" ) { return ArrowDirectionEnum::northwest; }
			else if ( value == "northeast" ) { return ArrowDirectionEnum::northeast; }
			else if ( value == "southeast" ) { return ArrowDirectionEnum::southeast; }
			else if ( value == "southwest" ) { return ArrowDirectionEnum::southwest; }
			else if ( value == "left right" ) { return ArrowDirectionEnum::leftRight; }
			else if ( value == "up down" ) { return ArrowDirectionEnum::upDown; }
			else if ( value == "northwest southeast" ) { return ArrowDirectionEnum::northwestSoutheast; }
			else if ( value == "northeast southwest" ) { return ArrowDirectionEnum::northeastSouthwest; }
			else if ( value == "other" ) { return ArrowDirectionEnum::other; }
			return ArrowDirectionEnum::left;
		}
		std::string toString( const ArrowDirectionEnum value )
		{
			switch ( value ) 
			{
				case ArrowDirectionEnum::left: return "left"; break;
				case ArrowDirectionEnum::up: return "up"; break;
				case ArrowDirectionEnum::right: return "right"; break;
				case ArrowDirectionEnum::down: return "down"; break;
				case ArrowDirectionEnum::northwest: return "northwest"; break;
				case ArrowDirectionEnum::northeast: return "northeast"; break;
				case ArrowDirectionEnum::southeast: return "southeast"; break;
				case ArrowDirectionEnum::southwest: return "southwest"; break;
				case ArrowDirectionEnum::leftRight: return "left right"; break;
				case ArrowDirectionEnum::upDown: return "up down"; break;
				case ArrowDirectionEnum::northwestSoutheast: return "northwest southeast"; break;
				case ArrowDirectionEnum::northeastSouthwest: return "northeast southwest"; break;
				case ArrowDirectionEnum::other: return "other"; break;
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

		ArrowStyleEnum parseArrowStyleEnum( const std::string& value )
		{
			if ( value == "single" ) { return ArrowStyleEnum::single; }
			else if ( value == "double" ) { return ArrowStyleEnum::double_; }
			else if ( value == "filled" ) { return ArrowStyleEnum::filled; }
			else if ( value == "hollow" ) { return ArrowStyleEnum::hollow; }
			else if ( value == "paired" ) { return ArrowStyleEnum::paired; }
			else if ( value == "combined" ) { return ArrowStyleEnum::combined; }
			else if ( value == "other" ) { return ArrowStyleEnum::other; }
			return ArrowStyleEnum::single;
		}
		std::string toString( const ArrowStyleEnum value )
		{
			switch ( value ) 
			{
				case ArrowStyleEnum::single: return "single"; break;
				case ArrowStyleEnum::double_: return "double"; break;
				case ArrowStyleEnum::filled: return "filled"; break;
				case ArrowStyleEnum::hollow: return "hollow"; break;
				case ArrowStyleEnum::paired: return "paired"; break;
				case ArrowStyleEnum::combined: return "combined"; break;
				case ArrowStyleEnum::other: return "other"; break;
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

		BeamValue parseBeamValue( const std::string& value )
		{
			if ( value == "begin" ) { return BeamValue::begin; }
			else if ( value == "continue" ) { return BeamValue::continue_; }
			else if ( value == "end" ) { return BeamValue::end; }
			else if ( value == "forward hook" ) { return BeamValue::forwardHook; }
			else if ( value == "backward hook" ) { return BeamValue::backwardHook; }
			return BeamValue::begin;
		}
		std::string toString( const BeamValue value )
		{
			switch ( value ) 
			{
				case BeamValue::begin: return "begin"; break;
				case BeamValue::continue_: return "continue"; break;
				case BeamValue::end: return "end"; break;
				case BeamValue::forwardHook: return "forward hook"; break;
				case BeamValue::backwardHook: return "backward hook"; break;
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

		BreathMarkValue parseBreathMarkValue( const std::string& value )
		{
			if ( value == "" ) { return BreathMarkValue::emptystring; }
			else if ( value == "comma" ) { return BreathMarkValue::comma; }
			else if ( value == "tick" ) { return BreathMarkValue::tick; }
			return BreathMarkValue::emptystring;
		}
		std::string toString( const BreathMarkValue value )
		{
			switch ( value ) 
			{
				case BreathMarkValue::emptystring: return ""; break;
				case BreathMarkValue::comma: return "comma"; break;
				case BreathMarkValue::tick: return "tick"; break;
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

		CircularArrowEnum parseCircularArrowEnum( const std::string& value )
		{
			if ( value == "clockwise" ) { return CircularArrowEnum::clockwise; }
			else if ( value == "anticlockwise" ) { return CircularArrowEnum::anticlockwise; }
			return CircularArrowEnum::clockwise;
		}
		std::string toString( const CircularArrowEnum value )
		{
			switch ( value ) 
			{
				case CircularArrowEnum::clockwise: return "clockwise"; break;
				case CircularArrowEnum::anticlockwise: return "anticlockwise"; break;
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

		Fan parseFan( const std::string& value )
		{
			if ( value == "accel" ) { return Fan::accel; }
			else if ( value == "rit" ) { return Fan::rit; }
			else if ( value == "none" ) { return Fan::none; }
			return Fan::accel;
		}
		std::string toString( const Fan value )
		{
			switch ( value ) 
			{
				case Fan::accel: return "accel"; break;
				case Fan::rit: return "rit"; break;
				case Fan::none: return "none"; break;
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

		HandbellValue parseHandbellValue( const std::string& value )
		{
			if ( value == "damp" ) { return HandbellValue::damp; }
			else if ( value == "echo" ) { return HandbellValue::echo; }
			else if ( value == "gyro" ) { return HandbellValue::gyro; }
			else if ( value == "hand martellato" ) { return HandbellValue::handMartellato; }
			else if ( value == "mallet lift" ) { return HandbellValue::malletLift; }
			else if ( value == "mallet table" ) { return HandbellValue::malletTable; }
			else if ( value == "martellato" ) { return HandbellValue::martellato; }
			else if ( value == "martellato lift" ) { return HandbellValue::martellatoLift; }
			else if ( value == "muted martellato" ) { return HandbellValue::mutedMartellato; }
			else if ( value == "pluck lift" ) { return HandbellValue::pluckLift; }
			else if ( value == "swing" ) { return HandbellValue::swing; }
			return HandbellValue::damp;
		}
		std::string toString( const HandbellValue value )
		{
			switch ( value ) 
			{
				case HandbellValue::damp: return "damp"; break;
				case HandbellValue::echo: return "echo"; break;
				case HandbellValue::gyro: return "gyro"; break;
				case HandbellValue::handMartellato: return "hand martellato"; break;
				case HandbellValue::malletLift: return "mallet lift"; break;
				case HandbellValue::malletTable: return "mallet table"; break;
				case HandbellValue::martellato: return "martellato"; break;
				case HandbellValue::martellatoLift: return "martellato lift"; break;
				case HandbellValue::mutedMartellato: return "muted martellato"; break;
				case HandbellValue::pluckLift: return "pluck lift"; break;
				case HandbellValue::swing: return "swing"; break;
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

		HoleClosedLocation parseHoleClosedLocation( const std::string& value )
		{
			if ( value == "right" ) { return HoleClosedLocation::right; }
			else if ( value == "bottom" ) { return HoleClosedLocation::bottom; }
			else if ( value == "left" ) { return HoleClosedLocation::left; }
			else if ( value == "top" ) { return HoleClosedLocation::top; }
			return HoleClosedLocation::right;
		}
		std::string toString( const HoleClosedLocation value )
		{
			switch ( value ) 
			{
				case HoleClosedLocation::right: return "right"; break;
				case HoleClosedLocation::bottom: return "bottom"; break;
				case HoleClosedLocation::left: return "left"; break;
				case HoleClosedLocation::top: return "top"; break;
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

		HoleClosedValue parseHoleClosedValue( const std::string& value )
		{
			if ( value == "yes" ) { return HoleClosedValue::yes; }
			else if ( value == "no" ) { return HoleClosedValue::no; }
			else if ( value == "half" ) { return HoleClosedValue::half; }
			return HoleClosedValue::yes;
		}
		std::string toString( const HoleClosedValue value )
		{
			switch ( value ) 
			{
				case HoleClosedValue::yes: return "yes"; break;
				case HoleClosedValue::no: return "no"; break;
				case HoleClosedValue::half: return "half"; break;
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

		NoteTypeValue parseNoteTypeValue( const std::string& value )
		{
			if ( value == "1024th" ) { return NoteTypeValue::oneThousandTwentyFourth; }
			else if ( value == "512th" ) { return NoteTypeValue::fiveHundredTwelfth; }
			else if ( value == "256th" ) { return NoteTypeValue::twoHundredFifthySixth; }
			else if ( value == "128th" ) { return NoteTypeValue::oneHundredTwentyEighth; }
			else if ( value == "64th" ) { return NoteTypeValue::sixtyFourth; }
			else if ( value == "32nd" ) { return NoteTypeValue::thirtySecond; }
			else if ( value == "16th" ) { return NoteTypeValue::sixteenth; }
			else if ( value == "eighth" ) { return NoteTypeValue::eighth; }
			else if ( value == "quarter" ) { return NoteTypeValue::quarter; }
			else if ( value == "half" ) { return NoteTypeValue::half; }
			else if ( value == "whole" ) { return NoteTypeValue::whole; }
			else if ( value == "breve" ) { return NoteTypeValue::breve; }
			else if ( value == "long" ) { return NoteTypeValue::long_; }
			else if ( value == "maxima" ) { return NoteTypeValue::maxima; }
			return NoteTypeValue::oneThousandTwentyFourth;
		}
		std::string toString( const NoteTypeValue value )
		{
			switch ( value ) 
			{
				case NoteTypeValue::oneThousandTwentyFourth: return "1024th"; break;
				case NoteTypeValue::fiveHundredTwelfth: return "512th"; break;
				case NoteTypeValue::twoHundredFifthySixth: return "256th"; break;
				case NoteTypeValue::oneHundredTwentyEighth: return "128th"; break;
				case NoteTypeValue::sixtyFourth: return "64th"; break;
				case NoteTypeValue::thirtySecond: return "32nd"; break;
				case NoteTypeValue::sixteenth: return "16th"; break;
				case NoteTypeValue::eighth: return "eighth"; break;
				case NoteTypeValue::quarter: return "quarter"; break;
				case NoteTypeValue::half: return "half"; break;
				case NoteTypeValue::whole: return "whole"; break;
				case NoteTypeValue::breve: return "breve"; break;
				case NoteTypeValue::long_: return "long"; break;
				case NoteTypeValue::maxima: return "maxima"; break;
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

		NoteheadValue parseNoteheadValue( const std::string& value )
		{
			if ( value == "slash" ) { return NoteheadValue::slash; }
			else if ( value == "triangle" ) { return NoteheadValue::triangle; }
			else if ( value == "diamond" ) { return NoteheadValue::diamond; }
			else if ( value == "square" ) { return NoteheadValue::square; }
			else if ( value == "cross" ) { return NoteheadValue::cross; }
			else if ( value == "x" ) { return NoteheadValue::x; }
			else if ( value == "circle-x" ) { return NoteheadValue::circleX; }
			else if ( value == "inverted triangle" ) { return NoteheadValue::invertedTriangle; }
			else if ( value == "arrow down" ) { return NoteheadValue::arrowDown; }
			else if ( value == "arrow up" ) { return NoteheadValue::arrowUp; }
			else if ( value == "slashed" ) { return NoteheadValue::slashed; }
			else if ( value == "back slashed" ) { return NoteheadValue::backSlashed; }
			else if ( value == "normal" ) { return NoteheadValue::normal; }
			else if ( value == "cluster" ) { return NoteheadValue::cluster; }
			else if ( value == "circle dot" ) { return NoteheadValue::circleDot; }
			else if ( value == "left triangle" ) { return NoteheadValue::leftTriangle; }
			else if ( value == "rectangle" ) { return NoteheadValue::rectangle; }
			else if ( value == "none" ) { return NoteheadValue::none; }
			else if ( value == "do" ) { return NoteheadValue::do_; }
			else if ( value == "re" ) { return NoteheadValue::re; }
			else if ( value == "mi" ) { return NoteheadValue::mi; }
			else if ( value == "fa" ) { return NoteheadValue::fa; }
			else if ( value == "fa up" ) { return NoteheadValue::faUp; }
			else if ( value == "so" ) { return NoteheadValue::so; }
			else if ( value == "la" ) { return NoteheadValue::la; }
			else if ( value == "ti" ) { return NoteheadValue::ti; }
			return NoteheadValue::slash;
		}
		std::string toString( const NoteheadValue value )
		{
			switch ( value ) 
			{
				case NoteheadValue::slash: return "slash"; break;
				case NoteheadValue::triangle: return "triangle"; break;
				case NoteheadValue::diamond: return "diamond"; break;
				case NoteheadValue::square: return "square"; break;
				case NoteheadValue::cross: return "cross"; break;
				case NoteheadValue::x: return "x"; break;
				case NoteheadValue::circleX: return "circle-x"; break;
				case NoteheadValue::invertedTriangle: return "inverted triangle"; break;
				case NoteheadValue::arrowDown: return "arrow down"; break;
				case NoteheadValue::arrowUp: return "arrow up"; break;
				case NoteheadValue::slashed: return "slashed"; break;
				case NoteheadValue::backSlashed: return "back slashed"; break;
				case NoteheadValue::normal: return "normal"; break;
				case NoteheadValue::cluster: return "cluster"; break;
				case NoteheadValue::circleDot: return "circle dot"; break;
				case NoteheadValue::leftTriangle: return "left triangle"; break;
				case NoteheadValue::rectangle: return "rectangle"; break;
				case NoteheadValue::none: return "none"; break;
				case NoteheadValue::do_: return "do"; break;
				case NoteheadValue::re: return "re"; break;
				case NoteheadValue::mi: return "mi"; break;
				case NoteheadValue::fa: return "fa"; break;
				case NoteheadValue::faUp: return "fa up"; break;
				case NoteheadValue::so: return "so"; break;
				case NoteheadValue::la: return "la"; break;
				case NoteheadValue::ti: return "ti"; break;
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

		ShowTuplet parseShowTuplet( const std::string& value )
		{
			if ( value == "actual" ) { return ShowTuplet::actual; }
			else if ( value == "both" ) { return ShowTuplet::both; }
			else if ( value == "none" ) { return ShowTuplet::none; }
			return ShowTuplet::actual;
		}
		std::string toString( const ShowTuplet value )
		{
			switch ( value ) 
			{
				case ShowTuplet::actual: return "actual"; break;
				case ShowTuplet::both: return "both"; break;
				case ShowTuplet::none: return "none"; break;
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

		StemValue parseStemValue( const std::string& value )
		{
			if ( value == "down" ) { return StemValue::down; }
			else if ( value == "up" ) { return StemValue::up; }
			else if ( value == "double" ) { return StemValue::double_; }
			else if ( value == "none" ) { return StemValue::none; }
			return StemValue::down;
		}
		std::string toString( const StemValue value )
		{
			switch ( value ) 
			{
				case StemValue::down: return "down"; break;
				case StemValue::up: return "up"; break;
				case StemValue::double_: return "double"; break;
				case StemValue::none: return "none"; break;
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

		StepEnum parseStepEnum( const std::string& value )
		{
			if ( value == "A" ) { return StepEnum::a; }
			else if ( value == "B" ) { return StepEnum::b; }
			else if ( value == "C" ) { return StepEnum::c; }
			else if ( value == "D" ) { return StepEnum::d; }
			else if ( value == "E" ) { return StepEnum::e; }
			else if ( value == "F" ) { return StepEnum::f; }
			else if ( value == "G" ) { return StepEnum::g; }
			return StepEnum::a;
		}
		std::string toString( const StepEnum value )
		{
			switch ( value ) 
			{
				case StepEnum::a: return "A"; break;
				case StepEnum::b: return "B"; break;
				case StepEnum::c: return "C"; break;
				case StepEnum::d: return "D"; break;
				case StepEnum::e: return "E"; break;
				case StepEnum::f: return "F"; break;
				case StepEnum::g: return "G"; break;
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

		SyllabicEnum parseSyllabicEnum( const std::string& value )
		{
			if ( value == "single" ) { return SyllabicEnum::single; }
			else if ( value == "begin" ) { return SyllabicEnum::begin; }
			else if ( value == "end" ) { return SyllabicEnum::end; }
			else if ( value == "middle" ) { return SyllabicEnum::middle; }
			return SyllabicEnum::single;
		}
		std::string toString( const SyllabicEnum value )
		{
			switch ( value ) 
			{
				case SyllabicEnum::single: return "single"; break;
				case SyllabicEnum::begin: return "begin"; break;
				case SyllabicEnum::end: return "end"; break;
				case SyllabicEnum::middle: return "middle"; break;
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

		GroupBarlineValue parseGroupBarlineValue( const std::string& value )
		{
			if ( value == "yes" ) { return GroupBarlineValue::yes; }
			else if ( value == "no" ) { return GroupBarlineValue::no; }
			else if ( value == "Mensurstrich" ) { return GroupBarlineValue::mensurstrich; }
			return GroupBarlineValue::yes;
		}
		std::string toString( const GroupBarlineValue value )
		{
			switch ( value ) 
			{
				case GroupBarlineValue::yes: return "yes"; break;
				case GroupBarlineValue::no: return "no"; break;
				case GroupBarlineValue::mensurstrich: return "Mensurstrich"; break;
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

		GroupSymbolValue parseGroupSymbolValue( const std::string& value )
		{
			if ( value == "none" ) { return GroupSymbolValue::none; }
			else if ( value == "brace" ) { return GroupSymbolValue::brace; }
			else if ( value == "line" ) { return GroupSymbolValue::line; }
			else if ( value == "bracket" ) { return GroupSymbolValue::bracket; }
			else if ( value == "square" ) { return GroupSymbolValue::square; }
			return GroupSymbolValue::none;
		}
		std::string toString( const GroupSymbolValue value )
		{
			switch ( value ) 
			{
				case GroupSymbolValue::none: return "none"; break;
				case GroupSymbolValue::brace: return "brace"; break;
				case GroupSymbolValue::line: return "line"; break;
				case GroupSymbolValue::bracket: return "bracket"; break;
				case GroupSymbolValue::square: return "square"; break;
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
        
        ModeEnum parseModeEnum( const std::string& value, bool& success  )
		{
            success = true;
			if ( value == "major" ) { return ModeEnum::major; }
			else if ( value == "minor" ) { return ModeEnum::minor; }
			else if ( value == "dorian" ) { return ModeEnum::dorian; }
			else if ( value == "phrygian" ) { return ModeEnum::phrygian; }
			else if ( value == "lydian" ) { return ModeEnum::lydian; }
			else if ( value == "mixolydian" ) { return ModeEnum::mixolydian; }
			else if ( value == "aeolian" ) { return ModeEnum::aeolian; }
			else if ( value == "ionian" ) { return ModeEnum::ionian; }
			else if ( value == "locrian" ) { return ModeEnum::locrian; }
			else if ( value == "none" ) { return ModeEnum::none; }
			else if ( value == "other" ) { success = false; return ModeEnum::other; }
            success = false;
			return ModeEnum::other;
		}
        ModeEnum parseModeEnum( const std::string& value )
		{
			bool temp;
			return parseModeEnum( value, temp );
		}
		std::string toString( const ModeEnum value )
		{
			switch ( value )
			{
				case ModeEnum::major: return "major"; break;
				case ModeEnum::minor: return "minor"; break;
				case ModeEnum::dorian: return "dorian"; break;
				case ModeEnum::phrygian: return "phrygian"; break;
				case ModeEnum::lydian: return "lydian"; break;
				case ModeEnum::mixolydian: return "mixolydian"; break;
				case ModeEnum::aeolian: return "aeolian"; break;
				case ModeEnum::ionian: return "ionian"; break;
				case ModeEnum::locrian: return "locrian"; break;
				case ModeEnum::none: return "none"; break;
				case ModeEnum::other: return "other"; break;
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
        :myEnum( value )
        ,myCustomValue( "" )
        {
            setValue( value );
        }
        ModeValue::ModeValue( const std::string& value )
        :myEnum( ModeEnum::other )
        ,myCustomValue( value )
        {
            setValue( value );
        }
        ModeValue::ModeValue()
        :myEnum( ModeEnum::major )
        ,myCustomValue( "" )
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
        }
        void ModeValue::setValue( const std::string& value )
        {
            bool found = false;
            ModeEnum temp = parseModeEnum( value, found );
            if ( found )
            {
                myEnum = temp;
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
        
        DistanceTypeEnum parseDistanceTypeEnum( const std::string& value, bool& success  )
		{
            success = true;
			if ( value == "beam" ) { return DistanceTypeEnum::beam; }
			else if ( value == "hyphen" ) { return DistanceTypeEnum::hyphen; }
			else if ( value == "other" ) { success = false; return DistanceTypeEnum::other; }
            success = false;
			return DistanceTypeEnum::other;
		}
        DistanceTypeEnum parseDistanceTypeEnum( const std::string& value )
		{
			bool temp;
			return parseDistanceTypeEnum( value, temp );
		}
		std::string toString( const DistanceTypeEnum value )
		{
			switch ( value )
			{
				case DistanceTypeEnum::beam: return "beam"; break;
				case DistanceTypeEnum::hyphen: return "hyphen"; break;
				case DistanceTypeEnum::other: return "other"; break;
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
        :myEnum( value )
        ,myCustomValue( "" )
        {
            setValue( value );
        }
        DistanceType::DistanceType( const std::string& value )
        :myEnum( DistanceTypeEnum::other )
        ,myCustomValue( value )
        {
            setValue( value );
        }
        DistanceType::DistanceType()
        :myEnum( DistanceTypeEnum::beam )
        ,myCustomValue( "" )
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
        }
        void DistanceType::setValue( const std::string& value )
        {
            bool found = false;
            DistanceTypeEnum temp = parseDistanceTypeEnum( value, found );
            if ( found )
            {
                myEnum = temp;
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

        LineWidthTypeEnum parseLineWidthTypeEnum( const std::string& value, bool& success  )
		{
            success = true;
			if ( value == "beam" ) { return LineWidthTypeEnum::beam; }
			else if ( value == "bracket" ) { return LineWidthTypeEnum::bracket; }
			else if ( value == "dashes" ) { return LineWidthTypeEnum::dashes; }
			else if ( value == "enclosure" ) { return LineWidthTypeEnum::enclosure; }
			else if ( value == "ending" ) { return LineWidthTypeEnum::ending; }
			else if ( value == "extend" ) { return LineWidthTypeEnum::extend; }
			else if ( value == "heavy barline" ) { return LineWidthTypeEnum::heavyBarline; }
			else if ( value == "leger" ) { return LineWidthTypeEnum::leger; }
			else if ( value == "light barline" ) { return LineWidthTypeEnum::lightBarline; }
			else if ( value == "octave shift" ) { return LineWidthTypeEnum::octaveShift; }
			else if ( value == "pedal" ) { return LineWidthTypeEnum::pedal; }
			else if ( value == "slur middle" ) { return LineWidthTypeEnum::slurMiddle; }
			else if ( value == "slur tip" ) { return LineWidthTypeEnum::slurTip; }
			else if ( value == "staff" ) { return LineWidthTypeEnum::staff; }
			else if ( value == "stem" ) { return LineWidthTypeEnum::stem; }
			else if ( value == "tie middle" ) { return LineWidthTypeEnum::tieMiddle; }
			else if ( value == "tie tip" ) { return LineWidthTypeEnum::tieTip; }
			else if ( value == "tuplet bracket" ) { return LineWidthTypeEnum::tupletBracket; }
			else if ( value == "wedge" ) { return LineWidthTypeEnum::wedge; }
			else if ( value == "other" ) { success = false; return LineWidthTypeEnum::other; }
            success = false;
			return LineWidthTypeEnum::other;
		}
        LineWidthTypeEnum parseLineWidthTypeEnum( const std::string& value )
		{
			bool temp;
			return parseLineWidthTypeEnum( value, temp );
		}
		std::string toString( const LineWidthTypeEnum value )
		{
			switch ( value )
			{
				case LineWidthTypeEnum::beam: return "beam"; break;
				case LineWidthTypeEnum::bracket: return "bracket"; break;
				case LineWidthTypeEnum::dashes: return "dashes"; break;
				case LineWidthTypeEnum::enclosure: return "enclosure"; break;
				case LineWidthTypeEnum::ending: return "ending"; break;
				case LineWidthTypeEnum::extend: return "extend"; break;
				case LineWidthTypeEnum::heavyBarline: return "heavy barline"; break;
				case LineWidthTypeEnum::leger: return "leger"; break;
				case LineWidthTypeEnum::lightBarline: return "light barline"; break;
				case LineWidthTypeEnum::octaveShift: return "octave shift"; break;
				case LineWidthTypeEnum::pedal: return "pedal"; break;
				case LineWidthTypeEnum::slurMiddle: return "slur middle"; break;
				case LineWidthTypeEnum::slurTip: return "slur tip"; break;
				case LineWidthTypeEnum::staff: return "staff"; break;
				case LineWidthTypeEnum::stem: return "stem"; break;
				case LineWidthTypeEnum::tieMiddle: return "tie middle"; break;
				case LineWidthTypeEnum::tieTip: return "tie tip"; break;
				case LineWidthTypeEnum::tupletBracket: return "tuplet bracket"; break;
				case LineWidthTypeEnum::wedge: return "wedge"; break;
				case LineWidthTypeEnum::other: return "other"; break;
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
        :myEnum( value )
        ,myCustomValue( "" )
        {
            setValue( value );
        }
        LineWidthType::LineWidthType( const std::string& value )
        :myEnum( LineWidthTypeEnum::other )
        ,myCustomValue( value )
        {
            setValue( value );
        }
        LineWidthType::LineWidthType()
        :myEnum( LineWidthTypeEnum::beam )
        ,myCustomValue( "" )
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
        }
        void LineWidthType::setValue( const std::string& value )
        {
            bool found = false;
            LineWidthTypeEnum temp = parseLineWidthTypeEnum( value, found );
            if ( found )
            {
                myEnum = temp;
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

        XlinkActuate parseXlinkActuate( const std::string& value )
		{
			if ( value == "onLoad" ) { return XlinkActuate::onLoad; }
			else if ( value == "onRequest" ) { return XlinkActuate::onRequest; }
			else if ( value == "other" ) { return XlinkActuate::other; }
			else if ( value == "none" ) { return XlinkActuate::none; }
			return XlinkActuate::onLoad;
		}
		std::string toString( const XlinkActuate value )
		{
			switch ( value )
			{
				case XlinkActuate::onLoad: return "onLoad"; break;
				case XlinkActuate::onRequest: return "onRequest"; break;
				case XlinkActuate::other: return "other"; break;
				case XlinkActuate::none: return "none"; break;
				default: break;
			}
			return "none";
		}
		std::ostream& toStream( std::ostream& os, const XlinkActuate value )
		{
			return os << toString( value );
		}
		std::ostream& operator<<( std::ostream& os, const XlinkActuate value )
		{
			return toStream( os, value );
		}
        
        XlinkShow parseXlinkShow( const std::string& value )
		{
			if ( value == "new" ) { return XlinkShow::new_; }
			else if ( value == "replace" ) { return XlinkShow::replace; }
			else if ( value == "embed" ) { return XlinkShow::embed; }
			else if ( value == "other" ) { return XlinkShow::other; }
			else if ( value == "none" ) { return XlinkShow::none; }
			return XlinkShow::new_;
		}
		std::string toString( const XlinkShow value )
		{
			switch ( value )
			{
				case XlinkShow::new_: return "new"; break;
				case XlinkShow::replace: return "replace"; break;
				case XlinkShow::embed: return "embed"; break;
				case XlinkShow::other: return "other"; break;
				case XlinkShow::none: return "none"; break;
				default: break;
			}
			return "none";
		}
		std::ostream& toStream( std::ostream& os, const XlinkShow value )
		{
			return os << toString( value );
		}
		std::ostream& operator<<( std::ostream& os, const XlinkShow value )
		{
			return toStream( os, value );
		}
        
        XlinkType parseXlinkType( const std::string& value )
		{
			if ( value == "simple" ) { return XlinkType::simple; }
			else if ( value == "extended" ) { return XlinkType::extended; }
			else if ( value == "title" ) { return XlinkType::title; }
			else if ( value == "resource" ) { return XlinkType::resource; }
			else if ( value == "locator" ) { return XlinkType::locator; }
			else if ( value == "arc" ) { return XlinkType::arc; }
			return XlinkType::simple;
		}
		std::string toString( const XlinkType value )
		{
			switch ( value )
			{
				case XlinkType::simple: return "simple"; break;
				case XlinkType::extended: return "extended"; break;
				case XlinkType::title: return "title"; break;
				case XlinkType::resource: return "resource"; break;
				case XlinkType::locator: return "locator"; break;
				case XlinkType::arc: return "arc"; break;
				default: break;
			}
			return "simple";
		}
		std::ostream& toStream( std::ostream& os, const XlinkType value )
		{
			return os << toString( value );
		}
		std::ostream& operator<<( std::ostream& os, const XlinkType value )
		{
			return toStream( os, value );
		}
        
        XmlSpace parseXmlSpace( const std::string& value )
		{
			if ( value == "default" ) { return XmlSpace::default_; }
			else if ( value == "preserve" ) { return XmlSpace::preserve; }
			return XmlSpace::default_;
		}
		std::string toString( const XmlSpace value )
		{
			switch ( value )
			{
				case XmlSpace::default_: return "default"; break;
				case XmlSpace::preserve: return "preserve"; break;
				default: break;
			}
			return "default";
		}
		std::ostream& toStream( std::ostream& os, const XmlSpace value )
		{
			return os << toString( value );
		}
		std::ostream& operator<<( std::ostream& os, const XmlSpace value )
		{
			return toStream( os, value );
		}
        
        DynamicsEnum parseDynamicsEnum( const std::string& value, bool& success )
		{
            success = true;
			if ( value == "p" ) { return DynamicsEnum::p; }
			else if ( value == "pp" ) { return DynamicsEnum::pp; }
			else if ( value == "ppp" ) { return DynamicsEnum::ppp; }
			else if ( value == "pppp" ) { return DynamicsEnum::pppp; }
			else if ( value == "ppppp" ) { return DynamicsEnum::ppppp; }
			else if ( value == "pppppp" ) { return DynamicsEnum::pppppp; }
			else if ( value == "f" ) { return DynamicsEnum::f; }
			else if ( value == "ff" ) { return DynamicsEnum::ff; }
			else if ( value == "fff" ) { return DynamicsEnum::fff; }
			else if ( value == "ffff" ) { return DynamicsEnum::ffff; }
			else if ( value == "fffff" ) { return DynamicsEnum::fffff; }
			else if ( value == "ffffff" ) { return DynamicsEnum::ffffff; }
			else if ( value == "mp" ) { return DynamicsEnum::mp; }
			else if ( value == "mf" ) { return DynamicsEnum::mf; }
			else if ( value == "sf" ) { return DynamicsEnum::sf; }
			else if ( value == "sfp" ) { return DynamicsEnum::sfp; }
			else if ( value == "sfpp" ) { return DynamicsEnum::sfpp; }
			else if ( value == "fp" ) { return DynamicsEnum::fp; }
			else if ( value == "rf" ) { return DynamicsEnum::rf; }
			else if ( value == "rfz" ) { return DynamicsEnum::rfz; }
			else if ( value == "sfz" ) { return DynamicsEnum::sfz; }
			else if ( value == "sffz" ) { return DynamicsEnum::sffz; }
			else if ( value == "fz" ) { return DynamicsEnum::fz; }
			else if ( value == "other-dynamics" ) { return DynamicsEnum::otherDynamics; }
			success = false;
            return DynamicsEnum::otherDynamics;
		}
        DynamicsEnum parseDynamicsEnum( const std::string& value )
        {
            bool success = true;
            return parseDynamicsEnum( value, success );
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
                case DynamicsEnum::otherDynamics: { return "other-dynamics"; }
				default: break;
			}
			return "default";
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
        :myEnum( value )
        ,myCustomValue( "" )
        {
            setValue( value );
        }
        DynamicsValue::DynamicsValue( const std::string& value )
        :myEnum( DynamicsEnum::otherDynamics )
        ,myCustomValue( value )
        {
            setValue( value );
        }
        DynamicsValue::DynamicsValue()
        :myEnum( DynamicsEnum::mf )
        ,myCustomValue( "" )
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
        }
        void DynamicsValue::setValue( const std::string& value )
        {
            bool found = false;
            DynamicsEnum temp = parseDynamicsEnum( value, found );
            if ( found )
            {
                myEnum = temp;
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
        
        
        /***************************************************************************************************************************
         ***************************************************************************************************************************
         ***************************************************************************************************************************
         ***************************************************************************************************************************
         **********************      Value Elements with NO Attributes *************************************************************
         ***************************************************************************************************************************
         ***************************************************************************************************************************
         ***************************************************************************************************************************
         ***************************************************************************************************************************/
        

	} // namespace core

} // namespace mx
