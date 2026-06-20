"""mx feature-audit entry point.

Usage:
  python3 -m audit files [--force]   write a *.features.xml sidecar next to each
                                     corpus file (skips existing unless --force)
  python3 -m audit corpus            (re)build data/corpus.xml from the corpus
  python3 -m audit all [--force]     run `files` then `corpus`
  python3 -m audit classify <dump_dir> [--data DIR] [--out FILE]
                                     classify api round-trip failures by root
                                     cause from a dump directory (see #211)

See audit/README.md. The audited set mirrors the corert round-trip suite.
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

from . import CORPUS_SUMMARY_NAME, classify, featuresfile, summary
from .discover import discover
from .extract import extract_file

DATA_ROOT = Path(__file__).resolve().parent.parent / "data"
BUILD_ROOT = Path(__file__).resolve().parent.parent / "build"


def _cmd_files(force: bool) -> int:
    files = discover(DATA_ROOT)
    written = skipped = 0
    for cf in files:
        out = cf.features_path
        if out.exists() and not force:
            skipped += 1
            continue
        ff = extract_file(cf.path, cf.rel)
        featuresfile.write(ff, out)
        written += 1
    print(f"features: {written} written, {skipped} skipped (existing), {len(files)} total")
    return 0


def _cmd_corpus() -> int:
    out = DATA_ROOT / CORPUS_SUMMARY_NAME
    out.write_text(summary.build(DATA_ROOT), encoding="utf-8")
    print(f"corpus: wrote {out.relative_to(DATA_ROOT.parent)}")
    return 0


def _cmd_classify(dump_dir: str, data: str | None, out: str | None) -> int:
    data_root = Path(data).resolve() if data else DATA_ROOT
    out_path = Path(out).resolve() if out else BUILD_ROOT / "api" / "classified.json"
    return classify.run(Path(dump_dir).resolve(), data_root, out_path)


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(prog="python3 -m audit")
    sub = parser.add_subparsers(dest="cmd", required=True)

    p_files = sub.add_parser("files", help="write per-file *.features.xml sidecars")
    p_files.add_argument(
        "--force", action="store_true", help="overwrite existing sidecars"
    )
    sub.add_parser("corpus", help="rebuild data/corpus.xml")
    p_all = sub.add_parser("all", help="run files then corpus")
    p_all.add_argument("--force", action="store_true", help="overwrite existing sidecars")

    p_classify = sub.add_parser(
        "classify", help="classify api round-trip failures from a dump directory"
    )
    p_classify.add_argument("dump_dir", help="directory of *.expected.xml/*.actual.xml dumps")
    p_classify.add_argument(
        "--data", default=None, help="data root (default: repo data/ directory)"
    )
    p_classify.add_argument(
        "--out", default=None, help="output JSON path (default: build/api/classified.json)"
    )

    args = parser.parse_args(argv)
    if args.cmd == "files":
        return _cmd_files(args.force)
    if args.cmd == "corpus":
        return _cmd_corpus()
    if args.cmd == "all":
        rc = _cmd_files(args.force)
        return rc or _cmd_corpus()
    if args.cmd == "classify":
        return _cmd_classify(args.dump_dir, args.data, args.out)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
