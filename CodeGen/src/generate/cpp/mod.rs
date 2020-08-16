pub mod constants;
mod cpp_template;
mod helpers;
pub mod modeler;
mod write_custom;
mod write_enums;
mod write_numerics;
mod write_unions;
pub mod writer;

use crate::error::Result;
use std::path::Path;

fn check_file_exists<P: AsRef<Path>>(path: P) -> Result<()> {
    if !path.as_ref().is_file() {
        return raise!("Expected file at '{}'", path.as_ref().display());
    }
    Ok(())
}
