#!/usr/bin/env python3
"""XSD-to-C++ type mapping tables and resolution logic.

Owns the lookup dicts that map XSD type names to their C++ counterparts in
mx/core, plus helper predicates for serialization pattern selection. Extracted
from generate.py to keep type-mapping concerns in one place and make the tables
easier to locate and extend (e.g. when adding MusicXML 4.0 types).
"""

from parse import pascal

# ---------------------------------------------------------------------------
# Mapping Tables
# ---------------------------------------------------------------------------

XSD_TO_CPP_TYPE = {
    "xs:string": "XsString",
    "xs:token": "XsToken",
    "xs:ID": "XsID",
    "xs:IDREF": "XsIDREF",
    "xs:NMTOKEN": "XsNMToken",
    "xs:anyURI": "XsAnyUri",
    "xs:decimal": "DecimalType",
    "xs:integer": "Integer",
    "xs:nonNegativeInteger": "NonNegativeInteger",
    "xs:positiveInteger": "PositiveInteger",
    "xs:date": "Date",
    "xs:time": "TimeOnly",
    "xml:lang": "XmlLang",
    "xml:space": "XmlSpace",
    "xlink:href": "XlinkHref",
    "xlink:type": "XlinkType",
    "xlink:role": "XlinkRole",
    "xlink:title": "XlinkTitle",
    "xlink:show": "XlinkShow",
    "xlink:actuate": "XlinkActuate",
}

SIMPLE_TYPE_TO_CPP = {
    "above-below": "AboveBelow",
    "accidental-value": "AccidentalValue",
    "backward-forward": "BackwardForward",
    "bar-style": "BarStyleEnum",
    "beam-value": "BeamValue",
    "cancel-location": "CancelLocation",
    "clef-sign": "ClefSign",
    "css-font-size": "CssFontSize",
    "degree-symbol-value": "DegreeSymbolValue",
    "degree-type-value": "DegreeTypeValue",
    "effect-value": "EffectValue",
    "enclosure-shape": "EnclosureShape",
    "fan": "Fan",
    "fermata-shape": "FermataShape",
    "font-style": "FontStyle",
    "font-weight": "FontWeight",
    "group-barline-value": "GroupBarlineValue",
    "group-symbol-value": "GroupSymbolValue",
    "handbell-value": "HandbellValue",
    "harmony-type": "HarmonyType",
    "kind-value": "KindValue",
    "left-center-right": "LeftCenterRight",
    "left-right": "LeftRight",
    "line-end": "LineEnd",
    "line-shape": "LineShape",
    "line-type": "LineType",
    "margin-type": "MarginType",
    "measure-numbering-value": "MeasureNumberingValue",
    "membrane-value": "MembraneValue",
    "metal-value": "MetalValue",
    "mute": "MuteEnum",
    "notehead-value": "NoteheadValue",
    "note-size-type": "NoteSizeType",
    "note-type-value": "NoteTypeValue",
    "on-off": "OnOff",
    "over-under": "OverUnder",
    "pitched-value": "PitchedValue",
    "placement": "AboveBelow",
    "right-left-middle": "RightLeftMiddle",
    "semi-pitched": "SemiPitchedEnum",
    "show-frets": "ShowFrets",
    "show-tuplet": "ShowTuplet",
    "staff-type": "StaffTypeEnum",
    "start-note": "StartNote",
    "start-stop": "StartStop",
    "start-stop-change-continue": "StartStopChangeContinue",
    "start-stop-continue": "StartStopContinue",
    "start-stop-discontinue": "StartStopDiscontinue",
    "start-stop-single": "StartStopSingle",
    "stem-value": "StemValue",
    "step": "StepEnum",
    "syllabic": "SyllabicEnum",
    "symbol-size": "SymbolSize",
    "tap-hand": "TapHand",
    "text-direction": "TextDirection",
    "tied-type": "TiedType",
    "time-relation": "TimeRelationEnum",
    "time-symbol": "TimeSymbol",
    "tip-direction": "TipDirection",
    "top-bottom": "TopBottom",
    "tremolo-type": "TremoloType",
    "trill-step": "TrillStep",
    "two-note-turn": "TwoNoteTurn",
    "up-down": "UpDown",
    "up-down-stop-continue": "UpDownStopContinue",
    "upright-inverted": "UprightInverted",
    "valign": "Valign",
    "valign-image": "ValignImage",
    "wedge-type": "WedgeType",
    "winged": "Winged",
    "wood-value": "WoodValue",
    "yes-no": "YesNo",
}

