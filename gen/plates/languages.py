"""Per-language defaults the Plates build seeds before config overrides.

Everything here is a default, not a decision: `[types]` entries override the
type map, `[reserved] words` extends the reserved set, `[docs]` overrides the
doc style. An unknown language starts from the neutral defaults (empty type
map, no reserved words), which is what a schema-style target wants.
"""

from __future__ import annotations

from gen.plates.model import DocStyle

# IR primitive -> target type. The single place a target decides what a
# `decimal` or a `token` is spelled like; overridable per-primitive in
# [types]. The IR primitive set is fixed by gen.ir.build.
TYPE_MAPS: dict[str, dict[str, str]] = {
    "go": {
        "string": "string",
        "token": "string",
        "nmtoken": "string",
        "date": "string",
        "decimal": "float64",
        "integer": "int",
        "positive_integer": "int",
        "non_negative_integer": "int",
    },
    "c": {
        "string": "char *",
        "token": "char *",
        "nmtoken": "char *",
        "date": "char *",
        "decimal": "double",
        "integer": "long",
        "positive_integer": "long",
        "non_negative_integer": "long",
    },
    "cpp": {
        "string": "std::string",
        "token": "std::string",
        "nmtoken": "std::string",
        "date": "std::string",
        "decimal": "double",
        "integer": "int",
        "positive_integer": "int",
        "non_negative_integer": "int",
    },
}

# How enum constants are scoped, per language: `bare` when the language
# scopes them inside the type (C++ enum class), `composed` when they share
# one flat namespace and carry the type's name (Go package-level constants,
# C's single global namespace). This is a language fact, not configuration;
# the composition itself happens in the projection so Variant.ident is final.
VARIANT_SCOPES: dict[str, str] = {
    "go": "composed",
    "c": "composed",
    "cpp": "bare",
}

# Identifiers the sanitizer must not emit bare, per language: keywords plus
# predeclared/builtin names a generated identifier could shadow disastrously.
# [reserved] words extends these.
RESERVED: dict[str, tuple[str, ...]] = {
    "go": (
        # keywords
        "break", "case", "chan", "const", "continue", "default", "defer",
        "else", "fallthrough", "for", "func", "go", "goto", "if", "import",
        "interface", "map", "package", "range", "return", "select", "struct",
        "switch", "type", "var",
        # predeclared identifiers
        "any", "append", "bool", "byte", "cap", "clear", "close", "comparable",
        "complex", "complex64", "complex128", "copy", "delete", "error",
        "false", "float32", "float64", "imag", "int", "int8", "int16", "int32",
        "int64", "iota", "len", "make", "max", "min", "new", "nil", "panic",
        "print", "println", "real", "recover", "rune", "string", "true",
        "uint", "uint8", "uint16", "uint32", "uint64", "uintptr",
    ),
    "c": (
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "inline", "int", "long", "register", "restrict", "return", "short",
        "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while",
        "bool", "true", "false",  # <stdbool.h> is assumed by generated code
    ),
    "cpp": (
        "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand",
        "bitor", "bool", "break", "case", "catch", "char", "char8_t",
        "char16_t", "char32_t", "class", "compl", "concept", "const",
        "consteval", "constexpr", "constinit", "const_cast", "continue",
        "co_await", "co_return", "co_yield", "decltype", "default", "delete",
        "do", "double", "dynamic_cast", "else", "enum", "explicit", "export",
        "extern", "false", "float", "for", "friend", "goto", "if", "inline",
        "int", "long", "mutable", "namespace", "new", "noexcept", "not",
        "not_eq", "nullptr", "operator", "or", "or_eq", "private",
        "protected", "public", "register", "reinterpret_cast", "requires",
        "return", "short", "signed", "sizeof", "static", "static_assert",
        "static_cast", "struct", "switch", "template", "this", "thread_local",
        "throw", "true", "try", "typedef", "typeid", "typename", "union",
        "unsigned", "using", "virtual", "void", "volatile", "wchar_t",
        "while", "xor", "xor_eq",
    ),
}

DOC_STYLES: dict[str, DocStyle] = {
    "go": DocStyle(style="//"),
    "c": DocStyle(style="/* */"),
    "cpp": DocStyle(style="///"),
}


def type_map_for(language: str) -> dict[str, str]:
    return dict(TYPE_MAPS.get(language, {}))


def reserved_for(language: str) -> tuple[str, ...]:
    return RESERVED.get(language, ())


def doc_style_for(language: str) -> DocStyle:
    style = DOC_STYLES.get(language)
    return DocStyle(style=style.style, wrap=style.wrap) if style else DocStyle(style="")


def variant_scope_for(language: str) -> str:
    return VARIANT_SCOPES.get(language, "bare")
