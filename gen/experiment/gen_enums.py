#!/usr/bin/env python3
"""Experimental codegen probe: derive mx/core Enums.h enum-class names from the XSD.

Goal is NOT a finished generator. It applies the hypothesized derivation rules to every
enumeration simpleType, then diffs the derived set against the real Enums.h. Every mismatch
is a discovered exception for docs/ai/project/gen-overrides.md.
"""
import re
import sys
import xml.etree.ElementTree as ET

XS = "{http://www.w3.org/2001/XMLSchema}"
XSD = "docs/musicxml.xsd"
ENUMS_H = "src/private/mx/core/Enums.h"


def pascal(name: str) -> str:
    return "".join(p[:1].upper() + p[1:] for p in re.split(r"[-_]", name))


def main() -> None:
    tree = ET.parse(XSD)
    root = tree.getroot()

    # Set of class names that an enum name could collide with: every element name and
    # every complexType name, PascalCased. (Hypothesis: collision -> 'Enum' suffix.)
    class_names = set()
    for el in root.iter(f"{XS}element"):
        if el.get("name"):
            class_names.add(pascal(el.get("name")))
    for ct in root.iter(f"{XS}complexType"):
        if ct.get("name"):
            class_names.add(pascal(ct.get("name")))

    # Every simpleType whose restriction carries xs:enumeration values is an enum candidate.
    enum_types = {}  # xsd type name -> [values]
    union_members = set()  # simpleTypes referenced inside an xs:union (open-extension hint)
    for st in root.iter(f"{XS}simpleType"):
        name = st.get("name")
        union = st.find(f"{XS}union")
        if union is not None and union.get("memberTypes"):
            for m in union.get("memberTypes").split():
                union_members.add(m)
        if not name:
            continue
        restr = st.find(f"{XS}restriction")
        if restr is None:
            continue
        vals = [e.get("value") for e in restr.findall(f"{XS}enumeration")]
        if vals:
            enum_types[name] = vals

    derived = {}  # derived C++ enum-class name -> xsd type name
    for xsd_name in enum_types:
        base = pascal(xsd_name)
        if base in class_names:
            cpp = base + "Enum"  # collision rule
        else:
            cpp = base
        derived[cpp] = xsd_name

    # Real enum-class names from Enums.h.
    real = set()
    for line in open(ENUMS_H):
        m = re.match(r"\s*enum class (\w+)", line)
        if m:
            real.add(m.group(1))

    dset = set(derived)
    only_real = sorted(real - dset)  # in mx/core, generator missed -> exceptions
    only_derived = sorted(dset - real)  # generator invented -> over-generation

    print(f"XSD enumeration simpleTypes : {len(enum_types)}")
    print(f"derived enum-class names    : {len(dset)}")
    print(f"real enum-class names       : {len(real)}")
    print(f"exact-match                 : {len(dset & real)}")
    print()
    print(f"--- in Enums.h but NOT derived ({len(only_real)}) [missed / special] ---")
    for n in only_real:
        print(f"  {n}")
    print()
    print(f"--- derived but NOT in Enums.h ({len(only_derived)}) [over-generated] ---")
    for n in only_derived:
        print(f"  {n}  (xsd: {derived[n]})")


if __name__ == "__main__":
    sys.exit(main())
