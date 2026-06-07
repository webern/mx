#!/usr/bin/env python3
"""C++ naming and casing utilities for the code generator.

Owns the set of C++ reserved keywords and the functions that transform
XSD/hyphenated names into legal C++ identifiers (camelCase, hasFlag, etc.).
"""
import re

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
    base = cpp_n[:-1] if cpp_n.endswith("_") and cpp_n[:-1] in CPP_KEYWORDS else cpp_n
    return "has" + base[0].upper() + base[1:]


def pascal_to_camel(pascal_name: str) -> str:
    if not pascal_name:
        return pascal_name
    return pascal_name[0].lower() + pascal_name[1:]
