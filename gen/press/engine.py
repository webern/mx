"""The press: a Mustache template engine.

The press renders the targets' templates. The template language is Mustache
-- the published spec's interpolation, sections, inverted sections, partials,
comments, and set-delimiter core, with spec whitespace semantics (standalone
lines, partial call-site indentation) -- and three deliberate deviations,
because code generation is not HTML (design: generator-agnosticism.md):

  1. Missing keys are render errors (template:line in the message). The spec
     mandates silent empty output, which is the worst failure mode a code
     generator can have. A key that is PRESENT with a None/empty value
     renders empty and is falsey in sections; only absence is an error.
  2. No HTML escaping: `{{x}}` interpolates verbatim ({{{x}}} and {{&x}} are
     accepted synonyms).
  3. No lambdas (the spec's escape hatch into logic). A callable in the
     context is an error.

Conformance to everything else is tested against the vendored official spec
suite (gen/tests/mustache_spec/); the constructor's `strict` and `escape`
parameters exist so that suite can exercise the spec's own semantics -- the
production pipeline never passes them.

What the engine will never grow: expressions, comparisons, arithmetic,
filters, string manipulation, casing, assignment, or new syntax. Dispatch
data (booleans, loop metadata, quoted literals) is the context builder's
job; if a template cannot express something, the plates must carry it.
"""

from __future__ import annotations

from collections.abc import Callable, Mapping
from dataclasses import dataclass, field


class PressError(Exception):
    """A template problem, always reported as `template:line: message`."""

    def __init__(self, name: str, line: int, message: str):
        self.template = name
        self.line = line
        super().__init__(f"{name}:{line}: {message}")


# --------------------------------------------------------------------------- #
# Parse tree
# --------------------------------------------------------------------------- #


@dataclass
class _Text:
    text: str


@dataclass
class _Var:
    path: tuple[str, ...]
    raw: bool  # {{{x}}} / {{&x}}: spec semantics; identical here by default
    line: int


@dataclass
class _Section:
    path: tuple[str, ...]
    inverted: bool
    line: int
    children: list = field(default_factory=list)


@dataclass
class _Partial:
    name: str
    indent: str
    line: int


# --------------------------------------------------------------------------- #
# Tokenizer (with set-delimiter support and the spec's standalone-line rules)
# --------------------------------------------------------------------------- #

_STANDALONE_KINDS = {"open", "inv", "close", "comment", "delim", "partial"}


def _tokenize(template: str, name: str) -> list:
    """Produce ('text', str) and ('tag', kind, key, line, indent) tokens.
    kind: var | raw | open | inv | close | partial | comment | delim."""
    tokens: list = []
    odelim, cdelim = "{{", "}}"
    pos = 0
    line = 1
    while pos < len(template):
        start = template.find(odelim, pos)
        if start < 0:
            tokens.append(("text", template[pos:]))
            break
        if start > pos:
            text = template[pos:start]
            tokens.append(("text", text))
            line += text.count("\n")

        # Triple mustache is only meaningful with the default delimiters.
        if odelim == "{{" and template.startswith("{{{", start):
            end = template.find("}}}", start + 3)
            if end < 0:
                raise PressError(name, line, "unclosed '{{{' tag")
            key = template[start + 3 : end].strip()
            tokens.append(("tag", "raw", key, line, ""))
            pos = end + 3
            continue

        end = template.find(cdelim, start + len(odelim))
        if end < 0:
            raise PressError(name, line, f"unclosed '{odelim}' tag")
        content = template[start + len(odelim) : end]
        pos = end + len(cdelim)
        line += content.count("\n")

        sigil = content[:1]
        if sigil == "#":
            tokens.append(("tag", "open", content[1:].strip(), line, ""))
        elif sigil == "^":
            tokens.append(("tag", "inv", content[1:].strip(), line, ""))
        elif sigil == "/":
            tokens.append(("tag", "close", content[1:].strip(), line, ""))
        elif sigil == ">":
            tokens.append(("tag", "partial", content[1:].strip(), line, ""))
        elif sigil == "!":
            tokens.append(("tag", "comment", "", line, ""))
        elif sigil == "&":
            tokens.append(("tag", "raw", content[1:].strip(), line, ""))
        elif sigil == "=":
            inner = content[1:].rstrip()
            if not inner.endswith("="):
                raise PressError(name, line, "malformed set-delimiter tag")
            parts = inner[:-1].split()
            if len(parts) != 2:
                raise PressError(name, line, "malformed set-delimiter tag")
            tokens.append(("tag", "delim", "", line, ""))
            odelim, cdelim = parts
        else:
            tokens.append(("tag", "var", content.strip(), line, ""))
    return _strip_standalone(tokens)


