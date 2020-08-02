mod compile_mx;
pub mod cpp;
pub mod paths;
mod template;

use crate::error::Result;
use crate::generate::cpp::modeler::MxModeler;
use crate::generate::paths::Paths;
use crate::model::create::Create;
use crate::model::creator::Creator;
use crate::model::post_process::PostProcess;
use crate::model::transform::Transform;
use crate::xsd::Xsd;
use cpp::constants::enum_member_substitutions;
use cpp::constants::{pseudo_enums, reserved_words, suffixed_enum_names};
use std::fs::read_to_string;
use std::sync::Arc;

#[derive(Clone, Debug)]
pub struct GenArgs {
    pub paths: Paths,
}

impl Default for GenArgs {
    fn default() -> Self {
        Self { paths: Paths::default() }
    }
}

/// Generate `mx::core` in C++
pub fn run(args: GenArgs) -> Result<()> {
    let xsd = read_to_string(&args.paths.xsd_3_0).unwrap();
    let doc = exile::parse(xsd.as_str()).unwrap();
    let new_xsd = Xsd::load(&args.paths.xsd_3_0)?;
    let transforms: Vec<Box<dyn Transform>> = vec![Box::new(MxModeler::new())];
    let creates: Vec<Box<dyn Create>> = vec![Box::new(MxModeler::new())];
    let post_processors: Vec<Box<dyn PostProcess>> = vec![Box::new(MxModeler::new())];
    let creator = Creator::new_with_default(Some(transforms), Some(creates), Some(post_processors));
    let models = creator.create(&new_xsd)?;
    let cpp_writer = cpp::writer::Writer::new(models);
    cpp_writer.write_code()?;
    Ok(())
}
