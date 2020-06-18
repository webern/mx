pub(crate) struct MusicXSD {}

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
