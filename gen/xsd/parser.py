"""Parse a MusicXML XSD file into the model in model.py.

Uses the stdlib ElementTree. The parser is intentionally strict about the
MusicXML subset: it understands the element kinds the schema actually uses and
ignores annotations and XML comments. It does not resolve imported schemas
(xml.xsd, xlink.xsd); attribute refs into those namespaces are kept verbatim.
"""

from __future__ import annotations

import xml.etree.ElementTree as ET
from pathlib import Path

from gen.xsd.model import (
    Attribute,
    AttributeGroup,
    AttributeGroupRef,
    Choice,
    ComplexContent,
    ComplexType,
    ElementParticle,
    Enumeration,
    Facets,
    Group,
    GroupRef,
    ImplicitContent,
    ListType,
    Restriction,
    Schema,
    Sequence,
    SimpleContent,
    SimpleType,
    TopLevelElement,
    Union,
    UNBOUNDED,
)

XS = "{http://www.w3.org/2001/XMLSchema}"


def parse(path: str | Path) -> Schema:
    root = ET.parse(path).getroot()
    return _Parser().schema(root)


def _local(tag: object) -> str:
    """Local name of an element tag; '' for comments/PIs (non-str tags)."""
    return tag.rsplit("}", 1)[-1] if isinstance(tag, str) else ""


def _kids(elem: ET.Element) -> list[ET.Element]:
    """Real child elements, skipping annotations and XML comments."""
    return [c for c in elem if isinstance(c.tag, str) and _local(c.tag) != "annotation"]


def _doc(elem: ET.Element) -> str | None:
    for child in elem:
        if _local(child.tag) == "annotation":
            for d in child:
                if _local(d.tag) == "documentation":
                    text = (d.text or "").strip()
                    return text or None
    return None


def _min_occurs(elem: ET.Element) -> int:
    return int(elem.get("minOccurs", "1"))


def _max_occurs(elem: ET.Element) -> int:
    value = elem.get("maxOccurs", "1")
    return UNBOUNDED if value == "unbounded" else int(value)


