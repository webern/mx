use crate::error::Result;
use crate::generate::cpp::write_custom::{
    write_color, write_comma_separated_text, write_ending_number, write_time_only,
};
use crate::generate::paths::Paths;
use crate::model::scalar::ScalarNumeric;
use crate::model::{Def, Model};

#[derive(Debug, Clone)]
pub struct Writer {
    pub model: Model,
    pub paths: Paths,
}

impl Writer {
    pub fn new(model: Model) -> Self {
        Self {
            model,
            paths: Paths::default(),
        }
    }

    pub fn write_code(&self) -> Result<()> {
        let mut enums = Vec::new();
        let mut decimals = Vec::new();
        let mut integers = Vec::new();
        let mut unions = Vec::new();
        for model in self.model.defs() {
            match model {
                Def::Enumeration(e) => enums.push(e),
                Def::ScalarString(s) => {
                    // We handle all of the scalar strings that we know of in musicxml.xsd with
                    // custom implementations, thus there is currently no 'generic' implementation.
                    return raise!(
                        "Encountered ScalarString '{}', no handler available.",
                        s.name.original()
                    );
                }
                Def::CustomScalarString(cs) => match cs.name.original() {
                    "color" => write_color(cs, &self.paths)?,
                    "comma-separated-text" => write_comma_separated_text(cs, &self.paths)?,
                    "time-only" => write_time_only(cs, &self.paths)?,
                    "ending-number" => write_ending_number(cs, &self.paths)?,
                    unhandled => return raise!("Unhandled CustomScalarString '{}'", unhandled),
                },
                Def::ScalarNumber(sn) => match sn {
                    ScalarNumeric::Decimal(d) => decimals.push(d.to_owned()),
                    ScalarNumeric::Integer(i) => integers.push(i.to_owned()),
                },
                Def::DerivedSimpleType(derived_from) => {
                    return raise!(
                        "DerivedSimpleTypes not implemented: '{}'",
                        derived_from.name.original()
                    )
                }
                Def::UnionSimpleType(u) => unions.push(u),
            }
        }
        self.write_enums(&mut enums)?;
        self.write_integers(integers)?;
        self.write_decimals(decimals)?;
        self.write_unions(unions.as_slice())?;
        Ok(())
    }
}