NUMERIC_TYPE_MAP = {
    "accordion-middle": "AccordionMiddleValue",
    "beam-level": "BeamLevel",
    "divisions": "DivisionsValue",
    "fifths": "FifthsValue",
    "midi-128": "Midi128",
    "midi-16": "Midi16",
    "midi-16384": "Midi16384",
    "millimeters": "MillimetersValue",
    "non-negative-decimal": "NonNegativeDecimal",
    "number-level": "NumberLevel",
    "number-of-lines": "NumberOfLines",
    "octave": "OctaveValue",
    "percent": "Percent",
    "positive-decimal": "PositiveDecimal",
    "positive-divisions": "PositiveDivisionsValue",
    "rotation-degrees": "RotationDegrees",
    "semitones": "Semitones",
    "staff-line": "StaffLine",
    "staff-number": "StaffNumber",
    "string-number": "StringNumber",
    "tenths": "TenthsValue",
    "trill-beats": "TrillBeats",
    "tremolo-marks": "TremoloMarks",
    "byte": "Byte",
}

BESPOKE_TYPES = {
    "color": "Color",
    "comma-separated-text": "CommaSeparatedText",
    "distance-type": "DistanceType",
    "font-size": "FontSize",
    "line-width-type": "LineWidthType",
    "mode": "ModeValue",
    "number-or-normal": "NumberOrNormal",
    "positive-integer-or-empty": "PositiveIntegerOrEmpty",
    "yes-no-number": "YesNoNumber",
    "ending-number": "EndingNumber",
    "date": "Date",
    "time-only": "TimeOnly",
}

STRING_LIKE_TYPES = {
    "XsString", "XsToken", "XsID", "XsIDREF", "XsNMToken", "XsAnyUri",
    "PlaybackSoundType",
}

XMACRO_ENUM_TYPES = {
    "PlaybackSound",
}

NEEDS_PARSE_FUNC_TYPES = {
    "FontStyle", "FontWeight", "AboveBelow", "LeftCenterRight", "Valign",
    "ValignImage", "OverUnder", "TopBottom", "EnclosureShape", "StartStop",
    "StartStopContinue", "StartStopSingle", "StartStopChangeContinue",
    "StartStopDiscontinue", "YesNo", "OnOff", "UpDown", "BackwardForward",
    "LineType", "LineShape", "WedgeType", "BarStyleEnum", "Fan",
    "TipDirection", "TextDirection", "UprightInverted", "LeftRight",
    "RightLeftMiddle", "BeamValue", "AccidentalValue", "ClefSign",
    "StemValue", "NoteheadValue", "StepEnum", "Syllabic", "SymbolSize",
    "TiedType", "FermataShape", "KindValue", "HarmonyType",
    "DegreeTypeValue", "DegreeSymbolValue", "GroupSymbolValue",
    "GroupBarlineValue", "MarginType", "TimeSymbol", "CancelLocation",
    "ShowTuplet", "NoteTypeValue", "HandbellValue", "EffectValue",
    "MetalValue", "WoodValue", "PitchedValue", "MembraneValue",
    "SemiPitched", "TapHand", "TimeRelation", "LineEnd", "ShowFrets",
    "CssFontSize", "MeasureNumberingValue", "StaffTypeEnum",
    "StartNote", "TrillStep", "TwoNoteTurn", "Winged", "TremoloType",
    "UpDownStopContinue", "NoteSizeType", "MuteEnum",
    "BeaterValue", "BreathMarkValue", "HoleClosedValue",
    "HoleClosedLocation", "TimeSeparator", "PrincipalVoiceSymbol",
    "ModeValue", "XmlSpace", "XlinkType", "XlinkShow", "XlinkActuate",
}

