#!/usr/bin/env python3
"""Iteration 2: derive enum MEMBER identifiers for every enumeration simpleType and diff
against the real Enums.h bodies. Surfaces the member-naming rule, the reserved-word rule,
and any value that is not algorithmically derivable (digit-leading values, etc.)."""
import re
import xml.etree.ElementTree as ET

XS = "{http://www.w3.org/2001/XMLSchema}"

CPP_KEYWORDS = {
    "continue", "double", "long", "short", "int", "float", "bool", "char", "class",
    "struct", "enum", "union", "void", "for", "while", "do", "if", "else", "switch",
    "case", "default", "break", "return", "new", "delete", "this", "true", "false",
    "const", "static", "virtual", "public", "private", "protected", "namespace",
    "using", "template", "typename", "operator", "and", "or", "not", "xor", "auto",
    "register", "signed", "unsigned", "goto", "throw", "try", "catch",
}


def camel(value: str) -> str:
    parts = re.split(r"[-_ ]", value)
    out = parts[0].lower() + "".join(p[:1].upper() + p[1:] for p in parts[1:])
    return out + "_" if out in CPP_KEYWORDS else out


def derivable(value: str) -> bool:
    # A value is rule-derivable iff it does not begin with a digit (digit-leading values
    # were hand-spelled to English in mx/core: 1024th -> oneThousandTwentyFourth).
    return not value[:1].isdigit()


def real_bodies(path):
    bodies, cur = {}, None
    for line in open(path):
        m = re.match(r"\s*enum class (\w+)", line)
        if m:
            cur, bodies[cur] = m.group(1), []
            continue
        if cur is not None:
            if line.strip() == "};":
                cur = None
                continue
            mv = re.match(r"\s*(\w+)\s*=\s*\d+", line)
            if mv:
                bodies[cur].append(mv.group(1))
    return bodies


def main():
    root = ET.parse("docs/musicxml.xsd").getroot()
    class_names = set()
    for tag in ("element", "complexType"):
        for e in root.iter(f"{XS}{tag}"):
            if e.get("name"):
                class_names.add("".join(p[:1].upper() + p[1:]
                                        for p in re.split(r"[-_]", e.get("name"))))
    real = real_bodies("src/private/mx/core/Enums.h")

    total = match = nonderiv_types = 0
    nonderiv_examples = []
    mismatch_examples = []
    for st in root.iter(f"{XS}simpleType"):
        name = st.get("name")
        if not name:
            continue
        restr = st.find(f"{XS}restriction")
        if restr is None:
            continue
        vals = [e.get("value") for e in restr.findall(f"{XS}enumeration")]
        if not vals:
            continue
        base = "".join(p[:1].upper() + p[1:] for p in re.split(r"[-_]", name))
        cpp = base + "Enum" if base in class_names else base
        if cpp not in real:
            continue
        total += 1
        derived_members = [camel(v) for v in vals]
        if not all(derivable(v) for v in vals):
            nonderiv_types += 1
            bad = [v for v in vals if not derivable(v)]
            nonderiv_examples.append(f"{name}: {bad[:6]}")
            continue
        if derived_members == real[cpp]:
            match += 1
        else:
            d = [(a, b) for a, b in zip(derived_members, real[cpp]) if a != b]
            mismatch_examples.append(f"{cpp}: {d[:4]} (len d{len(derived_members)} r{len(real[cpp])})")

    print(f"enums compared (name-matched)        : {total}")
    print(f"fully RULE-derivable members, exact  : {match}")
    print(f"types with digit-leading values (EXC): {nonderiv_types}")
    print(f"other member mismatches              : {len(mismatch_examples)}")
    print()
    print("--- digit-leading (hand-spelled English, not derivable) ---")
    for e in nonderiv_examples:
        print(f"  {e}")
    print()
    print("--- other mismatches (rule gaps) ---")
    for e in mismatch_examples[:25]:
        print(f"  {e}")


main()
