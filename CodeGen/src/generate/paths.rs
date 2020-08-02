use std::fs::{remove_file, File, OpenOptions};
use std::path::{Path, PathBuf};

pub fn repo() -> PathBuf {
    let mut p = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    p.pop();
    p.canonicalize().unwrap()
}

#[derive(Clone, Debug)]
pub struct Paths {
    pub repo: PathBuf,
    pub core: PathBuf,
    pub enums_h: PathBuf,
    pub enums_cpp: PathBuf,
    pub integers_h: PathBuf,
    pub integers_cpp: PathBuf,
    pub decimals_h: PathBuf,
    pub decimals_cpp: PathBuf,
    pub xsd_3_0: PathBuf,
    pub xsd_3_1: PathBuf,
}

impl Default for Paths {
    fn default() -> Self {
        let r = repo();
        let core = r.join("Sourcecode").join("private").join("mx").join("core");
        Self {
            repo: r,
            core: core.clone(),
            enums_h: core.join("Enums.h"),
            enums_cpp: core.join("Enums.cpp"),
            integers_h: core.join("Integers.h"),
            integers_cpp: core.join("Integers.cpp"),
            decimals_h: core.join("Decimals.h"),
            decimals_cpp: core.join("Decimals.cpp"),
            xsd_3_0: repo().join("Documents").join("musicxml.xsd"),
            xsd_3_1: repo().join("Documents").join("musicxml-3.1.xsd"),
        }
    }
}

fn open_file<P: AsRef<Path>>(p: P) -> File {
    println!("opening a file for writing: '{}'", p.as_ref().display());
    OpenOptions::new().read(true).write(true).create(true).open(p).unwrap() // TODO - error?
}

impl Paths {
    pub fn create_core_file<S: AsRef<str>>(&self, filename: S) -> File {
        let p = self.core.join(filename.as_ref());
        if p.is_file() {
            remove_file(&p).unwrap();
        }
        open_file(p)
    }
}
