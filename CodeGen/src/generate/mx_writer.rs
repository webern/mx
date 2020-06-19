use crate::generate::mx_enum_writer::MxEnumWriter;
use std::io::Write;

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
}