class _Parser:
    def schema(self, root: ET.Element) -> Schema:
        schema = Schema(
            simple_types={},
            complex_types={},
            groups={},
            attribute_groups={},
            elements=[],
            imports=[],
        )
        for child in _kids(root):
            kind = _local(child.tag)
            if kind == "import":
                schema.imports.append(
                    (child.get("namespace", ""), child.get("schemaLocation", ""))
                )
            elif kind == "simpleType":
                st = self.simple_type(child)
                schema.simple_types[st.name] = st
            elif kind == "complexType":
                ct = self.complex_type(child)
                schema.complex_types[ct.name] = ct
            elif kind == "group":
                g = self.group(child)
                schema.groups[g.name] = g
            elif kind == "attributeGroup":
                ag = self.attribute_group(child)
                schema.attribute_groups[ag.name] = ag
            elif kind == "element":
                schema.elements.append(self.top_level_element(child))
        return schema

    # ----- simple types ---------------------------------------------------- #

    def simple_type(self, elem: ET.Element) -> SimpleType:
        body = _kids(elem)[0]
        kind = _local(body.tag)
        if kind == "restriction":
            content = Restriction(body.get("base", ""), self.facets(body))
        elif kind == "union":
            members = body.get("memberTypes", "").split()
            inline = [self.simple_type(c) for c in _kids(body) if _local(c.tag) == "simpleType"]
            content = Union(members, inline)
        elif kind == "list":
            content = ListType(body.get("itemType", ""))
        else:
            raise ValueError(f"unexpected simpleType body: {kind}")
        return SimpleType(elem.get("name"), _doc(elem), content)

    def facets(self, restriction: ET.Element) -> Facets:
        facets = Facets()
        for f in _kids(restriction):
            kind = _local(f.tag)
            value = f.get("value", "")
            if kind == "enumeration":
                facets.enumerations.append(Enumeration(value, _doc(f)))
            elif kind == "pattern":
                facets.patterns.append(value)
            elif kind == "minInclusive":
                facets.min_inclusive = value
            elif kind == "maxInclusive":
                facets.max_inclusive = value
            elif kind == "minExclusive":
                facets.min_exclusive = value
            elif kind == "maxExclusive":
                facets.max_exclusive = value
            elif kind == "minLength":
                facets.min_length = value
            elif kind == "maxLength":
                facets.max_length = value
            elif kind == "length":
                facets.length = value
            # whiteSpace and other facets are not needed by the generator.
        return facets

    # ----- attributes ------------------------------------------------------ #

    def attribute(self, elem: ET.Element) -> Attribute:
        inline = next((self.simple_type(c) for c in _kids(elem) if _local(c.tag) == "simpleType"), None)
        return Attribute(
            name=elem.get("name"),
            ref=elem.get("ref"),
            type=elem.get("type"),
            inline_type=inline,
            use=elem.get("use", "optional"),
            default=elem.get("default"),
            fixed=elem.get("fixed"),
            doc=_doc(elem),
        )

    def _attributes(self, elem: ET.Element) -> tuple[list[Attribute], list[AttributeGroupRef]]:
        """Collect direct xs:attribute and xs:attributeGroup ref children."""
        attrs: list[Attribute] = []
        refs: list[AttributeGroupRef] = []
        for child in _kids(elem):
            kind = _local(child.tag)
            if kind == "attribute":
                attrs.append(self.attribute(child))
            elif kind == "attributeGroup":
                refs.append(AttributeGroupRef(child.get("ref", "")))
        return attrs, refs

    def attribute_group(self, elem: ET.Element) -> AttributeGroup:
        attrs, refs = self._attributes(elem)
        return AttributeGroup(elem.get("name", ""), _doc(elem), attrs, refs)

    # ----- particles ------------------------------------------------------- #

    def particle(self, elem: ET.Element):
        kind = _local(elem.tag)
        if kind == "element":
            return self.element_particle(elem)
        if kind == "group":
            return GroupRef(elem.get("ref", ""), _min_occurs(elem), _max_occurs(elem))
        if kind == "sequence":
            return Sequence(self._particle_items(elem), _min_occurs(elem), _max_occurs(elem))
        if kind == "choice":
            return Choice(self._particle_items(elem), _min_occurs(elem), _max_occurs(elem))
        raise ValueError(f"unexpected particle: {kind}")

    def _particle_items(self, container: ET.Element) -> list:
        return [self.particle(c) for c in _kids(container)]

    def element_particle(self, elem: ET.Element) -> ElementParticle:
        inline = None
        for child in _kids(elem):
            tag = _local(child.tag)
            if tag == "complexType":
                inline = self.complex_type(child)
            elif tag == "simpleType":
                inline = self.simple_type(child)
        return ElementParticle(
            name=elem.get("name", ""),
            type=elem.get("type"),
            inline_type=inline,
            min_occurs=_min_occurs(elem),
            max_occurs=_max_occurs(elem),
            doc=_doc(elem),
        )

    def group(self, elem: ET.Element) -> Group:
        return Group(elem.get("name", ""), _doc(elem), self.particle(_kids(elem)[0]))

    # ----- complex types --------------------------------------------------- #

    def complex_type(self, elem: ET.Element) -> ComplexType:
        children = _kids(elem)
        first = children[0] if children else None
        first_kind = _local(first.tag) if first is not None else ""

        if first_kind == "simpleContent":
            content = self._simple_content(first)
        elif first_kind == "complexContent":
            content = self._complex_content(first)
        else:
            content = self._implicit_content(elem)
        return ComplexType(elem.get("name"), _doc(elem), content)

    def _simple_content(self, elem: ET.Element) -> SimpleContent:
        body = _kids(elem)[0]  # extension or restriction
        attrs, refs = self._attributes(body)
        return SimpleContent(body.get("base", ""), _local(body.tag), attrs, refs)

    def _complex_content(self, elem: ET.Element) -> ComplexContent:
        body = _kids(elem)[0]  # extension or restriction
        attrs, refs = self._attributes(body)
        particle = self._find_particle(body)
        return ComplexContent(body.get("base", ""), _local(body.tag), particle, attrs, refs)

    def _implicit_content(self, elem: ET.Element) -> ImplicitContent:
        attrs, refs = self._attributes(elem)
        return ImplicitContent(self._find_particle(elem), attrs, refs)

    def _find_particle(self, container: ET.Element):
        for child in _kids(container):
            if _local(child.tag) in ("sequence", "choice", "group"):
                return self.particle(child)
        return None

    # ----- top-level element ----------------------------------------------- #

    def top_level_element(self, elem: ET.Element) -> TopLevelElement:
        inline = next((self.complex_type(c) for c in _kids(elem) if _local(c.tag) == "complexType"), None)
        return TopLevelElement(elem.get("name", ""), elem.get("type"), inline, _doc(elem))
