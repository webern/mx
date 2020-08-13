use crate::error::Result;
use handlebars::Handlebars;
use lazy_static::lazy_static;
use serde::Serialize;
use std::borrow::Borrow;
use std::collections::BTreeMap;
use std::collections::HashMap;
use std::fs;
use std::path::{Path, PathBuf};

pub const CORE_CPP: &str = "core.cpp.template";
pub const CORE_H: &str = "core.h.template";
pub const ENUM_CPP: &str = "enum.cpp.template";
pub const ENUM_H: &str = "enum.h.template";
pub const INTEGER_BUILTINS_CPP: &str = "integer_builtins.cpp.template";
pub const INTEGER_BUILTINS_H: &str = "integer_builtins.h.template";
pub const INTEGER_TYPE_CPP: &str = "integer_type.cpp.template";
pub const INTEGER_TYPE_H: &str = "integer_type.h.template";
pub const DECIMAL_BUILTINS_CPP: &str = "decimal_builtins.cpp.template";
pub const DECIMAL_BUILTINS_H: &str = "decimal_builtins.h.template";
pub const DECIMAL_TYPE_CPP: &str = "decimal_type.cpp.template";
pub const DECIMAL_TYPE_H: &str = "decimal_type.h.template";

lazy_static! {
    static ref DIR: PathBuf = manifest_dir().join("src").join("generate").join("data");
}

lazy_static! {
    static ref HANDLEBARS: Handlebars<'static> = {
        let mut handlebars = Handlebars::new();
        let files = list_template_files().unwrap();
        for (name, path) in &files {
            handlebars.register_template_string(name, read(path).unwrap());
        }
        handlebars
    };
}

lazy_static! {
    pub(crate) static ref NO_DATA: HashMap<String, String> = HashMap::new();
}

pub(crate) fn render<STR, SERIALIZE>(name: STR, data: &SERIALIZE) -> Result<String>
where
    STR: AsRef<str>,
    SERIALIZE: Serialize,
{
    let h = &HANDLEBARS;
    let rendered = h
        .render(name.as_ref(), data)
        .map_err(|e| make_err!("unable to render template '{}': {}", name.as_ref(), e))?;
    Ok(rendered
        .replace("&amp;", "&")
        .replace("&lt;", "<")
        .replace("&gt;", ">")
        .replace("&quot;", "\""))
}

fn template_dir() -> &'static Path {
    &DIR
}

fn manifest_dir() -> PathBuf {
    let dir = env!("CARGO_MANIFEST_DIR");
    PathBuf::from(dir)
}

fn template_path<S: AsRef<str>>(filename: S) -> PathBuf {
    template_dir().join(filename.as_ref())
}

fn read<P: AsRef<Path>>(p: P) -> Result<String> {
    let p = p.as_ref();
    if !p.is_file() {
        return raise!("not a file: '{}'", p.display());
    }
    Ok(std::fs::read_to_string(p).map_err(|e| make_err!("cannot read file '{}': {}", p.display(), e))?)
}

fn list_template_files() -> Result<Vec<(String, PathBuf)>> {
    let dir = template_dir();
    let dir = dir
        .clone()
        .canonicalize()
        .map_err(|e| make_err!("directory not found '{}': {}", dir.display(), e))?;
    if !dir.is_dir() {
        return raise!("not a directory: '{}'", dir.display())?;
    }
    let mut list = Vec::new();
    for entry in fs::read_dir(&dir).map_err(|e| make_err!("unable to read dir '{}': {}", dir.display(), e))? {
        let entry = entry.map_err(|e| make_err!("unable to read entry in dir '{}': {}", dir.display(), e))?;
        if !entry.path().is_file() {
            continue;
        }
        let path = entry.path();
        let ext = if let Some(ext) = path.extension() {
            if let Some(ext) = ext.to_str() {
                ext
            } else {
                return raise!("not utf-8: '{}'", entry.path().display());
            }
        } else {
            continue;
        };
        if ext != "template" {
            continue;
        }
        let filename = path
            .file_name()
            .ok_or_else(|| make_err!("no filename: '{}'", entry.path().display()))?;
        let filename = filename
            .to_str()
            .ok_or_else(|| make_err!("not utf-8: '{:?}'", filename))?;
        list.push((filename.to_owned(), path))
    }
    Ok(list)
}
