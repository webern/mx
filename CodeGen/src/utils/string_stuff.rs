// TODO - these string constants are inflexible.
pub const LINE_WIDTH: usize = 100;
pub const INDENT: &str = "    ";
pub const DOC_COMMENT: &str = "///";

use crate::error::Result;
use std::borrow::Cow;
use std::cmp::Ordering;
use std::fmt::{Display, Formatter};
use std::io::{Cursor, Write};

#[derive(Debug, Clone, Eq, Ord, Default)]
pub struct Altered {
    pub value: String,
    pub original: String,
}

impl Display for Altered {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}({})", self.value, self.original)
    }
}

impl PartialOrd for Altered {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.value.cmp(&other.value))
    }
}

impl PartialEq for Altered {
    fn eq(&self, other: &Altered) -> bool {
        self.value == other.value
    }
}

#[derive(Debug, Clone, Eq, Ord)]
pub enum Symbol {
    Unaltered(String),
    Altered(Altered),
}

impl PartialOrd for Symbol {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.value().cmp(other.value()))
    }
}

impl PartialEq for Symbol {
    fn eq(&self, other: &Symbol) -> bool {
        self.value() == other.value()
    }
}

impl Symbol {
    pub fn new<S: AsRef<str>>(s: S) -> Symbol {
        Symbol::Unaltered(s.as_ref().to_string())
    }

    /// If `altered` and `original` are equal, returns `Unaltered`, else returns `Altered`.
    pub fn from<S1, S2>(altered: S1, original: S2) -> Self
    where
        S1: AsRef<str>,
        S2: AsRef<str>,
    {
        if altered.as_ref() == original.as_ref() {
            Symbol::new(original.as_ref())
        } else {
            Symbol::Altered(Altered {
                value: altered.as_ref().into(),
                original: original.as_ref().into(),
            })
        }
    }

    pub fn value(&self) -> &str {
        match self {
            Symbol::Unaltered(s) => s.as_str(),
            Symbol::Altered(a) => a.value.as_str(),
        }
    }

    pub fn original(&self) -> &str {
        match self {
            Symbol::Unaltered(s) => s.as_str(),
            Symbol::Altered(a) => a.original.as_str(),
        }
    }
}

pub fn tokenize<S: AsRef<str>>(s: S) -> Vec<String> {
    let mut tokens = Vec::new();
    let mut buf = String::new();
    let mut is_word_start = true;
    let mut was_lowercase = true;
    let mut was_digit = false;
    for c in s.as_ref().chars() {
        if was_lowercase && c.is_uppercase() {
            is_word_start = true;
        }
        if c.is_ascii_digit() {
            is_word_start = !was_digit;
        }
        if !c.is_alphanumeric() {
            is_word_start = true;
        } else if is_word_start {
            if !buf.is_empty() {
                tokens.push(buf);
                buf = String::new();
            }
            buf.push(c.to_ascii_lowercase());
            is_word_start = false;
        } else {
            buf.push(c.to_ascii_lowercase());
            is_word_start = false;
        }
        was_lowercase = !c.is_ascii_digit() && c.is_lowercase();
        was_digit = c.is_ascii_digit();
    }
    if !buf.is_empty() {
        tokens.push(buf);
    }
    tokens
}

pub fn pascal_case<S: AsRef<str>>(s: S) -> Symbol {
    case(s, Case::Pascal)
}

pub fn camel_case<S: AsRef<str>>(s: S) -> Symbol {
    case(s, Case::Camel)
}

#[derive(Debug, Eq, PartialEq, Ord, PartialOrd)]
enum Case {
    Camel,
    Pascal,
}

fn case<S: AsRef<str>>(s: S, cs: Case) -> Symbol {
    let mut out = String::new();
    let mut is_next_upper = cs == Case::Pascal;
    for c in s.as_ref().chars() {
        if !c.is_alphanumeric() {
            is_next_upper = true;
        } else if c.is_ascii_digit() {
            out.push(c);
            is_next_upper = false;
        } else if is_next_upper {
            out.push(c.to_ascii_uppercase());
            is_next_upper = false;
        } else {
            out.push(c.to_ascii_lowercase());
            is_next_upper = false;
        }
    }
    Symbol::from(out, s)
}

pub fn sep<S: AsRef<str>>(s: S, subtract_indentations: usize) -> String {
    let mut out = DOC_COMMENT.to_owned();
    let width = LINE_WIDTH - (subtract_indentations * 4);
    let s = s.as_ref();
    if !s.is_empty() {
        out.push_str(format!(" {} ", s).as_str());
    }
    loop {
        if out.len() >= width {
            break;
        }
        out.push('/');
    }
    out
}

