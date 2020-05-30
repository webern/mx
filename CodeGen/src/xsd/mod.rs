enum RestringString {
    XsString,
    XsToken,
    Xs,
}

enum RestrictionBase {
    BuiltinString,
    BuiltinInt,
    BuilinString,
}

enum RestrictionType {
    Enumeration,
    Range,
}

enum TypeDefinition {
    Restriction(RestrictionType),
    Extension,
}

struct SimpleType {}