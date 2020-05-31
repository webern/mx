use crate::error::Result;
use std::fs::{read_to_string, File};
use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::{Command, Output, Stdio};
use tempfile::TempDir;
use uuid::Uuid;

pub(crate) fn compile_mx<P: AsRef<Path>>(path_to_mx: P) -> Result<()> {
    let tempdir = wrap!(TempDir::new())?;
    let _ = run(
        Command::new("cmake")
            .arg(path_to_mx.as_ref())
            .arg("-DMX_BUILD_TESTS=on")
            .arg("-DMX_BUILD_EXAMPLES=on")
            .arg("-DMX_BUILD_CORE_TESTS=off"),
        tempdir.path(),
    )?;
    let _ = run(Command::new("make").arg("-j9"), tempdir.path())?;
    Ok(())
}

fn run_mx_tests() -> Result<()> {
    Ok(())
}

fn run<P: AsRef<Path>>(cmd: &mut Command, dir: P) -> Result<(String, Output)> {
    let u = Uuid::new_v4();
    let opath = dir.as_ref().join(format!("combined_output.log.{}", u));
    let ofile = wrap!(File::create(&opath))?;
    let efile = wrap!(ofile.try_clone())?;
    let result = wrap!(cmd
        .current_dir(&dir.as_ref())
        .stdout(Stdio::from(ofile))
        .stderr(Stdio::from(efile))
        .spawn())?
    .wait_with_output();
    let output = wrap!(result)?;
    let outstr = wrap!(read_to_string(&opath))?;
    if !output.status.success() {
        return raise!("Command failed '{:?}':\n{}", &cmd, outstr);
    }
    println!("Command output '{:?}':\n{}", &cmd, outstr);
    Ok((outstr, output))
}