def _strip_standalone(tokens: list) -> list:
    """The spec's standalone-line rule: a line whose text is all whitespace
    and which carries exactly one section/inverted/close/comment/partial/
    set-delimiter tag contributes no output for the line itself. A standalone
    partial keeps the line's leading whitespace as the indentation applied to
    its rendered content."""
    # Split text tokens so each line of the template is its own token run.
    split: list = []
    for tok in tokens:
        if tok[0] != "text":
            split.append(tok)
            continue
        text = tok[1]
        while True:
            nl = text.find("\n")
            if nl < 0:
                if text:
                    split.append(("text", text))
                break
            split.append(("text", text[: nl + 1]))
            text = text[nl + 1 :]

    out: list = []
    line: list = []

    def flush(line_tokens: list) -> None:
        tags = [t for t in line_tokens if t[0] == "tag"]
        texts = [t[1] for t in line_tokens if t[0] == "text"]
        standalone = (
            len(tags) == 1
            and tags[0][1] in _STANDALONE_KINDS
            and all(not t.strip() for t in texts)
        )
        if not standalone:
            out.extend(line_tokens)
            return
        tag = tags[0]
        if tag[1] == "partial":
            indent = ""
            for t in line_tokens:
                if t[0] == "tag":
                    break
                indent += t[1]
            tag = ("tag", "partial", tag[2], tag[3], indent)
        out.append(tag)

    for tok in split:
        line.append(tok)
        if tok[0] == "text" and tok[1].endswith("\n"):
            flush(line)
            line = []
    if line:
        flush(line)
    return out


# --------------------------------------------------------------------------- #
# Parser
# --------------------------------------------------------------------------- #


def _path(key: str) -> tuple[str, ...]:
    return (".",) if key == "." else tuple(key.split("."))


def _parse(tokens: list, name: str) -> list:
    root: list = []
    stack: list[tuple[_Section, str]] = []
    current = root
    for tok in tokens:
        if tok[0] == "text":
            if tok[1]:
                current.append(_Text(tok[1]))
            continue
        _, kind, key, line, indent = tok
        if kind in ("var", "raw"):
            current.append(_Var(_path(key), kind == "raw", line))
        elif kind in ("open", "inv"):
            section = _Section(_path(key), kind == "inv", line)
            current.append(section)
            stack.append((section, key))
            current = section.children
        elif kind == "close":
            if not stack or stack[-1][1] != key:
                raise PressError(name, line, f"unexpected section close '{key}'")
            stack.pop()
            current = stack[-1][0].children if stack else root
        elif kind == "partial":
            current.append(_Partial(key, indent, line))
        # comments and delim changes contribute nothing
    if stack:
        section, key = stack[-1]
        raise PressError(name, section.line, f"unclosed section '{key}'")
    return root


# --------------------------------------------------------------------------- #
# Renderer
# --------------------------------------------------------------------------- #

_MISS = object()


