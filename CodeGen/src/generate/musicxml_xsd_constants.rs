/*
fn write_enums(&self, simple_types: &Vec<SimpleType>) -> Result<()> {
        let mut substitutions = HashMap::new();
        substitutions.insert("16th".to_string(), "sixteenth".to_string());
        substitutions.insert("32nd".to_string(), "thirtySecond".to_string());
        substitutions.insert("64th".to_string(), "sixtyFourth".to_string());
        substitutions.insert("128th".to_string(), "oneHundredTwentyEighth".to_string());
        substitutions.insert("256th".to_string(), "twoHundredFifthySixth".to_string());
        substitutions.insert("512th".to_string(), "fiveHundredTwelfth".to_string());
        substitutions.insert("1024th".to_string(), "oneThousandTwentyFourth".to_string());
        let sanitizer = StringSanitizer {
            keywords: vec!["double", "short", "long", "continue", "do", "explicit"],
            enum_substitutions: substitutions,
            enum_suffixed: vec![
                "Step",
                "ArrowDirection",
                "ArrowStyle",
                "CircularArrow",
                "Syllabic",
                "BarStyle",
                "TimeRelation",
            ],
        };
        self.write_enums_h(simple_types, &sanitizer)?;
        self.write_enums_cpp(simple_types, &sanitizer)?;
        Ok(())
 */

use indexmap::set::IndexSet;
use std::collections::HashMap;

/// Returns a list of simple-type enumeration values that are mapped to something different in the
/// generated code.
pub(crate) fn enum_member_substitutions() -> HashMap<String, String> {
    let mut substitutions = HashMap::new();
    // for better or worse, i chose to map these enum members that start with numbers like so...
    substitutions.insert("16th".to_string(), "sixteenth".to_string());
    substitutions.insert("32nd".to_string(), "thirtySecond".to_string());
    substitutions.insert("64th".to_string(), "sixtyFourth".to_string());
    substitutions.insert("128th".to_string(), "oneHundredTwentyEighth".to_string());
    substitutions.insert("256th".to_string(), "twoHundredFifthySixth".to_string());
    substitutions.insert("512th".to_string(), "fiveHundredTwelfth".to_string());
    substitutions.insert("1024th".to_string(), "oneThousandTwentyFourth".to_string());
    substitutions
}

/// Returns a list of enums that I added the word 'Enum' to suffix the name. In some cases this was
/// to prevent a name clash with a complex-type or element by the same name. In other cases this was
/// because I wanted to 'wrap' the enum in a custom class (i.e. when the usage in MusicXML allows
/// for an 'other' string field. These are given as uppercase, as in `Something` where the resulting
/// code will be `enum class SomethingEnum`.
pub(crate) fn suffixed_enum_names() -> IndexSet<String> {
    let mut names = IndexSet::new();
    names.insert("Step".into());
    names.insert("ArrowDirection".into());
    names.insert("ArrowStyle".into());
    names.insert("CircularArrow".into());
    names.insert("Syllabic".into());
    names.insert("BarStyle".into());
    names.insert("TimeRelation".into());
    names
}

/// Returns a list of C++ keywords, to be used to avoid conflict with MusicXML names.
pub(crate) fn reserved_words() -> IndexSet<String> {
    let mut keywords = IndexSet::new();
    keywords.insert("double".into());
    keywords.insert("short".into());
    keywords.insert("long".into());
    keywords.insert("continue".into());
    keywords.insert("explicit".into());
    keywords.insert("do".into());
    keywords.insert("while".into());
    keywords
}

pub(crate) struct PseudoEnumSpec {
    pub(crate) members: Vec<String>,
    pub(crate) extra_field_name: String,
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
        },
    );
    map
}
