use super::musicxml_xsd_parser::TypeDefinition;

pub(crate) struct MusicXSD {
    pub(crate) type_definitions: Vec<TypeDefinition>,
}

enum Type {
    Simple,
}

enum Simple {
    Builtin,
    Enum,
}

enum Enum {
    Standard,
    Algebraic,
}
