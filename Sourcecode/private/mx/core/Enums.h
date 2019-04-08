// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <iostream>
#include <string>

namespace mx
{
	namespace core
	{
		enum class AboveBelow
		{
			above = 0,
			below = 1
		};
		AboveBelow parseAboveBelow( const std::string& value );
		std::string toString( const AboveBelow value );
		std::ostream& toStream( std::ostream& os, const AboveBelow value );
		std::ostream& operator<<( std::ostream& os, const AboveBelow value );

		enum class CssFontSize
		{
			xxSmall = 0,
			xSmall = 1,
			small = 2,
			medium = 3,
			large = 4,
			xLarge = 5,
			xxLarge = 6
		};
		CssFontSize parseCssFontSize( const std::string& value );
		std::string toString( const CssFontSize value );
		std::ostream& toStream( std::ostream& os, const CssFontSize value );
		std::ostream& operator<<( std::ostream& os, const CssFontSize value );

		enum class EnclosureShape
		{
			rectangle = 0,
			square = 1,
			oval = 2,
			circle = 3,
			bracket = 4,
			triangle = 5,
			diamond = 6,
			none = 7
		};
		EnclosureShape parseEnclosureShape( const std::string& value );
		std::string toString( const EnclosureShape value );
		std::ostream& toStream( std::ostream& os, const EnclosureShape value );
		std::ostream& operator<<( std::ostream& os, const EnclosureShape value );

		enum class FermataShape
		{
			normal = 0,
			angled = 1,
			square = 2,
			emptystring = 3
		};
		FermataShape parseFermataShape( const std::string& value );
		std::string toString( const FermataShape value );
		std::ostream& toStream( std::ostream& os, const FermataShape value );
		std::ostream& operator<<( std::ostream& os, const FermataShape value );

		enum class FontStyle
		{
			normal = 0,
			italic = 1
		};
		FontStyle parseFontStyle( const std::string& value );
		std::string toString( const FontStyle value );
		std::ostream& toStream( std::ostream& os, const FontStyle value );
		std::ostream& operator<<( std::ostream& os, const FontStyle value );

		enum class FontWeight
		{
			normal = 0,
			bold = 1
		};
		FontWeight parseFontWeight( const std::string& value );
		std::string toString( const FontWeight value );
		std::ostream& toStream( std::ostream& os, const FontWeight value );
		std::ostream& operator<<( std::ostream& os, const FontWeight value );

		enum class LeftCenterRight
		{
			left = 0,
			center = 1,
			right = 2
		};
		LeftCenterRight parseLeftCenterRight( const std::string& value );
		std::string toString( const LeftCenterRight value );
		std::ostream& toStream( std::ostream& os, const LeftCenterRight value );
		std::ostream& operator<<( std::ostream& os, const LeftCenterRight value );

		enum class LeftRight
		{
			left = 0,
			right = 1
		};
		LeftRight parseLeftRight( const std::string& value );
		std::string toString( const LeftRight value );
		std::ostream& toStream( std::ostream& os, const LeftRight value );
		std::ostream& operator<<( std::ostream& os, const LeftRight value );

		enum class LineShape
		{
			straight = 0,
			curved = 1
		};
		LineShape parseLineShape( const std::string& value );
		std::string toString( const LineShape value );
		std::ostream& toStream( std::ostream& os, const LineShape value );
		std::ostream& operator<<( std::ostream& os, const LineShape value );

		enum class LineType
		{
			solid = 0,
			dashed = 1,
			dotted = 2,
			wavy = 3
		};
		LineType parseLineType( const std::string& value );
		std::string toString( const LineType value );
		std::ostream& toStream( std::ostream& os, const LineType value );
		std::ostream& operator<<( std::ostream& os, const LineType value );

		enum class MuteEnum
		{
			on = 0,
			off = 1,
			straight = 2,
			cup = 3,
			harmonNoStem = 4,
			harmonStem = 5,
			bucket = 6,
			plunger = 7,
			hat = 8,
			solotone = 9,
			practice = 10,
			stopMute = 11,
			stopHand = 12,
			echo = 13,
			palm = 14
		};
		MuteEnum parseMuteEnum( const std::string& value );
		std::string toString( const MuteEnum value );
		std::ostream& toStream( std::ostream& os, const MuteEnum value );
		std::ostream& operator<<( std::ostream& os, const MuteEnum value );

		enum class OverUnder
		{
			over = 0,
			under = 1
		};
		OverUnder parseOverUnder( const std::string& value );
		std::string toString( const OverUnder value );
		std::ostream& toStream( std::ostream& os, const OverUnder value );
		std::ostream& operator<<( std::ostream& os, const OverUnder value );

		enum class SemiPitchedEnum
		{
			high = 0,
			mediumHigh = 1,
			medium = 2,
			mediumLow = 3,
			low = 4,
			veryLow = 5
		};
		SemiPitchedEnum parseSemiPitchedEnum( const std::string& value );
		std::string toString( const SemiPitchedEnum value );
		std::ostream& toStream( std::ostream& os, const SemiPitchedEnum value );
		std::ostream& operator<<( std::ostream& os, const SemiPitchedEnum value );

		enum class StartNote
		{
			upper = 0,
			main = 1,
			below = 2
		};
		StartNote parseStartNote( const std::string& value );
		std::string toString( const StartNote value );
		std::ostream& toStream( std::ostream& os, const StartNote value );
		std::ostream& operator<<( std::ostream& os, const StartNote value );

		enum class StartStop
		{
			start = 0,
			stop = 1
		};
		StartStop parseStartStop( const std::string& value );
		std::string toString( const StartStop value );
		std::ostream& toStream( std::ostream& os, const StartStop value );
		std::ostream& operator<<( std::ostream& os, const StartStop value );

		enum class StartStopContinue
		{
			start = 0,
			stop = 1,
			continue_ = 2
		};
		StartStopContinue parseStartStopContinue( const std::string& value );
		std::string toString( const StartStopContinue value );
		std::ostream& toStream( std::ostream& os, const StartStopContinue value );
		std::ostream& operator<<( std::ostream& os, const StartStopContinue value );

		enum class StartStopSingle
		{
			start = 0,
			stop = 1,
			single = 2
		};
		StartStopSingle parseStartStopSingle( const std::string& value );
		std::string toString( const StartStopSingle value );
		std::ostream& toStream( std::ostream& os, const StartStopSingle value );
		std::ostream& operator<<( std::ostream& os, const StartStopSingle value );

		enum class SymbolSize
		{
			full = 0,
			cue = 1,
			large = 2
		};
		SymbolSize parseSymbolSize( const std::string& value );
		std::string toString( const SymbolSize value );
		std::ostream& toStream( std::ostream& os, const SymbolSize value );
		std::ostream& operator<<( std::ostream& os, const SymbolSize value );

		enum class TextDirection
		{
			ltr = 0,
			rtl = 1,
			lro = 2,
			rlo = 3
		};
		TextDirection parseTextDirection( const std::string& value );
		std::string toString( const TextDirection value );
		std::ostream& toStream( std::ostream& os, const TextDirection value );
		std::ostream& operator<<( std::ostream& os, const TextDirection value );

		enum class TopBottom
		{
			top = 0,
			bottom = 1
		};
		TopBottom parseTopBottom( const std::string& value );
		std::string toString( const TopBottom value );
		std::ostream& toStream( std::ostream& os, const TopBottom value );
		std::ostream& operator<<( std::ostream& os, const TopBottom value );

		enum class TrillStep
		{
			whole = 0,
			half = 1,
			unison = 2
		};
		TrillStep parseTrillStep( const std::string& value );
		std::string toString( const TrillStep value );
		std::ostream& toStream( std::ostream& os, const TrillStep value );
		std::ostream& operator<<( std::ostream& os, const TrillStep value );

		enum class TwoNoteTurn
		{
			whole = 0,
			half = 1,
			none = 2
		};
		TwoNoteTurn parseTwoNoteTurn( const std::string& value );
		std::string toString( const TwoNoteTurn value );
		std::ostream& toStream( std::ostream& os, const TwoNoteTurn value );
		std::ostream& operator<<( std::ostream& os, const TwoNoteTurn value );

		enum class UpDown
		{
			up = 0,
			down = 1
		};
		UpDown parseUpDown( const std::string& value );
		std::string toString( const UpDown value );
		std::ostream& toStream( std::ostream& os, const UpDown value );
		std::ostream& operator<<( std::ostream& os, const UpDown value );

		enum class UprightInverted
		{
			upright = 0,
			inverted = 1
		};
		UprightInverted parseUprightInverted( const std::string& value );
		std::string toString( const UprightInverted value );
		std::ostream& toStream( std::ostream& os, const UprightInverted value );
		std::ostream& operator<<( std::ostream& os, const UprightInverted value );

		enum class Valign
		{
			top = 0,
			middle = 1,
			bottom = 2,
			baseline = 3
		};
		Valign parseValign( const std::string& value );
		std::string toString( const Valign value );
		std::ostream& toStream( std::ostream& os, const Valign value );
		std::ostream& operator<<( std::ostream& os, const Valign value );

		enum class ValignImage
		{
			top = 0,
			middle = 1,
			bottom = 2
		};
		ValignImage parseValignImage( const std::string& value );
		std::string toString( const ValignImage value );
		std::ostream& toStream( std::ostream& os, const ValignImage value );
		std::ostream& operator<<( std::ostream& os, const ValignImage value );

		enum class YesNo
		{
			yes = 0,
			no = 1
		};
		YesNo parseYesNo( const std::string& value );
		std::string toString( const YesNo value );
		std::ostream& toStream( std::ostream& os, const YesNo value );
		std::ostream& operator<<( std::ostream& os, const YesNo value );

		enum class CancelLocation
		{
			left = 0,
			right = 1,
			beforeBarline = 2
		};
		CancelLocation parseCancelLocation( const std::string& value );
		std::string toString( const CancelLocation value );
		std::ostream& toStream( std::ostream& os, const CancelLocation value );
		std::ostream& operator<<( std::ostream& os, const CancelLocation value );

		enum class ClefSign
		{
			g = 0,
			f = 1,
			c = 2,
			percussion = 3,
			tab = 4,
			jianpu = 5,
			none = 6
		};
		ClefSign parseClefSign( const std::string& value );
		std::string toString( const ClefSign value );
		std::ostream& toStream( std::ostream& os, const ClefSign value );
		std::ostream& operator<<( std::ostream& os, const ClefSign value );

		enum class ShowFrets
		{
			numbers = 0,
			letters = 1
		};
		ShowFrets parseShowFrets( const std::string& value );
		std::string toString( const ShowFrets value );
		std::ostream& toStream( std::ostream& os, const ShowFrets value );
		std::ostream& operator<<( std::ostream& os, const ShowFrets value );

		enum class StaffTypeEnum
		{
			ossia = 0,
			cue = 1,
			editorial = 2,
			regular = 3,
			alternate = 4
		};
		StaffTypeEnum parseStaffTypeEnum( const std::string& value );
		std::string toString( const StaffTypeEnum value );
		std::ostream& toStream( std::ostream& os, const StaffTypeEnum value );
		std::ostream& operator<<( std::ostream& os, const StaffTypeEnum value );

		enum class TimeRelationEnum
		{
			parentheses = 0,
			bracket = 1,
			equals = 2,
			slash = 3,
			space = 4,
			hyphen = 5
		};
		TimeRelationEnum parseTimeRelationEnum( const std::string& value );
		std::string toString( const TimeRelationEnum value );
		std::ostream& toStream( std::ostream& os, const TimeRelationEnum value );
		std::ostream& operator<<( std::ostream& os, const TimeRelationEnum value );

		enum class TimeSeparator
		{
			none = 0,
			horizontal = 1,
			diagonal = 2,
			vertical = 3,
			adjacent = 4
		};
		TimeSeparator parseTimeSeparator( const std::string& value );
		std::string toString( const TimeSeparator value );
		std::ostream& toStream( std::ostream& os, const TimeSeparator value );
		std::ostream& operator<<( std::ostream& os, const TimeSeparator value );

		enum class TimeSymbol
		{
			common = 0,
			cut = 1,
			singleNumber = 2,
			note = 3,
			dottedNote = 4,
			normal = 5
		};
		TimeSymbol parseTimeSymbol( const std::string& value );
		std::string toString( const TimeSymbol value );
		std::ostream& toStream( std::ostream& os, const TimeSymbol value );
		std::ostream& operator<<( std::ostream& os, const TimeSymbol value );

		enum class BackwardForward
		{
			backward = 0,
			forward = 1
		};
		BackwardForward parseBackwardForward( const std::string& value );
		std::string toString( const BackwardForward value );
		std::ostream& toStream( std::ostream& os, const BackwardForward value );
		std::ostream& operator<<( std::ostream& os, const BackwardForward value );

		enum class BarStyleEnum
		{
			regular = 0,
			dotted = 1,
			dashed = 2,
			heavy = 3,
			lightLight = 4,
			lightHeavy = 5,
			heavyLight = 6,
			heavyHeavy = 7,
			tick = 8,
			short_ = 9,
			none = 10
		};
		BarStyleEnum parseBarStyleEnum( const std::string& value );
		std::string toString( const BarStyleEnum value );
		std::ostream& toStream( std::ostream& os, const BarStyleEnum value );
		std::ostream& operator<<( std::ostream& os, const BarStyleEnum value );

		enum class RightLeftMiddle
		{
			right = 0,
			left = 1,
			middle = 2
		};
		RightLeftMiddle parseRightLeftMiddle( const std::string& value );
		std::string toString( const RightLeftMiddle value );
		std::ostream& toStream( std::ostream& os, const RightLeftMiddle value );
		std::ostream& operator<<( std::ostream& os, const RightLeftMiddle value );

		enum class StartStopDiscontinue
		{
			start = 0,
			stop = 1,
			discontinue = 2
		};
		StartStopDiscontinue parseStartStopDiscontinue( const std::string& value );
		std::string toString( const StartStopDiscontinue value );
		std::ostream& toStream( std::ostream& os, const StartStopDiscontinue value );
		std::ostream& operator<<( std::ostream& os, const StartStopDiscontinue value );

		enum class Winged
		{
			none = 0,
			straight = 1,
			curved = 2,
			doubleStraight = 3,
			doubleCurved = 4
		};
		Winged parseWinged( const std::string& value );
		std::string toString( const Winged value );
		std::ostream& toStream( std::ostream& os, const Winged value );
		std::ostream& operator<<( std::ostream& os, const Winged value );

		enum class BeaterValue
		{
			bow = 0,
			chimeHammer = 1,
			coin = 2,
			finger = 3,
			fingernail = 4,
			fist = 5,
			guiroScraper = 6,
			hammer = 7,
			hand = 8,
			jazzStick = 9,
			knittingNeedle = 10,
			metalHammer = 11,
			snareStick = 12,
			spoonMallet = 13,
			triangleBeater = 14,
			triangleBeaterPlain = 15,
			wireBrush = 16
		};
		BeaterValue parseBeaterValue( const std::string& value );
		std::string toString( const BeaterValue value );
		std::ostream& toStream( std::ostream& os, const BeaterValue value );
		std::ostream& operator<<( std::ostream& os, const BeaterValue value );

		enum class DegreeSymbolValue
		{
			major = 0,
			minor = 1,
			augmented = 2,
			diminished = 3,
			halfDiminished = 4
		};
		DegreeSymbolValue parseDegreeSymbolValue( const std::string& value );
		std::string toString( const DegreeSymbolValue value );
		std::ostream& toStream( std::ostream& os, const DegreeSymbolValue value );
		std::ostream& operator<<( std::ostream& os, const DegreeSymbolValue value );

		enum class DegreeTypeValue
		{
			add = 0,
			alter = 1,
			subtract = 2
		};
		DegreeTypeValue parseDegreeTypeValue( const std::string& value );
		std::string toString( const DegreeTypeValue value );
		std::ostream& toStream( std::ostream& os, const DegreeTypeValue value );
		std::ostream& operator<<( std::ostream& os, const DegreeTypeValue value );

