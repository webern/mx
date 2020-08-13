use crate::error::Result;
use crate::generate::cpp::write_custom::{
    write_color, write_comma_separated_text, write_ending_number, write_time_only,
};
use crate::generate::paths::Paths;
use crate::model::scalar::ScalarNumeric;
use crate::model::Model;

#[derive(Debug, Clone)]
pub struct Writer {
    pub models: Vec<Model>,
    pub paths: Paths,
}

impl Writer {
    pub fn new(models: Vec<Model>) -> Self {
        Self {
            models,
            paths: Paths::default(),
        }
    }

    pub fn write_code(&self) -> Result<()> {
        let mut enums = Vec::new();
        let mut decimals = Vec::new();
        let mut integers = Vec::new();
        let mut unions = Vec::new();
        for model in &self.models {
            match model {
                Model::Enumeration(e) => enums.push(e),
                Model::ScalarString(s) => {
                    // We handle all of the scalar strings that we know of in musicxml.xsd with
                    // custom implementations, thus there is currently no 'generic' implementation.
                    return raise!(
                        "Encountered ScalarString '{}', no handler available.",
                        s.name.original()
                    );
                }
                Model::CustomScalarString(cs) => match cs.name.original() {
                    "color" => write_color(cs, &self.paths)?,
                    "comma-separated-text" => write_comma_separated_text(cs, &self.paths)?,
                    "time-only" => write_time_only(cs, &self.paths)?,
                    "ending-number" => write_ending_number(cs, &self.paths)?,
                    unhandled => return raise!("Unhandled CustomScalarString '{}'", unhandled),
                },
                Model::ScalarNumber(sn) => match sn {
                    ScalarNumeric::Decimal(d) => decimals.push(d.to_owned()),
                    ScalarNumeric::Integer(i) => integers.push(i.to_owned()),
                },
                Model::DerivedSimpleType(derived_from) => {
                    return raise!("DerivedSimpleTypes not implemented: '{}'", derived_from.name.original())
                }
                Model::UnionSimpleType(u) => unions.push(u),
            }
        }
        self.write_enums(&mut enums)?;
        self.write_integers(integers)?;
        self.write_decimals(decimals)?;
        self.write_unions(unions.as_slice())?;
        Ok(())
    }
}
