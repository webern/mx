#!/usr/bin/env python3
"""Group structural configuration for the code generator.

Owns the mutable sets that the XSD parser populates during parsing (passed by
reference via ParseConfig), the static dicts that control group synthesis, and
the group_class_name helper. Also includes WRAPPING_STREAMCONTENTS which
controls streaming behavior for a handful of complex types.
"""
from parse import pascal

# ---------------------------------------------------------------------------
# Mutable sets populated by XsdModel during parsing
# ---------------------------------------------------------------------------

# Populated dynamically by XsdModel._synthesize_optional_group when we
# discover an anonymous <xs:sequence minOccurs="0"> inside a parent sequence
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

# Group names whose generated class name omits the trailing "Group" suffix.
SUPPRESS_GROUP_SUFFIX: set = set()

# ---------------------------------------------------------------------------
# Static group configuration dicts
# ---------------------------------------------------------------------------

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

# Complex types whose streamContents uses the "wrapping" (forEachChild) pattern
# instead of explicit per-child streaming.
WRAPPING_STREAMCONTENTS = {
    "defaults", "grouping", "identification", "part-group", "print",
}

# Groups whose generated .cpp includes a real fromXElementImpl body (most
# groups just emit a stub that returns false).
GROUPS_WITH_REAL_FROM_X_ELEMENT = {
    "score-header",
    # ArrowGroup is the inline-group branch of the <arrow> inline-choice element
    # (INLINE_CHOICE_CONFIG["arrow"]). Arrow::fromXElementImpl dispatches its
    # group branch via myArrowGroup->fromXElement(message, xelement), so the
    # group needs a real parsing body.
    "arrow",
}

# ---------------------------------------------------------------------------
# Group class name resolution
# ---------------------------------------------------------------------------


def group_class_name(group_name: str) -> str:
    if group_name in SUPPRESS_GROUP_SUFFIX:
        return pascal(group_name)
    return pascal(group_name) + "Group"