		enum class EffectEnum
		{
			anvil = 0,
			autoHorn = 1,
			birdWhistle = 2,
			cannon = 3,
			duckCall = 4,
			gunShot = 5,
			klaxonHorn = 6,
			lionsRoar = 7,
			policeWhistle = 8,
			siren = 9,
			slideWhistle = 10,
			thunderSheet = 11,
			windMachine = 12,
			windWhistle = 13
		};
		EffectEnum parseEffectEnum( const std::string& value );
		std::string toString( const EffectEnum value );
		std::ostream& toStream( std::ostream& os, const EffectEnum value );
		std::ostream& operator<<( std::ostream& os, const EffectEnum value );

		enum class GlassEnum
		{
			windChimes = 0
		};
		GlassEnum parseGlassEnum( const std::string& value );
		std::string toString( const GlassEnum value );
		std::ostream& toStream( std::ostream& os, const GlassEnum value );
		std::ostream& operator<<( std::ostream& os, const GlassEnum value );

		enum class HarmonyType
		{
			explicit_ = 0,
			implied = 1,
			alternate = 2
		};
		HarmonyType parseHarmonyType( const std::string& value );
		std::string toString( const HarmonyType value );
		std::ostream& toStream( std::ostream& os, const HarmonyType value );
		std::ostream& operator<<( std::ostream& os, const HarmonyType value );

		enum class KindValue
		{
			major = 0,
			minor = 1,
			augmented = 2,
			diminished = 3,
			dominant = 4,
			majorSeventh = 5,
			minorSeventh = 6,
			diminishedSeventh = 7,
			augmentedSeventh = 8,
			halfDiminished = 9,
			majorMinor = 10,
			majorSixth = 11,
			minorSixth = 12,
			dominantNinth = 13,
			majorNinth = 14,
			minorNinth = 15,
			dominant11Th = 16,
			major11Th = 17,
			minor11Th = 18,
			dominant13Th = 19,
			major13Th = 20,
			minor13Th = 21,
			suspendedSecond = 22,
			suspendedFourth = 23,
			neapolitan = 24,
			italian = 25,
			french = 26,
			german = 27,
			pedal = 28,
			power = 29,
			tristan = 30,
			other = 31,
			none = 32
		};
		KindValue parseKindValue( const std::string& value );
		std::string toString( const KindValue value );
		std::ostream& toStream( std::ostream& os, const KindValue value );
		std::ostream& operator<<( std::ostream& os, const KindValue value );

		enum class LineEnd
		{
			up = 0,
			down = 1,
			both = 2,
			arrow = 3,
			none = 4
		};
		LineEnd parseLineEnd( const std::string& value );
		std::string toString( const LineEnd value );
		std::ostream& toStream( std::ostream& os, const LineEnd value );
		std::ostream& operator<<( std::ostream& os, const LineEnd value );

		enum class MeasureNumberingValue
		{
			none = 0,
			measure = 1,
			system = 2
		};
		MeasureNumberingValue parseMeasureNumberingValue( const std::string& value );
		std::string toString( const MeasureNumberingValue value );
		std::ostream& toStream( std::ostream& os, const MeasureNumberingValue value );
		std::ostream& operator<<( std::ostream& os, const MeasureNumberingValue value );

		enum class MembraneEnum
		{
			bassDrum = 0,
			bassDrumOnSide = 1,
			bongos = 2,
			congaDrum = 3,
			gobletDrum = 4,
			militaryDrum = 5,
			snareDrum = 6,
			snareDrumSnaresOff = 7,
			tambourine = 8,
			tenorDrum = 9,
			timbales = 10,
			tomtom = 11
		};
		MembraneEnum parseMembraneEnum( const std::string& value );
		std::string toString( const MembraneEnum value );
		std::ostream& toStream( std::ostream& os, const MembraneEnum value );
		std::ostream& operator<<( std::ostream& os, const MembraneEnum value );

		enum class MetalEnum
		{
			almglocken = 0,
			bell = 1,
			bellPlate = 2,
			brakeDrum = 3,
			chineseCymbal = 4,
			cowbell = 5,
			crashCymbals = 6,
			crotale = 7,
			cymbalTongs = 8,
			domedGong = 9,
			fingerCymbals = 10,
			flexatone = 11,
			gong = 12,
			hiHat = 13,
			highHatCymbals = 14,
			handbell = 15,
			sistrum = 16,
			sizzleCymbal = 17,
			sleighBells = 18,
			suspendedCymbal = 19,
			tamTam = 20,
			triangle = 21,
			vietnameseHat = 22
		};
		MetalEnum parseMetalEnum( const std::string& value );
		std::string toString( const MetalEnum value );
		std::ostream& toStream( std::ostream& os, const MetalEnum value );
		std::ostream& operator<<( std::ostream& os, const MetalEnum value );

		enum class OnOff
		{
			on = 0,
			off = 1
		};
		OnOff parseOnOff( const std::string& value );
		std::string toString( const OnOff value );
		std::ostream& toStream( std::ostream& os, const OnOff value );
		std::ostream& operator<<( std::ostream& os, const OnOff value );

		enum class PitchedEnum
		{
			chimes = 0,
			glockenspiel = 1,
			mallet = 2,
			marimba = 3,
			tubularChimes = 4,
			vibraphone = 5,
			xylophone = 6
		};
		PitchedEnum parsePitchedEnum( const std::string& value );
		std::string toString( const PitchedEnum value );
		std::ostream& toStream( std::ostream& os, const PitchedEnum value );
		std::ostream& operator<<( std::ostream& os, const PitchedEnum value );

        #define ZEUS_ENUM_CLASS_NAME PlaybackSound

