use crate::generate::string_stuff::tokenize;
use std::borrow::Cow;

#[derive(Debug, Clone, Eq, PartialEq, Ord, PartialOrd)]
pub struct Symbol {
    original: String,
    renamed_to: String,
    snake: String,
    screaming_snake: String,
    camel: String,
    pascal: String,
}

impl Symbol {
    pub fn original(&self) -> &str {
        self.original.as_str()
    }

    pub fn renamed_to(&self) -> &str {
        self.renamed_to.as_str()
    }

    pub fn screaming_snake(&self) -> &str {
        self.screaming_snake.as_str()
    }

    pub fn snake(&self) -> &str {
        self.snake.as_str()
    }

    pub fn camel(&self) -> &str {
        self.camel.as_str()
    }

    pub fn pascal(&self) -> &str {
        self.pascal.as_str()
    }

    pub fn new<S>(original: S) -> Self
    where
        S: AsRef<str>,
    {
        let tokens = tokenize(original.as_ref());
        Self::new_with_tokens(original, tokens.as_slice())
    }

    pub fn new_with_tokens<S>(original: S, tokenized: &[String]) -> Self
    where
        S: AsRef<str>,
    {
        let snake: String = tokenized.join("_").to_lowercase();
        let screaming_snake = snake.to_uppercase();
        let pascal = tokenized
            .iter()
            .map(|s| {
                let mut c = s.chars();
                match c.next() {
                    None => String::new(),
                    Some(f) => f.to_uppercase().chain(c).collect(),
                }
            })
            .collect::<Vec<String>>()
            .join("");
        let camel = {
            let mut c = pascal.chars();
            match c.next() {
                None => String::new(),
                Some(f) => f.to_lowercase().chain(c).collect(),
            }
        };
        Self {
            original: original.as_ref().to_owned(),
            renamed_to: original.as_ref().to_owned(),
            snake,
            screaming_snake,
            camel,
            pascal,
        }
    }
}

#[test]
fn symbol_1() {
    let original = "bones the cat";
    let tokenized = vec!["bones", "the", "cat"];
    let symbol = Symbol::new(original);
    assert_eq!(symbol.original(), original);
    assert_eq!(symbol.renamed_to(), original);
    assert_eq!(symbol.snake(), "bones_the_cat");
    assert_eq!(symbol.screaming_snake(), "BONES_THE_CAT");
    assert_eq!(symbol.camel(), "bonesTheCat");
    assert_eq!(symbol.pascal(), "BonesTheCat");
}
