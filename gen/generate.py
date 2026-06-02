#!/usr/bin/env python3
"""MusicXML codegen experiment: generate mx/core element classes from musicxml.xsd.

Iteration 6: Group inlining, fromXElementImpl/streamContents/hasContents fixes.
"""
import os
import re
import sys
from collections import OrderedDict
from dataclasses import dataclass, field
from typing import Optional

from parse import (
    XS,
    ChoiceNode,
    ElementRefNode,
    GroupRefNode,
    ParseConfig,
    SequenceNode,
    XsdAttribute,
    XsdChildRef,
    XsdComplexType,
    XsdElement,
    XsdModel,
    pascal,
)

XSD_PATH = "docs/musicxml.xsd"
CORE_DIR = "src/private/mx/core"
ELEM_DIR = os.path.join(CORE_DIR, "elements")

LICENSE = """\
// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License
"""

CPP_KEYWORDS = {
    "continue", "double", "long", "short", "int", "float", "bool", "char",
    "class", "struct", "enum", "union", "void", "for", "while", "do", "if",
    "else", "switch", "case", "default", "break", "return", "new", "delete",
    "this", "true", "false", "const", "static", "virtual", "public", "private",
    "protected", "namespace", "using", "template", "typename", "operator",
    "and", "or", "not", "xor", "auto", "register", "signed", "unsigned",
    "goto", "throw", "try", "catch", "explicit", "string",
}


def camel(name: str) -> str:
    parts = re.split(r"[-_]", name)
    result = parts[0].lower() + "".join(p[:1].upper() + p[1:] for p in parts[1:])
    if result in CPP_KEYWORDS:
        result += "_"
    return result


def has_flag_name(cpp_n: str) -> str:
    # The presence flag is built from the unescaped identifier: the value field may
    # be keyword-escaped (e.g. 'long_'), but the has-flag must not be ('hasLong',
    # not 'hasLong_'). Strip a trailing underscore added by camel() for keywords.
    base = cpp_n[:-1] if cpp_n.endswith("_") and cpp_n[:-1] in CPP_KEYWORDS else cpp_n
    return "has" + base[0].upper() + base[1:]


# ---------------------------------------------------------------------------
# C++ Type Mapping
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


def uses_set_value(cpp_type: str) -> bool:
    return cpp_type in STRING_LIKE_TYPES


def is_enum_value_type(cpp_type: str) -> bool:
    return needs_parse_func(cpp_type) or cpp_type.endswith("Enum") or cpp_type in XMACRO_ENUM_TYPES


def resolve_cpp_type(xsd_type: str, model: XsdModel) -> str:
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


def resolve_attr_cpp_type(attr: XsdAttribute, model: XsdModel) -> str:
    return resolve_cpp_type(attr.type_name, model)


ENUM_PARSE_FUNCS = {}


def needs_parse_func(cpp_type: str) -> bool:
    return cpp_type in {
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


def parse_func_name(cpp_type: str) -> str:
    if cpp_type in XMACRO_ENUM_TYPES:
        return f"{cpp_type}FromString"
    return f"parse{cpp_type}"


# ---------------------------------------------------------------------------
# Include resolution
# ---------------------------------------------------------------------------

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


# ---------------------------------------------------------------------------
# Enums.h / Enums.cpp generation
# ---------------------------------------------------------------------------


def generate_enums_h(model: XsdModel) -> str:
    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/EnumsBuiltin.h"\n')
    lines.append("#include <iostream>")
    lines.append("#include <optional>")
    lines.append("#include <string>\n")
    lines.append("namespace mx\n{")
    lines.append("namespace core\n{")

    for xsd_name, et in model.enum_types.items():
        cpp_name = pascal(xsd_name)
        if cpp_name in model.class_names:
            cpp_name += "Enum"

        doc = model.enum_docs.get(xsd_name, "")

        lines.append(f"/// {cpp_name} " + "/" * (80 - len(cpp_name) - 5))
        lines.append("///")
        if doc:
            for dline in _wrap_doc(doc, 96):
                lines.append(f"/// {dline}")
            lines.append("///")

        lines.append(f"enum class {cpp_name}")
        lines.append("{")
        for i, val in enumerate(et.values):
            member = camel(val)
            if val == "":
                member = "emptystring"
            lines.append(f"    {member} = {i}" + ("," if i < len(et.values) - 1 else ""))
        lines.append("};\n")

        lines.append(f"{cpp_name} parse{cpp_name}(const std::string &value);")
        lines.append(f"std::optional<{cpp_name}> tryParse{cpp_name}(const std::string &value);")
        lines.append(f"std::string toString(const {cpp_name} value);")
        lines.append(f"std::ostream &toStream(std::ostream &os, const {cpp_name} value);")
        lines.append(f"std::ostream &operator<<(std::ostream &os, const {cpp_name} value);\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def _wrap_doc(text, width):
    words = text.split()
    result = []
    current = ""
    for w in words:
        if current and len(current) + 1 + len(w) > width:
            result.append(current)
            current = w
        else:
            current = current + " " + w if current else w
    if current:
        result.append(current)
    return result


# ---------------------------------------------------------------------------
# Attributes Struct Generation
# ---------------------------------------------------------------------------


def attrs_struct_name(ct_name: str, model: XsdModel) -> str:
    return pascal(ct_name) + "Attributes"


def element_attrs_struct_name(elem_name: str, model: XsdModel) -> str:
    return element_class_name(elem_name) + "Attributes"


CORE_ROOT_ATTRS = {
    "EmptyPrintObjectStyleAlignAttributes",
}

ATTRS_TYPE_ALIAS = {
    "empty-print-style-align": "empty-print-object-style-align",
}

ELEMENTS_DIR_SHARED_ATTRS = {
    "EmptyPlacementAttributes",
    "EmptyLineAttributes",
    "EmptyTrillSoundAttributes",
    "EmptyFontAttributes",
    "EmptyPrintStyleAlignAttributes",
}


def resolve_attrs_name(elem_name: str, type_name: str, model: XsdModel) -> str:
    """Determine the correct attributes struct name for an element.
    Some empty-* types use the type name (shared). Others use element name."""
    aliased = ATTRS_TYPE_ALIAS.get(type_name, type_name)
    type_attrs = pascal(aliased) + "Attributes"
    if type_attrs in CORE_ROOT_ATTRS or type_attrs in ELEMENTS_DIR_SHARED_ATTRS:
        return type_attrs
    return element_class_name(elem_name) + "Attributes"


def generate_attrs_h(struct_name: str, attrs: list, model: XsdModel) -> str:
    preserves_xmlns = struct_name in XMLNS_PRESERVING_ATTRS
    includes = set()
    includes.add("mx/core/AttributesInterface.h")
    includes.add("mx/core/ForwardDeclare.h")
    for a in attrs:
        cpp_t = resolve_attr_cpp_type(a, model)
        h = header_for_type(cpp_t)
        includes.add(h)

    lines = [LICENSE, "#pragma once\n"]
    for inc in sorted(includes):
        lines.append(f'#include "{inc}"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    if preserves_xmlns:
        lines.append("#include <string>")
        lines.append("#include <utility>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    lines.append(f"MX_FORWARD_DECLARE_ATTRIBUTES({struct_name})\n")
    lines.append(f"struct {struct_name} : public AttributesInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {struct_name}();")
    lines.append("    virtual bool hasValues() const;")
    lines.append("    virtual std::ostream &toStream(std::ostream &os) const;")

    for a in attrs:
        cpp_t = resolve_attr_cpp_type(a, model)
        cpp_n = camel(a.name)
        lines.append(f"    {cpp_t} {cpp_n};")

    for a in attrs:
        cpp_n = camel(a.name)
        has_name = has_flag_name(cpp_n)
        const_prefix = "const " if a.use == "required" else ""
        lines.append(f"    {const_prefix}bool {has_name};")

    if preserves_xmlns:
        lines.append("    std::vector<std::pair<std::string, std::string>> xmlnsDeclarations;")

    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


# Per-attribute default value override, keyed by (attrs struct name, camelCase
# attribute field name). The value is the literal C++ initializer expression
# (e.g. '"it"'). Used when the committed code initializes an attribute to a
# value that is not encoded in the XSD (typically a hand-applied convention).
ATTR_DEFAULT_OVERRIDE = {
    # AccidentalText's xml:lang attribute default: hand-applied "it" by the
    # original codegen. Test01_AccidentalText asserts that setting hasLang
    # without a value yields xml:lang="it". Not in the XSD.
    ("AccidentalTextAttributes", "lang"): '"it"',
    ("DirectiveAttributes", "lang"): '"it"',
    # Lyric's justify default is hand-applied; the XSD says only "The default
    # value varies for different elements". The original codegen chose
    # 'center' here based on the doc text in the XSD annotation.
    ("LyricAttributes", "justify"): "LeftCenterRight::center",
    # Score{Partwise,Timewise}Attributes both expose the 'version' attribute
    # from the document-attributes group. XSD says default="1.0" but mx/core
    # hand-applies "3.0" so that newly-constructed scores serialize with the
    # most recent supported version. Schema-driven generators preserve the
    # hand-applied value via this override.
    ("ScorePartwiseAttributes", "version"): '"3.0"',
    ("ScoreTimewiseAttributes", "version"): '"3.0"',
    # R4: xml:lang defaults hand-applied as "it" across text-bearing elements.
    ("WordsAttributes", "lang"): '"it"',
    ("TextAttributes", "lang"): '"it"',
    ("RehearsalAttributes", "lang"): '"it"',
    ("LyricLanguageAttributes", "lang"): '"it"',
    ("CreditWordsAttributes", "lang"): '"it"',
    # R4: BracketAttributes line-end default is 'down', not first enum 'up'.
    ("BracketAttributes", "lineEnd"): "LineEnd::down",
    # R4: NoteSizeAttributes type default is 'large', not first enum 'cue'.
    ("NoteSizeAttributes", "type"): "NoteSizeType::large",
    # R4: EndingAttributes number default is "1".
    ("EndingAttributes", "number"): '"1"',
    # R4: GroupingAttributes number default is "1".
    ("GroupingAttributes", "number"): 'XsToken("1")',
    # R4: PageMarginsAttributes type default is 'both', not first enum 'odd'.
    ("PageMarginsAttributes", "type"): "MarginType::both",
    # R4: LinkAttributes show default is 'replace', not first enum 'new'.
    # Field name in struct is 'show', not 'xlinkShow'.
    ("LinkAttributes", "show"): "XlinkShow::replace",
    # R4: OtherAppearanceAttributes type default is "undefined".
    ("OtherAppearanceAttributes", "type"): '"undefined"',
    # R4: OtherNotationAttributes type default is 'start', not first enum 'single'.
    ("OtherNotationAttributes", "type"): "StartStopSingle::start",
    # R4: OtherOrnament/TechnicalAttributes placement: default_value_for_type
    # returns AboveBelow::below but the committed code uses the default ctor
    # which is AboveBelow::above (= 0).
    ("OtherOrnamentAttributes", "placement"): "AboveBelow::above",
    ("OtherTechnicalAttributes", "placement"): "AboveBelow::above",
    # R4: PrincipalVoiceAttributes symbol default is 'none'.
    ("PrincipalVoiceAttributes", "symbol"): "PrincipalVoiceSymbol::none",
    # R4: StringMuteAttributes type default is 'on', not 'off'.
    ("StringMuteAttributes", "type"): "OnOff::on",
    # R4: PartGroupAttributes number default is "1".
    ("PartGroupAttributes", "number"): 'XsToken("1")',
    # R4: MetronomeAttributes halign/justify defaults are 'center'.
    ("MetronomeAttributes", "halign"): "LeftCenterRight::center",
    ("MetronomeAttributes", "justify"): "LeftCenterRight::center",
}

# Attribute structs that preserve xmlns:* namespace declarations through
# round-trip. These elements may carry xmlns:xlink or other namespace
# declarations that mx does not model as typed fields, but must not drop.
XMLNS_PRESERVING_ATTRS = {
    "ScorePartwiseAttributes",
    "ScoreTimewiseAttributes",
    "OpusAttributes",
    "LinkAttributes",
}

# Per-(parent-element-xml-name, child-element-xml-name) override for the
# constructor argument passed to make{Child}() when initializing the child
# on the parent's ctor init list. Used when HEAD initializes a required child
# with a non-default value (e.g. historical author choice rather than XSD spec).
CHILD_INIT_VALUE_OVERRIDE = {
    # Scaling's millimeters and tenths use non-zero historical defaults.
    ("scaling", "millimeters"): "MillimetersValue(7)",
    ("scaling", "tenths"): "TenthsValue(40)",
    # StaffDetails defaults staff-lines to 5 (author convention, not in XSD).
    ("staff-details", "staff-lines"): "NonNegativeInteger(5)",
}


# Elements whose hasContents() should always return true regardless of what the
# XSD min/max-occurs analysis would produce. Keyed by element xml-name (not
# class name). Used when the committed HEAD hardcodes `return true;` for an
# element that has only optional children.
ELEMENT_HAS_CONTENTS_ALWAYS_TRUE = {
    # MeasureLayout has a single optional child (measure-distance), but HEAD
    # returns true unconditionally so that the element serialises as
    # <measure-layout></measure-layout> rather than <measure-layout/>.
    "measure-layout",
}

# Per-(element-name, child-xml-name) override for the min_occurs value that the
# generator uses when deciding whether a child needs a myHas flag. Keyed by
# (parent_element_xml_name, child_element_xml_name). Use this when XSD group
# inlining propagates minOccurs=0 from the enclosing group to an element that
# HEAD treats as unconditionally present (no getHas/setHas accessors).
CHILD_MIN_OCCURS_OVERRIDE = {}


def _apply_child_min_occurs_override(elem_name: str, children: list) -> list:
    """Return a new children list with CHILD_MIN_OCCURS_OVERRIDE applied."""
    if not any((elem_name, c.element_name) in CHILD_MIN_OCCURS_OVERRIDE
               for c in children):
        return children
    result = []
    for c in children:
        key = (elem_name, c.element_name)
        if key in CHILD_MIN_OCCURS_OVERRIDE:
            c = XsdChildRef(
                element_name=c.element_name,
                min_occurs=CHILD_MIN_OCCURS_OVERRIDE[key],
                max_occurs=c.max_occurs,
                is_group=c.is_group,
            )
        result.append(c)
    return result


def default_value_for_type(cpp_type: str) -> str:
    defaults = {
        "FontStyle": "FontStyle::normal",
        "FontWeight": "FontWeight::normal",
        "AboveBelow": "AboveBelow::below",
        "Valign": "Valign::bottom",
        "ValignImage": "ValignImage::bottom",
        "LeftCenterRight": "LeftCenterRight::left",
        "EnclosureShape": "EnclosureShape::none",
        "YesNo": "YesNo::no",
        "OnOff": "OnOff::off",
        "FontSize": "CssFontSize::medium",
        "StartStop": "StartStop::start",
        "StartStopContinue": "StartStopContinue::start",
        "StartStopSingle": "StartStopSingle::single",
        "LineType": "LineType::solid",
        "LineShape": "LineShape::straight",
        "SymbolSize": "SymbolSize::full",
    }
    return defaults.get(cpp_type, "")


def generate_attrs_cpp(struct_name: str, attrs: list, model: XsdModel) -> str:
    preserves_xmlns = struct_name in XMLNS_PRESERVING_ATTRS
    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{struct_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    # constructor
    init_parts = []
    for a in attrs:
        cpp_t = resolve_attr_cpp_type(a, model)
        cpp_n = camel(a.name)
        override = ATTR_DEFAULT_OVERRIDE.get((struct_name, cpp_n))
        if override:
            init_parts.append(f"{cpp_n}({override})")
            continue
        dv = default_value_for_type(cpp_t)
        if dv:
            init_parts.append(f"{cpp_n}({dv})")
        else:
            init_parts.append(f"{cpp_n}()")
    for a in attrs:
        cpp_n = camel(a.name)
        has_name = has_flag_name(cpp_n)
        init_val = "true" if a.use == "required" else "false"
        init_parts.append(f"{has_name}({init_val})")

    _emit_ctor_init(lines, f"{struct_name}::{struct_name}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    # hasValues
    has_parts = []
    for a in attrs:
        cpp_n = camel(a.name)
        has_name = has_flag_name(cpp_n)
        has_parts.append(has_name)
    if preserves_xmlns:
        has_parts.append("!xmlnsDeclarations.empty()")
    lines.append(f"bool {struct_name}::hasValues() const")
    lines.append("{")
    if has_parts:
        lines.append(f"    return {' || '.join(has_parts)};")
    else:
        lines.append("    return false;")
    lines.append("}\n")

    # toStream
    lines.append(f"std::ostream &{struct_name}::toStream(std::ostream &os) const")
    lines.append("{")
    lines.append("    if (hasValues())")
    lines.append("    {")
    for a in attrs:
        cpp_n = camel(a.name)
        has_name = has_flag_name(cpp_n)
        lines.append(f'        streamAttribute(os, {cpp_n}, "{a.get_xml_name()}", {has_name});')
    if preserves_xmlns:
        lines.append("        for (const auto &ns : xmlnsDeclarations)")
        lines.append("        {")
        lines.append('            os << " " << ns.first << "=\\"" << ns.second << "\\"";')
        lines.append("        }")
    lines.append("    }")
    lines.append("    return os;")
    lines.append("}\n")

    # fromXElementImpl
    lines.append(f"bool {struct_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append(f'    const char *const className = "{struct_name}";')
    lines.append("    bool isSuccess = true;")
    required_locals = []
    for a in attrs:
        if a.use == "required":
            cpp_n = camel(a.name)
            local_name = "is" + pascal(a.name) + "Found"
            required_locals.append((a, local_name))
            lines.append(f"    bool {local_name} = false;")
    lines.append("")
    lines.append("    auto it = xelement.attributesBegin();")
    lines.append("    auto endIter = xelement.attributesEnd();\n")
    lines.append("    for (; it != endIter; ++it)")
    lines.append("    {")
    required_local_map = {id(a): ln for a, ln in required_locals}
    for a in attrs:
        cpp_t = resolve_attr_cpp_type(a, model)
        cpp_n = camel(a.name)
        parse_has = required_local_map.get(id(a), has_flag_name(cpp_n))
        if needs_parse_func(cpp_t):
            pf = parse_func_name(cpp_t)
            lines.append(f"        if (parseAttribute(message, it, className, isSuccess, {cpp_n}, {parse_has}, "
                         f'"{a.get_xml_name()}", &{pf}))')
        else:
            lines.append(f"        if (parseAttribute(message, it, className, isSuccess, {cpp_n}, {parse_has}, "
                         f'"{a.get_xml_name()}"))')
        lines.append("        {")
        lines.append("            continue;")
        lines.append("        }")
    if preserves_xmlns:
        lines.append("        const auto attrName = it->getName();")
        lines.append('        if (attrName == "xmlns" || (attrName.size() > 6 && attrName.substr(0, 6) == "xmlns:"))')
        lines.append("        {")
        lines.append("            xmlnsDeclarations.emplace_back(attrName, it->getValue());")
        lines.append("            continue;")
        lines.append("        }")
    lines.append("    }\n")
    for a, local_name in required_locals:
        lines.append(f"    if (!{local_name})")
        lines.append("    {")
        lines.append("        isSuccess = false;")
        # Use the XSD attribute name (xml form, e.g. 'non-controlling') in
        # the error message rather than hardcoding 'number'. The original
        # codegen had a bug here that produced the wrong attribute name for
        # any required attribute not named 'number' (visible in committed
        # ScorePartAttributes.cpp, which says 'number' when it should say
        # 'id').
        xml_name = a.get_xml_name() or a.name
        lines.append(f'        message << className << ": \'{xml_name}\' is a required attribute but was not found" << std::endl;')
        lines.append("    }\n")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Element Class Generation
# ---------------------------------------------------------------------------


def classify_element(elem: XsdElement, model: XsdModel) -> str:
    if elem.anonymous_type is not None:
        ct = elem.anonymous_type
    elif not elem.type_name:
        return "unknown"
    else:
        ct = model.complex_types.get(elem.type_name)
    if ct is None:
        if elem.type_name in model.simple_types or elem.type_name in model.enum_types or \
           elem.type_name.startswith("xs:"):
            return "simple-value"
        return "unknown"

    if ct.has_simple_content:
        if ct.attributes:
            return "text-with-attrs"
        return "text-value"

    if ct.has_choice and not ct.children:
        return "choice"

    if ct.children:
        if ct.has_choice:
            return "sequence-with-choice"
        if ct.attributes:
            return "complex-with-attrs"
        return "complex"

    if ct.attributes and not ct.children:
        return "empty-with-attrs"

    if not ct.attributes and not ct.children:
        return "empty"

    return "unknown"


def child_class_name(child: XsdChildRef) -> str:
    name = pascal(child.element_name)
    if child.is_group and child.element_name in GENERATE_GROUPS:
        if child.element_name not in SUPPRESS_GROUP_SUFFIX:
            name += "Group"
    return name


PATTERN_B_EXCEPTIONS = {"String"}


def is_pattern_b(elem_type: str, ct: Optional[XsdComplexType], model: XsdModel,
                 class_name: str = "") -> bool:
    if class_name in PATTERN_B_EXCEPTIONS:
        return True
    if ct is None:
        return False
    if ct.children or ct.has_choice:
        return True
    type_name = ct.name
    if type_name in model.complex_types:
        real_ct = model.complex_types[type_name]
        if real_ct.children or real_ct.has_choice:
            return True
    return _ct_has_complex_content(type_name, model)


def _ct_has_complex_content(type_name: str, model: XsdModel) -> bool:
    return type_name in model.complex_content_or_group_cts


def generate_element_h(elem_name: str, class_name: str, stream_name: str,
                       elem_type: str, ct: Optional[XsdComplexType],
                       model: XsdModel, type_name: str = "") -> str:
    attrs_name = None
    value_type = None
    children = []

    if ct:
        if ct.attributes:
            attrs_name = resolve_attrs_name(elem_name, type_name or ct.name, model)
        if ct.has_simple_content:
            value_type = resolve_cpp_type(ct.simple_content_base, model)
        children = _apply_child_min_occurs_override(elem_name, ct.children)

    # Apply value-type override if configured for this element
    vt_override = ELEMENT_VALUE_TYPE_OVERRIDE.get(elem_name)
    if vt_override:
        value_type = vt_override["cpp_type"]

    lines = [LICENSE, "#pragma once\n"]
    project_includes = ['"mx/core/ElementInterface.h"', '"mx/core/ForwardDeclare.h"']
    if value_type:
        project_includes.append(f'"{header_for_type(value_type)}"')
    if vt_override:
        for extra in vt_override.get("extra_includes", []):
            project_includes.append(f'"{extra}"')
    if attrs_name:
        if attrs_name in CORE_ROOT_ATTRS:
            project_includes.append(f'"mx/core/{attrs_name}.h"')
        else:
            project_includes.append(f'"mx/core/elements/{attrs_name}.h"')
    for inc in sorted(project_includes):
        lines.append(f"#include {inc}")

    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")

    has_unbounded_synth = any(
        c.is_group and c.element_name in SYNTHETIC_UNBOUNDED_GROUPS
        for c in children
    )
    if has_unbounded_synth:
        lines.append("namespace ezxml\n{\nclass XElementIterator;\n}")
        lines.append("")

    lines.append("namespace mx\n{\nnamespace core\n{\n")

    if attrs_name:
        lines.append(f"MX_FORWARD_DECLARE_ATTRIBUTES({attrs_name})")

    child_classes = sorted(set(child_class_name(c) for c in children))
    for cc in child_classes:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({cc})")

    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({class_name})\n")

    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")

    if value_type:
        if value_type in XMACRO_ENUM_TYPES:
            lines.append(f"\ninline {class_name}Ptr make{class_name}({value_type} value)")
            lines.append("{")
            lines.append(f"    return std::make_shared<{class_name}>(value);")
            lines.append("}")
        else:
            lines.append(f"\ninline {class_name}Ptr make{class_name}(const {value_type} &value)")
            lines.append("{")
            lines.append(f"    return std::make_shared<{class_name}>(value);")
            lines.append("}")
            lines.append(f"\ninline {class_name}Ptr make{class_name}({value_type} &&value)")
            lines.append("{")
            lines.append(f"    return std::make_shared<{class_name}>(std::move(value));")
            lines.append("}")

    lines.append(f"\nclass {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")

    if value_type:
        lines.append(f"    {class_name}(const {value_type} &value);")

    lines.append("")
    pattern_b = is_pattern_b(elem_type, ct, model, class_name)
    if pattern_b:
        lines.append("    virtual bool hasAttributes() const;")
        lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
        lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
        lines.append("    virtual bool hasContents() const;")
        lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    else:
        lines.append("    virtual bool hasAttributes() const;")
        lines.append("    virtual bool hasContents() const;")
        lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
        lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
        lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")

    if attrs_name:
        lines.append(f"    {attrs_name}Ptr getAttributes() const;")
        lines.append(f"    void setAttributes(const {attrs_name}Ptr &attributes);")

    if value_type:
        lines.append(f"    {value_type} getValue() const;")
        lines.append(f"    void setValue(const {value_type} &value);")

    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f"\n    /* _________ {cc} minOccurs = {child.min_occurs}, maxOccurs = unbounded _________ */")
            lines.append(f"    const {cc}Set &get{cc}Set() const;")
            lines.append(f"    void add{cc}(const {cc}Ptr &value);")
            lines.append(f"    void remove{cc}(const {cc}SetIterConst &value);")
            lines.append(f"    void clear{cc}Set();")
            lines.append(f"    {cc}Ptr get{cc}(const {cc}SetIterConst &setIterator) const;")
        elif child.min_occurs == 0:
            lines.append(f"\n    /* _________ {cc} minOccurs = 0, maxOccurs = 1 _________ */")
            lines.append(f"    {cc}Ptr get{cc}() const;")
            lines.append(f"    void set{cc}(const {cc}Ptr &value);")
            lines.append(f"    bool getHas{cc}() const;")
            lines.append(f"    void setHas{cc}(const bool value);")
        else:
            lines.append(f"\n    /* _________ {cc} minOccurs = 1, maxOccurs = 1 _________ */")
            lines.append(f"    {cc}Ptr get{cc}() const;")
            lines.append(f"    void set{cc}(const {cc}Ptr &value);")

    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")

    has_private_members = bool(attrs_name) or bool(value_type) or bool(children)
    if has_private_members:
        lines.append("")
        lines.append("  private:")
        if value_type:
            lines.append(f"    {value_type} myValue;")
        if attrs_name:
            lines.append(f"    {attrs_name}Ptr myAttributes;")
        for child in children:
            cc = child_class_name(child)
            if child.max_occurs != 1:
                lines.append(f"    {cc}Set my{cc}Set;")
            else:
                lines.append(f"    {cc}Ptr my{cc};")
                if child.min_occurs == 0:
                    lines.append(f"    bool myHas{cc};")

        unbounded_synth_children = [
            c for c in children
            if c.is_group and c.element_name in SYNTHETIC_UNBOUNDED_GROUPS
        ]
        for child in unbounded_synth_children:
            cc = child_class_name(child)
            lines.append("")
            lines.append(f"    void import{cc}Set(std::ostream &message, ::ezxml::XElementIterator &iter,")
            indent = " " * (len(f"    void import{cc}Set("))
            lines.append(f"{indent}::ezxml::XElementIterator &endIter, bool &isSuccess);")

    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def _emit_ctor_init(lines: list, prefix: str, init_parts: list):
    if not init_parts:
        lines.append(prefix)
        return
    first_line = f"{prefix} : {init_parts[0]}"
    if len(init_parts) == 1:
        lines.append(first_line)
        return
    all_on_one = f"{prefix} : {', '.join(init_parts)}"
    if len(all_on_one) <= 110:
        lines.append(all_on_one)
        return
    indent = " " * 4 + ": "
    current = f"{prefix}\n{indent}{init_parts[0]}"
    for part in init_parts[1:]:
        candidate = current + f", {part}"
        last_line = candidate.split("\n")[-1]
        if len(last_line) > 110:
            current += f",\n      {part}"
        else:
            current += f", {part}"
    lines.append(current)


def generate_element_cpp(elem_name: str, class_name: str, stream_name: str,
                         elem_type: str, ct: Optional[XsdComplexType],
                         model: XsdModel, type_name: str = "") -> str:
    attrs_name = None
    value_type = None
    children = []

    if ct:
        if ct.attributes:
            attrs_name = resolve_attrs_name(elem_name, type_name or ct.name, model)
        if ct.has_simple_content:
            value_type = resolve_cpp_type(ct.simple_content_base, model)
        children = _apply_child_min_occurs_override(elem_name, ct.children)

    # Apply value-type override if configured for this element
    vt_override = ELEMENT_VALUE_TYPE_OVERRIDE.get(elem_name)
    if vt_override:
        value_type = vt_override["cpp_type"]

    has_contents = bool(value_type) or bool(children)

    # R5: elements whose XSD declares a `minOccurs >= 1, maxOccurs=unbounded`
    # child set must pre-seed the set with one default member, return
    # hasContents() unconditionally, and refuse to drop below size 1 in
    # remove/clear. Matches HEAD's pattern for harp-pedals, scordatura,
    # frame, figured-bass.
    seeded_children = [
        c for c in (children or [])
        if c.min_occurs >= 1 and c.max_occurs != 1 and not c.is_group
    ]

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    include_set = set(
        f'#include "mx/core/elements/{child_class_name(c)}.h"'
        for c in children
    )
    # The fromXElementImpl for a parent with a synthetic optional group
    # inlines the parsing of the group's members; pull those member headers
    # into the parent .cpp so make*() and getXxx() resolve to complete types.
    for child in children:
        if (child.is_group
                and (child.element_name in SYNTHETIC_OPTIONAL_GROUPS
                     or child.element_name in SYNTHETIC_UNBOUNDED_GROUPS)):
            for gm in model.groups.get(child.element_name, []):
                include_set.add(
                    f'#include "mx/core/elements/{child_class_name(gm)}.h"'
                )
    child_includes = sorted(include_set)
    for inc in child_includes:
        lines.append(inc)
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    # constructor
    init_parts = []
    if value_type:
        default_val = ELEMENT_DEFAULT_VALUE.get(
            elem_name, TYPE_DEFAULT_VALUE.get(value_type, ""))
        if default_val:
            init_parts.append(f"myValue({default_val})")
        else:
            init_parts.append("myValue()")
    if attrs_name:
        init_parts.append(f"myAttributes(std::make_shared<{attrs_name}>())")
    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            init_parts.append(f"my{cc}Set()")
        else:
            child_init_val = CHILD_INIT_VALUE_OVERRIDE.get((elem_name, child.element_name))
            if child_init_val:
                init_parts.append(f"my{cc}(make{cc}({child_init_val}))")
            else:
                init_parts.append(f"my{cc}(make{cc}())")
            if child.min_occurs == 0:
                init_parts.append(f"myHas{cc}(false)")

    use_ei_ctor = not value_type and not children
    if not value_type:
        if use_ei_ctor and init_parts:
            all_init = ["ElementInterface()"] + init_parts
            _emit_ctor_init(lines, f"{class_name}::{class_name}()", all_init)
        elif use_ei_ctor:
            lines.append(f"{class_name}::{class_name}() : ElementInterface()")
        elif init_parts:
            _emit_ctor_init(lines, f"{class_name}::{class_name}()", init_parts)
        else:
            lines.append(f"{class_name}::{class_name}() : ElementInterface()")
        lines.append("{")
        for sc in seeded_children:
            scc = child_class_name(sc)
            lines.append(f"    my{scc}Set.push_back(make{scc}());")
        lines.append("}\n")
    else:
        default_val = ELEMENT_DEFAULT_VALUE.get(
            elem_name, TYPE_DEFAULT_VALUE.get(value_type, ""))
        default_init = f"myValue({default_val})" if default_val else "myValue()"
        all_init = [default_init]
        if attrs_name:
            all_init.append(f"myAttributes(std::make_shared<{attrs_name}>())")
        ctor_prefix = f"{class_name}::{class_name}()"
        _emit_ctor_init(lines, ctor_prefix, all_init)
        lines.append("{")
        lines.append("}\n")
        val_init = ["myValue(value)"]
        if attrs_name:
            val_init.append(f"myAttributes(std::make_shared<{attrs_name}>())")
        val_prefix = f"{class_name}::{class_name}(const {value_type} &value)"
        _emit_ctor_init(lines, val_prefix, val_init)
        lines.append("{")
        lines.append("}\n")

    pattern_b = is_pattern_b(elem_type, ct, model, class_name)

    def _gen_hasAttributes():
        lines.append(f"bool {class_name}::hasAttributes() const")
        lines.append("{")
        if attrs_name:
            lines.append("    return myAttributes->hasValues();")
        else:
            lines.append("    return false;")
        lines.append("}\n")

    def _gen_hasContents():
        lines.append(f"bool {class_name}::hasContents() const")
        lines.append("{")
        if elem_name in ELEMENT_HAS_CONTENTS_ALWAYS_TRUE:
            lines.append("    return true;")
        elif value_type:
            lines.append("    return true;")
        elif children:
            has_required = any(
                c.min_occurs >= 1 and c.max_occurs == 1 and not c.is_group
                for c in children
            )
            if has_required or seeded_children:
                lines.append("    return true;")
            else:
                parts = []
                for child in children:
                    cc = child_class_name(child)
                    if child.max_occurs != 1:
                        parts.append(f"my{cc}Set.size() > 0")
                    elif child.is_group and child.max_occurs != 1:
                        parts.append(f"my{cc}Set.size() > 0")
                    elif child.is_group and child.min_occurs == 0:
                        # Optional group: gate on the has-flag, not on the
                        # group's own hasContents(), which may return true even
                        # when the group should not be emitted (e.g.
                        # DisplayStepOctaveGroup always returns hasContents=true
                        # because its members are required within the group).
                        parts.append(f"myHas{cc}")
                    elif child.is_group:
                        parts.append(f"my{cc}->hasContents()")
                    elif child.min_occurs == 0:
                        parts.append(f"myHas{cc}")
                if parts:
                    lines.append(f"    return {' || '.join(parts)};")
                else:
                    lines.append("    return false;")
        else:
            lines.append("    return false;")
        lines.append("}\n")

    def _gen_streamAttributes():
        lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
        lines.append("{")
        if attrs_name and children:
            lines.append("    return myAttributes->toStream(os);")
        elif attrs_name:
            lines.append("    if (myAttributes)")
            lines.append("    {")
            lines.append("        myAttributes->toStream(os);")
            lines.append("    }")
            lines.append("    return os;")
        else:
            lines.append("    return os;")
        lines.append("}\n")

    def _gen_streamName():
        lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
        lines.append("{")
        lines.append(f'    os << "{stream_name}";')
        lines.append("    return os;")
        lines.append("}\n")

    def _emit_stream_children(indent: str):
        for child in children:
            cc = child_class_name(child)
            if child.is_group and child.max_occurs != 1:
                is_unbounded_synth = (
                    child.element_name in SYNTHETIC_UNBOUNDED_GROUPS)
                lines.append(f"{indent}for (auto x : my{cc}Set)")
                lines.append(f"{indent}{{")
                if is_unbounded_synth:
                    # Synthetic unbounded group members are all-optional, so
                    # the wrapper class can be empty; gate the newline+stream
                    # on x->hasContents() to suppress blank entries.
                    lines.append(f"{indent}    if (x->hasContents())")
                    lines.append(f"{indent}    {{")
                    lines.append(f"{indent}        os << std::endl;")
                    lines.append(f"{indent}        x->streamContents(os, indentLevel + 1, isOneLineOnly);")
                    lines.append(f"{indent}    }}")
                else:
                    lines.append(f"{indent}    os << std::endl;")
                    lines.append(f"{indent}    x->streamContents(os, indentLevel + 1, isOneLineOnly);")
                lines.append(f"{indent}}}")
            elif child.is_group:
                # Any optional group (minOccurs=0) gates on its myHas flag
                # rather than on hasContents(). This matches the original code
                # for time-modification / NormalTypeNormalDotGroup (synthetic
                # optional groups) as well as inlined XSD groups like
                # display-step-octave that are also optional on the parent.
                if child.min_occurs == 0:
                    guard = f"myHas{cc}"
                else:
                    guard = f"my{cc}->hasContents()"
                lines.append(f"{indent}if ({guard})")
                lines.append(f"{indent}{{")
                lines.append(f"{indent}    os << std::endl;")
                lines.append(f"{indent}    my{cc}->streamContents(os, indentLevel + 1, isOneLineOnly);")
                lines.append(f"{indent}}}")
            elif child.max_occurs != 1:
                lines.append(f"{indent}for (auto x : my{cc}Set)")
                lines.append(f"{indent}{{")
                lines.append(f"{indent}    os << std::endl;")
                lines.append(f"{indent}    x->toStream(os, indentLevel + 1);")
                lines.append(f"{indent}}}")
            elif child.min_occurs == 0:
                lines.append(f"{indent}if (myHas{cc})")
                lines.append(f"{indent}{{")
                lines.append(f"{indent}    os << std::endl;")
                lines.append(f"{indent}    my{cc}->toStream(os, indentLevel + 1);")
                lines.append(f"{indent}}}")
            else:
                lines.append(f"{indent}os << std::endl;")
                lines.append(f"{indent}my{cc}->toStream(os, indentLevel + 1);")

    def _gen_streamContents():
        lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
        lines.append("{")
        if value_type:
            lines.append("    MX_UNUSED(indentLevel);")
            lines.append("    isOneLineOnly = true;")
            if value_type in XMACRO_ENUM_TYPES:
                lines.append(f"    os << {value_type}ToString(myValue);")
            else:
                lines.append("    os << myValue;")
        elif children:
            has_required = any(
                c.min_occurs >= 1 and c.max_occurs == 1 and not c.is_group
                for c in children
            )
            use_wrapping = elem_name in WRAPPING_STREAMCONTENTS
            if has_required or seeded_children:
                lines.append("    isOneLineOnly = false;")
                _emit_stream_children("    ")
                lines.append("    os << std::endl;")
            elif use_wrapping:
                lines.append("    if (hasContents())")
                lines.append("    {")
                _emit_stream_children("        ")
                lines.append("        isOneLineOnly = false;")
                lines.append("        os << std::endl;")
                lines.append("    }")
                lines.append("    else")
                lines.append("    {")
                lines.append("        isOneLineOnly = true;")
                lines.append("    }")
            else:
                _emit_stream_children("    ")
                # Build actual content check from children directly so that
                # ELEMENT_HAS_CONTENTS_ALWAYS_TRUE elements (which return
                # true unconditionally from hasContents()) still set
                # isOneLineOnly correctly based on what was actually emitted.
                content_parts = []
                for c in children:
                    cc = child_class_name(c)
                    if c.max_occurs != 1:
                        content_parts.append(f"my{cc}Set.size() > 0")
                    elif c.min_occurs == 0 and c.is_group:
                        content_parts.append(f"myHas{cc}")
                    elif c.min_occurs == 0:
                        content_parts.append(f"myHas{cc}")
                    else:
                        content_parts.append("true")
                actual_check = " || ".join(content_parts) if content_parts else "false"
                lines.append(f"    if ({actual_check})")
                lines.append("    {")
                lines.append("        isOneLineOnly = false;")
                lines.append("        os << std::endl;")
                lines.append("    }")
                lines.append("    else")
                lines.append("    {")
                lines.append("        isOneLineOnly = true;")
                lines.append("    }")
        else:
            lines.append("    MX_UNUSED(indentLevel);")
            lines.append("    isOneLineOnly = true;")
        lines.append("    return os;")

    if pattern_b:
        _gen_hasAttributes()
        _gen_streamAttributes()
        _gen_streamName()
        _gen_hasContents()
        _gen_streamContents()
    else:
        _gen_hasAttributes()
        _gen_hasContents()
        _gen_streamAttributes()
        _gen_streamName()
        _gen_streamContents()
    lines.append("}\n")

    # getAttributes / setAttributes
    if attrs_name:
        lines.append(f"{attrs_name}Ptr {class_name}::getAttributes() const")
        lines.append("{")
        lines.append("    return myAttributes;")
        lines.append("}\n")
        lines.append(f"void {class_name}::setAttributes(const {attrs_name}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append("        myAttributes = value;")
        lines.append("    }")
        lines.append("}\n")

    # getValue / setValue
    if value_type:
        lines.append(f"{value_type} {class_name}::getValue() const")
        lines.append("{")
        lines.append("    return myValue;")
        lines.append("}\n")
        lines.append(f"void {class_name}::setValue(const {value_type} &value)")
        lines.append("{")
        lines.append("    myValue = value;")
        lines.append("}\n")

    # child accessors
    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            is_seeded = child in seeded_children
            lines.append(f"const {cc}Set &{class_name}::get{cc}Set() const")
            lines.append("{")
            lines.append(f"    return my{cc}Set;")
            lines.append("}\n")
            lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
            lines.append("{")
            lines.append(f"    if (value != my{cc}Set.cend())")
            lines.append("    {")
            if is_seeded:
                lines.append(f"        if (my{cc}Set.size() > 1)")
                lines.append("        {")
                lines.append(f"            my{cc}Set.erase(value);")
                lines.append("        }")
            else:
                lines.append(f"        my{cc}Set.erase(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc}Set.push_back(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {class_name}::clear{cc}Set()")
            lines.append("{")
            lines.append(f"    my{cc}Set.clear();")
            if is_seeded:
                lines.append(f"    my{cc}Set.push_back(make{cc}());")
            lines.append("}\n")
            lines.append(f"{cc}Ptr {class_name}::get{cc}(const {cc}SetIterConst &setIterator) const")
            lines.append("{")
            lines.append(f"    if (setIterator != my{cc}Set.cend())")
            lines.append("    {")
            lines.append("        return *setIterator;")
            lines.append("    }")
            lines.append(f"    return {cc}Ptr();")
            lines.append("}\n")
        elif child.min_occurs == 0:
            lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
            lines.append("{")
            lines.append(f"    return my{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"bool {class_name}::getHas{cc}() const")
            lines.append("{")
            lines.append(f"    return myHas{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::setHas{cc}(const bool value)")
            lines.append("{")
            lines.append(f"    myHas{cc} = value;")
            lines.append("}\n")
        else:
            lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
            lines.append("{")
            lines.append(f"    return my{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}\n")

    # fromXElementImpl
    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    if attrs_name and not value_type and not children:
        lines.append("    return myAttributes->fromXElement(message, xelement);")
    elif value_type and attrs_name:
        if uses_set_value(value_type):
            parse_call = "myValue.setValue(xelement.getValue());"
        elif is_enum_value_type(value_type):
            pfn = parse_func_name(value_type)
            parse_call = f"myValue = {pfn}(xelement.getValue());"
        else:
            parse_call = "myValue.parse(xelement.getValue());"
        lines.append("    bool isSuccess = true;")
        lines.append("    isSuccess &= myAttributes->fromXElement(message, xelement);")
        lines.append(f"    {parse_call}")
        lines.append("    MX_RETURN_IS_SUCCESS;")
    elif value_type:
        is_enum = is_enum_value_type(value_type)
        if uses_set_value(value_type):
            parse_call = "myValue.setValue(xelement.getValue());"
        elif is_enum:
            pfn = parse_func_name(value_type)
            parse_call = f"myValue = {pfn}(xelement.getValue());"
        else:
            parse_call = "myValue.parse(xelement.getValue());"
        lines.append("    MX_UNUSED(message);")
        if not is_enum:
            lines.append("    MX_UNUSED(xelement);")
        lines.append(f"    {parse_call}")
        lines.append("    return true;")
    elif children:
        lines.append("    bool isSuccess = true;")
        if attrs_name:
            lines.append("    isSuccess &= myAttributes->fromXElement(message, xelement);")
        required_children = [
            c for c in children
            if c.min_occurs >= 1 and c.max_occurs == 1 and not c.is_group
        ]
        for child in required_children:
            cc = child_class_name(child)
            lines.append(f"    bool is{cc}Found = false;")
        lines.append("")
        lines.append("    auto endIter = xelement.end();")
        lines.append("    for (auto it = xelement.begin(); it != endIter; ++it)")
        lines.append("    {")
        for child in children:
            cc = child_class_name(child)
            if (child.is_group and child.max_occurs != 1
                    and child.element_name in SYNTHETIC_UNBOUNDED_GROUPS):
                # Synthetic unbounded group: emit a call to a private helper
                # method that parses consecutive members into one wrapper
                # group instance per pass. The helper is generated below.
                lines.append(
                    f"        import{cc}Set(message, it, endIter, isSuccess);")
            elif (child.is_group and child.min_occurs == 0
                    and child.element_name in SYNTHETIC_OPTIONAL_GROUPS):
                # No importGroup() exists for an anonymous synthetic group.
                # Inline the parsing of its members directly into the parent
                # element's fromXElementImpl loop, matching the original
                # TimeModification.cpp parser.
                group_members = list(model.groups.get(child.element_name, []))
                for gm in group_members:
                    gm_cc = child_class_name(gm)
                    lines.append(f'        if (it->getName() == "{gm.element_name}")')
                    lines.append("        {")
                    lines.append(f"            myHas{cc} = true;")
                    if gm.max_occurs != 1:
                        local = camel(gm.element_name)
                        lines.append(f"            auto {local} = make{gm_cc}();")
                        lines.append(f"            isSuccess &= {local}->fromXElement(message, *it);")
                        lines.append(f"            my{cc}->add{gm_cc}({local});")
                    else:
                        lines.append(f"            isSuccess = my{cc}->get{gm_cc}()->fromXElement(message, *it);")
                    lines.append("        }")
            elif child.is_group and child.min_occurs == 0:
                lines.append(f"        importGroup(message, it, endIter, isSuccess, my{cc}, myHas{cc});")
            elif child.is_group:
                lines.append(f"        importGroup(message, it, endIter, isSuccess, my{cc});")
            elif child.max_occurs != 1:
                lines.append(f'        importElementSet(message, it, endIter, isSuccess, "{child.element_name}", my{cc}Set);')
            elif child.min_occurs == 0:
                lines.append(f"        if (importElement(message, *it, isSuccess, *my{cc}, myHas{cc}))")
                lines.append("        {")
                lines.append("            continue;")
                lines.append("        }")
            else:
                lines.append(f"        if (importElement(message, *it, isSuccess, *my{cc}, is{cc}Found))")
                lines.append("        {")
                lines.append("            continue;")
                lines.append("        }")
        lines.append("    }")
        lines.append("")
        lines.append("    MX_RETURN_IS_SUCCESS;")
    else:
        lines.append("    MX_UNUSED(message);")
        lines.append("    MX_UNUSED(xelement);")
        lines.append("    return true;")
    lines.append("}\n")

    # Helper methods for synthetic unbounded groups (e.g.
    # ScorePart::importMidiDeviceInstrumentGroupSet).
    for child in children:
        if (child.is_group and child.max_occurs != 1
                and child.element_name in SYNTHETIC_UNBOUNDED_GROUPS):
            _emit_synthetic_unbounded_helper(
                lines, class_name, child, model)

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def _emit_synthetic_unbounded_helper(lines: list, parent_class: str,
                                     child_ref: "XsdChildRef",
                                     model: "XsdModel") -> None:
    group_class = child_class_name(child_ref)
    set_name = f"my{group_class}Set"
    members = list(model.groups.get(child_ref.element_name, []))
    if not members:
        return
    name_check = " || ".join(
        f'iter->getName() == "{m.element_name}"' for m in members)
    name_not_check = " && ".join(
        f'iter->getName() != "{m.element_name}"' for m in members)
    has_check = " || ".join(
        f"item->getHas{child_class_name(m)}()" for m in members)

    sig_first = f"void {parent_class}::import{group_class}Set(std::ostream &message, ::ezxml::XElementIterator &iter,"
    sig_indent = " " * len(f"void {parent_class}::import{group_class}Set(")
    lines.append(sig_first)
    lines.append(f"{sig_indent}::ezxml::XElementIterator &endIter, bool &isSuccess)")
    lines.append("{")
    lines.append("    bool doDecrementIter = false;")
    lines.append(f"    while (iter != endIter && ({name_check}))")
    lines.append("    {")
    lines.append(f"        auto item = make{group_class}();")
    for idx, m in enumerate(members):
        mc = child_class_name(m)
        local = camel(m.element_name)
        guard = (f'iter->getName() == "{m.element_name}"' if idx == 0
                 else f'iter != endIter && iter->getName() == "{m.element_name}"')
        lines.append(f"        if ({guard})")
        lines.append("        {")
        lines.append(f"            item->setHas{mc}(true);")
        lines.append(f"            auto {local} = item->get{mc}();")
        lines.append(f"            isSuccess &= {local}->fromXElement(message, *iter);")
        if m.element_name in SYNTHETIC_UNBOUNDED_GROUP_IMPORT_GROUP_AFTER:
            lines.append(f"            importGroup(message, iter, endIter, isSuccess, {local});")
        lines.append("            doDecrementIter = true;")
        lines.append("            ++iter;")
        lines.append("        }")
        lines.append("")
    lines.append(f"        if ({has_check})")
    lines.append("        {")
    lines.append(f"            {set_name}.push_back(item);")
    lines.append("        }")
    lines.append("")
    lines.append(f"        if (iter != endIter && {name_not_check})")
    lines.append("        {")
    lines.append("            if (doDecrementIter)")
    lines.append("            {")
    lines.append("                --iter;")
    lines.append("            }")
    lines.append("            return;")
    lines.append("        }")
    lines.append("    }")
    lines.append("    if (doDecrementIter)")
    lines.append("    {")
    lines.append("        --iter;")
    lines.append("    }")
    lines.append("}\n")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

# Elements whose generated code intentionally replaces the original bespoke
# implementation. Diffs are exempt from eval penalty scoring. The set contains
# file stems (PascalCase, no extension) that eval.py matches against.
OVERWRITE_FILE_STEMS = {
    "Direction", "DirectionType", "DirectionAttributes",
}

# Maps XSD element names to C++ class names when they differ from pascal(elem_name).
# The XML stream name remains the original elem_name; only the C++ identifier changes.
ELEMENT_CLASS_NAME_OVERRIDE = {
    "attributes": "Properties",  # XSD 'attributes' -> C++ class 'Properties'
}

# Override the value type for a simple-value or text-value element.
# Each entry maps elem_name -> dict with:
#   cpp_type:  the C++ type to use instead of whatever the XSD says
#   header:    the header file providing that type
#   default:   the default-value expression for the constructor
#   extra_includes: additional headers to include (list of strings)
# The streaming / parsing pattern is inferred from XMACRO_ENUM_TYPES,
# is_enum_value_type, or uses_set_value, just like any other value type.
ELEMENT_VALUE_TYPE_OVERRIDE = {
    "instrument-sound": {
        "cpp_type": "PlaybackSoundType",
        "header": "mx/core/PlaybackSoundType.h",
        "default": "PlaybackSoundType{}",
    },
}

# Types defined via X-macros (EnumWithString.h pattern) that provide
# XxxToString / XxxFromString free functions instead of operator<< / parseXxx.
XMACRO_ENUM_TYPES = {
    "PlaybackSound",
}


def element_class_name(elem_name: str) -> str:
    """Return the C++ class name for an element, consulting overrides first."""
    return ELEMENT_CLASS_NAME_OVERRIDE.get(elem_name, pascal(elem_name))


SKIP_ELEMENTS = {
    # score-partwise, score-timewise: handled by shared bespoke generator
    # _emit_score_wrapper_family, parameterized via SCORE_WRAPPER_FLAVOR_CONFIG.
    # Each emits {Outer, set holder, music-data holder} + their attrs structs.
    # part, measure: claimed by the score-wrapper-family handler (both
    # partwise and timewise dispatch entries claim each name under a
    # different class prefix). Listed in BESPOKE_FAMILY_OWNED rather than
    # SKIP_ELEMENTS because they ARE fully generated, just not by their
    # own dispatch entry.
    # directive: handled via anonymous_type path (text-with-attrs, anon CT)
    # part-list: handled by bespoke generator (PartGroupOrScorePart)
    # credit: handled by bespoke generator (CreditChoice + CreditWordsGroup)
    # key: handled by tree-based generation
    # lyric: handled by bespoke generator (LyricTextChoice + SyllabicTextGroup
    # + ElisionSyllabicTextGroup + ElisionSyllabicGroup)
    # notations, ornaments: handled by tree-based generation
    # part-abbreviation-display, part-name-display: handled by tree-based generation
    # score-instrument: handled by tree-based generation (SoloOrEnsembleChoice)
    # score-part: handled via UNBOUNDED_SEQUENCE_AS_GROUP -> MidiDeviceInstrumentGroup
    # time-modification: handled via synthetic NormalTypeNormalDotGroup
    # (anonymous nested optional sequence promoted to a group class)
}

# Elements whose code is emitted by some other bespoke handler as part of a
# family (e.g. score-partwise's family handler emits PartwisePart and
# PartwiseMeasure too). The main discovery loop must skip these so the default
# path doesn't try to generate competing files, but they are NOT counted as
# skipped because they ARE fully generated -- just not by their own dispatch
# entry. Distinct from SKIP_ELEMENTS which represents elements with no
# generator coverage at all.
BESPOKE_FAMILY_OWNED = {
    "part",     # PartwisePart (partwise) + TimewisePart (timewise)
    "measure",  # PartwiseMeasure (partwise) + TimewiseMeasure (timewise)
}

TREE_ELEMENTS = {
    "bend",
    "group-abbreviation-display",
    "group-name-display",
    "harmonic",
    "key",
    "metronome",
    "notations",
    "notehead-text",
    "ornaments",
    "part-abbreviation-display",
    "part-name-display",
    "play",
    "score-instrument",
}

TREE_ELEMENT_CONFIG = {
    "group-abbreviation-display": {
        "choice_class": "DisplayTextOrAccidentalText",
    },
    "group-name-display": {
        "choice_class": "DisplayTextOrAccidentalText",
    },
    "harmonic": {
        "inline_choices": [
            {"choice_class": "HarmonicTypeChoice"},
            {"choice_class": "HarmonicInfoChoice"},
        ],
    },
    "part-abbreviation-display": {
        "choice_class": "DisplayTextOrAccidentalText",
    },
    "part-name-display": {
        "choice_class": "DisplayTextOrAccidentalText",
        "always_has_contents": True,
    },
    "notehead-text": {
        "choice_class": "NoteheadTextChoice",
        # HEAD seeds the choice set with one default item (displayText) so that
        # hasContents() returns true and the element serialises with content.
        "always_has_contents": True,
        "seed_choice_set": True,
    },
    "play": {
        "inlined_choice": True,
    },
    "score-instrument": {
        "choice_class": "SoloOrEnsembleChoice",
    },
    "metronome": {
        "choice_class": "BeatUnitPerOrNoteRelationNoteChoice",
        "container_names": {
            0: "BeatUnitPer",
            1: "NoteRelationNote",
        },
        "branch_enum_names": {
            0: "beatUnitPer",
            1: "noteRelationNote",
        },
    },
    # Issues E/F: route choice-group parsing through private member functions
    # on the parent element (e.g. Key::importTraditionalKey,
    # Key::importNonTraditionalKey) instead of through public importGroup(...)
    # overloads in FromXElement.cpp. Each group branch in the parent's choice
    # produces one private member: bool import<BranchClass>(...). The parent's
    # fromXElementImpl dispatches by calling those members; the choice's
    # setChoice(...) is performed inside the member body.
    "key": {
        "parent_imports_choice_groups": True,
    },
}

# Populated dynamically by XsdModel._synthesize_optional_group as we discover
# anonymous <xs:sequence minOccurs="0"> inside parent sequences. The names
# stored here are the lowercase-hyphenated form (e.g. "normal-type-normal-dot")
# which round-trips through pascal() to produce the synthetic group class
# (e.g. "NormalTypeNormalDotGroup").
SYNTHETIC_OPTIONAL_GROUPS: set = set()

# Populated dynamically by XsdModel._synthesize_unbounded_group when we
# discover an anonymous <xs:sequence minOccurs="0" maxOccurs="unbounded">
# inside a parent sequence. The original codegen promoted some of these
# shapes to wrapper group classes used as Sets on the parent
# (e.g. score-part's midi-device + midi-instrument repeating sequence
# becomes MidiDeviceInstrumentGroup, held as a *Set on ScorePart).
SYNTHETIC_UNBOUNDED_GROUPS: set = set()

# Opt-in: complex types whose anonymous nested <xs:sequence minOccurs="0">
# should be promoted to a synthetic group rather than flattened. The XSD
# permits the same shape in several places (e.g. page-layout), but the
# original codegen only chose to promote it in specific spots. The value is
# the hyphenated-lowercase ref name used as the synthetic group's element_name.
NESTED_OPTIONAL_SEQUENCE_AS_GROUP: dict = {
    "time-modification": "normal-type-normal-dot",
}

# Opt-in: when an extending complexType inherits a synthetic optional group
# from its base, the default behavior is to flatten the group's members into
# the extending type. For specific extending types the original codegen
# instead kept the group as a *separately-named wrapper sub-element* with
# its own getHas/setHas accessors. The mapping is
#   extending_type_name -> { base_synthetic_group_name -> renamed_wrapper_group_name }
# The renamed group's class name omits the usual "Group" suffix (see
# SUPPRESS_GROUP_SUFFIX), so a child reference to it renders as a regular
# wrapper element on the parent. Its members are still parsed inline like any
# other synthetic optional group (the original hand-written MetronomeTuplet.cpp
# parsed the wrapper with a no-op importElement and dropped normal-type /
# normal-dot on round-trip; that was a bug).
EXTENSION_OPTIONAL_GROUP_RENAME: dict = {
    "metronome-tuplet": {
        "normal-type-normal-dot": "time-modification-normal-type-normal-dot",
    },
}

# Group names whose generated class name omits the trailing "Group" suffix.
SUPPRESS_GROUP_SUFFIX: set = set()


def group_class_name(group_name: str) -> str:
    if group_name in SUPPRESS_GROUP_SUFFIX:
        return pascal(group_name)
    return pascal(group_name) + "Group"

# Opt-in: complex types whose anonymous <xs:sequence minOccurs="0"
# maxOccurs="unbounded"> should be promoted to a synthetic unbounded group.
# Mapping parent_type_name -> hyphenated-lowercase synthetic group ref.
UNBOUNDED_SEQUENCE_AS_GROUP: dict = {
    "score-part": "midi-device-instrument",
}

# Element names whose generated synthetic-unbounded-group parser body should
# emit an additional importGroup(messsage, iter, endIter, isSuccess, elemPtr)
# call after parsing that element. The original codegen produced this call
# for midi-instrument (a no-op in practice because importGroup(MidiInstrument)
# inspects only sibling iterators that have already been consumed). Kept to
# minimize diff against committed.
SYNTHETIC_UNBOUNDED_GROUP_IMPORT_GROUP_AFTER = {
    "midi-instrument",
}

GENERATE_GROUPS = {
    "beat-unit", "display-step-octave", "editorial", "editorial-voice",
    "editorial-voice-direction", "layout", "score-header",
    # full-note: EXC - real code has FullNoteTypeChoice class
    # time-signature: EXC - real code adds Interchangeable not in XSD group
    # harmony-chord: EXC - real code has Choice logic not in XSD group def
    # music-data: EXC - real code wraps choice in MusicDataChoice class
}

WRAPPING_STREAMCONTENTS = {
    "defaults", "grouping", "identification", "part-group", "print",
}

TYPE_DEFAULT_VALUE = {
    "AccidentalValue": "AccidentalValue::natural",
    "ArrowDirectionEnum": "ArrowDirectionEnum::up",
    "ArrowStyleEnum": "ArrowStyleEnum::single",
    "BarStyleEnum": "BarStyleEnum::regular",
    "BeamValue": "BeamValue::begin",
    "BeaterValue": "BeaterValue::snareStick",
    "BreathMarkValue": "BreathMarkValue::emptystring",
    "CircularArrowEnum": "CircularArrowEnum::clockwise",
    "ClefSign": "ClefSign::g",
    "DegreeTypeValue": "DegreeTypeValue::add",
    "EffectEnum": "EffectEnum::anvil",
    "FermataShape": "FermataShape::normal",
    "GlassEnum": "GlassEnum::windChimes",
    "GroupBarlineValue": "GroupBarlineValue::yes",
    "GroupSymbolValue": "GroupSymbolValue::none",
    "HandbellValue": "HandbellValue::damp",
    "HoleClosedValue": "HoleClosedValue::no",
    "KindValue": "KindValue::none",
    "MeasureNumberingValue": "MeasureNumberingValue::none",
    "MembraneEnum": "MembraneEnum::snareDrum",
    "MetalEnum": "MetalEnum::bell",
    "MuteEnum": "MuteEnum::off",
    "NoteTypeValue": "NoteTypeValue::eighth",
    "NoteheadValue": "NoteheadValue::normal",
    "PitchedEnum": "PitchedEnum::xylophone",
    "SemiPitchedEnum": "SemiPitchedEnum::medium",
    "StaffTypeEnum": "StaffTypeEnum::regular",
    "StemValue": "StemValue::none",
    "StepEnum": "StepEnum::a",
    "StickLocationEnum": "StickLocationEnum::center",
    "StickMaterialEnum": "StickMaterialEnum::medium",
    "StickTypeEnum": "StickTypeEnum::yarn",
    "SyllabicEnum": "SyllabicEnum::begin",
    "TimeRelationEnum": "TimeRelationEnum::equals",
    "WoodEnum": "WoodEnum::claves",
    "PlaybackSound": "PlaybackSound::keyboardPiano",
}

ELEMENT_DEFAULT_VALUE = {
    "type": "NoteTypeValue::quarter",
    "duration": "1.0",
    "tremolo": "3",
    "metronome-relation": '"equals"',
}


def generate_group_h(group_name: str, children: list, model: XsdModel) -> str:
    class_name = group_class_name(group_name)

    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")

    child_classes = [child_class_name(c) for c in children]
    for cc in sorted(set(child_classes)):
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({cc})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({class_name})\n")

    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")

    lines.append(f"\nclass {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")

    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f"\n    /* _________ {cc} minOccurs = {child.min_occurs}, maxOccurs = unbounded _________ */")
            lines.append(f"    const {cc}Set &get{cc}Set() const;")
            lines.append(f"    void add{cc}(const {cc}Ptr &value);")
            lines.append(f"    void remove{cc}(const {cc}SetIterConst &value);")
            lines.append(f"    void clear{cc}Set();")
            lines.append(f"    {cc}Ptr get{cc}(const {cc}SetIterConst &setIterator) const;")
        elif child.min_occurs == 0:
            lines.append(f"\n    /* _________ {cc} minOccurs = 0, maxOccurs = 1 _________ */")
            lines.append(f"    {cc}Ptr get{cc}() const;")
            lines.append(f"    void set{cc}(const {cc}Ptr &value);")
            lines.append(f"    bool getHas{cc}() const;")
            lines.append(f"    void setHas{cc}(const bool value);")
        else:
            lines.append(f"\n    /* _________ {cc} minOccurs = 1, maxOccurs = 1 _________ */")
            lines.append(f"    {cc}Ptr get{cc}() const;")
            lines.append(f"    void set{cc}(const {cc}Ptr &value);")

    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f"    {cc}Set my{cc}Set;")
        else:
            lines.append(f"    {cc}Ptr my{cc};")
            if child.min_occurs == 0:
                lines.append(f"    bool myHas{cc};")

    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_group_cpp(group_name: str, children: list, model: XsdModel) -> str:
    class_name = group_class_name(group_name)

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    child_includes = sorted(set(
        f'#include "mx/core/elements/{child_class_name(c)}.h"'
        for c in children
    ))
    for inc in child_includes:
        lines.append(inc)
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    init_parts = []
    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            init_parts.append(f"my{cc}Set()")
        else:
            init_parts.append(f"my{cc}(make{cc}())")
            if child.min_occurs == 0:
                init_parts.append(f"myHas{cc}(false)")

    _emit_ctor_init(lines, f"{class_name}::{class_name}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    parts = []
    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            parts.append(f"my{cc}Set.size() > 0")
        elif child.min_occurs == 0:
            parts.append(f"myHas{cc}")
        else:
            parts.append("true")
    if any("true" == p for p in parts):
        lines.append("    return true;")
    elif parts:
        lines.append(f"    return {' || '.join(parts)};")
    else:
        lines.append("    return false;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    all_optional = all(c.min_occurs == 0 and c.max_occurs == 1 for c in children)
    if all_optional:
        lines.append("    bool firstItem = true;")
        lines.append("    isOneLineOnly = true;")
        for child in children:
            cc = child_class_name(child)
            lines.append(f"    if (myHas{cc})")
            lines.append("    {")
            lines.append("        if (!firstItem)")
            lines.append("            os << std::endl;")
            lines.append(f"        my{cc}->toStream(os, indentLevel);")
            lines.append("        firstItem = false;")
            lines.append("    }")
    else:
        lines.append("    bool isFirst = true;")
        for child in children:
            cc = child_class_name(child)
            if child.max_occurs != 1:
                lines.append(f"    for (auto x : my{cc}Set)")
                lines.append("    {")
                lines.append("        if (!isFirst)")
                lines.append("            os << std::endl;")
                lines.append("        x->toStream(os, indentLevel);")
                lines.append("        isFirst = false;")
                lines.append("    }")
            elif child.min_occurs == 0:
                lines.append(f"    if (myHas{cc})")
                lines.append("    {")
                lines.append("        if (!isFirst)")
                lines.append("            os << std::endl;")
                lines.append(f"        my{cc}->toStream(os, indentLevel);")
                lines.append("        isFirst = false;")
                lines.append("    }")
            else:
                lines.append("    if (!isFirst)")
                lines.append("        os << std::endl;")
                lines.append(f"    my{cc}->toStream(os, indentLevel);")
                lines.append("    isFirst = false;")
    lines.append("    isOneLineOnly = !hasContents();")
    lines.append("    return os;")
    lines.append("}\n")

    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f"const {cc}Set &{class_name}::get{cc}Set() const")
            lines.append("{")
            lines.append(f"    return my{cc}Set;")
            lines.append("}\n")
            lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
            lines.append("{")
            lines.append(f"    if (value != my{cc}Set.cend())")
            lines.append("    {")
            lines.append(f"        my{cc}Set.erase(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc}Set.push_back(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {class_name}::clear{cc}Set()")
            lines.append("{")
            lines.append(f"    my{cc}Set.clear();")
            lines.append("}\n")
            lines.append(f"{cc}Ptr {class_name}::get{cc}(const {cc}SetIterConst &setIterator) const")
            lines.append("{")
            lines.append(f"    if (setIterator != my{cc}Set.cend())")
            lines.append("    {")
            lines.append("        return *setIterator;")
            lines.append("    }")
            lines.append(f"    return {cc}Ptr();")
            lines.append("}\n")
        elif child.min_occurs == 0:
            lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
            lines.append("{")
            lines.append(f"    return my{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"bool {class_name}::getHas{cc}() const")
            lines.append("{")
            lines.append(f"    return myHas{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::setHas{cc}(const bool value)")
            lines.append("{")
            lines.append(f"    myHas{cc} = value;")
            lines.append("}\n")
        else:
            lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
            lines.append("{")
            lines.append(f"    return my{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}\n")

    # fromXElementImpl - most groups use the UNUSED macro since they are
    # imported via importGroup helpers in the parent. score-header is the
    # exception: ScorePartwise/ScoreTimewise call myScoreHeaderGroup->fromXElement
    # directly, so it needs a real parsing body.
    if _group_needs_real_from_x(group_name):
        _emit_group_real_from_x_impl(lines, class_name, children)
    else:
        lines.append(f"MX_FROM_XELEMENT_UNUSED({class_name});\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


GROUPS_WITH_REAL_FROM_X_ELEMENT = {
    "score-header",
    # ArrowGroup is the inline-group branch of the <arrow> inline-choice element
    # (INLINE_CHOICE_CONFIG["arrow"]). Arrow::fromXElementImpl dispatches its
    # group branch via myArrowGroup->fromXElement(message, xelement), so the
    # group needs a real parsing body.
    "arrow",
}


def _group_needs_real_from_x(group_name: str) -> bool:
    # The original codegen emits a real fromXElementImpl body for the
    # synthetic optional groups (e.g. NormalTypeNormalDotGroup), even though
    # they are never invoked directly by the parent (which inlines its own
    # parsing). Preserve that behavior to minimize diff against committed.
    return (group_name in GROUPS_WITH_REAL_FROM_X_ELEMENT
            or group_name in SYNTHETIC_OPTIONAL_GROUPS)


def _emit_group_real_from_x_impl(lines: list, class_name: str, children: list) -> None:
    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")

    required = [c for c in children if c.min_occurs >= 1 and c.max_occurs == 1]
    for child in required:
        cc = child_class_name(child)
        lines.append(f"    bool is{cc}Found = false;")

    unbounded = [c for c in children if c.max_occurs != 1]
    for child in unbounded:
        cc = child_class_name(child)
        lines.append(f"    bool isFirst{cc}Added = false;")

    lines.append("    for (auto it = xelement.begin(); it != xelement.end(); ++it)")
    lines.append("    {")
    lines.append("        const std::string elementName = it->getName();")
    lines.append("")
    for idx, child in enumerate(children):
        cc = child_class_name(child)
        elem_name = child.element_name
        cond = "if" if idx == 0 else "else if"
        lines.append(f'        {cond} (elementName == "{elem_name}")')
        lines.append("        {")
        if child.max_occurs != 1:
            local = camel(elem_name)
            lines.append(f"            auto {local} = make{cc}();")
            lines.append(f"            isSuccess &= {local}->fromXElement(message, *it);")
            lines.append("")
            lines.append(f"            if (!isFirst{cc}Added && my{cc}Set.size() == 1)")
            lines.append("            {")
            lines.append(f"                *(my{cc}Set.begin()) = {local};")
            lines.append(f"                isFirst{cc}Added = true;")
            lines.append("            }")
            lines.append("            else")
            lines.append("            {")
            lines.append(f"                my{cc}Set.push_back({local});")
            lines.append(f"                isFirst{cc}Added = true;")
            lines.append("            }")
        elif child.min_occurs == 0:
            lines.append(f"            myHas{cc} = true;")
            lines.append(f"            isSuccess &= my{cc}->fromXElement(message, *it);")
        else:
            lines.append(f"            is{cc}Found = true;")
            lines.append(f"            isSuccess &= my{cc}->fromXElement(message, *it);")
        lines.append("        }")

    lines.append("        else")
    lines.append("        {")
    if required:
        first_required = required[0]
        rcc = child_class_name(first_required)
        rname = first_required.element_name
        lines.append(f"            if (!is{rcc}Found)")
        lines.append("            {")
        lines.append(f'                message << "{class_name}: a \'{rname}\' element is required but was not found" << std::endl;')
        lines.append("                return false;")
        lines.append("            }")
    lines.append("        }")
    lines.append("    }")
    lines.append("")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}\n")

DYNAMICS_MARKS = {
    "p", "pp", "ppp", "pppp", "ppppp", "pppppp",
    "f", "ff", "fff", "ffff", "fffff", "ffffff",
    "mp", "mf", "sf", "sfp", "sfpp", "fp", "rf", "rfz", "sfz", "sffz", "fz",
    "other-dynamics",
}

CHOICE_SKIP = set()

ENUM_VALUE_CHOICE_CONFIG = {
    "dynamics": {
        "value_type": "DynamicsValue",
        "enum_type": "DynamicsEnum",
        "other_variant": "otherDynamics",
        "other_xml_name": "other-dynamics",
    },
}

INLINE_CHOICE_CONFIG = {
    "arrow": {
        "branches": [
            {
                "enum_name": "arrowGroup",
                "class_name": "ArrowGroup",
                "is_group": True,
                "children": [
                    {"name": "arrow-direction", "min": 1, "max": 1},
                    {"name": "arrow-style", "min": 0, "max": 1},
                ],
            },
            {
                "enum_name": "circularArrow",
                "class_name": "CircularArrow",
                "is_group": False,
                "element_name": "circular-arrow",
            },
        ],
        "enum_start": 1,
    },
}

CHOICE_ELEMENT_CONFIG = {
    "articulations": {
        "choice_class": "ArticulationsChoice", "is_set": True, "enum_start": 1,
        "choice_from_x": "manual",
        "choice_stream_start": "mx_unused", "choice_stream_end": None,
        "choice_indent_offset": 0, "choice_braces": True,
        "parent_from_x": "simple_loop", "parent_return": "bare",
        "parent_else_iol": "true", "parent_if_iol": True,
    },
    "technical": {
        "choice_class": "TechnicalChoice", "is_set": True, "enum_start": 1,
        "choice_from_x": "unused",
        "choice_stream_start": None, "choice_stream_end": "is_one_line",
        "choice_indent_offset": 0, "choice_braces": True,
        "parent_from_x": "dispatch", "parent_return": "macro",
        "parent_else_iol": "false", "parent_if_iol": False,
    },
    "encoding": {
        "choice_class": "EncodingChoice", "is_set": True, "enum_start": 1,
        "choice_from_x": "macro",
        "choice_stream_start": "endl", "choice_stream_end": "is_one_line",
        "choice_indent_offset": 1, "choice_braces": True,
        "parent_from_x": "simple_loop", "parent_return": "macro",
        "parent_else_iol": None, "parent_if_iol": False,
        "parent_stream_style": "is_first",
        "parent_method_order": "remove_add",
        "parent_no_get": True,
    },
    "percussion": {
        "choice_class": "PercussionChoice", "is_set": False, "enum_start": 1,
        "choice_is_set": True,
        "choice_qualified_ctor": True,
        "choice_from_x": "manual_bad",
        "choice_stream_start": None, "choice_stream_end": "is_one_line",
        "choice_indent_offset": 0, "choice_braces": True,
        # The percussion choice (glass | metal | wood | ...) is selected by the
        # *child* element of <percussion>, so the parent must iterate its
        # children and hand each to PercussionChoice (which dispatches on the
        # child name). "delegate" would pass the <percussion> element itself to
        # the choice, which then rejects 'percussion' as unrecognized.
        "parent_from_x": "child_loop", "parent_return": "macro",
        "extra_children": ["stick-type", "stick-material"],
        "extra_children_after": "stick",
    },
    "measure-style": {
        "choice_class": "MeasureStyleChoice", "is_set": False, "enum_start": 0,
        "choice_from_x": "macro",
        "choice_stream_start": "is_one_line", "choice_stream_end": None,
        "choice_indent_offset": 1, "choice_braces": False,
        "parent_from_x": "for_loop", "parent_return": "macro",
        "parent_stream_iol_first": True,
        "parent_stream_indent_offset": 0,
    },
    "direction-type": {
        "choice_class": "DirectionType", "is_set": True, "enum_start": 1,
        "skip_parent": True,
        "choice_from_x": "unused",
        "choice_stream_start": "is_one_line", "choice_stream_end": "is_one_line_endl",
        "choice_indent_offset": 1, "choice_braces": True,
    },
    "time": {
        "choice_class": "TimeChoice", "is_set": False, "enum_start": 0,
        "bespoke_choice": True,
        "parent_from_x": "time_group",
        "parent_stream_iol_last": True,
        "first_var_name": "TimeSignature",
    },
}


# ---------------------------------------------------------------------------
# Choice Class Generation
# ---------------------------------------------------------------------------


def generate_choice_class_h(choice_class: str, choice_children: list,
                            is_set: bool, enum_start: int,
                            parent_name: str, model: XsdModel) -> str:
    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")

    child_classes = sorted(set(pascal(c.element_name) for c in choice_children))
    for cc in child_classes:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({cc})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({choice_class})\n")

    lines.append(f"inline {choice_class}Ptr make{choice_class}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{choice_class}>();")
    lines.append("}")

    lines.append(f"\nclass {choice_class} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append("    enum class Choice")
    lines.append("    {")
    for i, child in enumerate(choice_children):
        cc_camel = camel(child.element_name)
        val = i + enum_start
        comma = "," if i < len(choice_children) - 1 else ""
        lines.append(f"        {cc_camel} = {val}{comma}")
    lines.append("    };")
    lines.append(f"    {choice_class}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")

    if is_set:
        lines.append(f"\n    /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */")
        lines.append(f"    {choice_class}::Choice getChoice() const;")
        lines.append(f"    void setChoice(const {choice_class}::Choice value);")
    else:
        lines.append(f"    Choice getChoice() const;")
        lines.append(f"    void setChoice(const Choice value);")

    for child in choice_children:
        cc = pascal(child.element_name)
        min_occurs = getattr(child, "min_occurs", 1)
        max_occurs = getattr(child, "max_occurs", 1)
        max_occurs_text = "unbounded" if max_occurs == -1 else str(max_occurs)
        lines.append(f"\n    /* _________ {cc} minOccurs = {min_occurs}, maxOccurs = {max_occurs_text} _________ */")
        if max_occurs != 1:
            lines.append(f"    const {cc}Set &get{cc}Set() const;")
            lines.append(f"    void add{cc}(const {cc}Ptr &value);")
            lines.append(f"    void remove{cc}(const {cc}SetIterConst &value);")
            lines.append(f"    void clear{cc}Set();")
            lines.append(f"    {cc}Ptr get{cc}(const {cc}SetIterConst &setIterator) const;")
        else:
            lines.append(f"    {cc}Ptr get{cc}() const;")
            lines.append(f"    void set{cc}(const {cc}Ptr &value);")

    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append("    Choice myChoice;")
    for child in choice_children:
        cc = pascal(child.element_name)
        if getattr(child, "max_occurs", 1) != 1:
            lines.append(f"    {cc}Set my{cc}Set;")
        else:
            lines.append(f"    {cc}Ptr my{cc};")

    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_choice_class_cpp(choice_class: str, choice_children: list,
                              is_set: bool, enum_start: int,
                              parent_name: str, model: XsdModel,
                              config: dict = None) -> str:
    if config is None:
        config = {}
    choice_from_x = config.get("choice_from_x", "manual")
    stream_start = config.get("choice_stream_start", "mx_unused" if is_set else "is_one_line")
    stream_end = config.get("choice_stream_end")
    indent_offset = config.get("choice_indent_offset", 0 if is_set else 1)
    use_braces = config.get("choice_braces", is_set)
    # When skip_parent is True the choice class is itself the wrapper element
    # (its streamName emits the tag), so each case branch must insert a leading
    # newline before the child's toStream. When skip_parent is False the
    # parent element handles inter-child newlines before calling
    # choice->streamContents, so case branches must NOT emit a leading endl
    # (doing so produces a spurious blank line + misindented child).
    branch_leading_endl = bool(config.get("skip_parent"))

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{choice_class}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    child_includes = sorted(set(
        f'#include "mx/core/elements/{pascal(c.element_name)}.h"'
        for c in choice_children
    ))
    for inc in child_includes:
        lines.append(inc)
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    first_child_camel = camel(choice_children[0].element_name)
    qualify_ctor = config.get("choice_qualified_ctor", not is_set)
    choice_prefix = f"{choice_class}::" if qualify_ctor else ""
    init_parts = [f"myChoice({choice_prefix}Choice::{first_child_camel})"]
    branch_set_children = []
    for child in choice_children:
        cc = pascal(child.element_name)
        if getattr(child, "max_occurs", 1) != 1:
            init_parts.append(f"my{cc}Set()")
            branch_set_children.append(child)
        else:
            init_parts.append(f"my{cc}(make{cc}())")
    _emit_ctor_init(lines, f"{choice_class}::{choice_class}()", init_parts)
    lines.append("{")
    for child in branch_set_children:
        cc = pascal(child.element_name)
        min_occurs = getattr(child, "min_occurs", 1)
        lines.append(f"    while (my{cc}Set.size() < {min_occurs})")
        lines.append("    {")
        lines.append(f"        my{cc}Set.push_back(make{cc}());")
        lines.append("    }")
    lines.append("}\n")

    lines.append(f"bool {choice_class}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")

    lines.append(f"std::ostream &{choice_class}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"std::ostream &{choice_class}::streamName(std::ostream &os) const")
    lines.append("{")
    if is_set:
        lines.append(f'    os << "{parent_name}";')
        lines.append("    return os;")
    else:
        lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"bool {choice_class}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")

    indent_expr = "indentLevel" if indent_offset == 0 else f"indentLevel + {indent_offset}"

    lines.append(f"std::ostream &{choice_class}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    if stream_start == "mx_unused":
        lines.append("    MX_UNUSED(isOneLineOnly);\n")
    elif stream_start == "endl":
        lines.append("    os << std::endl;")
    elif stream_start == "is_one_line":
        lines.append("    isOneLineOnly = false;")

    lines.append("    switch (myChoice)")
    lines.append("    {")
    for child in choice_children:
        cc = pascal(child.element_name)
        cc_camel = camel(child.element_name)
        branch_is_set = getattr(child, "max_occurs", 1) != 1
        if use_braces:
            lines.append(f"    case Choice::{cc_camel}: {{")
            if branch_is_set:
                lines.append(f"        for (auto x : my{cc}Set)")
                lines.append("        {")
                lines.append("            os << std::endl;")
                lines.append(f"            x->toStream(os, {indent_expr});")
                lines.append("        }")
            else:
                if branch_leading_endl:
                    lines.append("        os << std::endl;")
                lines.append(f"        my{cc}->toStream(os, {indent_expr});")
            lines.append("    }")
            lines.append("    break;")
        else:
            lines.append(f"    case Choice::{cc_camel}:")
            if branch_is_set:
                lines.append(f"        for (auto x : my{cc}Set)")
                lines.append("        {")
                lines.append("            os << std::endl;")
                lines.append(f"            x->toStream(os, {indent_expr});")
                lines.append("        }")
            else:
                if branch_leading_endl:
                    lines.append("        os << std::endl;")
                lines.append(f"        my{cc}->toStream(os, {indent_expr});")
            lines.append("        break;")
    lines.append("    default:")
    lines.append("        break;")
    lines.append("    }")
    if stream_end == "is_one_line":
        lines.append("    isOneLineOnly = false;")
    elif stream_end == "is_one_line_endl":
        lines.append("    os << std::endl;")
        lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"{choice_class}::Choice {choice_class}::getChoice() const")
    lines.append("{")
    lines.append("    return myChoice;")
    lines.append("}\n")

    if is_set:
        lines.append(f"void {choice_class}::setChoice(const {choice_class}::Choice value)")
    else:
        lines.append(f"void {choice_class}::setChoice(const {choice_class}::Choice value)")
    lines.append("{")
    lines.append("    myChoice = value;")
    lines.append("}\n")

    for child in choice_children:
        cc = pascal(child.element_name)
        min_occurs = getattr(child, "min_occurs", 1)
        if getattr(child, "max_occurs", 1) != 1:
            lines.append(f"const {cc}Set &{choice_class}::get{cc}Set() const")
            lines.append("{")
            lines.append(f"    return my{cc}Set;")
            lines.append("}\n")
            lines.append(f"void {choice_class}::add{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc}Set.push_back(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {choice_class}::remove{cc}(const {cc}SetIterConst &value)")
            lines.append("{")
            lines.append(f"    if (value != my{cc}Set.cend())")
            lines.append("    {")
            lines.append(f"        if (my{cc}Set.size() > {min_occurs})")
            lines.append("        {")
            lines.append(f"            my{cc}Set.erase(value);")
            lines.append("        }")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {choice_class}::clear{cc}Set()")
            lines.append("{")
            lines.append(f"    my{cc}Set.clear();")
            lines.append(f"    while (my{cc}Set.size() < {min_occurs})")
            lines.append("    {")
            lines.append(f"        my{cc}Set.push_back(make{cc}());")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"{cc}Ptr {choice_class}::get{cc}(const {cc}SetIterConst &setIterator) const")
            lines.append("{")
            lines.append(f"    if (setIterator != my{cc}Set.cend())")
            lines.append("    {")
            lines.append("        return *setIterator;")
            lines.append("    }")
            lines.append(f"    return {cc}Ptr();")
            lines.append("}\n")
        else:
            lines.append(f"{cc}Ptr {choice_class}::get{cc}() const")
            lines.append("{")
            lines.append(f"    return my{cc};")
            lines.append("}\n")
            lines.append(f"void {choice_class}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}\n")

    if choice_from_x == "unused":
        lines.append(f"MX_FROM_XELEMENT_UNUSED({choice_class});")
    elif choice_from_x == "macro":
        lines.append(f"bool {choice_class}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
        lines.append("{")
        for child in choice_children:
            cc = pascal(child.element_name)
            cc_camel = camel(child.element_name)
            lines.append(f'    MX_CHOICE_IF({cc_camel}, "{child.element_name}", {cc});')
        lines.append(f"    MX_BAD_ELEMENT_FAILURE({choice_class});")
        lines.append("}\n")
    elif choice_from_x == "manual_bad":
        lines.append(f"bool {choice_class}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
        lines.append("{")
        for child in choice_children:
            cc = pascal(child.element_name)
            cc_camel = camel(child.element_name)
            lines.append(f'    if (xelement.getName() == "{child.element_name}")')
            lines.append("    {")
            lines.append(f"        myChoice = Choice::{cc_camel};")
            lines.append(f"        return get{cc}()->fromXElement(message, xelement);")
            lines.append("    }\n")
        lines.append(f"    MX_BAD_ELEMENT_FAILURE({choice_class});")
        lines.append("}\n")
    else:
        lines.append(f"bool {choice_class}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
        lines.append("{")
        for child in choice_children:
            cc = pascal(child.element_name)
            cc_camel = camel(child.element_name)
            lines.append(f'    if (xelement.getName() == "{child.element_name}")')
            lines.append("    {")
            lines.append(f"        myChoice = Choice::{cc_camel};")
            lines.append(f"        return get{cc}()->fromXElement(message, xelement);")
            lines.append("    }\n")
        lines.append(f'    message << "{choice_class}: \'" << xelement.getName() << "\' is not allowed" << std::endl;')
        lines.append("    return false;")
        lines.append("}\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Choice Parent Element Generation
# ---------------------------------------------------------------------------


def generate_choice_parent_h(elem_name: str, class_name: str, choice_class: str,
                             is_set: bool, has_attrs: bool,
                             attrs_name: Optional[str], model: XsdModel,
                             config: dict = None) -> str:
    if config is None:
        config = {}
    parent_no_get = config.get("parent_no_get", False)
    lines = [LICENSE, "#pragma once\n"]
    project_includes = ['"mx/core/ElementInterface.h"', '"mx/core/ForwardDeclare.h"']
    if has_attrs and attrs_name:
        if attrs_name in CORE_ROOT_ATTRS:
            project_includes.append(f'"mx/core/{attrs_name}.h"')
        else:
            project_includes.append(f'"mx/core/elements/{attrs_name}.h"')
    for inc in sorted(project_includes):
        lines.append(f"#include {inc}")
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")

    if has_attrs and attrs_name:
        lines.append(f"MX_FORWARD_DECLARE_ATTRIBUTES({attrs_name})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({choice_class})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({class_name})\n")

    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")

    lines.append(f"\nclass {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")

    if has_attrs and attrs_name:
        lines.append(f"    {attrs_name}Ptr getAttributes() const;")
        lines.append(f"    void setAttributes(const {attrs_name}Ptr &value);")

    if is_set:
        lines.append(f"\n    /* _________ {choice_class} minOccurs = 0, maxOccurs = unbounded _________ */")
        lines.append(f"    const {choice_class}Set &get{choice_class}Set() const;")
        lines.append(f"    void add{choice_class}(const {choice_class}Ptr &value);")
        lines.append(f"    void remove{choice_class}(const {choice_class}SetIterConst &value);")
        lines.append(f"    void clear{choice_class}Set();")
        if not parent_no_get:
            lines.append(f"    {choice_class}Ptr get{choice_class}(const {choice_class}SetIterConst &setIterator) const;")
    else:
        lines.append(f"    {choice_class}Ptr get{choice_class}() const;")
        lines.append(f"    void set{choice_class}(const {choice_class}Ptr &value);")

    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    if has_attrs and attrs_name:
        lines.append(f"    {attrs_name}Ptr myAttributes;")
    if is_set:
        lines.append(f"    {choice_class}Set my{choice_class}Set;")
    else:
        lines.append(f"    {choice_class}Ptr myChoice;")

    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_choice_parent_cpp(elem_name: str, class_name: str, choice_class: str,
                               is_set: bool, has_attrs: bool,
                               attrs_name: Optional[str], stream_name: str,
                               model: XsdModel, config: dict = None,
                               choice_children: list = None) -> str:
    if config is None:
        config = {}
    parent_from_x = config.get("parent_from_x", "simple_loop" if is_set else "delegate")
    parent_return = config.get("parent_return", "bare")
    parent_else_iol = config.get("parent_else_iol", "true")
    parent_if_iol = config.get("parent_if_iol", True)
    parent_stream_style = config.get("parent_stream_style", "standard" if is_set else "single")
    parent_method_order = config.get("parent_method_order", "add_remove")
    parent_no_get = config.get("parent_no_get", False)

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')

    if parent_from_x == "dispatch" and choice_children:
        child_includes = sorted(set(
            f'#include "mx/core/elements/{pascal(c.element_name)}.h"'
            for c in choice_children
        ))
        for inc in child_includes:
            lines.append(inc)

    if parent_from_x == "time_group":
        group_class = config.get("group_class", "TimeSignatureGroup")
        single_class = config.get("single_class", "SenzaMisura")
        lines.append(f'#include "mx/core/elements/{single_class}.h"')

    lines.append(f'#include "mx/core/elements/{choice_class}.h"')

    if parent_from_x == "time_group":
        group_class = config.get("group_class", "TimeSignatureGroup")
        lines.append(f'#include "mx/core/elements/{group_class}.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    init_parts = []
    if has_attrs and attrs_name:
        init_parts.append(f"myAttributes(std::make_shared<{attrs_name}>())")
    if is_set:
        init_parts.append(f"my{choice_class}Set()")
    else:
        init_parts.append(f"myChoice(make{choice_class}())")
    _emit_ctor_init(lines, f"{class_name}::{class_name}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    if has_attrs:
        lines.append("    return myAttributes->hasValues();")
    else:
        lines.append("    return false;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    if has_attrs:
        lines.append("    return myAttributes->toStream(os);")
    else:
        lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append(f'    os << "{stream_name}";')
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    if is_set:
        lines.append(f"    return my{choice_class}Set.size() > 0;")
    else:
        lines.append("    return true;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    if is_set:
        if parent_stream_style == "is_first":
            lines.append("    bool isFirst = true;")
            lines.append(f"    for (auto x : my{choice_class}Set)")
            lines.append("    {")
            lines.append("        if (!isFirst)")
            lines.append("        {")
            lines.append("            os << std::endl;")
            lines.append("            isFirst = false;")
            lines.append("        }")
            lines.append("        x->streamContents(os, indentLevel, isOneLineOnly);")
            lines.append("    }")
            lines.append("    if (hasContents())")
            lines.append("    {")
            lines.append("        os << std::endl;")
            lines.append("    }")
            lines.append("    isOneLineOnly = !hasContents();")
        else:
            lines.append("    if (hasContents())")
            lines.append("    {")
            lines.append(f"        for (auto x : my{choice_class}Set)")
            lines.append("        {")
            lines.append("            os << std::endl;")
            lines.append("            x->streamContents(os, indentLevel + 1, isOneLineOnly);")
            lines.append("        }")
            lines.append("        os << std::endl;")
            if parent_if_iol:
                lines.append("        isOneLineOnly = false;")
            lines.append("    }")
            lines.append("    else")
            lines.append("    {")
            if parent_else_iol is not None:
                lines.append(f"        isOneLineOnly = {'true' if parent_else_iol == 'true' else 'false'};")
            lines.append("    }")
    else:
        p_indent_off = config.get("parent_stream_indent_offset", 1)
        p_indent = "indentLevel" if p_indent_off == 0 else f"indentLevel + {p_indent_off}"
        if config.get("parent_stream_iol_first"):
            lines.append("    isOneLineOnly = false;")
            lines.append("    os << std::endl;")
            lines.append(f"    myChoice->streamContents(os, {p_indent}, isOneLineOnly);")
            lines.append("    os << std::endl;")
        elif config.get("parent_stream_iol_last"):
            lines.append("    os << std::endl;")
            lines.append(f"    myChoice->streamContents(os, {p_indent}, isOneLineOnly);")
            lines.append("    os << std::endl;")
            lines.append("    isOneLineOnly = false;")
        else:
            lines.append("    os << std::endl;")
            lines.append(f"    myChoice->streamContents(os, {p_indent}, isOneLineOnly);")
            lines.append("    isOneLineOnly = false;")
            lines.append("    os << std::endl;")
    lines.append("    return os;")
    lines.append("}\n")

    if has_attrs and attrs_name:
        lines.append(f"{attrs_name}Ptr {class_name}::getAttributes() const")
        lines.append("{")
        lines.append("    return myAttributes;")
        lines.append("}\n")
        lines.append(f"void {class_name}::setAttributes(const {attrs_name}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append("        myAttributes = value;")
        lines.append("    }")
        lines.append("}\n")

    if is_set:
        cc = choice_class
        lines.append(f"const {cc}Set &{class_name}::get{cc}Set() const")
        lines.append("{")
        lines.append(f"    return my{cc}Set;")
        lines.append("}\n")

        if parent_method_order == "remove_add":
            lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
            lines.append("{")
            lines.append(f"    if (value != my{cc}Set.cend())")
            lines.append("    {")
            lines.append(f"        my{cc}Set.erase(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc}Set.push_back(value);")
            lines.append("    }")
            lines.append("}\n")
        else:
            lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc}Set.push_back(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
            lines.append("{")
            lines.append(f"    if (value != my{cc}Set.cend())")
            lines.append("    {")
            lines.append(f"        my{cc}Set.erase(value);")
            lines.append("    }")
            lines.append("}\n")

        lines.append(f"void {class_name}::clear{cc}Set()")
        lines.append("{")
        lines.append(f"    my{cc}Set.clear();")
        lines.append("}\n")
        if not parent_no_get:
            lines.append(f"{cc}Ptr {class_name}::get{cc}(const {cc}SetIterConst &setIterator) const")
            lines.append("{")
            lines.append(f"    if (setIterator != my{cc}Set.cend())")
            lines.append("    {")
            lines.append("        return *setIterator;")
            lines.append("    }")
            lines.append(f"    return {cc}Ptr();")
            lines.append("}\n")
    else:
        cc = choice_class
        lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
        lines.append("{")
        lines.append("    return myChoice;")
        lines.append("}\n")
        lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append("        myChoice = value;")
        lines.append("    }")
        lines.append("}\n")

    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    if has_attrs:
        lines.append("    isSuccess &= myAttributes->fromXElement(message, xelement);")

    if is_set and parent_from_x == "dispatch" and choice_children:
        lines.append(f"    ::ezxml::XElementIterator end = xelement.end();\n")
        lines.append("    for (auto it = xelement.begin(); it != end; ++it)")
        lines.append("    {")
        lines.append('        const std::string elementName = it->getName();')
        lines.append(f"        auto choice = make{choice_class}();\n")
        first = True
        for child in choice_children:
            cc = pascal(child.element_name)
            cc_camel = camel(child.element_name)
            prefix = "        if" if first else "        else if"
            lines.append(f'{prefix} (elementName == "{child.element_name}")')
            lines.append("        {")
            lines.append(f"            choice->setChoice({choice_class}::Choice::{cc_camel});")
            lines.append(f"            isSuccess &= choice->get{cc}()->fromXElement(message, *it);")
            lines.append(f"            my{choice_class}Set.push_back(choice);")
            lines.append("            continue;")
            lines.append("        }")
            first = False
        lines.append("        else")
        lines.append("        {")
        lines.append(f'            message << "{class_name}: unexpected element \'" << elementName << "\' encountered" << std::endl;')
        lines.append("            isSuccess = false;")
        lines.append("        }")
        lines.append("    }\n")
        lines.append("    MX_RETURN_IS_SUCCESS;")
    elif is_set:
        lines.append("")
        lines.append("    auto endIter = xelement.end();")
        lines.append("    for (auto it = xelement.begin(); it != endIter; ++it)")
        lines.append("    {")
        lines.append(f"        auto item = make{choice_class}();")
        lines.append("        isSuccess &= item->fromXElement(message, *it);")
        lines.append(f"        my{choice_class}Set.push_back(item);")
        lines.append("    }\n")
        if parent_return == "macro":
            lines.append("    MX_RETURN_IS_SUCCESS;")
        else:
            lines.append("    return isSuccess;")
    elif parent_from_x == "time_group":
        group_class = config.get("group_class", "TimeSignatureGroup")
        single_elem = config.get("single_element", "senza-misura")
        single_camel = config.get("single_camel", "senzaMisura")
        single_class = config.get("single_class", "SenzaMisura")
        group_camel = config.get("group_camel", "timeSignature")
        first_var = config.get("first_var_name", "TimeSignature")
        lines.append(f"    bool isFirst{first_var}Added = false;")
        lines.append("")
        lines.append("    auto endIter = xelement.end();")
        lines.append("")
        lines.append("    for (auto it = xelement.begin(); it != endIter; ++it)")
        lines.append("    {")
        lines.append(f'        if (it->getName() == "{single_elem}")')
        lines.append("        {")
        lines.append(f"            myChoice->setChoice({choice_class}::Choice::{single_camel});")
        lines.append(f"            isSuccess &= myChoice->get{single_class}()->fromXElement(message, *it);")
        lines.append("        }")
        lines.append("        else")
        lines.append("        {")
        lines.append(f"            myChoice->setChoice({choice_class}::Choice::{group_camel});")
        lines.append(f"            auto timeSignature = make{group_class}();")
        lines.append("            importGroup(message, it, endIter, isSuccess, timeSignature);")
        lines.append("")
        lines.append(f"            if (!isFirst{first_var}Added && myChoice->get{group_class}Set().size() == 1)")
        lines.append("            {")
        lines.append(f"                myChoice->add{group_class}(timeSignature);")
        lines.append(f"                myChoice->remove{group_class}(myChoice->get{group_class}Set().cbegin());")
        lines.append(f"                isFirst{first_var}Added = true;")
        lines.append("            }")
        lines.append("            else")
        lines.append("            {")
        lines.append(f"                myChoice->add{group_class}(timeSignature);")
        lines.append(f"                isFirst{first_var}Added = true;")
        lines.append("            }")
        lines.append("        }")
        lines.append("    }")
        lines.append("")
        lines.append("    return isSuccess;")
    elif parent_from_x == "delegate":
        lines.append(f"    isSuccess &= myChoice->fromXElement(message, xelement);")
        lines.append("    MX_RETURN_IS_SUCCESS;")
    else:
        # "child_loop" (and the implicit default): the choice is selected by the
        # child element, so iterate the parent's children and dispatch each to
        # the choice's fromXElement.
        lines.append("    for (auto it = xelement.begin(); it != xelement.end(); ++it)")
        lines.append("    {")
        lines.append("        isSuccess &= myChoice->fromXElement(message, *it);")
        lines.append("    }\n")
        lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Bespoke: Time Element
# ---------------------------------------------------------------------------


TIME_SIGNATURE_GROUP_CHILDREN = [
    XsdChildRef(element_name="beats", min_occurs=1, max_occurs=1),
    XsdChildRef(element_name="beat-type", min_occurs=1, max_occurs=1),
    XsdChildRef(element_name="interchangeable", min_occurs=0, max_occurs=1),
]


def generate_time_choice_h() -> str:
    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    lines.append("MX_FORWARD_DECLARE_ELEMENT(SenzaMisura)")
    lines.append("MX_FORWARD_DECLARE_ELEMENT(TimeSignatureGroup)")
    lines.append("MX_FORWARD_DECLARE_ELEMENT(TimeChoice)\n")
    lines.append("inline TimeChoicePtr makeTimeChoice()")
    lines.append("{")
    lines.append("    return std::make_shared<TimeChoice>();")
    lines.append("}")
    lines.append("\nclass TimeChoice : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append("    enum class Choice")
    lines.append("    {")
    lines.append("        timeSignature = 0,")
    lines.append("        senzaMisura = 1")
    lines.append("    };")
    lines.append("    TimeChoice();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append("")
    lines.append("    Choice getChoice() const;")
    lines.append("    void setChoice(const Choice value);")
    lines.append("")
    lines.append("    /* _________ TimeSignature minOccurs = 1, maxOccurs = unbounded _________ */")
    lines.append("    const TimeSignatureGroupSet &getTimeSignatureGroupSet() const;")
    lines.append("    void addTimeSignatureGroup(const TimeSignatureGroupPtr &value);")
    lines.append("    void removeTimeSignatureGroup(const TimeSignatureGroupSetIterConst &value);")
    lines.append("    void clearTimeSignatureGroupSet();")
    lines.append("    TimeSignatureGroupPtr getTimeSignatureGroup(const TimeSignatureGroupSetIterConst &setIterator) const;")
    lines.append("")
    lines.append("    SenzaMisuraPtr getSenzaMisura() const;")
    lines.append("    void setSenzaMisura(const SenzaMisuraPtr &value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append("    Choice myChoice;")
    lines.append("    TimeSignatureGroupSet myTimeSignatureGroupSet;")
    lines.append("    SenzaMisuraPtr mySenzaMisura;")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_time_choice_cpp() -> str:
    lines = [LICENSE]
    lines.append('#include "mx/core/elements/TimeChoice.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    lines.append('#include "mx/core/elements/BeatType.h"')
    lines.append('#include "mx/core/elements/Beats.h"')
    lines.append('#include "mx/core/elements/Interchangeable.h"')
    lines.append('#include "mx/core/elements/SenzaMisura.h"')
    lines.append('#include "mx/core/elements/TimeSignatureGroup.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")
    lines.append("TimeChoice::TimeChoice() : myChoice(Choice::timeSignature), myTimeSignatureGroupSet(), mySenzaMisura(makeSenzaMisura())")
    lines.append("{")
    lines.append("    myTimeSignatureGroupSet.push_back(makeTimeSignatureGroup());")
    lines.append("}\n")
    lines.append("bool TimeChoice::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")
    lines.append("std::ostream &TimeChoice::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append("std::ostream &TimeChoice::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append("bool TimeChoice::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")
    lines.append("std::ostream &TimeChoice::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    switch (myChoice)")
    lines.append("    {")
    lines.append("    case Choice::timeSignature: {")
    lines.append("        for (auto x : myTimeSignatureGroupSet)")
    lines.append("        {")
    lines.append("            x->streamContents(os, indentLevel, isOneLineOnly);")
    lines.append("        }")
    lines.append("    }")
    lines.append("    break;")
    lines.append("    case Choice::senzaMisura: {")
    lines.append("        mySenzaMisura->toStream(os, indentLevel);")
    lines.append("    }")
    lines.append("    break;")
    lines.append("    default:")
    lines.append("        break;")
    lines.append("    }")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append("TimeChoice::Choice TimeChoice::getChoice() const")
    lines.append("{")
    lines.append("    return myChoice;")
    lines.append("}\n")
    lines.append("void TimeChoice::setChoice(const Choice value)")
    lines.append("{")
    lines.append("    myChoice = value;")
    lines.append("}\n")
    lines.append("const TimeSignatureGroupSet &TimeChoice::getTimeSignatureGroupSet() const")
    lines.append("{")
    lines.append("    return myTimeSignatureGroupSet;")
    lines.append("}\n")
    lines.append("void TimeChoice::removeTimeSignatureGroup(const TimeSignatureGroupSetIterConst &value)")
    lines.append("{")
    lines.append("    if (value != myTimeSignatureGroupSet.cend())")
    lines.append("    {")
    lines.append("        myTimeSignatureGroupSet.erase(value);")
    lines.append("    }")
    lines.append("}\n")
    lines.append("void TimeChoice::addTimeSignatureGroup(const TimeSignatureGroupPtr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append("        myTimeSignatureGroupSet.push_back(value);")
    lines.append("    }")
    lines.append("}\n")
    lines.append("void TimeChoice::clearTimeSignatureGroupSet()")
    lines.append("{")
    lines.append("    myTimeSignatureGroupSet.clear();")
    lines.append("    myTimeSignatureGroupSet.push_back(makeTimeSignatureGroup());")
    lines.append("}\n")
    lines.append("TimeSignatureGroupPtr TimeChoice::getTimeSignatureGroup(const TimeSignatureGroupSetIterConst &setIterator) const")
    lines.append("{")
    lines.append("    if (setIterator != myTimeSignatureGroupSet.cend())")
    lines.append("    {")
    lines.append("        return *setIterator;")
    lines.append("    }")
    lines.append("    return TimeSignatureGroupPtr();")
    lines.append("}\n")
    lines.append("SenzaMisuraPtr TimeChoice::getSenzaMisura() const")
    lines.append("{")
    lines.append("    return mySenzaMisura;")
    lines.append("}\n")
    lines.append("void TimeChoice::setSenzaMisura(const SenzaMisuraPtr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append("        mySenzaMisura = value;")
    lines.append("    }")
    lines.append("}\n")
    lines.append("MX_FROM_XELEMENT_UNUSED(TimeChoice);")
    lines.append("")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Inline Choice Generation
# ---------------------------------------------------------------------------


def generate_inline_choice_h(elem_name: str, class_name: str,
                             config: dict, has_attrs: bool,
                             attrs_name: Optional[str]) -> str:
    branches = config["branches"]
    enum_start = config.get("enum_start", 1)

    lines = [LICENSE, "#pragma once\n"]
    project_includes = ['"mx/core/ElementInterface.h"', '"mx/core/ForwardDeclare.h"']
    if has_attrs and attrs_name:
        if attrs_name in CORE_ROOT_ATTRS:
            project_includes.append(f'"mx/core/{attrs_name}.h"')
        else:
            project_includes.append(f'"mx/core/elements/{attrs_name}.h"')
    for inc in sorted(project_includes):
        lines.append(f"#include {inc}")
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")

    if has_attrs and attrs_name:
        lines.append(f"MX_FORWARD_DECLARE_ATTRIBUTES({attrs_name})")
    for branch in branches:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({branch['class_name']})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({class_name})\n")

    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")

    lines.append(f"\nclass {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append("    enum class Choice")
    lines.append("    {")
    for i, branch in enumerate(branches):
        val = i + enum_start
        comma = "," if i < len(branches) - 1 else ""
        lines.append(f"        {branch['enum_name']} = {val}{comma}")
    lines.append("    };")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")

    if has_attrs and attrs_name:
        lines.append(f"    {attrs_name}Ptr getAttributes() const;")
        lines.append(f"    void setAttributes(const {attrs_name}Ptr &value);")

    lines.append(f"\n    /* _________ Choice _________ */")
    lines.append(f"    {class_name}::Choice getChoice() const;")
    lines.append(f"    void setChoice(const {class_name}::Choice value);")

    for branch in branches:
        bc = branch["class_name"]
        lines.append(f"\n    /* _________ {bc} minOccurs = 1, maxOccurs = 1 _________ */")
        lines.append(f"    {bc}Ptr get{bc}() const;")
        lines.append(f"    void set{bc}(const {bc}Ptr &value);")

    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append("    Choice myChoice;")
    if has_attrs and attrs_name:
        lines.append(f"    {attrs_name}Ptr myAttributes;")
    for branch in branches:
        bc = branch["class_name"]
        lines.append(f"    {bc}Ptr my{bc};")

    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_inline_choice_cpp(elem_name: str, class_name: str,
                               config: dict, has_attrs: bool,
                               attrs_name: Optional[str],
                               stream_name: str) -> str:
    branches = config["branches"]

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    branch_includes = sorted(set(
        f'#include "mx/core/elements/{b["class_name"]}.h"'
        for b in branches
    ))
    for inc in branch_includes:
        lines.append(inc)
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    first_branch = branches[0]
    init_parts = [f"myChoice(Choice::{first_branch['enum_name']})"]
    if has_attrs and attrs_name:
        init_parts.append(f"myAttributes(std::make_shared<{attrs_name}>())")
    for branch in branches:
        bc = branch["class_name"]
        init_parts.append(f"my{bc}(make{bc}())")

    _emit_ctor_init(lines, f"{class_name}::{class_name}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    if has_attrs:
        lines.append("    return myAttributes->hasValues();")
    else:
        lines.append("    return false;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    if has_attrs:
        lines.append("    return myAttributes->toStream(os);")
    else:
        lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append(f'    os << "{stream_name}";')
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    switch (myChoice)")
    lines.append("    {")
    for branch in branches:
        bc = branch["class_name"]
        lines.append(f"    case Choice::{branch['enum_name']}: {{")
        lines.append("        os << std::endl;")
        if branch.get("is_group"):
            lines.append(f"        my{bc}->streamContents(os, indentLevel + 1, isOneLineOnly);")
        else:
            lines.append(f"        my{bc}->toStream(os, indentLevel + 1);")
        lines.append("        os << std::endl;")
        lines.append("        isOneLineOnly = false;")
        lines.append("    }")
        lines.append("    break;")
    lines.append("    default:")
    lines.append("        break;")
    lines.append("    }")
    lines.append("    return os;")
    lines.append("}\n")

    if has_attrs and attrs_name:
        lines.append(f"{attrs_name}Ptr {class_name}::getAttributes() const")
        lines.append("{")
        lines.append("    return myAttributes;")
        lines.append("}\n")
        lines.append(f"void {class_name}::setAttributes(const {attrs_name}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append("        myAttributes = value;")
        lines.append("    }")
        lines.append("}\n")

    lines.append(f"{class_name}::Choice {class_name}::getChoice() const")
    lines.append("{")
    lines.append("    return myChoice;")
    lines.append("}\n")
    lines.append(f"void {class_name}::setChoice(const {class_name}::Choice value)")
    lines.append("{")
    lines.append("    myChoice = value;")
    lines.append("}\n")

    for branch in branches:
        bc = branch["class_name"]
        lines.append(f"{bc}Ptr {class_name}::get{bc}() const")
        lines.append("{")
        lines.append(f"    return my{bc};")
        lines.append("}\n")
        lines.append(f"void {class_name}::set{bc}(const {bc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{bc} = value;")
        lines.append("    }")
        lines.append("}\n")

    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    if has_attrs:
        lines.append("    isSuccess &= myAttributes->fromXElement(message, xelement);\n")

    lines.append("    auto endIter = xelement.end();")
    lines.append("    for (auto it = xelement.begin(); it != endIter; ++it)")
    lines.append("    {")

    element_branches = [b for b in branches if not b.get("is_group")]
    group_branches = [b for b in branches if b.get("is_group")]

    for eb in element_branches:
        bc = eb["class_name"]
        lines.append(f'        if (it->getName() == "{eb["element_name"]}")')
        lines.append("        {")
        lines.append(f"            myChoice = Choice::{eb['enum_name']};")
        lines.append(f"            isSuccess &= my{bc}->fromXElement(message, *it);")
        # Once a child matches an element branch, the choice is decided; the
        # group branch below must not also run (it parses the parent as the
        # group and would overwrite the choice, then fail on the missing
        # required group member). Move on to the next child.
        lines.append("            continue;")
        lines.append("        }")

    if group_branches:
        gb = group_branches[0]
        bc = gb["class_name"]
        lines.append(f"        myChoice = Choice::{gb['enum_name']};")
        lines.append(f"        isSuccess = my{bc}->fromXElement(message, xelement);")

    lines.append("    }\n")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_enum_value_choice_h(elem_name: str, class_name: str,
                                  value_type: str, has_attrs: bool,
                                  attrs_name: Optional[str]) -> str:
    lines = [LICENSE, "#pragma once\n"]
    project_includes = ['"mx/core/ElementInterface.h"', '"mx/core/Enums.h"',
                        '"mx/core/ForwardDeclare.h"']
    if has_attrs and attrs_name:
        project_includes.append(f'"mx/core/elements/{attrs_name}.h"')
    for inc in sorted(project_includes):
        lines.append(f"#include {inc}")
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    if has_attrs and attrs_name:
        lines.append(f"MX_FORWARD_DECLARE_ATTRIBUTES({attrs_name})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({class_name})\n")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append(f"\ninline {class_name}Ptr make{class_name}(const {value_type} &value)")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>(value);")
    lines.append("}")
    lines.append(f"\ninline {class_name}Ptr make{class_name}({value_type} &&value)")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>(std::move(value));")
    lines.append("}")
    lines.append(f"\nclass {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}(const {value_type} &value);")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    if has_attrs and attrs_name:
        lines.append(f"    {attrs_name}Ptr getAttributes() const;")
        lines.append(f"    void setAttributes(const {attrs_name} &attributes);")
    lines.append(f"    {value_type} getValue() const;")
    lines.append(f"    void setValue(const {value_type} &value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append(f"    {value_type} myValue;")
    if has_attrs and attrs_name:
        lines.append(f"    {attrs_name}Ptr myAttributes;")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_enum_value_choice_cpp(elem_name: str, class_name: str,
                                    value_type: str, enum_type: str,
                                    other_variant: str, other_xml_name: str,
                                    has_attrs: bool,
                                    attrs_name: Optional[str]) -> str:
    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    lines.append("#include <iostream>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{")

    init_value = "myValue(value)"
    init_default = "myValue()"
    if has_attrs and attrs_name:
        attrs_init = f"myAttributes(std::make_shared<{attrs_name}>())"
        init_value += f", {attrs_init}"
        init_default += f", {attrs_init}"

    lines.append(f"{class_name}::{class_name}(const {value_type} &value) : {init_value}")
    lines.append("{")
    lines.append("}\n")

    lines.append(f"{class_name}::{class_name}() : {init_default}")
    lines.append("{")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append(f'    return os << "{elem_name}";')
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    isOneLineOnly = false;")
    lines.append("    os << std::endl;")
    lines.append(f"    if (myValue.getValue() == {enum_type}::{other_variant})")
    lines.append("    {")
    lines.append('        indent(os, indentLevel + 1);')
    lines.append('        os << "<";')
    lines.append("        core::toStream(os, myValue.getValue());")
    lines.append('        os << ">";')
    lines.append("        os << myValue;")
    lines.append('        os << "</";')
    lines.append("        core::toStream(os, myValue.getValue());")
    lines.append('        os << ">";')
    lines.append("    }")
    lines.append("    else")
    lines.append("    {")
    lines.append('        indent(os, indentLevel + 1);')
    lines.append('        os << "<";')
    lines.append("        core::toStream(os, myValue.getValue());")
    lines.append('        os << "/>";')
    lines.append("    }")
    lines.append("    os << std::endl;")
    lines.append("    return os;")
    lines.append("}\n")

    if has_attrs and attrs_name:
        lines.append(f"{attrs_name}Ptr {class_name}::getAttributes() const")
        lines.append("{")
        lines.append("    return myAttributes;")
        lines.append("}\n")

    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    if has_attrs and attrs_name:
        lines.append("    return myAttributes->hasValues();")
    else:
        lines.append("    return false;")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    if has_attrs and attrs_name:
        lines.append("    return myAttributes->toStream(os);")
    else:
        lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"{value_type} {class_name}::getValue() const")
    lines.append("{")
    lines.append("    return myValue;")
    lines.append("}\n")

    lines.append(f"void {class_name}::setValue(const {value_type} &value)")
    lines.append("{")
    lines.append("    myValue = value;")
    lines.append("}\n")

    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    if has_attrs and attrs_name:
        lines.append("    isSuccess &= myAttributes->fromXElement(message, xelement);")
        lines.append("")
    lines.append("    auto b = xelement.begin();")
    lines.append("    auto e = xelement.end();")
    lines.append("")
    lines.append("    if (b != e)")
    lines.append("    {")
    lines.append(f'        if (b->getName() == "{other_xml_name}")')
    lines.append("        {")
    lines.append("            myValue.setValue(b->getValue());")
    lines.append("            MX_RETURN_IS_SUCCESS;")
    lines.append("        }")
    lines.append("        else")
    lines.append("        {")
    lines.append("            myValue.setValue(b->getName());")
    lines.append("            MX_RETURN_IS_SUCCESS;")
    lines.append("        }")
    lines.append("    }")
    lines.append("")
    lines.append(f'    message << "{class_name}: parsing failed" << std::endl;')
    lines.append("    return false;")
    lines.append("}\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Tree-Based Generation (for elements with nested choice/sequence structure)
# ---------------------------------------------------------------------------


@dataclass
class TreeChoiceBranch:
    enum_name: str
    class_name: str
    is_set: bool
    is_group: bool
    group_name: str = ""
    xml_name: str = ""
    is_container: bool = False
    # XML element names that trigger this container branch (computed from first
    # child of the container sequence). Empty for non-container branches.
    trigger_names: list = field(default_factory=list)


@dataclass
class ContainerMember:
    class_name: str
    var_name: str
    is_set: bool = False
    is_optional: bool = False
    use_stream_contents: bool = False
    element_name: str = ""


@dataclass
class InlineChoice:
    choice_class: str = ""
    branches: list = field(default_factory=list)
    is_set: bool = False
    is_optional: bool = False


@dataclass
class TreeContainer:
    class_name: str
    members: list = field(default_factory=list)


@dataclass
class TreeGenPlan:
    choice_class: str = ""
    choice_branches: list = field(default_factory=list)
    choice_is_set: bool = False
    choice_is_optional: bool = False
    groups_to_generate: list = field(default_factory=list)
    leading_groups: list = field(default_factory=list)
    leading_children: list = field(default_factory=list)
    trailing_children: list = field(default_factory=list)
    containers_to_generate: list = field(default_factory=list)
    nested_choices_to_generate: list = field(default_factory=list)
    optional_groups_to_generate: list = field(default_factory=list)
    inline_choices: list = field(default_factory=list)


def _tree_has_nested_structure(content_tree) -> bool:
    if isinstance(content_tree, SequenceNode):
        for child in content_tree.children:
            if isinstance(child, ChoiceNode):
                return True
    elif isinstance(content_tree, ChoiceNode):
        return True
    return False


def _get_tree_config(parent_name: str) -> dict:
    return TREE_ELEMENT_CONFIG.get(parent_name, {})


def _container_trigger_names(seq_node, model: "XsdModel") -> list:
    """Return the XML element names that can appear first in seq_node.

    Used to identify which container branch to activate when iterating the
    parent's child elements. Inspects the first child of seq_node:
      - ElementRefNode: returns [element_name]
      - GroupRefNode: returns the element_names of the group's children
    """
    if not seq_node.children:
        return []
    first = seq_node.children[0]
    if isinstance(first, ElementRefNode):
        return [first.element_name]
    if isinstance(first, GroupRefNode):
        group_children = model.groups.get(first.group_name, [])
        names = []
        for gc in group_children:
            if hasattr(gc, "element_name") and gc.element_name:
                names.append(gc.element_name)
        return names
    return []


def _derive_container_name(seq_node, parent_name: str, branch_idx: int) -> str:
    config = _get_tree_config(parent_name)
    container_names = config.get("container_names", {})
    if branch_idx in container_names:
        return container_names[branch_idx]

    first = seq_node.children[0] if seq_node.children else None
    if isinstance(first, GroupRefNode):
        return pascal(first.group_name) + "Sequence"
    elif isinstance(first, ElementRefNode):
        return pascal(first.element_name) + "Sequence"
    return pascal(parent_name) + f"Sequence{branch_idx + 1}"


def _derive_nested_choice_name(choice_node) -> str:
    parts = []
    for branch in choice_node.branches:
        if isinstance(branch, ElementRefNode):
            parts.append(pascal(branch.element_name))
        elif isinstance(branch, GroupRefNode):
            parts.append(pascal(branch.group_name))
        elif isinstance(branch, SequenceNode) and branch.children:
            first = branch.children[0]
            if isinstance(first, ElementRefNode):
                parts.append(pascal(first.element_name))
            elif isinstance(first, GroupRefNode):
                parts.append(pascal(first.group_name))
    return "Or".join(parts) + "Choice" if parts else "NestedChoice"


def _derive_optional_group_name(seq_node) -> str:
    first = seq_node.children[0] if seq_node.children else None
    if isinstance(first, ElementRefNode):
        return pascal(first.element_name) + "Group"
    elif isinstance(first, GroupRefNode):
        return pascal(first.group_name) + "Group"
    return "OptionalGroup"


def _analyze_sequence_members(seq_node, model: XsdModel, plan: TreeGenPlan,
                              parent_name: str) -> list:
    members = []
    for child in seq_node.children:
        if isinstance(child, ElementRefNode):
            cls = pascal(child.element_name)
            members.append(ContainerMember(
                class_name=cls,
                var_name=camel(child.element_name),
                is_set=child.max_occurs != 1,
                is_optional=child.min_occurs == 0,
                use_stream_contents=False,
                element_name=child.element_name,
            ))
        elif isinstance(child, GroupRefNode):
            if child.group_name in GENERATE_GROUPS:
                cls = pascal(child.group_name) + "Group"
                vname = camel(child.group_name) + "Group"
            else:
                cls = pascal(child.group_name)
                vname = camel(child.group_name)
                group_children = model.groups.get(child.group_name, [])
                plan.groups_to_generate.append((cls, group_children))
            members.append(ContainerMember(
                class_name=cls,
                var_name=vname,
                is_set=child.max_occurs != 1,
                use_stream_contents=True,
            ))
        elif isinstance(child, ChoiceNode):
            nested_name = _derive_nested_choice_name(child)
            nested_branches = []
            for nb in child.branches:
                if isinstance(nb, ElementRefNode):
                    nb_cls = pascal(nb.element_name)
                    nested_branches.append(TreeChoiceBranch(
                        enum_name=camel(nb.element_name),
                        class_name=nb_cls,
                        is_set=nb.max_occurs != 1,
                        is_group=False,
                        xml_name=nb.element_name,
                    ))
                elif isinstance(nb, GroupRefNode):
                    if nb.group_name in GENERATE_GROUPS:
                        nb_cls = pascal(nb.group_name) + "Group"
                        nb_enum = camel(nb.group_name) + "Group"
                    else:
                        nb_cls = pascal(nb.group_name)
                        nb_enum = camel(nb.group_name)
                        group_children = model.groups.get(nb.group_name, [])
                        plan.groups_to_generate.append((nb_cls, group_children))
                    nested_branches.append(TreeChoiceBranch(
                        enum_name=nb_enum,
                        class_name=nb_cls,
                        is_set=nb.max_occurs != 1,
                        is_group=True,
                        group_name=nb.group_name,
                    ))
            plan.nested_choices_to_generate.append(
                (nested_name, nested_branches, parent_name))
            members.append(ContainerMember(
                class_name=nested_name,
                var_name=nested_name[0].lower() + nested_name[1:],
                use_stream_contents=True,
            ))
        elif isinstance(child, SequenceNode) and child.min_occurs == 0:
            group_name = _derive_optional_group_name(child)
            group_members = []
            for gc in child.children:
                if isinstance(gc, ElementRefNode):
                    gc_cls = pascal(gc.element_name)
                    group_members.append(XsdChildRef(
                        element_name=gc.element_name,
                        min_occurs=1,
                        max_occurs=1,
                    ))
            plan.optional_groups_to_generate.append(
                (group_name, group_members))
            members.append(ContainerMember(
                class_name=group_name,
                var_name=group_name[0].lower() + group_name[1:],
                is_optional=True,
                use_stream_contents=True,
            ))
    return members


def analyze_tree(elem_name: str, content_tree, model: XsdModel) -> Optional[TreeGenPlan]:
    if not _tree_has_nested_structure(content_tree):
        return None

    plan = TreeGenPlan()
    tree_config = _get_tree_config(elem_name)

    if isinstance(content_tree, SequenceNode):
        root_unbounded = content_tree.max_occurs != 1
        seen_choice = False
        choice_index = 0
        inline_choice_configs = tree_config.get("inline_choices", [])
        for child in content_tree.children:
            if isinstance(child, ChoiceNode):
                if inline_choice_configs and choice_index < len(inline_choice_configs):
                    cc_name = inline_choice_configs[choice_index]["choice_class"]
                elif choice_index == 0:
                    cc_name = tree_config.get("choice_class", pascal(elem_name) + "Choice")
                else:
                    cc_name = pascal(elem_name) + f"Choice{choice_index + 1}"
                is_set = (child.max_occurs != 1) or root_unbounded
                is_optional = (
                    child.min_occurs == 0 and child.max_occurs == 1 and not root_unbounded
                )
                branches = []
                for branch in child.branches:
                    if isinstance(branch, GroupRefNode):
                        cls = pascal(branch.group_name)
                        branches.append(TreeChoiceBranch(
                            enum_name=camel(branch.group_name),
                            class_name=cls,
                            is_set=branch.max_occurs != 1,
                            is_group=True,
                            group_name=branch.group_name,
                        ))
                        group_children = model.groups.get(branch.group_name, [])
                        plan.groups_to_generate.append((cls, group_children))
                    elif isinstance(branch, ElementRefNode):
                        cls = pascal(branch.element_name)
                        branches.append(TreeChoiceBranch(
                            enum_name=camel(branch.element_name),
                            class_name=cls,
                            is_set=branch.max_occurs != 1,
                            is_group=False,
                            xml_name=branch.element_name,
                        ))
                plan.inline_choices.append(InlineChoice(
                    choice_class=cc_name, branches=branches,
                    is_set=is_set, is_optional=is_optional,
                ))
                if choice_index == 0:
                    plan.choice_class = cc_name
                    plan.choice_is_set = is_set
                    plan.choice_is_optional = is_optional
                    plan.choice_branches = branches
                choice_index += 1
                seen_choice = True
            elif isinstance(child, GroupRefNode):
                if child.group_name in GENERATE_GROUPS:
                    cls = pascal(child.group_name) + "Group"
                else:
                    cls = pascal(child.group_name)
                    group_children = model.groups.get(child.group_name, [])
                    plan.groups_to_generate.append((cls, group_children))
                plan.leading_groups.append((cls, child))
            elif isinstance(child, ElementRefNode):
                ref = XsdChildRef(
                    element_name=child.element_name,
                    min_occurs=child.min_occurs,
                    max_occurs=child.max_occurs,
                )
                if seen_choice:
                    plan.trailing_children.append(ref)
                else:
                    plan.leading_children.append(ref)

    elif isinstance(content_tree, ChoiceNode):
        plan.choice_class = tree_config.get("choice_class", pascal(elem_name) + "Choice")
        for i, branch in enumerate(content_tree.branches):
            if isinstance(branch, SequenceNode) and len(branch.children) > 1:
                container_name = _derive_container_name(
                    branch, elem_name, i)
                members = _analyze_sequence_members(
                    branch, model, plan, elem_name)
                plan.containers_to_generate.append(
                    TreeContainer(class_name=container_name, members=members))
                branch_enum_names = tree_config.get("branch_enum_names", {})
                enum_name = branch_enum_names.get(
                    i, camel(container_name.replace("Sequence", "")))
                trigger_names = _container_trigger_names(branch, model)
                plan.choice_branches.append(TreeChoiceBranch(
                    enum_name=enum_name,
                    class_name=container_name,
                    is_set=branch.max_occurs != 1,
                    is_group=False,
                    is_container=True,
                    trigger_names=trigger_names,
                ))
            elif isinstance(branch, SequenceNode) and len(branch.children) == 1:
                child = branch.children[0]
                if isinstance(child, ElementRefNode):
                    cls = pascal(child.element_name)
                    plan.choice_branches.append(TreeChoiceBranch(
                        enum_name=camel(child.element_name),
                        class_name=cls,
                        is_set=child.max_occurs != 1,
                        is_group=False,
                    ))
                elif isinstance(child, GroupRefNode):
                    if child.group_name in GENERATE_GROUPS:
                        cls = pascal(child.group_name) + "Group"
                    else:
                        cls = pascal(child.group_name)
                    plan.choice_branches.append(TreeChoiceBranch(
                        enum_name=camel(child.group_name),
                        class_name=cls,
                        is_set=child.max_occurs != 1,
                        is_group=True,
                        group_name=child.group_name,
                    ))
            elif isinstance(branch, ElementRefNode):
                cls = pascal(branch.element_name)
                plan.choice_branches.append(TreeChoiceBranch(
                    enum_name=camel(branch.element_name),
                    class_name=cls,
                    is_set=branch.max_occurs != 1,
                    is_group=False,
                ))
            elif isinstance(branch, GroupRefNode):
                if branch.group_name in GENERATE_GROUPS:
                    cls = pascal(branch.group_name) + "Group"
                else:
                    cls = pascal(branch.group_name)
                plan.choice_branches.append(TreeChoiceBranch(
                    enum_name=camel(branch.group_name),
                    class_name=cls,
                    is_set=branch.max_occurs != 1,
                    is_group=True,
                    group_name=branch.group_name,
                ))

    return plan if plan.choice_class else None


def generate_tree_group_h(class_name: str, children: list, model: XsdModel) -> str:
    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")

    child_classes = [child_class_name(c) for c in children]
    for cc in sorted(set(child_classes)):
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({cc})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({class_name})\n")

    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")

    lines.append(f"\nclass {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")

    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f"    const {cc}Set &get{cc}Set() const;")
            lines.append(f"    void add{cc}(const {cc}Ptr &value);")
            lines.append(f"    void remove{cc}(const {cc}SetIterConst &value);")
            lines.append(f"    void clear{cc}Set();")
        elif child.min_occurs == 0:
            lines.append(f"    {cc}Ptr get{cc}() const;")
            lines.append(f"    void set{cc}(const {cc}Ptr &value);")
            lines.append(f"    bool getHas{cc}() const;")
            lines.append(f"    void setHas{cc}(const bool value);")
        else:
            lines.append(f"    {cc}Ptr get{cc}() const;")
            lines.append(f"    void set{cc}(const {cc}Ptr &value);")

    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f"    {cc}Set my{cc}Set;")
        else:
            lines.append(f"    {cc}Ptr my{cc};")
            if child.min_occurs == 0:
                lines.append(f"    bool myHas{cc};")

    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_tree_group_cpp(class_name: str, children: list, model: XsdModel) -> str:
    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    child_includes = sorted(set(
        f'#include "mx/core/elements/{child_class_name(c)}.h"'
        for c in children
    ))
    for inc in child_includes:
        lines.append(inc)
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    init_parts = []
    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            init_parts.append(f"my{cc}Set()")
        else:
            init_parts.append(f"my{cc}(make{cc}())")
            if child.min_occurs == 0:
                init_parts.append(f"myHas{cc}(false)")

    _emit_ctor_init(lines, f"{class_name}::{class_name}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    parts = []
    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            parts.append(f"my{cc}Set.size() > 0")
        elif child.min_occurs == 0:
            parts.append(f"myHas{cc}")
        else:
            parts.append("true")
    if any("true" == p for p in parts):
        lines.append("    return true;")
    elif parts:
        lines.append(f"    return {' || '.join(parts)};")
    else:
        lines.append("    return false;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    isOneLineOnly = false;")
    lines.append("    bool isFirst = true;")
    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f"    for (auto x : my{cc}Set)")
            lines.append("    {")
            lines.append("        if (!isFirst)")
            lines.append("            os << std::endl;")
            lines.append("        x->toStream(os, indentLevel);")
            lines.append("        isFirst = false;")
            lines.append("    }")
        elif child.min_occurs == 0:
            lines.append(f"    if (myHas{cc})")
            lines.append("    {")
            lines.append("        if (!isFirst)")
            lines.append("            os << std::endl;")
            lines.append(f"        my{cc}->toStream(os, indentLevel);")
            lines.append("        isFirst = false;")
            lines.append("    }")
        else:
            lines.append("    if (!isFirst)")
            lines.append("        os << std::endl;")
            lines.append(f"    my{cc}->toStream(os, indentLevel);")
            lines.append("    isFirst = false;")
    lines.append("    return os;")
    lines.append("}\n")

    for child in children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f"const {cc}Set &{class_name}::get{cc}Set() const")
            lines.append("{")
            lines.append(f"    return my{cc}Set;")
            lines.append("}\n")
            lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc}Set.push_back(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
            lines.append("{")
            lines.append(f"    if (value != my{cc}Set.cend())")
            lines.append("    {")
            lines.append(f"        my{cc}Set.erase(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {class_name}::clear{cc}Set()")
            lines.append("{")
            lines.append(f"    my{cc}Set.clear();")
            lines.append("}\n")
        elif child.min_occurs == 0:
            lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
            lines.append("{")
            lines.append(f"    return my{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"bool {class_name}::getHas{cc}() const")
            lines.append("{")
            lines.append(f"    return myHas{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::setHas{cc}(const bool value)")
            lines.append("{")
            lines.append(f"    myHas{cc} = value;")
            lines.append("}\n")
        else:
            lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
            lines.append("{")
            lines.append(f"    return my{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}\n")

    lines.append(f"MX_FROM_XELEMENT_UNUSED({class_name});\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_container_h(container: TreeContainer) -> str:
    cn = container.class_name
    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")

    fwd = []
    for m in container.members:
        fwd.append(m.class_name)
    for f in sorted(set(fwd)):
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({f})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({cn})\n")

    lines.append(f"inline {cn}Ptr make{cn}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{cn}>();")
    lines.append("}")

    lines.append(f"\nclass {cn} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {cn}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")

    for m in container.members:
        cc = m.class_name
        if m.is_set:
            lines.append(f"\n    /* _________ {cc} minOccurs = 1, maxOccurs = unbounded _________ */")
            lines.append(f"    const {cc}Set &get{cc}Set() const;")
            lines.append(f"    void add{cc}(const {cc}Ptr &value);")
            lines.append(f"    void remove{cc}(const {cc}SetIterConst &setIterator);")
            lines.append(f"    void clear{cc}Set();")
        elif m.is_optional:
            lines.append(f"\n    /* _________ {cc} minOccurs = 0, maxOccurs = 1 _________ */")
            lines.append(f"    {cc}Ptr get{cc}() const;")
            lines.append(f"    void set{cc}(const {cc}Ptr &value);")
            lines.append(f"    bool getHas{cc}() const;")
            lines.append(f"    void setHas{cc}(const bool value);")
        else:
            lines.append(f"\n    /* _________ {cc} minOccurs = 1, maxOccurs = 1 _________ */")
            lines.append(f"    {cc}Ptr get{cc}() const;")
            lines.append(f"    void set{cc}(const {cc}Ptr &value);")

    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    for m in container.members:
        cc = m.class_name
        if m.is_set:
            lines.append(f"    {cc}Set my{cc}Set;")
        else:
            lines.append(f"    {cc}Ptr my{cc};")
            if m.is_optional:
                lines.append(f"    bool myHas{cc};")

    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_container_cpp(container: TreeContainer) -> str:
    cn = container.class_name
    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{cn}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    child_includes = sorted(set(
        f'#include "mx/core/elements/{m.class_name}.h"'
        for m in container.members
    ))
    for inc in child_includes:
        lines.append(inc)
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    init_parts = []
    for m in container.members:
        cc = m.class_name
        if m.is_set:
            init_parts.append(f"my{cc}Set()")
        else:
            init_parts.append(f"my{cc}(make{cc}())")
            if m.is_optional:
                init_parts.append(f"myHas{cc}(false)")

    _emit_ctor_init(lines, f"{cn}::{cn}()", init_parts)
    lines.append("{")
    set_inits = [m for m in container.members if m.is_set]
    for m in set_inits:
        cc = m.class_name
        lines.append(f"    my{cc}Set.push_back(make{cc}());")
    lines.append("}\n")

    lines.append(f"bool {cn}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")

    lines.append(f"std::ostream &{cn}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"std::ostream &{cn}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"bool {cn}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")

    lines.append(f"std::ostream &{cn}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    first = True
    for m in container.members:
        cc = m.class_name
        if m.is_set:
            lines.append(f"    for (auto it = my{cc}Set.cbegin(); it != my{cc}Set.cend(); ++it)")
            lines.append("    {")
            lines.append(f"        if (it != my{cc}Set.cbegin())")
            lines.append("        {")
            lines.append("            os << std::endl;")
            lines.append("        }")
            lines.append("        (*it)->toStream(os, indentLevel);")
            lines.append("    }")
            first = False
        elif m.is_optional:
            lines.append(f"    if (myHas{cc})")
            lines.append("    {")
            lines.append("        os << std::endl;")
            if m.use_stream_contents:
                lines.append(f"        my{cc}->streamContents(os, indentLevel, isOneLineOnly);")
            else:
                lines.append(f"        my{cc}->toStream(os, indentLevel);")
            lines.append("    }")
            first = False
        elif m.use_stream_contents:
            if not first:
                lines.append("    os << std::endl;")
            lines.append(f"    my{cc}->streamContents(os, indentLevel, isOneLineOnly);")
            first = False
        else:
            if not first:
                lines.append("    os << std::endl;")
            lines.append(f"    my{cc}->toStream(os, indentLevel);")
            first = False
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}\n")

    for m in container.members:
        cc = m.class_name
        if m.is_set:
            lines.append(f"const {cc}Set &{cn}::get{cc}Set() const")
            lines.append("{")
            lines.append(f"    return my{cc}Set;")
            lines.append("}\n")
            lines.append(f"void {cn}::add{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc}Set.push_back(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {cn}::remove{cc}(const {cc}SetIterConst &setIterator)")
            lines.append("{")
            lines.append(f"    if (setIterator != my{cc}Set.cend())")
            lines.append("    {")
            lines.append(f"        if (my{cc}Set.size() > 1)")
            lines.append("        {")
            lines.append(f"            my{cc}Set.erase(setIterator);")
            lines.append("        }")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {cn}::clear{cc}Set()")
            lines.append("{")
            lines.append(f"    my{cc}Set.clear();")
            lines.append(f"    my{cc}Set.push_back(make{cc}());")
            lines.append("}\n")
        elif m.is_optional:
            lines.append(f"{cc}Ptr {cn}::get{cc}() const")
            lines.append("{")
            lines.append(f"    return my{cc};")
            lines.append("}\n")
            lines.append(f"void {cn}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"bool {cn}::getHas{cc}() const")
            lines.append("{")
            lines.append(f"    return myHas{cc};")
            lines.append("}\n")
            lines.append(f"void {cn}::setHas{cc}(const bool value)")
            lines.append("{")
            lines.append(f"    myHas{cc} = value;")
            lines.append("}\n")
        else:
            lines.append(f"{cc}Ptr {cn}::get{cc}() const")
            lines.append("{")
            lines.append(f"    return my{cc};")
            lines.append("}\n")
            lines.append(f"void {cn}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}\n")

    lines.append(f"MX_FROM_XELEMENT_UNUSED({cn});\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_tree_choice_h(choice_class: str, branches: list,
                           parent_name: str) -> str:
    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")

    for b in branches:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({b.class_name})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({choice_class})\n")

    lines.append(f"inline {choice_class}Ptr make{choice_class}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{choice_class}>();")
    lines.append("}")

    lines.append(f"\nclass {choice_class} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append("    enum class Choice")
    lines.append("    {")
    for i, b in enumerate(branches):
        comma = "," if i < len(branches) - 1 else ""
        lines.append(f"        {b.enum_name} = {i}{comma}")
    lines.append("    };")
    lines.append(f"    {choice_class}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append(f"    Choice getChoice() const;")
    lines.append(f"    void setChoice(const Choice value);")

    for b in branches:
        if b.is_set:
            lines.append(f"\n    const {b.class_name}Set &get{b.class_name}Set() const;")
            lines.append(f"    void remove{b.class_name}(const {b.class_name}SetIterConst &value);")
            lines.append(f"    void add{b.class_name}(const {b.class_name}Ptr &value);")
            lines.append(f"    void clear{b.class_name}Set();")
        else:
            lines.append(f"    {b.class_name}Ptr get{b.class_name}() const;")
            lines.append(f"    void set{b.class_name}(const {b.class_name}Ptr &value);")

    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append("    Choice myChoice;")
    for b in branches:
        if b.is_set:
            lines.append(f"    {b.class_name}Set my{b.class_name}Set;")
        else:
            lines.append(f"    {b.class_name}Ptr my{b.class_name};")

    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_tree_choice_cpp(choice_class: str, branches: list,
                             parent_name: str) -> str:
    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{choice_class}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for b in branches:
        lines.append(f'#include "mx/core/elements/{b.class_name}.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    first_enum = branches[0].enum_name
    init_parts = [f"myChoice(Choice::{first_enum})"]
    for b in branches:
        if b.is_set:
            init_parts.append(f"my{b.class_name}Set()")
        else:
            init_parts.append(f"my{b.class_name}(make{b.class_name}())")
    _emit_ctor_init(lines, f"{choice_class}::{choice_class}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {choice_class}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")

    lines.append(f"std::ostream &{choice_class}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"std::ostream &{choice_class}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"bool {choice_class}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")

    lines.append(f"std::ostream &{choice_class}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    for b in branches:
        lines.append(f"    if (myChoice == Choice::{b.enum_name})")
        lines.append("    {")
        if b.is_set:
            lines.append(f"        for (auto it = my{b.class_name}Set.cbegin(); it != my{b.class_name}Set.cend(); ++it)")
            lines.append("        {")
            lines.append(f"            if (it != my{b.class_name}Set.cbegin())")
            lines.append("            {")
            lines.append("                os << std::endl;")
            lines.append("            }")
            lines.append("            (*it)->streamContents(os, indentLevel, isOneLineOnly);")
            lines.append("        }")
            lines.append(f"        if (my{b.class_name}Set.size() > 1)")
            lines.append("        {")
            lines.append("            isOneLineOnly = false;")
            lines.append("        }")
        else:
            if b.is_group:
                lines.append(f"        if (my{b.class_name})")
                lines.append("        {")
                lines.append(f"            my{b.class_name}->streamContents(os, indentLevel, isOneLineOnly);")
                lines.append("        }")
            elif b.is_container:
                lines.append(f"        my{b.class_name}->streamContents(os, indentLevel, isOneLineOnly);")
            else:
                lines.append(f"        my{b.class_name}->toStream(os, indentLevel);")
        lines.append("    }")
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"{choice_class}::Choice {choice_class}::getChoice() const")
    lines.append("{")
    lines.append("    return myChoice;")
    lines.append("}\n")

    lines.append(f"void {choice_class}::setChoice(const Choice value)")
    lines.append("{")
    lines.append("    myChoice = value;")
    lines.append("}\n")

    for b in branches:
        if b.is_set:
            lines.append(f"const {b.class_name}Set &{choice_class}::get{b.class_name}Set() const")
            lines.append("{")
            lines.append(f"    return my{b.class_name}Set;")
            lines.append("}\n")
            lines.append(f"void {choice_class}::remove{b.class_name}(const {b.class_name}SetIterConst &value)")
            lines.append("{")
            lines.append(f"    if (value != my{b.class_name}Set.cend())")
            lines.append("    {")
            lines.append(f"        my{b.class_name}Set.erase(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {choice_class}::add{b.class_name}(const {b.class_name}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{b.class_name}Set.push_back(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {choice_class}::clear{b.class_name}Set()")
            lines.append("{")
            lines.append(f"    my{b.class_name}Set.clear();")
            lines.append("}\n")
        else:
            lines.append(f"{b.class_name}Ptr {choice_class}::get{b.class_name}() const")
            lines.append("{")
            lines.append(f"    return my{b.class_name};")
            lines.append("}\n")
            lines.append(f"void {choice_class}::set{b.class_name}(const {b.class_name}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{b.class_name} = value;")
            lines.append("    }")
            lines.append("}\n")

    lines.append(f"MX_FROM_XELEMENT_UNUSED({choice_class});\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_tree_parent_h(elem_name: str, class_name: str,
                           choice_class: str, trailing: list,
                           has_attrs: bool, attrs_name: Optional[str],
                           model: XsdModel,
                           choice_is_set: bool = False,
                           leading_groups: list = None,
                           leading_children: list = None,
                           choice_is_optional: bool = False,
                           inline_choices: list = None,
                           choice_branches: list = None) -> str:
    if leading_groups is None:
        leading_groups = []
    if leading_children is None:
        leading_children = []
    if inline_choices is None:
        inline_choices = []
    multi_choice = len(inline_choices) > 1
    lines = [LICENSE, "#pragma once\n"]
    project_includes = ['"mx/core/ElementInterface.h"', '"mx/core/ForwardDeclare.h"']
    if has_attrs and attrs_name:
        if attrs_name in CORE_ROOT_ATTRS:
            project_includes.append(f'"mx/core/{attrs_name}.h"')
        else:
            project_includes.append(f'"mx/core/elements/{attrs_name}.h"')
    for inc in sorted(project_includes):
        lines.append(f"#include {inc}")
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    if multi_choice:
        if not any(ic.is_set for ic in inline_choices):
            lines.append("namespace ezxml\n{\nclass XElementIterator;\n}")
            lines.append("")
    elif not choice_is_set:
        lines.append("namespace ezxml\n{\nclass XElementIterator;\n}")
        lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")

    if has_attrs and attrs_name:
        lines.append(f"MX_FORWARD_DECLARE_ATTRIBUTES({attrs_name})")
    for lg_cls, lg_ref in leading_groups:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({lg_cls})")
    for child in leading_children:
        cc = child_class_name(child)
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({cc})")
    if multi_choice:
        for ic in inline_choices:
            lines.append(f"MX_FORWARD_DECLARE_ELEMENT({ic.choice_class})")
    else:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({choice_class})")
    for child in trailing:
        cc = child_class_name(child)
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({cc})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({class_name})\n")

    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")

    lines.append(f"\nclass {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")

    if has_attrs and attrs_name:
        lines.append(f"    {attrs_name}Ptr getAttributes() const;")
        lines.append(f"    void setAttributes(const {attrs_name}Ptr &value);")

    for lg_cls, lg_ref in leading_groups:
        lines.append(f"\n    /* _________ {lg_cls} minOccurs = 1, maxOccurs = 1 _________ */")
        lines.append(f"    {lg_cls}Ptr get{lg_cls}() const;")
        lines.append(f"    void set{lg_cls}(const {lg_cls}Ptr &value);")

    for child in leading_children:
        cc = child_class_name(child)
        max_o = "unbounded" if child.max_occurs == -1 else str(child.max_occurs)
        lines.append(f"\n    /* _________ {cc} minOccurs = {child.min_occurs}, maxOccurs = {max_o} _________ */")
        if child.max_occurs != 1:
            lines.append(f"    const {cc}Set &get{cc}Set() const;")
            lines.append(f"    void add{cc}(const {cc}Ptr &value);")
            lines.append(f"    void remove{cc}(const {cc}SetIterConst &value);")
            lines.append(f"    void clear{cc}Set();")
        elif child.min_occurs == 0:
            lines.append(f"    {cc}Ptr get{cc}() const;")
            lines.append(f"    void set{cc}(const {cc}Ptr &value);")
            lines.append(f"    bool getHas{cc}() const;")
            lines.append(f"    void setHas{cc}(const bool value);")
        else:
            lines.append(f"    {cc}Ptr get{cc}() const;")
            lines.append(f"    void set{cc}(const {cc}Ptr &value);")

    if multi_choice:
        for ic in inline_choices:
            icc = ic.choice_class
            if ic.is_optional and not ic.is_set:
                lines.append(f"\n    /* _________ {icc} minOccurs = 0, maxOccurs = 1 _________ */")
                lines.append(f"    {icc}Ptr get{icc}() const;")
                lines.append(f"    void set{icc}(const {icc}Ptr &value);")
                lines.append(f"    bool getHas{icc}() const;")
                lines.append(f"    void setHas{icc}(const bool value);")
            elif ic.is_set:
                lines.append(f"\n    /* _________ {icc} minOccurs = 0, maxOccurs = unbounded _________ */")
                lines.append(f"    const {icc}Set &get{icc}Set() const;")
                lines.append(f"    void add{icc}(const {icc}Ptr &value);")
                lines.append(f"    void remove{icc}(const {icc}SetIterConst &value);")
                lines.append(f"    void clear{icc}Set();")
            else:
                lines.append(f"    {icc}Ptr get{icc}() const;")
                lines.append(f"    void set{icc}(const {icc}Ptr &value);")
    else:
        if choice_is_optional and not choice_is_set:
            lines.append(f"\n    /* _________ {choice_class} minOccurs = 0, maxOccurs = 1 _________ */")
            lines.append(f"    {choice_class}Ptr get{choice_class}() const;")
            lines.append(f"    void set{choice_class}(const {choice_class}Ptr &value);")
            lines.append(f"    bool getHas{choice_class}() const;")
            lines.append(f"    void setHas{choice_class}(const bool value);")
        elif choice_is_set:
            lines.append(f"\n    /* _________ {choice_class} minOccurs = 0, maxOccurs = unbounded _________ */")
            lines.append(f"    const {choice_class}Set &get{choice_class}Set() const;")
            lines.append(f"    void add{choice_class}(const {choice_class}Ptr &value);")
            lines.append(f"    void remove{choice_class}(const {choice_class}SetIterConst &value);")
            lines.append(f"    void clear{choice_class}Set();")
            lines.append(f"    {choice_class}Ptr get{choice_class}(const {choice_class}SetIterConst &setIterator) const;")
        else:
            lines.append(f"    {choice_class}Ptr get{choice_class}() const;")
            lines.append(f"    void set{choice_class}(const {choice_class}Ptr &value);")

    for child in trailing:
        cc = child_class_name(child)
        max_o = "unbounded" if child.max_occurs == -1 else str(child.max_occurs)
        if leading_children:
            lines.append(f"\n    /* _________ {cc} minOccurs = {child.min_occurs}, maxOccurs = {max_o} _________ */")
        if child.max_occurs != 1:
            if choice_is_set:
                if not leading_children:
                    lines.append(f"\n    /* _________ {cc} minOccurs = 0, maxOccurs = unbounded _________ */")
                lines.append(f"    const {cc}Set &get{cc}Set() const;")
                lines.append(f"    void add{cc}(const {cc}Ptr &value);")
                lines.append(f"    void remove{cc}(const {cc}SetIterConst &value);")
                lines.append(f"    void clear{cc}Set();")
                lines.append(f"    {cc}Ptr get{cc}(const {cc}SetIterConst &setIterator) const;")
            else:
                lines.append(f"    const {cc}Set &get{cc}Set() const;")
                lines.append(f"    void remove{cc}(const {cc}SetIterConst &value);")
                lines.append(f"    void add{cc}(const {cc}Ptr &value);")
                lines.append(f"    void clear{cc}Set();")
        elif child.min_occurs == 0:
            lines.append(f"    {cc}Ptr get{cc}() const;")
            lines.append(f"    void set{cc}(const {cc}Ptr &value);")
            lines.append(f"    bool getHas{cc}() const;")
            lines.append(f"    void setHas{cc}(const bool value);")
        else:
            lines.append(f"    {cc}Ptr get{cc}() const;")
            lines.append(f"    void set{cc}(const {cc}Ptr &value);")

    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    if has_attrs and attrs_name:
        lines.append(f"    {attrs_name}Ptr myAttributes;")
    for lg_cls, lg_ref in leading_groups:
        lines.append(f"    {lg_cls}Ptr my{lg_cls};")
    for child in leading_children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f"    {cc}Set my{cc}Set;")
        else:
            lines.append(f"    {cc}Ptr my{cc};")
            if child.min_occurs == 0:
                lines.append(f"    bool myHas{cc};")
    if multi_choice:
        for ic in inline_choices:
            icc = ic.choice_class
            if ic.is_set:
                lines.append(f"    {icc}Set my{icc}Set;")
            else:
                lines.append(f"    {icc}Ptr my{icc};")
                if ic.is_optional:
                    lines.append(f"    bool myHas{icc};")
    else:
        if choice_is_set:
            lines.append(f"    {choice_class}Set my{choice_class}Set;")
        else:
            lines.append(f"    {choice_class}Ptr my{choice_class};")
            if choice_is_optional:
                lines.append(f"    bool myHas{choice_class};")
    for child in trailing:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f"    {cc}Set my{cc}Set;")
        else:
            lines.append(f"    {cc}Ptr my{cc};")
            if child.min_occurs == 0:
                lines.append(f"    bool myHas{cc};")

    # Private import helpers for parent-imported choice groups (Issues E/F).
    if not multi_choice and _get_tree_config(elem_name).get(
            "parent_imports_choice_groups", False) and choice_branches:
        lines.append("")
        for b in choice_branches:
            if not b.is_group:
                continue
            lines.append(f"    bool import{b.class_name}(std::ostream &message, ::ezxml::XElementIterator &iter,")
            lines.append(f"                                {' ' * len(b.class_name)}::ezxml::XElementIterator &endIter, bool &isSuccess);")

    # Private import helpers for container branches (is_container=True).
    if not multi_choice and choice_branches:
        container_branches = [b for b in choice_branches if b.is_container and b.trigger_names]
        if container_branches:
            lines.append("")
            for b in container_branches:
                fn_name = f"importContainer{b.class_name}"
                lines.append(f"    bool {fn_name}(std::ostream &message, ::ezxml::XElementIterator &it,")
                lines.append(f"    {' ' * (len(fn_name) + 6)}::ezxml::XElementIterator &endIter, bool &isSuccess);")

    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def _emit_choice_set_add(lines, choice_class, var, seed_choice_set):
    """Append a parsed choice `var` to my{choice_class}Set.

    When the set is seeded with a default in the constructor (seed_choice_set),
    the first parsed item replaces that default instead of appending after it;
    otherwise it is a plain push_back.
    """
    set_name = f"my{choice_class}Set"
    if seed_choice_set:
        lines.append(f"            if (!isFirstItemAdded && {set_name}.size() == 1)")
        lines.append("            {")
        lines.append(f"                *{set_name}.begin() = {var};")
        lines.append("                isFirstItemAdded = true;")
        lines.append("            }")
        lines.append("            else")
        lines.append("            {")
        lines.append(f"                {set_name}.push_back({var});")
        lines.append("                isFirstItemAdded = true;")
        lines.append("            }")
    else:
        lines.append(f"            {set_name}.push_back({var});")


def generate_tree_parent_cpp(elem_name: str, class_name: str,
                             choice_class: str, trailing: list,
                             has_attrs: bool, attrs_name: Optional[str],
                             plan: TreeGenPlan, model: XsdModel) -> str:
    choice_is_set = plan.choice_is_set
    choice_is_optional = plan.choice_is_optional
    leading_groups = plan.leading_groups or []
    leading_children = plan.leading_children or []
    inline_choices = plan.inline_choices or []
    multi_choice = len(inline_choices) > 1
    always_has_contents = _get_tree_config(elem_name).get("always_has_contents", False)
    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')

    all_includes = set()
    if multi_choice:
        for ic in inline_choices:
            all_includes.add(ic.choice_class)
            for b in ic.branches:
                all_includes.add(b.class_name)
                if b.is_group:
                    group_children = model.groups.get(b.group_name, [])
                    for gc in group_children:
                        all_includes.add(child_class_name(gc))
    else:
        for b in plan.choice_branches:
            all_includes.add(b.class_name)
            if b.is_group:
                group_children = model.groups.get(b.group_name, [])
                for gc in group_children:
                    all_includes.add(child_class_name(gc))
            if b.is_container:
                # Include headers for all members of the container, and for
                # nested choice branch classes so the importContainer<X> method
                # can reference them.
                container = next(
                    (c for c in plan.containers_to_generate if c.class_name == b.class_name),
                    None,
                )
                if container:
                    for m in container.members:
                        all_includes.add(m.class_name)
                        if m.use_stream_contents and not m.element_name and not m.is_optional:
                            # Nested choice: add its branch class names too
                            nc_entry = next(
                                (
                                    (nc_class, nc_branches)
                                    for (nc_class, nc_branches, _) in plan.nested_choices_to_generate
                                    if nc_class == m.class_name
                                ),
                                None,
                            )
                            if nc_entry:
                                _, nc_branches = nc_entry
                                for nb in nc_branches:
                                    all_includes.add(nb.class_name)
                                    if nb.is_group:
                                        gc_list = model.groups.get(nb.group_name, [])
                                        for gc in gc_list:
                                            all_includes.add(child_class_name(gc))
        all_includes.add(choice_class)
    for lg_cls, lg_ref in leading_groups:
        all_includes.add(lg_cls)
        if isinstance(lg_ref, GroupRefNode):
            group_children = model.groups.get(lg_ref.group_name, [])
            for gc in group_children:
                all_includes.add(child_class_name(gc))
    for child in leading_children:
        all_includes.add(child_class_name(child))
    for child in trailing:
        all_includes.add(child_class_name(child))
    for inc in sorted(all_includes):
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    init_parts = []
    if has_attrs and attrs_name:
        init_parts.append(f"myAttributes(std::make_shared<{attrs_name}>())")
    for lg_cls, lg_ref in leading_groups:
        init_parts.append(f"my{lg_cls}(make{lg_cls}())")
    for child in leading_children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            init_parts.append(f"my{cc}Set()")
        else:
            init_parts.append(f"my{cc}(make{cc}())")
            if child.min_occurs == 0:
                init_parts.append(f"myHas{cc}(false)")
    if multi_choice:
        for ic in inline_choices:
            icc = ic.choice_class
            if ic.is_set:
                init_parts.append(f"my{icc}Set()")
            else:
                init_parts.append(f"my{icc}(make{icc}())")
                if ic.is_optional:
                    init_parts.append(f"myHas{icc}(false)")
    else:
        if choice_is_set:
            init_parts.append(f"my{choice_class}Set()")
        else:
            init_parts.append(f"my{choice_class}(std::make_shared<{choice_class}>())")
            if choice_is_optional:
                init_parts.append(f"myHas{choice_class}(false)")
    for child in trailing:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            init_parts.append(f"my{cc}Set()")
        else:
            init_parts.append(f"my{cc}(make{cc}())")
            if child.min_occurs == 0:
                init_parts.append(f"myHas{cc}(false)")
    _emit_ctor_init(lines, f"{class_name}::{class_name}()", init_parts)
    lines.append("{")
    # If the config requests that the choice set be pre-seeded with one default
    # member (matching HEAD's pattern for notehead-text), emit the push_back.
    if choice_is_set and _get_tree_config(elem_name).get("seed_choice_set", False):
        lines.append(f"    my{choice_class}Set.push_back(make{choice_class}());")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    if has_attrs:
        lines.append("    return myAttributes->hasValues();")
    else:
        lines.append("    return false;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    if has_attrs:
        lines.append("    return myAttributes->toStream(os);")
    else:
        lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append(f'    os << "{elem_name}";')
    lines.append("    return os;")
    lines.append("}\n")

    has_contents_parts = []
    for lg_cls, lg_ref in leading_groups:
        has_contents_parts.append(f"my{lg_cls}->hasContents()")
    for child in leading_children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            has_contents_parts.append(f"my{cc}Set.size() > 0")
        elif child.min_occurs == 0:
            has_contents_parts.append(f"myHas{cc}")
        else:
            has_contents_parts.append("true")
    if multi_choice:
        for ic in inline_choices:
            icc = ic.choice_class
            if ic.is_set:
                has_contents_parts.append(f"my{icc}Set.size() > 0")
            elif ic.is_optional:
                has_contents_parts.append(f"myHas{icc}")
            else:
                has_contents_parts.append(f"my{icc}->hasContents()")
    else:
        if choice_is_set:
            has_contents_parts.append(f"my{choice_class}Set.size() > 0")
        elif choice_is_optional:
            has_contents_parts.append(f"myHas{choice_class}")
        else:
            has_contents_parts.append(f"my{choice_class}->hasContents()")
    for child in trailing:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            has_contents_parts.append(f"my{cc}Set.size() > 0")
        elif child.min_occurs == 0:
            has_contents_parts.append(f"myHas{cc}")
        else:
            has_contents_parts.append("true")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    if always_has_contents or "true" in has_contents_parts:
        lines.append("    return true;")
    else:
        lines.append(f"    return {' || '.join(has_contents_parts)};")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    if multi_choice:
        has_required = any(
            (child.min_occurs > 0 and child.max_occurs == 1)
            for child in list(leading_children) + list(trailing)
        ) or any(not ic.is_optional and not ic.is_set for ic in inline_choices)
        if not has_required:
            lines.append("    if (hasContents())")
            lines.append("    {")
            indent = "        "
        else:
            indent = "    "
        for child in leading_children:
            cc = child_class_name(child)
            if child.max_occurs != 1:
                lines.append(f"{indent}for (auto x : my{cc}Set)")
                lines.append(f"{indent}{{")
                lines.append(f"{indent}    os << std::endl;")
                lines.append(f"{indent}    x->toStream(os, indentLevel + 1);")
                lines.append(f"{indent}}}")
            elif child.min_occurs == 0:
                lines.append(f"{indent}if (myHas{cc})")
                lines.append(f"{indent}{{")
                lines.append(f"{indent}    os << std::endl;")
                lines.append(f"{indent}    my{cc}->toStream(os, indentLevel + 1);")
                lines.append(f"{indent}}}")
            else:
                lines.append(f"{indent}os << std::endl;")
                lines.append(f"{indent}my{cc}->toStream(os, indentLevel + 1);")
        for ic in inline_choices:
            icc = ic.choice_class
            if ic.is_optional:
                lines.append(f"{indent}if (myHas{icc})")
                lines.append(f"{indent}{{")
                lines.append(f"{indent}    os << std::endl;")
                lines.append(f"{indent}    my{icc}->streamContents(os, indentLevel + 1, isOneLineOnly);")
                lines.append(f"{indent}}}")
            elif ic.is_set:
                lines.append(f"{indent}for (auto x : my{icc}Set)")
                lines.append(f"{indent}{{")
                lines.append(f"{indent}    os << std::endl;")
                lines.append(f"{indent}    x->streamContents(os, indentLevel + 1, isOneLineOnly);")
                lines.append(f"{indent}}}")
            else:
                lines.append(f"{indent}if (my{icc}->hasContents())")
                lines.append(f"{indent}{{")
                lines.append(f"{indent}    os << std::endl;")
                lines.append(f"{indent}    my{icc}->streamContents(os, indentLevel + 1, isOneLineOnly);")
                lines.append(f"{indent}}}")
        for child in trailing:
            cc = child_class_name(child)
            if child.max_occurs != 1:
                lines.append(f"{indent}for (auto x : my{cc}Set)")
                lines.append(f"{indent}{{")
                lines.append(f"{indent}    os << std::endl;")
                lines.append(f"{indent}    x->toStream(os, indentLevel + 1);")
                lines.append(f"{indent}}}")
            elif child.min_occurs == 0:
                lines.append(f"{indent}if (myHas{cc})")
                lines.append(f"{indent}{{")
                lines.append(f"{indent}    os << std::endl;")
                lines.append(f"{indent}    my{cc}->toStream(os, indentLevel + 1);")
                lines.append(f"{indent}}}")
            else:
                lines.append(f"{indent}os << std::endl;")
                lines.append(f"{indent}my{cc}->toStream(os, indentLevel + 1);")
        lines.append(f"{indent}os << std::endl;")
        lines.append(f"{indent}isOneLineOnly = false;")
        if not has_required:
            lines.append("    }")
            lines.append("    else")
            lines.append("    {")
            lines.append("        isOneLineOnly = true;")
            lines.append("    }")
    elif choice_is_set:
        if leading_groups:
            lines.append("    if (hasContents())")
            lines.append("    {")
            for lg_cls, lg_ref in leading_groups:
                lines.append(f"        if (my{lg_cls}->hasContents())")
                lines.append("        {")
                lines.append("            os << std::endl;")
                lines.append(f"            my{lg_cls}->streamContents(os, indentLevel + 1, isOneLineOnly);")
                lines.append("        }")
            lines.append(f"        for (auto x : my{choice_class}Set)")
            lines.append("        {")
            lines.append("            os << std::endl;")
            lines.append("            x->streamContents(os, indentLevel + 1, isOneLineOnly);")
            lines.append("        }")
            for child in trailing:
                cc = child_class_name(child)
                if child.max_occurs != 1:
                    lines.append(f"        for (auto x : my{cc}Set)")
                    lines.append("        {")
                    lines.append("            os << std::endl;")
                    lines.append("            x->toStream(os, indentLevel + 1);")
                    lines.append("        }")
            lines.append("        os << std::endl;")
            lines.append("        isOneLineOnly = false;")
            lines.append("    }")
            lines.append("    else")
            lines.append("    {")
            lines.append("        isOneLineOnly = true;")
            lines.append("    }")
        elif always_has_contents and not trailing:
            lines.append(f"    if (my{choice_class}Set.size() > 0)")
            lines.append("    {")
            lines.append(f"        for (auto x : my{choice_class}Set)")
            lines.append("        {")
            lines.append("            os << std::endl;")
            lines.append("            x->streamContents(os, indentLevel + 1, isOneLineOnly);")
            lines.append("        }")
            lines.append("        isOneLineOnly = false;")
            lines.append("        os << std::endl;")
            lines.append("    }")
            lines.append("    else")
            lines.append("    {")
            lines.append("        isOneLineOnly = true;")
            lines.append("    }")
        else:
            for lg_cls, lg_ref in leading_groups:
                lines.append(f"    if (my{lg_cls}->hasContents())")
                lines.append("    {")
                lines.append("        os << std::endl;")
                lines.append(f"        my{lg_cls}->streamContents(os, indentLevel + 1, isOneLineOnly);")
                lines.append("    }")
            lines.append(f"    for (auto x : my{choice_class}Set)")
            lines.append("    {")
            lines.append("        os << std::endl;")
            lines.append("        x->streamContents(os, indentLevel + 1, isOneLineOnly);")
            lines.append("    }")
            for child in trailing:
                cc = child_class_name(child)
                if child.max_occurs != 1:
                    lines.append(f"    for (auto x : my{cc}Set)")
                    lines.append("    {")
                    lines.append("        os << std::endl;")
                    lines.append("        x->toStream(os, indentLevel + 1);")
                    lines.append("    }")
            lines.append("    isOneLineOnly = false;")
            lines.append("    os << std::endl;")
    elif leading_children or choice_is_optional:
        for child in leading_children:
            cc = child_class_name(child)
            if child.max_occurs != 1:
                lines.append(f"    for (auto x : my{cc}Set)")
                lines.append("    {")
                lines.append("        os << std::endl;")
                lines.append("        x->toStream(os, indentLevel + 1);")
                lines.append("    }")
            elif child.min_occurs == 0:
                lines.append(f"    if (myHas{cc})")
                lines.append("    {")
                lines.append("        os << std::endl;")
                lines.append(f"        my{cc}->toStream(os, indentLevel + 1);")
                lines.append("    }")
            else:
                lines.append("    os << std::endl;")
                lines.append(f"    my{cc}->toStream(os, indentLevel + 1);")
        if choice_is_optional:
            lines.append(f"    if (myHas{choice_class})")
            lines.append("    {")
            lines.append("        os << std::endl;")
            lines.append(f"        my{choice_class}->streamContents(os, indentLevel + 1, isOneLineOnly);")
            lines.append("    }")
        else:
            lines.append(f"    if (my{choice_class}->hasContents())")
            lines.append("    {")
            lines.append("        os << std::endl;")
            lines.append(f"        my{choice_class}->streamContents(os, indentLevel + 1, isOneLineOnly);")
            lines.append("    }")
        for child in trailing:
            cc = child_class_name(child)
            if child.max_occurs != 1:
                lines.append(f"    for (auto x : my{cc}Set)")
                lines.append("    {")
                lines.append("        os << std::endl;")
                lines.append("        x->toStream(os, indentLevel + 1);")
                lines.append("    }")
            elif child.min_occurs == 0:
                lines.append(f"    if (myHas{cc})")
                lines.append("    {")
                lines.append("        os << std::endl;")
                lines.append(f"        my{cc}->toStream(os, indentLevel + 1);")
                lines.append("    }")
            else:
                lines.append("    os << std::endl;")
                lines.append(f"    my{cc}->toStream(os, indentLevel + 1);")
        lines.append("    os << std::endl;")
        lines.append("    isOneLineOnly = false;")
    else:
        lines.append(f"    if (my{choice_class}->hasContents())")
        lines.append("    {")
        lines.append("        os << std::endl;")
        lines.append(f"        if (my{choice_class})")
        lines.append("        {")
        lines.append(f"            my{choice_class}->streamContents(os, indentLevel + 1, isOneLineOnly);")
        lines.append("        }")
        lines.append("        os << std::endl;")
        lines.append("    }")
        for child in trailing:
            cc = child_class_name(child)
            if child.max_occurs != 1:
                lines.append(f"    for (auto it = my{cc}Set.cbegin(); it != my{cc}Set.cend(); ++it)")
                lines.append("    {")
                lines.append(f"        if (it == my{cc}Set.cbegin())")
                lines.append("        {")
                lines.append("            os << std::endl;")
                lines.append("        }")
                lines.append("        (*it)->toStream(os, indentLevel + 1);")
                lines.append("        os << std::endl;")
                lines.append("    }")
            elif child.min_occurs == 0:
                lines.append(f"    if (myHas{cc})")
                lines.append("    {")
                lines.append("        os << std::endl;")
                lines.append(f"        my{cc}->toStream(os, indentLevel + 1);")
                lines.append("    }")
        lines.append(f"    if (my{choice_class}->hasContents())")
        lines.append("    {")
        lines.append("        isOneLineOnly = false;")
        lines.append("    }")
        for child in trailing:
            cc = child_class_name(child)
            if child.max_occurs != 1:
                lines.append(f"    if (my{cc}Set.size() > 0)")
                lines.append("    {")
                lines.append("        isOneLineOnly = false;")
                lines.append("    }")
    lines.append("    return os;")
    lines.append("}\n")

    if has_attrs and attrs_name:
        lines.append(f"{attrs_name}Ptr {class_name}::getAttributes() const")
        lines.append("{")
        lines.append("    return myAttributes;")
        lines.append("}\n")
        lines.append(f"void {class_name}::setAttributes(const {attrs_name}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append("        myAttributes = value;")
        lines.append("    }")
        lines.append("}\n")

    for lg_cls, lg_ref in leading_groups:
        lines.append(f"{lg_cls}Ptr {class_name}::get{lg_cls}() const")
        lines.append("{")
        lines.append(f"    return my{lg_cls};")
        lines.append("}\n")
        lines.append(f"void {class_name}::set{lg_cls}(const {lg_cls}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{lg_cls} = value;")
        lines.append("    }")
        lines.append("}\n")

    for child in leading_children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f"const {cc}Set &{class_name}::get{cc}Set() const")
            lines.append("{")
            lines.append(f"    return my{cc}Set;")
            lines.append("}\n")
            lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc}Set.push_back(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
            lines.append("{")
            lines.append(f"    if (value != my{cc}Set.cend())")
            lines.append("    {")
            lines.append(f"        my{cc}Set.erase(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {class_name}::clear{cc}Set()")
            lines.append("{")
            lines.append(f"    my{cc}Set.clear();")
            lines.append("}\n")
        else:
            lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
            lines.append("{")
            lines.append(f"    return my{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}\n")
            if child.min_occurs == 0:
                lines.append(f"bool {class_name}::getHas{cc}() const")
                lines.append("{")
                lines.append(f"    return myHas{cc};")
                lines.append("}\n")
                lines.append(f"void {class_name}::setHas{cc}(const bool value)")
                lines.append("{")
                lines.append(f"    myHas{cc} = value;")
                lines.append("}\n")

    if multi_choice:
        for ic in inline_choices:
            icc = ic.choice_class
            if ic.is_set:
                lines.append(f"const {icc}Set &{class_name}::get{icc}Set() const")
                lines.append("{")
                lines.append(f"    return my{icc}Set;")
                lines.append("}\n")
                lines.append(f"void {class_name}::add{icc}(const {icc}Ptr &value)")
                lines.append("{")
                lines.append("    if (value)")
                lines.append("    {")
                lines.append(f"        my{icc}Set.push_back(value);")
                lines.append("    }")
                lines.append("}\n")
                lines.append(f"void {class_name}::remove{icc}(const {icc}SetIterConst &value)")
                lines.append("{")
                lines.append(f"    if (value != my{icc}Set.cend())")
                lines.append("    {")
                lines.append(f"        my{icc}Set.erase(value);")
                lines.append("    }")
                lines.append("}\n")
                lines.append(f"void {class_name}::clear{icc}Set()")
                lines.append("{")
                lines.append(f"    my{icc}Set.clear();")
                lines.append("}\n")
            else:
                lines.append(f"{icc}Ptr {class_name}::get{icc}() const")
                lines.append("{")
                lines.append(f"    return my{icc};")
                lines.append("}\n")
                lines.append(f"void {class_name}::set{icc}(const {icc}Ptr &value)")
                lines.append("{")
                lines.append("    if (value)")
                lines.append("    {")
                lines.append(f"        my{icc} = value;")
                lines.append("    }")
                lines.append("}\n")
                if ic.is_optional:
                    lines.append(f"bool {class_name}::getHas{icc}() const")
                    lines.append("{")
                    lines.append(f"    return myHas{icc};")
                    lines.append("}\n")
                    lines.append(f"void {class_name}::setHas{icc}(const bool value)")
                    lines.append("{")
                    lines.append(f"    myHas{icc} = value;")
                    lines.append("}\n")
    else:
        if choice_is_set:
            lines.append(f"const {choice_class}Set &{class_name}::get{choice_class}Set() const")
            lines.append("{")
            lines.append(f"    return my{choice_class}Set;")
            lines.append("}\n")
            lines.append(f"void {class_name}::add{choice_class}(const {choice_class}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{choice_class}Set.push_back(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {class_name}::remove{choice_class}(const {choice_class}SetIterConst &value)")
            lines.append("{")
            lines.append(f"    if (value != my{choice_class}Set.cend())")
            lines.append("    {")
            lines.append(f"        my{choice_class}Set.erase(value);")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"void {class_name}::clear{choice_class}Set()")
            lines.append("{")
            lines.append(f"    my{choice_class}Set.clear();")
            if _get_tree_config(elem_name).get("seed_choice_set", False):
                lines.append(f"    my{choice_class}Set.push_back(make{choice_class}());")
            lines.append("}\n")
            lines.append(f"{choice_class}Ptr {class_name}::get{choice_class}(const {choice_class}SetIterConst &setIterator) const")
            lines.append("{")
            lines.append(f"    if (setIterator != my{choice_class}Set.cend())")
            lines.append("    {")
            lines.append("        return *setIterator;")
            lines.append("    }")
            lines.append(f"    return {choice_class}Ptr();")
            lines.append("}\n")
        else:
            lines.append(f"{choice_class}Ptr {class_name}::get{choice_class}() const")
            lines.append("{")
            lines.append(f"    return my{choice_class};")
            lines.append("}\n")
            lines.append(f"void {class_name}::set{choice_class}(const {choice_class}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{choice_class} = value;")
            lines.append("    }")
            lines.append("}\n")
            if choice_is_optional:
                lines.append(f"bool {class_name}::getHas{choice_class}() const")
                lines.append("{")
                lines.append(f"    return myHas{choice_class};")
                lines.append("}\n")
                lines.append(f"void {class_name}::setHas{choice_class}(const bool value)")
                lines.append("{")
                lines.append(f"    myHas{choice_class} = value;")
                lines.append("}\n")

    for child in trailing:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f"const {cc}Set &{class_name}::get{cc}Set() const")
            lines.append("{")
            lines.append(f"    return my{cc}Set;")
            lines.append("}\n")
            if choice_is_set:
                lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
                lines.append("{")
                lines.append("    if (value)")
                lines.append("    {")
                lines.append(f"        my{cc}Set.push_back(value);")
                lines.append("    }")
                lines.append("}\n")
                lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
                lines.append("{")
                lines.append(f"    if (value != my{cc}Set.cend())")
                lines.append("    {")
                lines.append(f"        my{cc}Set.erase(value);")
                lines.append("    }")
                lines.append("}\n")
                lines.append(f"void {class_name}::clear{cc}Set()")
                lines.append("{")
                lines.append(f"    my{cc}Set.clear();")
                lines.append("}\n")
                lines.append(f"{cc}Ptr {class_name}::get{cc}(const {cc}SetIterConst &setIterator) const")
                lines.append("{")
                lines.append(f"    if (setIterator != my{cc}Set.cend())")
                lines.append("    {")
                lines.append("        return *setIterator;")
                lines.append("    }")
                lines.append(f"    return {cc}Ptr();")
                lines.append("}\n")
            else:
                lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
                lines.append("{")
                lines.append(f"    if (value != my{cc}Set.cend())")
                lines.append("    {")
                lines.append(f"        my{cc}Set.erase(value);")
                lines.append("    }")
                lines.append("}\n")
                lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
                lines.append("{")
                lines.append("    if (value)")
                lines.append("    {")
                lines.append(f"        my{cc}Set.push_back(value);")
                lines.append("    }")
                lines.append("}\n")
                lines.append(f"void {class_name}::clear{cc}Set()")
                lines.append("{")
                lines.append(f"    my{cc}Set.clear();")
                lines.append("}\n")
        elif child.min_occurs == 0:
            lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
            lines.append("{")
            lines.append(f"    return my{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}\n")
            lines.append(f"bool {class_name}::getHas{cc}() const")
            lines.append("{")
            lines.append(f"    return myHas{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::setHas{cc}(const bool value)")
            lines.append("{")
            lines.append(f"    myHas{cc} = value;")
            lines.append("}\n")
        else:
            lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
            lines.append("{")
            lines.append(f"    return my{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}\n")

    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    if has_attrs:
        lines.append("    isSuccess &= myAttributes->fromXElement(message, xelement);")
    for child in leading_children:
        cc = child_class_name(child)
        if child.max_occurs == 1 and child.min_occurs == 1:
            lines.append(f"    bool is{cc}Found = false;")
    # When the choice set is seeded with a default item in the constructor (so
    # hasContents() is true), the first parsed item must replace that default
    # rather than append after it -- otherwise the round-trip emits a spurious
    # empty leading element.
    seed_choice_set = choice_is_set and _get_tree_config(elem_name).get("seed_choice_set", False)
    if seed_choice_set:
        lines.append("    bool isFirstItemAdded = false;")
    lines.append("")
    lines.append("    auto endIter = xelement.end();")
    lines.append("    for (auto it = xelement.begin(); it != endIter; ++it)")
    lines.append("    {")

    for child in leading_children:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f'        if (importElementSet(message, it, endIter, isSuccess, "{child.element_name}", my{cc}Set))')
            lines.append("        {")
            lines.append("            continue;")
            lines.append("        }")
        elif child.min_occurs == 0:
            lines.append(f"        if (importElement(message, *it, isSuccess, *my{cc}, myHas{cc}))")
            lines.append("        {")
            lines.append("            continue;")
            lines.append("        }")
        else:
            lines.append(f"        if (importElement(message, *it, isSuccess, *my{cc}, is{cc}Found))")
            lines.append("        {")
            lines.append("            continue;")
            lines.append("        }")

    for lg_cls, lg_ref in leading_groups:
        if isinstance(lg_ref, GroupRefNode):
            # A leading group ref (e.g. the editorial group: footnote, level)
            # is consumed by importGroup, which leaves the iterator pointing at
            # the next unconsumed sibling (or, at parent end, the last consumed
            # member). It must NOT be guarded by a name check followed by
            # `continue`: the `continue` would advance past the element the
            # iterator was left on, dropping the first member of whatever
            # follows the group (e.g. the articulations after a notations
            # editorial group). Emit a bare importGroup, matching the idiom in
            # Forward/Barline/Direction, and let the choice/trailing branches
            # below re-inspect the current element. importGroup is a no-op when
            # the iterator does not point at a group member.
            lines.append(f"        importGroup(message, it, endIter, isSuccess, my{lg_cls});")

    if multi_choice:
        for ic in inline_choices:
            icc = ic.choice_class
            for b in ic.branches:
                if b.is_group:
                    group_children = model.groups.get(b.group_name, [])
                    elem_names = [gc.element_name for gc in group_children]
                    cond = " || ".join(f'it->getName() == "{n}"' for n in elem_names)
                    lines.append(f"        if ({cond})")
                    lines.append("        {")
                    lines.append(f"            my{icc}->setChoice({icc}::Choice::{b.enum_name});")
                    lines.append(f"            auto groupPtr = my{icc}->get{b.class_name}();")
                    lines.append(f"            importGroup(message, it, endIter, isSuccess, groupPtr);")
                    if ic.is_optional:
                        lines.append(f"            myHas{icc} = true;")
                    lines.append("            continue;")
                    lines.append("        }")
                elif b.xml_name:
                    lines.append(f'        if (it->getName() == "{b.xml_name}")')
                    lines.append("        {")
                    lines.append(f"            my{icc}->setChoice({icc}::Choice::{b.enum_name});")
                    lines.append(f"            isSuccess &= my{icc}->get{b.class_name}()->fromXElement(message, *it);")
                    if ic.is_optional:
                        lines.append(f"            myHas{icc} = true;")
                    lines.append("            continue;")
                    lines.append("        }")
    else:
        parent_imports_groups = _get_tree_config(elem_name).get(
            "parent_imports_choice_groups", False)
        for b in plan.choice_branches:
            if b.is_group and parent_imports_groups:
                # Route group parsing through a private member function on the
                # parent class (see Issues E/F). The body is emitted after
                # fromXElementImpl below.
                lines.append(f"        if (import{b.class_name}(message, it, endIter, isSuccess))")
                lines.append("        {")
                lines.append("            continue;")
                lines.append("        }")
            elif b.is_group:
                group_children = model.groups.get(b.group_name, [])
                elem_names = [gc.element_name for gc in group_children]
                cond = " || ".join(f'it->getName() == "{n}"' for n in elem_names)
                lines.append(f"        if ({cond})")
                lines.append("        {")
                if choice_is_set:
                    lines.append(f"            auto choice = make{choice_class}();")
                    lines.append(f"            choice->setChoice({choice_class}::Choice::{b.enum_name});")
                    lines.append(f"            auto groupPtr = choice->get{b.class_name}();")
                    lines.append(f"            importGroup(message, it, endIter, isSuccess, groupPtr);")
                    _emit_choice_set_add(lines, choice_class, "choice", seed_choice_set)
                else:
                    lines.append(f"            my{choice_class}->setChoice({choice_class}::Choice::{b.enum_name});")
                    if b.is_set:
                        lines.append(f"            auto item = make{b.class_name}();")
                        lines.append(f"            importGroup(message, it, endIter, isSuccess, item);")
                        lines.append(f"            my{choice_class}->add{b.class_name}(item);")
                    else:
                        lines.append(f"            auto groupPtr = my{choice_class}->get{b.class_name}();")
                        lines.append(f"            importGroup(message, it, endIter, isSuccess, groupPtr);")
                lines.append("            continue;")
                lines.append("        }")
            elif not b.is_group and b.xml_name:
                xml_name_kebab = b.xml_name
                if choice_is_set:
                    lines.append(f'        if (it->getName() == "{xml_name_kebab}")')
                    lines.append("        {")
                    lines.append(f"            auto choice = make{choice_class}();")
                    lines.append(f"            choice->setChoice({choice_class}::Choice::{b.enum_name});")
                    lines.append(f"            isSuccess &= choice->get{b.class_name}()->fromXElement(message, *it);")
                    _emit_choice_set_add(lines, choice_class, "choice", seed_choice_set)
                    lines.append("            continue;")
                    lines.append("        }")
                else:
                    lines.append(f'        if (it->getName() == "{xml_name_kebab}")')
                    lines.append("        {")
                    lines.append(f"            my{choice_class}->setChoice({choice_class}::Choice::{b.enum_name});")
                    lines.append(f"            isSuccess &= my{choice_class}->get{b.class_name}()->fromXElement(message, *it);")
                    if choice_is_optional:
                        lines.append(f"            myHas{choice_class} = true;")
                    lines.append("            continue;")
                    lines.append("        }")
            elif b.is_container and b.trigger_names:
                # Dispatch to a private importContainer<X> method when any of
                # the container's trigger element names is seen. The method
                # handles parsing all container members in order and leaves
                # the iterator pointing at the last consumed element.
                cond = " || ".join(f'it->getName() == "{n}"' for n in b.trigger_names)
                lines.append(f"        if ({cond})")
                lines.append("        {")
                lines.append(f"            if (importContainer{b.class_name}(message, it, endIter, isSuccess))")
                lines.append("            {")
                lines.append("                continue;")
                lines.append("            }")
                lines.append("        }")

    for child in trailing:
        cc = child_class_name(child)
        if child.max_occurs != 1:
            lines.append(f'        if (importElementSet(message, it, endIter, isSuccess, "{child.element_name}", my{cc}Set))')
            lines.append("        {")
            lines.append("            continue;")
            lines.append("        }")
        elif child.min_occurs == 0:
            if leading_children:
                lines.append(f"        if (importElement(message, *it, isSuccess, *my{cc}, myHas{cc}))")
                lines.append("        {")
                lines.append("            continue;")
                lines.append("        }")
            else:
                lines.append(f'        importElement(message, "{child.element_name}", isSuccess, *it, myHas{cc}, my{cc});')
    lines.append("    }")
    lines.append("")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}\n")

    # Emit private member-function bodies for parent-imported choice groups
    # (Issues E/F). Routed through here when
    # TREE_ELEMENT_CONFIG[elem_name]["parent_imports_choice_groups"] is True.
    if not multi_choice and _get_tree_config(elem_name).get(
            "parent_imports_choice_groups", False):
        for b in plan.choice_branches:
            if not b.is_group:
                continue
            group_children = model.groups.get(b.group_name, [])
            elem_names = [gc.element_name for gc in group_children]
            mismatch_cond = " && ".join(f'iter->getName() != "{n}"' for n in elem_names)
            match_cond = " || ".join(f'iter->getName() == "{n}"' for n in elem_names)

            lines.append(f"bool {class_name}::import{b.class_name}(std::ostream &message, ::ezxml::XElementIterator &iter,")
            lines.append(f"                                {' ' * len(class_name)}::ezxml::XElementIterator &endIter, bool &isSuccess)")
            lines.append("{")
            lines.append("    if (iter == endIter)")
            lines.append("    {")
            lines.append("        return false;")
            lines.append("    }")
            lines.append("")
            lines.append(f"    if ({mismatch_cond})")
            lines.append("    {")
            lines.append("        return false;")
            lines.append("    }")
            lines.append("")
            lines.append("    bool isIterIncremented = false;")
            if b.is_set:
                lines.append("    bool isFirstItemAdded = false;")
                lines.append("")
                lines.append(f"    while (iter != endIter &&")
                lines.append(f"           ({match_cond}))")
                lines.append("    {")
                lines.append(f"        auto item = make{b.class_name}();")
                lines.append(f"        const auto &items = my{choice_class}->get{b.class_name}Set();")
                lines.append(f"        my{choice_class}->setChoice({choice_class}::Choice::{b.enum_name});")
                lines.append("")
                for gc in group_children:
                    gcc = child_class_name(gc)
                    lines.append(f'        if (iter != endIter && iter->getName() == "{gc.element_name}")')
                    lines.append("        {")
                    if gc.min_occurs == 0:
                        lines.append(f"            item->setHas{gcc}(true);")
                    lines.append(f"            isSuccess &= item->get{gcc}()->fromXElement(message, *iter);")
                    lines.append("            isIterIncremented = true;")
                    lines.append("            ++iter;")
                    lines.append("        }")
                    lines.append("")
                lines.append("        if (!isFirstItemAdded && items.size() == 1)")
                lines.append("        {")
                lines.append(f"            my{choice_class}->add{b.class_name}(item);")
                lines.append(f"            my{choice_class}->remove{b.class_name}(items.cbegin());")
                lines.append("            isFirstItemAdded = true;")
                lines.append("        }")
                lines.append("        else")
                lines.append("        {")
                lines.append(f"            my{choice_class}->add{b.class_name}(item);")
                lines.append("            isFirstItemAdded = true;")
                lines.append("        }")
                lines.append("    }")
                lines.append("")
            else:
                lines.append("")
                for gc in group_children:
                    gcc = child_class_name(gc)
                    lines.append(f'    if (iter != endIter && iter->getName() == "{gc.element_name}")')
                    lines.append("    {")
                    lines.append(f"        my{choice_class}->setChoice({choice_class}::Choice::{b.enum_name});")
                    if gc.min_occurs == 0:
                        lines.append(f"        my{choice_class}->get{b.class_name}()->setHas{gcc}(true);")
                    lines.append(f"        isSuccess &= my{choice_class}->get{b.class_name}()->get{gcc}()->fromXElement(message, *iter);")
                    lines.append("        isIterIncremented = true;")
                    lines.append("        ++iter;")
                    lines.append("    }")
                    lines.append("")
            lines.append("    if (isIterIncremented)")
            lines.append("    {")
            lines.append("        --iter;")
            lines.append("    }")
            lines.append("")
            lines.append("    return true;")
            lines.append("}\n")

    # Emit private member-function bodies for container branches (is_container=True).
    # Each container branch gets an importContainer<X> method that parses the
    # container's members in order from the shared iterator.
    if not multi_choice:
        # Build a lookup: nested choice class_name -> list of TreeChoiceBranch
        nested_choice_map = {}
        for (nc_class, nc_branches, _nc_parent) in plan.nested_choices_to_generate:
            nested_choice_map[nc_class] = nc_branches

        for b in plan.choice_branches:
            if not b.is_container or not b.trigger_names:
                continue

            # Find the container definition
            container = next(
                (c for c in plan.containers_to_generate if c.class_name == b.class_name),
                None,
            )
            if container is None:
                continue

            mismatch_cond = " && ".join(f'it->getName() != "{n}"' for n in b.trigger_names)
            fn_name = f"importContainer{b.class_name}"
            indent = " " * (len(class_name) + len(fn_name) + 7)
            lines.append(f"bool {class_name}::{fn_name}(std::ostream &message, ::ezxml::XElementIterator &it,")
            lines.append(f"    {indent}::ezxml::XElementIterator &endIter, bool &isSuccess)")
            lines.append("{")
            lines.append("    if (it == endIter)")
            lines.append("    {")
            lines.append("        return false;")
            lines.append("    }")
            lines.append("")
            lines.append(f"    if ({mismatch_cond})")
            lines.append("    {")
            lines.append("        return false;")
            lines.append("    }")
            lines.append("")
            lines.append(f"    my{choice_class}->setChoice({choice_class}::Choice::{b.enum_name});")
            lines.append(f"    auto containerPtr = my{choice_class}->get{b.class_name}();")
            lines.append("    bool isIterIncremented = false;")
            lines.append("")

            for m in container.members:
                cc = m.class_name
                if m.is_set and m.element_name:
                    # Repeating element - use a while loop
                    lines.append(f'    while (it != endIter && it->getName() == "{m.element_name}")')
                    lines.append("    {")
                    lines.append(f"        auto item = make{cc}();")
                    lines.append(f"        isSuccess &= item->fromXElement(message, *it);")
                    lines.append(f"        const auto &items = containerPtr->get{cc}Set();")
                    lines.append("        if (items.size() == 1 && !isIterIncremented)")
                    lines.append("        {")
                    lines.append(f"            containerPtr->add{cc}(item);")
                    lines.append(f"            containerPtr->remove{cc}(items.cbegin());")
                    lines.append("        }")
                    lines.append("        else")
                    lines.append("        {")
                    lines.append(f"            containerPtr->add{cc}(item);")
                    lines.append("        }")
                    lines.append("        isIterIncremented = true;")
                    lines.append("        ++it;")
                    lines.append("    }")
                    lines.append("")
                elif m.is_optional and m.use_stream_contents and not m.element_name:
                    # Optional group (e.g. MetronomeRelationGroup): call importGroup
                    lines.append(f"    bool has{cc} = false;")
                    lines.append(f"    auto {camel(cc)} = containerPtr->get{cc}();")
                    lines.append(f"    importGroup(message, it, endIter, isSuccess, {camel(cc)}, has{cc});")
                    lines.append(f"    if (has{cc})")
                    lines.append("    {")
                    lines.append(f"        containerPtr->setHas{cc}(true);")
                    lines.append("        isIterIncremented = true;")
                    lines.append("        ++it;")
                    lines.append("    }")
                    lines.append("")
                elif m.use_stream_contents and not m.element_name and not m.is_optional:
                    if cc in nested_choice_map:
                        # Nested choice: dispatch each branch
                        nc_branches = nested_choice_map[cc]
                        lines.append(f"    auto {camel(cc)} = containerPtr->get{cc}();")
                        first_branch = True
                        for nb in nc_branches:
                            kw = "if" if first_branch else "else if"
                            if nb.is_group:
                                group_children = model.groups.get(nb.group_name, [])
                                elem_names = [gc.element_name for gc in group_children
                                              if hasattr(gc, "element_name") and gc.element_name]
                                if not elem_names:
                                    continue
                                ncond = " || ".join(f'it->getName() == "{n}"' for n in elem_names)
                                lines.append(f"    {kw} (it != endIter && ({ncond}))")
                                lines.append("    {")
                                lines.append(f"        {camel(cc)}->setChoice({cc}::Choice::{nb.enum_name});")
                                lines.append(f"        auto groupPtr = {camel(cc)}->get{nb.class_name}();")
                                lines.append(f"        importGroup(message, it, endIter, isSuccess, groupPtr);")
                                lines.append("        isIterIncremented = true;")
                                lines.append("        ++it;")
                                lines.append("    }")
                                first_branch = False
                            elif nb.xml_name:
                                lines.append(f'    {kw} (it != endIter && it->getName() == "{nb.xml_name}")')
                                lines.append("    {")
                                lines.append(f"        {camel(cc)}->setChoice({cc}::Choice::{nb.enum_name});")
                                lines.append(f"        isSuccess &= {camel(cc)}->get{nb.class_name}()->fromXElement(message, *it);")
                                lines.append("        isIterIncremented = true;")
                                lines.append("        ++it;")
                                lines.append("    }")
                                first_branch = False
                        lines.append("")
                    else:
                        # Plain group (use importGroup)
                        lines.append(f"    if (it != endIter)")
                        lines.append("    {")
                        lines.append(f"        auto groupPtr = containerPtr->get{cc}();")
                        lines.append(f"        importGroup(message, it, endIter, isSuccess, groupPtr);")
                        lines.append("        isIterIncremented = true;")
                        lines.append("        ++it;")
                        lines.append("    }")
                        lines.append("")
                elif not m.is_optional and not m.is_set and m.element_name:
                    # Required single element
                    lines.append(f'    if (it != endIter && it->getName() == "{m.element_name}")')
                    lines.append("    {")
                    lines.append(f"        isSuccess &= containerPtr->get{cc}()->fromXElement(message, *it);")
                    lines.append("        isIterIncremented = true;")
                    lines.append("        ++it;")
                    lines.append("    }")
                    lines.append("")
                elif m.is_optional and m.element_name:
                    # Optional single element
                    lines.append(f'    if (it != endIter && it->getName() == "{m.element_name}")')
                    lines.append("    {")
                    lines.append(f"        containerPtr->setHas{cc}(true);")
                    lines.append(f"        isSuccess &= containerPtr->get{cc}()->fromXElement(message, *it);")
                    lines.append("        isIterIncremented = true;")
                    lines.append("        ++it;")
                    lines.append("    }")
                    lines.append("")

            lines.append("    if (isIterIncremented)")
            lines.append("    {")
            lines.append("        --it;")
            lines.append("    }")
            lines.append("")
            lines.append("    return true;")
            lines.append("}\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Bespoke generator: credit
# ---------------------------------------------------------------------------
#
# The credit element has a structure not produced by any other element:
#
#   sequence
#     credit-type*    (leading optional-unbounded)
#     link*           (leading optional-unbounded)
#     bookmark*       (leading optional-unbounded)
#     choice
#       credit-image            -- single element branch
#       sequence
#         credit-words          -- always-present singleton
#         sequence min=0 max=unbounded
#           link*
#           bookmark*
#           credit-words        -- always-present singleton inside the inner pair
#
# The original codegen promoted:
#   - the outer choice to CreditChoice (enum branches = creditImage,
#     creditWords; the creditWords branch encompasses both the singleton
#     credit-words and a CreditWordsGroupSet for the inner repeating sequence).
#   - the inner unbounded sequence to CreditWordsGroup (one instance per
#     iteration; holds LinkSet, BookmarkSet, and a single CreditWordsPtr).
#
# Names below are derived from the parsed XSD content_tree so that spec
# changes (added/renamed attributes, additional leading children, additional
# group members) propagate automatically.


def _extract_credit_structure(ct):
    """Walk credit's content_tree and return (leading, single_branch,
    pair_first, group_children) where:
      - leading: list[ElementRefNode] preceding the choice
      - single_branch: ElementRefNode (the credit-image-style branch)
      - pair_first: ElementRefNode (the credit-words singleton)
      - group_children: list[ElementRefNode] inside the unbounded inner seq
    """
    tree = ct.content_tree
    assert isinstance(tree, SequenceNode)
    leading = []
    choice_node = None
    for c in tree.children:
        if isinstance(c, ChoiceNode):
            choice_node = c
            break
        if isinstance(c, ElementRefNode):
            leading.append(c)
    assert choice_node is not None, "credit: expected a choice in content_tree"

    single_branch = None
    pair_first = None
    group_children = []
    for b in choice_node.branches:
        if isinstance(b, ElementRefNode):
            single_branch = b
        elif isinstance(b, SequenceNode):
            # pair branch: first child is the singleton, second is the inner unbounded seq
            pair_first = b.children[0]
            inner = b.children[1]
            assert isinstance(inner, SequenceNode)
            group_children = [c for c in inner.children if isinstance(c, ElementRefNode)]
    assert single_branch is not None and pair_first is not None
    return leading, single_branch, pair_first, group_children


def generate_credit_choice_h(choice_class, single_branch, pair_first, group_children):
    single_cls = pascal(single_branch.element_name)   # CreditImage
    pair_cls = pascal(pair_first.element_name)        # CreditWords
    group_cls = pascal(pair_first.element_name) + "Group"  # CreditWordsGroup

    # Forward-declare order matches the original codegen: branch[0] class,
    # the synthetic group class for the pair branch, then the pair branch's
    # singleton class.
    fwds = [single_cls, group_cls, pair_cls]

    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    for fwd in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({fwd})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({choice_class})\n")
    lines.append(f"inline {choice_class}Ptr make{choice_class}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{choice_class}>();")
    lines.append("}")
    lines.append(f"\nclass {choice_class} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append("    enum class Choice")
    lines.append("    {")
    lines.append(f"        {camel(single_branch.element_name)} = 1,")
    lines.append(f"        {camel(pair_first.element_name)} = 2")
    lines.append("    };")
    lines.append(f"    {choice_class}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append("")
    lines.append("    /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {choice_class}::Choice getChoice() const;")
    lines.append(f"    void setChoice(const {choice_class}::Choice value);")
    lines.append("")
    lines.append(f"    /* _________ {single_cls} minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {single_cls}Ptr get{single_cls}() const;")
    lines.append(f"    void set{single_cls}(const {single_cls}Ptr &value);")
    lines.append("")
    lines.append(f"    /* _________ {pair_cls} minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {pair_cls}Ptr get{pair_cls}() const;")
    lines.append(f"    void set{pair_cls}(const {pair_cls}Ptr &value);")
    lines.append("")
    lines.append(f"    /* _________ {group_cls} minOccurs = 0, maxOccurs = unbounded _________ */")
    lines.append(f"    const {group_cls}Set &get{group_cls}Set() const;")
    lines.append(f"    void add{group_cls}(const {group_cls}Ptr &value);")
    lines.append(f"    void remove{group_cls}(const {group_cls}SetIterConst &value);")
    lines.append(f"    void clear{group_cls}Set();")
    lines.append(f"    {group_cls}Ptr get{group_cls}(const {group_cls}SetIterConst &setIterator) const;")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append("    Choice myChoice;")
    lines.append(f"    {single_cls}Ptr my{single_cls};")
    lines.append(f"    {pair_cls}Ptr my{pair_cls};")
    lines.append(f"    {group_cls}Set my{group_cls}Set;")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_credit_choice_cpp(choice_class, single_branch, pair_first, group_children):
    single_cls = pascal(single_branch.element_name)   # CreditImage
    pair_cls = pascal(pair_first.element_name)        # CreditWords
    group_cls = pascal(pair_first.element_name) + "Group"  # CreditWordsGroup

    # Includes match the original: union of all element classes referenced by
    # the choice + its group (incl. transitive Link/Bookmark from the group).
    inc_classes = {single_cls, pair_cls, group_cls}
    for gc in group_children:
        inc_classes.add(pascal(gc.element_name))

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{choice_class}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in sorted(inc_classes):
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    # Default branch = the pair branch (creditWords), matching original codegen.
    init_parts = [
        f"myChoice(Choice::{camel(pair_first.element_name)})",
        f"my{single_cls}(make{single_cls}())",
        f"my{pair_cls}(make{pair_cls}())",
        f"my{group_cls}Set()",
    ]
    _emit_ctor_init(lines, f"{choice_class}::{choice_class}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {choice_class}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")

    lines.append(f"std::ostream &{choice_class}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"std::ostream &{choice_class}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"bool {choice_class}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")

    lines.append(f"std::ostream &{choice_class}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    switch (myChoice)")
    lines.append("    {")
    lines.append(f"    case Choice::{camel(single_branch.element_name)}:")
    lines.append(f"        my{single_cls}->toStream(os, indentLevel);")
    lines.append("        break;")
    lines.append(f"    case Choice::{camel(pair_first.element_name)}: {{")
    lines.append(f"        my{pair_cls}->toStream(os, indentLevel);")
    lines.append("")
    lines.append(f"        if (my{group_cls}Set.size() > 0)")
    lines.append("        {")
    lines.append("            os << std::endl;")
    lines.append("        }")
    lines.append("")
    lines.append(f"        for (auto x : my{group_cls}Set)")
    lines.append("        {")
    lines.append("            x->streamContents(os, indentLevel, isOneLineOnly);")
    lines.append("        }")
    lines.append("")
    lines.append("        break;")
    lines.append("    }")
    lines.append("    default:")
    lines.append("        break;")
    lines.append("    }")
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"{choice_class}::Choice {choice_class}::getChoice() const")
    lines.append("{")
    lines.append("    return myChoice;")
    lines.append("}\n")

    lines.append(f"void {choice_class}::setChoice(const {choice_class}::Choice value)")
    lines.append("{")
    lines.append("    myChoice = value;")
    lines.append("}\n")

    for cls in (single_cls, pair_cls):
        lines.append(f"{cls}Ptr {choice_class}::get{cls}() const")
        lines.append("{")
        lines.append(f"    return my{cls};")
        lines.append("}\n")
        lines.append(f"void {choice_class}::set{cls}(const {cls}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{cls} = value;")
        lines.append("    }")
        lines.append("}\n")

    # group set accessors: order is getSet, remove, add, clear, get(setIterator)
    lines.append(f"const {group_cls}Set &{choice_class}::get{group_cls}Set() const")
    lines.append("{")
    lines.append(f"    return my{group_cls}Set;")
    lines.append("}\n")
    lines.append(f"void {choice_class}::remove{group_cls}(const {group_cls}SetIterConst &value)")
    lines.append("{")
    lines.append(f"    if (value != my{group_cls}Set.cend())")
    lines.append("    {")
    lines.append(f"        my{group_cls}Set.erase(value);")
    lines.append("    }")
    lines.append("}\n")
    lines.append(f"void {choice_class}::add{group_cls}(const {group_cls}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{group_cls}Set.push_back(value);")
    lines.append("    }")
    lines.append("}\n")
    lines.append(f"void {choice_class}::clear{group_cls}Set()")
    lines.append("{")
    lines.append(f"    my{group_cls}Set.clear();")
    lines.append("}\n")
    lines.append(f"{group_cls}Ptr {choice_class}::get{group_cls}(const {group_cls}SetIterConst &setIterator) const")
    lines.append("{")
    lines.append(f"    if (setIterator != my{group_cls}Set.cend())")
    lines.append("    {")
    lines.append("        return *setIterator;")
    lines.append("    }")
    lines.append(f"    return {group_cls}Ptr();")
    lines.append("}\n")

    lines.append(f"MX_FROM_XELEMENT_UNUSED({choice_class});\n")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_credit_words_group_h(group_cls, group_children, singleton_name):
    # group_children includes link*, bookmark*, credit-words(single)
    # singleton_name is 'credit-words' (the always-present last member)
    set_members = [c for c in group_children if c.element_name != singleton_name]
    singleton_cls = pascal(singleton_name)

    fwds = sorted({pascal(c.element_name) for c in group_children})

    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    for fwd in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({fwd})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({group_cls})\n")
    lines.append(f"inline {group_cls}Ptr make{group_cls}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{group_cls}>();")
    lines.append("}")
    lines.append(f"\nclass {group_cls} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {group_cls}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    # The committed CreditWordsGroup.h has a small inconsistency: its first
    # set's comment header is "LinkSet" but the second is "Bookmark". The
    # general project convention (matching other group headers like
    # Appearance, Barline) uses {cc} without the Set suffix; we use that
    # convention. The single residual line is an EXC documented in state.md.
    for c in set_members:
        cc = pascal(c.element_name)
        lines.append("")
        lines.append(f"    /* _________ {cc} minOccurs = 0, maxOccurs = unbounded _________ */")
        lines.append(f"    const {cc}Set &get{cc}Set() const;")
        lines.append(f"    void add{cc}(const {cc}Ptr &value);")
        lines.append(f"    void remove{cc}(const {cc}SetIterConst &value);")
        lines.append(f"    void clear{cc}Set();")
        lines.append(f"    {cc}Ptr get{cc}(const {cc}SetIterConst &setIterator) const;")
    lines.append("")
    lines.append(f"    /* _________ {singleton_cls} minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {singleton_cls}Ptr get{singleton_cls}() const;")
    lines.append(f"    void set{singleton_cls}(const {singleton_cls}Ptr &value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    for c in set_members:
        cc = pascal(c.element_name)
        lines.append(f"    {cc}Set my{cc}Set;")
    lines.append(f"    {singleton_cls}Ptr my{singleton_cls};")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_credit_words_group_cpp(group_cls, group_children, singleton_name):
    set_members = [c for c in group_children if c.element_name != singleton_name]
    singleton_cls = pascal(singleton_name)
    inc_classes = sorted({pascal(c.element_name) for c in group_children})

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{group_cls}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in inc_classes:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    init_parts = [f"my{pascal(c.element_name)}Set()" for c in set_members]
    init_parts.append(f"my{singleton_cls}(make{singleton_cls}())")
    _emit_ctor_init(lines, f"{group_cls}::{group_cls}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {group_cls}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")
    lines.append(f"std::ostream &{group_cls}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"std::ostream &{group_cls}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"bool {group_cls}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")

    # streamContents: bespoke isFirst pattern across sets, then singleton at the end
    lines.append(f"std::ostream &{group_cls}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    bool isFirst = true;")
    for c in set_members:
        cc = pascal(c.element_name)
        lines.append(f"    for (auto x : my{cc}Set)")
        lines.append("    {")
        lines.append("        if (!isFirst)")
        lines.append("        {")
        lines.append("            os << std::endl;")
        lines.append("        }")
        lines.append("        x->toStream(os, indentLevel);")
        lines.append("        isFirst = false;")
        lines.append("    }")
    lines.append("    if (!isFirst)")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append("    }")
    lines.append(f"    my{singleton_cls}->toStream(os, indentLevel);")
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}\n")

    # Per-set accessors: order is getSet, add, remove, clear, get(setIterator).
    # This matches the committed CreditWordsGroup.cpp (add-before-remove), an
    # outlier vs the rest of the codebase. See iter-33 gotcha for the analogous
    # MidiDeviceInstrumentGroupSet outlier.
    for c in set_members:
        cc = pascal(c.element_name)
        lines.append(f"const {cc}Set &{group_cls}::get{cc}Set() const")
        lines.append("{")
        lines.append(f"    return my{cc}Set;")
        lines.append("}\n")
        lines.append(f"void {group_cls}::add{cc}(const {cc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{cc}Set.push_back(value);")
        lines.append("    }")
        lines.append("}\n")
        lines.append(f"void {group_cls}::remove{cc}(const {cc}SetIterConst &value)")
        lines.append("{")
        lines.append(f"    if (value != my{cc}Set.cend())")
        lines.append("    {")
        lines.append(f"        my{cc}Set.erase(value);")
        lines.append("    }")
        lines.append("}\n")
        lines.append(f"void {group_cls}::clear{cc}Set()")
        lines.append("{")
        lines.append(f"    my{cc}Set.clear();")
        lines.append("}\n")
        lines.append(f"{cc}Ptr {group_cls}::get{cc}(const {cc}SetIterConst &setIterator) const")
        lines.append("{")
        lines.append(f"    if (setIterator != my{cc}Set.cend())")
        lines.append("    {")
        lines.append("        return *setIterator;")
        lines.append("    }")
        lines.append(f"    return {cc}Ptr();")
        lines.append("}\n")

    lines.append(f"{singleton_cls}Ptr {group_cls}::get{singleton_cls}() const")
    lines.append("{")
    lines.append(f"    return my{singleton_cls};")
    lines.append("}\n")
    lines.append(f"void {group_cls}::set{singleton_cls}(const {singleton_cls}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{singleton_cls} = value;")
    lines.append("    }")
    lines.append("}\n")

    lines.append(f"MX_FROM_XELEMENT_UNUSED({group_cls});\n")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_credit_h(class_name, leading, choice_class, attrs_name):
    fwds = sorted({pascal(c.element_name) for c in leading} | {choice_class})

    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append(f'#include "mx/core/elements/{attrs_name}.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace ezxml\n{\nclass XElementIterator;\n}")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    lines.append(f"MX_FORWARD_DECLARE_ATTRIBUTES({attrs_name})")
    for fwd in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({fwd})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({class_name})\n")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append(f"\nclass {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append(f"    {attrs_name}Ptr getAttributes() const;")
    lines.append(f"    void setAttributes(const {attrs_name}Ptr &value);")
    for c in leading:
        cc = pascal(c.element_name)
        lines.append("")
        lines.append(f"    /* _________ {cc} minOccurs = {c.min_occurs}, maxOccurs = unbounded _________ */")
        lines.append(f"    const {cc}Set &get{cc}Set() const;")
        lines.append(f"    void add{cc}(const {cc}Ptr &value);")
        lines.append(f"    void remove{cc}(const {cc}SetIterConst &value);")
        lines.append(f"    void clear{cc}Set();")
        lines.append(f"    {cc}Ptr get{cc}(const {cc}SetIterConst &setIterator) const;")
    lines.append("")
    lines.append(f"    /* _________ {choice_class} minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {choice_class}Ptr get{choice_class}() const;")
    lines.append(f"    void set{choice_class}(const {choice_class}Ptr &value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append(f"    {attrs_name}Ptr myAttributes;")
    for c in leading:
        cc = pascal(c.element_name)
        lines.append(f"    {cc}Set my{cc}Set;")
    lines.append(f"    {choice_class}Ptr my{choice_class};")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_credit_cpp(elem_name, class_name, leading, single_branch, pair_first,
                        group_children, choice_class, group_cls, attrs_name):
    single_cls = pascal(single_branch.element_name)
    pair_cls = pascal(pair_first.element_name)
    singleton_name = pair_first.element_name  # credit-words

    # Includes: leading children + single_branch + pair_first + group_cls + choice_class
    inc_classes = sorted({pascal(c.element_name) for c in leading}
                         | {single_cls, pair_cls, group_cls, choice_class})

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in inc_classes:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    init_parts = [f"myAttributes(std::make_shared<{attrs_name}>())"]
    for c in leading:
        cc = pascal(c.element_name)
        init_parts.append(f"my{cc}Set()")
    init_parts.append(f"my{choice_class}(make{choice_class}())")
    _emit_ctor_init(lines, f"{class_name}::{class_name}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return myAttributes->hasValues();")
    lines.append("}\n")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return myAttributes->toStream(os);")
    lines.append("}\n")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append(f'    os << "{elem_name}";')
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    for c in leading:
        cc = pascal(c.element_name)
        lines.append(f"    for (auto x : my{cc}Set)")
        lines.append("    {")
        lines.append("        os << std::endl;")
        lines.append("        x->toStream(os, indentLevel + 1);")
        lines.append("    }")
    lines.append("    os << std::endl;")
    lines.append(f"    my{choice_class}->streamContents(os, indentLevel + 1, isOneLineOnly);")
    lines.append("    os << std::endl;")
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}\n")

    # Attributes accessors
    lines.append(f"{attrs_name}Ptr {class_name}::getAttributes() const")
    lines.append("{")
    lines.append("    return myAttributes;")
    lines.append("}\n")
    lines.append(f"void {class_name}::setAttributes(const {attrs_name}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append("        myAttributes = value;")
    lines.append("    }")
    lines.append("}\n")

    # Set accessors for leading children: getSet, remove, add, clear, get
    for c in leading:
        cc = pascal(c.element_name)
        lines.append(f"const {cc}Set &{class_name}::get{cc}Set() const")
        lines.append("{")
        lines.append(f"    return my{cc}Set;")
        lines.append("}\n")
        lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
        lines.append("{")
        lines.append(f"    if (value != my{cc}Set.cend())")
        lines.append("    {")
        lines.append(f"        my{cc}Set.erase(value);")
        lines.append("    }")
        lines.append("}\n")
        lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{cc}Set.push_back(value);")
        lines.append("    }")
        lines.append("}\n")
        lines.append(f"void {class_name}::clear{cc}Set()")
        lines.append("{")
        lines.append(f"    my{cc}Set.clear();")
        lines.append("}\n")
        lines.append(f"{cc}Ptr {class_name}::get{cc}(const {cc}SetIterConst &setIterator) const")
        lines.append("{")
        lines.append(f"    if (setIterator != my{cc}Set.cend())")
        lines.append("    {")
        lines.append("        return *setIterator;")
        lines.append("    }")
        lines.append(f"    return {cc}Ptr();")
        lines.append("}\n")

    # Choice accessors
    lines.append(f"{choice_class}Ptr {class_name}::get{choice_class}() const")
    lines.append("{")
    lines.append(f"    return my{choice_class};")
    lines.append("}\n")
    lines.append(f"void {class_name}::set{choice_class}(const {choice_class}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{choice_class} = value;")
    lines.append("    }")
    lines.append("}\n")

    # MX_CREDIT_RETURN_IF_END macro and fromXElementImpl
    # All names here are derived from the parsed children so spec changes
    # (e.g. a renamed choice element) will propagate.
    single_name = single_branch.element_name  # e.g. credit-image
    macro_name = "MX_CREDIT_RETURN_IF_END"
    found_flag = f"is{single_cls}Or{pair_cls}Found"
    err_msg = (f"\"{class_name}: neither '{singleton_name}' nor '{single_name}' was present"
               f" - one of these is required\"")
    lines.append(f"#ifndef {macro_name}")
    lines.append(f"#define {macro_name}                                                                                        \\")
    lines.append("    if (i == endIter)                                                                                                  \\")
    lines.append("    {                                                                                                                  \\")
    lines.append(f"        if (!{found_flag})                                                                          \\")
    lines.append("        {                                                                                                              \\")
    lines.append(f"            message << {err_msg}      \\")
    lines.append("                    << std::endl;                                                                                      \\")
    lines.append("            isSuccess = false;                                                                                         \\")
    lines.append("        }                                                                                                              \\")
    lines.append("        return isSuccess;                                                                                              \\")
    lines.append("    }")
    lines.append("#endif")
    lines.append("")

    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    lines.append("    isSuccess &= myAttributes->fromXElement(message, xelement);")
    lines.append(f"    bool {found_flag} = false;")
    lines.append("")
    lines.append("    ::ezxml::XElementIterator i = xelement.begin();")
    lines.append("    ::ezxml::XElementIterator endIter = xelement.end();")
    lines.append(f"    {macro_name};")
    lines.append("")
    for c in leading:
        cc = pascal(c.element_name)
        cname = c.element_name
        lines.append(f'    if (i->getName() == "{cname}")')
        lines.append("    {")
        lines.append(f'        while (i != endIter && i->getName() == "{cname}")')
        lines.append("        {")
        lines.append(f"            auto item = make{cc}();")
        lines.append("            isSuccess &= item->fromXElement(message, *i);")
        lines.append(f"            add{cc}(item);")
        lines.append("            ++i;")
        lines.append("        }")
        lines.append("    }")
        lines.append(f"    {macro_name};")
        lines.append("")
    lines.append(f'    if (i->getName() == "{single_name}" || i->getName() == "{singleton_name}")')
    lines.append("    {")
    lines.append(f"        {found_flag} = true;")
    lines.append(f'        if (i->getName() == "{single_name}")')
    lines.append("        {")
    lines.append(f"            my{choice_class}->setChoice({choice_class}::Choice::{camel(single_name)});")
    lines.append(f"            isSuccess &= my{choice_class}->get{single_cls}()->fromXElement(message, *i);")
    lines.append("            MX_RETURN_IS_SUCCESS;")
    lines.append("        }")
    lines.append("")
    lines.append(f'        if (i->getName() == "{singleton_name}")')
    lines.append("        {")
    lines.append(f"            my{choice_class}->setChoice({choice_class}::Choice::{camel(singleton_name)});")
    lines.append(f"            isSuccess &= my{choice_class}->get{pair_cls}()->fromXElement(message, *i);")
    lines.append("            ++i;")
    lines.append(f"            {macro_name};")
    lines.append("")
    set_members = [c for c in group_children if c.element_name != singleton_name]
    set_names_or = " || ".join(f'(i->getName() == "{c.element_name}")' for c in set_members)
    set_names_or += f' || (i->getName() == "{singleton_name}")'
    lines.append(f"            auto creditWordsGroup = make{group_cls}();")
    lines.append(f"            while (i != endIter &&")
    lines.append(f"                   ({set_names_or}))")
    lines.append("            {")
    for c in set_members:
        cc = pascal(c.element_name)
        cname = c.element_name
        local = camel(c.element_name)
        lines.append(f'                while (i != endIter && i->getName() == "{cname}")')
        lines.append("                {")
        lines.append(f"                    auto {local} = make{cc}();")
        lines.append(f"                    isSuccess &= {local}->fromXElement(message, xelement);")
        lines.append(f"                    creditWordsGroup->add{cc}({local});")
        lines.append("                    ++i;")
        lines.append("                }")
        lines.append(f"                {macro_name};")
        lines.append("")
    lines.append(f'                if (i->getName() == "{singleton_name}")')
    lines.append("                {")
    lines.append(f"                    isSuccess &= creditWordsGroup->get{pair_cls}()->fromXElement(message, *i);")
    lines.append(f"                    my{choice_class}->add{group_cls}(creditWordsGroup);")
    lines.append(f"                    creditWordsGroup = make{group_cls}();")
    lines.append("                    ++i;")
    lines.append("                }")
    lines.append(f"                {macro_name};")
    lines.append("            }")
    lines.append("        }")
    lines.append("    }")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def _emit_credit_family(elem_name, elem, ct, model, generated_attrs, stats):
    leading, single_branch, pair_first, group_children = _extract_credit_structure(ct)

    class_name = element_class_name(elem_name)
    choice_class = pascal(elem_name) + "Choice"             # CreditChoice
    group_cls = pascal(pair_first.element_name) + "Group"   # CreditWordsGroup
    type_name = elem.type_name or ""

    # 1. Attrs struct via the standard generator
    if ct.attributes:
        attrs_name = resolve_attrs_name(elem_name, type_name, model)
        if attrs_name not in generated_attrs and attrs_name not in CORE_ROOT_ATTRS:
            h = generate_attrs_h(attrs_name, ct.attributes, model)
            c = generate_attrs_cpp(attrs_name, ct.attributes, model)
            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.h"), h)
            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.cpp"), c)
            generated_attrs.add(attrs_name)
            stats["attrs_written"] += 1
    else:
        attrs_name = None

    # 2. CreditChoice
    h = generate_credit_choice_h(choice_class, single_branch, pair_first, group_children)
    c = generate_credit_choice_cpp(choice_class, single_branch, pair_first, group_children)
    write_file(os.path.join(ELEM_DIR, f"{choice_class}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{choice_class}.cpp"), c)

    # 3. CreditWordsGroup
    h = generate_credit_words_group_h(group_cls, group_children, pair_first.element_name)
    c = generate_credit_words_group_cpp(group_cls, group_children, pair_first.element_name)
    write_file(os.path.join(ELEM_DIR, f"{group_cls}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{group_cls}.cpp"), c)

    # 4. Credit
    h = generate_credit_h(class_name, leading, choice_class, attrs_name)
    c = generate_credit_cpp(elem_name, class_name, leading, single_branch, pair_first,
                            group_children, choice_class, group_cls, attrs_name)
    write_file(os.path.join(ELEM_DIR, f"{class_name}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{class_name}.cpp"), c)

    stats["elem_written"] += 1
    stats["bespoke_written"] = stats.get("bespoke_written", 0) + 1


# ---------------------------------------------------------------------------
# Lyric (bespoke, schema-driven)
#
# Lyric's complex type is structured as:
#   sequence
#     choice:
#       sequence:  syllabic? text (sequence)*[unbounded] extend?
#                  inner sequence: (sequence?: elision syllabic?) text
#       extend
#       laughing
#       humming
#     end-line?
#     end-paragraph?
#     group ref editorial
#
# This produces four hand-written-shaped classes in the original codegen:
#   - LyricTextChoice         (the outer required choice)
#   - SyllabicTextGroup       (the sequence branch promoted to a group)
#   - ElisionSyllabicTextGroup(the unbounded inner sequence promoted to a group)
#   - ElisionSyllabicGroup    (the optional inner sub-sequence promoted to a group)
#
# All names that come from the XSD (element refs, attribute refs, etc.) are
# read from the parsed model so that spec changes propagate automatically.
# The class names of the synthetic groups are derived from the names of the
# first two children of each synthetic group plus a "Group" suffix.


def _synth_group_name_from_children(children):
    """Compose a synthetic group name from the first two children, then
    append 'Group'. For an ElementRefNode child the stem is
    pascal(element_name); for an already-named synthetic group child whose
    name ends with 'Group' the stem is the name with 'Group' stripped.
    """
    stems = []
    for c in children[:2]:
        if isinstance(c, ElementRefNode):
            stems.append(pascal(c.element_name))
        elif isinstance(c, str):
            stems.append(c[:-5] if c.endswith("Group") else c)
        else:
            raise AssertionError(f"unexpected child for group naming: {c!r}")
    return "".join(stems) + "Group"


def _extract_lyric_structure(ct):
    """Walk lyric's content_tree and return a dict describing the structure.

    Returns a dict with these keys (all element references are
    ElementRefNode pulled directly from the parsed XSD):
      - choice:                ChoiceNode (the required outer choice)
      - seq_branch_pre:        elements in the sequence branch before the
                               inner unbounded sequence (e.g. syllabic?, text)
      - seq_branch_post:       elements after the inner unbounded sequence
                               (e.g. extend?)
      - inner_seq:             the unbounded SequenceNode inside the seq branch
      - inner_text:            ElementRefNode for the text inside inner_seq
      - inner_optional_seq:    the optional SequenceNode inside inner_seq
      - inner_optional_elems:  elements inside the optional sub-sequence
                               (e.g. elision, syllabic?)
      - singleton_branches:    other branches of the choice (extend, laughing,
                               humming) as a list of ElementRefNode
      - trailing_elems:        ElementRefNodes after the choice (end-line?,
                               end-paragraph?)
      - editorial_group:       GroupRefNode for the trailing group ref
    """
    tree = ct.content_tree
    assert isinstance(tree, SequenceNode)

    choice = None
    trailing_elems = []
    editorial_group = None
    for c in tree.children:
        if isinstance(c, ChoiceNode):
            assert choice is None
            choice = c
        elif isinstance(c, ElementRefNode):
            trailing_elems.append(c)
        elif isinstance(c, GroupRefNode):
            editorial_group = c
        else:
            raise AssertionError(f"unexpected top-level child: {type(c).__name__}")
    assert choice is not None

    seq_branch = None
    singleton_branches = []
    for b in choice.branches:
        if isinstance(b, SequenceNode):
            assert seq_branch is None
            seq_branch = b
        elif isinstance(b, ElementRefNode):
            singleton_branches.append(b)
        else:
            raise AssertionError(f"unexpected choice branch: {type(b).__name__}")
    assert seq_branch is not None

    seq_branch_pre = []
    inner_seq = None
    seq_branch_post = []
    for c in seq_branch.children:
        if isinstance(c, SequenceNode):
            assert inner_seq is None
            inner_seq = c
        elif isinstance(c, ElementRefNode):
            if inner_seq is None:
                seq_branch_pre.append(c)
            else:
                seq_branch_post.append(c)
        else:
            raise AssertionError(f"unexpected seq branch child: {type(c).__name__}")
    assert inner_seq is not None

    inner_optional_seq = None
    inner_text = None
    for c in inner_seq.children:
        if isinstance(c, SequenceNode):
            assert inner_optional_seq is None
            inner_optional_seq = c
        elif isinstance(c, ElementRefNode):
            assert inner_text is None
            inner_text = c
        else:
            raise AssertionError(f"unexpected inner seq child: {type(c).__name__}")
    assert inner_optional_seq is not None and inner_text is not None

    inner_optional_elems = []
    for c in inner_optional_seq.children:
        assert isinstance(c, ElementRefNode)
        inner_optional_elems.append(c)

    return {
        "choice": choice,
        "seq_branch_pre": seq_branch_pre,
        "seq_branch_post": seq_branch_post,
        "inner_seq": inner_seq,
        "inner_text": inner_text,
        "inner_optional_seq": inner_optional_seq,
        "inner_optional_elems": inner_optional_elems,
        "singleton_branches": singleton_branches,
        "trailing_elems": trailing_elems,
        "editorial_group": editorial_group,
    }


def generate_elision_syllabic_group_h(group_cls, elems):
    """elems = [required_elision, optional_syllabic]"""
    fwds = sorted({pascal(e.element_name) for e in elems})

    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    for f in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({f})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({group_cls})\n")
    lines.append(f"inline {group_cls}Ptr make{group_cls}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{group_cls}>();")
    lines.append("}")
    lines.append(f"\nclass {group_cls} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {group_cls}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    for e in elems:
        cc = pascal(e.element_name)
        optional = e.min_occurs == 0
        lines.append("")
        lines.append(f"    /* _________ {cc} minOccurs = {e.min_occurs}, maxOccurs = {e.max_occurs} _________ */")
        lines.append(f"    {cc}Ptr get{cc}() const;")
        lines.append(f"    void set{cc}(const {cc}Ptr &value);")
        if optional:
            lines.append(f"    bool getHas{cc}() const;")
            lines.append(f"    void setHas{cc}(const bool value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    for e in elems:
        cc = pascal(e.element_name)
        optional = e.min_occurs == 0
        lines.append(f"    {cc}Ptr my{cc};")
        if optional:
            lines.append(f"    bool myHas{cc};")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_elision_syllabic_group_cpp(group_cls, elems):
    inc_classes = sorted({pascal(e.element_name) for e in elems})

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{group_cls}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in inc_classes:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    init_parts = []
    for e in elems:
        cc = pascal(e.element_name)
        init_parts.append(f"my{cc}(make{cc}())")
        if e.min_occurs == 0:
            init_parts.append(f"myHas{cc}(false)")
    _emit_ctor_init(lines, f"{group_cls}::{group_cls}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {group_cls}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")
    lines.append(f"std::ostream &{group_cls}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"std::ostream &{group_cls}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"bool {group_cls}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")

    # streamContents: required elements emitted plainly; optional elements
    # emitted only when their HasX flag is true, and each optional emission
    # flips isOneLineOnly to false and emits a leading newline.
    lines.append(f"std::ostream &{group_cls}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    isOneLineOnly = true;")
    for e in elems:
        cc = pascal(e.element_name)
        if e.min_occurs == 0:
            lines.append(f"    if (myHas{cc})")
            lines.append("    {")
            lines.append("        isOneLineOnly = false;")
            lines.append("        os << std::endl;")
            lines.append(f"        my{cc}->toStream(os, indentLevel);")
            lines.append("    }")
        else:
            lines.append(f"    my{cc}->toStream(os, indentLevel);")
    lines.append("    return os;")
    lines.append("}\n")

    for e in elems:
        cc = pascal(e.element_name)
        lines.append(f"{cc}Ptr {group_cls}::get{cc}() const")
        lines.append("{")
        lines.append(f"    return my{cc};")
        lines.append("}\n")
        lines.append(f"void {group_cls}::set{cc}(const {cc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{cc} = value;")
        lines.append("    }")
        lines.append("}\n")
        if e.min_occurs == 0:
            lines.append(f"bool {group_cls}::getHas{cc}() const")
            lines.append("{")
            lines.append(f"    return myHas{cc};")
            lines.append("}\n")
            lines.append(f"void {group_cls}::setHas{cc}(const bool value)")
            lines.append("{")
            lines.append(f"    myHas{cc} = value;")
            lines.append("}\n")

    lines.append(f"MX_FROM_XELEMENT_UNUSED({group_cls});\n")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_elision_syllabic_text_group_h(group_cls, inner_group_cls, text_elem):
    """ElisionSyllabicTextGroup has optional inner group + required text."""
    text_cls = pascal(text_elem.element_name)
    fwds = sorted({inner_group_cls, text_cls})

    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    for f in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({f})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({group_cls})\n")
    lines.append(f"inline {group_cls}Ptr make{group_cls}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{group_cls}>();")
    lines.append("}")
    lines.append(f"\nclass {group_cls} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {group_cls}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append("")
    lines.append(f"    /* _________ {inner_group_cls} minOccurs = 0, maxOccurs = 1 _________ */")
    lines.append(f"    {inner_group_cls}Ptr get{inner_group_cls}() const;")
    lines.append(f"    void set{inner_group_cls}(const {inner_group_cls}Ptr &value);")
    lines.append(f"    bool getHas{inner_group_cls}() const;")
    lines.append(f"    void setHas{inner_group_cls}(const bool value);")
    lines.append("")
    lines.append(f"    /* _________ {text_cls} minOccurs = {text_elem.min_occurs}, maxOccurs = {text_elem.max_occurs} _________ */")
    lines.append(f"    {text_cls}Ptr get{text_cls}() const;")
    lines.append(f"    void set{text_cls}(const {text_cls}Ptr &value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append(f"    {inner_group_cls}Ptr my{inner_group_cls};")
    lines.append(f"    bool myHas{inner_group_cls};")
    lines.append(f"    {text_cls}Ptr my{text_cls};")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_elision_syllabic_text_group_cpp(group_cls, inner_group_cls, text_elem):
    text_cls = pascal(text_elem.element_name)
    inc_classes = sorted({inner_group_cls, text_cls})

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{group_cls}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in inc_classes:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    init_parts = [
        f"my{inner_group_cls}(make{inner_group_cls}())",
        f"myHas{inner_group_cls}(false)",
        f"my{text_cls}(make{text_cls}())",
    ]
    _emit_ctor_init(lines, f"{group_cls}::{group_cls}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {group_cls}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")
    lines.append(f"std::ostream &{group_cls}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"std::ostream &{group_cls}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"bool {group_cls}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")

    # streamContents: optional inner group, then required text.
    # The committed file streams the inner group via streamContents (not
    # toStream) because the inner group represents a flattened sub-sequence,
    # not a self-contained XML element.
    lines.append(f"std::ostream &{group_cls}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    isOneLineOnly = true;")
    lines.append(f"    if (myHas{inner_group_cls})")
    lines.append("    {")
    lines.append(f"        my{inner_group_cls}->streamContents(os, indentLevel, isOneLineOnly);")
    lines.append("        isOneLineOnly = false;")
    lines.append("        os << std::endl;")
    lines.append("    }")
    lines.append(f"    my{text_cls}->toStream(os, indentLevel);")
    lines.append("    return os;")
    lines.append("}\n")

    # Accessors: inner group (optional), then text (required)
    lines.append(f"{inner_group_cls}Ptr {group_cls}::get{inner_group_cls}() const")
    lines.append("{")
    lines.append(f"    return my{inner_group_cls};")
    lines.append("}\n")
    lines.append(f"void {group_cls}::set{inner_group_cls}(const {inner_group_cls}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{inner_group_cls} = value;")
    lines.append("    }")
    lines.append("}\n")
    lines.append(f"bool {group_cls}::getHas{inner_group_cls}() const")
    lines.append("{")
    lines.append(f"    return myHas{inner_group_cls};")
    lines.append("}\n")
    lines.append(f"void {group_cls}::setHas{inner_group_cls}(const bool value)")
    lines.append("{")
    lines.append(f"    myHas{inner_group_cls} = value;")
    lines.append("}\n")
    lines.append(f"{text_cls}Ptr {group_cls}::get{text_cls}() const")
    lines.append("{")
    lines.append(f"    return my{text_cls};")
    lines.append("}\n")
    lines.append(f"void {group_cls}::set{text_cls}(const {text_cls}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{text_cls} = value;")
    lines.append("    }")
    lines.append("}\n")

    lines.append(f"MX_FROM_XELEMENT_UNUSED({group_cls});\n")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_syllabic_text_group_h(group_cls, pre_elems, inner_group_cls,
                                   inner_set_name, post_elems):
    """SyllabicTextGroup. pre_elems = [syllabic?, text]; inner_group_cls =
    'ElisionSyllabicTextGroup' (used as a set); post_elems = [extend?]."""
    elem_classes = {pascal(e.element_name) for e in pre_elems + post_elems}
    fwds = sorted(elem_classes | {inner_group_cls})

    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    for f in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({f})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({group_cls})\n")
    lines.append(f"inline {group_cls}Ptr make{group_cls}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{group_cls}>();")
    lines.append("}")
    lines.append(f"\nclass {group_cls} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {group_cls}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")

    for e in pre_elems:
        cc = pascal(e.element_name)
        optional = e.min_occurs == 0
        lines.append("")
        lines.append(f"    /* _________ {cc} minOccurs = {e.min_occurs}, maxOccurs = {e.max_occurs} _________ */")
        lines.append(f"    {cc}Ptr get{cc}() const;")
        lines.append(f"    void set{cc}(const {cc}Ptr &value);")
        if optional:
            lines.append(f"    bool getHas{cc}() const;")
            lines.append(f"    void setHas{cc}(const bool value);")

    # ElisionSyllabicTextGroup* (unbounded set)
    lines.append("")
    lines.append(f"    /* _________ {inner_group_cls} minOccurs = 0, maxOccurs = unbounded _________ */")
    lines.append(f"    const {inner_group_cls}Set &get{inner_group_cls}Set() const;")
    lines.append(f"    void add{inner_group_cls}(const {inner_group_cls}Ptr &value);")
    lines.append(f"    void remove{inner_group_cls}(const {inner_group_cls}SetIterConst &value);")
    lines.append(f"    void clear{inner_group_cls}Set();")
    lines.append(f"    {inner_group_cls}Ptr get{inner_group_cls}(")
    lines.append(f"        const {inner_group_cls}SetIterConst &setIterator) const;")

    for e in post_elems:
        cc = pascal(e.element_name)
        optional = e.min_occurs == 0
        lines.append("")
        lines.append(f"    /* _________ {cc} minOccurs = {e.min_occurs}, maxOccurs = {e.max_occurs} _________ */")
        lines.append(f"    {cc}Ptr get{cc}() const;")
        lines.append(f"    void set{cc}(const {cc}Ptr &value);")
        if optional:
            lines.append(f"    bool getHas{cc}() const;")
            lines.append(f"    void setHas{cc}(const bool value);")

    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    for e in pre_elems:
        cc = pascal(e.element_name)
        lines.append(f"    {cc}Ptr my{cc};")
        if e.min_occurs == 0:
            lines.append(f"    bool myHas{cc};")
    lines.append(f"    {inner_group_cls}Set my{inner_group_cls}Set;")
    for e in post_elems:
        cc = pascal(e.element_name)
        lines.append(f"    {cc}Ptr my{cc};")
        if e.min_occurs == 0:
            lines.append(f"    bool myHas{cc};")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_syllabic_text_group_cpp(group_cls, pre_elems, inner_group_cls,
                                     post_elems):
    elem_classes = sorted({pascal(e.element_name) for e in pre_elems + post_elems}
                          | {inner_group_cls})

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{group_cls}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in elem_classes:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    init_parts = []
    for e in pre_elems:
        cc = pascal(e.element_name)
        init_parts.append(f"my{cc}(make{cc}())")
        if e.min_occurs == 0:
            init_parts.append(f"myHas{cc}(false)")
    init_parts.append(f"my{inner_group_cls}Set()")
    for e in post_elems:
        cc = pascal(e.element_name)
        init_parts.append(f"my{cc}(make{cc}())")
        if e.min_occurs == 0:
            init_parts.append(f"myHas{cc}(false)")
    _emit_ctor_init(lines, f"{group_cls}::{group_cls}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {group_cls}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")
    lines.append(f"std::ostream &{group_cls}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"std::ostream &{group_cls}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"bool {group_cls}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")

    # streamContents: pre_elems (in order; optional ones emit endl AFTER if
    # required text follows; the committed pattern emits the optional
    # syllabic first then endl, then text). For maximal fidelity we model:
    #   if optional: emit elem, endl
    #   if required: emit elem (no surrounding endl)
    # The set iterates with leading endl per element, calling streamContents.
    # Trailing optional elements emit leading endl then elem.
    lines.append(f"std::ostream &{group_cls}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    isOneLineOnly = true;")
    for e in pre_elems:
        cc = pascal(e.element_name)
        if e.min_occurs == 0:
            lines.append(f"    if (myHas{cc})")
            lines.append("    {")
            lines.append(f"        my{cc}->toStream(os, indentLevel);")
            lines.append("        os << std::endl;")
            lines.append("    }")
        else:
            lines.append(f"    my{cc}->toStream(os, indentLevel);")
    lines.append(f"    for (auto x : my{inner_group_cls}Set)")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append("        x->streamContents(os, indentLevel, isOneLineOnly);")
    lines.append("    }")
    for e in post_elems:
        cc = pascal(e.element_name)
        if e.min_occurs == 0:
            lines.append(f"    if (myHas{cc})")
            lines.append("    {")
            lines.append("        os << std::endl;")
            lines.append(f"        my{cc}->toStream(os, indentLevel);")
            lines.append("    }")
        else:
            lines.append(f"    my{cc}->toStream(os, indentLevel);")

    # final isOneLineOnly: true only if NO optional pre/post and NO set entries
    cond_parts = []
    for e in pre_elems:
        if e.min_occurs == 0:
            cond_parts.append(f"myHas{pascal(e.element_name)}")
    cond_parts.append(f"my{inner_group_cls}Set.size() > 0")
    for e in post_elems:
        if e.min_occurs == 0:
            cond_parts.append(f"myHas{pascal(e.element_name)}")
    lines.append(f"    isOneLineOnly = !({' || '.join(cond_parts)});")
    lines.append("    return os;")
    lines.append("}\n")

    # Accessor blocks, in declaration order. For each element: get, set,
    # then if optional also getHas, setHas. For the set: getSet, add, remove,
    # clear, get-by-iter (add-before-remove pattern, matching
    # CreditWordsGroup / MidiDeviceInstrumentGroup).
    def emit_single_accessors(e):
        cc = pascal(e.element_name)
        optional = e.min_occurs == 0
        lines.append(f"{cc}Ptr {group_cls}::get{cc}() const")
        lines.append("{")
        lines.append(f"    return my{cc};")
        lines.append("}\n")
        lines.append(f"void {group_cls}::set{cc}(const {cc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{cc} = value;")
        lines.append("    }")
        lines.append("}\n")
        if optional:
            lines.append(f"bool {group_cls}::getHas{cc}() const")
            lines.append("{")
            lines.append(f"    return myHas{cc};")
            lines.append("}\n")
            lines.append(f"void {group_cls}::setHas{cc}(const bool value)")
            lines.append("{")
            lines.append(f"    myHas{cc} = value;")
            lines.append("}\n")

    for e in pre_elems:
        emit_single_accessors(e)

    cc = inner_group_cls
    lines.append(f"const {cc}Set &{group_cls}::get{cc}Set() const")
    lines.append("{")
    lines.append(f"    return my{cc}Set;")
    lines.append("}\n")
    lines.append(f"void {group_cls}::add{cc}(const {cc}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{cc}Set.push_back(value);")
    lines.append("    }")
    lines.append("}\n")
    lines.append(f"void {group_cls}::remove{cc}(const {cc}SetIterConst &value)")
    lines.append("{")
    lines.append(f"    if (value != my{cc}Set.cend())")
    lines.append("    {")
    lines.append(f"        my{cc}Set.erase(value);")
    lines.append("    }")
    lines.append("}\n")
    lines.append(f"void {group_cls}::clear{cc}Set()")
    lines.append("{")
    lines.append(f"    my{cc}Set.clear();")
    lines.append("}\n")
    lines.append(f"{cc}Ptr {group_cls}::get{cc}(const {cc}SetIterConst &setIterator) const")
    lines.append("{")
    lines.append(f"    if (setIterator != my{cc}Set.cend())")
    lines.append("    {")
    lines.append("        return *setIterator;")
    lines.append("    }")
    lines.append(f"    return {cc}Ptr();")
    lines.append("}\n")

    for e in post_elems:
        emit_single_accessors(e)

    lines.append(f"MX_FROM_XELEMENT_UNUSED({group_cls});\n")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_lyric_text_choice_h(choice_class, seq_group_cls, singleton_branches):
    """choice_class = 'LyricTextChoice'; seq_group_cls = 'SyllabicTextGroup';
    singleton_branches = list of ElementRefNode (extend, laughing, humming)."""
    classes = [seq_group_cls] + [pascal(b.element_name) for b in singleton_branches]
    fwds = sorted(classes)

    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    for f in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({f})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({choice_class})\n")
    lines.append(f"inline {choice_class}Ptr make{choice_class}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{choice_class}>();")
    lines.append("}")
    lines.append(f"\nclass {choice_class} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append("    enum class Choice")
    lines.append("    {")
    # First enum: the sequence-branch synthetic group. Then singletons in branch order.
    seq_group_camel = camel(seq_group_cls[:1].lower() + seq_group_cls[1:])
    # the seq group's identifier is just camel of the class name's first segment
    # but committed uses 'syllabicTextGroup' = camelCase of the class name.
    seq_group_choice_enum = seq_group_cls[:1].lower() + seq_group_cls[1:]
    enum_lines = [f"        {seq_group_choice_enum} = 1"]
    for i, b in enumerate(singleton_branches, start=2):
        enum_lines.append(f"        {camel(b.element_name)} = {i}")
    lines.append(",\n".join(enum_lines))
    lines.append("    };")
    lines.append(f"    {choice_class}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append(f"    {choice_class}::Choice getChoice() const;")
    lines.append(f"    void setChoice(const {choice_class}::Choice value);")
    lines.append("")
    lines.append(f"    /* _________ {seq_group_cls} minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {seq_group_cls}Ptr get{seq_group_cls}() const;")
    lines.append(f"    void set{seq_group_cls}(const {seq_group_cls}Ptr &value);")
    for b in singleton_branches:
        cc = pascal(b.element_name)
        lines.append("")
        lines.append(f"    /* _________ {cc} minOccurs = 1, maxOccurs = 1 _________ */")
        lines.append(f"    {cc}Ptr get{cc}() const;")
        lines.append(f"    void set{cc}(const {cc}Ptr &value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append("    Choice myChoice;")
    lines.append(f"    {seq_group_cls}Ptr my{seq_group_cls};")
    for b in singleton_branches:
        cc = pascal(b.element_name)
        lines.append(f"    {cc}Ptr my{cc};")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_lyric_text_choice_cpp(choice_class, seq_group_cls, singleton_branches):
    inc_classes = sorted({seq_group_cls} | {pascal(b.element_name) for b in singleton_branches})

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{choice_class}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in inc_classes:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    seq_group_choice_enum = seq_group_cls[:1].lower() + seq_group_cls[1:]
    init_parts = [
        f"myChoice(Choice::{seq_group_choice_enum})",
        f"my{seq_group_cls}(make{seq_group_cls}())",
    ]
    for b in singleton_branches:
        cc = pascal(b.element_name)
        init_parts.append(f"my{cc}(make{cc}())")
    _emit_ctor_init(lines, f"{choice_class}::{choice_class}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {choice_class}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")
    lines.append(f"std::ostream &{choice_class}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"std::ostream &{choice_class}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    # hasContents: returns true ONLY when choice is the sequence branch.
    # This is a committed quirk: the singletons emit via toStream which
    # write their own XML wrapping, so the parent treats the choice as
    # "contentless" in those cases. See state.md gotcha.
    lines.append(f"bool {choice_class}::hasContents() const")
    lines.append("{")
    lines.append(f"    return myChoice == Choice::{seq_group_choice_enum};")
    lines.append("}\n")

    # streamContents: switch on choice; sequence branch uses streamContents,
    # singletons use toStream.
    lines.append(f"std::ostream &{choice_class}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    switch (myChoice)")
    lines.append("    {")
    lines.append(f"    case Choice::{seq_group_choice_enum}: {{")
    lines.append(f"        my{seq_group_cls}->streamContents(os, indentLevel, isOneLineOnly);")
    lines.append("    }")
    lines.append("    break;")
    for b in singleton_branches:
        cc = pascal(b.element_name)
        lines.append(f"    case Choice::{camel(b.element_name)}: {{")
        lines.append(f"        my{cc}->toStream(os, indentLevel);")
        lines.append("    }")
        lines.append("    break;")
    lines.append("    default:")
    lines.append("        break;")
    lines.append("    }")
    lines.append("    return os;")
    lines.append("}\n")

    lines.append(f"{choice_class}::Choice {choice_class}::getChoice() const")
    lines.append("{")
    lines.append("    return myChoice;")
    lines.append("}\n")
    lines.append(f"void {choice_class}::setChoice(const {choice_class}::Choice value)")
    lines.append("{")
    lines.append("    myChoice = value;")
    lines.append("}\n")

    # Accessors per member, in declaration order. For each: get, set (no
    # 'has' since each member is always present in the choice).
    members = [seq_group_cls] + [pascal(b.element_name) for b in singleton_branches]
    for cc in members:
        lines.append(f"{cc}Ptr {choice_class}::get{cc}() const")
        lines.append("{")
        lines.append(f"    return my{cc};")
        lines.append("}\n")
        lines.append(f"void {choice_class}::set{cc}(const {cc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{cc} = value;")
        lines.append("    }")
        lines.append("}\n")

    lines.append(f"MX_FROM_XELEMENT_UNUSED({choice_class});\n")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_lyric_h(class_name, attrs_name, choice_class, trailing_elems,
                     editorial_group_cls):
    fwds = sorted({choice_class, editorial_group_cls}
                  | {pascal(e.element_name) for e in trailing_elems})

    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append(f'#include "mx/core/elements/{attrs_name}.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    lines.append(f"MX_FORWARD_DECLARE_ATTRIBUTES({attrs_name})")
    for f in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({f})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({class_name})\n")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append(f"\nclass {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append(f"    {attrs_name}Ptr getAttributes() const;")
    lines.append(f"    void setAttributes(const {attrs_name}Ptr &value);")
    lines.append("")
    lines.append(f"    /* _________ {choice_class} minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {choice_class}Ptr get{choice_class}() const;")
    lines.append(f"    void set{choice_class}(const {choice_class}Ptr &value);")
    for e in trailing_elems:
        cc = pascal(e.element_name)
        lines.append("")
        lines.append(f"    /* _________ {cc} minOccurs = {e.min_occurs}, maxOccurs = {e.max_occurs} _________ */")
        lines.append(f"    {cc}Ptr get{cc}() const;")
        lines.append(f"    void set{cc}(const {cc}Ptr &value);")
        if e.min_occurs == 0:
            lines.append(f"    bool getHas{cc}() const;")
            lines.append(f"    void setHas{cc}(const bool value);")
    lines.append("")
    lines.append(f"    /* _________ {editorial_group_cls} minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {editorial_group_cls}Ptr get{editorial_group_cls}() const;")
    lines.append(f"    void set{editorial_group_cls}(const {editorial_group_cls}Ptr &value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append(f"    {attrs_name}Ptr myAttributes;")
    lines.append(f"    {choice_class}Ptr my{choice_class};")
    for e in trailing_elems:
        cc = pascal(e.element_name)
        lines.append(f"    {cc}Ptr my{cc};")
        if e.min_occurs == 0:
            lines.append(f"    bool myHas{cc};")
    lines.append(f"    {editorial_group_cls}Ptr my{editorial_group_cls};")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_lyric_cpp(elem_name, class_name, attrs_name, choice_class,
                       seq_group_cls, singleton_branches, trailing_elems,
                       editorial_group_cls, seq_branch_starters):
    # Includes: choice class, editorial group, trailing elems, plus the
    # singletons + seq-group because fromXElement uses them.
    inc_classes = sorted({choice_class, editorial_group_cls, seq_group_cls}
                         | {pascal(b.element_name) for b in singleton_branches}
                         | {pascal(e.element_name) for e in trailing_elems})

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in inc_classes:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    init_parts = [
        f"myAttributes(std::make_shared<{attrs_name}>())",
        f"my{choice_class}(make{choice_class}())",
    ]
    for e in trailing_elems:
        cc = pascal(e.element_name)
        init_parts.append(f"my{cc}(make{cc}())")
        if e.min_occurs == 0:
            init_parts.append(f"myHas{cc}(false)")
    init_parts.append(f"my{editorial_group_cls}(make{editorial_group_cls}())")
    _emit_ctor_init(lines, f"{class_name}::{class_name}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return myAttributes->hasValues();")
    lines.append("}\n")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    if (myAttributes->hasValues())")
    lines.append("    {")
    lines.append("        myAttributes->toStream(os);")
    lines.append("    }")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append(f'    return os << "{elem_name}";')
    lines.append("}\n")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")

    # Attribute accessors before streamContents (matches committed Lyric.cpp).
    lines.append(f"{attrs_name}Ptr {class_name}::getAttributes() const")
    lines.append("{")
    lines.append("    return myAttributes;")
    lines.append("}\n")
    lines.append(f"void {class_name}::setAttributes(const {attrs_name}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append("        myAttributes = value;")
    lines.append("    }")
    lines.append("}\n")

    # streamContents: leading endl, choice streamContents, trailing optional
    # elements (each guarded by has-flag), then editorial group (with leading
    # endl only when it has contents), then trailing endl.
    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    os << std::endl;")
    lines.append(f"    my{choice_class}->streamContents(os, indentLevel + 1, isOneLineOnly);")
    for e in trailing_elems:
        cc = pascal(e.element_name)
        if e.min_occurs == 0:
            lines.append(f"    if (myHas{cc})")
            lines.append("    {")
            lines.append("        os << std::endl;")
            lines.append(f"        my{cc}->toStream(os, indentLevel + 1);")
            lines.append("    }")
        else:
            lines.append(f"    my{cc}->toStream(os, indentLevel + 1);")
    lines.append(f"    if (my{editorial_group_cls}->hasContents())")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append("    }")
    lines.append(f"    my{editorial_group_cls}->streamContents(os, indentLevel + 1, isOneLineOnly);")
    lines.append("    os << std::endl;")
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}\n")

    # Choice accessors
    lines.append(f"{choice_class}Ptr {class_name}::get{choice_class}() const")
    lines.append("{")
    lines.append(f"    return my{choice_class};")
    lines.append("}\n")
    lines.append(f"void {class_name}::set{choice_class}(const {choice_class}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{choice_class} = value;")
    lines.append("    }")
    lines.append("}\n")

    for e in trailing_elems:
        cc = pascal(e.element_name)
        lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
        lines.append("{")
        lines.append(f"    return my{cc};")
        lines.append("}\n")
        lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{cc} = value;")
        lines.append("    }")
        lines.append("}\n")
        if e.min_occurs == 0:
            lines.append(f"bool {class_name}::getHas{cc}() const")
            lines.append("{")
            lines.append(f"    return myHas{cc};")
            lines.append("}\n")
            lines.append(f"void {class_name}::setHas{cc}(const bool value)")
            lines.append("{")
            lines.append(f"    myHas{cc} = value;")
            lines.append("}\n")

    lines.append(f"{editorial_group_cls}Ptr {class_name}::get{editorial_group_cls}() const")
    lines.append("{")
    lines.append(f"    return my{editorial_group_cls};")
    lines.append("}\n")
    lines.append(f"void {class_name}::set{editorial_group_cls}(const {editorial_group_cls}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{editorial_group_cls} = value;")
    lines.append("    }")
    lines.append("}\n")

    # fromXElementImpl: iterate xelement; for each child:
    #   - try checkSetChoiceMember for each singleton branch
    #   - else: set choice to seq-group, get the seq-group ptr, importGroup
    #   - then importElement for each trailing optional
    #   - then importGroup for editorial
    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    lines.append("    isSuccess &= myAttributes->fromXElement(message, xelement);")
    lines.append("")
    lines.append("    auto endIter = xelement.end();")
    lines.append("    for (auto it = xelement.begin(); it != endIter; ++it)")
    lines.append("    {")
    # The committed Lyric.cpp has deliberate blank lines around each
    # checkSetChoiceMember block (after the opening brace, before/after
    # continue, and between the if/else-if branches). `make fmt` preserves
    # those blank lines, so we emit them explicitly.
    lines.append("")
    seq_group_enum = seq_group_cls[:1].lower() + seq_group_cls[1:]
    for i, b in enumerate(singleton_branches):
        cc = pascal(b.element_name)
        cname = b.element_name
        guard = "if" if i == 0 else "else if"
        lines.append(f'        {guard} (checkSetChoiceMember(message, *it, isSuccess, my{choice_class}, "{cname}",')
        lines.append(f'                                 &{choice_class}::get{cc},')
        lines.append(f'                                 static_cast<int>({choice_class}::Choice::{camel(cname)})))')
        lines.append("        {")
        lines.append("")
        lines.append("            continue;")
        lines.append("        }")
        lines.append("")
    # The sequence-branch (syllabicTextGroup) is only entered when the current
    # element can *start* that group (e.g. syllabic, text). importGroup consumes
    # the whole group in one call, so later siblings (end-line, end-paragraph,
    # footnote, level) must NOT re-enter this branch -- a bare `else` would feed
    # them to importGroup, which then reports the next required element as
    # missing. Guarding on the starter names lets those siblings fall through to
    # their own handlers below.
    starter_cond = " || ".join(
        f'it->getName() == "{e.element_name}"' for e in seq_branch_starters)
    lines.append(f"        else if ({starter_cond})")
    lines.append("        {")
    lines.append(f"            my{choice_class}->setChoice({choice_class}::Choice::{seq_group_enum});")
    lines.append(f"            {seq_group_cls}Ptr ptr = my{choice_class}->get{seq_group_cls}();")
    lines.append(f"            importGroup(message, it, endIter, isSuccess, ptr);")
    lines.append("        }")
    for e in trailing_elems:
        cc = pascal(e.element_name)
        if e.min_occurs == 0:
            lines.append(f"        if (importElement(message, *it, isSuccess, *my{cc}, myHas{cc}))")
            lines.append("        {")
            lines.append("            continue;")
            lines.append("        }")
        else:
            lines.append(f"        importElement(message, *it, isSuccess, *my{cc});")
    lines.append(f"        importGroup(message, it, endIter, isSuccess, my{editorial_group_cls});")
    lines.append("    }")
    lines.append("")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def _emit_lyric_family(elem_name, elem, ct, model, generated_attrs, stats):
    s = _extract_lyric_structure(ct)

    class_name = element_class_name(elem_name)            # Lyric
    # The choice class name is bespoke: 'LyricTextChoice'. It is not
    # pascal(elem_name) + 'Choice' (which would be 'LyricChoice'). The
    # original codegen named it after the "text" sense of its branches.
    choice_class = pascal(elem_name) + "TextChoice"

    # Synthetic group class names are derived bottom-up from the first two
    # children of each synthetic group plus a 'Group' suffix.
    inner_optional_group = _synth_group_name_from_children(s["inner_optional_elems"])
    inner_seq_group = _synth_group_name_from_children([inner_optional_group, s["inner_text"]])
    seq_branch_group = _synth_group_name_from_children(s["seq_branch_pre"])

    type_name = elem.type_name or ""

    # 1. Attrs struct
    if ct.attributes:
        attrs_name = resolve_attrs_name(elem_name, type_name, model)
        if attrs_name not in generated_attrs and attrs_name not in CORE_ROOT_ATTRS:
            h = generate_attrs_h(attrs_name, ct.attributes, model)
            c = generate_attrs_cpp(attrs_name, ct.attributes, model)
            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.h"), h)
            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.cpp"), c)
            generated_attrs.add(attrs_name)
            stats["attrs_written"] += 1
    else:
        attrs_name = None

    # 2. Innermost group: ElisionSyllabicGroup
    h = generate_elision_syllabic_group_h(inner_optional_group, s["inner_optional_elems"])
    c = generate_elision_syllabic_group_cpp(inner_optional_group, s["inner_optional_elems"])
    write_file(os.path.join(ELEM_DIR, f"{inner_optional_group}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{inner_optional_group}.cpp"), c)

    # 3. Middle group: ElisionSyllabicTextGroup
    h = generate_elision_syllabic_text_group_h(inner_seq_group, inner_optional_group, s["inner_text"])
    c = generate_elision_syllabic_text_group_cpp(inner_seq_group, inner_optional_group, s["inner_text"])
    write_file(os.path.join(ELEM_DIR, f"{inner_seq_group}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{inner_seq_group}.cpp"), c)

    # 4. Outer sequence-branch group: SyllabicTextGroup
    h = generate_syllabic_text_group_h(seq_branch_group, s["seq_branch_pre"],
                                       inner_seq_group, None, s["seq_branch_post"])
    c = generate_syllabic_text_group_cpp(seq_branch_group, s["seq_branch_pre"],
                                         inner_seq_group, s["seq_branch_post"])
    write_file(os.path.join(ELEM_DIR, f"{seq_branch_group}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{seq_branch_group}.cpp"), c)

    # 5. LyricTextChoice
    h = generate_lyric_text_choice_h(choice_class, seq_branch_group, s["singleton_branches"])
    c = generate_lyric_text_choice_cpp(choice_class, seq_branch_group, s["singleton_branches"])
    write_file(os.path.join(ELEM_DIR, f"{choice_class}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{choice_class}.cpp"), c)

    # 6. Editorial group class name (already generated elsewhere via GENERATE_GROUPS)
    editorial_group_cls = pascal(s["editorial_group"].group_name) + "Group"

    # 7. Lyric
    h = generate_lyric_h(class_name, attrs_name, choice_class, s["trailing_elems"],
                         editorial_group_cls)
    c = generate_lyric_cpp(elem_name, class_name, attrs_name, choice_class,
                           seq_branch_group, s["singleton_branches"], s["trailing_elems"],
                           editorial_group_cls, s["seq_branch_pre"])
    write_file(os.path.join(ELEM_DIR, f"{class_name}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{class_name}.cpp"), c)

    stats["elem_written"] += 1
    stats["bespoke_written"] = stats.get("bespoke_written", 0) + 1


# ---------------------------------------------------------------------------
# PartList (bespoke, schema-driven)
#
# part-list's complex type is:
#   sequence:
#     group ref part-group   (0..unbounded)
#     group ref score-part   (1..1)
#     choice (0..unbounded):
#       group ref part-group
#       group ref score-part
#
# In MusicXML each xs:group of this form wraps a same-named element, so the
# group references resolve to element references with identical names.
# Two classes are produced:
#   - PartGroupOrScorePart: the (trailing) choice promoted to a class. It
#     is a simple 2-branch choice (each branch is a single element ref),
#     with no attributes and no fromXElement logic (MX_FROM_XELEMENT_UNUSED).
#   - PartList: with myPartGroupSet, myScorePart, myPartGroupOrScorePartSet,
#     and a custom fromXElementImpl that uses a 'isFirstScorePartEncountered'
#     state machine to split incoming children before/after the first
#     score-part.
#
# All element names come from the parsed XSD model so that spec changes
# (renamed elements, etc.) propagate. The bespoke choice class name
# 'PartGroupOrScorePart' is composed from the two branch element names:
# pascal(branch[0]) + 'Or' + pascal(branch[1]).


def pascal_to_camel(pascal_name: str) -> str:
    """Convert a PascalCase identifier to camelCase by lowercasing only
    the first character. Used for variable names derived from class names
    that have no hyphen/underscore separators."""
    if not pascal_name:
        return pascal_name
    return pascal_name[0].lower() + pascal_name[1:]


def _extract_part_list_structure(ct):
    """Walk part-list's content_tree and return a dict describing the structure.

    Returns a dict with these keys (all element names come from the parsed XSD):
      - leading_unbounded: element name of the leading unbounded group-ref
                           (e.g. 'part-group')
      - singleton:         element name of the middle required group-ref
                           (e.g. 'score-part')
      - choice_branches:   list of element names of the trailing choice's
                           branches (e.g. ['part-group', 'score-part'])
    """
    tree = ct.content_tree
    assert isinstance(tree, SequenceNode)
    assert len(tree.children) == 3, (
        f"part-list: expected 3 top-level children, got {len(tree.children)}")

    leading_node = tree.children[0]
    middle_node = tree.children[1]
    choice_node = tree.children[2]

    def _grouplike_name(n):
        if isinstance(n, GroupRefNode):
            return n.group_name
        if isinstance(n, ElementRefNode):
            return n.element_name
        raise AssertionError(f"unexpected node: {type(n).__name__}")

    assert isinstance(leading_node, GroupRefNode) and leading_node.max_occurs == -1, (
        "part-list: expected leading unbounded group ref")
    assert isinstance(middle_node, GroupRefNode) and middle_node.max_occurs == 1, (
        "part-list: expected middle singleton group ref")
    assert isinstance(choice_node, ChoiceNode) and choice_node.max_occurs == -1, (
        "part-list: expected trailing unbounded choice")

    return {
        "leading_unbounded": _grouplike_name(leading_node),
        "singleton":         _grouplike_name(middle_node),
        "choice_branches":   [_grouplike_name(b) for b in choice_node.branches],
    }


def generate_part_group_or_score_part_h(class_name, branches):
    """branches = list of element names (e.g. ['part-group', 'score-part']).
    Emits the simple 2-branch element-ref choice class header.
    """
    branch_cls = [pascal(n) for n in branches]
    fwds = sorted(set(branch_cls))

    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    for fwd in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({fwd})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({class_name})\n")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append(f"\nclass {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append("    enum class Choice")
    lines.append("    {")
    for i, n in enumerate(branches):
        comma = "" if i == len(branches) - 1 else ","
        lines.append(f"        {camel(n)} = {i + 1}{comma}")
    lines.append("    };")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append("")
    lines.append("    /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {class_name}::Choice getChoice() const;")
    lines.append(f"    void setChoice(const {class_name}::Choice value);")
    # Bespoke quirk to match committed: the PartGroup accessor block uses
    # 'maxPartGroupOccurs' in its comment header rather than 'maxOccurs'.
    # See state.md gotcha.
    for i, n in enumerate(branches):
        cc = pascal(n)
        lines.append("")
        if i == 0:
            lines.append(f"    /* _________ {cc} minOccurs = 1, max{cc}Occurs = 1 _________ */")
        else:
            lines.append(f"    /* _________ {cc} minOccurs = 1, maxOccurs = 1 _________ */")
        lines.append(f"    {cc}Ptr get{cc}() const;")
        lines.append(f"    void set{cc}(const {cc}Ptr &value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append("    Choice myChoice;")
    for n in branches:
        cc = pascal(n)
        lines.append(f"    {cc}Ptr my{cc};")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_part_group_or_score_part_cpp(class_name, branches):
    branch_cls = [pascal(n) for n in branches]
    inc_classes = sorted(set(branch_cls))

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in inc_classes:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    # Constructor: default choice is the first branch.
    init_parts = [f"myChoice(Choice::{camel(branches[0])})"]
    for n in branches:
        cc = pascal(n)
        init_parts.append(f"my{cc}(make{cc}())")
    _emit_ctor_init(lines, f"{class_name}::{class_name}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")

    # hasContents: any one of the chosen branches has contents
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    cond_parts = []
    for n in branches:
        cc = pascal(n)
        cond_parts.append(f"(myChoice == Choice::{camel(n)} && my{cc}->hasContents())")
    lines.append("    return " + (" ||\n           ".join(cond_parts)) + ";")
    lines.append("}\n")

    # streamContents: switch on the choice, call toStream on the selected branch
    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    switch (myChoice)")
    lines.append("    {")
    for n in branches:
        cc = pascal(n)
        lines.append(f"    case Choice::{camel(n)}:")
        lines.append(f"        my{cc}->toStream(os, indentLevel);")
        lines.append("        break;")
    lines.append("    default:")
    lines.append("        break;")
    lines.append("    }")
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}\n")

    # Choice accessors
    lines.append(f"{class_name}::Choice {class_name}::getChoice() const")
    lines.append("{")
    lines.append("    return myChoice;")
    lines.append("}\n")
    lines.append(f"void {class_name}::setChoice(const {class_name}::Choice value)")
    lines.append("{")
    lines.append("    myChoice = value;")
    lines.append("}\n")

    # Per-branch accessors: getter then setter
    for n in branches:
        cc = pascal(n)
        lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
        lines.append("{")
        lines.append(f"    return my{cc};")
        lines.append("}\n")
        lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{cc} = value;")
        lines.append("    }")
        lines.append("}\n")

    lines.append(f"MX_FROM_XELEMENT_UNUSED({class_name})\n")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_part_list_h(class_name, leading_name, singleton_name, choice_class_name):
    leading_cls = pascal(leading_name)
    singleton_cls = pascal(singleton_name)
    fwds = sorted({leading_cls, choice_class_name, singleton_cls}) + [class_name]

    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    for fwd in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({fwd})")
    lines.append("")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append(f"\nclass {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append("")
    lines.append(f"    /* _________ {leading_cls} minOccurs = 0, maxOccurs = unbounded _________ */")
    lines.append(f"    const {leading_cls}Set &get{leading_cls}Set() const;")
    lines.append(f"    void add{leading_cls}(const {leading_cls}Ptr &value);")
    lines.append(f"    void remove{leading_cls}(const {leading_cls}SetIterConst &value);")
    lines.append(f"    void clear{leading_cls}Set();")
    lines.append(f"    {leading_cls}Ptr get{leading_cls}(const {leading_cls}SetIterConst &setIterator) const;")
    lines.append("")
    lines.append(f"    /* _________ {singleton_cls} minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {singleton_cls}Ptr get{singleton_cls}() const;")
    lines.append(f"    void set{singleton_cls}(const {singleton_cls}Ptr &value);")
    lines.append("")
    lines.append(f"    /* _________ {choice_class_name} minOccurs = 0, maxOccurs = unbounded _________ */")
    lines.append(f"    const {choice_class_name}Set &get{choice_class_name}Set() const;")
    lines.append(f"    void add{choice_class_name}(const {choice_class_name}Ptr &value);")
    lines.append(f"    void remove{choice_class_name}(const {choice_class_name}SetIterConst &value);")
    lines.append(f"    void clear{choice_class_name}Set();")
    lines.append(f"    {choice_class_name}Ptr get{choice_class_name}(const {choice_class_name}SetIterConst &setIterator) const;")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append(f"    {leading_cls}Set my{leading_cls}Set;")
    lines.append(f"    {singleton_cls}Ptr my{singleton_cls};")
    lines.append(f"    {choice_class_name}Set my{choice_class_name}Set;")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_part_list_cpp(elem_name, class_name, leading_name, singleton_name,
                           choice_class_name, choice_branches):
    leading_cls = pascal(leading_name)
    singleton_cls = pascal(singleton_name)
    inc_classes = sorted({leading_cls, choice_class_name, singleton_cls})

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in inc_classes:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>\n")
    lines.append("namespace mx\n{\nnamespace core\n{")

    init_parts = [
        f"my{leading_cls}Set()",
        f"my{singleton_cls}(make{singleton_cls}())",
        f"my{choice_class_name}Set()",
    ]
    _emit_ctor_init(lines, f"{class_name}::{class_name}()", init_parts)
    lines.append("{")
    lines.append("}\n")

    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}\n")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append(f'    os << "{elem_name}";')
    lines.append("    return os;")
    lines.append("}\n")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}\n")

    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append(f"    for (auto x : my{leading_cls}Set)")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append("        x->toStream(os, indentLevel + 1);")
    lines.append("    }")
    lines.append("    os << std::endl;")
    lines.append(f"    my{singleton_cls}->toStream(os, indentLevel + 1);")
    lines.append(f"    for (auto x : my{choice_class_name}Set)")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append("        x->streamContents(os, indentLevel + 1, isOneLineOnly);")
    lines.append("    }")
    lines.append("    isOneLineOnly = false;")
    lines.append("    os << std::endl;")
    lines.append("    return os;")
    lines.append("}\n")

    # leading set accessors (order: getSet, add, remove, clear, get(setIterator))
    lines.append(f"const {leading_cls}Set &{class_name}::get{leading_cls}Set() const")
    lines.append("{")
    lines.append(f"    return my{leading_cls}Set;")
    lines.append("}\n")
    lines.append(f"void {class_name}::add{leading_cls}(const {leading_cls}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{leading_cls}Set.push_back(value);")
    lines.append("    }")
    lines.append("}\n")
    lines.append(f"void {class_name}::remove{leading_cls}(const {leading_cls}SetIterConst &value)")
    lines.append("{")
    lines.append(f"    if (value != my{leading_cls}Set.cend())")
    lines.append("    {")
    lines.append(f"        my{leading_cls}Set.erase(value);")
    lines.append("    }")
    lines.append("}\n")
    lines.append(f"void {class_name}::clear{leading_cls}Set()")
    lines.append("{")
    lines.append(f"    my{leading_cls}Set.clear();")
    lines.append("}\n")
    lines.append(f"{leading_cls}Ptr {class_name}::get{leading_cls}(const {leading_cls}SetIterConst &setIterator) const")
    lines.append("{")
    lines.append(f"    if (setIterator != my{leading_cls}Set.cend())")
    lines.append("    {")
    lines.append("        return *setIterator;")
    lines.append("    }")
    lines.append(f"    return {leading_cls}Ptr();")
    lines.append("}\n")

    # Singleton accessors
    lines.append(f"{singleton_cls}Ptr {class_name}::get{singleton_cls}() const")
    lines.append("{")
    lines.append(f"    return my{singleton_cls};")
    lines.append("}\n")
    lines.append(f"void {class_name}::set{singleton_cls}(const {singleton_cls}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{singleton_cls} = value;")
    lines.append("    }")
    lines.append("}\n")

    # Trailing choice-set accessors (order: getSet, add, remove, clear, get)
    cc = choice_class_name
    lines.append(f"const {cc}Set &{class_name}::get{cc}Set() const")
    lines.append("{")
    lines.append(f"    return my{cc}Set;")
    lines.append("}\n")
    lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{cc}Set.push_back(value);")
    lines.append("    }")
    lines.append("}\n")
    lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
    lines.append("{")
    lines.append(f"    if (value != my{cc}Set.cend())")
    lines.append("    {")
    lines.append(f"        my{cc}Set.erase(value);")
    lines.append("    }")
    lines.append("}\n")
    lines.append(f"void {class_name}::clear{cc}Set()")
    lines.append("{")
    lines.append(f"    my{cc}Set.clear();")
    lines.append("}\n")
    lines.append(f"{cc}Ptr {class_name}::get{cc}(const {cc}SetIterConst &setIterator) const")
    lines.append("{")
    lines.append(f"    if (setIterator != my{cc}Set.cend())")
    lines.append("    {")
    lines.append("        return *setIterator;")
    lines.append("    }")
    lines.append(f"    return {cc}Ptr();")
    lines.append("}\n")

    # Custom fromXElementImpl with isFirstScorePartEncountered state machine.
    # leading_name e.g. 'part-group', singleton_name e.g. 'score-part'.
    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    lines.append(f"    bool isFirst{singleton_cls}Encountered = false;")
    lines.append("")
    lines.append("    for (auto it = xelement.begin(); it != xelement.end(); ++it)")
    lines.append("    {")
    lines.append("        const std::string elementName = it->getName();")
    lines.append(f'        if (elementName != "{leading_name}" && elementName != "{singleton_name}")')
    lines.append("        {")
    lines.append(f'            message << "{class_name}: fromXElement encountered unexpected element \'" << elementName << "\'" << std::endl;')
    lines.append("        }")
    lines.append(f"        if (!isFirst{singleton_cls}Encountered)")
    lines.append("        {")
    lines.append(f'            if (elementName == "{leading_name}")')
    lines.append("            {")
    lines.append(f"                auto {camel(leading_name)} = make{leading_cls}();")
    lines.append(f"                isSuccess &= {camel(leading_name)}->fromXElement(message, *it);")
    lines.append(f"                my{leading_cls}Set.push_back({camel(leading_name)});")
    lines.append("            }")
    lines.append(f'            else if (elementName == "{singleton_name}")')
    lines.append("            {")
    lines.append(f"                isFirst{singleton_cls}Encountered = true;")
    lines.append(f"                isSuccess &= my{singleton_cls}->fromXElement(message, *it);")
    lines.append("            }")
    lines.append("        }")
    lines.append("        else")
    lines.append("        {")
    lines.append(f"            auto {pascal_to_camel(choice_class_name)} = make{choice_class_name}();")
    lines.append("")
    cc_local = pascal_to_camel(choice_class_name)
    for i, n in enumerate(choice_branches):
        cc_b = pascal(n)
        prefix = "if" if i == 0 else "else if"
        lines.append(f'            {prefix} (elementName == "{n}")')
        lines.append("            {")
        lines.append(f"                {cc_local}->setChoice({choice_class_name}::Choice::{camel(n)});")
        lines.append(f"                {cc_local}->get{cc_b}()->fromXElement(message, *it);")
        lines.append("            }")
    lines.append("")
    lines.append(f"            my{choice_class_name}Set.push_back({cc_local});")
    lines.append("        }")
    lines.append("    }")
    lines.append("")
    lines.append(f"    if (!isFirst{singleton_cls}Encountered)")
    lines.append("    {")
    lines.append(f'        message << "{class_name}: at least one \'{singleton_name}\' elements are required but none were found" << std::endl;')
    lines.append("        isSuccess = false;")
    lines.append("    }")
    lines.append("")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}\n")

    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Harmony (bespoke, schema-driven)
#
# harmony's complex type:
#   sequence:
#     group ref harmony-chord (1..unbounded)  -> emits HarmonyChordGroup
#     element frame  (0..1)
#     element offset (0..1)
#     group ref editorial (1..1)
#     group ref staff (0..1)
#   plus attribute groups -> HarmonyAttributes
#
# The harmony-chord xs:group internal structure (not exposed via
# model.groups because the inner xs:choice is dropped by _parse_groups,
# and not exposed via complex_types because it's a group, not a type):
#   sequence:
#     choice:
#       element root
#       element function   (type style-text)
#     element kind
#     element inversion  (0..1)
#     element bass       (0..1)
#     element degree     (0..unbounded)
#
# The bespoke handler walks model.root directly to recover the harmony-chord
# inner choice. All names (group, choice branches, elements) come from the
# parsed XSD.
#
# Output classes:
#   - HarmonyChordGroup: synthetic group with internal Choice (root|function),
#     required kind, optional inversion/bass, unbounded degree set. The
#     class is invoked via streamContents (not toStream) from the parent,
#     and is MX_FROM_XELEMENT_UNUSED -- the parent Harmony's fromXElementImpl
#     state-machine builds the chord groups itself.
#   - Harmony: with the HarmonyChordGroup set (always has >=1 entry, ctor
#     pushes one), optional frame/offset, required EditorialGroup,
#     optional Staff, plus the HarmonyAttributes. Includes a custom
#     fromXElementImpl that consumes incoming root/function as the start of
#     a new chord group, then required kind, then optional inversion/bass,
#     then unbounded degree, then falls through to import frame/offset/
#     editorial/staff. A private addGroup helper replaces the default-
#     constructed first entry on first add, then appends subsequently.


def _extract_harmony_chord_group_structure(model):
    """Walk model.root to find the harmony-chord xs:group and recover its
    full structure (the inner xs:choice that model.groups['harmony-chord']
    drops).

    Returns a dict:
      - choice_branches: list[str] of element names (e.g. ['root', 'function'])
      - kind:        str (the required singleton element name)
      - optionals:   list[str] (optional singleton element names, in order)
      - unbounded:   str (the trailing unbounded element name)
    """
    grp = None
    for g in model.root.iter(f"{XS}group"):
        if g.get("name") == "harmony-chord":
            grp = g
            break
    assert grp is not None, "harmony-chord group not found in XSD"

    seq = grp.find(f"{XS}sequence")
    assert seq is not None, "harmony-chord: expected sequence"

    choice_branches = []
    singletons = []  # (name, min_occurs)
    unbounded = None

    for child in seq:
        if child.tag == f"{XS}choice":
            for b in child:
                if b.tag == f"{XS}element":
                    nm = b.get("ref") or b.get("name")
                    if nm:
                        choice_branches.append(nm)
        elif child.tag == f"{XS}element":
            nm = child.get("ref") or child.get("name")
            if not nm:
                continue
            min_o = int(child.get("minOccurs", "1"))
            max_o = child.get("maxOccurs", "1")
            max_o = -1 if max_o == "unbounded" else int(max_o)
            if max_o == -1:
                unbounded = nm
            else:
                singletons.append((nm, min_o))

    assert choice_branches, "harmony-chord: expected at least one choice branch"
    assert singletons, "harmony-chord: expected at least the required kind element"
    kind = singletons[0][0]
    assert singletons[0][1] == 1, "harmony-chord: first singleton must be required"
    optionals = [n for (n, mo) in singletons[1:] if mo == 0]
    assert unbounded is not None, "harmony-chord: expected an unbounded trailing element"

    return {
        "choice_branches": choice_branches,
        "kind": kind,
        "optionals": optionals,
        "unbounded": unbounded,
    }


def _extract_harmony_structure(ct, model):
    """Walk harmony's content_tree to identify the leading harmony-chord
    group ref, the optional frame/offset elements, the editorial group ref,
    and the optional staff group ref. Returns a dict."""
    tree = ct.content_tree
    assert isinstance(tree, SequenceNode)

    chord_group = None
    optional_elems = []   # list[ElementRefNode] - frame, offset
    editorial_group = None
    staff_group = None

    for c in tree.children:
        if isinstance(c, GroupRefNode) and c.max_occurs == -1:
            chord_group = c
        elif isinstance(c, ElementRefNode) and c.min_occurs == 0:
            optional_elems.append(c)
        elif isinstance(c, GroupRefNode) and c.group_name == "staff":
            staff_group = c
        elif isinstance(c, GroupRefNode):
            editorial_group = c

    assert chord_group is not None, "harmony: expected leading unbounded chord group ref"
    assert editorial_group is not None, "harmony: expected editorial group ref"

    inner = _extract_harmony_chord_group_structure(model)

    return {
        "chord_group_name": chord_group.group_name,     # 'harmony-chord'
        "optional_elems":   [c.element_name for c in optional_elems],
        "editorial_group":  editorial_group.group_name, # 'editorial'
        "staff_group":      staff_group.group_name if staff_group else None,
        "inner":            inner,
    }


def generate_harmony_chord_group_h(class_name, inner):
    """class_name e.g. 'HarmonyChordGroup'. inner = dict from
    _extract_harmony_chord_group_structure."""
    branches = inner["choice_branches"]
    kind = inner["kind"]
    optionals = inner["optionals"]
    unbounded = inner["unbounded"]

    branch_cls = [pascal(n) for n in branches]
    kind_cls = pascal(kind)
    optional_cls = [pascal(n) for n in optionals]
    unbounded_cls = pascal(unbounded)

    fwds = sorted(set(branch_cls + [kind_cls] + optional_cls + [unbounded_cls]))

    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    for fwd in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({fwd})")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({class_name})\n")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append(f"\nclass {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append("    enum class Choice")
    lines.append("    {")
    for i, n in enumerate(branches):
        comma = "" if i == len(branches) - 1 else ","
        lines.append(f"        {camel(n)} = {i + 1}{comma}")
    lines.append("    };")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append(f"    {class_name}::Choice getChoice() const;")
    lines.append(f"    void setChoice(const {class_name}::Choice value);")
    # branch accessors
    for n in branches:
        cc = pascal(n)
        lines.append("")
        lines.append(f"    /* _________ {cc} minOccurs = 1, maxOccurs = 1 _________ */")
        lines.append(f"    {cc}Ptr get{cc}() const;")
        lines.append(f"    void set{cc}(const {cc}Ptr &value);")
    # kind (required singleton)
    lines.append("")
    lines.append(f"    /* _________ {kind_cls} minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {kind_cls}Ptr get{kind_cls}() const;")
    lines.append(f"    void set{kind_cls}(const {kind_cls}Ptr &value);")
    # optionals
    for n in optionals:
        cc = pascal(n)
        lines.append("")
        lines.append(f"    /* _________ {cc} minOccurs = 0, maxOccurs = 1 _________ */")
        lines.append(f"    {cc}Ptr get{cc}() const;")
        lines.append(f"    void set{cc}(const {cc}Ptr &value);")
        lines.append(f"    bool getHas{cc}() const;")
        lines.append(f"    void setHas{cc}(const bool value);")
    # unbounded set
    cc = unbounded_cls
    lines.append("")
    lines.append(f"    /* _________ {cc} minOccurs = 0, maxOccurs = unbounded _________ */")
    lines.append(f"    const {cc}Set &get{cc}Set() const;")
    lines.append(f"    void add{cc}(const {cc}Ptr &value);")
    lines.append(f"    void remove{cc}(const {cc}SetIterConst &value);")
    lines.append(f"    void clear{cc}Set();")
    lines.append(f"    {cc}Ptr get{cc}(const {cc}SetIterConst &setIterator) const;")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append("    Choice myChoice;")
    for n in branches:
        cc = pascal(n)
        lines.append(f"    {cc}Ptr my{cc};")
    lines.append(f"    {kind_cls}Ptr my{kind_cls};")
    for n in optionals:
        cc = pascal(n)
        lines.append(f"    {cc}Ptr my{cc};")
        lines.append(f"    bool myHas{cc};")
    lines.append(f"    {unbounded_cls}Set my{unbounded_cls}Set;")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_harmony_chord_group_cpp(class_name, inner):
    branches = inner["choice_branches"]
    kind = inner["kind"]
    optionals = inner["optionals"]
    unbounded = inner["unbounded"]

    branch_cls = [pascal(n) for n in branches]
    kind_cls = pascal(kind)
    optional_cls = [pascal(n) for n in optionals]
    unbounded_cls = pascal(unbounded)

    includes = sorted(set(branch_cls + [kind_cls] + optional_cls + [unbounded_cls]))

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in includes:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{")

    # Ctor
    init_parts = []
    init_parts.append(f"myChoice(Choice::{camel(branches[0])})")
    for n in branches:
        cc = pascal(n)
        init_parts.append(f"my{cc}(make{cc}())")
    init_parts.append(f"my{kind_cls}(make{kind_cls}())")
    for n in optionals:
        cc = pascal(n)
        init_parts.append(f"my{cc}(make{cc}())")
        init_parts.append(f"myHas{cc}(false)")
    init_parts.append(f"my{unbounded_cls}Set()")
    lines.append(f"{class_name}::{class_name}()")
    lines.append("    : " + ", ".join(init_parts))
    lines.append("{")
    lines.append("}")
    lines.append("")

    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}")
    lines.append("")
    # streamContents
    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    switch (myChoice)")
    lines.append("    {")
    for n in branches:
        cc = pascal(n)
        lines.append(f"    case Choice::{camel(n)}:")
        lines.append(f"        my{cc}->toStream(os, indentLevel);")
        lines.append("        break;")
    lines.append("    default:")
    lines.append("        break;")
    lines.append("    }")
    lines.append("    os << std::endl;")
    lines.append(f"    my{kind_cls}->toStream(os, indentLevel);")
    for n in optionals:
        cc = pascal(n)
        lines.append(f"    if (myHas{cc})")
        lines.append("    {")
        lines.append("        os << std::endl;")
        lines.append(f"        my{cc}->toStream(os, indentLevel);")
        lines.append("    }")
    lines.append(f"    for (auto x : my{unbounded_cls}Set)")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append("        x->toStream(os, indentLevel);")
    lines.append("    }")
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")

    # getChoice / setChoice
    lines.append(f"{class_name}::Choice {class_name}::getChoice() const")
    lines.append("{")
    lines.append("    return myChoice;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::setChoice(const {class_name}::Choice value)")
    lines.append("{")
    lines.append("    myChoice = value;")
    lines.append("}")
    lines.append("")

    # branch accessors (get/set)
    for n in branches:
        cc = pascal(n)
        lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
        lines.append("{")
        lines.append(f"    return my{cc};")
        lines.append("}")
        lines.append("")
        lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{cc} = value;")
        lines.append("    }")
        lines.append("}")
        lines.append("")

    # kind accessors
    cc = kind_cls
    lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
    lines.append("{")
    lines.append(f"    return my{cc};")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{cc} = value;")
    lines.append("    }")
    lines.append("}")
    lines.append("")

    # optional accessors
    for n in optionals:
        cc = pascal(n)
        lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
        lines.append("{")
        lines.append(f"    return my{cc};")
        lines.append("}")
        lines.append("")
        lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{cc} = value;")
        lines.append("    }")
        lines.append("}")
        lines.append("")
        lines.append(f"bool {class_name}::getHas{cc}() const")
        lines.append("{")
        lines.append(f"    return myHas{cc};")
        lines.append("}")
        lines.append("")
        lines.append(f"void {class_name}::setHas{cc}(const bool value)")
        lines.append("{")
        lines.append(f"    myHas{cc} = value;")
        lines.append("}")
        lines.append("")

    # unbounded set accessors
    cc = unbounded_cls
    lines.append(f"const {cc}Set &{class_name}::get{cc}Set() const")
    lines.append("{")
    lines.append(f"    return my{cc}Set;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{cc}Set.push_back(value);")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
    lines.append("{")
    lines.append(f"    if (value != my{cc}Set.cend())")
    lines.append("    {")
    lines.append(f"        my{cc}Set.erase(value);")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::clear{cc}Set()")
    lines.append("{")
    lines.append(f"    my{cc}Set.clear();")
    lines.append("}")
    lines.append("")
    lines.append(f"{cc}Ptr {class_name}::get{cc}(const {cc}SetIterConst &setIterator) const")
    lines.append("{")
    lines.append(f"    if (setIterator != my{cc}Set.cend())")
    lines.append("    {")
    lines.append("        return *setIterator;")
    lines.append("    }")
    lines.append(f"    return {cc}Ptr();")
    lines.append("}")
    lines.append("")
    lines.append(f"MX_FROM_XELEMENT_UNUSED({class_name});")
    lines.append("")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_harmony_h(class_name, attrs_name, chord_group_cls, optional_elems,
                       editorial_group_cls, staff_cls):
    """Emit Harmony.h. optional_elems is a list of element names (frame, offset)."""
    optional_cls = [pascal(n) for n in optional_elems]

    fwds = [f"MX_FORWARD_DECLARE_ATTRIBUTES({attrs_name})"]
    elem_fwds = [editorial_group_cls] + optional_cls + [chord_group_cls]
    if staff_cls:
        elem_fwds.append(staff_cls)
    # sort alphabetically (this matches committed Harmony.h ordering of fwd decls)
    for fwd in sorted(set(elem_fwds)):
        fwds.append(f"MX_FORWARD_DECLARE_ELEMENT({fwd})")
    fwds.append(f"MX_FORWARD_DECLARE_ELEMENT({class_name})")

    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append(f'#include "mx/core/elements/{attrs_name}.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    lines.extend(fwds)
    lines.append("")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append("")
    lines.append(f"class {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append(f"    {attrs_name}Ptr getAttributes() const;")
    lines.append(f"    void setAttributes(const {attrs_name}Ptr &value);")
    lines.append("")
    # chord group set
    cc = chord_group_cls
    lines.append(f"    /* _________ {cc} minOccurs = 1, maxOccurs = unbounded _________ */")
    lines.append(f"    const {cc}Set &get{cc}Set() const;")
    lines.append(f"    void add{cc}(const {cc}Ptr &value);")
    lines.append(f"    void remove{cc}(const {cc}SetIterConst &value);")
    lines.append(f"    void clear{cc}Set();")
    lines.append(f"    {cc}Ptr get{cc}(const {cc}SetIterConst &setIterator) const;")
    # optional elements
    for n in optional_elems:
        cc = pascal(n)
        lines.append("")
        lines.append(f"    /* _________ {cc} minOccurs = 0, maxOccurs = 1 _________ */")
        lines.append(f"    {cc}Ptr get{cc}() const;")
        lines.append(f"    void set{cc}(const {cc}Ptr &value);")
        lines.append(f"    bool getHas{cc}() const;")
        lines.append(f"    void setHas{cc}(const bool value);")
    # editorial group (required)
    cc = editorial_group_cls
    lines.append("")
    lines.append(f"    /* _________ {cc} minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {cc}Ptr get{cc}() const;")
    lines.append(f"    void set{cc}(const {cc}Ptr &value);")
    # optional staff
    if staff_cls:
        cc = staff_cls
        lines.append("")
        lines.append(f"    /* _________ {cc} minOccurs = 0, maxOccurs = 1 _________ */")
        lines.append(f"    {cc}Ptr get{cc}() const;")
        lines.append(f"    void set{cc}(const {cc}Ptr &value);")
        lines.append(f"    bool getHas{cc}() const;")
        lines.append(f"    void setHas{cc}(const bool value);")

    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append(f"    {attrs_name}Ptr myAttributes;")
    lines.append(f"    {chord_group_cls}Set my{chord_group_cls}Set;")
    for n in optional_elems:
        cc = pascal(n)
        lines.append(f"    {cc}Ptr my{cc};")
        lines.append(f"    bool myHas{cc};")
    lines.append(f"    {editorial_group_cls}Ptr my{editorial_group_cls};")
    if staff_cls:
        lines.append(f"    {staff_cls}Ptr my{staff_cls};")
        lines.append(f"    bool myHas{staff_cls};")
    lines.append("")
    lines.append(f"    void addGroup({chord_group_cls}Ptr &grp, bool &isFirst);")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_harmony_cpp(elem_name, class_name, attrs_name, chord_group_cls,
                          inner, optional_elems, editorial_group_cls, staff_cls):
    """Emit Harmony.cpp including the custom fromXElementImpl state machine."""
    branches = inner["choice_branches"]              # ['root', 'function']
    kind = inner["kind"]                              # 'kind'
    inner_optionals = inner["optionals"]              # ['inversion', 'bass']
    unbounded = inner["unbounded"]                    # 'degree'

    branch_cls = [pascal(n) for n in branches]
    kind_cls = pascal(kind)
    inner_optional_cls = [pascal(n) for n in inner_optionals]
    unbounded_cls = pascal(unbounded)
    optional_cls = [pascal(n) for n in optional_elems]

    # Include set: parent attrs, chord-group's members (so makeXxx() calls
    # resolve), the chord group class itself, the optional elements, the
    # editorial group, the staff. Match the committed include layout, which
    # lists every element class touched in fromXElementImpl plus the chord
    # group itself.
    includes = sorted(set(
        branch_cls + [kind_cls] + inner_optional_cls + [unbounded_cls]
        + [editorial_group_cls] + optional_cls + [chord_group_cls]
        + ([staff_cls] if staff_cls else [])
    ))

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in includes:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{")

    # Constructor
    init_parts = [
        f"myAttributes(std::make_shared<{attrs_name}>())",
        f"my{chord_group_cls}Set()",
    ]
    for n in optional_elems:
        cc = pascal(n)
        init_parts.append(f"my{cc}(make{cc}())")
        init_parts.append(f"myHas{cc}(false)")
    init_parts.append(f"my{editorial_group_cls}(make{editorial_group_cls}())")
    if staff_cls:
        init_parts.append(f"my{staff_cls}(make{staff_cls}())")
        init_parts.append(f"myHas{staff_cls}(false)")

    lines.append(f"{class_name}::{class_name}()")
    lines.append("    : " + ", ".join(init_parts))
    lines.append("{")
    lines.append(f"    my{chord_group_cls}Set.push_back(make{chord_group_cls}());")
    lines.append("}")
    lines.append("")

    # hasAttributes / streamAttributes / streamName
    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return myAttributes->hasValues();")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return myAttributes->toStream(os);")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append(f'    os << "{elem_name}";')
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}")
    lines.append("")

    # streamContents
    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append(f"    for (auto x : my{chord_group_cls}Set)")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append("        x->streamContents(os, indentLevel + 1, isOneLineOnly);")
    lines.append("    }")
    for n in optional_elems:
        cc = pascal(n)
        lines.append(f"    if (myHas{cc})")
        lines.append("    {")
        lines.append("        os << std::endl;")
        lines.append(f"        my{cc}->toStream(os, indentLevel + 1);")
        lines.append("    }")
    lines.append(f"    if (my{editorial_group_cls}->hasContents())")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append(f"        my{editorial_group_cls}->streamContents(os, indentLevel + 1, isOneLineOnly);")
    lines.append("    }")
    if staff_cls:
        lines.append(f"    if (myHas{staff_cls})")
        lines.append("    {")
        lines.append("        os << std::endl;")
        lines.append(f"        my{staff_cls}->toStream(os, indentLevel + 1);")
        lines.append("    }")
    lines.append("    os << std::endl;")
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")

    # Attribute accessors
    lines.append(f"{attrs_name}Ptr {class_name}::getAttributes() const")
    lines.append("{")
    lines.append("    return myAttributes;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::setAttributes(const {attrs_name}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append("        myAttributes = value;")
    lines.append("    }")
    lines.append("}")
    lines.append("")

    # Chord group set accessors. Note: removeXxx for a "must have at least 1"
    # set guards against removing the last element. clearXxxSet resets to a
    # single default entry.
    cc = chord_group_cls
    lines.append(f"const {cc}Set &{class_name}::get{cc}Set() const")
    lines.append("{")
    lines.append(f"    return my{cc}Set;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{cc}Set.push_back(value);")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
    lines.append("{")
    lines.append(f"    if (value != my{cc}Set.cend())")
    lines.append("    {")
    lines.append(f"        if (my{cc}Set.size() > 1)")
    lines.append("        {")
    lines.append(f"            my{cc}Set.erase(value);")
    lines.append("        }")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::clear{cc}Set()")
    lines.append("{")
    lines.append(f"    my{cc}Set.clear();")
    lines.append(f"    my{cc}Set.push_back(make{cc}());")
    lines.append("}")
    lines.append("")
    lines.append(f"{cc}Ptr {class_name}::get{cc}(const {cc}SetIterConst &setIterator) const")
    lines.append("{")
    lines.append(f"    if (setIterator != my{cc}Set.cend())")
    lines.append("    {")
    lines.append("        return *setIterator;")
    lines.append("    }")
    lines.append(f"    return {cc}Ptr();")
    lines.append("}")
    lines.append("")

    # Optional element accessors (frame/offset)
    for n in optional_elems:
        cc = pascal(n)
        lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
        lines.append("{")
        lines.append(f"    return my{cc};")
        lines.append("}")
        lines.append("")
        lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{cc} = value;")
        lines.append("    }")
        lines.append("}")
        lines.append("")
        lines.append(f"bool {class_name}::getHas{cc}() const")
        lines.append("{")
        lines.append(f"    return myHas{cc};")
        lines.append("}")
        lines.append("")
        lines.append(f"void {class_name}::setHas{cc}(const bool value)")
        lines.append("{")
        lines.append(f"    myHas{cc} = value;")
        lines.append("}")
        lines.append("")

    # Editorial group accessor
    cc = editorial_group_cls
    lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
    lines.append("{")
    lines.append(f"    return my{cc};")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{cc} = value;")
    lines.append("    }")
    lines.append("}")
    lines.append("")

    # Staff accessors (optional)
    if staff_cls:
        cc = staff_cls
        lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
        lines.append("{")
        lines.append(f"    return my{cc};")
        lines.append("}")
        lines.append("")
        lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{cc} = value;")
        lines.append("    }")
        lines.append("}")
        lines.append("")
        lines.append(f"bool {class_name}::getHas{cc}() const")
        lines.append("{")
        lines.append(f"    return myHas{cc};")
        lines.append("}")
        lines.append("")
        lines.append(f"void {class_name}::setHas{cc}(const bool value)")
        lines.append("{")
        lines.append(f"    myHas{cc} = value;")
        lines.append("}")
        lines.append("")

    # fromXElementImpl. The state machine: on each pass, if we see a choice
    # branch (root|function), we start consuming the chord-group elements
    # in fixed order (kind, optional inversion, optional bass, then any
    # number of degree). On any other element name, we try importElement
    # for the optional frame/offset, then importGroup for editorial,
    # then importElement for staff.
    branch_check = ' || '.join(f'elementName == "{n}"' for n in branches)
    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    lines.append("    isSuccess &= myAttributes->fromXElement(message, xelement);")
    lines.append("    bool isChoiceFound = false;")
    lines.append(f"    bool isFirst{chord_group_cls}Added = false;")
    lines.append("")
    lines.append("    auto endIter = xelement.end();")
    lines.append("    for (auto it = xelement.begin(); it != endIter; ++it)")
    lines.append("    {")
    lines.append("        const std::string elementName = it->getName();")
    lines.append(f"        if ({branch_check})")
    lines.append("        {")
    lines.append("            bool decrementIter = false;")
    lines.append("            isChoiceFound = true;")
    lines.append(f"            auto item = make{chord_group_cls}();")

    # Per-branch dispatch
    for i, n in enumerate(branches):
        cc = pascal(n)
        prefix = "if" if i == 0 else "else"
        if i == 0:
            lines.append(f'            if (elementName == "{n}")')
        elif i == len(branches) - 1:
            lines.append("            else")
        else:
            lines.append(f'            else if (elementName == "{n}")')
        lines.append("            {")
        lines.append(f"                item->setChoice({chord_group_cls}::Choice::{camel(n)});")
        lines.append(f"                isSuccess &= item->get{cc}()->fromXElement(message, *it);")
        lines.append("            }")
    lines.append("            decrementIter = true;")
    lines.append("            ++it;")
    # required kind
    lines.append("            if (it == endIter || it->getName() != \"" + kind + "\")")
    lines.append("            {")
    lines.append(f'                message << "{class_name}: \'{kind}\' is a required element and is missing aborting" << std::endl;')
    lines.append("                return false;")
    lines.append("            }")
    lines.append(f"            isSuccess &= item->get{kind_cls}()->fromXElement(message, *it);")
    lines.append("            decrementIter = true;")
    lines.append("            ++it;")
    lines.append("            if (it == endIter)")
    lines.append("            {")
    lines.append(f"                addGroup(item, isFirst{chord_group_cls}Added);")
    lines.append("                MX_RETURN_IS_SUCCESS;")
    lines.append("            }")
    # optionals
    for n in inner_optionals:
        cc = pascal(n)
        lines.append(f'            if (it->getName() == "{n}")')
        lines.append("            {")
        lines.append(f"                isSuccess &= item->get{cc}()->fromXElement(message, *it);")
        lines.append(f"                item->setHas{cc}(true);")
        lines.append("                decrementIter = true;")
        lines.append("                ++it;")
        lines.append("            }")
        lines.append("            if (it == endIter)")
        lines.append("            {")
        lines.append(f"                addGroup(item, isFirst{chord_group_cls}Added);")
        lines.append("                MX_RETURN_IS_SUCCESS;")
        lines.append("            }")
    # unbounded (degree)
    lines.append("")
    lines.append(f'            while (it != endIter && it->getName() == "{unbounded}")')
    lines.append("            {")
    lines.append(f"                auto {camel(unbounded)} = make{unbounded_cls}();")
    lines.append(f"                isSuccess &= {camel(unbounded)}->fromXElement(message, *it);")
    lines.append(f"                item->add{unbounded_cls}({camel(unbounded)});")
    lines.append("                decrementIter = true;")
    lines.append("                ++it;")
    lines.append("            }")
    lines.append("")
    lines.append(f"            addGroup(item, isFirst{chord_group_cls}Added);")
    lines.append("")
    lines.append("            if (decrementIter)")
    lines.append("            {")
    lines.append("                --it;")
    lines.append("            }")
    lines.append("            continue;")
    lines.append("        }")
    lines.append("")
    # frame/offset
    for n in optional_elems:
        cc = pascal(n)
        lines.append(f"        if (importElement(message, *it, isSuccess, *my{cc}, myHas{cc}))")
        lines.append("        {")
        lines.append("            continue;")
        lines.append("        }")
    # editorial group
    lines.append(f"        importGroup(message, it, endIter, isSuccess, my{editorial_group_cls});")
    # staff
    if staff_cls:
        lines.append(f"        if (importElement(message, *it, isSuccess, *my{staff_cls}, myHas{staff_cls}))")
        lines.append("        {")
        lines.append("            continue;")
        lines.append("        }")
    lines.append("    }")
    lines.append("")
    lines.append("    if (!isChoiceFound)")
    lines.append("    {")
    # Quoted branch names: emit ' as a backslash-escape only when the
    # surrounding C++ string is wrapped in single quotes; here it's a
    # double-quoted C++ string so plain ' is correct.
    branch_names_quoted = " or ".join(f"'{n}'" for n in branches)
    lines.append(f'        message << "{class_name}: either {branch_names_quoted} is required but neither was present" << std::endl;')
    lines.append("        isSuccess = false;")
    lines.append("    }")
    lines.append("")
    lines.append("    return isSuccess;")
    lines.append("}")
    lines.append("")

    # addGroup helper
    lines.append(f"void {class_name}::addGroup({chord_group_cls}Ptr &grp, bool &isFirst)")
    lines.append("{")
    lines.append(f"    if (!isFirst && my{chord_group_cls}Set.size() == 1)")
    lines.append("    {")
    lines.append(f"        *my{chord_group_cls}Set.begin() = grp;")
    lines.append("        isFirst = true;")
    lines.append("    }")
    lines.append("    else")
    lines.append("    {")
    lines.append(f"        my{chord_group_cls}Set.push_back(grp);")
    lines.append("        isFirst = true;")
    lines.append("    }")
    lines.append("}")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def _emit_harmony_family(elem_name, elem, ct, model, generated_attrs, stats):
    s = _extract_harmony_structure(ct, model)

    class_name = element_class_name(elem_name)                              # Harmony
    chord_group_cls = pascal(s["chord_group_name"]) + "Group"               # HarmonyChordGroup
    editorial_group_cls = pascal(s["editorial_group"]) + "Group"            # EditorialGroup
    staff_cls = pascal(s["staff_group"]) if s["staff_group"] else None      # Staff

    # 1. Attrs struct
    type_name = elem.type_name or ""
    attrs_name = None
    if ct.attributes:
        attrs_name = resolve_attrs_name(elem_name, type_name, model)
        if attrs_name not in generated_attrs and attrs_name not in CORE_ROOT_ATTRS:
            h = generate_attrs_h(attrs_name, ct.attributes, model)
            c = generate_attrs_cpp(attrs_name, ct.attributes, model)
            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.h"), h)
            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.cpp"), c)
            generated_attrs.add(attrs_name)
            stats["attrs_written"] += 1

    # 2. HarmonyChordGroup
    h = generate_harmony_chord_group_h(chord_group_cls, s["inner"])
    c = generate_harmony_chord_group_cpp(chord_group_cls, s["inner"])
    write_file(os.path.join(ELEM_DIR, f"{chord_group_cls}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{chord_group_cls}.cpp"), c)

    # 3. Harmony
    h = generate_harmony_h(class_name, attrs_name, chord_group_cls,
                            s["optional_elems"], editorial_group_cls, staff_cls)
    c = generate_harmony_cpp(elem_name, class_name, attrs_name, chord_group_cls,
                              s["inner"], s["optional_elems"],
                              editorial_group_cls, staff_cls)
    write_file(os.path.join(ELEM_DIR, f"{class_name}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{class_name}.cpp"), c)

    stats["elem_written"] += 1
    stats["bespoke_written"] = stats.get("bespoke_written", 0) + 1


def _emit_part_list_family(elem_name, elem, ct, model, generated_attrs, stats):
    s = _extract_part_list_structure(ct)

    class_name = element_class_name(elem_name)              # PartList
    branches = s["choice_branches"]                         # ['part-group', 'score-part']
    # Choice class name composed from the two branch names:
    # pascal(b[0]) + 'Or' + pascal(b[1]). For part-list this is
    # 'PartGroupOrScorePart', matching the committed codegen.
    assert len(branches) == 2, "part-list: expected exactly 2 choice branches"
    choice_class_name = pascal(branches[0]) + "Or" + pascal(branches[1])

    # No attributes on part-list.
    if ct.attributes:
        attrs_name = resolve_attrs_name(elem_name, elem.type_name or "", model)
        if attrs_name not in generated_attrs and attrs_name not in CORE_ROOT_ATTRS:
            h = generate_attrs_h(attrs_name, ct.attributes, model)
            c = generate_attrs_cpp(attrs_name, ct.attributes, model)
            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.h"), h)
            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.cpp"), c)
            generated_attrs.add(attrs_name)
            stats["attrs_written"] += 1

    # 1. PartGroupOrScorePart
    h = generate_part_group_or_score_part_h(choice_class_name, branches)
    c = generate_part_group_or_score_part_cpp(choice_class_name, branches)
    write_file(os.path.join(ELEM_DIR, f"{choice_class_name}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{choice_class_name}.cpp"), c)

    # 2. PartList
    h = generate_part_list_h(class_name, s["leading_unbounded"], s["singleton"], choice_class_name)
    c = generate_part_list_cpp(elem_name, class_name, s["leading_unbounded"], s["singleton"],
                               choice_class_name, branches)
    write_file(os.path.join(ELEM_DIR, f"{class_name}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{class_name}.cpp"), c)

    stats["elem_written"] += 1
    stats["bespoke_written"] = stats.get("bespoke_written", 0) + 1


# ---------------------------------------------------------------------------
# score-partwise / score-timewise family (bespoke, schema-driven)
#
# Both top-level wrappers live as inline anonymous complex types inside
# top-level xs:element declarations, with the same three-level shape:
#   <xs:element name="score-partwise">          (or "score-timewise")
#     <xs:complexType>
#       <xs:sequence>
#         <xs:group ref="score-header"/>
#         <xs:element name="part" maxOccurs="unbounded">    ("measure" for timewise)
#           <xs:complexType>
#             <xs:sequence>
#               <xs:element name="measure" maxOccurs="unbounded"> ("part" for timewise)
#                 <xs:complexType>
#                   <xs:group ref="music-data"/>
#                   <xs:attributeGroup ref="measure-attributes"/> (or part-attributes)
#                 </xs:complexType>
#               </xs:element>
#             </xs:sequence>
#             <xs:attributeGroup ref="part-attributes"/>   (or measure-attributes)
#           </xs:complexType>
#         </xs:element>
#       </xs:sequence>
#       <xs:attributeGroup ref="document-attributes"/>
#     </xs:complexType>
#   </xs:element>
#
# The standard ``_parse_elements`` path captures the outer level but the
# inner XML name collisions (``part`` and ``measure`` recur across both
# wrappers) and the direct ``xs:group ref`` at CT level (no wrapping
# sequence) cause partial data. The bespoke handler walks ``model.root``
# directly to recover the full nested structure.
#
# Structural roles (consistent across both flavors):
#   outer            -> ScorePartwise   / ScoreTimewise
#   set_holder       -> PartwisePart    / TimewiseMeasure   (mid level; holds
#                       a set of music_data_holder children)
#   music_data_holder-> PartwiseMeasure / TimewisePart      (deepest; holds
#                       a MusicDataGroup with the choice classes)
#
# The hand-written original code applies a small number of per-flavor
# quirks (extra includes, JIT vs eager attrs, message wording, presence of
# MX_DEBUG_THROW_ON_PARSE_ISSUE, post-loop required-child check, ...). All
# such knobs are captured in SCORE_WRAPPER_FLAVOR_CONFIG below so the same
# shared handler can emit either family.
# ---------------------------------------------------------------------------


# Per-flavor knobs that capture hand-written variations between the partwise
# and timewise families. Keys are the outer XSD element names.
SCORE_WRAPPER_FLAVOR_CONFIG = {
    "score-partwise": {
        # ScorePartwise.cpp
        "outer_extra_includes": [],
        "outer_loop_uses_end_var": False,
        # PartwiseMeasure (music-data holder)
        "music_data_holder_attrs_jit": True,
        "music_data_holder_debug_throw": True,
        # PartwisePart (set holder)
        "set_holder_clear_repushes_default": True,
        "set_holder_remove_has_size_guard": True,
        "set_holder_post_loop_required": False,
        "set_holder_first_flag_name": "isFirstAdded",
        "set_holder_use_return_macro": True,
        # Loop body style differences (partwise variant).
        "set_holder_loop_uses_element_name_var": False,
        "set_holder_unexpected_order": "message_first",   # message << ...; isSuccess = false;
        "set_holder_unexpected_msg": "encountered_quoted", # "...: encountered an unexpected element '...'"
        "set_holder_begin_deref_parens": False,           # *mySet.begin() vs *(mySet.begin())
        "set_holder_from_x_before_first_check": True,
        "set_holder_blank_after_first_decl": False,
        "set_holder_blank_inside_else": False,
        "set_holder_child_var_source": "xml_name",        # "xml_name" => camel(xml); "class_name" => pascal_to_camel(cls)
    },
    "score-timewise": {
        # ScoreTimewise.cpp
        "outer_extra_includes": [
            "ezxml/XElement.h",
            "ezxml/XElementIterator.h",
        ],
        "outer_loop_uses_end_var": True,
        # TimewisePart (music-data holder)
        "music_data_holder_attrs_jit": False,
        "music_data_holder_debug_throw": False,
        # TimewiseMeasure (set holder)
        "set_holder_clear_repushes_default": False,
        "set_holder_remove_has_size_guard": False,
        "set_holder_post_loop_required": True,
        "set_holder_first_flag_name": "isFirstTimewisePartFound",
        "set_holder_use_return_macro": False,
        # Loop body style differences (timewise variant).
        "set_holder_loop_uses_element_name_var": True,
        "set_holder_unexpected_order": "issuccess_first", # isSuccess = false; message << ...;
        "set_holder_unexpected_msg": "trailing_encountered", # "...: unexpected element '...' encountered"
        "set_holder_begin_deref_parens": True,
        "set_holder_from_x_before_first_check": True,
        "set_holder_blank_after_first_decl": True,
        "set_holder_blank_inside_else": True,
        "set_holder_child_var_source": "class_name",
    },
}


def _extract_score_wrapper_structure(model, outer_name):
    """Walk model.root to recover the full nested structure of a top-level
    score wrapper. Returns a dict with role-based keys; no XML name is
    hardcoded here -- every name flows from the parsed XSD."""
    outer_el = None
    for el in model.root.iter(f"{XS}element"):
        if el.get("name") == outer_name:
            outer_el = el
            break
    if outer_el is None:
        raise RuntimeError(
            f"score wrapper '{outer_name}' not found in XSD")

    outer_ct = outer_el.find(f"{XS}complexType")
    outer_seq = outer_ct.find(f"{XS}sequence")
    outer_attr_group = outer_ct.find(f"{XS}attributeGroup").get("ref")

    header_group = outer_seq.find(f"{XS}group").get("ref")

    inner_el = outer_seq.find(f"{XS}element")
    inner_name = inner_el.get("name")
    inner_ct = inner_el.find(f"{XS}complexType")
    inner_attr_group = inner_ct.find(f"{XS}attributeGroup").get("ref")
    inner_seq = inner_ct.find(f"{XS}sequence")

    deepest_el = inner_seq.find(f"{XS}element")
    deepest_name = deepest_el.get("name")
    deepest_ct = deepest_el.find(f"{XS}complexType")
    deepest_content_group = deepest_ct.find(f"{XS}group").get("ref")
    deepest_attr_group = deepest_ct.find(f"{XS}attributeGroup").get("ref")

    return {
        "outer_name": outer_name,
        "outer_attr_group": outer_attr_group,        # document-attributes
        "header_group": header_group,                # score-header
        "inner_name": inner_name,                    # part
        "inner_attr_group": inner_attr_group,        # part-attributes
        "deepest_name": deepest_name,                # measure
        "deepest_content_group": deepest_content_group,  # music-data
        "deepest_attr_group": deepest_attr_group,    # measure-attributes
    }


def _score_flavor_prefix(outer_name):
    """Derive the mx/core flavor prefix from outer XSD element name.
    'score-partwise' -> 'Partwise', 'score-timewise' -> 'Timewise'."""
    if not outer_name.startswith("score-"):
        raise RuntimeError(
            f"_score_flavor_prefix expects 'score-X', got '{outer_name}'")
    return pascal(outer_name[len("score-"):])


def _generate_score_outer_h(class_name, attrs_name, header_group_cls,
                             child_cls, fwd_order):
    """Emit ScorePartwise.h (or ScoreTimewise.h with appropriate inputs)."""
    lines = []
    lines.append("// MusicXML Class Library")
    lines.append("// Copyright (c) by Matthew James Briggs")
    lines.append("// Distributed under the MIT License")
    lines.append("")
    lines.append("#pragma once")
    lines.append("")
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append(f'#include "mx/core/elements/{attrs_name}.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx")
    lines.append("{")
    lines.append("namespace core")
    lines.append("{")
    lines.append("")
    lines.append(f"MX_FORWARD_DECLARE_ATTRIBUTES({attrs_name})")
    for n in fwd_order:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({n})")
    lines.append("")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append("")
    lines.append(f"class {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append(f"    {attrs_name}Ptr getAttributes() const;")
    lines.append(f"    void setAttributes(const {attrs_name}Ptr &value);")
    lines.append("")
    lines.append(f"    /* _________ {header_group_cls} minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {header_group_cls}Ptr get{header_group_cls}() const;")
    lines.append(f"    void set{header_group_cls}(const {header_group_cls}Ptr &value);")
    lines.append("")
    lines.append(f"    /* _________ {child_cls} minOccurs = 1, maxOccurs = unbounded _________ */")
    lines.append(f"    const {child_cls}Set &get{child_cls}Set() const;")
    lines.append(f"    void add{child_cls}(const {child_cls}Ptr &value);")
    lines.append(f"    void remove{child_cls}(const {child_cls}SetIterConst &value);")
    lines.append(f"    void clear{child_cls}Set();")
    lines.append(f"    {child_cls}Ptr get{child_cls}(const {child_cls}SetIterConst &setIterator) const;")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append(f"    {attrs_name}Ptr myAttributes;")
    lines.append(f"    {header_group_cls}Ptr my{header_group_cls};")
    lines.append(f"    {child_cls}Set my{child_cls}Set;")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def _generate_score_outer_cpp(outer_xml_name, class_name, attrs_name,
                                header_group_cls, child_cls, inner_xml_name,
                                header_skip_names, flavor_cfg):
    """Emit ScorePartwise.cpp or ScoreTimewise.cpp.

    header_skip_names: list of XML element names (from score-header group) that
    the loop silently ``continue``s past because ScoreHeaderGroup already
    consumed them. Schema-driven from model.groups['score-header'].
    flavor_cfg: per-flavor knobs from SCORE_WRAPPER_FLAVOR_CONFIG.
    """
    is_first_var = "isFirst" + pascal(inner_xml_name) + "Added"
    lines = []
    lines.append("// MusicXML Class Library")
    lines.append("// Copyright (c) by Matthew James Briggs")
    lines.append("// Distributed under the MIT License")
    lines.append("")
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    for inc in flavor_cfg.get("outer_extra_includes", []):
        lines.append(f'#include "{inc}"')
    lines.append('#include "mx/core/FromXElement.h"')
    lines.append(f'#include "mx/core/elements/{child_cls}.h"')
    lines.append(f'#include "mx/core/elements/{header_group_cls}.h"')
    lines.append("#include <iostream>")
    lines.append("")
    lines.append("namespace mx")
    lines.append("{")
    lines.append("namespace core")
    lines.append("{")
    lines.append(f"{class_name}::{class_name}()")
    lines.append(f"    : myAttributes(std::make_shared<{attrs_name}>()), my{header_group_cls}(make{header_group_cls}()),")
    lines.append(f"      my{child_cls}Set()")
    lines.append("{")
    lines.append(f"    my{child_cls}Set.push_back(make{child_cls}());")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return myAttributes->hasValues();")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return myAttributes->toStream(os);")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append(f'    os << "{outer_xml_name}";')
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    os << std::endl;")
    lines.append(f"    my{header_group_cls}->streamContents(os, indentLevel + 1, isOneLineOnly);")
    lines.append(f"    for (auto x : my{child_cls}Set)")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append("        x->toStream(os, indentLevel + 1);")
    lines.append("    }")
    lines.append("    os << std::endl;")
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"{attrs_name}Ptr {class_name}::getAttributes() const")
    lines.append("{")
    lines.append("    return myAttributes;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::setAttributes(const {attrs_name}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append("        myAttributes = value;")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"{header_group_cls}Ptr {class_name}::get{header_group_cls}() const")
    lines.append("{")
    lines.append(f"    return my{header_group_cls};")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::set{header_group_cls}(const {header_group_cls}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{header_group_cls} = value;")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"const {child_cls}Set &{class_name}::get{child_cls}Set() const")
    lines.append("{")
    lines.append(f"    return my{child_cls}Set;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::remove{child_cls}(const {child_cls}SetIterConst &value)")
    lines.append("{")
    lines.append(f"    if (value != my{child_cls}Set.cend())")
    lines.append("    {")
    lines.append(f"        if (my{child_cls}Set.size() > 1)")
    lines.append("        {")
    lines.append(f"            my{child_cls}Set.erase(value);")
    lines.append("        }")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::add{child_cls}(const {child_cls}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{child_cls}Set.push_back(value);")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::clear{child_cls}Set()")
    lines.append("{")
    lines.append(f"    my{child_cls}Set.clear();")
    lines.append(f"    my{child_cls}Set.push_back(make{child_cls}());")
    lines.append("}")
    lines.append("")
    lines.append(f"{child_cls}Ptr {class_name}::get{child_cls}(const {child_cls}SetIterConst &setIterator) const")
    lines.append("{")
    lines.append(f"    if (setIterator != my{child_cls}Set.cend())")
    lines.append("    {")
    lines.append("        return *setIterator;")
    lines.append("    }")
    lines.append(f"    return {child_cls}Ptr();")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    lines.append(f"    bool {is_first_var} = false;")
    lines.append("")
    lines.append("    isSuccess &= myAttributes->fromXElement(message, xelement);")
    lines.append(f"    isSuccess &= my{header_group_cls}->fromXElement(message, xelement);")
    lines.append("")
    lines.append("    auto it = xelement.begin();")
    lines.append("    auto end = xelement.end();")
    lines.append("")
    if flavor_cfg.get("outer_loop_uses_end_var", False):
        lines.append("    for (; it != end; ++it)")
    else:
        lines.append("    for (; it != xelement.end(); ++it)")
    lines.append("    {")
    lines.append("        const std::string elementName = it->getName();")
    lines.append("")
    skip_check = " || ".join(f'elementName == "{n}"' for n in header_skip_names)
    lines.append(f"        if ({skip_check})")
    lines.append("        {")
    lines.append("            continue;")
    lines.append("        }")
    lines.append(f'        else if (elementName == "{inner_xml_name}")')
    lines.append("        {")
    lines.append(f"            auto {camel(inner_xml_name)} = make{child_cls}();")
    lines.append("")
    lines.append(f"            if (!{is_first_var} && my{child_cls}Set.size() == 1)")
    lines.append("            {")
    lines.append(f"                *(my{child_cls}Set.begin()) = {camel(inner_xml_name)};")
    lines.append(f"                {is_first_var} = true;")
    lines.append("            }")
    lines.append("            else")
    lines.append("            {")
    lines.append(f"                my{child_cls}Set.push_back({camel(inner_xml_name)});")
    lines.append(f"                {is_first_var} = true;")
    lines.append("            }")
    lines.append("")
    lines.append(f"            isSuccess &= {camel(inner_xml_name)}->fromXElement(message, *it);")
    lines.append("        }")
    lines.append("    }")
    lines.append("")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}")
    lines.append("")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"
def _generate_partwise_part_h(class_name, attrs_name, child_cls):
    """Header for PartwisePart: wraps an unbounded set of PartwiseMeasure."""
    lines = []
    lines.append("// MusicXML Class Library")
    lines.append("// Copyright (c) by Matthew James Briggs")
    lines.append("// Distributed under the MIT License")
    lines.append("")
    lines.append("#pragma once")
    lines.append("")
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append(f'#include "mx/core/elements/{attrs_name}.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx")
    lines.append("{")
    lines.append("namespace core")
    lines.append("{")
    lines.append("")
    lines.append(f"MX_FORWARD_DECLARE_ATTRIBUTES({attrs_name})")
    # Forward declares ordered: PartwiseMeasure < PartwisePart.
    for n in sorted([child_cls, class_name]):
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({n})")
    lines.append("")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append("")
    lines.append(f"class {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append(f"    {attrs_name}Ptr getAttributes() const;")
    lines.append(f"    void setAttributes(const {attrs_name}Ptr &value);")
    lines.append("")
    lines.append(f"    /* _________ {child_cls} minOccurs = 0, maxOccurs = unbounded _________ */")
    lines.append(f"    const {child_cls}Set &get{child_cls}Set() const;")
    lines.append(f"    void add{child_cls}(const {child_cls}Ptr &value);")
    lines.append(f"    void remove{child_cls}(const {child_cls}SetIterConst &value);")
    lines.append(f"    void clear{child_cls}Set();")
    lines.append(f"    {child_cls}Ptr get{child_cls}(const {child_cls}SetIterConst &setIterator) const;")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append(f"    {attrs_name}Ptr myAttributes;")
    lines.append(f"    {child_cls}Set my{child_cls}Set;")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def _generate_partwise_part_cpp(outer_xml_name, class_name, attrs_name,
                                  child_cls, child_xml_name, flavor_cfg):
    """Cpp for the set holder (PartwisePart / TimewiseMeasure).

    outer_xml_name is the XSD name (typically 'part' for partwise,
    'measure' for timewise) used in streamName.
    flavor_cfg: per-flavor knobs from SCORE_WRAPPER_FLAVOR_CONFIG.
    """
    is_first_var = flavor_cfg.get("set_holder_first_flag_name", "isFirstAdded")
    if flavor_cfg.get("set_holder_child_var_source", "xml_name") == "class_name":
        child_var = pascal_to_camel(child_cls)
    else:
        child_var = camel(child_xml_name)
    lines = []
    lines.append("// MusicXML Class Library")
    lines.append("// Copyright (c) by Matthew James Briggs")
    lines.append("// Distributed under the MIT License")
    lines.append("")
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    lines.append(f'#include "mx/core/elements/{child_cls}.h"')
    lines.append("#include <iostream>")
    lines.append("")
    lines.append("namespace mx")
    lines.append("{")
    lines.append("namespace core")
    lines.append("{")
    lines.append(f"{class_name}::{class_name}() : myAttributes(std::make_shared<{attrs_name}>()), my{child_cls}Set()")
    lines.append("{")
    lines.append(f"    my{child_cls}Set.push_back(make{child_cls}());")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return myAttributes->hasValues();")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return myAttributes->toStream(os);")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append(f'    os << "{outer_xml_name}";')
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    if (hasContents())")
    lines.append("    {")
    lines.append(f"        for (auto x : my{child_cls}Set)")
    lines.append("        {")
    lines.append("            os << std::endl;")
    lines.append("            x->toStream(os, indentLevel + 1);")
    lines.append("        }")
    lines.append("        os << std::endl;")
    lines.append("        isOneLineOnly = false;")
    lines.append("    }")
    lines.append("    else")
    lines.append("    {")
    lines.append("        isOneLineOnly = true;")
    lines.append("    }")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"{attrs_name}Ptr {class_name}::getAttributes() const")
    lines.append("{")
    lines.append("    return myAttributes;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::setAttributes(const {attrs_name}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append("        myAttributes = value;")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"const {child_cls}Set &{class_name}::get{child_cls}Set() const")
    lines.append("{")
    lines.append(f"    return my{child_cls}Set;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::add{child_cls}(const {child_cls}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{child_cls}Set.push_back(value);")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::remove{child_cls}(const {child_cls}SetIterConst &value)")
    lines.append("{")
    lines.append(f"    if (value != my{child_cls}Set.cend())")
    lines.append("    {")
    if flavor_cfg.get("set_holder_remove_has_size_guard", True):
        lines.append(f"        if (my{child_cls}Set.size() > 1)")
        lines.append("        {")
        lines.append(f"            my{child_cls}Set.erase(value);")
        lines.append("        }")
    else:
        lines.append(f"        my{child_cls}Set.erase(value);")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::clear{child_cls}Set()")
    lines.append("{")
    lines.append(f"    my{child_cls}Set.clear();")
    if flavor_cfg.get("set_holder_clear_repushes_default", True):
        lines.append(f"    my{child_cls}Set.push_back(make{child_cls}());")
    lines.append("}")
    lines.append("")
    lines.append(f"{child_cls}Ptr {class_name}::get{child_cls}(const {child_cls}SetIterConst &setIterator) const")
    lines.append("{")
    lines.append(f"    if (setIterator != my{child_cls}Set.cend())")
    lines.append("    {")
    lines.append("        return *setIterator;")
    lines.append("    }")
    lines.append(f"    return {child_cls}Ptr();")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    lines.append("    isSuccess &= myAttributes->fromXElement(message, xelement);")
    if flavor_cfg.get("set_holder_blank_after_first_decl", False):
        lines.append("")
    lines.append(f"    bool {is_first_var} = false;")
    lines.append("")
    lines.append("    for (auto it = xelement.begin(); it != xelement.end(); ++it)")
    lines.append("    {")
    if flavor_cfg.get("set_holder_loop_uses_element_name_var", False):
        lines.append("        const std::string elementName = it->getName();")
        lines.append("")
        name_expr = "elementName"
    else:
        name_expr = "it->getName()"
    lines.append(f'        if ({name_expr} != "{child_xml_name}")')
    lines.append("        {")
    unexpected_msg = flavor_cfg.get("set_holder_unexpected_msg", "encountered_quoted")
    if unexpected_msg == "trailing_encountered":
        msg_line = (f'            message << "{class_name}: unexpected element \'"'
                    f' << {name_expr} << "\' encountered" << std::endl;')
    else:  # "encountered_quoted"
        msg_line = (f'            message << "{class_name}: encountered an unexpected element \'"'
                    f' << {name_expr} << "\'" << std::endl;')
    if flavor_cfg.get("set_holder_unexpected_order", "message_first") == "issuccess_first":
        lines.append("            isSuccess = false;")
        lines.append(msg_line)
    else:
        lines.append(msg_line)
        lines.append("            isSuccess = false;")
    lines.append("        }")
    lines.append("        else")
    lines.append("        {")
    lines.append(f"            auto {child_var} = make{child_cls}();")
    if flavor_cfg.get("set_holder_blank_inside_else", False):
        lines.append(f"            isSuccess &= {child_var}->fromXElement(message, *it);")
        lines.append("")
    else:
        lines.append(f"            isSuccess &= {child_var}->fromXElement(message, *it);")
    lines.append(f"            if (!{is_first_var} && my{child_cls}Set.size() == 1)")
    lines.append("            {")
    if flavor_cfg.get("set_holder_begin_deref_parens", False):
        lines.append(f"                *(my{child_cls}Set.begin()) = {child_var};")
    else:
        lines.append(f"                *my{child_cls}Set.begin() = {child_var};")
    lines.append(f"                {is_first_var} = true;")
    lines.append("            }")
    lines.append("            else")
    lines.append("            {")
    lines.append(f"                my{child_cls}Set.push_back({child_var});")
    lines.append(f"                {is_first_var} = true;")
    lines.append("            }")
    lines.append("        }")
    lines.append("    }")
    lines.append("")
    if flavor_cfg.get("set_holder_post_loop_required", False):
        lines.append(f"    if (!{is_first_var})")
        lines.append("    {")
        lines.append(f'        message << "{class_name}: no \'{child_xml_name}\' elements were found";')
        lines.append("        isSuccess = false;")
        lines.append("    }")
        lines.append("")
    if flavor_cfg.get("set_holder_use_return_macro", True):
        lines.append("    MX_RETURN_IS_SUCCESS;")
    else:
        lines.append("    return isSuccess;")
    lines.append("}")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"
def _generate_partwise_measure_h(class_name, attrs_name, music_data_group_cls,
                                   flavor_cfg):
    """Header for the music-data holder (PartwiseMeasure / TimewisePart).

    flavor_cfg.music_data_holder_attrs_jit controls whether the attrs member
    is JIT-allocated under MX_MUTEX (partwise) or eagerly held (timewise).
    """
    lines = []
    lines.append("// MusicXML Class Library")
    lines.append("// Copyright (c) by Matthew James Briggs")
    lines.append("// Distributed under the MIT License")
    lines.append("")
    lines.append("#pragma once")
    lines.append("")
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append(f'#include "mx/core/elements/{attrs_name}.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx")
    lines.append("{")
    lines.append("namespace core")
    lines.append("{")
    lines.append("")
    lines.append(f"MX_FORWARD_DECLARE_ATTRIBUTES({attrs_name})")
    for n in sorted([music_data_group_cls, class_name]):
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({n})")
    lines.append("")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append("")
    lines.append(f"class {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append(f"    {attrs_name}Ptr getAttributes() const;")
    lines.append(f"    void setAttributes(const {attrs_name}Ptr &value);")
    lines.append("")
    lines.append(f"    /* _________ {music_data_group_cls} minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {music_data_group_cls}Ptr get{music_data_group_cls}() const;")
    lines.append(f"    void set{music_data_group_cls}(const {music_data_group_cls}Ptr &value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    if flavor_cfg.get("music_data_holder_attrs_jit", True):
        lines.append("    MX_MUTEX")
        lines.append(f"    mutable {attrs_name}Ptr myAttributes;")
    else:
        lines.append(f"    {attrs_name}Ptr myAttributes;")
    lines.append(f"    {music_data_group_cls}Ptr my{music_data_group_cls};")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def _generate_partwise_measure_cpp(outer_xml_name, class_name, attrs_name,
                                     music_data_group_cls, music_data_choices,
                                     flavor_cfg):
    """Cpp for the music-data holder (PartwiseMeasure / TimewisePart).

    music_data_choices: list of dicts each with:
      xml_name        - XML element name ('note', 'attributes', ...)
      class_name      - C++ class name ('Note', 'Properties', ...)
      enum_name       - Choice enum name ('note', 'properties', ...)
    flavor_cfg: per-flavor knobs from SCORE_WRAPPER_FLAVOR_CONFIG.
    """
    attrs_jit = flavor_cfg.get("music_data_holder_attrs_jit", True)
    debug_throw = flavor_cfg.get("music_data_holder_debug_throw", True)
    # JIT flavor (partwise) refers to attrs via getAttributes() everywhere
    # to trigger the lazy allocation; eager flavor (timewise) accesses
    # myAttributes directly because it is initialized in the ctor.
    attrs_ref = "getAttributes()" if attrs_jit else "myAttributes"
    # Includes: alphabetical list of music-data choice classes, plus
    # MusicDataChoice and MusicDataGroup.
    choice_class_includes = sorted({c["class_name"] for c in music_data_choices})
    extra = sorted({"MusicDataChoice", music_data_group_cls})

    lines = []
    lines.append("// MusicXML Class Library")
    lines.append("// Copyright (c) by Matthew James Briggs")
    lines.append("// Distributed under the MIT License")
    lines.append("")
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    # The committed PartwiseMeasure.cpp has #includes sorted alphabetically
    # for each choice class then MusicDataChoice then MusicDataGroup then Note
    # (because Note alphabetically falls between MusicDataGroup and Print).
    # We compose the full sorted set:
    all_includes = sorted(set(choice_class_includes) | set(extra))
    for n in all_includes:
        lines.append(f'#include "mx/core/elements/{n}.h"')
    lines.append("#include <iostream>")
    lines.append("")
    lines.append("namespace mx")
    lines.append("{")
    lines.append("namespace core")
    lines.append("{")
    if attrs_jit:
        ctor_init = f"myAttributes(nullptr)"
    else:
        ctor_init = f"myAttributes(std::make_shared<{attrs_name}>())"
    lines.append(f"{class_name}::{class_name}() : {ctor_init}, my{music_data_group_cls}(make{music_data_group_cls}())")
    lines.append("{")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append(f"    return {attrs_ref}->hasValues();")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append(f"    return {attrs_ref}->toStream(os);")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append(f'    os << "{outer_xml_name}";')
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append(f"    return my{music_data_group_cls}->hasContents();")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    if (hasContents())")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append(f"        my{music_data_group_cls}->streamContents(os, indentLevel + 1, isOneLineOnly);")
    lines.append("        os << std::endl;")
    lines.append("        isOneLineOnly = false;")
    lines.append("    }")
    lines.append("    else")
    lines.append("    {")
    lines.append("        isOneLineOnly = true;")
    lines.append("    }")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"{attrs_name}Ptr {class_name}::getAttributes() const")
    lines.append("{")
    if attrs_jit:
        lines.append("    MX_LOCK")
        lines.append(f"    MX_JIT_ALLOCATE_ATTRIBUTES({attrs_name});")
    lines.append("    return myAttributes;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::setAttributes(const {attrs_name}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append("        myAttributes = value;")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"{music_data_group_cls}Ptr {class_name}::get{music_data_group_cls}() const")
    lines.append("{")
    lines.append(f"    return my{music_data_group_cls};")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::set{music_data_group_cls}(const {music_data_group_cls}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{music_data_group_cls} = value;")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    lines.append("    bool isFirstMusicDataChoiceAdded = false;")
    lines.append(f"    isSuccess &= {attrs_ref}->fromXElement(message, xelement);")
    lines.append("")
    lines.append("    for (auto it = xelement.begin(); it != xelement.end(); ++it)")
    lines.append("    {")
    lines.append("        const std::string elementName = it->getName();")
    lines.append("")
    lines.append("        auto choiceObject = makeMusicDataChoice();")
    lines.append("        bool choiceObjectShouldBeAdded = true;")
    lines.append("")
    for i, c in enumerate(music_data_choices):
        prefix = "if" if i == 0 else "else if"
        lines.append(f'        {prefix} (elementName == "{c["xml_name"]}")')
        lines.append("        {")
        lines.append(f"            choiceObject->setChoice(MusicDataChoice::Choice::{c['enum_name']});")
        lines.append(f"            isSuccess &= choiceObject->get{c['class_name']}()->fromXElement(message, *it);")
        if debug_throw:
            lines.append("            MX_DEBUG_THROW_ON_PARSE_ISSUE;")
        lines.append("        }")
    lines.append("        else")
    lines.append("        {")
    lines.append("            choiceObjectShouldBeAdded = false;")
    lines.append("        }")
    lines.append("")
    lines.append("        if (choiceObjectShouldBeAdded)")
    lines.append("        {")
    lines.append(f"            if (!isFirstMusicDataChoiceAdded && my{music_data_group_cls}->getMusicDataChoiceSet().size() == 1)")
    lines.append("            {")
    lines.append(f"                my{music_data_group_cls}->addMusicDataChoice(choiceObject);")
    lines.append(f"                my{music_data_group_cls}->removeMusicDataChoice(my{music_data_group_cls}->getMusicDataChoiceSet().cbegin());")
    lines.append("                isFirstMusicDataChoiceAdded = true;")
    lines.append("            }")
    lines.append("            else")
    lines.append("            {")
    lines.append(f"                my{music_data_group_cls}->addMusicDataChoice(choiceObject);")
    lines.append("                isFirstMusicDataChoiceAdded = true;")
    lines.append("            }")
    lines.append("        }")
    lines.append("    }")
    lines.append("")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}")
    lines.append("")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def _emit_score_wrapper_family(elem_name, elem, ct, model, generated_attrs, stats):
    """Shared bespoke handler for the score-partwise / score-timewise family.

    Emits the three classes that make up the wrapper (outer, set holder,
    music-data holder) plus their attribute structs from a single
    schema-driven structural walk of the XSD. Per-flavor hand-written
    variations are pulled from SCORE_WRAPPER_FLAVOR_CONFIG[elem_name].
    """
    flavor_cfg = SCORE_WRAPPER_FLAVOR_CONFIG.get(elem_name, {})
    s = _extract_score_wrapper_structure(model, elem_name)

    flavor = _score_flavor_prefix(elem_name)                       # "Partwise" / "Timewise"
    outer_class = element_class_name(elem_name)                    # "ScorePartwise" / "ScoreTimewise"
    inner_class = flavor + pascal(s["inner_name"])                 # "PartwisePart" / "TimewiseMeasure"
    deepest_class = flavor + pascal(s["deepest_name"])             # "PartwiseMeasure" / "TimewisePart"
    header_group_cls = pascal(s["header_group"]) + "Group"         # "ScoreHeaderGroup"
    music_data_group_cls = pascal(s["deepest_content_group"]) + "Group"  # "MusicDataGroup"

    # Attribute struct names follow the standard convention based on the
    # underlying element XML name; resolve_attrs_name returns:
    #   "score-partwise" -> "ScorePartwiseAttributes"
    #   "part"           -> "PartAttributes"
    #   "measure"        -> "MeasureAttributes"
    outer_attrs_name = resolve_attrs_name(elem_name, "", model)
    inner_attrs_name = resolve_attrs_name(s["inner_name"], "", model)
    deepest_attrs_name = resolve_attrs_name(s["deepest_name"], "", model)

    # 1. Outer attrs struct (document-attributes -> ScorePartwiseAttributes).
    if ct.attributes:
        if outer_attrs_name not in generated_attrs and outer_attrs_name not in CORE_ROOT_ATTRS:
            h = generate_attrs_h(outer_attrs_name, ct.attributes, model)
            c = generate_attrs_cpp(outer_attrs_name, ct.attributes, model)
            write_file(os.path.join(ELEM_DIR, f"{outer_attrs_name}.h"), h)
            write_file(os.path.join(ELEM_DIR, f"{outer_attrs_name}.cpp"), c)
            generated_attrs.add(outer_attrs_name)
            stats["attrs_written"] += 1

    # 2. Inner attrs struct (part-attributes -> PartAttributes).
    inner_attrs_list = model.attribute_groups.get(s["inner_attr_group"], [])
    if inner_attrs_list:
        if inner_attrs_name not in generated_attrs and inner_attrs_name not in CORE_ROOT_ATTRS:
            h = generate_attrs_h(inner_attrs_name, inner_attrs_list, model)
            c = generate_attrs_cpp(inner_attrs_name, inner_attrs_list, model)
            write_file(os.path.join(ELEM_DIR, f"{inner_attrs_name}.h"), h)
            write_file(os.path.join(ELEM_DIR, f"{inner_attrs_name}.cpp"), c)
            generated_attrs.add(inner_attrs_name)
            stats["attrs_written"] += 1

    # 3. Deepest attrs struct (measure-attributes -> MeasureAttributes).
    deepest_attrs_list = model.attribute_groups.get(s["deepest_attr_group"], [])
    if deepest_attrs_list:
        if deepest_attrs_name not in generated_attrs and deepest_attrs_name not in CORE_ROOT_ATTRS:
            h = generate_attrs_h(deepest_attrs_name, deepest_attrs_list, model)
            c = generate_attrs_cpp(deepest_attrs_name, deepest_attrs_list, model)
            write_file(os.path.join(ELEM_DIR, f"{deepest_attrs_name}.h"), h)
            write_file(os.path.join(ELEM_DIR, f"{deepest_attrs_name}.cpp"), c)
            generated_attrs.add(deepest_attrs_name)
            stats["attrs_written"] += 1

    # 4. ScorePartwise.h / .cpp
    # Forward declares in ScorePartwise.h come out as
    #   [PartwisePart, ScoreHeaderGroup, ScorePartwise]
    # which is sorted lexicographically.
    fwd_order = sorted([inner_class, header_group_cls, outer_class])
    # The score-header group's child element XML names are the children that
    # the outer fromXElementImpl should silently 'continue' past (they were
    # already consumed by ScoreHeaderGroup). Schema-driven from model.groups.
    header_skip_names = [c.element_name for c in model.groups.get(s["header_group"], [])]
    h = _generate_score_outer_h(outer_class, outer_attrs_name,
                                header_group_cls, inner_class, fwd_order)
    c = _generate_score_outer_cpp(elem_name, outer_class, outer_attrs_name,
                                   header_group_cls, inner_class,
                                   s["inner_name"], header_skip_names,
                                   flavor_cfg)
    write_file(os.path.join(ELEM_DIR, f"{outer_class}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{outer_class}.cpp"), c)

    # 5. PartwisePart.h / .cpp
    h = _generate_partwise_part_h(inner_class, inner_attrs_name, deepest_class)
    c = _generate_partwise_part_cpp(s["inner_name"], inner_class,
                                      inner_attrs_name, deepest_class,
                                      s["deepest_name"], flavor_cfg)
    write_file(os.path.join(ELEM_DIR, f"{inner_class}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{inner_class}.cpp"), c)

    # 6. PartwiseMeasure.h / .cpp
    # Build the MusicDataChoice schema-driven from model.groups["music-data"].
    # The music-data group is an xs:choice; _parse_groups would store the
    # choice children directly. Walk model.root to be safe.
    md_group = s["deepest_content_group"]  # "music-data"
    music_data_choices = []
    for group_node in model.root.iter(f"{XS}group"):
        if group_node.get("name") == md_group:
            choice_node = None
            seq_node = group_node.find(f"{XS}sequence")
            if seq_node is not None:
                choice_node = seq_node.find(f"{XS}choice")
            if choice_node is None:
                choice_node = group_node.find(f"{XS}choice")
            if choice_node is None:
                raise RuntimeError(f"group '{md_group}' has no xs:choice")
            for el in choice_node.findall(f"{XS}element"):
                xml_n = el.get("name") or el.get("ref")
                cls = element_class_name(xml_n)
                enum_n = pascal_to_camel(cls)
                music_data_choices.append({
                    "xml_name": xml_n,
                    "class_name": cls,
                    "enum_name": enum_n,
                })
            break

    h = _generate_partwise_measure_h(deepest_class, deepest_attrs_name,
                                       music_data_group_cls, flavor_cfg)
    c = _generate_partwise_measure_cpp(s["deepest_name"], deepest_class,
                                         deepest_attrs_name,
                                         music_data_group_cls,
                                         music_data_choices,
                                         flavor_cfg)
    write_file(os.path.join(ELEM_DIR, f"{deepest_class}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{deepest_class}.cpp"), c)

    stats["elem_written"] += 1
    stats["bespoke_written"] = stats.get("bespoke_written", 0) + 1


# ---------------------------------------------------------------------------
# Note (bespoke, schema-driven)
#
# note's complex type is structurally unique. It has:
#
#   sequence:
#     choice:                                    -> NoteChoice (synthetic)
#       sequence(grace, full-note group, tie*)   -> GraceNoteGroup (synthetic)
#       sequence(cue, full-note group, duration) -> CueNoteGroup   (synthetic)
#       sequence(full-note group, duration, tie*)-> NormalNoteGroup(synthetic)
#     <16 trailing children: instrument? editorial-voice group type? dot* ...>
#
# The full-note group is itself complex:
#
#   sequence:
#     chord?
#     choice(pitch | unpitched | rest)           -> FullNoteTypeChoice (synth)
#
# Note also has a 19-attribute NoteAttributes struct.
#
# The committed code uses MX_MUTEX / MX_LOCK / MX_JIT_ALLOCATE in Note (the
# only top-level element with lazy allocation) and in FullNoteGroup +
# FullNoteTypeChoice. The branch groups (Grace/Cue/Normal) eagerly construct
# all members.
#
# Note::fromXElementImpl is a hand-written state machine with three private
# helpers: parseNoteChoice, parseFullNoteGroup, parseEditorialVoiceGroup. All
# six synthetic helper classes use MX_FROM_XELEMENT_UNUSED -- Note owns the
# parse entirely.
#
# Schema-driven inputs (everything that should propagate when the XSD changes):
#   - The 3 outer-choice branches and their member sequences (model.elements,
#     content_tree).
#   - The trailing children (instrument, type, dot, ..., play).
#   - The full-note group's inner choice members (pitch/unpitched/rest) and
#     pre-choice element (chord).
#   - The editorial-voice group's members (footnote/level/voice).
#   - The duration group's single element name.
#   - The staff group's single element name.
#   - The note attributes (model.complex_types['note'].attributes).
#
# Hand-naming decisions (not in the XSD):
#   - The choice class name is 'NoteChoice'.
#   - The third (no-leading-element) branch's group class is 'NormalNoteGroup'
#     and its enum variant is 'normal'. Stored as NOTE_THIRD_BRANCH_*.
#   - The full-note inner choice class is 'FullNoteTypeChoice'.


# Naming decision for the third outer-choice branch (the one without a
# leading singleton element ref). The XSD does not give this branch a name;
# the original codegen called it 'Normal'.
NOTE_THIRD_BRANCH_CLASS = "NormalNoteGroup"
NOTE_THIRD_BRANCH_ENUM = "normal"


def _extract_note_structure(ct, model):
    """Return a structured dict describing note's content tree.

    Returns:
        {
          "branches": [BranchInfo, BranchInfo, BranchInfo],
          "trailing": [(kind, name, min, max), ...],  # outer-sequence trailing
          "full_note": {
              "pre": ["chord"],     # element refs before the inner choice
              "choice_branches": ["pitch", "unpitched", "rest"],
          },
          "editorial_voice": ["footnote", "level", "voice"],
          "duration": "duration",
          "staff": "staff",
        }

    BranchInfo is a dict:
        {
          "class_name": "GraceNoteGroup",
          "enum_name": "grace",
          "leading_element": "grace" or None,     # the singleton element name
          "members": [(kind, name, min, max), ...] # all members in order
          "tie_max": int or 0,
          "has_full_note": bool,
          "has_duration": bool,
        }
    """
    seq = ct.content_tree
    if not isinstance(seq, SequenceNode):
        raise RuntimeError(f"note: expected outer SequenceNode, got {type(seq).__name__}")
    if not seq.children or not isinstance(seq.children[0], ChoiceNode):
        raise RuntimeError("note: first child of outer sequence must be a ChoiceNode")

    outer_choice = seq.children[0]

    branches = []
    for branch in outer_choice.branches:
        if not isinstance(branch, SequenceNode):
            raise RuntimeError(
                f"note: outer choice branch must be SequenceNode, got {type(branch).__name__}")
        members = []
        leading_element = None
        tie_max = 0
        has_full_note = False
        has_duration = False
        for c in branch.children:
            if isinstance(c, ElementRefNode):
                members.append(("element", c.element_name, c.min_occurs, c.max_occurs))
                if leading_element is None and not members[:-1]:
                    leading_element = c.element_name
                if c.element_name == "tie":
                    tie_max = c.max_occurs
            elif isinstance(c, GroupRefNode):
                members.append(("group", c.group_name, c.min_occurs, c.max_occurs))
                if c.group_name == "full-note":
                    has_full_note = True
                elif c.group_name == "duration":
                    has_duration = True

        if leading_element == "grace":
            cls = "GraceNoteGroup"
            enum_n = "grace"
        elif leading_element == "cue":
            cls = "CueNoteGroup"
            enum_n = "cue"
        else:
            cls = NOTE_THIRD_BRANCH_CLASS
            enum_n = NOTE_THIRD_BRANCH_ENUM
        branches.append({
            "class_name": cls,
            "enum_name": enum_n,
            "leading_element": leading_element,
            "members": members,
            "tie_max": tie_max,
            "has_full_note": has_full_note,
            "has_duration": has_duration,
        })

    trailing = []
    for c in seq.children[1:]:
        if isinstance(c, ElementRefNode):
            trailing.append(("element", c.element_name, c.min_occurs, c.max_occurs))
        elif isinstance(c, GroupRefNode):
            trailing.append(("group", c.group_name, c.min_occurs, c.max_occurs))

    # full-note group: walk model.root because _parse_groups drops the inner choice.
    fn_pre = []
    fn_choice = []
    for g in model.root.iter(f"{XS}group"):
        if g.get("name") != "full-note":
            continue
        sq = g.find(f"{XS}sequence")
        if sq is None:
            raise RuntimeError("full-note group missing xs:sequence")
        for child in sq:
            tag = child.tag.split("}")[-1]
            if tag == "element":
                fn_pre.append(child.get("name") or child.get("ref"))
            elif tag == "choice":
                for el in child.findall(f"{XS}element"):
                    fn_choice.append(el.get("name") or el.get("ref"))
        break

    ev = [c.element_name for c in model.groups.get("editorial-voice", [])]
    dur_elems = [c.element_name for c in model.groups.get("duration", [])]
    staff_elems = [c.element_name for c in model.groups.get("staff", [])]

    return {
        "branches": branches,
        "trailing": trailing,
        "full_note": {"pre": fn_pre, "choice_branches": fn_choice},
        "editorial_voice": ev,
        "duration": dur_elems[0] if dur_elems else "duration",
        "staff": staff_elems[0] if staff_elems else "staff",
    }


# ---------------------------------------------------------------------------
# FullNoteTypeChoice (synthetic choice over pitch | unpitched | rest)
# ---------------------------------------------------------------------------

def generate_full_note_type_choice_h(class_name, branch_names):
    """branch_names: ordered list of XSD element names, e.g. ['pitch','unpitched','rest']"""
    branch_classes = [pascal(b) for b in branch_names]
    fwds = sorted(set(branch_classes)) + [class_name]
    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    for f in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({f})")
    lines.append("")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append("")
    lines.append(f"class {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append("    enum class Choice")
    lines.append("    {")
    for i, b in enumerate(branch_names):
        comma = "," if i < len(branch_names) - 1 else ""
        lines.append(f"        {camel(b)} = {i + 1}{comma}")
    lines.append("    };")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append("")
    lines.append("    /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {class_name}::Choice getChoice() const;")
    lines.append(f"    void setChoice(const {class_name}::Choice value);")
    for b, bc in zip(branch_names, branch_classes):
        lines.append("")
        lines.append(f"    /* _________ {bc} minOccurs = 1, maxOccurs = 1 _________ */")
        lines.append(f"    {bc}Ptr get{bc}() const;")
        lines.append(f"    void set{bc}(const {bc}Ptr &value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append("    MX_MUTEX")
    lines.append("    Choice myChoice;")
    for bc in branch_classes:
        lines.append(f"    mutable {bc}Ptr my{bc};")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_full_note_type_choice_cpp(class_name, branch_names):
    branch_classes = [pascal(b) for b in branch_names]
    includes = sorted(set([f"mx/core/elements/{bc}.h" for bc in branch_classes]))
    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in includes:
        lines.append(f'#include "{inc}"')
    lines.append("#include <iostream>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{")
    # Constructor
    inits = [f"myChoice(Choice::{camel(branch_names[0])})"]
    for bc in branch_classes:
        inits.append(f"my{bc}(make{bc}())")
    lines.append(f"{class_name}::{class_name}()")
    lines.append(f"    : {', '.join(inits)}")
    lines.append("{")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("")
    lines.append("    MX_UNUSED(indentLevel);")
    lines.append("    MX_UNUSED(isOneLineOnly);")
    lines.append("")
    lines.append("    switch (myChoice)")
    lines.append("    {")
    for b, bc in zip(branch_names, branch_classes):
        lines.append(f"    case Choice::{camel(b)}:")
        lines.append(f"        get{bc}()->toStream(os, indentLevel);")
        lines.append("        break;")
    lines.append("    default:")
    lines.append("        break;")
    lines.append("    }")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"{class_name}::Choice {class_name}::getChoice() const")
    lines.append("{")
    lines.append("    return myChoice;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::setChoice(const {class_name}::Choice value)")
    lines.append("{")
    lines.append("    myChoice = value;")
    lines.append("}")
    for bc in branch_classes:
        lines.append("")
        lines.append(f"{bc}Ptr {class_name}::get{bc}() const")
        lines.append("{")
        lines.append("    MX_LOCK")
        lines.append(f"    MX_JIT_ALLOCATE({bc});")
        lines.append(f"    return my{bc};")
        lines.append("}")
        lines.append("")
        lines.append(f"void {class_name}::set{bc}(const {bc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{bc} = value;")
        lines.append("    }")
        lines.append("}")
    lines.append("")
    lines.append(f"MX_FROM_XELEMENT_UNUSED({class_name});")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# FullNoteGroup (chord? + FullNoteTypeChoice)
# ---------------------------------------------------------------------------

def generate_full_note_group_h(class_name, pre_elems, inner_choice_cls):
    """pre_elems: list of element names before the inner choice (e.g. ['chord'])"""
    fwds = sorted(set([pascal(e) for e in pre_elems] + [inner_choice_cls])) + [class_name]
    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    for f in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({f})")
    lines.append("")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append("")
    lines.append(f"class {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    for e in pre_elems:
        ec = pascal(e)
        lines.append("")
        lines.append(f"    /* _________ {ec} minOccurs = 0, maxOccurs = 1 _________ */")
        lines.append(f"    {ec}Ptr get{ec}() const;")
        lines.append(f"    void set{ec}(const {ec}Ptr &value);")
        lines.append(f"    bool getHas{ec}() const;")
        lines.append(f"    void setHas{ec}(const bool value);")
    lines.append("")
    lines.append(f"    /* _________ {inner_choice_cls} minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {inner_choice_cls}Ptr get{inner_choice_cls}() const;")
    lines.append(f"    void set{inner_choice_cls}(const {inner_choice_cls}Ptr &value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append("    MX_MUTEX")
    for e in pre_elems:
        ec = pascal(e)
        lines.append(f"    {ec}Ptr my{ec};")
        lines.append(f"    bool myHas{ec};")
    lines.append(f"    mutable {inner_choice_cls}Ptr my{inner_choice_cls};")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_full_note_group_cpp(class_name, pre_elems, inner_choice_cls):
    includes = sorted(set([f"mx/core/elements/{pascal(e)}.h" for e in pre_elems] +
                          [f"mx/core/elements/{inner_choice_cls}.h"]))
    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in includes:
        lines.append(f'#include "{inc}"')
    lines.append("#include <iostream>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{")
    inits = []
    for e in pre_elems:
        ec = pascal(e)
        inits.append(f"my{ec}(make{ec}())")
        inits.append(f"myHas{ec}(false)")
    inits.append(f"my{inner_choice_cls}(nullptr)")
    lines.append(f"{class_name}::{class_name}() : {', '.join(inits)}")
    lines.append("{")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    for e in pre_elems:
        ec = pascal(e)
        lines.append(f"    if (myHas{ec})")
        lines.append("    {")
        lines.append(f"        my{ec}->toStream(os, indentLevel);")
        lines.append("        os << std::endl;")
        lines.append("    }")
    lines.append(f"    get{inner_choice_cls}()->streamContents(os, indentLevel, isOneLineOnly);")
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}")
    for e in pre_elems:
        ec = pascal(e)
        lines.append("")
        lines.append(f"{ec}Ptr {class_name}::get{ec}() const")
        lines.append("{")
        lines.append(f"    return my{ec};")
        lines.append("}")
        lines.append("")
        lines.append(f"void {class_name}::set{ec}(const {ec}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{ec} = value;")
        lines.append("    }")
        lines.append("}")
        lines.append("")
        lines.append(f"bool {class_name}::getHas{ec}() const")
        lines.append("{")
        lines.append(f"    return myHas{ec};")
        lines.append("}")
        lines.append("")
        lines.append(f"void {class_name}::setHas{ec}(const bool value)")
        lines.append("{")
        lines.append(f"    myHas{ec} = value;")
        lines.append("}")
    lines.append("")
    lines.append(f"{inner_choice_cls}Ptr {class_name}::get{inner_choice_cls}() const")
    lines.append("{")
    lines.append("    MX_LOCK")
    lines.append(f"    MX_JIT_ALLOCATE({inner_choice_cls});")
    lines.append(f"    return my{inner_choice_cls};")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::set{inner_choice_cls}(const {inner_choice_cls}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{inner_choice_cls} = value;")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"MX_FROM_XELEMENT_UNUSED({class_name});")
    lines.append("")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Outer-choice branch group (Grace/Cue/Normal)
# Each branch is a sequence of element/group refs. The committed branch
# classes share a uniform shape: each member is either a singleton element
# (chord-like: get/set/has) or a set (Tie, max=2 with size cap).
# ---------------------------------------------------------------------------

def generate_outer_branch_group_h(class_name, members, full_note_cls):
    """members: list of (kind, name, min, max).
    'group' members with name 'full-note' refer to FullNoteGroup;
    'group' members with name 'duration' refer to a single Duration element."""
    fwds = set()
    decls = []   # tuple-of-decl-lines for ordered member emission
    privates = []
    for kind, name, mn, mx in members:
        if kind == "group" and name == "full-note":
            fwds.add(full_note_cls)
            decls.append([
                "",
                f"    /* _________ {full_note_cls} minOccurs = 1, maxOccurs = 1 _________ */",
                f"    {full_note_cls}Ptr get{full_note_cls}() const;",
                f"    void set{full_note_cls}(const {full_note_cls}Ptr &value);",
            ])
            privates.append(f"    {full_note_cls}Ptr my{full_note_cls};")
        elif kind == "group" and name == "duration":
            cc = "Duration"
            fwds.add(cc)
            decls.append([
                "",
                f"    /* _________ {cc} minOccurs = 1, maxOccurs = 1 _________ */",
                f"    {cc}Ptr get{cc}() const;",
                f"    void set{cc}(const {cc}Ptr &value);",
            ])
            privates.append(f"    {cc}Ptr my{cc};")
        elif kind == "element":
            cc = pascal(name)
            fwds.add(cc)
            if mx != 1:
                # Set member (e.g. tie maxOccurs=2)
                mx_str = "unbounded" if mx == -1 else str(mx)
                decls.append([
                    "",
                    f"    /* _________ {cc} minOccurs = {mn}, maxOccurs = {mx_str} _________ */",
                    f"    const {cc}Set &get{cc}Set() const;",
                    f"    void add{cc}(const {cc}Ptr &value);",
                    f"    void remove{cc}(const {cc}SetIterConst &value);",
                    f"    void clear{cc}Set();",
                    f"    {cc}Ptr get{cc}(const {cc}SetIterConst &setIterator) const;",
                ])
                privates.append(f"    {cc}Set my{cc}Set;")
            elif mn == 0:
                decls.append([
                    "",
                    f"    /* _________ {cc} minOccurs = 0, maxOccurs = 1 _________ */",
                    f"    {cc}Ptr get{cc}() const;",
                    f"    void set{cc}(const {cc}Ptr &value);",
                    f"    bool getHas{cc}() const;",
                    f"    void setHas{cc}(const bool value);",
                ])
                privates.append(f"    {cc}Ptr my{cc};")
                privates.append(f"    bool myHas{cc};")
            else:
                decls.append([
                    "",
                    f"    /* _________ {cc} minOccurs = 1, maxOccurs = 1 _________ */",
                    f"    {cc}Ptr get{cc}() const;",
                    f"    void set{cc}(const {cc}Ptr &value);",
                ])
                privates.append(f"    {cc}Ptr my{cc};")

    fwd_list = sorted(fwds) + [class_name]
    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    for f in fwd_list:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({f})")
    lines.append("")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append("")
    lines.append(f"class {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    for d in decls:
        for ln in d:
            lines.append(ln)
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    for p in privates:
        lines.append(p)
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_outer_branch_group_cpp(class_name, members, full_note_cls):
    """Stream contents: walk members in order. Singletons stream directly
    (no os<<endl before first item; os<<endl BEFORE subsequent items).
    Set members: for each item, os<<endl + toStream.

    NB: The exact endl placement is taken from the committed Grace/Cue/Normal
    cpps. The committed pattern is:
      - For each member in order:
        - if it's the FIRST member: toStream(member) then os<<endl (single
          element members like Grace, Cue) OR for FullNoteGroup leading
          (NormalNoteGroup) streamContents() then os<<endl
        - subsequent singletons: toStream(member) preceded by nothing/followed
          by os<<endl, OR streamContents preceded by nothing (FullNoteGroup)
        - sets (tie): for each item: os<<endl + toStream
    Look at committed for exact pattern:
      Grace:   Grace.toStream; os<<endl; FullNote.streamContents;
               for tie: os<<endl + tie.toStream; isOneLineOnly=false;
      Cue:     Cue.toStream; os<<endl; FullNote.streamContents;
               os<<endl; Duration.toStream; isOneLineOnly=false;
      Normal:  FullNote.streamContents; os<<endl; Duration.toStream;
               for tie: os<<endl + tie.toStream; (no isOneLineOnly=false)
    NormalNoteGroup omits the isOneLineOnly=false line; that is a hand-
    inconsistency between the three. Will accept residual diff for that line.
    """
    includes = set()
    for kind, name, mn, mx in members:
        if kind == "group" and name == "full-note":
            includes.add(full_note_cls)
        elif kind == "group" and name == "duration":
            includes.add("Duration")
        elif kind == "element":
            includes.add(pascal(name))
    inc_list = sorted(includes)
    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in inc_list:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{")

    # Constructor inits
    inits = []
    for kind, name, mn, mx in members:
        if kind == "group" and name == "full-note":
            inits.append(f"my{full_note_cls}(make{full_note_cls}())")
        elif kind == "group" and name == "duration":
            inits.append("myDuration(makeDuration())")
        elif kind == "element":
            cc = pascal(name)
            if mx != 1:
                inits.append(f"my{cc}Set()")
            elif mn == 0:
                inits.append(f"my{cc}(make{cc}())")
                inits.append(f"myHas{cc}(false)")
            else:
                inits.append(f"my{cc}(make{cc}())")
    lines.append(f"{class_name}::{class_name}() : {', '.join(inits)}")
    lines.append("{")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    # Walk members in order, producing the committed streamContents pattern.
    has_set = False
    for i, (kind, name, mn, mx) in enumerate(members):
        is_first = (i == 0)
        if kind == "group" and name == "full-note":
            # FullNoteGroup -> streamContents (not toStream) because it's a synthetic group
            if not is_first:
                lines.append("    os << std::endl;")
                lines.append(f"    my{full_note_cls}->streamContents(os, indentLevel, isOneLineOnly);")
            else:
                lines.append(f"    my{full_note_cls}->streamContents(os, indentLevel, isOneLineOnly);")
        elif kind == "group" and name == "duration":
            if not is_first:
                lines.append("    os << std::endl;")
            lines.append("    myDuration->toStream(os, indentLevel);")
        elif kind == "element":
            cc = pascal(name)
            if mx != 1:
                # Set
                has_set = True
                lines.append(f"    for (auto x : my{cc}Set)")
                lines.append("    {")
                lines.append("        os << std::endl;")
                lines.append("        x->toStream(os, indentLevel);")
                lines.append("    }")
            else:
                if not is_first:
                    lines.append("    os << std::endl;")
                lines.append(f"    my{cc}->toStream(os, indentLevel);")
                # The committed Grace/Cue cases emit os<<endl AFTER the leading
                # singleton (before the next member). That is handled by the
                # 'if not is_first: os<<endl' before the next member's emit.
                # However Grace.cpp has: Grace.toStream; os<<endl;
                # FullNote.streamContents. Our loop above produces:
                #   Grace.toStream (this iter, is_first=true)
                #   os<<endl; FullNote.streamContents (next iter, not first)
                # which matches. Cue.cpp has: Cue.toStream; os<<endl;
                # FullNote.streamContents; os<<endl; Duration.toStream. Our
                # loop produces:
                #   Cue.toStream
                #   os<<endl; FullNote.streamContents
                #   os<<endl; Duration.toStream
                # which matches.
                pass

    # Trailing isOneLineOnly = false. NormalNoteGroup committed omits it (hand
    # inconsistency); we always emit it for now and accept the residual diff.
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}")

    # Getters and setters
    for kind, name, mn, mx in members:
        if kind == "group" and name == "full-note":
            lines.append("")
            lines.append(f"{full_note_cls}Ptr {class_name}::get{full_note_cls}() const")
            lines.append("{")
            lines.append(f"    return my{full_note_cls};")
            lines.append("}")
            lines.append("")
            lines.append(f"void {class_name}::set{full_note_cls}(const {full_note_cls}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{full_note_cls} = value;")
            lines.append("    }")
            lines.append("}")
        elif kind == "group" and name == "duration":
            lines.append("")
            lines.append(f"DurationPtr {class_name}::getDuration() const")
            lines.append("{")
            lines.append("    return myDuration;")
            lines.append("}")
            lines.append("")
            lines.append(f"void {class_name}::setDuration(const DurationPtr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append("        myDuration = value;")
            lines.append("    }")
            lines.append("}")
        elif kind == "element":
            cc = pascal(name)
            if mx != 1:
                # Set accessors. The committed code emits getSet, addX (with
                # size cap), removeX, clearSet, getX(iter).
                cap = mx if mx > 0 else None
                lines.append("")
                lines.append(f"const {cc}Set &{class_name}::get{cc}Set() const")
                lines.append("{")
                lines.append(f"    return my{cc}Set;")
                lines.append("}")
                lines.append("")
                lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
                lines.append("{")
                lines.append("    if (value)")
                lines.append("    {")
                if cap is not None:
                    lines.append(f"        if (my{cc}Set.size() < {cap})")
                    lines.append("        {")
                    lines.append(f"            my{cc}Set.push_back(value);")
                    lines.append("        }")
                else:
                    lines.append(f"        my{cc}Set.push_back(value);")
                lines.append("    }")
                lines.append("}")
                lines.append("")
                lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
                lines.append("{")
                lines.append(f"    if (value != my{cc}Set.cend())")
                lines.append("    {")
                lines.append(f"        my{cc}Set.erase(value);")
                lines.append("    }")
                lines.append("}")
                lines.append("")
                lines.append(f"void {class_name}::clear{cc}Set()")
                lines.append("{")
                lines.append(f"    my{cc}Set.clear();")
                lines.append("}")
                lines.append("")
                lines.append(f"{cc}Ptr {class_name}::get{cc}(const {cc}SetIterConst &setIterator) const")
                lines.append("{")
                lines.append(f"    if (setIterator != my{cc}Set.cend())")
                lines.append("    {")
                lines.append("        return *setIterator;")
                lines.append("    }")
                lines.append(f"    return {cc}Ptr();")
                lines.append("}")
            elif mn == 0:
                lines.append("")
                lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
                lines.append("{")
                lines.append(f"    return my{cc};")
                lines.append("}")
                lines.append("")
                lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
                lines.append("{")
                lines.append("    if (value)")
                lines.append("    {")
                lines.append(f"        my{cc} = value;")
                lines.append("    }")
                lines.append("}")
                lines.append("")
                lines.append(f"bool {class_name}::getHas{cc}() const")
                lines.append("{")
                lines.append(f"    return myHas{cc};")
                lines.append("}")
                lines.append("")
                lines.append(f"void {class_name}::setHas{cc}(const bool value)")
                lines.append("{")
                lines.append(f"    myHas{cc} = value;")
                lines.append("}")
            else:
                lines.append("")
                lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
                lines.append("{")
                lines.append(f"    return my{cc};")
                lines.append("}")
                lines.append("")
                lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
                lines.append("{")
                lines.append("    if (value)")
                lines.append("    {")
                lines.append(f"        my{cc} = value;")
                lines.append("    }")
                lines.append("}")

    lines.append("")
    lines.append(f"MX_FROM_XELEMENT_UNUSED({class_name});")
    lines.append("")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# NoteChoice (3-branch choice over the synthetic outer-branch groups)
# Members are eagerly constructed (not JIT). Mirrors committed.
# ---------------------------------------------------------------------------

def generate_note_choice_h(class_name, branches):
    """branches: list of dicts (from _extract_note_structure) with class_name + enum_name."""
    branch_classes = [b["class_name"] for b in branches]
    fwds = sorted(set(branch_classes)) + [class_name]
    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    for f in fwds:
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({f})")
    lines.append("")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append("")
    lines.append(f"class {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append("    enum class Choice")
    lines.append("    {")
    for i, b in enumerate(branches):
        comma = "," if i < len(branches) - 1 else ""
        lines.append(f"        {b['enum_name']} = {i + 1}{comma}")
    lines.append("    };")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append("")
    lines.append("    /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append(f"    {class_name}::Choice getChoice() const;")
    lines.append(f"    void setChoice(const {class_name}::Choice value);")
    for b in branches:
        bc = b["class_name"]
        lines.append("")
        lines.append(f"    /* _________ {bc} minOccurs = 1, maxOccurs = 1 _________ */")
        lines.append(f"    {bc}Ptr get{bc}() const;")
        lines.append(f"    void set{bc}(const {bc}Ptr &value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append("    Choice myChoice;")
    for bc in branch_classes:
        lines.append(f"    {bc}Ptr my{bc};")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_note_choice_cpp(class_name, branches):
    branch_classes = [b["class_name"] for b in branches]
    incs = sorted(set(branch_classes))
    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in incs:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{")
    # Default choice is the LAST branch (the no-leading-element branch).
    # The committed code defaults to Choice::normal which is branch index 2.
    default_branch = branches[-1]
    inits = [f"myChoice(Choice::{default_branch['enum_name']})"]
    for bc in branch_classes:
        inits.append(f"my{bc}(make{bc}())")
    lines.append(f"{class_name}::{class_name}()")
    lines.append(f"    : {', '.join(inits)}")
    lines.append("{")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return false;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    switch (myChoice)")
    lines.append("    {")
    for b in branches:
        lines.append(f"    case Choice::{b['enum_name']}:")
        lines.append(f"        my{b['class_name']}->streamContents(os, indentLevel, isOneLineOnly);")
        lines.append("        break;")
    lines.append("    default:")
    lines.append("        break;")
    lines.append("    }")
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"{class_name}::Choice {class_name}::getChoice() const")
    lines.append("{")
    lines.append("    return myChoice;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::setChoice(const {class_name}::Choice value)")
    lines.append("{")
    lines.append("    myChoice = value;")
    lines.append("}")
    for bc in branch_classes:
        lines.append("")
        lines.append(f"{bc}Ptr {class_name}::get{bc}() const")
        lines.append("{")
        lines.append(f"    return my{bc};")
        lines.append("}")
        lines.append("")
        lines.append(f"void {class_name}::set{bc}(const {bc}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        my{bc} = value;")
        lines.append("    }")
        lines.append("}")
    lines.append("")
    lines.append(f"MX_FROM_XELEMENT_UNUSED({class_name});")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Note.h / Note.cpp
#
# Note is a TREE-style flat container, but with:
#   - MX_MUTEX + JIT-allocated singleton members (only top-level element with this)
#   - A custom fromXElementImpl that dispatches by element name into three
#     private parse helpers: parseNoteChoice, parseFullNoteGroup,
#     parseEditorialVoiceGroup.
# ---------------------------------------------------------------------------

def _note_member_info(kind, name, mn, mx):
    """Return (display_cls, is_set, is_optional, is_singleton_required, member_name).

    For 'group' members the class is the synthetic group class
    (EditorialVoiceGroup, Staff has just 'staff' element so it uses the
    Staff element class directly via group_ref mapping). Returns None for
    members that should be skipped.
    """
    if kind == "element":
        cc = pascal(name)
        is_set = (mx != 1)
        is_optional = (mn == 0 and mx == 1)
        return (cc, is_set, is_optional, False, name)
    elif kind == "group":
        if name == "editorial-voice":
            return ("EditorialVoiceGroup", False, False, False, name)
        elif name == "staff":
            # staff group contains a single 'staff' element. Committed Note
            # treats Staff as a singleton optional element directly.
            return ("Staff", False, mn == 0, False, "staff")
    return None


def generate_note_h(class_name, attrs_name, choice_class, trailing, model):
    """Generate Note.h. attrs_name is e.g. 'NoteAttributes'. choice_class is
    'NoteChoice'. trailing is the list of (kind,name,mn,mx) from the outer
    sequence after the outer choice."""
    fwds = set([attrs_name, choice_class, "FullNoteGroup"])
    member_decls = []
    member_privates = []

    # First member is always the NoteChoice
    member_decls.append([
        "",
        f"    /* _________ {choice_class} minOccurs = 1, maxOccurs = 1 _________ */",
        f"    {choice_class}Ptr get{choice_class}() const;",
        f"    void set{choice_class}(const {choice_class}Ptr &value);",
    ])
    member_privates.append([f"mutable {choice_class}Ptr my{choice_class};"])

    for kind, name, mn, mx in trailing:
        info = _note_member_info(kind, name, mn, mx)
        if info is None:
            continue
        cc, is_set, is_optional, _, _ = info
        fwds.add(cc)
        if is_set:
            mx_str = "unbounded" if mx == -1 else str(mx)
            member_decls.append([
                "",
                f"    /* _________ {cc} minOccurs = {mn}, maxOccurs = {mx_str} _________ */",
                f"    const {cc}Set &get{cc}Set() const;",
                f"    void add{cc}(const {cc}Ptr &value);",
                f"    void remove{cc}(const {cc}SetIterConst &value);",
                f"    void clear{cc}Set();",
                f"    {cc}Ptr get{cc}(const {cc}SetIterConst &setIterator) const;",
            ])
            member_privates.append([f"{cc}Set my{cc}Set;"])
        elif kind == "group" and name == "editorial-voice":
            # Required group (treated as min=1 max=1)
            member_decls.append([
                "",
                f"    /* _________ {cc} minOccurs = 1, maxOccurs = 1 _________ */",
                f"    {cc}Ptr get{cc}() const;",
                f"    void set{cc}(const {cc}Ptr &value);",
            ])
            member_privates.append([f"mutable {cc}Ptr my{cc};"])
        elif is_optional:
            member_decls.append([
                "",
                f"    /* _________ {cc} minOccurs = 0, maxOccurs = 1 _________ */",
                f"    {cc}Ptr get{cc}() const;",
                f"    void set{cc}(const {cc}Ptr &value);",
                f"    bool getHas{cc}() const;",
                f"    void setHas{cc}(const bool value);",
            ])
            member_privates.append([
                f"mutable {cc}Ptr my{cc};",
                f"bool myHas{cc};",
            ])

    fwd_list = sorted(fwds) + [class_name]
    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append(f'#include "mx/core/elements/{attrs_name}.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace ezxml")
    lines.append("{")
    lines.append("class XElementIterator;")
    lines.append("}")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    # Attributes forward declare is a struct, not element
    lines.append(f"MX_FORWARD_DECLARE_ATTRIBUTES({attrs_name})")
    for f in fwd_list:
        if f == attrs_name:
            continue
        lines.append(f"MX_FORWARD_DECLARE_ELEMENT({f})")
    lines.append("")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append("")
    lines.append(f"class {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append(f"    {attrs_name}Ptr getAttributes() const;")
    lines.append(f"    void setAttributes(const {attrs_name}Ptr &value);")
    for d in member_decls:
        for ln in d:
            lines.append(ln)
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append("    MX_MUTEX")
    lines.append(f"    mutable {attrs_name}Ptr myAttributes;")
    for grp in member_privates:
        for ln in grp:
            lines.append(f"    {ln}")
    lines.append("")
    # The three private parse helpers
    lines.append(f"    bool parseNoteChoice(std::ostream &message, ::ezxml::XElement &noteElement, ::ezxml::XElementIterator &iter);")
    lines.append("")
    lines.append(f"    bool parseFullNoteGroup(std::ostream &message, ::ezxml::XElement &noteElement, ::ezxml::XElementIterator &iter,")
    lines.append("                            FullNoteGroupPtr &outFullNoteGroup);")
    lines.append("")
    lines.append(f"    bool parseEditorialVoiceGroup(std::ostream &message, ::ezxml::XElement &noteElement,")
    lines.append("                                  ::ezxml::XElementIterator &iter);")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


def generate_note_cpp(class_name, attrs_name, choice_class, trailing,
                     full_note_pre, full_note_choice, editorial_voice_members,
                     branches, duration_elem, model):
    """Big one. Generates:
      - Includes
      - Constructor with init list
      - hasAttributes/streamAttributes/streamName/hasContents
      - streamContents (walk trailing members in order)
      - All getters/setters/etc.
      - fromXElementImpl: dispatch by element name (schema-driven)
      - parseNoteChoice helper
      - parseFullNoteGroup helper
      - parseEditorialVoiceGroup helper
    """

    # ---- Compute member info table for trailing ----
    members = []  # ordered list of dicts
    for kind, name, mn, mx in trailing:
        info = _note_member_info(kind, name, mn, mx)
        if info is None:
            continue
        cc, is_set, is_optional, _, xml_name = info
        is_required_group = (kind == "group" and name == "editorial-voice")
        members.append({
            "cls": cc,
            "xml_name": xml_name,
            "is_set": is_set,
            "is_optional": is_optional,
            "is_group": is_required_group,
            "is_unbounded": (mx == -1),
            "max_occurs": mx,
            "kind": kind,
            "group_name": name if kind == "group" else None,
        })

    # ---- Includes ----
    incs = set([attrs_name, choice_class, "FullNoteGroup", "FullNoteTypeChoice"])
    incs |= set([b["class_name"] for b in branches])
    # Include all member class headers
    for m in members:
        incs.add(m["cls"])
    # Note's fromXElementImpl references the inner-choice element classes
    # via the FullNoteGroup parse. Include them so the .cpp can call e.g.
    # outFullNoteGroup->getFullNoteTypeChoice()->getPitch()->fromXElement.
    for fc in full_note_choice:
        incs.add(pascal(fc))
    # FullNoteGroup's pre-elements (chord) also referenced for setHasChord.
    for pe in full_note_pre:
        incs.add(pascal(pe))
    # Tie element class for the parseNoteChoice helper.
    incs.add("Tie")
    # Cue / Grace element classes referenced by parseNoteChoice.
    for b in branches:
        if b["leading_element"]:
            incs.add(pascal(b["leading_element"]))
    # Duration class referenced in parseNoteChoice helper.
    incs.add(pascal(duration_elem))
    # Editorial-voice group's element classes referenced in parseEditorialVoiceGroup.
    for evn in editorial_voice_members:
        incs.add(pascal(evn))

    inc_list = sorted(incs)

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    for inc in inc_list:
        lines.append(f'#include "mx/core/elements/{inc}.h"')
    lines.append("#include <iostream>")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{")

    # ---- Constructor ----
    # The committed Note ctor inits myAttributes(nullptr), myNoteChoice(nullptr),
    # then for each trailing member: ptr(nullptr) for singletons (with myHasX(false)
    # if optional), or default ctor for sets, or required-group ptr(nullptr).
    inits = ["myAttributes(nullptr)", f"my{choice_class}(nullptr)"]
    for m in members:
        cc = m["cls"]
        if m["is_set"]:
            inits.append(f"my{cc}Set()")
        elif m["is_group"]:
            inits.append(f"my{cc}(nullptr)")
        elif m["is_optional"]:
            inits.append(f"my{cc}(nullptr)")
            inits.append(f"myHas{cc}(false)")

    lines.append(f"{class_name}::{class_name}()")
    # Wrap inits across multiple lines like committed (we'll let make fmt
    # normalize the line breaking).
    lines.append(f"    : {', '.join(inits)}")
    lines.append("{")
    lines.append("}")
    lines.append("")

    # hasAttributes / streamAttributes
    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return getAttributes()->hasValues();")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return getAttributes()->toStream(os);")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append('    os << "note";')
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}")
    lines.append("")

    # streamContents -- the committed pattern:
    # os << std::endl; (always at start)
    # getNoteChoice()->streamContents(...);
    # for each trailing member in order:
    #   - singleton optional: if (myHasX) { os<<endl; getX()->toStream(os, indentLevel+1); }
    #   - required group:    if (getX()->hasContents()) { os<<endl; getX()->streamContents(os, indentLevel+1, isOneLineOnly); }
    #   - set:               for (auto x : myXSet) { os<<endl; x->toStream(os, indentLevel+1); }
    # isOneLineOnly = false; os << std::endl; return os;
    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    os << std::endl;")
    lines.append(f"    get{choice_class}()->streamContents(os, indentLevel + 1, isOneLineOnly);")
    for m in members:
        cc = m["cls"]
        if m["is_set"]:
            lines.append(f"    for (auto x : my{cc}Set)")
            lines.append("    {")
            lines.append("        os << std::endl;")
            lines.append("        x->toStream(os, indentLevel + 1);")
            lines.append("    }")
        elif m["is_group"]:
            lines.append(f"    if (get{cc}()->hasContents())")
            lines.append("    {")
            lines.append("        os << std::endl;")
            lines.append(f"        get{cc}()->streamContents(os, indentLevel + 1, isOneLineOnly);")
            lines.append("    }")
        elif m["is_optional"]:
            lines.append(f"    if (myHas{cc})")
            lines.append("    {")
            lines.append("        os << std::endl;")
            lines.append(f"        get{cc}()->toStream(os, indentLevel + 1);")
            lines.append("    }")
    lines.append("    isOneLineOnly = false;")
    lines.append("    os << std::endl;")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")

    # ---- Attribute getters/setters ----
    lines.append(f"{attrs_name}Ptr {class_name}::getAttributes() const")
    lines.append("{")
    lines.append("    MX_LOCK;")
    lines.append(f"    MX_JIT_ALLOCATE_ATTRIBUTES({attrs_name});")
    lines.append("    return myAttributes;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::setAttributes(const {attrs_name}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append("        myAttributes = value;")
    lines.append("    }")
    lines.append("}")
    lines.append("")

    # ---- NoteChoice getter/setter ----
    lines.append(f"{choice_class}Ptr {class_name}::get{choice_class}() const")
    lines.append("{")
    lines.append("    MX_LOCK;")
    lines.append(f"    MX_JIT_ALLOCATE({choice_class});")
    lines.append(f"    return my{choice_class};")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::set{choice_class}(const {choice_class}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append(f"        my{choice_class} = value;")
    lines.append("    }")
    lines.append("}")
    lines.append("")

    # ---- Trailing member getters/setters ----
    for m in members:
        cc = m["cls"]
        if m["is_set"]:
            mx = m["max_occurs"]
            lines.append(f"const {cc}Set &{class_name}::get{cc}Set() const")
            lines.append("{")
            lines.append(f"    return my{cc}Set;")
            lines.append("}")
            lines.append("")
            lines.append(f"void {class_name}::remove{cc}(const {cc}SetIterConst &value)")
            lines.append("{")
            lines.append(f"    if (value != my{cc}Set.cend())")
            lines.append("    {")
            lines.append(f"        my{cc}Set.erase(value);")
            lines.append("    }")
            lines.append("}")
            lines.append("")
            lines.append(f"void {class_name}::add{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            if mx > 0:
                lines.append(f"        if (my{cc}Set.size() < {mx})")
                lines.append("        {")
                lines.append(f"            my{cc}Set.push_back(value);")
                lines.append("        }")
            else:
                lines.append(f"        my{cc}Set.push_back(value);")
            lines.append("    }")
            lines.append("}")
            lines.append("")
            lines.append(f"void {class_name}::clear{cc}Set()")
            lines.append("{")
            lines.append(f"    my{cc}Set.clear();")
            lines.append("}")
            lines.append("")
            lines.append(f"{cc}Ptr {class_name}::get{cc}(const {cc}SetIterConst &setIterator) const")
            lines.append("{")
            lines.append(f"    if (setIterator != my{cc}Set.cend())")
            lines.append("    {")
            lines.append("        return *setIterator;")
            lines.append("    }")
            lines.append(f"    return {cc}Ptr();")
            lines.append("}")
            lines.append("")
        elif m["is_group"]:
            # JIT-allocated group
            lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
            lines.append("{")
            lines.append("    MX_LOCK;")
            lines.append(f"    MX_JIT_ALLOCATE({cc});")
            lines.append(f"    return my{cc};")
            lines.append("}")
            lines.append("")
            lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}")
            lines.append("")
        elif m["is_optional"]:
            lines.append(f"{cc}Ptr {class_name}::get{cc}() const")
            lines.append("{")
            lines.append("    MX_LOCK;")
            lines.append(f"    MX_JIT_ALLOCATE({cc});")
            lines.append(f"    return my{cc};")
            lines.append("}")
            lines.append("")
            lines.append(f"void {class_name}::set{cc}(const {cc}Ptr &value)")
            lines.append("{")
            lines.append("    if (value)")
            lines.append("    {")
            lines.append(f"        my{cc} = value;")
            lines.append("    }")
            lines.append("}")
            lines.append("")
            lines.append(f"bool {class_name}::getHas{cc}() const")
            lines.append("{")
            lines.append(f"    return myHas{cc};")
            lines.append("}")
            lines.append("")
            lines.append(f"void {class_name}::setHas{cc}(const bool value)")
            lines.append("{")
            lines.append(f"    myHas{cc} = value;")
            lines.append("}")
            lines.append("")

    # ============================================================
    # fromXElementImpl
    # ============================================================
    # Build the dispatch list -- schema-driven from:
    #   - the outer choice branches' "starter" element names (the elements
    #     that signal "this is a NoteChoice" iteration): the leading element
    #     of each branch (grace/cue) PLUS the elements that introduce the
    #     full-note group (chord + full-note-choice members) -- collectively
    #     the set of "note-choice elements".
    #   - editorial-voice group member names
    #   - trailing element names (instrument, type, dot, ...)
    #
    # The dispatch ordering matches the committed code's hand-written order.
    # In committed: pitch | unpitched | rest | chord | grace | cue (note-choice)
    # Then trailing in declared XSD order:
    # instrument, editorial-voice, type, dot, accidental, time-modification,
    # stem, notehead, notehead-text, staff, beam, notations, lyric, play.

    note_choice_elements = []  # element XML names that trigger parseNoteChoice
    note_choice_elements.extend(full_note_choice)   # pitch, unpitched, rest
    note_choice_elements.extend(full_note_pre)       # chord
    for b in branches:
        if b["leading_element"]:
            note_choice_elements.append(b["leading_element"])

    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    lines.append("    bool isNoteChoiceFound = false;")
    # For each set member with maxOccurs != 1, we need a "first added" flag
    # that handles the "default-constructed singleton already in the set"
    # idiom that the committed code uses. The set is initialized empty
    # (myDotSet()) so the size==1 branch actually never fires here. But
    # the committed code does have that pattern -- emit it for parity.
    first_added_flags = []
    for m in members:
        if m["is_set"]:
            flag = f"isFirst{m['cls']}Added"
            first_added_flags.append((flag, m["cls"]))
            lines.append(f"    bool {flag} = false;")
    lines.append("")
    lines.append("    isSuccess &= getAttributes()->fromXElement(message, xelement);")
    lines.append("")
    lines.append("    for (auto it = xelement.begin(); it != xelement.end(); ++it)")
    lines.append("    {")
    lines.append("        const std::string elementName = it->getName();")
    lines.append("")

    # First branch: note-choice elements (handled by parseNoteChoice with --it).
    nc_cond = " || ".join([f'elementName == "{n}"' for n in note_choice_elements])
    lines.append(f"        if ({nc_cond})")
    lines.append("        {")
    lines.append("            isNoteChoiceFound = true;")
    lines.append("            isSuccess &= parseNoteChoice(message, xelement, it);")
    lines.append("            --it;")
    lines.append("        }")

    # Then walk trailing members in order, emitting if/else if branches.
    for m in members:
        cls = m["cls"]
        if m["is_set"]:
            xml_n = m["xml_name"]
            lines.append(f'        else if (elementName == "{xml_n}")')
            lines.append("        {")
            lines.append(f"            auto {camel(xml_n)} = make{cls}();")
            lines.append(f"            isSuccess &= {camel(xml_n)}->fromXElement(message, *it);")
            lines.append("")
            lines.append(f"            if (!isFirst{cls}Added && my{cls}Set.size() == 1)")
            lines.append("            {")
            lines.append(f"                *(my{cls}Set.begin()) = {camel(xml_n)};")
            lines.append(f"                isFirst{cls}Added = true;")
            lines.append("            }")
            lines.append("            else")
            lines.append("            {")
            lines.append(f"                my{cls}Set.push_back({camel(xml_n)});")
            lines.append(f"                isFirst{cls}Added = true;")
            lines.append("            }")
            lines.append("        }")
        elif m["is_group"]:
            # editorial-voice: dispatch to parseEditorialVoiceGroup
            ev_cond = " || ".join([f'elementName == "{n}"' for n in editorial_voice_members])
            lines.append(f"        else if ({ev_cond})")
            lines.append("        {")
            lines.append("            isSuccess &= parseEditorialVoiceGroup(message, xelement, it);")
            lines.append("        }")
        elif m["is_optional"]:
            xml_n = m["xml_name"]
            lines.append(f'        else if (elementName == "{xml_n}")')
            lines.append("        {")
            lines.append(f"            myHas{cls} = true;")
            lines.append(f"            isSuccess &= get{cls}()->fromXElement(message, *it);")
            lines.append("        }")

    lines.append("        else")
    lines.append("        {")
    lines.append("            isSuccess = false;")
    lines.append(f'            message << "Note: unexpected element \'" << elementName << "\'" << std::endl;')
    lines.append("        }")
    lines.append("    }")
    lines.append("")
    lines.append("    if (!isNoteChoiceFound)")
    lines.append("    {")
    lines.append("        isSuccess = false;")
    lines.append(f'        message << "Note: \'note-choice\' elements were required but not found" << std::endl;')
    lines.append("    }")
    lines.append("")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}")
    lines.append("")

    # ============================================================
    # parseNoteChoice
    # ============================================================
    # Dispatch on the first child element name:
    #   - "grace" -> setChoice(grace); parse Grace; fullNoteGroup = getGrace().getFullNoteGroup; ++iter
    #   - "cue"   -> setChoice(cue); parse Cue; fullNoteGroup = getCue().getFullNoteGroup; ++iter
    #   - else    -> setChoice(normal); fullNoteGroup = getNormal().getFullNoteGroup
    # Then parseFullNoteGroup(message, noteElement, iter, fullNoteGroup).
    # Then if NOT grace, expect 'duration'; assign via the right branch.
    # Then if NOT cue, parse tie* (assigning via the right branch).
    lines.append(f"bool {class_name}::parseNoteChoice(std::ostream &message, ::ezxml::XElement &noteElement, ::ezxml::XElementIterator &iter)")
    lines.append("{")
    lines.append("    if (iter == noteElement.end())")
    lines.append("    {")
    lines.append("        return false;")
    lines.append("    }")
    lines.append("")
    lines.append("    bool isSuccess = true;")
    lines.append("")
    lines.append("    const std::string elementName = iter->getName();")
    lines.append("    FullNoteGroupPtr fullNoteGroup = nullptr;")
    lines.append("")

    # Find normal branch (no leading element) and the named branches.
    named_branches = [b for b in branches if b["leading_element"]]
    normal_branch = next((b for b in branches if not b["leading_element"]), None)
    if normal_branch is None:
        raise RuntimeError("Note: no third (no-leading-element) branch found")

    for i, b in enumerate(named_branches):
        le = b["leading_element"]
        le_cls = pascal(le)
        bg = b["class_name"]
        prefix = "if" if i == 0 else "else if"
        lines.append(f'    {prefix} (elementName == "{le}")')
        lines.append("    {")
        lines.append(f"        get{choice_class}()->setChoice({choice_class}::Choice::{b['enum_name']});")
        lines.append(f"        isSuccess &= get{choice_class}()->get{bg}()->get{le_cls}()->fromXElement(message, *iter);")
        lines.append(f"        fullNoteGroup = get{choice_class}()->get{bg}()->getFullNoteGroup();")
        lines.append("        ++iter;")
        lines.append("    }")

    nbg = normal_branch["class_name"]
    lines.append("    else")
    lines.append("    {")
    lines.append(f"        get{choice_class}()->setChoice({choice_class}::Choice::{normal_branch['enum_name']});")
    lines.append(f"        fullNoteGroup = get{choice_class}()->get{nbg}()->getFullNoteGroup();")
    lines.append("    }")
    lines.append("")
    lines.append("    // we should now be pointing at the full note group")
    lines.append("    isSuccess &= parseFullNoteGroup(message, noteElement, iter, fullNoteGroup);")
    lines.append("")

    # Branches that require duration after the full-note group: any branch
    # whose members include a duration group ref (i.e. branches "cue" and
    # "normal" in MusicXML 3.0).
    duration_branches = [b for b in branches if b["has_duration"]]
    # Filter named branches that have duration -> "cue"
    named_dur_branches = [b for b in duration_branches if b["leading_element"]]
    # The committed code does:
    #   if (elementName != "grace") -> the only branch that has NO duration.
    # We invert by listing the leading-element name that is duration-FREE.
    no_dur_branches = [b for b in branches if not b["has_duration"]]
    # Expecting exactly one of these (grace).
    if len(no_dur_branches) == 1 and no_dur_branches[0]["leading_element"]:
        no_dur_name = no_dur_branches[0]["leading_element"]
        lines.append(f"    // {pascal(no_dur_branches[0]['class_name'])}s do not have a duration element")
        lines.append(f'    if (elementName != "{no_dur_name}")')
    else:
        # fallback: enumerate
        cond = " || ".join([f'elementName == "{b["leading_element"]}"' for b in duration_branches if b["leading_element"]])
        if not duration_branches[-1]["leading_element"]:
            # Normal branch (no leading element) is reached when elementName
            # is none of the named leading elements.
            named_le = " && ".join([f'elementName != "{b["leading_element"]}"' for b in named_branches])
            cond = f"({cond}) || ({named_le})"
        lines.append(f"    if ({cond})")
    lines.append("    {")
    lines.append(f'        if (iter == noteElement.end() || iter->getName() != "{duration_elem}")')
    lines.append("        {")
    lines.append(f'            message << "Note: parseNoteChoice - a \'{duration_elem}\' element was required but not found" << std::endl;')
    lines.append("            return false;")
    lines.append("        }")
    lines.append("")
    # For each branch that has duration, emit:
    #   if (getNoteChoice()->getChoice() == NoteChoice::Choice::normal) { ... }
    #   else if (...cue) { ... }
    for j, b in enumerate(duration_branches):
        prefix = "if" if j == 0 else "else if"
        bg = b["class_name"]
        lines.append(f"        {prefix} (get{choice_class}()->getChoice() == {choice_class}::Choice::{b['enum_name']})")
        lines.append("        {")
        lines.append(f"            get{choice_class}()->get{bg}()->get{pascal(duration_elem)}()->fromXElement(message, *iter);")
        lines.append("        }")
    lines.append("        ++iter;")
    lines.append("    }")
    lines.append("")
    lines.append("    // additional stuff is optional so we may be at the end iter")
    lines.append("    if (iter == noteElement.end())")
    lines.append("    {")
    lines.append("        MX_RETURN_IS_SUCCESS;")
    lines.append("    }")
    lines.append("")

    # Tie parsing: only branches whose tie_max > 0
    tie_branches = [b for b in branches if b["tie_max"] > 0]
    # Branches WITHOUT tie support -> early-return.
    no_tie_branches = [b for b in branches if b["tie_max"] == 0]

    tie_cond = " || ".join(
        [f"get{choice_class}()->getChoice() == {choice_class}::Choice::{b['enum_name']}" for b in tie_branches])
    lines.append(f"    // now we may be pointing at tie elements, but only if the choice supports them")
    lines.append(f"    if ({tie_cond})")
    lines.append("    {")
    lines.append("        std::string possibleTieElementName = iter->getName();")
    lines.append('        while (iter != noteElement.end() && iter->getName() == "tie")')
    lines.append("        {")
    lines.append("            auto tie = makeTie();")
    lines.append("            isSuccess &= tie->fromXElement(message, *iter);")
    for k, b in enumerate(tie_branches):
        prefix = "if" if k == 0 else "else if"
        bg = b["class_name"]
        lines.append(f"            {prefix} (get{choice_class}()->getChoice() == {choice_class}::Choice::{b['enum_name']})")
        lines.append("            {")
        lines.append(f"                get{choice_class}()->get{bg}()->addTie(tie);")
        lines.append("            }")
    lines.append("            ++iter;")
    lines.append("        }")
    lines.append("    }")
    for b in no_tie_branches:
        lines.append(f"    else if (get{choice_class}()->getChoice() == {choice_class}::Choice::{b['enum_name']})")
        lines.append("    {")
        lines.append("        MX_RETURN_IS_SUCCESS;")
        lines.append("    }")
    lines.append("")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}")
    lines.append("")

    # ============================================================
    # parseFullNoteGroup
    # ============================================================
    lines.append(f"bool {class_name}::parseFullNoteGroup(std::ostream &message, ::ezxml::XElement &noteElement, ::ezxml::XElementIterator &iter,")
    lines.append("                              FullNoteGroupPtr &outFullNoteGroup)")
    lines.append("{")
    lines.append("    if (iter == noteElement.end())")
    lines.append("    {")
    lines.append("        return false;")
    lines.append("    }")
    lines.append("")
    lines.append("    bool isSuccess = true;")
    lines.append("")
    # chord pre-element: if first iter name == "chord", set has chord and advance.
    for pe in full_note_pre:
        pe_cls = pascal(pe)
        lines.append(f'    if (iter->getName() == "{pe}")')
        lines.append("    {")
        lines.append(f"        outFullNoteGroup->setHas{pe_cls}(true);")
        lines.append("        ++iter;")
        lines.append("    }")
        lines.append("")
    lines.append("    // we should now be pointing at the FullNoteTypeChoice")
    lines.append("    if (iter == noteElement.end())")
    lines.append("    {")
    lines.append('        message << "Note: parseFullNoteGroup did not find the FullNoteChoice" << std::endl;')
    lines.append("        return false;")
    lines.append("    }")
    lines.append("")
    lines.append("    const std::string noteChoiceElementName = iter->getName();")
    lines.append("")
    # full_note_choice members: pitch, unpitched, rest
    for k, fc in enumerate(full_note_choice):
        prefix = "if" if k == 0 else "else if"
        fc_cls = pascal(fc)
        lines.append(f'    {prefix} (noteChoiceElementName == "{fc}")')
        lines.append("    {")
        lines.append(f"        outFullNoteGroup->getFullNoteTypeChoice()->setChoice(FullNoteTypeChoice::Choice::{camel(fc)});")
        lines.append(f"        isSuccess &= outFullNoteGroup->getFullNoteTypeChoice()->get{fc_cls}()->fromXElement(message, *iter);")
        lines.append("        ++iter;")
        lines.append("    }")
    lines.append("    else")
    lines.append("    {")
    lines.append('        message << "Note: parseFullNoteGroup encountered unexpected element \'" << noteChoiceElementName << "\'"')
    lines.append("                << std::endl;")
    lines.append("        isSuccess = false;")
    lines.append("        ++iter;")
    lines.append("    }")
    lines.append("")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}")
    lines.append("")

    # ============================================================
    # parseEditorialVoiceGroup
    # ============================================================
    lines.append(f"bool {class_name}::parseEditorialVoiceGroup(std::ostream &message, ::ezxml::XElement &noteElement,")
    lines.append("                                    ::ezxml::XElementIterator &iter)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    lines.append("    bool isIterIncremented = false;")
    ev_cond = " || ".join([f'iter->getName() == "{n}"' for n in editorial_voice_members])
    lines.append(f"    while (iter != noteElement.end() &&")
    lines.append(f"           ({ev_cond}))")
    lines.append("    {")
    for j, evn in enumerate(editorial_voice_members):
        prefix = "if" if j == 0 else "else if"
        ev_cls = pascal(evn)
        lines.append(f'        {prefix} (iter->getName() == "{evn}")')
        lines.append("        {")
        lines.append(f"            getEditorialVoiceGroup()->setHas{ev_cls}(true);")
        lines.append(f"            isSuccess &= getEditorialVoiceGroup()->get{ev_cls}()->fromXElement(message, *iter);")
        lines.append("        }")
    lines.append("")
    lines.append("        ++iter;")
    lines.append("        isIterIncremented = true;")
    lines.append("    }")
    lines.append("")
    lines.append("    if (isIterIncremented)")
    lines.append("    {")
    lines.append("        --iter;")
    lines.append("    }")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Orchestrator
# ---------------------------------------------------------------------------

def _emit_note_family(elem_name, elem, ct, model, generated_attrs, stats):
    """Emit Note + NoteAttributes + NoteChoice + 5 helper classes.

    File set (8 .h + 8 .cpp):
      NoteAttributes, FullNoteTypeChoice, FullNoteGroup,
      GraceNoteGroup, CueNoteGroup, NormalNoteGroup, NoteChoice, Note.
    """
    s = _extract_note_structure(ct, model)

    class_name = element_class_name(elem_name)            # "Note"
    choice_class = pascal(elem_name) + "Choice"           # "NoteChoice"
    full_note_cls = pascal("full-note") + "Group"         # "FullNoteGroup"
    full_note_type_choice_cls = pascal("full-note") + "TypeChoice"  # "FullNoteTypeChoice"

    type_name = elem.type_name or ""
    attrs_name = resolve_attrs_name(elem_name, type_name, model)  # "NoteAttributes"

    # 1. NoteAttributes -- standard attrs path
    if ct.attributes and attrs_name not in generated_attrs and attrs_name not in CORE_ROOT_ATTRS:
        h = generate_attrs_h(attrs_name, ct.attributes, model)
        c = generate_attrs_cpp(attrs_name, ct.attributes, model)
        write_file(os.path.join(ELEM_DIR, f"{attrs_name}.h"), h)
        write_file(os.path.join(ELEM_DIR, f"{attrs_name}.cpp"), c)
        generated_attrs.add(attrs_name)
        stats["attrs_written"] += 1

    # 2. FullNoteTypeChoice (inner pitch/unpitched/rest choice)
    h = generate_full_note_type_choice_h(full_note_type_choice_cls, s["full_note"]["choice_branches"])
    c = generate_full_note_type_choice_cpp(full_note_type_choice_cls, s["full_note"]["choice_branches"])
    write_file(os.path.join(ELEM_DIR, f"{full_note_type_choice_cls}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{full_note_type_choice_cls}.cpp"), c)

    # 3. FullNoteGroup
    h = generate_full_note_group_h(full_note_cls, s["full_note"]["pre"], full_note_type_choice_cls)
    c = generate_full_note_group_cpp(full_note_cls, s["full_note"]["pre"], full_note_type_choice_cls)
    write_file(os.path.join(ELEM_DIR, f"{full_note_cls}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{full_note_cls}.cpp"), c)

    # 4. The three outer branch groups (Grace/Cue/Normal)
    for b in s["branches"]:
        h = generate_outer_branch_group_h(b["class_name"], b["members"], full_note_cls)
        c = generate_outer_branch_group_cpp(b["class_name"], b["members"], full_note_cls)
        write_file(os.path.join(ELEM_DIR, f"{b['class_name']}.h"), h)
        write_file(os.path.join(ELEM_DIR, f"{b['class_name']}.cpp"), c)

    # 5. NoteChoice
    h = generate_note_choice_h(choice_class, s["branches"])
    c = generate_note_choice_cpp(choice_class, s["branches"])
    write_file(os.path.join(ELEM_DIR, f"{choice_class}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{choice_class}.cpp"), c)

    # 6. Note
    h = generate_note_h(class_name, attrs_name, choice_class, s["trailing"], model)
    c = generate_note_cpp(class_name, attrs_name, choice_class, s["trailing"],
                         s["full_note"]["pre"], s["full_note"]["choice_branches"],
                         s["editorial_voice"], s["branches"], s["duration"], model)
    write_file(os.path.join(ELEM_DIR, f"{class_name}.h"), h)
    write_file(os.path.join(ELEM_DIR, f"{class_name}.cpp"), c)

    stats["elem_written"] += 1
    stats["bespoke_written"] = stats.get("bespoke_written", 0) + 1


# ---------------------------------------------------------------------------
# Direction (bespoke, schema-driven)
#
# Direction's child <direction-type> contains a choice whose branches are
# read from the parsed XSD (each branch is an ElementRefNode with
# max_occurs == -1 indicating a "multi" branch). Two anonymous-namespace
# arrays in Direction.cpp -- directions[] (every branch name) and
# multiDirections[] (only the unbounded ones) -- are derived directly from
# those branches so MusicXML 4.0 changes propagate automatically.
#
# The bulk of the Direction.cpp body is a hand-shaped parser that promotes
# the first parsed direction-type into the default-seeded slot and then
# appends. For each "multi" branch (rehearsal, segno, words, coda,
# dynamics, percussion) the parser also swaps the DirectionType's first
# default-seeded sub-element using addX + removeX. The shape of that
# parser cannot be expressed by the shared rule-based path, so it lives
# here.


def _emit_direction_family(elem_name, elem, ct, model, generated_attrs, stats):
    assert elem_name == "direction"

    # Read the direction-type choice branches from the parsed model. These
    # drive the two anonymous-namespace arrays in Direction.cpp and the
    # per-branch dispatch in createDirectionType().
    dt_ct = model.complex_types["direction-type"]
    dt_branches = list(dt_ct.content_tree.branches)
    branch_names = [b.element_name for b in dt_branches]
    multi_branch_names = [b.element_name for b in dt_branches if b.max_occurs == -1]

    class_name = element_class_name(elem_name)  # "Direction"
    type_name = elem.type_name or ""

    # 1. Attrs struct via the standard generator.
    attrs_name = None
    if ct.attributes:
        attrs_name = resolve_attrs_name(elem_name, type_name, model)
        if attrs_name not in generated_attrs and attrs_name not in CORE_ROOT_ATTRS:
            h = generate_attrs_h(attrs_name, ct.attributes, model)
            c = generate_attrs_cpp(attrs_name, ct.attributes, model)
            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.h"), h)
            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.cpp"), c)
            generated_attrs.add(attrs_name)
            stats["attrs_written"] += 1

    # 2. Direction.h
    h_content = _generate_direction_h(class_name, attrs_name)
    write_file(os.path.join(ELEM_DIR, f"{class_name}.h"), h_content)

    # 3. Direction.cpp
    c_content = _generate_direction_cpp(class_name, attrs_name, branch_names, multi_branch_names)
    write_file(os.path.join(ELEM_DIR, f"{class_name}.cpp"), c_content)

    stats["elem_written"] += 1
    stats["bespoke_written"] = stats.get("bespoke_written", 0) + 1


def _generate_direction_h(class_name, attrs_name):
    lines = [LICENSE, "#pragma once\n"]
    lines.append('#include "mx/core/ElementInterface.h"')
    lines.append('#include "mx/core/ForwardDeclare.h"')
    lines.append(f'#include "mx/core/elements/{attrs_name}.h"')
    lines.append("")
    lines.append("#include <iosfwd>")
    lines.append("#include <memory>")
    lines.append("#include <vector>")
    lines.append("")
    lines.append("namespace ezxml\n{\nclass XElementIterator;\n}")
    lines.append("")
    lines.append("namespace mx\n{\nnamespace core\n{\n")
    lines.append(f"MX_FORWARD_DECLARE_ATTRIBUTES({attrs_name})")
    lines.append("MX_FORWARD_DECLARE_ELEMENT(DirectionType)")
    lines.append("MX_FORWARD_DECLARE_ELEMENT(EditorialVoiceDirectionGroup)")
    lines.append("MX_FORWARD_DECLARE_ELEMENT(Offset)")
    lines.append("MX_FORWARD_DECLARE_ELEMENT(Sound)")
    lines.append("MX_FORWARD_DECLARE_ELEMENT(Staff)")
    lines.append(f"MX_FORWARD_DECLARE_ELEMENT({class_name})")
    lines.append("")
    lines.append(f"inline {class_name}Ptr make{class_name}()")
    lines.append("{")
    lines.append(f"    return std::make_shared<{class_name}>();")
    lines.append("}")
    lines.append("")
    lines.append(f"class {class_name} : public ElementInterface")
    lines.append("{")
    lines.append("  public:")
    lines.append(f"    {class_name}();")
    lines.append("")
    lines.append("    virtual bool hasAttributes() const;")
    lines.append("    virtual std::ostream &streamAttributes(std::ostream &os) const;")
    lines.append("    virtual std::ostream &streamName(std::ostream &os) const;")
    lines.append("    virtual bool hasContents() const;")
    lines.append("    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;")
    lines.append(f"    {attrs_name}Ptr getAttributes() const;")
    lines.append(f"    void setAttributes(const {attrs_name}Ptr &value);")
    lines.append("")
    lines.append("    /* _________ DirectionType minOccurs = 1, maxOccurs = unbounded _________ */")
    lines.append("    const DirectionTypeSet &getDirectionTypeSet() const;")
    lines.append("    void addDirectionType(const DirectionTypePtr &value);")
    lines.append("    void removeDirectionType(const DirectionTypeSetIterConst &value);")
    lines.append("    void clearDirectionTypeSet();")
    lines.append("    DirectionTypePtr getDirectionType(const DirectionTypeSetIterConst &setIterator) const;")
    lines.append("")
    lines.append("    /* _________ Offset minOccurs = 0, maxOccurs = 1 _________ */")
    lines.append("    OffsetPtr getOffset() const;")
    lines.append("    void setOffset(const OffsetPtr &value);")
    lines.append("    bool getHasOffset() const;")
    lines.append("    void setHasOffset(const bool value);")
    lines.append("")
    lines.append("    /* _________ EditorialVoiceDirectionGroup minOccurs = 1, maxOccurs = 1 _________ */")
    lines.append("    EditorialVoiceDirectionGroupPtr getEditorialVoiceDirectionGroup() const;")
    lines.append("    void setEditorialVoiceDirectionGroup(const EditorialVoiceDirectionGroupPtr &value);")
    lines.append("")
    lines.append("    /* _________ Staff minOccurs = 0, maxOccurs = 1 _________ */")
    lines.append("    StaffPtr getStaff() const;")
    lines.append("    void setStaff(const StaffPtr &value);")
    lines.append("    bool getHasStaff() const;")
    lines.append("    void setHasStaff(const bool value);")
    lines.append("")
    lines.append("    /* _________ Sound minOccurs = 0, maxOccurs = 1 _________ */")
    lines.append("    SoundPtr getSound() const;")
    lines.append("    void setSound(const SoundPtr &value);")
    lines.append("    bool getHasSound() const;")
    lines.append("    void setHasSound(const bool value);")
    lines.append("")
    lines.append("  private:")
    lines.append("    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);")
    lines.append("")
    lines.append("  private:")
    lines.append(f"    {attrs_name}Ptr myAttributes;")
    lines.append("    DirectionTypeSet myDirectionTypeSet;")
    lines.append("    OffsetPtr myOffset;")
    lines.append("    bool myHasOffset;")
    lines.append("    EditorialVoiceDirectionGroupPtr myEditorialVoiceDirectionGroup;")
    lines.append("    StaffPtr myStaff;")
    lines.append("    bool myHasStaff;")
    lines.append("    SoundPtr mySound;")
    lines.append("    bool myHasSound;")
    lines.append("")
    lines.append("    bool importDirectionTypeSet(std::ostream &message, ::ezxml::XElementIterator &iter,")
    lines.append("                                ::ezxml::XElementIterator &endIter, bool &isSuccess, bool &isFound);")
    lines.append("    DirectionTypePtr createDirectionType(std::ostream &message, ::ezxml::XElementIterator &iter,")
    lines.append("                                         ::ezxml::XElementIterator &endIter, bool &isSuccess);")
    lines.append("")
    lines.append("    bool isDirectionType(const std::string &elementName) const;")
    lines.append("    bool isMultiDirectionType(const std::string &elementName) const;")
    lines.append("};")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    lines.append("")
    return "\n".join(lines)


def _generate_direction_cpp(class_name, attrs_name, branch_names, multi_branch_names):
    # Build the anonymous-namespace arrays from XSD-derived branch names.
    directions_init = ", ".join(f'"{n}"' for n in branch_names)
    multi_init = ", ".join(f'"{n}"' for n in multi_branch_names)

    lines = [LICENSE]
    lines.append(f'#include "mx/core/elements/{class_name}.h"')
    lines.append('#include "mx/core/FromXElement.h"')
    lines.append('#include "mx/core/elements/DirectionType.h"')
    lines.append('#include "mx/core/elements/EditorialVoiceDirectionGroup.h"')
    lines.append('#include "mx/core/elements/Footnote.h"')
    lines.append('#include "mx/core/elements/Level.h"')
    lines.append('#include "mx/core/elements/Offset.h"')
    lines.append('#include "mx/core/elements/Sound.h"')
    lines.append('#include "mx/core/elements/Staff.h"')
    lines.append('#include "mx/core/elements/Voice.h"')
    lines.append("")
    # Per-branch direction-type element includes, sorted to match HEAD
    # (alphabetical by C++ class name).
    sorted_branches = sorted(branch_names, key=lambda n: pascal(n))
    for n in sorted_branches:
        lines.append(f'#include "mx/core/elements/{pascal(n)}.h"')
    lines.append("")
    lines.append("#include <cstdio>")
    lines.append("#include <cstring>")
    lines.append("#include <iostream>")
    lines.append("")
    lines.append("namespace")
    lines.append("{")
    lines.append("")
    lines.append(f"constexpr const size_t directionsSize = {len(branch_names)};")
    lines.append(f"constexpr const char *const directions[] = {{{directions_init}}};")
    lines.append("")
    lines.append(f"constexpr const size_t multiDirectionsSize = {len(multi_branch_names)};")
    lines.append(f"constexpr const char *const multiDirections[] = {{{multi_init}}};")
    lines.append("} // namespace")
    lines.append("")
    lines.append("namespace mx")
    lines.append("{")
    lines.append("namespace core")
    lines.append("{")
    lines.append(f"{class_name}::{class_name}()")
    lines.append(f"    : myAttributes(std::make_shared<{attrs_name}>()), myDirectionTypeSet(), myOffset(makeOffset()),")
    lines.append("      myHasOffset(false), myEditorialVoiceDirectionGroup(makeEditorialVoiceDirectionGroup()), myStaff(makeStaff()),")
    lines.append("      myHasStaff(false), mySound(makeSound()), myHasSound(false)")
    lines.append("{")
    lines.append("    myDirectionTypeSet.push_back(makeDirectionType());")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasAttributes() const")
    lines.append("{")
    lines.append("    return myAttributes->hasValues();")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamAttributes(std::ostream &os) const")
    lines.append("{")
    lines.append("    return myAttributes->toStream(os);")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamName(std::ostream &os) const")
    lines.append("{")
    lines.append('    os << "direction";')
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"bool {class_name}::hasContents() const")
    lines.append("{")
    lines.append("    return true;")
    lines.append("}")
    lines.append("")
    lines.append(f"std::ostream &{class_name}::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const")
    lines.append("{")
    lines.append("    for (auto x : myDirectionTypeSet)")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append("        x->toStream(os, indentLevel + 1);")
    lines.append("    }")
    lines.append("    if (myHasOffset)")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append("        myOffset->toStream(os, indentLevel + 1);")
    lines.append("    }")
    lines.append("    if (myEditorialVoiceDirectionGroup->hasContents())")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append("        myEditorialVoiceDirectionGroup->streamContents(os, indentLevel + 1, isOneLineOnly);")
    lines.append("    }")
    lines.append("    if (myHasStaff)")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append("        myStaff->toStream(os, indentLevel + 1);")
    lines.append("    }")
    lines.append("    if (myHasSound)")
    lines.append("    {")
    lines.append("        os << std::endl;")
    lines.append("        mySound->toStream(os, indentLevel + 1);")
    lines.append("    }")
    lines.append("    os << std::endl;")
    lines.append("    isOneLineOnly = false;")
    lines.append("    return os;")
    lines.append("}")
    lines.append("")
    lines.append(f"{attrs_name}Ptr {class_name}::getAttributes() const")
    lines.append("{")
    lines.append("    return myAttributes;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::setAttributes(const {attrs_name}Ptr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append("        myAttributes = value;")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"const DirectionTypeSet &{class_name}::getDirectionTypeSet() const")
    lines.append("{")
    lines.append("    return myDirectionTypeSet;")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::addDirectionType(const DirectionTypePtr &value)")
    lines.append("{")
    lines.append("    if (value)")
    lines.append("    {")
    lines.append("        myDirectionTypeSet.push_back(value);")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::removeDirectionType(const DirectionTypeSetIterConst &value)")
    lines.append("{")
    lines.append("    if (value != myDirectionTypeSet.cend())")
    lines.append("    {")
    lines.append("        if (myDirectionTypeSet.size() > 1)")
    lines.append("        {")
    lines.append("            myDirectionTypeSet.erase(value);")
    lines.append("        }")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append(f"void {class_name}::clearDirectionTypeSet()")
    lines.append("{")
    lines.append("    myDirectionTypeSet.clear();")
    lines.append("    myDirectionTypeSet.push_back(makeDirectionType());")
    lines.append("}")
    lines.append("")
    lines.append(f"DirectionTypePtr {class_name}::getDirectionType(const DirectionTypeSetIterConst &setIterator) const")
    lines.append("{")
    lines.append("    if (setIterator != myDirectionTypeSet.cend())")
    lines.append("    {")
    lines.append("        return *setIterator;")
    lines.append("    }")
    lines.append("    return DirectionTypePtr();")
    lines.append("}")
    lines.append("")
    # Plain single-occurrence accessors: offset, editorial-voice-direction, staff, sound
    for (cap_field, lower_field, with_has) in [
        ("Offset", "myOffset", True),
        ("EditorialVoiceDirectionGroup", "myEditorialVoiceDirectionGroup", False),
        ("Staff", "myStaff", True),
        ("Sound", "mySound", True),
    ]:
        lines.append(f"{cap_field}Ptr {class_name}::get{cap_field}() const")
        lines.append("{")
        lines.append(f"    return {lower_field};")
        lines.append("}")
        lines.append("")
        lines.append(f"void {class_name}::set{cap_field}(const {cap_field}Ptr &value)")
        lines.append("{")
        lines.append("    if (value)")
        lines.append("    {")
        lines.append(f"        {lower_field} = value;")
        lines.append("    }")
        lines.append("}")
        lines.append("")
        if with_has:
            lines.append(f"bool {class_name}::getHas{cap_field}() const")
            lines.append("{")
            lines.append(f"    return myHas{cap_field};")
            lines.append("}")
            lines.append("")
            lines.append(f"void {class_name}::setHas{cap_field}(const bool value)")
            lines.append("{")
            lines.append(f"    myHas{cap_field} = value;")
            lines.append("}")
            lines.append("")

    # fromXElementImpl
    lines.append(f"bool {class_name}::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)")
    lines.append("{")
    lines.append("    bool isSuccess = true;")
    lines.append("    bool isDirectionTypeFound = false;")
    lines.append("    isSuccess &= myAttributes->fromXElement(message, xelement);")
    lines.append("")
    lines.append("    auto endIter = xelement.end();")
    lines.append("    for (auto it = xelement.begin(); it != endIter; ++it)")
    lines.append("    {")
    lines.append("        if (importDirectionTypeSet(message, it, endIter, isSuccess, isDirectionTypeFound))")
    lines.append("        {")
    lines.append("            continue;")
    lines.append("        }")
    lines.append("        if (importElement(message, *it, isSuccess, *myOffset, myHasOffset))")
    lines.append("        {")
    lines.append("            continue;")
    lines.append("        }")
    lines.append("        importGroup(message, it, endIter, isSuccess, myEditorialVoiceDirectionGroup);")
    lines.append("        if (importElement(message, *it, isSuccess, *myStaff, myHasStaff))")
    lines.append("        {")
    lines.append("            continue;")
    lines.append("        }")
    lines.append("        if (importElement(message, *it, isSuccess, *mySound, myHasSound))")
    lines.append("        {")
    lines.append("            continue;")
    lines.append("        }")
    lines.append("    }")
    lines.append("")
    lines.append("    MX_RETURN_IS_SUCCESS;")
    lines.append("}")
    lines.append("")

    # importDirectionTypeSet
    lines.append(f"bool {class_name}::importDirectionTypeSet(std::ostream &message, ::ezxml::XElementIterator &iter,")
    lines.append("                                       ::ezxml::XElementIterator &endIter, bool &isSuccess, bool &isFound)")
    lines.append("{")
    lines.append("    if (iter == endIter)")
    lines.append("    {")
    lines.append("        isFound = false;")
    lines.append("        return false;")
    lines.append("    }")
    lines.append("")
    lines.append('    if (iter->getName() != "direction-type")')
    lines.append("    {")
    lines.append("        isFound = false;")
    lines.append("        return false;")
    lines.append("    }")
    lines.append("")
    lines.append("    isFound = true;")
    lines.append("    bool isIterIncremented = false;")
    lines.append("    bool isFirstDirectionTypeAdded = false;")
    lines.append("")
    lines.append('    while ((iter != endIter) && (iter->getName() == "direction-type"))')
    lines.append("    {")
    lines.append("        auto subiter = iter->begin();")
    lines.append("        auto subiterEnd = iter->end();")
    lines.append("")
    lines.append("        auto directionType = createDirectionType(message, subiter, subiterEnd, isSuccess);")
    lines.append("")
    lines.append("        if (!isFirstDirectionTypeAdded && myDirectionTypeSet.size() == 1)")
    lines.append("        {")
    lines.append("            *myDirectionTypeSet.begin() = directionType;")
    lines.append("            isFirstDirectionTypeAdded = true;")
    lines.append("        }")
    lines.append("        else")
    lines.append("        {")
    lines.append("            myDirectionTypeSet.push_back(directionType);")
    lines.append("            isFirstDirectionTypeAdded = true;")
    lines.append("        }")
    lines.append("")
    lines.append("        isIterIncremented = true;")
    lines.append("        ++iter;")
    lines.append("    }")
    lines.append("")
    lines.append("    if (isIterIncremented)")
    lines.append("    {")
    lines.append("        --iter;")
    lines.append("    }")
    lines.append("    return isFirstDirectionTypeAdded;")
    lines.append("}")
    lines.append("")

    # isDirectionType
    lines.append(f"bool {class_name}::isDirectionType(const std::string &elementName) const")
    lines.append("{")
    lines.append("    for (size_t i = 0; i < directionsSize; ++i)")
    lines.append("    {")
    lines.append("        if (strcmp(directions[i], elementName.c_str()) == 0)")
    lines.append("        {")
    lines.append("            return true;")
    lines.append("        }")
    lines.append("    }")
    lines.append("    return false;")
    lines.append("}")
    lines.append("")

    # isMultiDirectionType
    lines.append(f"bool {class_name}::isMultiDirectionType(const std::string &elementName) const")
    lines.append("{")
    lines.append("    for (size_t i = 0; i < multiDirectionsSize; ++i)")
    lines.append("    {")
    lines.append("        if (strcmp(multiDirections[i], elementName.c_str()) == 0)")
    lines.append("        {")
    lines.append("            return true;")
    lines.append("        }")
    lines.append("    }")
    lines.append("    return false;")
    lines.append("}")
    lines.append("")

    # createDirectionType
    lines.append(f"DirectionTypePtr {class_name}::createDirectionType(std::ostream &message, ::ezxml::XElementIterator &subIter,")
    lines.append("                                                ::ezxml::XElementIterator &subIterEnd, bool &isSuccess)")
    lines.append("{")
    lines.append("    auto directionType = makeDirectionType();")
    lines.append("")
    lines.append("    if (subIter == subIterEnd)")
    lines.append("    {")
    lines.append('        message << "Direction: well thats weird - should not get here" << std::endl;')
    lines.append("        isSuccess = false;")
    lines.append("        return directionType;")
    lines.append("    }")
    lines.append("")

    # Single-occurrence branches (non-multi): emit if-block with setChoice + fromXElement.
    # HEAD emits these BEFORE the multi branches.
    multi_set = set(multi_branch_names)
    for n in branch_names:
        if n in multi_set:
            continue
        cls = pascal(n)            # e.g. OctaveShift
        getter = f"get{cls}"       # e.g. getOctaveShift
        choice_enum = camel(n)     # e.g. octaveShift
        lines.append(f'    if (subIter->getName() == "{n}")')
        lines.append("    {")
        lines.append(f"        directionType->setChoice(DirectionType::Choice::{choice_enum});")
        lines.append(f"        isSuccess &= directionType->{getter}()->fromXElement(message, *subIter);")
        lines.append("        return directionType;")
        lines.append("    }")
        lines.append("")

    # Multi branches: HEAD orders them rehearsal, segno, words, coda, dynamics,
    # percussion -- which is the XSD order filtered to multi.
    for idx, n in enumerate(multi_branch_names):
        cls = pascal(n)
        choice_enum = camel(n)
        if idx == 0:
            lines.append(f'    std::string name = "{n}";')
        else:
            lines.append(f'    name = "{n}";')
        lines.append("    if (subIter->getName() == name)")
        lines.append("    {")
        lines.append(f"        directionType->setChoice(DirectionType::Choice::{choice_enum});")
        lines.append("        bool isFirstSubItemAdded = false;")
        lines.append("")
        lines.append("        while (subIter != subIterEnd)")
        lines.append("        {")
        lines.append("            if (subIter->getName() != name)")
        lines.append("            {")
        lines.append(f'                message << "Direction: createDirectionType encountered an unexpected element \'" << subIter->getName()')
        lines.append(f'                        << "\' while parsing a collection of \'" << name << "\' elements" << std::endl;')
        lines.append("                isSuccess = false;")
        lines.append("                return directionType;")
        lines.append("            }")
        lines.append(f"            auto itemToAdd = make{cls}();")
        lines.append("            isSuccess &= itemToAdd->fromXElement(message, *subIter);")
        lines.append(f"            if (!isFirstSubItemAdded && directionType->get{cls}Set().size() == 1)")
        lines.append("            {")
        lines.append(f"                directionType->add{cls}(itemToAdd);")
        lines.append(f"                directionType->remove{cls}(directionType->get{cls}Set().cbegin());")
        lines.append("            }")
        lines.append("            else")
        lines.append("            {")
        lines.append(f"                directionType->add{cls}(itemToAdd);")
        lines.append("            }")
        lines.append("            isFirstSubItemAdded = true;")
        lines.append("            ++subIter;")
        lines.append("        } // end loop")
        lines.append("        return directionType;")
        lines.append(f"    }} // end {n}")
        lines.append("")

    lines.append("    return directionType;")
    lines.append("}")
    lines.append("")
    lines.append("} // namespace core")
    lines.append("} // namespace mx")
    lines.append("")
    return "\n".join(lines)


BESPOKE_ELEMENTS = {
    "credit": _emit_credit_family,
    "lyric": _emit_lyric_family,
    "note": _emit_note_family,
    "part-list": _emit_part_list_family,
    "harmony": _emit_harmony_family,
    "score-partwise": _emit_score_wrapper_family,
    "score-timewise": _emit_score_wrapper_family,
    "direction": _emit_direction_family,
}


def _parse_config() -> ParseConfig:
    """Bundle the C++-aware structural-config globals for injection into the parser.

    The four set fields are passed by reference: the parser records synthetic groups
    into them during parsing, and the emission code below reads the same objects.
    """
    return ParseConfig(
        generate_groups=GENERATE_GROUPS,
        synthetic_optional_groups=SYNTHETIC_OPTIONAL_GROUPS,
        synthetic_unbounded_groups=SYNTHETIC_UNBOUNDED_GROUPS,
        suppress_group_suffix=SUPPRESS_GROUP_SUFFIX,
        extension_optional_group_rename=EXTENSION_OPTIONAL_GROUP_RENAME,
        nested_optional_sequence_as_group=NESTED_OPTIONAL_SEQUENCE_AS_GROUP,
        unbounded_sequence_as_group=UNBOUNDED_SEQUENCE_AS_GROUP,
    )


def main():
    model = XsdModel(XSD_PATH, _parse_config())

    stats = {"attrs_written": 0, "elem_written": 0, "elem_skipped": 0}
    categories = {}

    generated_attrs = set()

    for elem_name, elem in model.elements.items():
        if elem_name in SKIP_ELEMENTS or elem_name in DYNAMICS_MARKS:
            continue
        # Elements claimed by a family-handler (e.g. score-partwise emits
        # PartwisePart and PartwiseMeasure for "part" and "measure") must be
        # silently skipped here so the default path doesn't double-write or
        # produce broken files.
        if elem_name in BESPOKE_FAMILY_OWNED:
            continue

        cat = classify_element(elem, model)
        categories.setdefault(cat, []).append(elem_name)

        ct = model.complex_types.get(elem.type_name) if elem.type_name else None
        if ct is None and elem.anonymous_type is not None:
            ct = elem.anonymous_type

        # Bespoke per-element generators (last-resort path for elements that
        # don't fit any reusable mechanism). Each handler is schema-driven
        # (reads names from the parsed XSD).
        bespoke = BESPOKE_ELEMENTS.get(elem_name)
        if bespoke and ct:
            bespoke(elem_name, elem, ct, model, generated_attrs, stats)
            continue

        # Tree-based generation for elements with nested choice/sequence
        if elem_name in TREE_ELEMENTS and ct and ct.content_tree:
            if True:
                plan = analyze_tree(elem_name, ct.content_tree, model)
                if plan:
                    class_name = element_class_name(elem_name)
                    type_name = elem.type_name or ""
                    has_attrs = bool(ct.attributes)
                    attrs_name = None

                    if has_attrs:
                        attrs_name = resolve_attrs_name(elem_name, type_name, model)
                        if attrs_name not in generated_attrs and attrs_name not in CORE_ROOT_ATTRS:
                            h_content = generate_attrs_h(attrs_name, ct.attributes, model)
                            cpp_content = generate_attrs_cpp(attrs_name, ct.attributes, model)
                            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.h"), h_content)
                            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.cpp"), cpp_content)
                            generated_attrs.add(attrs_name)
                            stats["attrs_written"] += 1

                    for group_class, group_children in plan.groups_to_generate:
                        gh = generate_tree_group_h(group_class, group_children, model)
                        gc = generate_tree_group_cpp(group_class, group_children, model)
                        write_file(os.path.join(ELEM_DIR, f"{group_class}.h"), gh)
                        write_file(os.path.join(ELEM_DIR, f"{group_class}.cpp"), gc)

                    for og_class, og_children in plan.optional_groups_to_generate:
                        gh = generate_tree_group_h(og_class, og_children, model)
                        gc = generate_tree_group_cpp(og_class, og_children, model)
                        write_file(os.path.join(ELEM_DIR, f"{og_class}.h"), gh)
                        write_file(os.path.join(ELEM_DIR, f"{og_class}.cpp"), gc)

                    for nc_class, nc_branches, nc_parent in plan.nested_choices_to_generate:
                        nch = generate_tree_choice_h(nc_class, nc_branches, nc_parent)
                        ncc = generate_tree_choice_cpp(nc_class, nc_branches, nc_parent)
                        write_file(os.path.join(ELEM_DIR, f"{nc_class}.h"), nch)
                        write_file(os.path.join(ELEM_DIR, f"{nc_class}.cpp"), ncc)

                    for container in plan.containers_to_generate:
                        cth = generate_container_h(container)
                        ctc = generate_container_cpp(container)
                        write_file(os.path.join(ELEM_DIR, f"{container.class_name}.h"), cth)
                        write_file(os.path.join(ELEM_DIR, f"{container.class_name}.cpp"), ctc)

                    tree_config = _get_tree_config(elem_name)
                    if tree_config.get("inlined_choice"):
                        inline_cfg = {
                            "branches": [
                                {
                                    "enum_name": b.enum_name,
                                    "class_name": b.class_name,
                                    "is_group": b.is_group,
                                    "element_name": b.xml_name or b.class_name,
                                }
                                for b in plan.choice_branches
                            ],
                            "enum_start": 1,
                        }
                        ph = generate_inline_choice_h(elem_name, class_name, inline_cfg,
                                                     has_attrs, attrs_name)
                        pc = generate_inline_choice_cpp(elem_name, class_name, inline_cfg,
                                                       has_attrs, attrs_name, elem_name)
                        write_file(os.path.join(ELEM_DIR, f"{class_name}.h"), ph)
                        write_file(os.path.join(ELEM_DIR, f"{class_name}.cpp"), pc)
                        stats["elem_written"] += 1
                        stats["tree_written"] = stats.get("tree_written", 0) + 1
                        continue

                    if len(plan.inline_choices) > 1:
                        for ic in plan.inline_choices:
                            ch = generate_tree_choice_h(ic.choice_class, ic.branches, elem_name)
                            cc = generate_tree_choice_cpp(ic.choice_class, ic.branches, elem_name)
                            write_file(os.path.join(ELEM_DIR, f"{ic.choice_class}.h"), ch)
                            write_file(os.path.join(ELEM_DIR, f"{ic.choice_class}.cpp"), cc)
                    else:
                        ch = generate_tree_choice_h(plan.choice_class, plan.choice_branches, elem_name)
                        cc = generate_tree_choice_cpp(plan.choice_class, plan.choice_branches, elem_name)
                        write_file(os.path.join(ELEM_DIR, f"{plan.choice_class}.h"), ch)
                        write_file(os.path.join(ELEM_DIR, f"{plan.choice_class}.cpp"), cc)

                    ph = generate_tree_parent_h(elem_name, class_name, plan.choice_class,
                                                plan.trailing_children, has_attrs, attrs_name, model,
                                                choice_is_set=plan.choice_is_set,
                                                leading_groups=plan.leading_groups,
                                                leading_children=plan.leading_children,
                                                choice_is_optional=plan.choice_is_optional,
                                                inline_choices=plan.inline_choices,
                                                choice_branches=plan.choice_branches)
                    pc = generate_tree_parent_cpp(elem_name, class_name, plan.choice_class,
                                                  plan.trailing_children, has_attrs, attrs_name,
                                                  plan, model)
                    write_file(os.path.join(ELEM_DIR, f"{class_name}.h"), ph)
                    write_file(os.path.join(ELEM_DIR, f"{class_name}.cpp"), pc)
                    stats["elem_written"] += 1
                    stats["tree_written"] = stats.get("tree_written", 0) + 1
                    continue

        if cat == "choice" and elem_name not in CHOICE_SKIP:
            enum_val_cfg = ENUM_VALUE_CHOICE_CONFIG.get(elem_name)
            inline_cfg = INLINE_CHOICE_CONFIG.get(elem_name)
            if enum_val_cfg and ct:
                class_name = element_class_name(elem_name)
                has_attrs = bool(ct.attributes)
                attrs_name = None
                type_name = elem.type_name or ""
                if has_attrs:
                    attrs_name = resolve_attrs_name(elem_name, type_name, model)
                    if attrs_name not in generated_attrs and attrs_name not in CORE_ROOT_ATTRS:
                        h_content = generate_attrs_h(attrs_name, ct.attributes, model)
                        cpp_content = generate_attrs_cpp(attrs_name, ct.attributes, model)
                        write_file(os.path.join(ELEM_DIR, f"{attrs_name}.h"), h_content)
                        write_file(os.path.join(ELEM_DIR, f"{attrs_name}.cpp"), cpp_content)
                        generated_attrs.add(attrs_name)
                        stats["attrs_written"] += 1
                h_content = generate_enum_value_choice_h(
                    elem_name, class_name, enum_val_cfg["value_type"],
                    has_attrs, attrs_name)
                cpp_content = generate_enum_value_choice_cpp(
                    elem_name, class_name, enum_val_cfg["value_type"],
                    enum_val_cfg["enum_type"], enum_val_cfg["other_variant"],
                    enum_val_cfg["other_xml_name"], has_attrs, attrs_name)
                write_file(os.path.join(ELEM_DIR, f"{class_name}.h"), h_content)
                write_file(os.path.join(ELEM_DIR, f"{class_name}.cpp"), cpp_content)
                stats["elem_written"] += 1
            elif inline_cfg and ct:
                class_name = element_class_name(elem_name)
                stream_name = elem_name
                type_name = elem.type_name or ""
                has_attrs = bool(ct.attributes)
                attrs_name = None

                if has_attrs:
                    attrs_name = resolve_attrs_name(elem_name, type_name, model)
                    if attrs_name not in generated_attrs and attrs_name not in CORE_ROOT_ATTRS:
                        h_content = generate_attrs_h(attrs_name, ct.attributes, model)
                        cpp_content = generate_attrs_cpp(attrs_name, ct.attributes, model)
                        write_file(os.path.join(ELEM_DIR, f"{attrs_name}.h"), h_content)
                        write_file(os.path.join(ELEM_DIR, f"{attrs_name}.cpp"), cpp_content)
                        generated_attrs.add(attrs_name)
                        stats["attrs_written"] += 1

                for branch in inline_cfg["branches"]:
                    if branch.get("is_group"):
                        group_children = [
                            XsdChildRef(
                                element_name=c["name"],
                                min_occurs=c["min"],
                                max_occurs=c["max"],
                            )
                            for c in branch["children"]
                        ]
                        group_class = branch["class_name"]
                        group_base = group_class.replace("Group", "").lower()
                        gh = generate_group_h(group_base, group_children, model)
                        gc = generate_group_cpp(group_base, group_children, model)
                        write_file(os.path.join(ELEM_DIR, f"{group_class}.h"), gh)
                        write_file(os.path.join(ELEM_DIR, f"{group_class}.cpp"), gc)

                ph = generate_inline_choice_h(elem_name, class_name, inline_cfg,
                                              has_attrs, attrs_name)
                pc = generate_inline_choice_cpp(elem_name, class_name, inline_cfg,
                                                has_attrs, attrs_name, stream_name)
                write_file(os.path.join(ELEM_DIR, f"{class_name}.h"), ph)
                write_file(os.path.join(ELEM_DIR, f"{class_name}.cpp"), pc)
                stats["elem_written"] += 1
                stats["inline_choice_written"] = stats.get("inline_choice_written", 0) + 1

            else:
                config = CHOICE_ELEMENT_CONFIG.get(elem_name)
                if config and ct and ct.choice_children:
                    choice_class = config["choice_class"]
                    is_set = config["is_set"]
                    enum_start = config["enum_start"]
                    class_name = element_class_name(elem_name)
                    stream_name = elem_name
                    type_name = elem.type_name or ""
                    has_attrs = bool(ct.attributes)
                    attrs_name = None

                    if has_attrs:
                        attrs_name = resolve_attrs_name(elem_name, type_name, model)
                        if attrs_name not in generated_attrs and attrs_name not in CORE_ROOT_ATTRS:
                            h_content = generate_attrs_h(attrs_name, ct.attributes, model)
                            cpp_content = generate_attrs_cpp(attrs_name, ct.attributes, model)
                            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.h"), h_content)
                            write_file(os.path.join(ELEM_DIR, f"{attrs_name}.cpp"), cpp_content)
                            generated_attrs.add(attrs_name)
                            stats["attrs_written"] += 1

                    choice_children = list(ct.choice_children)
                    extra = config.get("extra_children", [])
                    if extra:
                        after_name = config.get("extra_children_after")
                        if after_name:
                            insert_idx = next((i for i, c in enumerate(choice_children)
                                              if c.element_name == after_name), len(choice_children) - 1) + 1
                        else:
                            insert_idx = len(choice_children) - 1
                        for ec in extra:
                            choice_children.insert(insert_idx, type('obj', (), {
                                'element_name': ec, 'min_occurs': 1, 'max_occurs': 1})())
                            insert_idx += 1

                    if config.get("bespoke_choice"):
                        ch = generate_time_choice_h()
                        cc = generate_time_choice_cpp()
                        write_file(os.path.join(ELEM_DIR, f"{choice_class}.h"), ch)
                        write_file(os.path.join(ELEM_DIR, f"{choice_class}.cpp"), cc)
                        gh = generate_group_h("time-signature",
                                              TIME_SIGNATURE_GROUP_CHILDREN, model)
                        gc = generate_group_cpp("time-signature",
                                                TIME_SIGNATURE_GROUP_CHILDREN, model)
                        write_file(os.path.join(ELEM_DIR, "TimeSignatureGroup.h"), gh)
                        write_file(os.path.join(ELEM_DIR, "TimeSignatureGroup.cpp"), gc)
                    else:
                        choice_is_set = config.get("choice_is_set", is_set)
                        ch = generate_choice_class_h(choice_class, choice_children,
                                                     choice_is_set, enum_start, elem_name, model)
                        cc = generate_choice_class_cpp(choice_class, choice_children,
                                                      choice_is_set, enum_start, elem_name,
                                                      model, config)
                        write_file(os.path.join(ELEM_DIR, f"{choice_class}.h"), ch)
                        write_file(os.path.join(ELEM_DIR, f"{choice_class}.cpp"), cc)

                    if not config.get("skip_parent"):
                        ph = generate_choice_parent_h(elem_name, class_name, choice_class,
                                                      is_set, has_attrs, attrs_name, model,
                                                      config)
                        pc = generate_choice_parent_cpp(elem_name, class_name, choice_class,
                                                       is_set, has_attrs, attrs_name,
                                                       stream_name, model, config,
                                                       choice_children)
                        write_file(os.path.join(ELEM_DIR, f"{class_name}.h"), ph)
                        write_file(os.path.join(ELEM_DIR, f"{class_name}.cpp"), pc)
                    stats["elem_written"] += 1
                    stats["choice_written"] = stats.get("choice_written", 0) + 1
                else:
                    stats["elem_skipped"] += 1

        elif cat in ("empty-with-attrs", "text-with-attrs", "complex-with-attrs",
                    "complex", "text-value", "empty", "simple-value"):

            class_name = element_class_name(elem_name)
            stream_name = elem_name
            type_name = elem.type_name or ""

            if ct and ct.attributes:
                sname = resolve_attrs_name(elem_name, type_name, model)
                if sname not in generated_attrs and sname not in CORE_ROOT_ATTRS:
                    h_content = generate_attrs_h(sname, ct.attributes, model)
                    cpp_content = generate_attrs_cpp(sname, ct.attributes, model)
                    write_file(os.path.join(ELEM_DIR, f"{sname}.h"), h_content)
                    write_file(os.path.join(ELEM_DIR, f"{sname}.cpp"), cpp_content)
                    generated_attrs.add(sname)
                    stats["attrs_written"] += 1

            if cat == "simple-value":
                value_type = resolve_cpp_type(elem.type_name, model)
                fake_ct = XsdComplexType(name=elem.type_name)
                fake_ct.has_simple_content = True
                fake_ct.simple_content_base = elem.type_name
                h_content = generate_element_h(elem_name, class_name, stream_name, cat, fake_ct, model, type_name)
                cpp_content = generate_element_cpp(elem_name, class_name, stream_name, cat, fake_ct, model, type_name)
            else:
                h_content = generate_element_h(elem_name, class_name, stream_name, cat, ct, model, type_name)
                cpp_content = generate_element_cpp(elem_name, class_name, stream_name, cat, ct, model, type_name)

            write_file(os.path.join(ELEM_DIR, f"{class_name}.h"), h_content)
            write_file(os.path.join(ELEM_DIR, f"{class_name}.cpp"), cpp_content)
            stats["elem_written"] += 1
        else:
            stats["elem_skipped"] += 1

    # Generate Group wrapper classes
    groups_written = 0
    for group_name, group_children in model.groups.items():
        if group_name not in GENERATE_GROUPS:
            continue
        h_content = generate_group_h(group_name, group_children, model)
        cpp_content = generate_group_cpp(group_name, group_children, model)
        class_name = group_class_name(group_name)
        write_file(os.path.join(ELEM_DIR, f"{class_name}.h"), h_content)
        write_file(os.path.join(ELEM_DIR, f"{class_name}.cpp"), cpp_content)
        groups_written += 1

    print("=== Generation Stats ===")
    print(f"Attributes structs written: {stats['attrs_written']}")
    print(f"Element classes written:    {stats['elem_written']}")
    print(f"Choice classes written:     {stats.get('choice_written', 0)}")
    print(f"Inline choice written:     {stats.get('inline_choice_written', 0)}")
    print(f"Tree-based written:        {stats.get('tree_written', 0)}")
    print(f"Group classes written:      {groups_written}")
    print(f"Element classes skipped:    {stats['elem_skipped']}")
    print()
    print("=== Element Categories ===")
    for cat, names in sorted(categories.items()):
        print(f"  {cat}: {len(names)}")
    print()
    print("=== Skipped Elements ===")
    for cat in ["choice", "sequence-with-choice", "anonymous", "unknown"]:
        if cat in categories:
            for n in categories[cat][:10]:
                print(f"  [{cat}] {n}")


def write_file(path, content):
    with open(path, "w") as f:
        f.write(content)


if __name__ == "__main__":
    main()
