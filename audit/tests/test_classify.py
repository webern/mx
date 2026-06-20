"""Tests for the api round-trip failure classifier (audit/classify.py, #211/#212).

Builds synthetic dump directories that mirror the #210 dump format and asserts
the measured-divergence model: the signature set per file, the distance metric
(unique signatures; a tag dropped many times still counts once), the reorder
exclusion from candidates, and the sole-blocker worklist ranking. The classifier
consults no support index -- nothing here writes ``api.features.xml``.

Run with: python3 -m unittest audit.tests.test_classify
"""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from audit import classify


def _wrap(body: str) -> str:
    return f"<score-partwise><part><measure>{body}</measure></part></score-partwise>"


class ClassifierTest(unittest.TestCase):
    def setUp(self) -> None:
        self._tmp = tempfile.TemporaryDirectory()
        self.dump = Path(self._tmp.name) / "dump"
        self.dump.mkdir()
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
        self.report = classify.build_report(self.dump, self.warnings.append)
        return {r["file"]: r for r in self.report["files"]}

    # --- drops ----------------------------------------------------------- #

    def test_drop_enumerates_all_missing(self) -> None:
        # credit AND defaults are both dropped; a positional walk would only see
        # the first. The multiset must report both, as two drop signatures.
        self._pair(
            "wild/drop.xml",
            _wrap("<credit>a</credit><defaults>b</defaults>"
                  "<note><pitch><step>C</step></pitch></note>"),
            _wrap("<note><pitch><step>C</step></pitch></note>"),
        )
        rec = self._classify()["wild/drop.xml"]
        self.assertEqual(rec["status"], "FAIL")
        self.assertEqual(rec["signatures"], ["drop:credit", "drop:defaults"])
        self.assertEqual(rec["distance"], 2)
        self.assertTrue(rec["is_candidate"])

    def test_repeated_drop_counts_once(self) -> None:
        # <credit> dropped three times is still distance 1 (unique signatures).
        self._pair(
            "wild/repeat.xml",
            _wrap("<credit>a</credit><credit>b</credit><credit>c</credit><note/>"),
            _wrap("<note/>"),
        )
        rec = self._classify()["wild/repeat.xml"]
        self.assertEqual(rec["signatures"], ["drop:credit"])
        self.assertEqual(rec["distance"], 1)

    def test_drop_records_sample_path(self) -> None:
        self._pair("wild/path.xml",
                   _wrap("<credit>a</credit><note/>"), _wrap("<note/>"))
        rec = self._classify()["wild/path.xml"]
        self.assertEqual(
            rec["sample_paths"]["drop:credit"],
            "/score-partwise/part/measure/credit",
        )

    # --- value / attr ---------------------------------------------------- #

    def test_value_mismatch(self) -> None:
        self._pair(
            "wild/value.xml",
            _wrap("<note><accidental>sharp-sharp</accidental></note>"),
            _wrap("<note><accidental>sharp</accidental></note>"),
        )
        rec = self._classify()["wild/value.xml"]
        self.assertEqual(rec["signatures"], ["value:accidental"])
        self.assertEqual(rec["signature_counts"], {"value": 1})

    def test_value_survives_a_sibling_drop(self) -> None:
        # A dropped sibling must not desync the walk: the value diff on the
        # surviving <step> is still found alongside the drop.
        self._pair(
            "wild/both.xml",
            _wrap("<credit>a</credit><note><pitch><step>C</step></pitch></note>"),
            _wrap("<note><pitch><step>D</step></pitch></note>"),
        )
        rec = self._classify()["wild/both.xml"]
        self.assertEqual(rec["signatures"], ["drop:credit", "value:step"])

    def test_attribute_dropped(self) -> None:
        self._pair(
            "wild/attr.xml",
            _wrap('<note><dynamics placement="below"/></note>'),
            _wrap("<note><dynamics/></note>"),
        )
        rec = self._classify()["wild/attr.xml"]
        self.assertEqual(rec["signatures"], ["attr:dynamics@placement"])

    def test_attribute_value_mismatch(self) -> None:
        self._pair(
            "wild/attrval.xml",
            _wrap('<note><dynamics placement="below"/></note>'),
            _wrap('<note><dynamics placement="above"/></note>'),
        )
        rec = self._classify()["wild/attrval.xml"]
        self.assertEqual(rec["signatures"], ["value:dynamics@placement"])

    # --- reorder --------------------------------------------------------- #

    def test_reorder_is_not_a_candidate(self) -> None:
        self._pair(
            "wild/reorder.xml",
            _wrap("<note>1</note><backup>2</backup>"),
            _wrap("<backup>2</backup><note>1</note>"),
        )
        rec = self._classify()["wild/reorder.xml"]
        self.assertEqual(rec["signatures"], ["reorder:measure"])
        self.assertTrue(rec["has_reorder"])
        self.assertFalse(rec["is_candidate"])

    # --- crashes --------------------------------------------------------- #

    def test_crash_with_status(self) -> None:
        self._pair("wild/load.xml", _wrap("<note/>"), None)
        self._status("wild/load.xml", "LOADFAIL")
        rec = self._classify()["wild/load.xml"]
        self.assertEqual(rec["status"], "CRASH")
        self.assertEqual(rec["crash_kind"], "LOADFAIL")
        self.assertFalse(rec["is_candidate"])

    def test_crash_without_status(self) -> None:
        self._pair("wild/nostatus.xml", _wrap("<note/>"), None)
        rec = self._classify()["wild/nostatus.xml"]
        self.assertEqual(rec["status"], "CRASH")
        self.assertIsNone(rec["crash_kind"])

    # --- pass ------------------------------------------------------------ #

    def test_identical_is_pass(self) -> None:
        self._pair("wild/ok.xml", _wrap("<note/>"), _wrap("<note/>"))
        rec = self._classify()["wild/ok.xml"]
        self.assertEqual(rec["status"], "PASS")
        self.assertEqual(rec["distance"], 0)
        self.assertFalse(rec["is_candidate"])

    def test_numeric_text_equivalent_is_pass(self) -> None:
        self._pair("wild/num.xml",
                   _wrap("<note>1.0</note>"), _wrap("<note>1</note>"))
        rec = self._classify()["wild/num.xml"]
        self.assertEqual(rec["status"], "PASS")

    # --- record shape ---------------------------------------------------- #

    def test_every_record_has_all_fields(self) -> None:
        self._pair("wild/x.xml", _wrap("<credit>a</credit><note/>"), _wrap("<note/>"))
        rec = next(iter(self._classify().values()))
        for key in (
            "file", "status", "crash_kind", "signatures", "sample_paths",
            "signature_counts", "distance", "has_reorder", "is_candidate",
        ):
            self.assertIn(key, rec)

    # --- worklist / aggregation ----------------------------------------- #

    def test_worklist_ranks_sole_blocker_first(self) -> None:
        # credit blocks 3 files (sole in all 3); defaults blocks 2 but is sole in
        # only 1 (the other also drops bar). credit must rank first.
        self._pair("wild/a.xml", _wrap("<credit>x</credit><note/>"), _wrap("<note/>"))
        self._pair("wild/b.xml", _wrap("<credit>x</credit><note/>"), _wrap("<note/>"))
        self._pair("wild/c.xml", _wrap("<credit>x</credit><note/>"), _wrap("<note/>"))
        self._pair("wild/d.xml", _wrap("<defaults>x</defaults><note/>"), _wrap("<note/>"))
        self._pair("wild/e.xml",
                   _wrap("<defaults>x</defaults><bar>y</bar><note/>"), _wrap("<note/>"))
        self._classify()
        worklist = {r["signature"]: r for r in self.report["worklist"]}
        self.assertEqual(self.report["worklist"][0]["signature"], "drop:credit")
        self.assertEqual(worklist["drop:credit"]["sole_blocker"], 3)
        self.assertEqual(worklist["drop:credit"]["files_blocked"], 3)
        self.assertEqual(worklist["drop:defaults"]["sole_blocker"], 1)
        self.assertEqual(worklist["drop:defaults"]["files_blocked"], 2)

    def test_reorder_excluded_from_worklist(self) -> None:
        self._pair("wild/r.xml",
                   _wrap("<note>1</note><backup>2</backup>"),
                   _wrap("<backup>2</backup><note>1</note>"))
        self._classify()
        self.assertEqual(self.report["worklist"], [])
        self.assertEqual(self.report["summary"]["reorder_blocked"], 1)
        self.assertEqual(self.report["summary"]["candidates"], 0)

    def test_near_misses_grouped_by_distance(self) -> None:
        self._pair("wild/one.xml", _wrap("<credit>x</credit><note/>"), _wrap("<note/>"))
        self._pair("wild/two.xml",
                   _wrap("<credit>x</credit><defaults>y</defaults><note/>"),
                   _wrap("<note/>"))
        self._classify()
        nm = self.report["near_misses"]
        self.assertEqual([f["file"] for f in nm["1"]], ["wild/one.xml"])
        self.assertEqual([f["file"] for f in nm["2"]], ["wild/two.xml"])
        self.assertEqual(nm["3"], [])

    def test_batch_plan_greedy_cover(self) -> None:
        # Greedy picks credit first (clears the one distance-1 file), then
        # defaults (clears the two that need both), then other.
        cands = [
            {"signatures": ["drop:credit", "drop:defaults"]},
            {"signatures": ["drop:credit", "drop:defaults"]},
            {"signatures": ["drop:credit"]},
            {"signatures": ["drop:credit", "drop:other"]},
        ]
        plan = classify.build_batch_plan(cands)
        self.assertEqual(
            [(r["fix"], r["cumulative_files"], r["added_files"]) for r in plan],
            [("drop:credit", 1, 1), ("drop:defaults", 3, 2), ("drop:other", 4, 1)],
        )

    def test_batch_plan_in_report_excludes_reorder(self) -> None:
        self._pair("wild/one.xml", _wrap("<credit/><note/>"), _wrap("<note/>"))
        self._pair("wild/r.xml",
                   _wrap("<note>1</note><backup>2</backup>"),
                   _wrap("<backup>2</backup><note>1</note>"))
        self._classify()
        plan = self.report["batch_plan"]
        self.assertEqual([r["fix"] for r in plan], ["drop:credit"])

    def test_summary_status_counts(self) -> None:
        self._pair("wild/ok.xml", _wrap("<note/>"), _wrap("<note/>"))
        self._pair("wild/fail.xml", _wrap("<credit/><note/>"), _wrap("<note/>"))
        self._pair("wild/crash.xml", _wrap("<note/>"), None)
        self._classify()
        self.assertEqual(self.report["summary"]["by_status"],
                         {"CRASH": 1, "FAIL": 1, "PASS": 1})


if __name__ == "__main__":
    unittest.main()
