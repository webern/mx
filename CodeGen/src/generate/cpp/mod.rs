mod cpp_template;
use crate::error::Result;
use std::path::Path;
pub mod constants;
pub mod modeler;
mod write_custom;
mod write_enums;
mod write_numerics;
pub mod writer;

fn check_file_exists<P: AsRef<Path>>(path: P) -> Result<()> {
    if !path.as_ref().is_file() {
        return raise!("Expected file at '{}'", path.as_ref().display());
    }
    Ok(())
}
