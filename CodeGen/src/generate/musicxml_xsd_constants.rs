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

use std::collections::HashMap;
use indexmap::set::IndexSet;

/// Returns a list of simple-type enumeration values that are mapped to something different in the
/// generated code.
fn enum_member_substitutions() -> HashMap<String, String> {
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
fn suffixed_enum_names() -> IndexSet<String> {
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
fn keywords() -> IndexSet<String> {
    let mut keywords = IndexSet::new();
    keywords.insert("double".into());
    keywords.insert("short".into());
    keywords.insert("long".into());
    keywords.insert("continue".into());
    keywords.insert("explicit".into());
    keywords
}