pub fn documentation<S: AsRef<str>>(documentation: S, indents: usize) -> Result<String> {
    let mut c = Cursor::new(Vec::new());
    wrap!(write_documentation(&mut c, documentation, indents))?;
    Ok(String::from_utf8_lossy(c.get_ref().as_slice()).to_string())
}

pub fn write_documentation<W, S>(w: &mut W, documentation: S, indents: usize) -> std::io::Result<()>
where
    W: Write,
    S: AsRef<str>,
{
    let orig = documentation.as_ref();
    let orig_lines = orig.lines();
    let mut consec_empty = 0;
    let mut clean_lines: Vec<String> = Vec::new();
    for line in orig_lines {
        let mut line: String = line.into();
        let line = line.replace("\t", "    ");
        if is_empty(&line) {
            if consec_empty < 2 {
                clean_lines.push(line.clone());
            }
            consec_empty += 1;
        } else {
            clean_lines.push(line.clone());
            consec_empty = 0;
        }
        // clean_lines.push(line.clone());
    }
    let width = LINE_WIDTH - (DOC_COMMENT.len() + 1) - (INDENT.len() * indents);
    let mut final_lines: Vec<Cow<'_, str>> = Vec::new();
    for line in &clean_lines {
        if is_empty(line.as_str()) {
            final_lines.push(Cow::Borrowed(line))
        }
        let mut result = textwrap::wrap(line.as_str(), width);
        final_lines.append(&mut result);
    }
    consec_empty = 0;
    for (i, line) in final_lines.iter().enumerate() {
        let line = line.as_ref();
        let line_is_empty = is_empty(line);
        let last_line = i == final_lines.len() - 1;
        if line_is_empty && consec_empty > 0 {
            consec_empty += 1;
            continue;
        }
        if line_is_empty {
            linestart(w, indents, false)?;
            consec_empty += 1;
        } else {
            linestart(w, indents, true)?;
            consec_empty = 0;
        }
        if last_line {
            write!(w, "{}", line)?;
        } else {
            writeln!(w, "{}", line)?;
        }
    }
    Ok(())
}

fn is_empty<S: AsRef<str>>(line: S) -> bool {
    for c in line.as_ref().chars() {
        if !c.is_whitespace() {
            return false;
        }
    }
    true
}

pub fn linestart<W: Write>(w: &mut W, indents: usize, add_space: bool) -> std::io::Result<usize> {
    let mut pos: usize = 0;
    for _ in 0..indents {
        write!(w, "{}", INDENT)?;
        pos += INDENT.len();
    }
    write!(w, "{}", DOC_COMMENT)?;
    pos += DOC_COMMENT.len();
    if add_space {
        write!(w, " ")?;
        pos += 1;
    }
    Ok(pos)
}

pub fn words<S: AsRef<str>>(s: S) -> Vec<String> {
    let mut result = Vec::new();
    let mut consecutive_newlines = 0;
    let mut word = String::new();
    for c in s.as_ref().chars() {
        if c.is_whitespace() {
            if !word.is_empty() {
                result.push(word.clone());
                word.clear();
            }
            if c == '\n' {
                if consecutive_newlines == 0 {
                    result.push("\n".into());
                }
                consecutive_newlines += 1;
                continue;
            }
        } else {
            word.push(c);
        }
        consecutive_newlines = 0;
    }
    if !word.is_empty() {
        result.push(word);
    }
    result
}

#[cfg(test)]
mod tests {
    use super::{documentation, words, Symbol};
    use crate::utils::string_stuff::tokenize;
    use std::io::Cursor;

