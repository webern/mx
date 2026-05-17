#!/usr/bin/env python3
"""Iteration 3: derive *Attributes struct field lists by flattening each complexType's own
attributes + transitive attributeGroup refs, in document order, and diff field names/order
against the real *Attributes.h headers."""
import os
import re
import xml.etree.ElementTree as ET

XS = "{http://www.w3.org/2001/XMLSchema}"
ELDIR = "src/private/mx/core/elements"


def pascal(n):
    return "".join(p[:1].upper() + p[1:] for p in re.split(r"[-_]", n))


def camel(n):
    p = re.split(r"[-_]", n)
    return p[0].lower() + "".join(x[:1].upper() + x[1:] for x in p[1:])


def main():
    root = ET.parse("docs/musicxml.xsd").getroot()

    attr_groups = {}  # name -> element (xs:attributeGroup definition)
    for ag in root.iter(f"{XS}attributeGroup"):
        if ag.get("name"):
            attr_groups[ag.get("name")] = ag

    def collect(node, seen):
        """Ordered list of attribute names from node's own xs:attribute + nested
        xs:attributeGroup refs (transitive, document order)."""
        out = []
        for child in node:
            tag = child.tag
            if tag == f"{XS}attribute" and child.get("name"):
                out.append(child.get("name"))
            elif tag == f"{XS}attributeGroup" and child.get("ref"):
                ref = child.get("ref")
                if ref in seen:
                    continue
                seen.add(ref)
                if ref in attr_groups:
                    out += collect(attr_groups[ref], seen)
            elif tag in (f"{XS}simpleContent", f"{XS}complexContent",
                         f"{XS}extension", f"{XS}restriction"):
                out += collect(child, seen)
        return out

    # complexType name -> ordered attribute names
    derived = {}
    for ct in root.iter(f"{XS}complexType"):
        if not ct.get("name"):
            continue
        attrs = collect(ct, set())
        if attrs:
            derived[pascal(ct.get("name")) + "Attributes"] = [camel(a) for a in attrs]

    def real_fields(path):
        fields, instruct = [], False
        for line in open(path):
            s = line.strip()
            if s.startswith("struct ") and "Attributes" in s:
                instruct = True
                continue
            if instruct:
                if s.startswith("private:") or s == "};":
                    break
                m = re.match(r"[\w:]+(?:<[^>]+>)?\s+(\w+);$", s)
                if m and not m.group(1).startswith("has"):
                    fields.append(m.group(1))
        return fields

    total = exact = name_only = mismatch = 0
    missing = []
    mism_ex = []
    for h in sorted(os.listdir(ELDIR)):
        if not h.endswith("Attributes.h"):
            continue
        cls = h[:-2]
        if cls not in derived:
            missing.append(cls)
            continue
        total += 1
        rf = real_fields(os.path.join(ELDIR, h))
        df = derived[cls]
        if df == rf:
            exact += 1
        elif sorted(df) == sorted(rf):
            name_only += 1  # same set, different order
            mism_ex.append(f"{cls} ORDER d={df[:5]} r={rf[:5]}")
        else:
            mismatch += 1
            mism_ex.append(f"{cls} SET d-only={sorted(set(df)-set(rf))[:4]} "
                           f"r-only={sorted(set(rf)-set(df))[:4]}")

    print(f"*Attributes headers in elements/      : "
          f"{len([h for h in os.listdir(ELDIR) if h.endswith('Attributes.h')])}")
    print(f"matched a derived complexType         : {total}")
    print(f"  exact (fields + order)              : {exact}")
    print(f"  same fields, different order        : {name_only}")
    print(f"  field-set mismatch                  : {mismatch}")
    print(f"no derived complexType (name unknown) : {len(missing)}")
    print()
    print("--- headers with no matching complexType (struct-name rule gap) ---")
    print("  " + ", ".join(missing[:30]))
    print()
    print("--- field mismatches (first 20) ---")
    for e in mism_ex[:20]:
        print("  " + e)


main()
