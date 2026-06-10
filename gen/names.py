"""Name expansion: tokenize wire names, recase per convention, sanitize.

A fundamental (wire) name is split into an ordered word vector of lowercase
words, then recased by each registered convention. The wire form is preserved
untouched alongside the casings -- tokenization feeds only the cased
identifiers, never serialization (design R3).

Conventions live in a registry keyed by name, so adding one later is
registering one function; every Name simply grows a key (design R1).

This module is deliberately a leaf: it is shared vocabulary for the config
loader (which validates convention names and rename-entry keys) and for the
Plates projection, so it sits below both and imports neither.
"""

from __future__ import annotations

from dataclasses import dataclass

# Word separators, split on and consumed. Hyphen covers ordinary kebab names;
# dot covers sound ids like `brass.alphorn`; whitespace covers space-separated
# enum values like `up down`; colon covers external refs like `xml:lang`.
_SEPARATORS = set("-._: \t\n\r\v\f")

# Words uppercased whole by capitalizing conventions (Pascal, non-leading
# camel). Config-extensible via [naming] acronyms.
DEFAULT_ACRONYMS = ("midi", "id", "xml", "css", "smufl", "uri", "url")

# Fallback word vector for wire names that tokenize to nothing (the empty enum
# value of positive-integer-or-empty and a few *-value enums). The wire form
# stays ""; only the identifier gets a name. A target wanting a different word
# for a particular enum renames it: [rename.enum-value.<enum>] "" = "none".
EMPTY_WORD = "empty"


@dataclass
class Name:
    """The neutral/bound name bundle. `wire` is the immutable on-the-wire
    string (never a code identifier); `words` is the tokenized vector the
    casings expand from; `cased` maps convention name -> identifier, filled
    by iterating the convention registry."""

    wire: str
    words: tuple[str, ...]
    cased: dict[str, str]

    @property
    def pascal(self) -> str:
        return self.cased["pascal"]

    @property
    def camel(self) -> str:
        return self.cased["camel"]

    @property
    def snake(self) -> str:
        return self.cased["snake"]

    @property
    def kebab(self) -> str:
        return self.cased["kebab"]

    @property
    def screaming(self) -> str:
        return self.cased["screaming"]


def tokenize(wire: str, empty_word: str = EMPTY_WORD) -> tuple[str, ...]:
    """Split a wire name into its canonical lowercase word vector."""
    tokens: list[str] = []
    current: list[str] = []
    for ch in wire:
        if ch in _SEPARATORS:
            if current:
                tokens.append("".join(current))
                current = []
        else:
            current.append(ch)
    if current:
        tokens.append("".join(current))

    words: list[str] = []
    for token in tokens:
        words.extend(w.lower() for w in _split_case_transitions(token))
    return tuple(words) if words else (empty_word,)


def _split_case_transitions(token: str) -> list[str]:
    """Split an already-mixed-case token at a lower-to-upper boundary
    (`fooBar` -> foo, Bar) and at an acronym boundary, where an uppercase run
    is followed by uppercase+lowercase (`MIDIChannel` -> MIDI, Channel): the
    last capital of the run begins the next word. Letter-digit boundaries do
    not split; digits ride with their adjacent letters."""
    if not token:
        return []
    starts = [0]
    for i in range(1, len(token)):
        prev, cur = token[i - 1], token[i]
        if cur.isupper() and prev.islower():
            starts.append(i)
        elif (
            cur.isupper()
            and prev.isupper()
            and i + 1 < len(token)
            and token[i + 1].islower()
        ):
            starts.append(i)
    return [token[a:b] for a, b in zip(starts, starts[1:] + [len(token)])]


def _capitalize(word: str, acronyms: frozenset[str]) -> str:
    if word in acronyms:
        return word.upper()
    if word and word[0].isalpha():
        return word[0].upper() + word[1:]
    return word  # digit-led words like `1024th` stay lowercase


# Each convention maps (word vector, acronym set) -> identifier string. The
# camelCase leading word is always fully lowercased, so a leading acronym
# yields `midiChannel`, never `MIDIChannel`. snake/kebab/screaming are
# case-uniform and ignore the acronym set.
CONVENTIONS = {
    "pascal": lambda ws, ac: "".join(_capitalize(w, ac) for w in ws),
    "camel": lambda ws, ac: ws[0] + "".join(_capitalize(w, ac) for w in ws[1:]),
    "snake": lambda ws, ac: "_".join(ws),
    "kebab": lambda ws, ac: "-".join(ws),
    "screaming": lambda ws, ac: "_".join(w.upper() for w in ws),
}

# How a convention joins two already-cased parts when an identifier is
# composed from a scope plus a member (a type name plus a variant name, for
# targets whose enum constants share one namespace). Concatenating
# conventions join with nothing; delimited conventions reuse their delimiter.
JOINERS = {
    "pascal": "",
    "camel": "",
    "snake": "_",
    "kebab": "-",
    "screaming": "_",
}


def sanitize_identifier(ident: str, reserved: frozenset[str], invalid_prefix: str = "_") -> str:
    """Make a recased identifier legal for a code target: non-identifier
    characters become underscores, a leading digit or empty result gets the
    configured prefix, and reserved words get a trailing underscore. The
    pre-sanitized casing stays available on the Name; collision detection
    runs on the sanitized result."""
    out = "".join(ch if ch.isalnum() or ch == "_" else "_" for ch in ident)
    if not out or out[0].isdigit():
        out = invalid_prefix + out
    if out in reserved:
        out += "_"
    return out


class NameFactory:
    """Builds Name bundles: tokenize once, expand every registered convention,
    honoring a fundamental rename (re-expands all casings from the new root)
    and per-convention overrides (pin one flavor, leave the rest expanded)."""

    def __init__(self, acronyms=DEFAULT_ACRONYMS):
        # The acronym set matches against already-lowercased words, so it is
        # normalized here: acronyms = ["MIDI"] must behave like ["midi"].
        self.acronyms = frozenset(a.lower() for a in acronyms)

    def make(
        self,
        wire: str,
        fundamental: str | None = None,
        overrides: dict[str, str] | None = None,
        pluralize: bool = False,
    ) -> Name:
        words = tokenize(fundamental if fundamental is not None else wire)
        if pluralize:
            words = words[:-1] + (words[-1] + "s",)
        cased = {conv: fn(words, self.acronyms) for conv, fn in CONVENTIONS.items()}
        if overrides:
            for conv, value in overrides.items():
                cased[conv] = value
        return Name(wire=wire, words=words, cased=cased)
