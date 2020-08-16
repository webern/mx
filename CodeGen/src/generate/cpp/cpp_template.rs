use crate::error::Result;
use crate::generate::template;
use crate::generate::template::{render, CORE_CPP, CORE_H};
use std::borrow::Cow;
use std::collections::HashMap;

pub(super) fn render_core_h<S>(
    contents: S,
    lib_includes: Option<&mut [&str]>,
    std_includes: Option<&mut [&str]>,
) -> Result<String>
where
    S: AsRef<str>,
{
    let mut map = HashMap::new();
    if let Some(includes) = lib_includes {
        if !includes.is_empty() {
            includes.sort();
            let mut rendered = String::new();
            rendered.push('\n');
            for include in includes {
                rendered.push('\n');
                rendered.push_str(format!("#include \"{}\"", include).as_str());
            }
            map.insert("lib_includes", rendered);
        }
    }
    if let Some(includes) = std_includes {
        if !includes.is_empty() {
            includes.sort();
            let mut rendered = String::new();
            rendered.push('\n');
            for include in includes {
                rendered.push('\n');
                rendered.push_str(format!("#include <{}>", include).as_str());
            }
            map.insert("std_includes", rendered);
        }
    }
    map.insert("contents", contents.as_ref().to_owned());
    render(CORE_H, &map)
}

pub(super) fn render_core_cpp<S1, S2>(
    contents: S1,
    self_include: Option<S2>,
    lib_includes: Option<&mut [&str]>,
    std_includes: Option<&mut [&str]>,
) -> Result<String>
where
    S1: AsRef<str>,
    S2: AsRef<str>,
{
    let mut map = HashMap::new();
    if self_include.is_some() || lib_includes.is_some() || std_includes.is_some() {
        map.insert("lead_whitespace", "\n\n".to_owned());
    }
    if let Some(include) = self_include {
        let include = include.as_ref();
        let mut rendered = String::new();
        rendered.push_str(format!("// self\n#include \"{}\"", include).as_str());
        map.insert("self_include", rendered);
    }
    if let Some(includes) = lib_includes {
        if !includes.is_empty() {
            includes.sort();
            let mut rendered = String::new();
            rendered.push_str("\n\n// lib");
            for include in includes {
                rendered.push('\n');
                rendered.push_str(format!("#include \"{}\"", include).as_str());
            }
            map.insert("lib_includes", rendered);
        }
    }
    if let Some(includes) = std_includes {
        if !includes.is_empty() {
            includes.sort();
            let mut rendered = String::new();
            rendered.push_str("\n\n// std");
            for include in includes {
                rendered.push('\n');
                rendered.push_str(format!("#include <{}>", include).as_str());
            }
            map.insert("std_includes", rendered);
        }
    }
    map.insert("contents", contents.as_ref().to_owned());
    render(CORE_CPP, &map)
}