        #define ZEUS_ENUM_MEMBERS( ZEUS_ENUM_VALUE )      \
        ZEUS_ENUM_VALUE( brassAlphorn, "brass.alphorn" ) \
        ZEUS_ENUM_VALUE( brassAltoHorn, "brass.alto-horn" ) \
        ZEUS_ENUM_VALUE( brassBaritoneHorn, "brass.baritone-horn" ) \
        ZEUS_ENUM_VALUE( brassBugle, "brass.bugle" ) \
        ZEUS_ENUM_VALUE( brassBugleAlto, "brass.bugle.alto" ) \
        ZEUS_ENUM_VALUE( brassBugleBaritone, "brass.bugle.baritone" ) \
        ZEUS_ENUM_VALUE( brassBugleContrabass, "brass.bugle.contrabass" ) \
        ZEUS_ENUM_VALUE( brassBugleEuphoniumBugle, "brass.bugle.euphonium-bugle" ) \
        ZEUS_ENUM_VALUE( brassBugleMellophoneBugle, "brass.bugle.mellophone-bugle" ) \
        ZEUS_ENUM_VALUE( brassBugleSoprano, "brass.bugle.soprano" ) \
        ZEUS_ENUM_VALUE( brassCimbasso, "brass.cimbasso" ) \
        ZEUS_ENUM_VALUE( brassConchShell, "brass.conch-shell" ) \
        ZEUS_ENUM_VALUE( brassCornet, "brass.cornet" ) \
        ZEUS_ENUM_VALUE( brassCornetSoprano, "brass.cornet.soprano" ) \
        ZEUS_ENUM_VALUE( brassCornett, "brass.cornett" ) \
        ZEUS_ENUM_VALUE( brassCornettTenor, "brass.cornett.tenor" ) \
        ZEUS_ENUM_VALUE( brassCornettino, "brass.cornettino" ) \
        ZEUS_ENUM_VALUE( brassDidgeridoo, "brass.didgeridoo" ) \
        ZEUS_ENUM_VALUE( brassEuphonium, "brass.euphonium" ) \
        ZEUS_ENUM_VALUE( brassFiscorn, "brass.fiscorn" ) \
        ZEUS_ENUM_VALUE( brassFlugelhorn, "brass.flugelhorn" ) \
        ZEUS_ENUM_VALUE( brassFrenchHorn, "brass.french-horn" ) \
        ZEUS_ENUM_VALUE( brassGroup, "brass.group" ) \
        ZEUS_ENUM_VALUE( brassGroupSynth, "brass.group.synth" ) \
        ZEUS_ENUM_VALUE( brassHelicon, "brass.helicon" ) \
        ZEUS_ENUM_VALUE( brassHoragai, "brass.horagai" ) \
        ZEUS_ENUM_VALUE( brassKuhlohorn, "brass.kuhlohorn" ) \
        ZEUS_ENUM_VALUE( brassMellophone, "brass.mellophone" ) \
        ZEUS_ENUM_VALUE( brassNaturalHorn, "brass.natural-horn" ) \
        ZEUS_ENUM_VALUE( brassOphicleide, "brass.ophicleide" ) \
        ZEUS_ENUM_VALUE( brassPosthorn, "brass.posthorn" ) \
        ZEUS_ENUM_VALUE( brassRagDung, "brass.rag-dung" ) \
        ZEUS_ENUM_VALUE( brassSackbutt, "brass.sackbutt" ) \
        ZEUS_ENUM_VALUE( brassSackbuttAlto, "brass.sackbutt.alto" ) \
        ZEUS_ENUM_VALUE( brassSackbuttBass, "brass.sackbutt.bass" ) \
        ZEUS_ENUM_VALUE( brassSackbuttTenor, "brass.sackbutt.tenor" ) \
        ZEUS_ENUM_VALUE( brassSaxhorn, "brass.saxhorn" ) \
        ZEUS_ENUM_VALUE( brassSerpent, "brass.serpent" ) \
        ZEUS_ENUM_VALUE( brassShofar, "brass.shofar" ) \
        ZEUS_ENUM_VALUE( brassSousaphone, "brass.sousaphone" ) \
        ZEUS_ENUM_VALUE( brassTrombone, "brass.trombone" ) \
        ZEUS_ENUM_VALUE( brassTromboneAlto, "brass.trombone.alto" ) \
        ZEUS_ENUM_VALUE( brassTromboneBass, "brass.trombone.bass" ) \
        ZEUS_ENUM_VALUE( brassTromboneContrabass, "brass.trombone.contrabass" ) \
        ZEUS_ENUM_VALUE( brassTromboneTenor, "brass.trombone.tenor" ) \
        ZEUS_ENUM_VALUE( brassTrumpet, "brass.trumpet" ) \
        ZEUS_ENUM_VALUE( brassTrumpetBaroque, "brass.trumpet.baroque" ) \
        ZEUS_ENUM_VALUE( brassTrumpetBass, "brass.trumpet.bass" ) \
        ZEUS_ENUM_VALUE( brassTrumpetBflat, "brass.trumpet.bflat" ) \
        ZEUS_ENUM_VALUE( brassTrumpetC, "brass.trumpet.c" ) \
        ZEUS_ENUM_VALUE( brassTrumpetD, "brass.trumpet.d" ) \
        ZEUS_ENUM_VALUE( brassTrumpetPiccolo, "brass.trumpet.piccolo" ) \
        ZEUS_ENUM_VALUE( brassTrumpetPocket, "brass.trumpet.pocket" ) \
        ZEUS_ENUM_VALUE( brassTrumpetSlide, "brass.trumpet.slide" ) \
        ZEUS_ENUM_VALUE( brassTrumpetTenor, "brass.trumpet.tenor" ) \
        ZEUS_ENUM_VALUE( brassTuba, "brass.tuba" ) \
        ZEUS_ENUM_VALUE( brassTubaBass, "brass.tuba.bass" ) \
        ZEUS_ENUM_VALUE( brassTubaSubcontrabass, "brass.tuba.subcontrabass" ) \
        ZEUS_ENUM_VALUE( brassViennaHorn, "brass.vienna-horn" ) \
        ZEUS_ENUM_VALUE( brassVuvuzela, "brass.vuvuzela" ) \
        ZEUS_ENUM_VALUE( brassWagnerTuba, "brass.wagner-tuba" ) \
        ZEUS_ENUM_VALUE( drumApentemma, "drum.apentemma" ) \
        ZEUS_ENUM_VALUE( drumAshiko, "drum.ashiko" ) \
        ZEUS_ENUM_VALUE( drumAtabaque, "drum.atabaque" ) \
        ZEUS_ENUM_VALUE( drumAtoke, "drum.atoke" ) \
        ZEUS_ENUM_VALUE( drumAtsimevu, "drum.atsimevu" ) \
        ZEUS_ENUM_VALUE( drumAxatse, "drum.axatse" ) \
        ZEUS_ENUM_VALUE( drumBassDrum, "drum.bass-drum" ) \
        ZEUS_ENUM_VALUE( drumBata, "drum.bata" ) \
        ZEUS_ENUM_VALUE( drumBataItotele, "drum.bata.itotele" ) \
        ZEUS_ENUM_VALUE( drumBataIya, "drum.bata.iya" ) \
        ZEUS_ENUM_VALUE( drumBataOkonkolo, "drum.bata.okonkolo" ) \
        ZEUS_ENUM_VALUE( drumBendir, "drum.bendir" ) \
        ZEUS_ENUM_VALUE( drumBodhran, "drum.bodhran" ) \
        ZEUS_ENUM_VALUE( drumBombo, "drum.bombo" ) \
        ZEUS_ENUM_VALUE( drumBongo, "drum.bongo" ) \
        ZEUS_ENUM_VALUE( drumBougarabou, "drum.bougarabou" ) \
        ZEUS_ENUM_VALUE( drumBuffaloDrum, "drum.buffalo-drum" ) \
        ZEUS_ENUM_VALUE( drumCajon, "drum.cajon" ) \
        ZEUS_ENUM_VALUE( drumChenda, "drum.chenda" ) \
        ZEUS_ENUM_VALUE( drumChuDaiko, "drum.chu-daiko" ) \
        ZEUS_ENUM_VALUE( drumConga, "drum.conga" ) \
        ZEUS_ENUM_VALUE( drumCuica, "drum.cuica" ) \
        ZEUS_ENUM_VALUE( drumDabakan, "drum.dabakan" ) \
        ZEUS_ENUM_VALUE( drumDaff, "drum.daff" ) \
        ZEUS_ENUM_VALUE( drumDafli, "drum.dafli" ) \
        ZEUS_ENUM_VALUE( drumDaibyosi, "drum.daibyosi" ) \
        ZEUS_ENUM_VALUE( drumDamroo, "drum.damroo" ) \
        ZEUS_ENUM_VALUE( drumDarabuka, "drum.darabuka" ) \
        ZEUS_ENUM_VALUE( drumDef, "drum.def" ) \
        ZEUS_ENUM_VALUE( drumDhol, "drum.dhol" ) \
        ZEUS_ENUM_VALUE( drumDholak, "drum.dholak" ) \
        ZEUS_ENUM_VALUE( drumDjembe, "drum.djembe" ) \
        ZEUS_ENUM_VALUE( drumDoira, "drum.doira" ) \
        ZEUS_ENUM_VALUE( drumDondo, "drum.dondo" ) \
        ZEUS_ENUM_VALUE( drumDounDounBa, "drum.doun-doun-ba" ) \
        ZEUS_ENUM_VALUE( drumDuff, "drum.duff" ) \
        ZEUS_ENUM_VALUE( drumDumbek, "drum.dumbek" ) \
        ZEUS_ENUM_VALUE( drumFontomfrom, "drum.fontomfrom" ) \
        ZEUS_ENUM_VALUE( drumFrameDrum, "drum.frame-drum" ) \
        ZEUS_ENUM_VALUE( drumFrameDrumArabian, "drum.frame-drum.arabian" ) \
        ZEUS_ENUM_VALUE( drumGeduk, "drum.geduk" ) \
        ZEUS_ENUM_VALUE( drumGhatam, "drum.ghatam" ) \
        ZEUS_ENUM_VALUE( drumGome, "drum.gome" ) \
        ZEUS_ENUM_VALUE( drumGroup, "drum.group" ) \
        ZEUS_ENUM_VALUE( drumGroupChinese, "drum.group.chinese" ) \
        ZEUS_ENUM_VALUE( drumGroupEwe, "drum.group.ewe" ) \
        ZEUS_ENUM_VALUE( drumGroupIndian, "drum.group.indian" ) \
        ZEUS_ENUM_VALUE( drumGroupSet, "drum.group.set" ) \
        ZEUS_ENUM_VALUE( drumHandDrum, "drum.hand-drum" ) \
        ZEUS_ENUM_VALUE( drumHiraDaiko, "drum.hira-daiko" ) \
        ZEUS_ENUM_VALUE( drumIbo, "drum.ibo" ) \
        ZEUS_ENUM_VALUE( drumIgihumurizo, "drum.igihumurizo" ) \
        ZEUS_ENUM_VALUE( drumInyahura, "drum.inyahura" ) \
        ZEUS_ENUM_VALUE( drumIshakwe, "drum.ishakwe" ) \
        ZEUS_ENUM_VALUE( drumJangGu, "drum.jang-gu" ) \
        ZEUS_ENUM_VALUE( drumKagan, "drum.kagan" ) \
        ZEUS_ENUM_VALUE( drumKakko, "drum.kakko" ) \
        ZEUS_ENUM_VALUE( drumKanjira, "drum.kanjira" ) \
        ZEUS_ENUM_VALUE( drumKendhang, "drum.kendhang" ) \
        ZEUS_ENUM_VALUE( drumKendhangAgeng, "drum.kendhang.ageng" ) \
        ZEUS_ENUM_VALUE( drumKendhangCiblon, "drum.kendhang.ciblon" ) \
        ZEUS_ENUM_VALUE( drumKenkeni, "drum.kenkeni" ) \
        ZEUS_ENUM_VALUE( drumKhol, "drum.khol" ) \
        ZEUS_ENUM_VALUE( drumKickDrum, "drum.kick-drum" ) \
        ZEUS_ENUM_VALUE( drumKidi, "drum.kidi" ) \
        ZEUS_ENUM_VALUE( drumKoDaiko, "drum.ko-daiko" ) \
        ZEUS_ENUM_VALUE( drumKpanlogo, "drum.kpanlogo" ) \
        ZEUS_ENUM_VALUE( drumKudum, "drum.kudum" ) \
        ZEUS_ENUM_VALUE( drumLambeg, "drum.lambeg" ) \
        ZEUS_ENUM_VALUE( drumLionDrum, "drum.lion-drum" ) \
        ZEUS_ENUM_VALUE( drumLogDrum, "drum.log-drum" ) \
        ZEUS_ENUM_VALUE( drumLogDrumAfrican, "drum.log-drum.african" ) \
        ZEUS_ENUM_VALUE( drumLogDrumNative, "drum.log-drum.native" ) \
        ZEUS_ENUM_VALUE( drumLogDrumNigerian, "drum.log-drum.nigerian" ) \
        ZEUS_ENUM_VALUE( drumMadal, "drum.madal" ) \
        ZEUS_ENUM_VALUE( drumMaddale, "drum.maddale" ) \
        ZEUS_ENUM_VALUE( drumMridangam, "drum.mridangam" ) \
        ZEUS_ENUM_VALUE( drumNaal, "drum.naal" ) \
        ZEUS_ENUM_VALUE( drumNagadoDaiko, "drum.nagado-daiko" ) \
        ZEUS_ENUM_VALUE( drumNagara, "drum.nagara" ) \
        ZEUS_ENUM_VALUE( drumNaqara, "drum.naqara" ) \
        ZEUS_ENUM_VALUE( drumODaiko, "drum.o-daiko" ) \
        ZEUS_ENUM_VALUE( drumOkawa, "drum.okawa" ) \
        ZEUS_ENUM_VALUE( drumOkedoDaiko, "drum.okedo-daiko" ) \
        ZEUS_ENUM_VALUE( drumPahuHula, "drum.pahu-hula" ) \
        ZEUS_ENUM_VALUE( drumPakhawaj, "drum.pakhawaj" ) \
        ZEUS_ENUM_VALUE( drumPandeiro, "drum.pandeiro" ) \
        ZEUS_ENUM_VALUE( drumPandero, "drum.pandero" ) \
        ZEUS_ENUM_VALUE( drumPowwow, "drum.powwow" ) \
        ZEUS_ENUM_VALUE( drumPueblo, "drum.pueblo" ) \
        ZEUS_ENUM_VALUE( drumRepinique, "drum.repinique" ) \
        ZEUS_ENUM_VALUE( drumRiq, "drum.riq" ) \
        ZEUS_ENUM_VALUE( drumRototom, "drum.rototom" ) \
        ZEUS_ENUM_VALUE( drumSabar, "drum.sabar" ) \
        ZEUS_ENUM_VALUE( drumSakara, "drum.sakara" ) \
        ZEUS_ENUM_VALUE( drumSampho, "drum.sampho" ) \
        ZEUS_ENUM_VALUE( drumSangban, "drum.sangban" ) \
        ZEUS_ENUM_VALUE( drumShimeDaiko, "drum.shime-daiko" ) \
        ZEUS_ENUM_VALUE( drumSlitDrum, "drum.slit-drum" ) \
        ZEUS_ENUM_VALUE( drumSlitDrumKrin, "drum.slit-drum.krin" ) \
        ZEUS_ENUM_VALUE( drumSnareDrum, "drum.snare-drum" ) \
        ZEUS_ENUM_VALUE( drumSnareDrumElectric, "drum.snare-drum.electric" ) \
        ZEUS_ENUM_VALUE( drumSogo, "drum.sogo" ) \
        ZEUS_ENUM_VALUE( drumSurdo, "drum.surdo" ) \
        ZEUS_ENUM_VALUE( drumTabla, "drum.tabla" ) \
        ZEUS_ENUM_VALUE( drumTablaBayan, "drum.tabla.bayan" ) \
        ZEUS_ENUM_VALUE( drumTablaDayan, "drum.tabla.dayan" ) \
        ZEUS_ENUM_VALUE( drumTaiko, "drum.taiko" ) \
        ZEUS_ENUM_VALUE( drumTalking, "drum.talking" ) \
        ZEUS_ENUM_VALUE( drumTama, "drum.tama" ) \
        ZEUS_ENUM_VALUE( drumTamborita, "drum.tamborita" ) \
        ZEUS_ENUM_VALUE( drumTambourine, "drum.tambourine" ) \
        ZEUS_ENUM_VALUE( drumTamte, "drum.tamte" ) \
        ZEUS_ENUM_VALUE( drumTangku, "drum.tangku" ) \
        ZEUS_ENUM_VALUE( drumTanTan, "drum.tan-tan" ) \
        ZEUS_ENUM_VALUE( drumTaphon, "drum.taphon" ) \
        ZEUS_ENUM_VALUE( drumTar, "drum.tar" ) \
        ZEUS_ENUM_VALUE( drumTasha, "drum.tasha" ) \
        ZEUS_ENUM_VALUE( drumTenorDrum, "drum.tenor-drum" ) \
        ZEUS_ENUM_VALUE( drumTeponaxtli, "drum.teponaxtli" ) \
        ZEUS_ENUM_VALUE( drumThavil, "drum.thavil" ) \
        ZEUS_ENUM_VALUE( drumTheBox, "drum.the-box" ) \
        ZEUS_ENUM_VALUE( drumTimbale, "drum.timbale" ) \
        ZEUS_ENUM_VALUE( drumTimpani, "drum.timpani" ) \
        ZEUS_ENUM_VALUE( drumTinaja, "drum.tinaja" ) \
        ZEUS_ENUM_VALUE( drumToere, "drum.toere" ) \
        ZEUS_ENUM_VALUE( drumTombak, "drum.tombak" ) \
        ZEUS_ENUM_VALUE( drumTomTom, "drum.tom-tom" ) \
        ZEUS_ENUM_VALUE( drumTomTomSynth, "drum.tom-tom.synth" ) \
        ZEUS_ENUM_VALUE( drumTsuzumi, "drum.tsuzumi" ) \
        ZEUS_ENUM_VALUE( drumTumbak, "drum.tumbak" ) \
        ZEUS_ENUM_VALUE( drumUchiwaDaiko, "drum.uchiwa-daiko" ) \
        ZEUS_ENUM_VALUE( drumUdaku, "drum.udaku" ) \
        ZEUS_ENUM_VALUE( drumUdu, "drum.udu" ) \
        ZEUS_ENUM_VALUE( drumZarb, "drum.zarb" ) \
        ZEUS_ENUM_VALUE( effectAeolianHarp, "effect.aeolian-harp" ) \
        ZEUS_ENUM_VALUE( effectAirHorn, "effect.air-horn" ) \
        ZEUS_ENUM_VALUE( effectApplause, "effect.applause" ) \
        ZEUS_ENUM_VALUE( effectBassStringSlap, "effect.bass-string-slap" ) \
        ZEUS_ENUM_VALUE( effectBird, "effect.bird" ) \
        ZEUS_ENUM_VALUE( effectBirdNightingale, "effect.bird.nightingale" ) \
        ZEUS_ENUM_VALUE( effectBirdTweet, "effect.bird.tweet" ) \
        ZEUS_ENUM_VALUE( effectBreath, "effect.breath" ) \
        ZEUS_ENUM_VALUE( effectBubble, "effect.bubble" ) \
        ZEUS_ENUM_VALUE( effectBullroarer, "effect.bullroarer" ) \
        ZEUS_ENUM_VALUE( effectBurst, "effect.burst" ) \
        ZEUS_ENUM_VALUE( effectCar, "effect.car" ) \
        ZEUS_ENUM_VALUE( effectCarCrash, "effect.car.crash" ) \
        ZEUS_ENUM_VALUE( effectCarEngine, "effect.car.engine" ) \
        ZEUS_ENUM_VALUE( effectCarPass, "effect.car.pass" ) \
        ZEUS_ENUM_VALUE( effectCarStop, "effect.car.stop" ) \
        ZEUS_ENUM_VALUE( effectCrickets, "effect.crickets" ) \
        ZEUS_ENUM_VALUE( effectDog, "effect.dog" ) \
        ZEUS_ENUM_VALUE( effectDoorCreak, "effect.door.creak" ) \
        ZEUS_ENUM_VALUE( effectDoorSlam, "effect.door.slam" ) \
        ZEUS_ENUM_VALUE( effectExplosion, "effect.explosion" ) \
        ZEUS_ENUM_VALUE( effectFluteKeyClick, "effect.flute-key-click" ) \
        ZEUS_ENUM_VALUE( effectFootsteps, "effect.footsteps" ) \
        ZEUS_ENUM_VALUE( effectFrogs, "effect.frogs" ) \
        ZEUS_ENUM_VALUE( effectGuitarCutting, "effect.guitar-cutting" ) \
        ZEUS_ENUM_VALUE( effectGuitarFret, "effect.guitar-fret" ) \
        ZEUS_ENUM_VALUE( effectGunshot, "effect.gunshot" ) \
        ZEUS_ENUM_VALUE( effectHandClap, "effect.hand-clap" ) \
        ZEUS_ENUM_VALUE( effectHeartbeat, "effect.heartbeat" ) \
        ZEUS_ENUM_VALUE( effectHelicopter, "effect.helicopter" ) \
        ZEUS_ENUM_VALUE( effectHighQ, "effect.high-q" ) \
        ZEUS_ENUM_VALUE( effectHorseGallop, "effect.horse-gallop" ) \
        ZEUS_ENUM_VALUE( effectJetPlane, "effect.jet-plane" ) \
        ZEUS_ENUM_VALUE( effectLaserGun, "effect.laser-gun" ) \
        ZEUS_ENUM_VALUE( effectLaugh, "effect.laugh" ) \
        ZEUS_ENUM_VALUE( effectLionsRoar, "effect.lions-roar" ) \
        ZEUS_ENUM_VALUE( effectMachineGun, "effect.machine-gun" ) \
        ZEUS_ENUM_VALUE( effectMarchingMachine, "effect.marching-machine" ) \
        ZEUS_ENUM_VALUE( effectMetronomeBell, "effect.metronome-bell" ) \
        ZEUS_ENUM_VALUE( effectMetronomeClick, "effect.metronome-click" ) \
        ZEUS_ENUM_VALUE( effectPat, "effect.pat" ) \
        ZEUS_ENUM_VALUE( effectPunch, "effect.punch" ) \
        ZEUS_ENUM_VALUE( effectRain, "effect.rain" ) \
        ZEUS_ENUM_VALUE( effectScratch, "effect.scratch" ) \
        ZEUS_ENUM_VALUE( effectScream, "effect.scream" ) \
        ZEUS_ENUM_VALUE( effectSeashore, "effect.seashore" ) \
        ZEUS_ENUM_VALUE( effectSiren, "effect.siren" ) \
        ZEUS_ENUM_VALUE( effectSlap, "effect.slap" ) \
        ZEUS_ENUM_VALUE( effectSnap, "effect.snap" ) \
        ZEUS_ENUM_VALUE( effectStamp, "effect.stamp" ) \
        ZEUS_ENUM_VALUE( effectStarship, "effect.starship" ) \
        ZEUS_ENUM_VALUE( effectStream, "effect.stream" ) \
        ZEUS_ENUM_VALUE( effectTelephoneRing, "effect.telephone-ring" ) \
        ZEUS_ENUM_VALUE( effectThunder, "effect.thunder" ) \
        ZEUS_ENUM_VALUE( effectTrain, "effect.train" ) \
        ZEUS_ENUM_VALUE( effectTrashCan, "effect.trash-can" ) \
        ZEUS_ENUM_VALUE( effectWhip, "effect.whip" ) \
        ZEUS_ENUM_VALUE( effectWhistle, "effect.whistle" ) \
        ZEUS_ENUM_VALUE( effectWhistleMouthSiren, "effect.whistle.mouth-siren" ) \
        ZEUS_ENUM_VALUE( effectWhistlePolice, "effect.whistle.police" ) \
        ZEUS_ENUM_VALUE( effectWhistleSlide, "effect.whistle.slide" ) \
        ZEUS_ENUM_VALUE( effectWhistleTrain, "effect.whistle.train" ) \
        ZEUS_ENUM_VALUE( effectWind, "effect.wind" ) \
        ZEUS_ENUM_VALUE( keyboardAccordion, "keyboard.accordion" ) \
        ZEUS_ENUM_VALUE( keyboardBandoneon, "keyboard.bandoneon" ) \
        ZEUS_ENUM_VALUE( keyboardCelesta, "keyboard.celesta" ) \
        ZEUS_ENUM_VALUE( keyboardClavichord, "keyboard.clavichord" ) \
        ZEUS_ENUM_VALUE( keyboardClavichordSynth, "keyboard.clavichord.synth" ) \
        ZEUS_ENUM_VALUE( keyboardConcertina, "keyboard.concertina" ) \
        ZEUS_ENUM_VALUE( keyboardFortepiano, "keyboard.fortepiano" ) \
        ZEUS_ENUM_VALUE( keyboardHarmonium, "keyboard.harmonium" ) \
        ZEUS_ENUM_VALUE( keyboardHarpsichord, "keyboard.harpsichord" ) \
        ZEUS_ENUM_VALUE( keyboardOndesMartenot, "keyboard.ondes-martenot" ) \
        ZEUS_ENUM_VALUE( keyboardOrgan, "keyboard.organ" ) \
        ZEUS_ENUM_VALUE( keyboardOrganDrawbar, "keyboard.organ.drawbar" ) \
        ZEUS_ENUM_VALUE( keyboardOrganPercussive, "keyboard.organ.percussive" ) \
        ZEUS_ENUM_VALUE( keyboardOrganPipe, "keyboard.organ.pipe" ) \
        ZEUS_ENUM_VALUE( keyboardOrganReed, "keyboard.organ.reed" ) \
        ZEUS_ENUM_VALUE( keyboardOrganRotary, "keyboard.organ.rotary" ) \
        ZEUS_ENUM_VALUE( keyboardPiano, "keyboard.piano" ) \
        ZEUS_ENUM_VALUE( keyboardPianoElectric, "keyboard.piano.electric" ) \
        ZEUS_ENUM_VALUE( keyboardPianoGrand, "keyboard.piano.grand" ) \
        ZEUS_ENUM_VALUE( keyboardPianoHonkyTonk, "keyboard.piano.honky-tonk" ) \
        ZEUS_ENUM_VALUE( keyboardPianoPrepared, "keyboard.piano.prepared" ) \
        ZEUS_ENUM_VALUE( keyboardPianoToy, "keyboard.piano.toy" ) \
        ZEUS_ENUM_VALUE( keyboardPianoUpright, "keyboard.piano.upright" ) \
        ZEUS_ENUM_VALUE( keyboardVirginal, "keyboard.virginal" ) \
        ZEUS_ENUM_VALUE( metalAdodo, "metal.adodo" ) \
        ZEUS_ENUM_VALUE( metalAnvil, "metal.anvil" ) \
        ZEUS_ENUM_VALUE( metalBabendil, "metal.babendil" ) \
        ZEUS_ENUM_VALUE( metalBellsAgogo, "metal.bells.agogo" ) \
        ZEUS_ENUM_VALUE( metalBellsAlmglocken, "metal.bells.almglocken" ) \
        ZEUS_ENUM_VALUE( metalBellsBellPlate, "metal.bells.bell-plate" ) \
        ZEUS_ENUM_VALUE( metalBellsBellTree, "metal.bells.bell-tree" ) \
        ZEUS_ENUM_VALUE( metalBellsCarillon, "metal.bells.carillon" ) \
        ZEUS_ENUM_VALUE( metalBellsChimes, "metal.bells.chimes" ) \
        ZEUS_ENUM_VALUE( metalBellsChimta, "metal.bells.chimta" ) \
        ZEUS_ENUM_VALUE( metalBellsChippli, "metal.bells.chippli" ) \
        ZEUS_ENUM_VALUE( metalBellsChurch, "metal.bells.church" ) \
        ZEUS_ENUM_VALUE( metalBellsCowbell, "metal.bells.cowbell" ) \
        ZEUS_ENUM_VALUE( metalBellsDawuro, "metal.bells.dawuro" ) \
        ZEUS_ENUM_VALUE( metalBellsGankokwe, "metal.bells.gankokwe" ) \
        ZEUS_ENUM_VALUE( metalBellsGhungroo, "metal.bells.ghungroo" ) \
        ZEUS_ENUM_VALUE( metalBellsHatheli, "metal.bells.hatheli" ) \
        ZEUS_ENUM_VALUE( metalBellsJingleBell, "metal.bells.jingle-bell" ) \
        ZEUS_ENUM_VALUE( metalBellsKhartal, "metal.bells.khartal" ) \
        ZEUS_ENUM_VALUE( metalBellsMarkTree, "metal.bells.mark-tree" ) \
        ZEUS_ENUM_VALUE( metalBellsSistrum, "metal.bells.sistrum" ) \
        ZEUS_ENUM_VALUE( metalBellsSleighBells, "metal.bells.sleigh-bells" ) \
        ZEUS_ENUM_VALUE( metalBellsTemple, "metal.bells.temple" ) \
        ZEUS_ENUM_VALUE( metalBellsTibetan, "metal.bells.tibetan" ) \
        ZEUS_ENUM_VALUE( metalBellsTinklebell, "metal.bells.tinklebell" ) \
        ZEUS_ENUM_VALUE( metalBellsTrychel, "metal.bells.trychel" ) \
        ZEUS_ENUM_VALUE( metalBellsWindChimes, "metal.bells.wind-chimes" ) \
        ZEUS_ENUM_VALUE( metalBellsZills, "metal.bells.zills" ) \
        ZEUS_ENUM_VALUE( metalBerimbau, "metal.berimbau" ) \
        ZEUS_ENUM_VALUE( metalBrakeDrums, "metal.brake-drums" ) \
        ZEUS_ENUM_VALUE( metalCrotales, "metal.crotales" ) \
        ZEUS_ENUM_VALUE( metalCymbalBo, "metal.cymbal.bo" ) \
        ZEUS_ENUM_VALUE( metalCymbalCengCeng, "metal.cymbal.ceng-ceng" ) \
        ZEUS_ENUM_VALUE( metalCymbalChabara, "metal.cymbal.chabara" ) \
        ZEUS_ENUM_VALUE( metalCymbalChinese, "metal.cymbal.chinese" ) \
        ZEUS_ENUM_VALUE( metalCymbalChing, "metal.cymbal.ching" ) \
        ZEUS_ENUM_VALUE( metalCymbalClash, "metal.cymbal.clash" ) \
        ZEUS_ENUM_VALUE( metalCymbalCrash, "metal.cymbal.crash" ) \
        ZEUS_ENUM_VALUE( metalCymbalFinger, "metal.cymbal.finger" ) \
        ZEUS_ENUM_VALUE( metalCymbalHand, "metal.cymbal.hand" ) \
        ZEUS_ENUM_VALUE( metalCymbalKesi, "metal.cymbal.kesi" ) \
        ZEUS_ENUM_VALUE( metalCymbalManjeera, "metal.cymbal.manjeera" ) \
        ZEUS_ENUM_VALUE( metalCymbalReverse, "metal.cymbal.reverse" ) \
        ZEUS_ENUM_VALUE( metalCymbalRide, "metal.cymbal.ride" ) \
        ZEUS_ENUM_VALUE( metalCymbalSizzle, "metal.cymbal.sizzle" ) \
        ZEUS_ENUM_VALUE( metalCymbalSplash, "metal.cymbal.splash" ) \
        ZEUS_ENUM_VALUE( metalCymbalSuspended, "metal.cymbal.suspended" ) \
        ZEUS_ENUM_VALUE( metalCymbalTebyoshi, "metal.cymbal.tebyoshi" ) \
        ZEUS_ENUM_VALUE( metalCymbalTibetan, "metal.cymbal.tibetan" ) \
        ZEUS_ENUM_VALUE( metalCymbalTingsha, "metal.cymbal.tingsha" ) \
        ZEUS_ENUM_VALUE( metalFlexatone, "metal.flexatone" ) \
        ZEUS_ENUM_VALUE( metalGong, "metal.gong" ) \
        ZEUS_ENUM_VALUE( metalGongAgeng, "metal.gong.ageng" ) \
        ZEUS_ENUM_VALUE( metalGongAgung, "metal.gong.agung" ) \
        ZEUS_ENUM_VALUE( metalGongChanchiki, "metal.gong.chanchiki" ) \
        ZEUS_ENUM_VALUE( metalGongChinese, "metal.gong.chinese" ) \
        ZEUS_ENUM_VALUE( metalGongGandingan, "metal.gong.gandingan" ) \
        ZEUS_ENUM_VALUE( metalGongKempul, "metal.gong.kempul" ) \
        ZEUS_ENUM_VALUE( metalGongKempyang, "metal.gong.kempyang" ) \
        ZEUS_ENUM_VALUE( metalGongKetuk, "metal.gong.ketuk" ) \
        ZEUS_ENUM_VALUE( metalGongKkwenggwari, "metal.gong.kkwenggwari" ) \
        ZEUS_ENUM_VALUE( metalGongLuo, "metal.gong.luo" ) \
        ZEUS_ENUM_VALUE( metalGongSinging, "metal.gong.singing" ) \
        ZEUS_ENUM_VALUE( metalGongThai, "metal.gong.thai" ) \
        ZEUS_ENUM_VALUE( metalGuira, "metal.guira" ) \
        ZEUS_ENUM_VALUE( metalHang, "metal.hang" ) \
        ZEUS_ENUM_VALUE( metalHiHat, "metal.hi-hat" ) \
        ZEUS_ENUM_VALUE( metalJawHarp, "metal.jaw-harp" ) \
        ZEUS_ENUM_VALUE( metalKengong, "metal.kengong" ) \
        ZEUS_ENUM_VALUE( metalMurchang, "metal.murchang" ) \
        ZEUS_ENUM_VALUE( metalMusicalSaw, "metal.musical-saw" ) \
        ZEUS_ENUM_VALUE( metalSingingBowl, "metal.singing-bowl" ) \
        ZEUS_ENUM_VALUE( metalSpoons, "metal.spoons" ) \
        ZEUS_ENUM_VALUE( metalSteelDrums, "metal.steel-drums" ) \
        ZEUS_ENUM_VALUE( metalTamtam, "metal.tamtam" ) \
        ZEUS_ENUM_VALUE( metalThundersheet, "metal.thundersheet" ) \
        ZEUS_ENUM_VALUE( metalTriangle, "metal.triangle" ) \
        ZEUS_ENUM_VALUE( metalWashboard, "metal.washboard" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionAngklung, "pitched-percussion.angklung" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionBalafon, "pitched-percussion.balafon" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionBellLyre, "pitched-percussion.bell-lyre" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionBells, "pitched-percussion.bells" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionBianqing, "pitched-percussion.bianqing" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionBianzhong, "pitched-percussion.bianzhong" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionBonang, "pitched-percussion.bonang" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionCimbalom, "pitched-percussion.cimbalom" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionCrystalGlasses, "pitched-percussion.crystal-glasses" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionDanTamThapLuc, "pitched-percussion.dan-tam-thap-luc" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionFangxiang, "pitched-percussion.fangxiang" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionGandinganAKayo, "pitched-percussion.gandingan-a-kayo" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionGangsa, "pitched-percussion.gangsa" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionGender, "pitched-percussion.gender" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionGiying, "pitched-percussion.giying" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionGlassHarmonica, "pitched-percussion.glass-harmonica" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionGlockenspiel, "pitched-percussion.glockenspiel" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionGlockenspielAlto, "pitched-percussion.glockenspiel.alto" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionGlockenspielSoprano, "pitched-percussion.glockenspiel.soprano" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionGyil, "pitched-percussion.gyil" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionHammerDulcimer, "pitched-percussion.hammer-dulcimer" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionHandbells, "pitched-percussion.handbells" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionKalimba, "pitched-percussion.kalimba" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionKantil, "pitched-percussion.kantil" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionKhim, "pitched-percussion.khim" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionKulintang, "pitched-percussion.kulintang" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionKulintangAKayo, "pitched-percussion.kulintang-a-kayo" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionKulintangATiniok, "pitched-percussion.kulintang-a-tiniok" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionLikembe, "pitched-percussion.likembe" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionLuntang, "pitched-percussion.luntang" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionMarimba, "pitched-percussion.marimba" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionMarimbaBass, "pitched-percussion.marimba.bass" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionMbira, "pitched-percussion.mbira" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionMbiraArray, "pitched-percussion.mbira.array" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionMetallophone, "pitched-percussion.metallophone" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionMetallophoneAlto, "pitched-percussion.metallophone.alto" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionMetallophoneBass, "pitched-percussion.metallophone.bass" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionMetallophoneSoprano, "pitched-percussion.metallophone.soprano" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionMusicBox, "pitched-percussion.music-box" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionPelogPanerus, "pitched-percussion.pelog-panerus" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionPemade, "pitched-percussion.pemade" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionPenyacah, "pitched-percussion.penyacah" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionRanatEk, "pitched-percussion.ranat.ek" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionRanatEkLek, "pitched-percussion.ranat.ek-lek" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionRanatThum, "pitched-percussion.ranat.thum" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionRanatThumLek, "pitched-percussion.ranat.thum-lek" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionReyong, "pitched-percussion.reyong" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionSanza, "pitched-percussion.sanza" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionSaronBarung, "pitched-percussion.saron-barung" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionSaronDemong, "pitched-percussion.saron-demong" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionSaronPanerus, "pitched-percussion.saron-panerus" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionSlendroPanerus, "pitched-percussion.slendro-panerus" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionSlentem, "pitched-percussion.slentem" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionTsymbaly, "pitched-percussion.tsymbaly" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionTubes, "pitched-percussion.tubes" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionTubularBells, "pitched-percussion.tubular-bells" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionVibraphone, "pitched-percussion.vibraphone" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionXylophone, "pitched-percussion.xylophone" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionXylophoneAlto, "pitched-percussion.xylophone.alto" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionXylophoneBass, "pitched-percussion.xylophone.bass" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionXylophoneSoprano, "pitched-percussion.xylophone.soprano" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionXylorimba, "pitched-percussion.xylorimba" ) \
        ZEUS_ENUM_VALUE( pitchedPercussionYangqin, "pitched-percussion.yangqin" ) \
        ZEUS_ENUM_VALUE( pluckArchlute, "pluck.archlute" ) \
        ZEUS_ENUM_VALUE( pluckAutoharp, "pluck.autoharp" ) \
        ZEUS_ENUM_VALUE( pluckBaglama, "pluck.baglama" ) \
        ZEUS_ENUM_VALUE( pluckBajo, "pluck.bajo" ) \
        ZEUS_ENUM_VALUE( pluckBalalaika, "pluck.balalaika" ) \
        ZEUS_ENUM_VALUE( pluckBalalaikaAlto, "pluck.balalaika.alto" ) \
        ZEUS_ENUM_VALUE( pluckBalalaikaBass, "pluck.balalaika.bass" ) \
        ZEUS_ENUM_VALUE( pluckBalalaikaContrabass, "pluck.balalaika.contrabass" ) \
        ZEUS_ENUM_VALUE( pluckBalalaikaPiccolo, "pluck.balalaika.piccolo" ) \
        ZEUS_ENUM_VALUE( pluckBalalaikaPrima, "pluck.balalaika.prima" ) \
        ZEUS_ENUM_VALUE( pluckBalalaikaSecunda, "pluck.balalaika.secunda" ) \
        ZEUS_ENUM_VALUE( pluckBandola, "pluck.bandola" ) \
        ZEUS_ENUM_VALUE( pluckBandura, "pluck.bandura" ) \
        ZEUS_ENUM_VALUE( pluckBandurria, "pluck.bandurria" ) \
        ZEUS_ENUM_VALUE( pluckBanjo, "pluck.banjo" ) \
        ZEUS_ENUM_VALUE( pluckBanjoTenor, "pluck.banjo.tenor" ) \
        ZEUS_ENUM_VALUE( pluckBanjolele, "pluck.banjolele" ) \
        ZEUS_ENUM_VALUE( pluckBarbat, "pluck.barbat" ) \
        ZEUS_ENUM_VALUE( pluckBass, "pluck.bass" ) \
        ZEUS_ENUM_VALUE( pluckBassAcoustic, "pluck.bass.acoustic" ) \
        ZEUS_ENUM_VALUE( pluckBassBolon, "pluck.bass.bolon" ) \
        ZEUS_ENUM_VALUE( pluckBassElectric, "pluck.bass.electric" ) \
        ZEUS_ENUM_VALUE( pluckBassFretless, "pluck.bass.fretless" ) \
        ZEUS_ENUM_VALUE( pluckBassGuitarron, "pluck.bass.guitarron" ) \
        ZEUS_ENUM_VALUE( pluckBassSynth, "pluck.bass.synth" ) \
        ZEUS_ENUM_VALUE( pluckBassSynthLead, "pluck.bass.synth.lead" ) \
        ZEUS_ENUM_VALUE( pluckBassWashtub, "pluck.bass.washtub" ) \
        ZEUS_ENUM_VALUE( pluckBassWhamola, "pluck.bass.whamola" ) \
        ZEUS_ENUM_VALUE( pluckBegena, "pluck.begena" ) \
        ZEUS_ENUM_VALUE( pluckBiwa, "pluck.biwa" ) \
        ZEUS_ENUM_VALUE( pluckBordonua, "pluck.bordonua" ) \
        ZEUS_ENUM_VALUE( pluckBouzouki, "pluck.bouzouki" ) \
        ZEUS_ENUM_VALUE( pluckBouzoukiIrish, "pluck.bouzouki.irish" ) \
        ZEUS_ENUM_VALUE( pluckCelticHarp, "pluck.celtic-harp" ) \
        ZEUS_ENUM_VALUE( pluckCharango, "pluck.charango" ) \
        ZEUS_ENUM_VALUE( pluckChitarraBattente, "pluck.chitarra-battente" ) \
        ZEUS_ENUM_VALUE( pluckCithara, "pluck.cithara" ) \
        ZEUS_ENUM_VALUE( pluckCittern, "pluck.cittern" ) \
        ZEUS_ENUM_VALUE( pluckCuatro, "pluck.cuatro" ) \
        ZEUS_ENUM_VALUE( pluckDanBau, "pluck.dan-bau" ) \
        ZEUS_ENUM_VALUE( pluckDanNguyet, "pluck.dan-nguyet" ) \
        ZEUS_ENUM_VALUE( pluckDanTranh, "pluck.dan-tranh" ) \
        ZEUS_ENUM_VALUE( pluckDanTyBa, "pluck.dan-ty-ba" ) \
        ZEUS_ENUM_VALUE( pluckDiddleyBow, "pluck.diddley-bow" ) \
        ZEUS_ENUM_VALUE( pluckDomra, "pluck.domra" ) \
        ZEUS_ENUM_VALUE( pluckDomu, "pluck.domu" ) \
        ZEUS_ENUM_VALUE( pluckDulcimer, "pluck.dulcimer" ) \
        ZEUS_ENUM_VALUE( pluckDutar, "pluck.dutar" ) \
        ZEUS_ENUM_VALUE( pluckDuxianqin, "pluck.duxianqin" ) \
        ZEUS_ENUM_VALUE( pluckEktara, "pluck.ektara" ) \
        ZEUS_ENUM_VALUE( pluckGeomungo, "pluck.geomungo" ) \
        ZEUS_ENUM_VALUE( pluckGottuvadhyam, "pluck.gottuvadhyam" ) \
        ZEUS_ENUM_VALUE( pluckGuitar, "pluck.guitar" ) \
        ZEUS_ENUM_VALUE( pluckGuitarAcoustic, "pluck.guitar.acoustic" ) \
        ZEUS_ENUM_VALUE( pluckGuitarElectric, "pluck.guitar.electric" ) \
        ZEUS_ENUM_VALUE( pluckGuitarNylonString, "pluck.guitar.nylon-string" ) \
        ZEUS_ENUM_VALUE( pluckGuitarPedalSteel, "pluck.guitar.pedal-steel" ) \
        ZEUS_ENUM_VALUE( pluckGuitarPortuguese, "pluck.guitar.portuguese" ) \
        ZEUS_ENUM_VALUE( pluckGuitarRequinto, "pluck.guitar.requinto" ) \
        ZEUS_ENUM_VALUE( pluckGuitarResonator, "pluck.guitar.resonator" ) \
        ZEUS_ENUM_VALUE( pluckGuitarSteelString, "pluck.guitar.steel-string" ) \
        ZEUS_ENUM_VALUE( pluckGuitjo, "pluck.guitjo" ) \
        ZEUS_ENUM_VALUE( pluckGuitjoDoubleNeck, "pluck.guitjo.double-neck" ) \
        ZEUS_ENUM_VALUE( pluckGuqin, "pluck.guqin" ) \
        ZEUS_ENUM_VALUE( pluckGuzheng, "pluck.guzheng" ) \
        ZEUS_ENUM_VALUE( pluckGuzhengChoazhou, "pluck.guzheng.choazhou" ) \
        ZEUS_ENUM_VALUE( pluckHarp, "pluck.harp" ) \
        ZEUS_ENUM_VALUE( pluckHarpGuitar, "pluck.harp-guitar" ) \
        ZEUS_ENUM_VALUE( pluckHuapanguera, "pluck.huapanguera" ) \
        ZEUS_ENUM_VALUE( pluckJaranaHuasteca, "pluck.jarana-huasteca" ) \
        ZEUS_ENUM_VALUE( pluckJaranaJarocha, "pluck.jarana-jarocha" ) \
        ZEUS_ENUM_VALUE( pluckJaranaJarochaMosquito, "pluck.jarana-jarocha.mosquito" ) \
        ZEUS_ENUM_VALUE( pluckJaranaJarochaPrimera, "pluck.jarana-jarocha.primera" ) \
        ZEUS_ENUM_VALUE( pluckJaranaJarochaSegunda, "pluck.jarana-jarocha.segunda" ) \
        ZEUS_ENUM_VALUE( pluckJaranaJarochaTercera, "pluck.jarana-jarocha.tercera" ) \
        ZEUS_ENUM_VALUE( pluckKabosy, "pluck.kabosy" ) \
        ZEUS_ENUM_VALUE( pluckKantele, "pluck.kantele" ) \
        ZEUS_ENUM_VALUE( pluckKanun, "pluck.kanun" ) \
        ZEUS_ENUM_VALUE( pluckKayagum, "pluck.kayagum" ) \
        ZEUS_ENUM_VALUE( pluckKobza, "pluck.kobza" ) \
        ZEUS_ENUM_VALUE( pluckKomuz, "pluck.komuz" ) \
        ZEUS_ENUM_VALUE( pluckKora, "pluck.kora" ) \
        ZEUS_ENUM_VALUE( pluckKoto, "pluck.koto" ) \
        ZEUS_ENUM_VALUE( pluckKutiyapi, "pluck.kutiyapi" ) \
        ZEUS_ENUM_VALUE( pluckLangeleik, "pluck.langeleik" ) \
        ZEUS_ENUM_VALUE( pluckLaud, "pluck.laud" ) \
        ZEUS_ENUM_VALUE( pluckLute, "pluck.lute" ) \
        ZEUS_ENUM_VALUE( pluckLyre, "pluck.lyre" ) \
        ZEUS_ENUM_VALUE( pluckMandobass, "pluck.mandobass" ) \
        ZEUS_ENUM_VALUE( pluckMandocello, "pluck.mandocello" ) \
        ZEUS_ENUM_VALUE( pluckMandola, "pluck.mandola" ) \
        ZEUS_ENUM_VALUE( pluckMandolin, "pluck.mandolin" ) \
        ZEUS_ENUM_VALUE( pluckMandolinOctave, "pluck.mandolin.octave" ) \
        ZEUS_ENUM_VALUE( pluckMandora, "pluck.mandora" ) \
        ZEUS_ENUM_VALUE( pluckMandore, "pluck.mandore" ) \
        ZEUS_ENUM_VALUE( pluckMarovany, "pluck.marovany" ) \
        ZEUS_ENUM_VALUE( pluckMusicalBow, "pluck.musical-bow" ) \
        ZEUS_ENUM_VALUE( pluckNgoni, "pluck.ngoni" ) \
        ZEUS_ENUM_VALUE( pluckOud, "pluck.oud" ) \
        ZEUS_ENUM_VALUE( pluckPipa, "pluck.pipa" ) \
        ZEUS_ENUM_VALUE( pluckPsaltery, "pluck.psaltery" ) \
        ZEUS_ENUM_VALUE( pluckRuan, "pluck.ruan" ) \
        ZEUS_ENUM_VALUE( pluckSallaneh, "pluck.sallaneh" ) \
        ZEUS_ENUM_VALUE( pluckSanshin, "pluck.sanshin" ) \
        ZEUS_ENUM_VALUE( pluckSantoor, "pluck.santoor" ) \
        ZEUS_ENUM_VALUE( pluckSanxian, "pluck.sanxian" ) \
        ZEUS_ENUM_VALUE( pluckSarod, "pluck.sarod" ) \
        ZEUS_ENUM_VALUE( pluckSaung, "pluck.saung" ) \
        ZEUS_ENUM_VALUE( pluckSaz, "pluck.saz" ) \
        ZEUS_ENUM_VALUE( pluckSe, "pluck.se" ) \
        ZEUS_ENUM_VALUE( pluckSetar, "pluck.setar" ) \
        ZEUS_ENUM_VALUE( pluckShamisen, "pluck.shamisen" ) \
        ZEUS_ENUM_VALUE( pluckSitar, "pluck.sitar" ) \
        ZEUS_ENUM_VALUE( pluckSynth, "pluck.synth" ) \
        ZEUS_ENUM_VALUE( pluckSynthCharang, "pluck.synth.charang" ) \
        ZEUS_ENUM_VALUE( pluckSynthChiff, "pluck.synth.chiff" ) \
        ZEUS_ENUM_VALUE( pluckSynthStick, "pluck.synth.stick" ) \
        ZEUS_ENUM_VALUE( pluckTambura, "pluck.tambura" ) \
        ZEUS_ENUM_VALUE( pluckTamburaBulgarian, "pluck.tambura.bulgarian" ) \
        ZEUS_ENUM_VALUE( pluckTamburaFemale, "pluck.tambura.female" ) \
        ZEUS_ENUM_VALUE( pluckTamburaMale, "pluck.tambura.male" ) \
        ZEUS_ENUM_VALUE( pluckTar, "pluck.tar" ) \
        ZEUS_ENUM_VALUE( pluckTheorbo, "pluck.theorbo" ) \
        ZEUS_ENUM_VALUE( pluckTimple, "pluck.timple" ) \
        ZEUS_ENUM_VALUE( pluckTiple, "pluck.tiple" ) \
        ZEUS_ENUM_VALUE( pluckTres, "pluck.tres" ) \
        ZEUS_ENUM_VALUE( pluckUkulele, "pluck.ukulele" ) \
        ZEUS_ENUM_VALUE( pluckUkuleleTenor, "pluck.ukulele.tenor" ) \
        ZEUS_ENUM_VALUE( pluckValiha, "pluck.valiha" ) \
        ZEUS_ENUM_VALUE( pluckVeena, "pluck.veena" ) \
        ZEUS_ENUM_VALUE( pluckVeenaMohan, "pluck.veena.mohan" ) \
        ZEUS_ENUM_VALUE( pluckVeenaRudra, "pluck.veena.rudra" ) \
        ZEUS_ENUM_VALUE( pluckVeenaVichitra, "pluck.veena.vichitra" ) \
        ZEUS_ENUM_VALUE( pluckVihuela, "pluck.vihuela" ) \
        ZEUS_ENUM_VALUE( pluckVihuelaMexican, "pluck.vihuela.mexican" ) \
        ZEUS_ENUM_VALUE( pluckXalam, "pluck.xalam" ) \
        ZEUS_ENUM_VALUE( pluckYueqin, "pluck.yueqin" ) \
        ZEUS_ENUM_VALUE( pluckZither, "pluck.zither" ) \
        ZEUS_ENUM_VALUE( pluckZitherOvertone, "pluck.zither.overtone" ) \
        ZEUS_ENUM_VALUE( rattleAfoxe, "rattle.afoxe" ) \
        ZEUS_ENUM_VALUE( rattleBirds, "rattle.birds" ) \
        ZEUS_ENUM_VALUE( rattleCabasa, "rattle.cabasa" ) \
        ZEUS_ENUM_VALUE( rattleCaxixi, "rattle.caxixi" ) \
        ZEUS_ENUM_VALUE( rattleCog, "rattle.cog" ) \
        ZEUS_ENUM_VALUE( rattleGanza, "rattle.ganza" ) \
        ZEUS_ENUM_VALUE( rattleHosho, "rattle.hosho" ) \
        ZEUS_ENUM_VALUE( rattleJawbone, "rattle.jawbone" ) \
        ZEUS_ENUM_VALUE( rattleKayamba, "rattle.kayamba" ) \
        ZEUS_ENUM_VALUE( rattleKpokoKpoko, "rattle.kpoko-kpoko" ) \
        ZEUS_ENUM_VALUE( rattleLavaStones, "rattle.lava-stones" ) \
        ZEUS_ENUM_VALUE( rattleMaraca, "rattle.maraca" ) \
        ZEUS_ENUM_VALUE( rattleRainStick, "rattle.rain-stick" ) \
        ZEUS_ENUM_VALUE( rattleRatchet, "rattle.ratchet" ) \
        ZEUS_ENUM_VALUE( rattleRattle, "rattle.rattle" ) \
        ZEUS_ENUM_VALUE( rattleShaker, "rattle.shaker" ) \
        ZEUS_ENUM_VALUE( rattleShakerEgg, "rattle.shaker.egg" ) \
        ZEUS_ENUM_VALUE( rattleShekere, "rattle.shekere" ) \
        ZEUS_ENUM_VALUE( rattleSistre, "rattle.sistre" ) \
        ZEUS_ENUM_VALUE( rattleTelevi, "rattle.televi" ) \
        ZEUS_ENUM_VALUE( rattleVibraslap, "rattle.vibraslap" ) \
        ZEUS_ENUM_VALUE( rattleWasembe, "rattle.wasembe" ) \
        ZEUS_ENUM_VALUE( stringsAjaeng, "strings.ajaeng" ) \
        ZEUS_ENUM_VALUE( stringsArpeggione, "strings.arpeggione" ) \
        ZEUS_ENUM_VALUE( stringsBaryton, "strings.baryton" ) \
        ZEUS_ENUM_VALUE( stringsCello, "strings.cello" ) \
        ZEUS_ENUM_VALUE( stringsCelloPiccolo, "strings.cello.piccolo" ) \
        ZEUS_ENUM_VALUE( stringsContrabass, "strings.contrabass" ) \
        ZEUS_ENUM_VALUE( stringsCrwth, "strings.crwth" ) \
        ZEUS_ENUM_VALUE( stringsDanGao, "strings.dan-gao" ) \
        ZEUS_ENUM_VALUE( stringsDihu, "strings.dihu" ) \
        ZEUS_ENUM_VALUE( stringsErhu, "strings.erhu" ) \
        ZEUS_ENUM_VALUE( stringsErxian, "strings.erxian" ) \
        ZEUS_ENUM_VALUE( stringsEsraj, "strings.esraj" ) \
        ZEUS_ENUM_VALUE( stringsFiddle, "strings.fiddle" ) \
        ZEUS_ENUM_VALUE( stringsFiddleHardanger, "strings.fiddle.hardanger" ) \
        ZEUS_ENUM_VALUE( stringsGadulka, "strings.gadulka" ) \
        ZEUS_ENUM_VALUE( stringsGaohu, "strings.gaohu" ) \
        ZEUS_ENUM_VALUE( stringsGehu, "strings.gehu" ) \
        ZEUS_ENUM_VALUE( stringsGroup, "strings.group" ) \
        ZEUS_ENUM_VALUE( stringsGroupSynth, "strings.group.synth" ) \
        ZEUS_ENUM_VALUE( stringsHaegeum, "strings.haegeum" ) \
        ZEUS_ENUM_VALUE( stringsHurdyGurdy, "strings.hurdy-gurdy" ) \
        ZEUS_ENUM_VALUE( stringsIgil, "strings.igil" ) \
        ZEUS_ENUM_VALUE( stringsKamancha, "strings.kamancha" ) \
        ZEUS_ENUM_VALUE( stringsKokyu, "strings.kokyu" ) \
        ZEUS_ENUM_VALUE( stringsLaruan, "strings.laruan" ) \
        ZEUS_ENUM_VALUE( stringsLeiqin, "strings.leiqin" ) \
        ZEUS_ENUM_VALUE( stringsLirone, "strings.lirone" ) \
        ZEUS_ENUM_VALUE( stringsLyraByzantine, "strings.lyra.byzantine" ) \
        ZEUS_ENUM_VALUE( stringsLyraCretan, "strings.lyra.cretan" ) \
        ZEUS_ENUM_VALUE( stringsMorinKhuur, "strings.morin-khuur" ) \
        ZEUS_ENUM_VALUE( stringsNyckelharpa, "strings.nyckelharpa" ) \
        ZEUS_ENUM_VALUE( stringsOctobass, "strings.octobass" ) \
        ZEUS_ENUM_VALUE( stringsRebab, "strings.rebab" ) \
        ZEUS_ENUM_VALUE( stringsRebec, "strings.rebec" ) \
        ZEUS_ENUM_VALUE( stringsSarangi, "strings.sarangi" ) \
        ZEUS_ENUM_VALUE( stringsStrohViolin, "strings.stroh-violin" ) \
        ZEUS_ENUM_VALUE( stringsTrombaMarina, "strings.tromba-marina" ) \
        ZEUS_ENUM_VALUE( stringsVielle, "strings.vielle" ) \
        ZEUS_ENUM_VALUE( stringsViol, "strings.viol" ) \
        ZEUS_ENUM_VALUE( stringsViolAlto, "strings.viol.alto" ) \
        ZEUS_ENUM_VALUE( stringsViolBass, "strings.viol.bass" ) \
        ZEUS_ENUM_VALUE( stringsViolTenor, "strings.viol.tenor" ) \
        ZEUS_ENUM_VALUE( stringsViolTreble, "strings.viol.treble" ) \
        ZEUS_ENUM_VALUE( stringsViolViolone, "strings.viol.violone" ) \
        ZEUS_ENUM_VALUE( stringsViola, "strings.viola" ) \
        ZEUS_ENUM_VALUE( stringsViolaDamore, "strings.viola-damore" ) \
        ZEUS_ENUM_VALUE( stringsViolin, "strings.violin" ) \
        ZEUS_ENUM_VALUE( stringsViolonoPiccolo, "strings.violono.piccolo" ) \
        ZEUS_ENUM_VALUE( stringsViolotta, "strings.violotta" ) \
        ZEUS_ENUM_VALUE( stringsYayliTanbur, "strings.yayli-tanbur" ) \
        ZEUS_ENUM_VALUE( stringsYazheng, "strings.yazheng" ) \
        ZEUS_ENUM_VALUE( stringsZhonghu, "strings.zhonghu" ) \
        ZEUS_ENUM_VALUE( synthEffects, "synth.effects" ) \
        ZEUS_ENUM_VALUE( synthEffectsAtmosphere, "synth.effects.atmosphere" ) \
        ZEUS_ENUM_VALUE( synthEffectsBrightness, "synth.effects.brightness" ) \
        ZEUS_ENUM_VALUE( synthEffectsCrystal, "synth.effects.crystal" ) \
        ZEUS_ENUM_VALUE( synthEffectsEchoes, "synth.effects.echoes" ) \
        ZEUS_ENUM_VALUE( synthEffectsGoblins, "synth.effects.goblins" ) \
        ZEUS_ENUM_VALUE( synthEffectsRain, "synth.effects.rain" ) \
        ZEUS_ENUM_VALUE( synthEffectsSciFi, "synth.effects.sci-fi" ) \
        ZEUS_ENUM_VALUE( synthEffectsSoundtrack, "synth.effects.soundtrack" ) \
        ZEUS_ENUM_VALUE( synthGroup, "synth.group" ) \
        ZEUS_ENUM_VALUE( synthGroupFifths, "synth.group.fifths" ) \
        ZEUS_ENUM_VALUE( synthGroupOrchestra, "synth.group.orchestra" ) \
        ZEUS_ENUM_VALUE( synthPad, "synth.pad" ) \
        ZEUS_ENUM_VALUE( synthPadBowed, "synth.pad.bowed" ) \
        ZEUS_ENUM_VALUE( synthPadChoir, "synth.pad.choir" ) \
        ZEUS_ENUM_VALUE( synthPadHalo, "synth.pad.halo" ) \
        ZEUS_ENUM_VALUE( synthPadMetallic, "synth.pad.metallic" ) \
        ZEUS_ENUM_VALUE( synthPadPolysynth, "synth.pad.polysynth" ) \
        ZEUS_ENUM_VALUE( synthPadSweep, "synth.pad.sweep" ) \
        ZEUS_ENUM_VALUE( synthPadWarm, "synth.pad.warm" ) \
        ZEUS_ENUM_VALUE( synthTheremin, "synth.theremin" ) \
        ZEUS_ENUM_VALUE( synthToneSawtooth, "synth.tone.sawtooth" ) \
        ZEUS_ENUM_VALUE( synthToneSine, "synth.tone.sine" ) \
        ZEUS_ENUM_VALUE( synthToneSquare, "synth.tone.square" ) \
        ZEUS_ENUM_VALUE( voiceAa, "voice.aa" ) \
        ZEUS_ENUM_VALUE( voiceAlto, "voice.alto" ) \
        ZEUS_ENUM_VALUE( voiceAw, "voice.aw" ) \
        ZEUS_ENUM_VALUE( voiceBaritone, "voice.baritone" ) \
        ZEUS_ENUM_VALUE( voiceBass, "voice.bass" ) \
        ZEUS_ENUM_VALUE( voiceChild, "voice.child" ) \
        ZEUS_ENUM_VALUE( voiceCountertenor, "voice.countertenor" ) \
        ZEUS_ENUM_VALUE( voiceDoo, "voice.doo" ) \
        ZEUS_ENUM_VALUE( voiceEe, "voice.ee" ) \
        ZEUS_ENUM_VALUE( voiceFemale, "voice.female" ) \
        ZEUS_ENUM_VALUE( voiceKazoo, "voice.kazoo" ) \
        ZEUS_ENUM_VALUE( voiceMale, "voice.male" ) \
        ZEUS_ENUM_VALUE( voiceMezzoSoprano, "voice.mezzo-soprano" ) \
        ZEUS_ENUM_VALUE( voiceMm, "voice.mm" ) \
        ZEUS_ENUM_VALUE( voiceOo, "voice.oo" ) \
        ZEUS_ENUM_VALUE( voicePercussion, "voice.percussion" ) \
        ZEUS_ENUM_VALUE( voicePercussionBeatbox, "voice.percussion.beatbox" ) \
        ZEUS_ENUM_VALUE( voiceSoprano, "voice.soprano" ) \
        ZEUS_ENUM_VALUE( voiceSynth, "voice.synth" ) \
        ZEUS_ENUM_VALUE( voiceTalkBox, "voice.talk-box" ) \
        ZEUS_ENUM_VALUE( voiceTenor, "voice.tenor" ) \
        ZEUS_ENUM_VALUE( voiceVocals, "voice.vocals" ) \
        ZEUS_ENUM_VALUE( windFlutesBansuri, "wind.flutes.bansuri" ) \
        ZEUS_ENUM_VALUE( windFlutesBlownBottle, "wind.flutes.blown-bottle" ) \
        ZEUS_ENUM_VALUE( windFlutesCalliope, "wind.flutes.calliope" ) \
        ZEUS_ENUM_VALUE( windFlutesDanso, "wind.flutes.danso" ) \
        ZEUS_ENUM_VALUE( windFlutesDiZi, "wind.flutes.di-zi" ) \
        ZEUS_ENUM_VALUE( windFlutesDvojnice, "wind.flutes.dvojnice" ) \
        ZEUS_ENUM_VALUE( windFlutesFife, "wind.flutes.fife" ) \
        ZEUS_ENUM_VALUE( windFlutesFlageolet, "wind.flutes.flageolet" ) \
        ZEUS_ENUM_VALUE( windFlutesFlute, "wind.flutes.flute" ) \
        ZEUS_ENUM_VALUE( windFlutesFluteAlto, "wind.flutes.flute.alto" ) \
        ZEUS_ENUM_VALUE( windFlutesFluteBass, "wind.flutes.flute.bass" ) \
        ZEUS_ENUM_VALUE( windFlutesFluteContraAlto, "wind.flutes.flute.contra-alto" ) \
        ZEUS_ENUM_VALUE( windFlutesFluteContrabass, "wind.flutes.flute.contrabass" ) \
        ZEUS_ENUM_VALUE( windFlutesFluteDoubleContrabass, "wind.flutes.flute.double-contrabass" ) \
        ZEUS_ENUM_VALUE( windFlutesFluteIrish, "wind.flutes.flute.irish" ) \
        ZEUS_ENUM_VALUE( windFlutesFlutePiccolo, "wind.flutes.flute.piccolo" ) \
        ZEUS_ENUM_VALUE( windFlutesFluteSubcontrabass, "wind.flutes.flute.subcontrabass" ) \
        ZEUS_ENUM_VALUE( windFlutesFujara, "wind.flutes.fujara" ) \
        ZEUS_ENUM_VALUE( windFlutesGemshorn, "wind.flutes.gemshorn" ) \
        ZEUS_ENUM_VALUE( windFlutesHocchiku, "wind.flutes.hocchiku" ) \
        ZEUS_ENUM_VALUE( windFlutesHun, "wind.flutes.hun" ) \
        ZEUS_ENUM_VALUE( windFlutesKaval, "wind.flutes.kaval" ) \
        ZEUS_ENUM_VALUE( windFlutesKawala, "wind.flutes.kawala" ) \
        ZEUS_ENUM_VALUE( windFlutesKhlui, "wind.flutes.khlui" ) \
        ZEUS_ENUM_VALUE( windFlutesKnotweed, "wind.flutes.knotweed" ) \
        ZEUS_ENUM_VALUE( windFlutesKoncovkaAlto, "wind.flutes.koncovka.alto" ) \
        ZEUS_ENUM_VALUE( windFlutesKoudi, "wind.flutes.koudi" ) \
        ZEUS_ENUM_VALUE( windFlutesNey, "wind.flutes.ney" ) \
        ZEUS_ENUM_VALUE( windFlutesNohkan, "wind.flutes.nohkan" ) \
        ZEUS_ENUM_VALUE( windFlutesNose, "wind.flutes.nose" ) \
        ZEUS_ENUM_VALUE( windFlutesOcarina, "wind.flutes.ocarina" ) \
        ZEUS_ENUM_VALUE( windFlutesOvertoneTenor, "wind.flutes.overtone.tenor" ) \
        ZEUS_ENUM_VALUE( windFlutesPalendag, "wind.flutes.palendag" ) \
        ZEUS_ENUM_VALUE( windFlutesPanpipes, "wind.flutes.panpipes" ) \
        ZEUS_ENUM_VALUE( windFlutesQuena, "wind.flutes.quena" ) \
        ZEUS_ENUM_VALUE( windFlutesRecorder, "wind.flutes.recorder" ) \
        ZEUS_ENUM_VALUE( windFlutesRecorderAlto, "wind.flutes.recorder.alto" ) \
        ZEUS_ENUM_VALUE( windFlutesRecorderBass, "wind.flutes.recorder.bass" ) \
        ZEUS_ENUM_VALUE( windFlutesRecorderContrabass, "wind.flutes.recorder.contrabass" ) \
        ZEUS_ENUM_VALUE( windFlutesRecorderDescant, "wind.flutes.recorder.descant" ) \
        ZEUS_ENUM_VALUE( windFlutesRecorderGarklein, "wind.flutes.recorder.garklein" ) \
        ZEUS_ENUM_VALUE( windFlutesRecorderGreatBass, "wind.flutes.recorder.great-bass" ) \
        ZEUS_ENUM_VALUE( windFlutesRecorderSopranino, "wind.flutes.recorder.sopranino" ) \
        ZEUS_ENUM_VALUE( windFlutesRecorderSoprano, "wind.flutes.recorder.soprano" ) \
        ZEUS_ENUM_VALUE( windFlutesRecorderTenor, "wind.flutes.recorder.tenor" ) \
        ZEUS_ENUM_VALUE( windFlutesRyuteki, "wind.flutes.ryuteki" ) \
        ZEUS_ENUM_VALUE( windFlutesShakuhachi, "wind.flutes.shakuhachi" ) \
        ZEUS_ENUM_VALUE( windFlutesShepherdsPipe, "wind.flutes.shepherds-pipe" ) \
        ZEUS_ENUM_VALUE( windFlutesShinobue, "wind.flutes.shinobue" ) \
        ZEUS_ENUM_VALUE( windFlutesShvi, "wind.flutes.shvi" ) \
        ZEUS_ENUM_VALUE( windFlutesSuling, "wind.flutes.suling" ) \
        ZEUS_ENUM_VALUE( windFlutesTarka, "wind.flutes.tarka" ) \
        ZEUS_ENUM_VALUE( windFlutesTumpong, "wind.flutes.tumpong" ) \
        ZEUS_ENUM_VALUE( windFlutesVenu, "wind.flutes.venu" ) \
        ZEUS_ENUM_VALUE( windFlutesWhistle, "wind.flutes.whistle" ) \
        ZEUS_ENUM_VALUE( windFlutesWhistleAlto, "wind.flutes.whistle.alto" ) \
        ZEUS_ENUM_VALUE( windFlutesWhistleLowIrish, "wind.flutes.whistle.low-irish" ) \
        ZEUS_ENUM_VALUE( windFlutesWhistleShiva, "wind.flutes.whistle.shiva" ) \
        ZEUS_ENUM_VALUE( windFlutesWhistleSlide, "wind.flutes.whistle.slide" ) \
        ZEUS_ENUM_VALUE( windFlutesWhistleTin, "wind.flutes.whistle.tin" ) \
        ZEUS_ENUM_VALUE( windFlutesWhistleTinBflat, "wind.flutes.whistle.tin.bflat" ) \
        ZEUS_ENUM_VALUE( windFlutesWhistleTinD, "wind.flutes.whistle.tin.d" ) \
        ZEUS_ENUM_VALUE( windFlutesXiao, "wind.flutes.xiao" ) \
        ZEUS_ENUM_VALUE( windFlutesXun, "wind.flutes.xun" ) \
        ZEUS_ENUM_VALUE( windGroup, "wind.group" ) \
        ZEUS_ENUM_VALUE( windJug, "wind.jug" ) \
        ZEUS_ENUM_VALUE( windPipesBagpipes, "wind.pipes.bagpipes" ) \
        ZEUS_ENUM_VALUE( windPipesGaida, "wind.pipes.gaida" ) \
        ZEUS_ENUM_VALUE( windPipesHighland, "wind.pipes.highland" ) \
        ZEUS_ENUM_VALUE( windPipesUilleann, "wind.pipes.uilleann" ) \
        ZEUS_ENUM_VALUE( windPungi, "wind.pungi" ) \
        ZEUS_ENUM_VALUE( windReedAlbogue, "wind.reed.albogue" ) \
        ZEUS_ENUM_VALUE( windReedAlboka, "wind.reed.alboka" ) \
        ZEUS_ENUM_VALUE( windReedAlgaita, "wind.reed.algaita" ) \
        ZEUS_ENUM_VALUE( windReedArghul, "wind.reed.arghul" ) \
        ZEUS_ENUM_VALUE( windReedBassetHorn, "wind.reed.basset-horn" ) \
        ZEUS_ENUM_VALUE( windReedBassoon, "wind.reed.bassoon" ) \
        ZEUS_ENUM_VALUE( windReedBawu, "wind.reed.bawu" ) \
        ZEUS_ENUM_VALUE( windReedBifora, "wind.reed.bifora" ) \
        ZEUS_ENUM_VALUE( windReedBombarde, "wind.reed.bombarde" ) \
        ZEUS_ENUM_VALUE( windReedChalumeau, "wind.reed.chalumeau" ) \
        ZEUS_ENUM_VALUE( windReedClarinet, "wind.reed.clarinet" ) \
        ZEUS_ENUM_VALUE( windReedClarinetA, "wind.reed.clarinet.a" ) \
        ZEUS_ENUM_VALUE( windReedClarinetAlto, "wind.reed.clarinet.alto" ) \
        ZEUS_ENUM_VALUE( windReedClarinetBass, "wind.reed.clarinet.bass" ) \
        ZEUS_ENUM_VALUE( windReedClarinetBasset, "wind.reed.clarinet.basset" ) \
        ZEUS_ENUM_VALUE( windReedClarinetBflat, "wind.reed.clarinet.bflat" ) \
        ZEUS_ENUM_VALUE( windReedClarinetContraAlto, "wind.reed.clarinet.contra-alto" ) \
        ZEUS_ENUM_VALUE( windReedClarinetContrabass, "wind.reed.clarinet.contrabass" ) \
        ZEUS_ENUM_VALUE( windReedClarinetEflat, "wind.reed.clarinet.eflat" ) \
        ZEUS_ENUM_VALUE( windReedClarinetPiccoloAflat, "wind.reed.clarinet.piccolo.aflat" ) \
        ZEUS_ENUM_VALUE( windReedClarinetteDamour, "wind.reed.clarinette-damour" ) \
        ZEUS_ENUM_VALUE( windReedContrabass, "wind.reed.contrabass" ) \
        ZEUS_ENUM_VALUE( windReedContrabassoon, "wind.reed.contrabassoon" ) \
        ZEUS_ENUM_VALUE( windReedCornamuse, "wind.reed.cornamuse" ) \
        ZEUS_ENUM_VALUE( windReedCromorne, "wind.reed.cromorne" ) \
        ZEUS_ENUM_VALUE( windReedCrumhorn, "wind.reed.crumhorn" ) \
        ZEUS_ENUM_VALUE( windReedCrumhornAlto, "wind.reed.crumhorn.alto" ) \
        ZEUS_ENUM_VALUE( windReedCrumhornBass, "wind.reed.crumhorn.bass" ) \
        ZEUS_ENUM_VALUE( windReedCrumhornGreatBass, "wind.reed.crumhorn.great-bass" ) \
        ZEUS_ENUM_VALUE( windReedCrumhornSoprano, "wind.reed.crumhorn.soprano" ) \
        ZEUS_ENUM_VALUE( windReedCrumhornTenor, "wind.reed.crumhorn.tenor" ) \
        ZEUS_ENUM_VALUE( windReedDiple, "wind.reed.diple" ) \
        ZEUS_ENUM_VALUE( windReedDiplica, "wind.reed.diplica" ) \
        ZEUS_ENUM_VALUE( windReedDuduk, "wind.reed.duduk" ) \
        ZEUS_ENUM_VALUE( windReedDulcian, "wind.reed.dulcian" ) \
        ZEUS_ENUM_VALUE( windReedDulzaina, "wind.reed.dulzaina" ) \
        ZEUS_ENUM_VALUE( windReedEnglishHorn, "wind.reed.english-horn" ) \
        ZEUS_ENUM_VALUE( windReedGuanzi, "wind.reed.guanzi" ) \
        ZEUS_ENUM_VALUE( windReedHarmonica, "wind.reed.harmonica" ) \
        ZEUS_ENUM_VALUE( windReedHarmonicaBass, "wind.reed.harmonica.bass" ) \
        ZEUS_ENUM_VALUE( windReedHeckelClarina, "wind.reed.heckel-clarina" ) \
        ZEUS_ENUM_VALUE( windReedHeckelphone, "wind.reed.heckelphone" ) \
        ZEUS_ENUM_VALUE( windReedHeckelphonePiccolo, "wind.reed.heckelphone.piccolo" ) \
        ZEUS_ENUM_VALUE( windReedHeckelphoneClarinet, "wind.reed.heckelphone-clarinet" ) \
        ZEUS_ENUM_VALUE( windReedHichiriki, "wind.reed.hichiriki" ) \
        ZEUS_ENUM_VALUE( windReedHirtenschalmei, "wind.reed.hirtenschalmei" ) \
        ZEUS_ENUM_VALUE( windReedHne, "wind.reed.hne" ) \
        ZEUS_ENUM_VALUE( windReedHornpipe, "wind.reed.hornpipe" ) \
        ZEUS_ENUM_VALUE( windReedHouguan, "wind.reed.houguan" ) \
        ZEUS_ENUM_VALUE( windReedHulusi, "wind.reed.hulusi" ) \
        ZEUS_ENUM_VALUE( windReedJogiBaja, "wind.reed.jogi-baja" ) \
        ZEUS_ENUM_VALUE( windReedKenBau, "wind.reed.ken-bau" ) \
        ZEUS_ENUM_VALUE( windReedKhaenMouthOrgan, "wind.reed.khaen-mouth-organ" ) \
        ZEUS_ENUM_VALUE( windReedLauneddas, "wind.reed.launeddas" ) \
        ZEUS_ENUM_VALUE( windReedMaqrunah, "wind.reed.maqrunah" ) \
        ZEUS_ENUM_VALUE( windReedMelodica, "wind.reed.melodica" ) \
        ZEUS_ENUM_VALUE( windReedMijwiz, "wind.reed.mijwiz" ) \
        ZEUS_ENUM_VALUE( windReedMizmar, "wind.reed.mizmar" ) \
        ZEUS_ENUM_VALUE( windReedNadaswaram, "wind.reed.nadaswaram" ) \
        ZEUS_ENUM_VALUE( windReedOboe, "wind.reed.oboe" ) \
        ZEUS_ENUM_VALUE( windReedOboeBass, "wind.reed.oboe.bass" ) \
        ZEUS_ENUM_VALUE( windReedOboePiccolo, "wind.reed.oboe.piccolo" ) \
        ZEUS_ENUM_VALUE( windReedOboeDaCaccia, "wind.reed.oboe-da-caccia" ) \
        ZEUS_ENUM_VALUE( windReedOboeDamore, "wind.reed.oboe-damore" ) \
        ZEUS_ENUM_VALUE( windReedOctavin, "wind.reed.octavin" ) \
        ZEUS_ENUM_VALUE( windReedPi, "wind.reed.pi" ) \
        ZEUS_ENUM_VALUE( windReedPibgorn, "wind.reed.pibgorn" ) \
        ZEUS_ENUM_VALUE( windReedPiri, "wind.reed.piri" ) \
        ZEUS_ENUM_VALUE( windReedRackett, "wind.reed.rackett" ) \
        ZEUS_ENUM_VALUE( windReedRauschpfeife, "wind.reed.rauschpfeife" ) \
        ZEUS_ENUM_VALUE( windReedRhaita, "wind.reed.rhaita" ) \
        ZEUS_ENUM_VALUE( windReedRothphone, "wind.reed.rothphone" ) \
        ZEUS_ENUM_VALUE( windReedSarrusaphone, "wind.reed.sarrusaphone" ) \
        ZEUS_ENUM_VALUE( windReedSaxonette, "wind.reed.saxonette" ) \
        ZEUS_ENUM_VALUE( windReedSaxophone, "wind.reed.saxophone" ) \
        ZEUS_ENUM_VALUE( windReedSaxophoneAlto, "wind.reed.saxophone.alto" ) \
        ZEUS_ENUM_VALUE( windReedSaxophoneAulochrome, "wind.reed.saxophone.aulochrome" ) \
        ZEUS_ENUM_VALUE( windReedSaxophoneBaritone, "wind.reed.saxophone.baritone" ) \
        ZEUS_ENUM_VALUE( windReedSaxophoneBass, "wind.reed.saxophone.bass" ) \
        ZEUS_ENUM_VALUE( windReedSaxophoneContrabass, "wind.reed.saxophone.contrabass" ) \
        ZEUS_ENUM_VALUE( windReedSaxophoneMelody, "wind.reed.saxophone.melody" ) \
        ZEUS_ENUM_VALUE( windReedSaxophoneMezzoSoprano, "wind.reed.saxophone.mezzo-soprano" ) \
        ZEUS_ENUM_VALUE( windReedSaxophoneSopranino, "wind.reed.saxophone.sopranino" ) \
        ZEUS_ENUM_VALUE( windReedSaxophoneSopranissimo, "wind.reed.saxophone.sopranissimo" ) \
        ZEUS_ENUM_VALUE( windReedSaxophoneSoprano, "wind.reed.saxophone.soprano" ) \
        ZEUS_ENUM_VALUE( windReedSaxophoneSubcontrabass, "wind.reed.saxophone.subcontrabass" ) \
        ZEUS_ENUM_VALUE( windReedSaxophoneTenor, "wind.reed.saxophone.tenor" ) \
        ZEUS_ENUM_VALUE( windReedShawm, "wind.reed.shawm" ) \
        ZEUS_ENUM_VALUE( windReedShenai, "wind.reed.shenai" ) \
        ZEUS_ENUM_VALUE( windReedSheng, "wind.reed.sheng" ) \
        ZEUS_ENUM_VALUE( windReedSipsi, "wind.reed.sipsi" ) \
        ZEUS_ENUM_VALUE( windReedSopila, "wind.reed.sopila" ) \
        ZEUS_ENUM_VALUE( windReedSorna, "wind.reed.sorna" ) \
        ZEUS_ENUM_VALUE( windReedSralai, "wind.reed.sralai" ) \
        ZEUS_ENUM_VALUE( windReedSuona, "wind.reed.suona" ) \
        ZEUS_ENUM_VALUE( windReedSurnai, "wind.reed.surnai" ) \
        ZEUS_ENUM_VALUE( windReedTaepyeongso, "wind.reed.taepyeongso" ) \
        ZEUS_ENUM_VALUE( windReedTarogato, "wind.reed.tarogato" ) \
        ZEUS_ENUM_VALUE( windReedTarogatoAncient, "wind.reed.tarogato.ancient" ) \
        ZEUS_ENUM_VALUE( windReedTrompetaChina, "wind.reed.trompeta-china" ) \
        ZEUS_ENUM_VALUE( windReedTubax, "wind.reed.tubax" ) \
        ZEUS_ENUM_VALUE( windReedXaphoon, "wind.reed.xaphoon" ) \
        ZEUS_ENUM_VALUE( windReedZhaleika, "wind.reed.zhaleika" ) \
        ZEUS_ENUM_VALUE( windReedZurla, "wind.reed.zurla" ) \
        ZEUS_ENUM_VALUE( windReedZurna, "wind.reed.zurna" ) \
        ZEUS_ENUM_VALUE( woodAgogoBlock, "wood.agogo-block" ) \
        ZEUS_ENUM_VALUE( woodAgungATamlang, "wood.agung-a-tamlang" ) \
        ZEUS_ENUM_VALUE( woodAhoko, "wood.ahoko" ) \
        ZEUS_ENUM_VALUE( woodBones, "wood.bones" ) \
        ZEUS_ENUM_VALUE( woodCastanets, "wood.castanets" ) \
        ZEUS_ENUM_VALUE( woodClaves, "wood.claves" ) \
        ZEUS_ENUM_VALUE( woodDrumSticks, "wood.drum-sticks" ) \
        ZEUS_ENUM_VALUE( woodGourd, "wood.gourd" ) \
        ZEUS_ENUM_VALUE( woodGraniteBlock, "wood.granite-block" ) \
        ZEUS_ENUM_VALUE( woodGuban, "wood.guban" ) \
        ZEUS_ENUM_VALUE( woodGuiro, "wood.guiro" ) \
        ZEUS_ENUM_VALUE( woodHyoushigi, "wood.hyoushigi" ) \
        ZEUS_ENUM_VALUE( woodIpu, "wood.ipu" ) \
        ZEUS_ENUM_VALUE( woodJamBlock, "wood.jam-block" ) \
        ZEUS_ENUM_VALUE( woodKaekeeke, "wood.kaekeeke" ) \
        ZEUS_ENUM_VALUE( woodKagul, "wood.kagul" ) \
        ZEUS_ENUM_VALUE( woodKalaau, "wood.kalaau" ) \
        ZEUS_ENUM_VALUE( woodKashiklar, "wood.kashiklar" ) \
        ZEUS_ENUM_VALUE( woodKubing, "wood.kubing" ) \
        ZEUS_ENUM_VALUE( woodPanClappers, "wood.pan-clappers" ) \
        ZEUS_ENUM_VALUE( woodSandBlock, "wood.sand-block" ) \
        ZEUS_ENUM_VALUE( woodSlapstick, "wood.slapstick" ) \
        ZEUS_ENUM_VALUE( woodStirDrum, "wood.stir-drum" ) \
        ZEUS_ENUM_VALUE( woodTempleBlock, "wood.temple-block" ) \
        ZEUS_ENUM_VALUE( woodTicTocBlock, "wood.tic-toc-block" ) \
        ZEUS_ENUM_VALUE( woodTonetang, "wood.tonetang" ) \
        ZEUS_ENUM_VALUE( woodWoodBlock, "wood.wood-block" ) \