class Press:
    """Renders Mustache templates. `partials` maps a partial name to its
    template text (a dict, or a callable for file-backed loading). The
    `strict` and `escape` knobs exist for the spec conformance suite; the
    production pipeline uses the defaults (strict, verbatim)."""

    def __init__(
        self,
        partials: Mapping[str, str] | Callable[[str], str] | None = None,
        strict: bool = True,
        escape: Callable[[str], str] | None = None,
        max_partial_depth: int = 64,
    ):
        self._partials = partials or {}
        self._strict = strict
        self._escape = escape
        self._max_depth = max_partial_depth
        self._cache: dict[tuple[str, str, str], list] = {}

    def render(self, template: str, context, name: str = "<template>") -> str:
        nodes = self._parsed(template, name, "")
        out: list[str] = []
        self._render_nodes(nodes, [context], out, name, 0)
        return "".join(out)

    # ----- parsing --------------------------------------------------------- #

    def _parsed(self, template: str, name: str, indent: str) -> list:
        key = (template, name, indent)
        cached = self._cache.get(key)
        if cached is None:
            text = template
            if indent:
                # Spec: a standalone partial's indentation is applied to
                # every line of the partial's text before rendering.
                lines = text.split("\n")
                last = lines.pop() if lines and lines[-1] == "" else None
                text = "\n".join(indent + line if line else line for line in lines)
                if last is not None:
                    text += "\n"
            cached = _parse(_tokenize(text, name), name)
            self._cache[key] = cached
        return cached

    def _partial_text(self, name: str, line: int, template: str) -> str:
        if callable(self._partials):
            try:
                return self._partials(name)
            except KeyError:
                raise PressError(template, line, f"unknown partial '{name}'") from None
        if name in self._partials:
            return self._partials[name]
        if self._strict:
            raise PressError(template, line, f"unknown partial '{name}'")
        return ""  # spec: missing partials render as empty

    # ----- rendering ------------------------------------------------------- #

    def _render_nodes(self, nodes, stack, out, name, depth) -> None:
        for node in nodes:
            if isinstance(node, _Text):
                out.append(node.text)
            elif isinstance(node, _Var):
                out.append(self._interpolate(node, stack, name))
            elif isinstance(node, _Section):
                self._render_section(node, stack, out, name, depth)
            else:  # _Partial
                if depth >= self._max_depth:
                    raise PressError(
                        name, node.line,
                        f"partial recursion deeper than {self._max_depth} "
                        f"(at '{node.name}')",
                    )
                text = self._partial_text(node.name, node.line, name)
                parsed = self._parsed(text, node.name, node.indent)
                self._render_nodes(parsed, stack, out, node.name, depth + 1)

    def _interpolate(self, node: _Var, stack, name) -> str:
        value = self._resolve(node.path, stack)
        if value is _MISS:
            if self._strict:
                raise PressError(
                    name, node.line, f"missing key '{'.'.join(node.path)}'"
                )
            return ""
        if value is None:
            return ""
        if callable(value):
            raise PressError(
                name, node.line,
                f"'{'.'.join(node.path)}' is callable; the press has no lambdas",
            )
        if value is True or value is False:
            text = "true" if value else "false"
        else:
            text = str(value)
        if self._escape is not None and not node.raw:
            text = self._escape(text)
        return text

    def _render_section(self, node: _Section, stack, out, name, depth) -> None:
        value = self._resolve(node.path, stack)
        if value is _MISS:
            if self._strict:
                raise PressError(
                    name, node.line, f"missing key '{'.'.join(node.path)}'"
                )
            value = None
        if callable(value):
            raise PressError(
                name, node.line,
                f"'{'.'.join(node.path)}' is callable; the press has no lambdas",
            )
        if node.inverted:
            if not value:
                self._render_nodes(node.children, stack, out, name, depth)
            return
        if not value:
            return
        if isinstance(value, (list, tuple)):
            for item in value:
                stack.append(item)
                self._render_nodes(node.children, stack, out, name, depth)
                stack.pop()
        else:
            stack.append(value)
            self._render_nodes(node.children, stack, out, name, depth)
            stack.pop()

    def _resolve(self, path: tuple[str, ...], stack):
        """Spec name resolution: the first segment is looked up frame by
        frame from the innermost context outward; remaining segments descend
        without further stack walking."""
        if path == (".",):
            return stack[-1]
        head, rest = path[0], path[1:]
        value = _MISS
        for frame in reversed(stack):
            if isinstance(frame, Mapping) and head in frame:
                value = frame[head]
                break
        for segment in rest:
            if value is _MISS:
                return _MISS
            if value is None:
                # Present-but-None is empty/falsey (the strict-mode deviation
                # applies to ABSENT keys); descending through it stays None.
                return None
            if isinstance(value, Mapping) and segment in value:
                value = value[segment]
            else:
                return _MISS
        return value
