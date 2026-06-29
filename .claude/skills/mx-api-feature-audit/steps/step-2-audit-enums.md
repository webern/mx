# Step 2: Hunt enum bugs (`mx::core` vs `mx::api`)

This is the highest-severity part of the audit. An enum bug is silent data loss: a value the corpus
(and spec) can contain, that `mx::api` cannot represent, so it is dropped on read and cannot be
written back.

## Why these bugs appear

`mx::core` enums are *generated from the XSD* (`src/private/mx/core/generated/`), so they
automatically gain any member a new MusicXML version adds. The `mx::api` enums
(`src/include/mx/api/*.h`) are *hand-written*. When 3.1 or 4.0 adds a member to a core enum, nobody
necessarily updates the matching api enum or the translation table -- and the gap is invisible until
a file uses the new value.

## The canonical mechanism: `Converter` EnumMap tables

`src/private/mx/impl/Converter.cpp` declares ~30 translation tables, each of the form:

```cpp
const Converter::EnumMap<core::X, api::Y> Converter::someMap = {
    {core::X::a, api::Y::a},
    {core::X::b, api::Y::b},
    ...
};
```

An `EnumMap` is just `std::vector<std::pair<CORE, API>>`. Translation uses:

```cpp
findApiItem(someMap, api::Y::<fallback>, coreValue)   // core -> api (read)
findCoreItem(someMap, core::X::<fallback>, apiValue)  // api -> core (write)
```

If a core value is not a key in the table, `findApiItem` returns the fallback (`unspecified`,
`otherDynamics`, `otherArticulation`, ...). That fallback IS the data loss.

So the bug test for each table is mechanical:

1. Enumerate every member of the `core::X` enum from its generated header (the `enum class Kind`/
   `Tag`/value list, or the named factories). For choice-group kinds, that is
   `src/private/mx/core/generated/<X>.h`.
2. Enumerate the core-side keys present in the table.
3. Any core member absent from the table is a BUG candidate: it falls back on read.
4. Confirm the `api::Y` enum has no member that could hold it (usually it does not, which is why the
   table omits it). If `api::Y` genuinely lacks the member, the fix is two-sided: add the `api::Y`
   member AND the table row (and check the writer path).

List the tables to audit:

```
grep -n "const Converter::EnumMap" src/private/mx/impl/Converter.cpp
```

Each line names the `core::X` and `api::Y` pair. Work through all of them. The worked example from
the SKILL overview -- `dynamicsMap` (`Converter.cpp:170`) missing `n`/`pf`/`sfzp` from
`core::DynamicsChoice::Kind` -- is exactly this procedure applied to one table.

## Enums not behind an EnumMap

Some core enums are translated inline (a `switch`, or direct field copy) rather than via a table, and
some api enums are populated by hand in a `*Reader`. For completeness:

```
grep -rln "enum class" src/include/mx/api/         # every api enum
grep -rln "enum class" src/private/mx/core/generated/  # every core enum/choice kind
```

For each api enum, find where the matching core value is translated (`grep` the api enum name across
`src/private/mx/impl/`). If the translation is a `switch`, check for a `default:` that swallows
unlisted core values -- same bug, different shape.

## Prioritize with the corpus

For every BUG candidate, check `data/corpus.xml` for the enum's element:

- Is the missing value actually present in a wild file? Search the relevant `*.features.xml`
  sidecars or grep the corpus for the literal. A bug hit by a wild file is urgent.
- Even if only synthetic files use it, it is still a correctness bug (the spec allows it), just
  lower urgency.

## Record findings

Capture each confirmed bug as: core enum + missing member(s), the api enum, the table/site
(`file:line`), the fallback it lands on, and whether any wild file triggers it. These become the
top-priority section of the Step 4 report, and the `<enums>` entries in `data/api.features.xml`
(Step 3).
