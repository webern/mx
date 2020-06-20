use indexmap::set::IndexSet;
use std::collections::HashMap;

/// Returns a list of simple-type enumeration values that are mapped to something different in the
/// generated code.
pub(crate) fn enum_member_substitutions() -> HashMap<String, String> {
    let mut substitutions = HashMap::new();
    // for better or worse, i chose to map these enum members that start with numbers like so...
    substitutions.insert("16Th".to_string(), "sixteenth".to_string());
    substitutions.insert("32Nd".to_string(), "thirtySecond".to_string());
    substitutions.insert("64Th".to_string(), "sixtyFourth".to_string());
    substitutions.insert("128Th".to_string(), "oneHundredTwentyEighth".to_string());
    substitutions.insert("256Th".to_string(), "twoHundredFifthySixth".to_string());
    substitutions.insert("512Th".to_string(), "fiveHundredTwelfth".to_string());
    substitutions.insert("1024Th".to_string(), "oneThousandTwentyFourth".to_string());
    substitutions
}

/// Returns a list of enums that I added the word 'Enum' to suffix the name. In some cases this was
/// to prevent a name clash with a complex-type or element by the same name. In other cases this was
/// because I wanted to 'wrap' the enum in a custom class (i.e. when the usage in MusicXML allows
/// for an 'other' string field. These are given as uppercase, as in `Something` where the resulting
/// code will be `enum class SomethingEnum`.
pub(crate) fn suffixed_enum_names() -> IndexSet<String> {
    /// TODO - this list can probably be computed by observing the name conflicts in the XSD.
    let mut names = IndexSet::new();
    names.insert("ArrowDirection".into());
    names.insert("ArrowStyle".into());
    names.insert("BarStyle".into());
    names.insert("CircularArrow".into());
    names.insert("DistanceType".into());
    names.insert("Effect".into());
    names.insert("Glass".into());
    names.insert("LineWidthType".into());
    names.insert("Membrane".into());
    names.insert("Metal".into());
    names.insert("Mode".into());
    names.insert("Mute".into());
    names.insert("Pitched".into());
    names.insert("Step".into());
    names.insert("Syllabic".into());
    names.insert("TimeRelation".into());
    names.insert("StickLocation".into());
    names.insert("StickMaterial".into());
    names.insert("StickType".into());
    names.insert("Wood".into());
    names.insert("SemiPitched".into());
    names.insert("StaffType".into());
    //names.insert("XXX".into());
    //names.insert("XXX".into());
    //names.insert("XXX".into());
    //names.insert("XXX".into());
    //names.insert("XXX".into());
    //names.insert("XXX".into());
    names
}

/// Returns a list of C++ keywords, to be used to avoid conflict with MusicXML names.
pub(crate) fn reserved_words() -> IndexSet<String> {
    let mut keywords = IndexSet::new();
    keywords.insert("continue".into());
    keywords.insert("do".into());
    keywords.insert("double".into());
    keywords.insert("explicit".into());
    keywords.insert("long".into());
    keywords.insert("short".into());
    keywords.insert("while".into());
    keywords
}

pub(crate) struct PseudoEnumSpec {
    pub(crate) members: Vec<String>,
    /// The name of the field that is added to the enum, which can hold any value.
    pub(crate) extra_field_name: String,
    /// The name of the class which "wraps" the enum with the ability to hold an arbitrary string.
    pub(crate) class_name: String,
    /// The enum value when the wrapper class is default constructed.
    pub(crate) default_value: String,
}

/// e.g. `line-width-type` and `distance-type` which give enum values in the documentation but do
/// not restrict the string via xsd. In this case we have extracted the enum values from the
/// documentation by hand and use an `other` 'variant' to hold any string other than those listed.
pub(crate) fn pseudo_enums() -> HashMap<String, PseudoEnumSpec> {
    let mut map = HashMap::new();
    map.insert(
        "xs:simpleType:distance-type".to_owned(),
        PseudoEnumSpec {
            members: vec!["beam".to_owned(), "hyphen".to_owned()],
            extra_field_name: "other".to_owned(),
            class_name: "DistanceType".to_owned(),
            default_value: "beam".to_owned(),
        },
    );
    map.insert(
        "xs:simpleType:line-width-type".to_owned(),
        PseudoEnumSpec {
            members: vec![
                "beam".to_owned(),
                "bracket".to_owned(),
                "dashes".to_owned(),
                "enclosure".to_owned(),
                "ending".to_owned(),
                "extend".to_owned(),
                "heavy barline".to_owned(),
                "leger".to_owned(),
                "light barline".to_owned(),
                "octave shift".to_owned(),
                "pedal".to_owned(),
                "slur middle".to_owned(),
                "slur tip".to_owned(),
                "staff".to_owned(),
                "stem".to_owned(),
                "tie middle".to_owned(),
                "tie tip".to_owned(),
                "tuplet bracket".to_owned(),
                "wedge".to_owned(),
            ],
            extra_field_name: "other".to_owned(),
            class_name: "LineWidthType".to_owned(),
            default_value: "beam".to_owned(),
        },
    );
    map.insert(
        "xs:simpleType:mode".to_owned(),
        PseudoEnumSpec {
            members: vec![
                "major".to_owned(),
                "minor".to_owned(),
                "dorian".to_owned(),
                "phrygian".to_owned(),
                "lydian".to_owned(),
                "mixolydian".to_owned(),
                "aeolian".to_owned(),
                "ionian".to_owned(),
                "locrian".to_owned(),
                "none".to_owned(),
            ],
            extra_field_name: "other".to_owned(),
            class_name: "ModeValue".to_owned(),
            default_value: "major".to_owned(),
        },
    );
    map
}