        #include "mx/utility/EnumWithString.h"
        #undef ZEUS_ENUM_CLASS_NAME
        #undef ZEUS_ENUM_VALUE
        #undef ZEUS_ENUM_MEMBERS

		enum class PrincipalVoiceSymbol
		{
			hauptstimme = 0,
			nebenstimme = 1,
			plain = 2,
			none = 3
		};
		PrincipalVoiceSymbol parsePrincipalVoiceSymbol( const std::string& value );
		std::string toString( const PrincipalVoiceSymbol value );
		std::ostream& toStream( std::ostream& os, const PrincipalVoiceSymbol value );
		std::ostream& operator<<( std::ostream& os, const PrincipalVoiceSymbol value );

		enum class StartStopChangeContinue
		{
			start = 0,
			stop = 1,
			change = 2,
			continue_ = 3
		};
		StartStopChangeContinue parseStartStopChangeContinue( const std::string& value );
		std::string toString( const StartStopChangeContinue value );
		std::ostream& toStream( std::ostream& os, const StartStopChangeContinue value );
		std::ostream& operator<<( std::ostream& os, const StartStopChangeContinue value );

		enum class TipDirection
		{
			up = 0,
			down = 1,
			left = 2,
			right = 3,
			northwest = 4,
			northeast = 5,
			southeast = 6,
			southwest = 7
		};
		TipDirection parseTipDirection( const std::string& value );
		std::string toString( const TipDirection value );
		std::ostream& toStream( std::ostream& os, const TipDirection value );
		std::ostream& operator<<( std::ostream& os, const TipDirection value );

