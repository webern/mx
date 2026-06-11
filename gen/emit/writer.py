"""Transitional shim: the writer moved to gen.press.writer; the legacy
backends import through here until they are replaced by templates."""

from gen.press.writer import EmitResult, banner, is_generated, write_files

__all__ = ["EmitResult", "banner", "is_generated", "write_files"]
