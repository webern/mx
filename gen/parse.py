#!/usr/bin/env python3
"""Parse ``docs/musicxml.xsd`` into a target-neutral, self-contained XSD model.

This module owns every step that touches the XML: the dataclasses describing XSD constructs,
the ``XsdModel`` parser, and ``NodeId`` assignment. It produces pure XSD facts -- no C++ names,
type maps, or output formatting -- so that a future Rust / docs / JSON-schema backend can consume
the same model.

Structural configuration that the original hand-generation baked in (which anonymous sequences
become synthetic group classes, which inherited groups are renamed, which groups are emitted as
their own class) is C++-aware and lives in ``generate.py``. It is injected via ``ParseConfig`` so
that this module stays free of hardcoded config.

Self-containment: after parsing, ``model.tree`` is not retained. ``model.root`` is still read by
the four bespoke family handlers in ``generate.py`` (harmony-chord, score-wrapper, music-data,
full-note); severing it fully waits on those handlers migrating to parsed data.
"""
import re
import xml.etree.ElementTree as ET
from collections import OrderedDict
from dataclasses import dataclass, field
from typing import Optional

from ids import NodeId

XS = "{http://www.w3.org/2001/XMLSchema}"


def pascal(name: str) -> str:
    return "".join(p[:1].upper() + p[1:] for p in re.split(r"[-_]", name))


@dataclass
class ParseConfig:
    """C++-aware structural config injected into the parser by ``generate.py``.

    The four set fields are mutated in place during parsing (the parser discovers and records
    synthetic groups); ``generate.py`` reads the same set objects after parse. The dict fields
    are read-only inputs.
    """
    generate_groups: set
    synthetic_optional_groups: set
    synthetic_unbounded_groups: set
    suppress_group_suffix: set
    extension_optional_group_rename: dict
    nested_optional_sequence_as_group: dict
    unbounded_sequence_as_group: dict


@dataclass
class XsdAttribute:
    name: str
    type_name: str
    use: str = "optional"
    xml_name: str = ""
    node_id: Optional[NodeId] = field(default=None, compare=False)

    def get_xml_name(self) -> str:
        return self.xml_name or self.name


@dataclass
class XsdEnumType:
    name: str
    values: list
    node_id: Optional[NodeId] = field(default=None, compare=False)


@dataclass
class XsdChildRef:
    """A reference to a child element within a sequence or choice."""
    element_name: str
    min_occurs: int = 1
    max_occurs: int = 1  # -1 = unbounded
    is_group: bool = False
    node_id: Optional[NodeId] = field(default=None, compare=False)


# ---------------------------------------------------------------------------
# Content Tree Nodes (preserves nested choice/sequence structure)
# ---------------------------------------------------------------------------


@dataclass
class ElementRefNode:
    element_name: str
    min_occurs: int = 1
    max_occurs: int = 1  # -1 = unbounded
    node_id: Optional[NodeId] = field(default=None, compare=False)


@dataclass
class GroupRefNode:
    group_name: str
    min_occurs: int = 1
    max_occurs: int = 1
    node_id: Optional[NodeId] = field(default=None, compare=False)


@dataclass
class SequenceNode:
    children: list = field(default_factory=list)
    min_occurs: int = 1
    max_occurs: int = 1
    node_id: Optional[NodeId] = field(default=None, compare=False)


@dataclass
class ChoiceNode:
    branches: list = field(default_factory=list)
    min_occurs: int = 1
    max_occurs: int = 1
    node_id: Optional[NodeId] = field(default=None, compare=False)


@dataclass
class XsdComplexType:
    name: str
    attributes: list = field(default_factory=list)
    children: list = field(default_factory=list)
    has_simple_content: bool = False
    simple_content_base: str = ""
    has_choice: bool = False
    choice_children: list = field(default_factory=list)
    mixed: bool = False
    extension_base: str = ""
    content_tree: Optional[object] = None
    node_id: Optional[NodeId] = field(default=None, compare=False)