		enum class StickLocationEnum
		{
			center = 0,
			rim = 1,
			cymbalBell = 2,
			cymbalEdge = 3
		};
		StickLocationEnum parseStickLocationEnum( const std::string& value );
		std::string toString( const StickLocationEnum value );
		std::ostream& toStream( std::ostream& os, const StickLocationEnum value );
		std::ostream& operator<<( std::ostream& os, const StickLocationEnum value );

		enum class StickMaterialEnum
		{
			soft = 0,
			medium = 1,
			hard = 2,
			shaded = 3,
			x = 4
		};
		StickMaterialEnum parseStickMaterialEnum( const std::string& value );
		std::string toString( const StickMaterialEnum value );
		std::ostream& toStream( std::ostream& os, const StickMaterialEnum value );
		std::ostream& operator<<( std::ostream& os, const StickMaterialEnum value );

		enum class StickTypeEnum
		{
			bassDrum = 0,
			doubleBassDrum = 1,
			timpani = 2,
			xylophone = 3,
			yarn = 4
		};
		StickTypeEnum parseStickTypeEnum( const std::string& value );
		std::string toString( const StickTypeEnum value );
		std::ostream& toStream( std::ostream& os, const StickTypeEnum value );
		std::ostream& operator<<( std::ostream& os, const StickTypeEnum value );

