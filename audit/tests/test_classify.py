"""Tests for the api round-trip failure classifier (audit/classify.py, #211).

Builds synthetic dump directories that mirror the #210 dump format and asserts
the classifier's category assignment, the multiset diff (the core fix: it must
enumerate *every* missing element class, not just the first), and the ranking.

Run with: python3 -m unittest audit.tests.test_classify
"""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from audit import classify

# A small api.features.xml fixture with controlled support levels.
API_FEATURES = """<?xml version="1.0" encoding="UTF-8"?>
<feature-audit>
  <audited>mx::api</audited>
  <audited-version>4.0</audited-version>
  <features>
    <feature name="score-partwise" support="full"/>
    <feature name="part" support="full"/>
    <feature name="part-list" support="full"/>
    <feature name="measure" support="full"/>
    <feature name="note" support="full"/>
    <feature name="pitch" support="full"/>
    <feature name="step" support="full"/>
    <feature name="backup" support="full"/>
    <feature name="credit" support="none"/>
    <feature name="defaults" support="none"/>
    <feature name="accidental" support="partial">
      <enums>
        <enum core-type="AccidentalValue" api-type="Accidental" status="missing-members">
          <missing>sharpSharp</missing>
        </enum>
      </enums>
    </feature>
    <feature name="dynamics" support="partial">
      <attributes>
        <attribute name="placement" support="partial"/>
      </attributes>
    </feature>
  </features>
</feature-audit>
"""


def _wrap(body: str) -> str:
    return f"<score-partwise><part><measure>{body}</measure></part></score-partwise>"


