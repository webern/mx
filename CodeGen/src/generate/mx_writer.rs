use crate::generate::mx_enum_writer::MxEnumWriter;
use std::io::Write;

pub(crate) const LINE_WIDTH: usize = 100;
pub(crate) const INDENT: &str = "    ";
pub(crate) const DOC_COMMENT: &str = "///";

pub(crate) struct MxWriter {
    pub(crate) enum_writer: MxEnumWriter,
}

impl MxWriter {
    pub(crate) fn new(enum_writer: MxEnumWriter) -> Self {
        Self { enum_writer }
    }

    pub(crate) fn write_enum_declarations<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        self.enum_writer.write_declarations(w)
    }

    pub(crate) fn write_enum_definitions<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        self.enum_writer.write_definitions(w)
    }
}