		enum class UpDownStopContinue
		{
			up = 0,
			down = 1,
			stop = 2,
			continue_ = 3
		};
		UpDownStopContinue parseUpDownStopContinue( const std::string& value );
		std::string toString( const UpDownStopContinue value );
		std::ostream& toStream( std::ostream& os, const UpDownStopContinue value );
		std::ostream& operator<<( std::ostream& os, const UpDownStopContinue value );

		enum class WedgeType
		{
			crescendo = 0,
			diminuendo = 1,
			stop = 2,
			continue_ = 3
		};
		WedgeType parseWedgeType( const std::string& value );
		std::string toString( const WedgeType value );
		std::ostream& toStream( std::ostream& os, const WedgeType value );
		std::ostream& operator<<( std::ostream& os, const WedgeType value );

		enum class WoodEnum
		{
			boardClapper = 0,
			cabasa = 1,
			castanets = 2,
			claves = 3,
			guiro = 4,
			logDrum = 5,
			maraca = 6,
			maracas = 7,
			ratchet = 8,
			sandpaperBlocks = 9,
			slitDrum = 10,
			templeBlock = 11,
			vibraslap = 12,
			woodBlock = 13
		};
		WoodEnum parseWoodEnum( const std::string& value );
		std::string toString( const WoodEnum value );
		std::ostream& toStream( std::ostream& os, const WoodEnum value );
		std::ostream& operator<<( std::ostream& os, const WoodEnum value );

