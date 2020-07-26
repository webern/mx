use crate::error::Result;
use crate::generate::cpp::check_file_exists;
use crate::generate::paths::Paths;
use crate::model::scalar_string::ScalarString;

pub(super) fn write_color(model: &ScalarString, paths: &Paths) -> Result<()> {
    if model.name.original() != "color" {
        return raise!("expected 'color' got '{}'", model.name.original());
    }
    check_file_exists(paths.core.join("Color.h"))?;
    check_file_exists(paths.core.join("Color.cpp"))?;
    Ok(())
}

pub(super) fn write_comma_separated_text(model: &ScalarString, paths: &Paths) -> Result<()> {
    let expected = "comma-separated-text";
    if model.name.original() != expected {
        return raise!("expected '{}' got '{}'", expected, model.name.original());
    }
    check_file_exists(paths.core.join("CommaSeparatedText.h"))?;
    check_file_exists(paths.core.join("CommaSeparatedText.cpp"))?;
    Ok(())
}

pub(super) fn write_time_only(model: &ScalarString, paths: &Paths) -> Result<()> {
    let expected = "time-only";
    if model.name.original() != expected {
        return raise!("expected '{}' got '{}'", expected, model.name.original());
    }
    check_file_exists(paths.core.join("TimeOnly.h"))?;
    // no cpp file, alias only
    Ok(())
}

pub(super) fn write_ending_number(model: &ScalarString, paths: &Paths) -> Result<()> {
    let expected = "ending-number";
    if model.name.original() != expected {
        return raise!("expected '{}' got '{}'", expected, model.name.original());
    }
    check_file_exists(paths.core.join("TimeOnly.h"))?;
    // no cpp file, alias only
    Ok(())
}