TYPE_TO_HEADER = {
    "XsString": "mx/core/XsString.h",
    "XsToken": "mx/core/XsToken.h",
    "XsID": "mx/core/XsID.h",
    "XsIDREF": "mx/core/XsIDREF.h",
    "XsNMToken": "mx/core/XsNMToken.h",
    "XsAnyUri": "mx/core/XsAnyUri.h",
    "XmlLang": "mx/core/XmlLang.h",
    "XlinkHref": "mx/core/XlinkHref.h",
    "XlinkRole": "mx/core/XlinkRole.h",
    "XlinkTitle": "mx/core/XlinkTitle.h",
    "Color": "mx/core/Color.h",
    "CommaSeparatedText": "mx/core/CommaSeparatedText.h",
    "CommaSeparatedPositiveIntegers": "mx/core/CommaSeparatedPositiveIntegers.h",
    "FontSize": "mx/core/FontSize.h",
    "NumberOrNormal": "mx/core/NumberOrNormal.h",
    "PositiveIntegerOrEmpty": "mx/core/PositiveIntegerOrEmpty.h",
    "YesNoNumber": "mx/core/YesNoNumber.h",
    "EndingNumber": "mx/core/EndingNumber.h",
    "Date": "mx/core/Date.h",
    "TimeOnly": "mx/core/TimeOnly.h",
    "PlaybackSound": "mx/core/PlaybackSound.h",
    "PlaybackSoundType": "mx/core/PlaybackSoundType.h",
}


# ---------------------------------------------------------------------------
# Predicates and Resolution
# ---------------------------------------------------------------------------


def needs_parse_func(cpp_type: str) -> bool:
    return cpp_type in NEEDS_PARSE_FUNC_TYPES


def uses_set_value(cpp_type: str) -> bool:
    return cpp_type in STRING_LIKE_TYPES


def is_enum_value_type(cpp_type: str) -> bool:
    return needs_parse_func(cpp_type) or cpp_type.endswith("Enum") or cpp_type in XMACRO_ENUM_TYPES


def parse_func_name(cpp_type: str) -> str:
    if cpp_type in XMACRO_ENUM_TYPES:
        return f"{cpp_type}FromString"
    return f"parse{cpp_type}"


def resolve_cpp_type(xsd_type: str, model) -> str:
    if xsd_type in XSD_TO_CPP_TYPE:
        return XSD_TO_CPP_TYPE[xsd_type]
    if xsd_type.startswith("xs:"):
        return XSD_TO_CPP_TYPE.get(xsd_type, "XsString")
    if xsd_type in SIMPLE_TYPE_TO_CPP:
        return SIMPLE_TYPE_TO_CPP[xsd_type]
    if xsd_type in NUMERIC_TYPE_MAP:
        return NUMERIC_TYPE_MAP[xsd_type]
    if xsd_type in BESPOKE_TYPES:
        return BESPOKE_TYPES[xsd_type]
    if xsd_type in model.enum_types:
        base = pascal(xsd_type)
        if base in model.class_names:
            return base + "Enum"
        return base
    if xsd_type in model.simple_types:
        st = model.simple_types[xsd_type]
        if st["kind"] == "restriction":
            return resolve_cpp_type(st["base"], model)
        if st["kind"] == "union":
            return pascal(xsd_type)
    return pascal(xsd_type)


def resolve_attr_cpp_type(attr, model) -> str:
    return resolve_cpp_type(attr.type_name, model)


def header_for_type(cpp_type: str) -> str:
    if cpp_type in TYPE_TO_HEADER:
        return TYPE_TO_HEADER[cpp_type]
    if "Decimal" in cpp_type or "Tenths" in cpp_type or "Millimeters" in cpp_type or \
       "Percent" in cpp_type or "Semitones" in cpp_type or "TrillBeats" in cpp_type or \
       "RotationDegrees" in cpp_type or "Divisions" in cpp_type:
        return "mx/core/Decimals.h"
    if any(cpp_type == t for t in [
        "AccordionMiddleValue", "BeamLevel", "Byte", "FifthsValue", "Integer",
        "Midi128", "Midi16", "Midi16384", "NonNegativeInteger", "NumberLevel",
        "NumberOfLines", "OctaveValue", "PositiveInteger", "StaffLine",
        "StaffNumber", "StringNumber", "TremoloMarks",
    ]):
        return "mx/core/Integers.h"
    return "mx/core/Enums.h"