		enum class MarginType
		{
			odd = 0,
			even = 1,
			both = 2
		};
		MarginType parseMarginType( const std::string& value );
		std::string toString( const MarginType value );
		std::ostream& toStream( std::ostream& os, const MarginType value );
		std::ostream& operator<<( std::ostream& os, const MarginType value );

		enum class NoteSizeType
		{
			cue = 0,
			grace = 1,
			large = 2
		};
		NoteSizeType parseNoteSizeType( const std::string& value );
		std::string toString( const NoteSizeType value );
		std::ostream& toStream( std::ostream& os, const NoteSizeType value );
		std::ostream& operator<<( std::ostream& os, const NoteSizeType value );

		enum class AccidentalValue
		{
			sharp = 0,
			natural = 1,
			flat = 2,
			doubleSharp = 3,
			sharpSharp = 4,
			flatFlat = 5,
			naturalSharp = 6,
			naturalFlat = 7,
			quarterFlat = 8,
			quarterSharp = 9,
			threeQuartersFlat = 10,
			threeQuartersSharp = 11,
			sharpDown = 12,
			sharpUp = 13,
			naturalDown = 14,
			naturalUp = 15,
			flatDown = 16,
			flatUp = 17,
			tripleSharp = 18,
			tripleFlat = 19,
			slashQuarterSharp = 20,
			slashSharp = 21,
			slashFlat = 22,
			doubleSlashFlat = 23,
			sharp1 = 24,
			sharp2 = 25,
			sharp3 = 26,
			sharp5 = 27,
			flat1 = 28,
			flat2 = 29,
			flat3 = 30,
			flat4 = 31,
			sori = 32,
			koron = 33
		};
		AccidentalValue parseAccidentalValue( const std::string& value );
		std::string toString( const AccidentalValue value );
		std::ostream& toStream( std::ostream& os, const AccidentalValue value );
		std::ostream& operator<<( std::ostream& os, const AccidentalValue value );