class ClassifierTest(unittest.TestCase):
    def setUp(self) -> None:
        self._tmp = tempfile.TemporaryDirectory()
        self.root = Path(self._tmp.name)
        self.data = self.root / "data"
        self.dump = self.root / "dump"
        self.data.mkdir()
        self.dump.mkdir()
        (self.data / "api.features.xml").write_text(API_FEATURES, encoding="utf-8")
        self.warnings: list[str] = []

    def tearDown(self) -> None:
        self._tmp.cleanup()

    def _pair(self, rel: str, expected: str, actual: str | None) -> None:
        flat = rel.replace("/", "__")
        (self.dump / f"{flat}.expected.xml").write_text(expected, encoding="utf-8")
        if actual is not None:
            (self.dump / f"{flat}.actual.xml").write_text(actual, encoding="utf-8")

    def _status(self, rel: str, code: str) -> None:
        flat = rel.replace("/", "__")
        (self.dump / f"{flat}.status").write_text(code + "\n", encoding="utf-8")

    def _classify(self) -> dict[str, dict]:
        report = classify.build_report(
            self.dump, self.data, self.data / "api.features.xml", self.warnings.append
        )
        self.report = report
        return {r["file"]: r for r in report["files"]}

    # --- individual categories ------------------------------------------- #

    def test_drop_only_enumerates_all_missing(self) -> None:
        # credit AND defaults are both dropped; a positional walk would only see
        # the first. The multiset must report both.
        self._pair(
            "wild/drop.xml",
            _wrap("<credit>a</credit><defaults>b</defaults>"
                  "<note><pitch><step>C</step></pitch></note>"),
            _wrap("<note><pitch><step>C</step></pitch></note>"),
        )
        rec = self._classify()["wild/drop.xml"]
        self.assertEqual(rec["primary_category"], "B")
        self.assertEqual(rec["missing_elements"], ["credit", "defaults"])
        self.assertEqual(rec["distinct_missing_count"], 2)
        self.assertFalse(rec["is_single_blocker"])
        self.assertEqual(rec["blocking_features"], ["credit", "defaults"])

    def test_drop_only_single_blocker(self) -> None:
        self._pair(
            "wild/single.xml",
            _wrap("<credit>a</credit><note><pitch><step>C</step></pitch></note>"),
            _wrap("<note><pitch><step>C</step></pitch></note>"),
        )
        rec = self._classify()["wild/single.xml"]
        self.assertEqual(rec["primary_category"], "B")
        self.assertEqual(rec["distinct_missing_count"], 1)
        self.assertTrue(rec["is_single_blocker"])

    def test_reorder_only(self) -> None:
        self._pair(
            "wild/reorder.xml",
            _wrap("<note>1</note><backup>2</backup>"),
            _wrap("<backup>2</backup><note>1</note>"),
        )
        rec = self._classify()["wild/reorder.xml"]
        self.assertEqual(rec["primary_category"], "C")
        self.assertEqual(rec["missing_elements"], [])
        self.assertEqual(rec["added_elements"], [])

    def test_enum_bug(self) -> None:
        self._pair(
            "wild/enum.xml",
            _wrap("<note><accidental>sharp-sharp</accidental></note>"),
            _wrap("<note><accidental>sharp</accidental></note>"),
        )
        rec = self._classify()["wild/enum.xml"]
        self.assertEqual(rec["primary_category"], "D")
        self.assertEqual(rec["mismatch_type"], "text")
        self.assertEqual(rec["first_divergence_element"], "accidental")

    def test_missing_attribute(self) -> None:
        self._pair(
            "wild/attr.xml",
            _wrap('<note><dynamics placement="below"/></note>'),
            _wrap("<note><dynamics/></note>"),
        )
        rec = self._classify()["wild/attr.xml"]
        self.assertEqual(rec["primary_category"], "E")
        self.assertEqual(rec["mismatch_type"], "attribute-count")

    def test_supported_element_drop(self) -> None:
        # backup is support="full" but vanishes. That is not category B (which
        # needs *every* drop to be support="none"), so it must surface as G
        # rather than fall through to "unknown".
        self._pair(
            "wild/supdrop.xml",
            _wrap("<backup>1</backup><note><pitch><step>C</step></pitch></note>"),
            _wrap("<note><pitch><step>C</step></pitch></note>"),
        )
        rec = self._classify()["wild/supdrop.xml"]
        self.assertEqual(rec["primary_category"], "G")
        self.assertEqual(rec["missing_elements"], ["backup"])
        self.assertEqual(rec["blocking_features"], ["backup"])
        self.assertTrue(rec["is_single_blocker"])

    def test_mixed_supported_and_none_drop_is_g(self) -> None:
        # A supported drop (backup=full) mixed with an unsupported drop
        # (credit=none) is G, not B -- and only the supported tag is a blocker.
        self._pair(
            "wild/mixed.xml",
            _wrap("<backup>1</backup><credit>c</credit><note/>"),
            _wrap("<note/>"),
        )
        rec = self._classify()["wild/mixed.xml"]
        self.assertEqual(rec["primary_category"], "G")
        self.assertEqual(rec["missing_elements"], ["backup", "credit"])
        self.assertEqual(rec["blocking_features"], ["backup"])
        self.assertEqual(rec["secondary_categories"], [])

    def test_pipeline_error_with_status(self) -> None:
        self._pair("wild/load.xml", _wrap("<note/>"), None)
        self._status("wild/load.xml", "LOADFAIL")
        rec = self._classify()["wild/load.xml"]
        self.assertEqual(rec["primary_category"], "F")
        self.assertEqual(rec["status"], "LOADFAIL")
        self.assertEqual(rec["pipeline_error_kind"], "LOADFAIL")

    def test_pipeline_error_without_status(self) -> None:
        self._pair("wild/nostatus.xml", _wrap("<note/>"), None)
        rec = self._classify()["wild/nostatus.xml"]
        self.assertEqual(rec["primary_category"], "F")
        self.assertEqual(rec["status"], "PIPELINE_ERROR")
        self.assertIsNone(rec["pipeline_error_kind"])

    def test_unknown_full_support_mismatch(self) -> None:
        self._pair(
            "wild/unknown.xml",
            _wrap("<note><pitch><step>C</step></pitch></note>"),
            _wrap("<note><pitch><step>D</step></pitch></note>"),
        )
        rec = self._classify()["wild/unknown.xml"]
        self.assertEqual(rec["primary_category"], "unknown")

    def test_every_record_has_all_fields(self) -> None:
        self._pair("wild/single.xml",
                   _wrap("<credit>a</credit><note/>"), _wrap("<note/>"))
        rec = next(iter(self._classify().values()))
        for key in (
            "file", "status", "primary_category", "secondary_categories",
            "first_divergence_element", "first_divergence_path", "mismatch_type",
            "missing_elements", "missing_element_counts", "distinct_missing_count",
            "added_elements", "is_single_blocker", "total_missing_instances",
            "blocking_features", "pipeline_error_kind",
        ):
            self.assertIn(key, rec)

    def test_missing_sidecar_warns_not_aborts(self) -> None:
        self._pair("wild/single.xml",
                   _wrap("<credit>a</credit><note/>"), _wrap("<note/>"))
        self._classify()  # no sidecars created -> warnings, no exception
        self.assertTrue(any("no feature sidecar" in w for w in self.warnings))

    def test_ranking_orders_by_files_unblocked(self) -> None:
        # credit blocks two files, defaults blocks one.
        self._pair("wild/a.xml", _wrap("<credit>x</credit><note/>"), _wrap("<note/>"))
        self._pair("wild/b.xml", _wrap("<credit>x</credit><note/>"), _wrap("<note/>"))
        self._pair("wild/c.xml", _wrap("<defaults>x</defaults><note/>"), _wrap("<note/>"))
        records = list(self._classify().values())
        ranked = classify._rank_blocking_features(records)
        self.assertEqual(ranked[0][0], "credit")
        self.assertEqual(ranked[0][1], 2)  # files unblocked
        self.assertEqual(ranked[0][2], 2)  # single-blocker count


if __name__ == "__main__":
    unittest.main()
