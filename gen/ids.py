#!/usr/bin/env python3
"""NodeId: a typed, canonically-stringified identifier for nodes in the parsed XSD model.

Assigned in ``parse.py``. Currently additive and unconsumed: every parsed node carries a
``NodeId``, but no generation logic reads it yet. Later milestones key structural configuration
and cross-unit lookups off these IDs.

Canonical string form
---------------------
A ``NodeId`` is a ``/``-joined path of segments. The first segment names a top-level XSD
construct by kind and name; each subsequent segment names a child relative to its parent.

  named construct:   ``<kind>:<name>``     e.g. ``el:note``, ``cx:note-type``,
                                                ``st:above-below``, ``gr:editorial``,
                                                ``ag:bend-sound``, ``cx:note-type/at:type``
  anonymous child:   ``<kind>#<ordinal>``  e.g. ``cx:note-type/seq#0``,
                                                ``cx:note-type/seq#0/choice#1``

Kinds: ``el`` (element), ``cx`` (complexType), ``st`` (simpleType), ``gr`` (group),
``ag`` (attributeGroup), ``at`` (attribute), ``seq`` (sequence), ``choice`` (choice).

Kind is embedded in every segment so that a local element and a local attribute sharing a name
under one owner cannot collide. Same-kind anonymous siblings are disambiguated by ordinal.

Only the leading named-construct segment is version-stable; nested/anonymous segments are
positional and run-local by design.
"""
from dataclasses import dataclass


@dataclass(frozen=True)
class NodeId:
    """An immutable identifier with a canonical ``str`` form (``value``)."""

    value: str

    def __str__(self) -> str:
        return self.value

    @classmethod
    def root(cls, kind: str, name: str) -> "NodeId":
        """Identifier for a top-level named construct, e.g. ``el:note``."""
        return cls(f"{kind}:{name}")

    def named_child(self, kind: str, name: str) -> "NodeId":
        """Identifier for a named child, e.g. ``cx:note-type/el:pitch``."""
        return NodeId(f"{self.value}/{kind}:{name}")

    def anon_child(self, kind: str, ordinal: int) -> "NodeId":
        """Identifier for an anonymous child, e.g. ``cx:note-type/seq#0``."""
        return NodeId(f"{self.value}/{kind}#{ordinal}")