    #[test]
    fn pascal_case_clef_tab() {
        let input = "TAB";
        let got = super::pascal_case(input);
        let want = "Tab".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn camel_case_clef_tab() {
        let input = "TAB";
        let got = super::camel_case(input);
        let want = "tab".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn pascal_case_step_a_lower() {
        let input = "a";
        let got = super::pascal_case(input);
        let want = "A".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn camel_case_step_a_lower() {
        let input = "a";
        let got = super::camel_case(input);
        let want = "a".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Unaltered(unaltered) = got {
            assert_eq!(unaltered.as_str(), input);
        } else {
            panic!("expected Symbol::Unaltered");
        }
    }

    #[test]
    fn pascal_case_wind_chimes() {
        let input = "wind cHIMES";
        let got = super::pascal_case(input);
        let want = "WindChimes".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn camel_case_step_wind_chimes() {
        let input = "wind cHIMES";
        let got = super::camel_case(input);
        let want = "windChimes".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn pascal_case_chord_kind() {
        let input = "dominant-11th";
        let got = super::pascal_case(input);
        let want = "Dominant11th".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn camel_case_step_chord_kind() {
        let input = "DomInAnt 1 1th";
        let got = super::camel_case(input);
        let want = "dominant11th".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn get_words() {
        let input = r#"Here is a sentence. And another sentence, which has a comma:
        With extra spaces and newlines, etc."#;
        let got = words(input);
        let want = vec![
            "Here".to_owned(),
            "is".to_owned(),
            "a".to_owned(),
            "sentence.".to_owned(),
            "And".to_owned(),
            "another".to_owned(),
            "sentence,".to_owned(),
            "which".to_owned(),
            "has".to_owned(),
            "a".to_owned(),
            "comma:".to_owned(),
            "\n".to_owned(),
            "With".to_owned(),
            "extra".to_owned(),
            "spaces".to_owned(),
            "and".to_owned(),
            "newlines,".to_owned(),
            "etc.".to_owned(),
        ];
        assert_eq!(got.len(), want.len());
        for (i, want_word) in want.iter().enumerate() {
            let got_word = got.get(i).unwrap();
            assert_eq!(got_word, want_word);
        }
    }

    #[test]
    fn write_documentation() {
        let input = r#"Here is a sentence. And another sentence, which has a comma:
        With extra spaces and newlines, etc."#;
        let mut w = Cursor::new(vec![0; 15]);
        super::write_documentation(&mut w, input, 1).unwrap();
        let got = String::from_utf8_lossy(w.get_ref());
        let want = r#"    /// Here is a sentence. And another sentence, which has a comma:
    ///         With extra spaces and newlines, etc."#;
        assert_eq!(got, want);
    }

    #[test]
    fn write_documentation_wrap() {
        let input = "Here is a sentence. And another sentence, which has a comma: without a \
        newline it goes on for a long time and must be wrapped it's really quite remarkable";
        let mut w = Cursor::new(vec![0; 15]);
        super::write_documentation(&mut w, input, 1).unwrap();
        let got = String::from_utf8_lossy(w.get_ref());
        let want = r#"    /// Here is a sentence. And another sentence, which has a comma: without a newline it goes on
    /// for a long time and must be wrapped it's really quite remarkable"#;
        assert_eq!(got, want);
    }

    #[test]
    fn write_documentation_newlines() {
        let input = "a\nb\n\nc\n\n\n\nd";
        let mut w = Cursor::new(vec![0; 15]);
        super::write_documentation(&mut w, input, 1).unwrap();
        let got = String::from_utf8_lossy(w.get_ref());
        let want = r#"    /// a
    /// b
    ///
    /// c
    ///
    /// d"#;
        assert_eq!(got, want);
    }

    #[test]
    fn tokenize_strings() {
        struct TestCase {
            input: &'static str,
            want: Vec<String>,
        }
        let mut test_cases = vec![
            TestCase {
                input: "modeValue",
                want: vec!["mode".to_owned(), "value".to_owned()],
            },
            TestCase {
                input: "DomInAnt11th",
                want: vec![
                    "dom".to_owned(),
                    "in".to_owned(),
                    "ant".to_owned(),
                    "11th".to_owned(),
                ],
            },
            TestCase {
                input: "dominant11th",
                want: vec!["dominant".to_owned(), "11th".to_owned()],
            },
        ];
        for test_case in &test_cases {
            let got = tokenize(test_case.input);
            assert_eq!(got, test_case.want);
        }
    }

    const DYNAMICS_DOCUMENTATION: &str = r#"Dynamics can be associated either with a note or a general musical direction. To avoid inconsistencies between and amongst the letter abbreviations for dynamics (what is sf vs. sfz, standing alone or with a trailing dynamic that is not always piano), we use the actual letters as the names of these dynamic elements. The other-dynamics element allows other dynamic marks that are not covered here, but many of those should perhaps be included in a more general musical direction element. Dynamics elements may also be combined to create marks not covered by a single element, such as sfmp.




These letter dynamic symbols are separated from crescendo, decrescendo, and wedge indications. Dynamic representation is inconsistent in scores. Many things are assumed by the composer and left out, such as returns to original dynamics. Systematic representations are quite complex: for example, Humdrum has at least 3 representation formats related to dynamics. The MusicXML format captures what is in the score, but does not try to be optimal for analysis or synthesis of dynamics."#;
    const DYNAMICS_EXPECTED: &str = r#"        /// Dynamics can be associated either with a note or a general musical direction. To avoid
        /// inconsistencies between and amongst the letter abbreviations for dynamics (what is sf
        /// vs. sfz, standing alone or with a trailing dynamic that is not always piano), we use the
        /// actual letters as the names of these dynamic elements. The other-dynamics element allows
        /// other dynamic marks that are not covered here, but many of those should perhaps be
        /// included in a more general musical direction element. Dynamics elements may also be
        /// combined to create marks not covered by a single element, such as sfmp.
        ///
        /// These letter dynamic symbols are separated from crescendo, decrescendo, and wedge
        /// indications. Dynamic representation is inconsistent in scores. Many things are assumed
        /// by the composer and left out, such as returns to original dynamics. Systematic
        /// representations are quite complex: for example, Humdrum has at least 3 representation
        /// formats related to dynamics. The MusicXML format captures what is in the score, but does
        /// not try to be optimal for analysis or synthesis of dynamics."#;

    #[test]
    fn test_documentation() {
        let got = documentation(DYNAMICS_DOCUMENTATION, 2).unwrap();
        let want = DYNAMICS_EXPECTED;
        assert_eq!(got, want);
    }

    const KIND_DOCUMENTATION: &str = r#"A kind-value indicates the type of chord. Degree elements can then add, subtract, or alter from these starting points. Values include:

Triads:
	major (major third, perfect fifth)
	minor (minor third, perfect fifth)
	augmented (major third, augmented fifth)
	diminished (minor third, diminished fifth)
Sevenths:
	dominant (major triad, minor seventh)
	major-seventh (major triad, major seventh)
	minor-seventh (minor triad, minor seventh)
	diminished-seventh (diminished triad, diminished seventh)
	augmented-seventh (augmented triad, minor seventh)
	half-diminished (diminished triad, minor seventh)
	major-minor (minor triad, major seventh)
Sixths:
	major-sixth (major triad, added sixth)
	minor-sixth (minor triad, added sixth)
Ninths:
	dominant-ninth (dominant-seventh, major ninth)
	major-ninth (major-seventh, major ninth)
	minor-ninth (minor-seventh, major ninth)
11ths (usually as the basis for alteration):
	dominant-11th (dominant-ninth, perfect 11th)
	major-11th (major-ninth, perfect 11th)
	minor-11th (minor-ninth, perfect 11th)
13ths (usually as the basis for alteration):
	dominant-13th (dominant-11th, major 13th)
	major-13th (major-11th, major 13th)
	minor-13th (minor-11th, major 13th)
Suspended:
	suspended-second (major second, perfect fifth)
	suspended-fourth (perfect fourth, perfect fifth)
Functional sixths:
	Neapolitan
	Italian
	French
	German
Other:
	pedal (pedal-point bass)
	power (perfect fifth)
	Tristan

The "other" kind is used when the harmony is entirely composed of add elements. The "none" kind is used to explicitly encode absence of chords or functional harmony."#;
    const KIND_EXPECTED: &str = r#"        /// A kind-value indicates the type of chord. Degree elements can then add, subtract, or
        /// alter from these starting points. Values include:
        ///
        /// Triads:
        ///     major (major third, perfect fifth)
        ///     minor (minor third, perfect fifth)
        ///     augmented (major third, augmented fifth)
        ///     diminished (minor third, diminished fifth)
        /// Sevenths:
        ///     dominant (major triad, minor seventh)
        ///     major-seventh (major triad, major seventh)
        ///     minor-seventh (minor triad, minor seventh)
        ///     diminished-seventh (diminished triad, diminished seventh)
        ///     augmented-seventh (augmented triad, minor seventh)
        ///     half-diminished (diminished triad, minor seventh)
        ///     major-minor (minor triad, major seventh)
        /// Sixths:
        ///     major-sixth (major triad, added sixth)
        ///     minor-sixth (minor triad, added sixth)
        /// Ninths:
        ///     dominant-ninth (dominant-seventh, major ninth)
        ///     major-ninth (major-seventh, major ninth)
        ///     minor-ninth (minor-seventh, major ninth)
        /// 11ths (usually as the basis for alteration):
        ///     dominant-11th (dominant-ninth, perfect 11th)
        ///     major-11th (major-ninth, perfect 11th)
        ///     minor-11th (minor-ninth, perfect 11th)
        /// 13ths (usually as the basis for alteration):
        ///     dominant-13th (dominant-11th, major 13th)
        ///     major-13th (major-11th, major 13th)
        ///     minor-13th (minor-11th, major 13th)
        /// Suspended:
        ///     suspended-second (major second, perfect fifth)
        ///     suspended-fourth (perfect fourth, perfect fifth)
        /// Functional sixths:
        ///     Neapolitan
        ///     Italian
        ///     French
        ///     German
        /// Other:
        ///     pedal (pedal-point bass)
        ///     power (perfect fifth)
        ///     Tristan
        ///
        /// The "other" kind is used when the harmony is entirely composed of add elements. The
        /// "none" kind is used to explicitly encode absence of chords or functional harmony."#;

    #[test]
    fn test_kind_documentation() {
        let got = documentation(KIND_DOCUMENTATION, 2).unwrap();
        let want = KIND_EXPECTED;
        assert_eq!(got, want);
    }
}
