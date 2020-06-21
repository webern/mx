use crate::generate::mx_enum_writer::MxEnumOption;

pub struct MusicXSD {
    type_definitions: Vec<TypeDefinition>,
}

impl MusicXSD {
    pub fn new(type_definitions: Vec<TypeDefinition>) -> Self {
        Self { type_definitions }
    }
}

impl MusicXSD {
    pub fn enumerations(&self) -> Vec<Enumeration> {
        let mut result = Vec::new();
        for t in &self.type_definitions {
            if let TypeDefinition::Simple(simple_type) = t {
                if let SimpleType::Enum(enumeration) = simple_type {
                    result.push(enumeration.clone());
                }
            }
        }
        result.sort_by(|a, b| Ord::cmp(&a.name, &b.name));
        result
    }
}

pub enum TypeDefinition {
    Simple(SimpleType),
    #[allow(dead_code)]
    Todo,
}

pub enum SimpleType {
    Enum(Enumeration),
    #[allow(dead_code)]
    Todo,
}

#[derive(Clone)]
pub struct Enumeration {
    pub index: usize,
    pub id: String,
    pub name: String,
    pub base: String,
    pub documentation: String,
    pub members: Vec<String>,
    /// For pseudo-enums and other types that append an enumeration named, e.g. `other` which
    /// signals that any string can be contained with the enum value is set to e.g. `other`
    pub other_field: Option<MxEnumOption>,
}

impl Default for Enumeration {
    fn default() -> Enumeration {
        Enumeration {
            index: 0,
            id: String::default(),
            name: String::default(),
            base: String::default(),
            documentation: String::default(),
            members: Vec::new(),
            other_field: None,
        }
    }
}
