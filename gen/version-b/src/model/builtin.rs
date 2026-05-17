#[derive(Debug, Clone, Copy, Eq, PartialEq, Ord, PartialOrd)]
pub enum Builtin {
    String(BuiltinString),
    Number(BuiltinNumber),
}

#[derive(Debug, Clone, Copy, Eq, PartialEq, Ord, PartialOrd)]
pub enum BuiltinString {
    String,
    Token,
    IDToken,
}

#[derive(Debug, Clone, Copy, Eq, PartialEq, Ord, PartialOrd)]
pub enum BuiltinNumber {
    Integer,
    NonNegativeInteger,
    PositiveInteger,
    Decimal,
    NonNegativeDecimal,
    PositiveDecimal,
}
