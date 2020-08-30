mod xsd;

use crate::error::Result;
use crate::xsd::Xsd;
use exile::Element;
use flate2::read::GzDecoder;
use lazy_static::lazy_static;
use pathdiff::diff_paths;
use reqwest::blocking::Client;
use std::fs::{read_to_string, File};
use std::path::{Path, PathBuf};
use std::time::Duration;
use tar::Archive;
use url::Url;

const TESTSUITE_URL: &str =
    "https://www.w3.org/XML/2004/xml-schema-test-suite/xmlschema2006-11-06/xsts-2007-06-20.tar.gz";

const TESTSUITE_TAR: &str = "xsts-2007-06-20.tar.gz";
const TESTSUITE_DIRNAME: &str = "xmlschema2006-11-06";

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct TestID {
    pub suite: TestSuite,
    pub set: String,
    pub group: String,
    pub name: String,
}

#[derive(Debug, Clone)]
pub struct Test {
    pub id: TestID,
    pub test_type: TestType,
    pub validity: Validity,
    pub document: PathBuf,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Validity {
    Valid,
    Invalid,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum TestType {
    Schema,
    Instance,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum TestSuite {
    Boeing,
    Ms,
    Nist,
    Sun,
}

lazy_static! {
    static ref DATA_DIR: PathBuf = {
        let p_str = env!("CARGO_MANIFEST_DIR");
        let p = PathBuf::from(p_str).join("data");
        std::fs::create_dir_all(&p).unwrap();
        p
    };
}

lazy_static! {
    static ref TESTSUITE_DIR: PathBuf = { data_dir().join(TESTSUITE_DIRNAME) };
}

lazy_static! {
    static ref XSD_TESTS: Vec<Test> = {
        if !data_dir().is_dir() {
            std::fs::create_dir_all(data_dir()).unwrap();
        }
        if !testsuite_dir().is_dir() || !tarball_path().is_file() {
            let _ = std::fs::remove_dir_all(testsuite_dir());
            let _ = std::fs::remove_file(testsuite_dir());
            let _ = std::fs::remove_dir_all(tarball_path());
            let _ = std::fs::remove_file(tarball_path());
            download().unwrap();
            unpack().unwrap();
        }
        find_suite_definition_files().unwrap()
    };
}

fn xsd_tests() -> &'static [Test] {
    &XSD_TESTS
}

/// Provide the path to the data directory
fn data_dir() -> &'static PathBuf {
    &DATA_DIR
}

fn testsuite_dir() -> &'static PathBuf {
    &TESTSUITE_DIR
}

fn tarball_exists() -> bool {
    tarball_path().is_file()
}

fn tarball_path() -> PathBuf {
    data_dir().join(TESTSUITE_TAR)
}

/// Download the tarball
fn download() -> Result<()> {
    // TODO - check if tarball already exists
    let url = Url::parse(TESTSUITE_URL)
        .map_err(|err| make_err!("error parsing url '{}': {:?}", TESTSUITE_URL, err))?;
    let bytes = http_get(url)?;
    wrap!(std::fs::write(tarball_path(), bytes.as_slice()))
}

/// Unpack the Tarball
fn unpack() -> Result<()> {
    let path = tarball_path();
    if !tarball_exists() {
        return raise!("tarball does not exist at '{}'", path.display());
    }

    let tar_gz =
        File::open(&path).map_err(|err| make_err!("unable to open file '{}'", path.display()))?;
    let tar = GzDecoder::new(tar_gz);
    let mut archive = Archive::new(tar);
    wrap!(archive.unpack(data_dir()))?;
    Ok(())
}

/// Walk the Dir for Paths
fn walk_dir() {}

fn http_get(url: Url) -> Result<Vec<u8>> {
    //debug!("sending: {}", url.as_str());
    let client = Client::builder()
        .timeout(Duration::from_secs(10))
        .build()
        .map_err(|err| make_err!("error creating reqwest client: {:?}", err))?;
    let response = client
        .get(url.clone())
        .send()
        .map_err(|err| make_err!("error getting reqwest response: {:?}", err))?;
    let response = response
        .error_for_status()
        .map_err(|err| make_err!("http status error: {:?}", err))?;
    let reponse_bytes = response
        .bytes()
        .map_err(|err| make_err!("error getting response bytes: {:?}", err))?;
    Ok(reponse_bytes.to_vec())
}

fn find_suite_definition_files() -> Result<Vec<Test>> {
    let suite_path = testsuite_dir().join("suite.xml");
    let suite_doc = wrap!(exile::load(&suite_path))?;
    let list = suite_doc.root().children();
    let mut the_many_tests = Vec::new();
    for element in list {
        if element.name == "testSetRef" {
            if let Some(path_str) = element.attributes.map().get("xlink:href") {
                if path_str.ends_with("testSet") {
                    let mut tests = find_tests_in_test_set(path_str)?;
                    the_many_tests.append(&mut tests);
                }
            }
        }
    }
    Ok(the_many_tests)
}

