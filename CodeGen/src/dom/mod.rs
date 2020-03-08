pub enum ElementContent {
    Empty,
    Text(String),
    Parent(Vec<Element>),
}

pub struct Attribute {
    name: String,
    value: String,
}

pub struct Element {
    line: u64,
    column: u64,
    namespace: Option<String>,
    name: String,
    attributes: Vec<Attribute>,
    content: ElementContent,
}