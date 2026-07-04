"""MusicXML feature-audit tooling.

This package compares three views of the MusicXML feature surface:

  1. the test corpus under ``data/`` (what real and synthetic files actually use),
  2. the MusicXML specification (approximated by the synthetic corpus, which is
     built to exercise every element/attribute/enum symbol of 3.0/3.1/4.0), and
  3. the ``mx::api`` public layer (audited separately by the ``mx-api-feature-audit``
     skill, which reads source and emits ``data/api.features.xml``).

A "feature" is an element together with the set of attribute names that appear
on it. The tool emits one ``*.features.xml`` sibling per corpus file and a single
aggregate at ``data/corpus.xml``. See ``audit/README.md``.
"""

from __future__ import annotations

# Suffix of the per-file audit sidecars, and the basename of the aggregate. The
# C++/Go/C corpus-discovery walkers skip both so the round-trip suites never try
# to parse an audit artifact as a MusicXML score. Keep these in sync with the
# `isFeaturesSidecar`/`is_features_sidecar` predicates in those walkers.
FEATURES_SUFFIX = ".features.xml"
CORPUS_SUMMARY_NAME = "corpus.xml"
