// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/EnumsBuiltin.h"

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

        enum class BreathMarkValue
        {
            emptyString = 0,
            comma = 1,
            tick = 2
        };

        BreathMarkValue parseBreathMarkValue( const std::string& value );
        std::string toString( const BreathMarkValue value );
        std::ostream& toStream( std::ostream& os, const BreathMarkValue value );
        std::ostream& operator<<( std::ostream& os, const BreathMarkValue value );

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

        enum class CircularArrowEnum
        {
            clockwise = 0,
            anticlockwise = 1
        };

        CircularArrowEnum parseCircularArrowEnum( const std::string& value );
        std::string toString( const CircularArrowEnum value );
        std::ostream& toStream( std::ostream& os, const CircularArrowEnum value );
        std::ostream& operator<<( std::ostream& os, const CircularArrowEnum value );

        enum class ClefSign
        {
            g = 0,
            f = 1,
            c = 2,
            percussion = 3,
            TAB = 4,
            jianpu = 5,
            none = 6
        };

        ClefSign parseClefSign( const std::string& value );
        std::string toString( const ClefSign value );
        std::ostream& toStream( std::ostream& os, const ClefSign value );
        std::ostream& operator<<( std::ostream& os, const ClefSign value );

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

        enum class Effect
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

        Effect parseEffect( const std::string& value );
        std::string toString( const Effect value );
        std::ostream& toStream( std::ostream& os, const Effect value );
        std::ostream& operator<<( std::ostream& os, const Effect value );

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

        enum class FermataShape
        {
            normal = 0,
            angled = 1,
            square = 2,
            emptyString = 3
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

        enum class Glass
        {
            windChimes = 0
        };

        Glass parseGlass( const std::string& value );
        std::string toString( const Glass value );
        std::ostream& toStream( std::ostream& os, const Glass value );
        std::ostream& operator<<( std::ostream& os, const Glass value );

        enum class GroupBarlineValue
        {
            yes = 0,
            no = 1,
            Mensurstrich = 2
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
            dominant11th = 16,
            major11th = 17,
            minor11th = 18,
            dominant13th = 19,
            major13th = 20,
            minor13th = 21,
            suspendedSecond = 22,
            suspendedFourth = 23,
            Neapolitan = 24,
            Italian = 25,
            French = 26,
            German = 27,
            pedal = 28,
            power = 29,
            Tristan = 30,
            other = 31,
            none = 32
        };

        KindValue parseKindValue( const std::string& value );
        std::string toString( const KindValue value );
        std::ostream& toStream( std::ostream& os, const KindValue value );
        std::ostream& operator<<( std::ostream& os, const KindValue value );

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

        enum class Membrane
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

        Membrane parseMembrane( const std::string& value );
        std::string toString( const Membrane value );
        std::ostream& toStream( std::ostream& os, const Membrane value );
        std::ostream& operator<<( std::ostream& os, const Membrane value );

        enum class Metal
        {
            almglocken = 0,
            bell = 1,
            bellPlate = 2,
            brakeDrum = 3,
            ChineseCymbal = 4,
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
            VietnameseHat = 22
        };

        Metal parseMetal( const std::string& value );
        std::string toString( const Metal value );
        std::ostream& toStream( std::ostream& os, const Metal value );
        std::ostream& operator<<( std::ostream& os, const Metal value );

        enum class Mute
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

        Mute parseMute( const std::string& value );
        std::string toString( const Mute value );
        std::ostream& toStream( std::ostream& os, const Mute value );
        std::ostream& operator<<( std::ostream& os, const Mute value );

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

        enum class OnOff
        {
            on = 0,
            off = 1
        };

        OnOff parseOnOff( const std::string& value );
        std::string toString( const OnOff value );
        std::ostream& toStream( std::ostream& os, const OnOff value );
        std::ostream& operator<<( std::ostream& os, const OnOff value );

        enum class OverUnder
        {
            over = 0,
            under = 1
        };

        OverUnder parseOverUnder( const std::string& value );
        std::string toString( const OverUnder value );
        std::ostream& toStream( std::ostream& os, const OverUnder value );
        std::ostream& operator<<( std::ostream& os, const OverUnder value );

        enum class Pitched
        {
            chimes = 0,
            glockenspiel = 1,
            mallet = 2,
            marimba = 3,
            tubularChimes = 4,
            vibraphone = 5,
            xylophone = 6
        };

        Pitched parsePitched( const std::string& value );
        std::string toString( const Pitched value );
        std::ostream& toStream( std::ostream& os, const Pitched value );
        std::ostream& operator<<( std::ostream& os, const Pitched value );

        enum class PrincipalVoiceSymbol
        {
            Hauptstimme = 0,
            Nebenstimme = 1,
            plain = 2,
            none = 3
        };

        PrincipalVoiceSymbol parsePrincipalVoiceSymbol( const std::string& value );
        std::string toString( const PrincipalVoiceSymbol value );
        std::ostream& toStream( std::ostream& os, const PrincipalVoiceSymbol value );
        std::ostream& operator<<( std::ostream& os, const PrincipalVoiceSymbol value );

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

        enum class SemiPitched
        {
            high = 0,
            mediumHigh = 1,
            medium = 2,
            mediumLow = 3,
            low = 4,
            veryLow = 5
        };

        SemiPitched parseSemiPitched( const std::string& value );
        std::string toString( const SemiPitched value );
        std::ostream& toStream( std::ostream& os, const SemiPitched value );
        std::ostream& operator<<( std::ostream& os, const SemiPitched value );

        enum class ShowFrets
        {
            numbers = 0,
            letters = 1
        };

        ShowFrets parseShowFrets( const std::string& value );
        std::string toString( const ShowFrets value );
        std::ostream& toStream( std::ostream& os, const ShowFrets value );
        std::ostream& operator<<( std::ostream& os, const ShowFrets value );

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

        enum class StaffType
        {
            ossia = 0,
            cue = 1,
            editorial = 2,
            regular = 3,
            alternate = 4
        };

        StaffType parseStaffType( const std::string& value );
        std::string toString( const StaffType value );
        std::ostream& toStream( std::ostream& os, const StaffType value );
        std::ostream& operator<<( std::ostream& os, const StaffType value );

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

        enum class StickLocation
        {
            center = 0,
            rim = 1,
            cymbalBell = 2,
            cymbalEdge = 3
        };

        StickLocation parseStickLocation( const std::string& value );
        std::string toString( const StickLocation value );
        std::ostream& toStream( std::ostream& os, const StickLocation value );
        std::ostream& operator<<( std::ostream& os, const StickLocation value );

        enum class StickMaterial
        {
            soft = 0,
            medium = 1,
            hard = 2,
            shaded = 3,
            x = 4
        };

        StickMaterial parseStickMaterial( const std::string& value );
        std::string toString( const StickMaterial value );
        std::ostream& toStream( std::ostream& os, const StickMaterial value );
        std::ostream& operator<<( std::ostream& os, const StickMaterial value );

        enum class StickType
        {
            bassDrum = 0,
            doubleBassDrum = 1,
            timpani = 2,
            xylophone = 3,
            yarn = 4
        };

        StickType parseStickType( const std::string& value );
        std::string toString( const StickType value );
        std::ostream& toStream( std::ostream& os, const StickType value );
        std::ostream& operator<<( std::ostream& os, const StickType value );

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

        enum class Wood
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

        Wood parseWood( const std::string& value );
        std::string toString( const Wood value );
        std::ostream& toStream( std::ostream& os, const Wood value );
        std::ostream& operator<<( std::ostream& os, const Wood value );

        enum class YesNo
        {
            yes = 0,
            no = 1
        };

        YesNo parseYesNo( const std::string& value );
        std::string toString( const YesNo value );
        std::ostream& toStream( std::ostream& os, const YesNo value );
        std::ostream& operator<<( std::ostream& os, const YesNo value );
    }
}