@dataclass
class XsdElement:
    name: str
    type_name: str = ""
    anonymous_type: Optional[XsdComplexType] = None
    node_id: Optional[NodeId] = field(default=None, compare=False)


class XsdModel:
    def __init__(self, xsd_path, cfg: ParseConfig):
        self.cfg = cfg
        # The ElementTree is a parse-time detail and is not retained on the model
        # (model.tree is severed). model.root is kept only because four bespoke
        # family handlers in generate.py still read it directly.
        tree = ET.parse(xsd_path)
        self.root = tree.getroot()
        self.enum_types: dict[str, XsdEnumType] = OrderedDict()
        # enum_docs: enum simpleType name -> its annotation/documentation text (or "").
        # Extracted here so generation never re-walks the XML for enum docs.
        self.enum_docs: dict[str, str] = OrderedDict()
        # Named complexTypes whose direct content is xs:complexContent or an xs:group
        # ref. Recorded so the "pattern B" predicate does not re-walk the XML.
        self.complex_content_or_group_cts: set = set()
        self.complex_types: dict[str, XsdComplexType] = OrderedDict()
        self.simple_types: dict[str, dict] = OrderedDict()
        self.attribute_groups: dict[str, list] = OrderedDict()
        self.groups: dict[str, list] = OrderedDict()
        self.elements: dict[str, XsdElement] = OrderedDict()
        self.class_names: set = set()
        self._parse()

    def _parse(self):
        self._parse_attribute_groups()
        self._parse_simple_types()
        self._parse_complex_types()
        self._parse_groups()
        self._inline_non_generated_groups()
        self._parse_elements()
        self._build_class_names()
        self._assign_ids()

    def _parse_attribute_groups(self):
        for ag in self.root.iter(f"{XS}attributeGroup"):
            name = ag.get("name")
            if name:
                self.attribute_groups[name] = self._collect_attrs(ag)

    def _collect_attrs(self, node, seen=None):
        if seen is None:
            seen = set()
        out = []
        for child in node:
            tag = child.tag
            if tag == f"{XS}attribute" and child.get("name"):
                out.append(XsdAttribute(
                    name=child.get("name"),
                    type_name=child.get("type", "xs:string"),
                    use=child.get("use", "optional"),
                ))
            elif tag == f"{XS}attribute" and child.get("ref"):
                ref = child.get("ref")
                local_name = ref.split(":")[-1] if ":" in ref else ref
                out.append(XsdAttribute(
                    name=local_name,
                    type_name=ref,
                    use=child.get("use", "optional"),
                    xml_name=ref,
                ))
            elif tag == f"{XS}attributeGroup" and child.get("ref"):
                ref = child.get("ref")
                if ref not in seen:
                    seen.add(ref)
                    if ref in self.attribute_groups:
                        out.extend(self.attribute_groups[ref])
                    else:
                        deferred = self._collect_attrs_from_ref(ref)
                        out.extend(deferred)
            elif tag in (f"{XS}simpleContent", f"{XS}complexContent",
                         f"{XS}extension", f"{XS}restriction"):
                out.extend(self._collect_attrs(child, seen))
        return out

    def _collect_attrs_from_ref(self, ref):
        for ag in self.root.iter(f"{XS}attributeGroup"):
            if ag.get("name") == ref:
                return self._collect_attrs(ag)
        return []

    @staticmethod
    def _extract_documentation(node) -> str:
        ann = node.find(f"{XS}annotation")
        if ann is not None:
            doc_el = ann.find(f"{XS}documentation")
            if doc_el is not None and doc_el.text:
                return doc_el.text.strip()
        return ""

    def _parse_simple_types(self):
        union_members = set()
        for st in self.root.iter(f"{XS}simpleType"):
            union = st.find(f"{XS}union")
            if union is not None and union.get("memberTypes"):
                for m in union.get("memberTypes").split():
                    union_members.add(m)

        for st in self.root.iter(f"{XS}simpleType"):
            name = st.get("name")
            if not name:
                continue
            restr = st.find(f"{XS}restriction")
            if restr is not None:
                vals = [e.get("value") for e in restr.findall(f"{XS}enumeration")]
                if vals:
                    self.enum_types[name] = XsdEnumType(name=name, values=vals)
                    self.enum_docs[name] = self._extract_documentation(st)
                    self.simple_types[name] = {
                        "kind": "enum",
                        "base": restr.get("base", ""),
                        "values": vals,
                        "is_union_member": name in union_members,
                    }
                else:
                    self.simple_types[name] = {
                        "kind": "restriction",
                        "base": restr.get("base", ""),
                    }
            else:
                union = st.find(f"{XS}union")
                if union is not None:
                    self.simple_types[name] = {
                        "kind": "union",
                        "member_types": union.get("memberTypes", "").split(),
                    }
                else:
                    self.simple_types[name] = {"kind": "other"}

    def _parse_complex_types(self):
        for ct in self.root.iter(f"{XS}complexType"):
            name = ct.get("name")
            if not name:
                continue
            ctype = XsdComplexType(name=name, mixed=ct.get("mixed") == "true")
            ctype.attributes = self._collect_attrs(ct)

            if (ct.find(f"{XS}complexContent") is not None
                    or ct.find(f"{XS}group") is not None):
                self.complex_content_or_group_cts.add(name)

            sc = ct.find(f"{XS}simpleContent")
            if sc is not None:
                ctype.has_simple_content = True
                ext = sc.find(f"{XS}extension")
                if ext is not None:
                    ctype.simple_content_base = ext.get("base", "")
                    ctype.attributes = self._collect_attrs(ext)

            cc = ct.find(f"{XS}complexContent")
            if cc is not None:
                ext = cc.find(f"{XS}extension")
                if ext is not None:
                    ctype.extension_base = ext.get("base", "")
                    ext_seq = ext.find(f"{XS}sequence")
                    if ext_seq is not None:
                        ctype.children = self._parse_children(ext_seq, name)

            if not ctype.children:
                seq = ct.find(f"{XS}sequence")
                if seq is not None:
                    ctype.children = self._parse_children(seq, name)

            if not ctype.children:
                grp = ct.find(f"{XS}group")
                if grp is not None and grp.get("ref"):
                    ctype.children = self._parse_children(ct, name)

            choice = ct.find(f"{XS}choice")
            if choice is not None:
                ctype.has_choice = True
                ctype.choice_children = self._parse_children(choice, name)

            seq_for_tree = ct.find(f"{XS}sequence")
            choice_for_tree = ct.find(f"{XS}choice")
            if seq_for_tree is not None:
                min_o = int(seq_for_tree.get("minOccurs", "1"))
                max_o = seq_for_tree.get("maxOccurs", "1")
                max_o = -1 if max_o == "unbounded" else int(max_o)
                tree_children = self._parse_content_tree(seq_for_tree)
                ctype.content_tree = SequenceNode(
                    children=tree_children, min_occurs=min_o, max_occurs=max_o)
            elif choice_for_tree is not None:
                min_o = int(choice_for_tree.get("minOccurs", "1"))
                max_o = choice_for_tree.get("maxOccurs", "1")
                max_o = -1 if max_o == "unbounded" else int(max_o)
                tree_branches = self._parse_content_tree(choice_for_tree)
                ctype.content_tree = ChoiceNode(
                    branches=tree_branches, min_occurs=min_o, max_occurs=max_o)

            self.complex_types[name] = ctype
        self._resolve_complex_content_extensions()

    def _resolve_complex_content_extensions(self):
        for ctype in self.complex_types.values():
            if not ctype.extension_base:
                continue
            base = self.complex_types.get(ctype.extension_base)
            if not base:
                continue
            ctype.attributes = base.attributes + ctype.attributes
            if not ctype.children and base.children:
                # The original codegen usually flattened inherited children:
                # a type extending ``time-modification`` would expose the
                # inner normal-type / normal-dot directly. For specific
                # extending types (see EXTENSION_OPTIONAL_GROUP_RENAME) the
                # original kept the inherited synthetic optional group as a
                # distinctly-named wrapper sub-element on the extending
                # type. Honor that override here; otherwise flatten as
                # before.
                rename_map = self.cfg.extension_optional_group_rename.get(
                    ctype.name, {})
                inherited = []
                for c in base.children:
                    if (c.is_group
                            and c.element_name in self.cfg.synthetic_optional_groups):
                        if c.element_name in rename_map:
                            new_name = rename_map[c.element_name]
                            if new_name not in self.groups:
                                self.groups[new_name] = list(
                                    self.groups.get(c.element_name, []))
                                self.cfg.generate_groups.add(new_name)
                                self.cfg.synthetic_optional_groups.add(new_name)
                                self.cfg.suppress_group_suffix.add(new_name)
                            inherited.append(XsdChildRef(
                                element_name=new_name,
                                min_occurs=0,
                                max_occurs=1,
                                is_group=True,
                            ))
                            continue
                        for gm in self.groups.get(c.element_name, []):
                            inherited.append(XsdChildRef(
                                element_name=gm.element_name,
                                min_occurs=gm.min_occurs,
                                max_occurs=gm.max_occurs,
                                is_group=gm.is_group,
                            ))
                    else:
                        inherited.append(c)
                ctype.children = inherited + ctype.children

    def _parse_children(self, container, parent_type_name: Optional[str] = None):
        children = []
        for child in container:
            if child.tag == f"{XS}element":
                elem_name = child.get("ref") or child.get("name")
                if elem_name:
                    min_o = int(child.get("minOccurs", "1"))
                    max_o = child.get("maxOccurs", "1")
                    max_o = -1 if max_o == "unbounded" else int(max_o)
                    children.append(XsdChildRef(
                        element_name=elem_name, min_occurs=min_o, max_occurs=max_o
                    ))
            elif child.tag == f"{XS}group":
                ref = child.get("ref")
                if ref:
                    min_o = int(child.get("minOccurs", "1"))
                    max_o = child.get("maxOccurs", "1")
                    max_o = -1 if max_o == "unbounded" else int(max_o)
                    children.append(XsdChildRef(
                        element_name=ref, min_occurs=min_o, max_occurs=max_o,
                        is_group=True,
                    ))
            elif child.tag == f"{XS}sequence":
                seq_min = int(child.get("minOccurs", "1"))
                seq_max = child.get("maxOccurs", "1")
                seq_max = -1 if seq_max == "unbounded" else int(seq_max)
                # An anonymous nested optional sequence (minOccurs=0,
                # maxOccurs=1) inside a parent sequence is the XSD shape that
                # the original code generation sometimes promoted to a
                # synthetic group class (e.g. NormalTypeNormalDotGroup inside
                # time-modification). The naming and the decision to promote
                # were human choices and are not consistent across the
                # schema -- page-layout, for example, flattens the same
                # shape rather than promoting it. Opt-in via
                # NESTED_OPTIONAL_SEQUENCE_AS_GROUP keyed on the enclosing
                # complex-type name.
                if (seq_min == 0 and seq_max == 1
                        and parent_type_name in self.cfg.nested_optional_sequence_as_group):
                    nested_children = self._parse_children(child)
                    if nested_children and all(
                        not c.is_group for c in nested_children
                    ):
                        group_name = self.cfg.nested_optional_sequence_as_group[
                            parent_type_name]
                        group_ref = self._synthesize_optional_group(
                            group_name, nested_children)
                        children.append(group_ref)
                        continue
                if (seq_min == 0 and seq_max == -1
                        and parent_type_name in self.cfg.unbounded_sequence_as_group):
                    nested_children = self._parse_children(child)
                    if nested_children and all(
                        not c.is_group for c in nested_children
                    ):
                        group_name = self.cfg.unbounded_sequence_as_group[
                            parent_type_name]
                        group_ref = self._synthesize_unbounded_group(
                            group_name, nested_children)
                        children.append(group_ref)
                        continue
                children.extend(self._parse_children(child, parent_type_name))
            elif child.tag == f"{XS}choice":
                pass
        return children

    def _synthesize_optional_group(self, group_name: str,
                                   nested_children: list) -> "XsdChildRef":
        # ``group_name`` is the hyphenated-lowercase ref name (e.g.
        # "normal-type-normal-dot") so that pascal(name) + "Group" via
        # child_class_name + GENERATE_GROUPS yields the expected class name
        # (e.g. NormalTypeNormalDotGroup).
        if group_name not in self.groups:
            self.groups[group_name] = list(nested_children)
            self.cfg.generate_groups.add(group_name)
            self.cfg.synthetic_optional_groups.add(group_name)
        return XsdChildRef(
            element_name=group_name,
            min_occurs=0,
            max_occurs=1,
            is_group=True,
        )

    def _synthesize_unbounded_group(self, group_name: str,
                                    nested_children: list) -> "XsdChildRef":
        # Members of an unbounded synthetic group are forced to min_occurs=0,
        # max_occurs=1 because the unbounded multiplicity belongs to the
        # wrapping sequence at the parent. The original codegen produced
        # group classes (e.g. MidiDeviceInstrumentGroup) with all-optional
        # members in this exact shape.
        if group_name not in self.groups:
            members = []
            for c in nested_children:
                members.append(XsdChildRef(
                    element_name=c.element_name,
                    min_occurs=0,
                    max_occurs=1,
                    is_group=c.is_group,
                ))
            self.groups[group_name] = members
            self.cfg.generate_groups.add(group_name)
            self.cfg.synthetic_unbounded_groups.add(group_name)
        return XsdChildRef(
            element_name=group_name,
            min_occurs=0,
            max_occurs=-1,
            is_group=True,
        )

    def _parse_content_tree(self, container):
        nodes = []
        for child in container:
            min_o = int(child.get("minOccurs", "1"))
            max_o = child.get("maxOccurs", "1")
            max_o = -1 if max_o == "unbounded" else int(max_o)
            if child.tag == f"{XS}element":
                name = child.get("ref") or child.get("name")
                if name:
                    nodes.append(ElementRefNode(
                        element_name=name, min_occurs=min_o, max_occurs=max_o))
            elif child.tag == f"{XS}group":
                ref = child.get("ref")
                if ref:
                    nodes.append(GroupRefNode(
                        group_name=ref, min_occurs=min_o, max_occurs=max_o))
            elif child.tag == f"{XS}sequence":
                sub = self._parse_content_tree(child)
                nodes.append(SequenceNode(
                    children=sub, min_occurs=min_o, max_occurs=max_o))
            elif child.tag == f"{XS}choice":
                branches = self._parse_content_tree(child)
                nodes.append(ChoiceNode(
                    branches=branches, min_occurs=min_o, max_occurs=max_o))
        return nodes

    def _parse_groups(self):
        for grp in self.root.iter(f"{XS}group"):
            name = grp.get("name")
            if name:
                seq = grp.find(f"{XS}sequence")
                if seq is not None:
                    self.groups[name] = self._parse_children(seq)
                else:
                    choice = grp.find(f"{XS}choice")
                    if choice is not None:
                        self.groups[name] = self._parse_children(choice)

    def _inline_group_children(self, children: list) -> list:
        result = []
        for child in children:
            if child.is_group and child.element_name not in self.cfg.generate_groups:
                group_children = self.groups.get(child.element_name, [])
                inlined = self._inline_group_children(group_children)
                for gc in inlined:
                    min_o = gc.min_occurs
                    if child.min_occurs == 0:
                        min_o = 0
                    gc_copy = XsdChildRef(
                        element_name=gc.element_name,
                        min_occurs=min_o,
                        max_occurs=gc.max_occurs,
                        is_group=gc.is_group,
                    )
                    result.append(gc_copy)
            else:
                result.append(child)
        return result

    def _inline_non_generated_groups(self):
        for ct in self.complex_types.values():
            if ct.children:
                ct.children = self._inline_group_children(ct.children)

    def _parse_elements(self):
        for el in self.root.iter(f"{XS}element"):
            name = el.get("name")
            if not name:
                continue
            if name in self.elements:
                continue
            typ = el.get("type", "")
            anon = None
            ct = el.find(f"{XS}complexType")
            if ct is not None:
                anon = XsdComplexType(
                    name=f"_anon_{name}", mixed=ct.get("mixed") == "true")
                anon.attributes = self._collect_attrs(ct)
                sc = ct.find(f"{XS}simpleContent")
                if sc is not None:
                    anon.has_simple_content = True
                    ext = sc.find(f"{XS}extension")
                    if ext is not None:
                        anon.simple_content_base = ext.get("base", "")
                seq = ct.find(f"{XS}sequence")
                if seq is not None:
                    anon.children = self._parse_children(seq)
            self.elements[name] = XsdElement(name=name, type_name=typ, anonymous_type=anon)

    def _build_class_names(self):
        for name in self.elements:
            self.class_names.add(pascal(name))
        for name in self.complex_types:
            self.class_names.add(pascal(name))
        self._build_type_usage_counts()

    def _build_type_usage_counts(self):
        self.type_usage_count = {}
        for elem in self.elements.values():
            if elem.type_name:
                self.type_usage_count[elem.type_name] = \
                    self.type_usage_count.get(elem.type_name, 0) + 1

    # -- NodeId assignment ---------------------------------------------------
    # Total coverage: every dataclass-backed node reachable from a top-level
    # construct receives a NodeId. Additive and unconsumed -- no generation
    # logic reads these yet. (simple_types, groups, and attribute_groups are
    # stored as plain dicts/lists rather than node objects, so the construct
    # itself has no field to carry an ID; its member nodes are still keyed
    # under the construct's root ID.)

    def _assign_ids(self):
        for name, enum in self.enum_types.items():
            enum.node_id = NodeId.root("st", name)
        for name, attrs in self.attribute_groups.items():
            self._assign_attr_ids(NodeId.root("ag", name), attrs)
        for name, children in self.groups.items():
            self._assign_childref_ids(NodeId.root("gr", name), children)
        for name, ctype in self.complex_types.items():
            ctype.node_id = NodeId.root("cx", name)
            self._assign_complex_type_ids(ctype.node_id, ctype)
        for name, elem in self.elements.items():
            elem.node_id = NodeId.root("el", name)
            if elem.anonymous_type is not None:
                anon_id = elem.node_id.anon_child("cx", 0)
                elem.anonymous_type.node_id = anon_id
                self._assign_complex_type_ids(anon_id, elem.anonymous_type)

    def _assign_complex_type_ids(self, base: NodeId, ctype: XsdComplexType):
        self._assign_attr_ids(base, ctype.attributes)
        self._assign_childref_ids(base, ctype.children)
        self._assign_childref_ids(base, ctype.choice_children)
        if ctype.content_tree is not None:
            self._assign_tree_ids(base, [ctype.content_tree], {})

    @staticmethod
    def _assign_attr_ids(base: NodeId, attrs: list):
        for attr in attrs:
            attr.node_id = base.named_child("at", attr.name)

    @staticmethod
    def _assign_childref_ids(base: NodeId, children: list):
        for child in children:
            kind = "gr" if child.is_group else "el"
            child.node_id = base.named_child(kind, child.element_name)

    def _assign_tree_ids(self, base: NodeId, nodes: list, counters: dict):
        for node in nodes:
            if isinstance(node, ElementRefNode):
                node.node_id = base.named_child("el", node.element_name)
            elif isinstance(node, GroupRefNode):
                node.node_id = base.named_child("gr", node.group_name)
            elif isinstance(node, SequenceNode):
                ordinal = counters.get("seq", 0)
                counters["seq"] = ordinal + 1
                node.node_id = base.anon_child("seq", ordinal)
                self._assign_tree_ids(node.node_id, node.children, {})
            elif isinstance(node, ChoiceNode):
                ordinal = counters.get("choice", 0)
                counters["choice"] = ordinal + 1
                node.node_id = base.anon_child("choice", ordinal)
                self._assign_tree_ids(node.node_id, node.branches, {})