fn find_tests_in_test_set(path_str: &str) -> Result<Vec<Test>> {
    // TODO - won't work on Windows?
    let test_set_path = testsuite_dir()
        .join(path_str)
        .canonicalize()
        .map_err(|err| {
            make_err!(
                "unable to canonicalize '{}'",
                testsuite_dir().join(path_str).display()
            )
        })?;
    let mut suite_dir = test_set_path
        .parent()
        .ok_or_else(|| make_err!("unable to get parent of '{}'", test_set_path.display()))?
        .canonicalize()
        .map_err(|err| {
            make_err!(
                "unable to canonicalize parent of '{}'",
                test_set_path.display()
            )
        })?;
    let dirname = suite_dir
        .file_name()
        .ok_or_else(|| make_err!("could not get suite directory"))?
        .to_str()
        .ok_or_else(|| make_err!("not utf-8"))?;
    let suite = if dirname.starts_with("boeing") {
        TestSuite::Boeing
    } else if dirname.starts_with("ms") {
        TestSuite::Ms
    } else if dirname.starts_with("nist") {
        TestSuite::Ms
    } else if dirname.starts_with("sun") {
        TestSuite::Ms
    } else {
        return raise!("unknown test suite directory '{}'", dirname);
    };
    let test_set_doc = wrap!(exile::load(&test_set_path))?;
    let root = test_set_doc.root();
    if root.name != "testSet" {
        return raise!("wrong element: '{}", root.name);
    }
    let mut tests = Vec::new();
    let set_name = root
        .attributes
        .map()
        .get("name")
        .ok_or_else(|| make_err!("no name"))?;
    for test_group_element in root.children() {
        if test_group_element.name != "testGroup" {
            continue;
        }
        let group_name = test_group_element
            .attributes
            .map()
            .get("name")
            .ok_or_else(|| make_err!("no name"))?;
        for test_element in test_group_element.children() {
            match test_element.name.as_str() {
                "schemaTest" | "instanceTest" => tests.push(parse_test_element(
                    suite,
                    &suite_dir,
                    set_name,
                    group_name,
                    test_element,
                )?),
                _ => continue,
            }
        }
    }
    Ok(tests)
}

fn parse_test_element(
    suite: TestSuite,
    suite_dir: &Path,
    set_name: &str,
    group_name: &str,
    element: &Element,
) -> Result<Test> {
    let test_type = match element.name.as_str() {
        "schemaTest" => TestType::Schema,
        "instanceTest" => TestType::Instance,
        s => return raise!("unexpected '{}'", s),
    };
    let name = element
        .attributes
        .map()
        .get("name")
        .ok_or_else(|| make_err!("no test name"))?;
    let mut validity = None;
    let mut document = None;
    for child in element.children() {
        match child.name.as_str() {
            "expected" => {
                validity = Some(
                    match child
                        .attributes
                        .map()
                        .get("validity")
                        .ok_or_else(|| make_err!("validity attribute missing"))?
                        .as_str()
                    {
                        "valid" => Validity::Valid,
                        "invalid" => Validity::Invalid,
                        s => return raise!("validity value of '{}' not understood", s),
                    },
                );
            }
            "instanceDocument" | "schemaDocument" => {
                let document_str = child
                    .attributes
                    .map()
                    .get("xlink:href")
                    .ok_or_else(|| make_err!("href not found"))?;
                document = Some(suite_dir.join(document_str))
            }
            _ => continue,
        }
    }
    let document_unwrap = document.ok_or_else(|| make_err!("document href property not found"))?;
    let document = document_unwrap.clone().canonicalize().map_err(|e| {
        make_err!(
            "unable to canonicalize '{}', does it exist?: {:?}",
            document_unwrap.display(),
            e
        )
    })?;
    if !document.is_file() {
        return raise!("somehow not a file? '{}'", document.display());
    }
    Ok(Test {
        id: TestID {
            suite,
            set: set_name.into(),
            group: group_name.into(),
            name: name.clone(),
        },
        test_type,
        validity: validity.ok_or_else(|| make_err!("validity property not found"))?,
        document,
    })
}

#[test]
fn generate_tests() {
    // let d = data_dir();
    // download().unwrap();
    // unpack().unwrap();
    let dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"))
        .join("src")
        .join("schematest")
        .join("xsd")
        .canonicalize()
        .unwrap();

    let tests = xsd_tests();
    for test in tests {
        let xsd = Xsd::load(&test.document).unwrap();
        println!("{:?}", xsd)
    }
}
