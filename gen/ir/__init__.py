"""Intermediate representation for the mx generator."""

from gen.ir.build import build_ir
from gen.ir.model import Ir
from gen.ir.resolve import Resolver

__all__ = ["Ir", "Resolver", "build_ir"]
