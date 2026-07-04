---
applyTo: "src/**/*.cpp,src/**/*.h"
---

Repo-wide C++ rules (see AGENTS.md):

- No new anonymous namespaces (`namespace { }`) -- they break unity builds. File-local helpers
  get named functions (`tokenIsNameChar`); file-local constants get a per-type prefix
  (`kYesNoWire`). Existing anonymous namespaces are being retired as touched; do not demand
  drive-by fixes.
- Formatting is enforced by clang-format (`make fmt` / `make check`); do not comment on layout
  the formatter controls.
- Keep source ASCII.
- Comments should state constraints and effects hard to ascertain from the code.