		enum class ArrowDirectionEnum
		{
			left = 0,
			up = 1,
			right = 2,
			down = 3,
			northwest = 4,
			northeast = 5,
			southeast = 6,
			southwest = 7,
			leftRight = 8,
			upDown = 9,
			northwestSoutheast = 10,
			northeastSouthwest = 11,
			other = 12
		};
		ArrowDirectionEnum parseArrowDirectionEnum( const std::string& value );
		std::string toString( const ArrowDirectionEnum value );
		std::ostream& toStream( std::ostream& os, const ArrowDirectionEnum value );
		std::ostream& operator<<( std::ostream& os, const ArrowDirectionEnum value );

		enum class ArrowStyleEnum
		{
			single = 0,
			double_ = 1,
			filled = 2,
			hollow = 3,
			paired = 4,
			combined = 5,
			other = 6
		};
		ArrowStyleEnum parseArrowStyleEnum( const std::string& value );
		std::string toString( const ArrowStyleEnum value );
		std::ostream& toStream( std::ostream& os, const ArrowStyleEnum value );
		std::ostream& operator<<( std::ostream& os, const ArrowStyleEnum value );

		enum class BeamValue
		{
			begin = 0,
			continue_ = 1,
			end = 2,
			forwardHook = 3,
			backwardHook = 4
		};
		BeamValue parseBeamValue( const std::string& value );
		std::string toString( const BeamValue value );
		std::ostream& toStream( std::ostream& os, const BeamValue value );
		std::ostream& operator<<( std::ostream& os, const BeamValue value );

		enum class BreathMarkValue
		{
			emptystring = 0,
			comma = 1,
			tick = 2
		};
		BreathMarkValue parseBreathMarkValue( const std::string& value );
		std::string toString( const BreathMarkValue value );
		std::ostream& toStream( std::ostream& os, const BreathMarkValue value );
		std::ostream& operator<<( std::ostream& os, const BreathMarkValue value );

		enum class CircularArrowEnum
		{
			clockwise = 0,
			anticlockwise = 1
		};
		CircularArrowEnum parseCircularArrowEnum( const std::string& value );
		std::string toString( const CircularArrowEnum value );
		std::ostream& toStream( std::ostream& os, const CircularArrowEnum value );
		std::ostream& operator<<( std::ostream& os, const CircularArrowEnum value );

		enum class Fan
		{
			accel = 0,
			rit = 1,
			none = 2
		};
		Fan parseFan( const std::string& value );
		std::string toString( const Fan value );
		std::ostream& toStream( std::ostream& os, const Fan value );
		std::ostream& operator<<( std::ostream& os, const Fan value );

		enum class HandbellValue
		{
			damp = 0,
			echo = 1,
			gyro = 2,
			handMartellato = 3,
			malletLift = 4,
			malletTable = 5,
			martellato = 6,
			martellatoLift = 7,
			mutedMartellato = 8,
			pluckLift = 9,
			swing = 10
		};
		HandbellValue parseHandbellValue( const std::string& value );
		std::string toString( const HandbellValue value );
		std::ostream& toStream( std::ostream& os, const HandbellValue value );
		std::ostream& operator<<( std::ostream& os, const HandbellValue value );

		enum class HoleClosedLocation
		{
			right = 0,
			bottom = 1,
			left = 2,
			top = 3
		};
		HoleClosedLocation parseHoleClosedLocation( const std::string& value );
		std::string toString( const HoleClosedLocation value );
		std::ostream& toStream( std::ostream& os, const HoleClosedLocation value );
		std::ostream& operator<<( std::ostream& os, const HoleClosedLocation value );

		enum class HoleClosedValue
		{
			yes = 0,
			no = 1,
			half = 2
		};
		HoleClosedValue parseHoleClosedValue( const std::string& value );
		std::string toString( const HoleClosedValue value );
		std::ostream& toStream( std::ostream& os, const HoleClosedValue value );
		std::ostream& operator<<( std::ostream& os, const HoleClosedValue value );

		enum class NoteTypeValue
		{
			oneThousandTwentyFourth = 0,
			fiveHundredTwelfth = 1,
			twoHundredFifthySixth = 2,
			oneHundredTwentyEighth = 3,
			sixtyFourth = 4,
			thirtySecond = 5,
			sixteenth = 6,
			eighth = 7,
			quarter = 8,
			half = 9,
			whole = 10,
			breve = 11,
			long_ = 12,
			maxima = 13
		};
		NoteTypeValue parseNoteTypeValue( const std::string& value );
		std::string toString( const NoteTypeValue value );
		std::ostream& toStream( std::ostream& os, const NoteTypeValue value );
		std::ostream& operator<<( std::ostream& os, const NoteTypeValue value );

		enum class NoteheadValue
		{
			slash = 0,
			triangle = 1,
			diamond = 2,
			square = 3,
			cross = 4,
			x = 5,
			circleX = 6,
			invertedTriangle = 7,
			arrowDown = 8,
			arrowUp = 9,
			slashed = 10,
			backSlashed = 11,
			normal = 12,
			cluster = 13,
			circleDot = 14,
			leftTriangle = 15,
			rectangle = 16,
			none = 17,
			do_ = 18,
			re = 19,
			mi = 20,
			fa = 21,
			faUp = 22,
			so = 23,
			la = 24,
			ti = 25
		};
		NoteheadValue parseNoteheadValue( const std::string& value );
		std::string toString( const NoteheadValue value );
		std::ostream& toStream( std::ostream& os, const NoteheadValue value );
		std::ostream& operator<<( std::ostream& os, const NoteheadValue value );

		enum class ShowTuplet
		{
			actual = 0,
			both = 1,
			none = 2
		};
		ShowTuplet parseShowTuplet( const std::string& value );
		std::string toString( const ShowTuplet value );
		std::ostream& toStream( std::ostream& os, const ShowTuplet value );
		std::ostream& operator<<( std::ostream& os, const ShowTuplet value );

		enum class StemValue
		{
			down = 0,
			up = 1,
			double_ = 2,
			none = 3
		};
		StemValue parseStemValue( const std::string& value );
		std::string toString( const StemValue value );
		std::ostream& toStream( std::ostream& os, const StemValue value );
		std::ostream& operator<<( std::ostream& os, const StemValue value );

		enum class StepEnum
		{
			a = 0,
			b = 1,
			c = 2,
			d = 3,
			e = 4,
			f = 5,
			g = 6
		};
		StepEnum parseStepEnum( const std::string& value );
		std::string toString( const StepEnum value );
		std::ostream& toStream( std::ostream& os, const StepEnum value );
		std::ostream& operator<<( std::ostream& os, const StepEnum value );

		enum class SyllabicEnum
		{
			single = 0,
			begin = 1,
			end = 2,
			middle = 3
		};
		SyllabicEnum parseSyllabicEnum( const std::string& value );
		std::string toString( const SyllabicEnum value );
		std::ostream& toStream( std::ostream& os, const SyllabicEnum value );
		std::ostream& operator<<( std::ostream& os, const SyllabicEnum value );

		enum class GroupBarlineValue
		{
			yes = 0,
			no = 1,
			mensurstrich = 2
		};
		GroupBarlineValue parseGroupBarlineValue( const std::string& value );
		std::string toString( const GroupBarlineValue value );
		std::ostream& toStream( std::ostream& os, const GroupBarlineValue value );
		std::ostream& operator<<( std::ostream& os, const GroupBarlineValue value );

		enum class GroupSymbolValue
		{
			none = 0,
			brace = 1,
			line = 2,
			bracket = 3,
			square = 4
		};
		GroupSymbolValue parseGroupSymbolValue( const std::string& value );
		std::string toString( const GroupSymbolValue value );
		std::ostream& toStream( std::ostream& os, const GroupSymbolValue value );
		std::ostream& operator<<( std::ostream& os, const GroupSymbolValue value );

        enum class ModeEnum
		{
			major = 0,
			minor = 1,
			dorian = 2,
			phrygian = 3,
			lydian = 4,
			mixolydian = 5,
			aeolian = 6,
			ionian = 7,
			locrian = 8,
			none = 9,
			other = 10
		};
        ModeEnum parseModeEnum( const std::string& value );
        ModeEnum parseModeEnum( const std::string& value, bool& success );
		std::string toString( const ModeEnum value );
		std::ostream& toStream( std::ostream& os, const ModeEnum value );
		std::ostream& operator<<( std::ostream& os, const ModeEnum value );
        
        class ModeValue
        {
        public:
            explicit ModeValue( const ModeEnum value );
            explicit ModeValue( const std::string& value );
            ModeValue();
            ModeEnum getValue() const;
            std::string getValueString() const;
            void setValue( const ModeEnum value );
            void setValue( const std::string& value );
        private:
            ModeEnum myEnum;
            std::string myCustomValue;
        };
        ModeValue parseModeValue( const std::string& value );
		std::string toString( const ModeValue& value );
		std::ostream& toStream( std::ostream& os, const ModeValue& value );
		std::ostream& operator<<( std::ostream& os, const ModeValue& value );
        
        enum class DistanceTypeEnum
		{
			beam = 0,
			hyphen = 1,
			other = 2
		};
        DistanceTypeEnum parseDistanceTypeEnum( const std::string& value );
        DistanceTypeEnum parseDistanceTypeEnum( const std::string& value, bool& success );
		std::string toString( const DistanceTypeEnum value );
		std::ostream& toStream( std::ostream& os, const DistanceTypeEnum value );
		std::ostream& operator<<( std::ostream& os, const DistanceTypeEnum value );
        
        class DistanceType
        {
        public:
            explicit DistanceType( const DistanceTypeEnum value );
            explicit DistanceType( const std::string& value );
            DistanceType();
            DistanceTypeEnum getValue() const;
            std::string getValueString() const;
            void setValue( const DistanceTypeEnum value );
            void setValue( const std::string& value );
        private:
            DistanceTypeEnum myEnum;
            std::string myCustomValue;
        };
        DistanceType parseDistanceType( const std::string& value );
		std::string toString( const DistanceType& value );
		std::ostream& toStream( std::ostream& os, const DistanceType& value );
		std::ostream& operator<<( std::ostream& os, const DistanceType& value );
        
        enum class LineWidthTypeEnum
		{
			beam = 0,
			bracket = 1,
			dashes = 2,
			enclosure = 3,
			ending = 4,
			extend = 5,
			heavyBarline = 6,
			leger = 7,
			lightBarline = 8,
			octaveShift = 9,
			pedal = 10,
			slurMiddle = 11,
			slurTip = 12,
			staff = 13,
			stem = 14,
			tieMiddle = 15,
			tieTip = 16,
			tupletBracket = 17,
			wedge = 18,
			other = 19
		};
        LineWidthTypeEnum parseLineWidthTypeEnum( const std::string& value );
        LineWidthTypeEnum parseLineWidthTypeEnum( const std::string& value, bool& success );
		std::string toString( const LineWidthTypeEnum value );
		std::ostream& toStream( std::ostream& os, const LineWidthTypeEnum value );
		std::ostream& operator<<( std::ostream& os, const LineWidthTypeEnum value );
        
        class LineWidthType
        {
        public:
            explicit LineWidthType( const LineWidthTypeEnum value );
            explicit LineWidthType( const std::string& value );
            LineWidthType();
            LineWidthTypeEnum getValue() const;
            std::string getValueString() const;
            void setValue( const LineWidthTypeEnum value );
            void setValue( const std::string& value );
        private:
            LineWidthTypeEnum myEnum;
            std::string myCustomValue;
        };
        LineWidthType parseLineWidthType( const std::string& value );
		std::string toString( const LineWidthType& value );
		std::ostream& toStream( std::ostream& os, const LineWidthType& value );
		std::ostream& operator<<( std::ostream& os, const LineWidthType& value );
        
        enum class XlinkActuate
        {
            onLoad = 0,
			onRequest = 1,
            other = 2,
            none = 3
        };
        XlinkActuate parseXlinkActuate( const std::string& value );
		std::string toString( const XlinkActuate value );
		std::ostream& toStream( std::ostream& os, const XlinkActuate value );
		std::ostream& operator<<( std::ostream& os, const XlinkActuate value );
        
        enum class XlinkShow
        {
            new_ = 0,
			replace = 1,
            embed = 2,
            other = 3,
            none = 4
        };
        XlinkShow parseXlinkShow( const std::string& value );
		std::string toString( const XlinkShow value );
		std::ostream& toStream( std::ostream& os, const XlinkShow value );
		std::ostream& operator<<( std::ostream& os, const XlinkShow value );
        
        enum class XlinkType
        {
            simple = 0,
			extended = 1,
            title = 2,
            resource = 3,
            locator = 4,
            arc = 5
        };
        XlinkType parseXlinkType( const std::string& value );
		std::string toString( const XlinkType value );
		std::ostream& toStream( std::ostream& os, const XlinkType value );
		std::ostream& operator<<( std::ostream& os, const XlinkType value );
        
        
        enum class XmlSpace
        {
            default_ = 0,
			preserve = 1
        };
        XmlSpace parseXmlSpace( const std::string& value );
		std::string toString( const XmlSpace value );
		std::ostream& toStream( std::ostream& os, const XmlSpace value );
		std::ostream& operator<<( std::ostream& os, const XmlSpace value );

        enum class DynamicsEnum
        {
            p = 0,
            pp = 1,
            ppp = 2,
            pppp = 3,
            ppppp = 4,
            pppppp = 5,
            f = 6,
            ff = 7,
            fff = 8,
            ffff = 9,
            fffff = 10,
            ffffff = 11,
            mp = 12,
            mf = 13,
            sf = 14,
            sfp = 15,
            sfpp = 16,
            fp = 17,
            rf = 18,
            rfz = 19,
            sfz = 20,
            sffz = 21,
            fz = 22,
            otherDynamics = 23
        };
        DynamicsEnum parseDynamicsEnum( const std::string& value );
        DynamicsEnum parseDynamicsEnum( const std::string& value, bool& success );
		std::string toString( const DynamicsEnum value );
		std::ostream& toStream( std::ostream& os, const DynamicsEnum value );
		std::ostream& operator<<( std::ostream& os, const DynamicsEnum value );
        
        class DynamicsValue
        {
        public:
            explicit DynamicsValue( const DynamicsEnum value );
            explicit DynamicsValue( const std::string& value );
            DynamicsValue();
            DynamicsEnum getValue() const;
            std::string getValueString() const;
            void setValue( const DynamicsEnum value );
            void setValue( const std::string& value );
        private:
            DynamicsEnum myEnum;
            std::string myCustomValue;
        };
        DynamicsValue parseDynamicsValue( const std::string& value );
		std::string toString( const DynamicsValue& value );
		std::ostream& toStream( std::ostream& os, const DynamicsValue& value );
		std::ostream& operator<<( std::ostream& os, const DynamicsValue& value );
	}
}
