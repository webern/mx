#[cfg(test)]
mod tests;
use crate::error::Result;
use crate::xsd::Xsd;
use exile::Element;
use flate2::read::GzDecoder;
use lazy_static::lazy_static;
use pathdiff::diff_paths;
use reqwest::blocking::Client;
use std::collections::HashMap;
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
pub struct Tests {
    list: Vec<Test>,
    map: HashMap<String, usize>,
}

impl Tests {
    fn new(list: Vec<Test>) -> Self {
        let mut list = list;
        let mut map_temp = HashMap::new();
        for test in list.iter_mut() {
            let smallest = test.id.smallest();
            let bigger = test.id.bigger();
            let biggest = test.id.biggest();
            if !map_temp.contains_key(smallest.as_str()) {
                test.unique_symbol = smallest.clone();
            } else if !map_temp.contains_key(bigger.as_str()) {
                test.unique_symbol = bigger.clone();
            // println!("warning: bigger was used for '{}'", test.id.name);
            } else {
                test.unique_symbol = biggest.clone();
                // println!("warning: biggest was used for '{}'", test.id.name);
            }
            map_temp.insert(test.unique_symbol.as_str(), ());
        }
        let mut map = HashMap::new();
        for (i, test) in list.iter().enumerate() {
            if map.insert(test.unique_symbol.clone(), i).is_some() {
                panic!("this should be impossible");
            }
        }
        Self { list, map }
    }

    pub fn tests(&self) -> &Vec<Test> {
        &self.list
    }

    pub fn map(&self) -> &HashMap<String, usize> {
        &self.map
    }

    pub fn get<S: AsRef<str>>(&self, id: S) -> Option<&Test> {
        if let Some(index) = self.map.get(id.as_ref()) {
            self.list.get(*index)
        } else {
            None
        }
    }
}

pub fn snake<S: AsRef<str>>(s: S) -> String {
    let mut o = String::new();
    for c in s.as_ref().chars() {
        if c.is_alphanumeric() {
            o.push_str(c.to_lowercase().to_string().as_str());
        } else {
            o.push('_');
        }
    }
    o
}

impl TestID {
    pub fn snake(&self) -> String {
        snake(format!("{:?}", self))
    }
}

#[derive(Debug, Clone)]
pub struct Test {
    pub id: TestID,
    pub test_type: TestType,
    pub validity: Validity,
    pub document: PathBuf,
    pub unique_symbol: String,
}

impl Validity {
    pub fn snake(&self) -> String {
        snake(format!("{:?}", self))
    }
}

impl TestType {
    pub fn snake(&self) -> String {
        snake(format!("{:?}", self))
    }
}

impl TestID {
    pub fn snake_suite(&self) -> String {
        self.suite.snake()
    }

    pub fn snake_set(&self) -> String {
        snake(&self.set)
    }

    pub fn snake_group(&self) -> String {
        snake(&self.group)
    }

    pub fn snake_name(&self) -> String {
        snake(&self.name)
    }

    pub fn smallest(&self) -> String {
        format!("{}_{}", self.snake_suite(), self.snake_name())
    }

    pub fn bigger(&self) -> String {
        format!(
            "{}_{}_{}",
            self.snake_suite(),
            self.snake_set(),
            self.snake_name(),
        )
    }

    pub fn biggest(&self) -> String {
        format!(
            "{}_{}_{}_{}",
            self.snake_suite(),
            self.snake_set(),
            self.snake_group(),
            self.snake_name(),
        )
    }
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

impl TestSuite {
    fn snake(&self) -> String {
        snake(format!("{:?}", self))
    }
}

lazy_static! {
    static ref DATA_DIR: PathBuf = {
        let p_str = env!("CARGO_MANIFEST_DIR");
        PathBuf::from(p_str).join("data")
    };
}

lazy_static! {
    static ref TESTSUITE_DIR: PathBuf = { data_dir().join(TESTSUITE_DIRNAME) };
}

lazy_static! {
    static ref XSD_TESTS: Tests = {
        if !data_dir().is_dir() {
            std::fs::create_dir_all(data_dir()).unwrap();
        }
        ensure_test_data();
        let list = find_suite_definition_files().unwrap();
        Tests::new(list)
    };
}

pub(crate) fn ensure_test_data() {
    if !testsuite_dir().is_dir() || !tarball_path().is_file() {
        let _ = std::fs::remove_dir_all(testsuite_dir());
        let _ = std::fs::remove_file(testsuite_dir());
        let _ = std::fs::remove_dir_all(tarball_path());
        let _ = std::fs::remove_file(tarball_path());
        download().unwrap();
        unpack().unwrap();
    }
}

fn xsd_tests() -> &'static Tests {
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
        TestSuite::Nist
    } else if dirname.starts_with("sun") {
        TestSuite::Sun
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
        unique_symbol: String::from(""),
    })
}

#[test]
#[ignore]
fn generate_tests() {
    use pathdiff::diff_paths;
    use std::io::Write;
    let dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"))
        .join("src")
        .join("schematest")
        .canonicalize()
        .unwrap();
    let file = dir.join("tests.rs");

    let _ = std::fs::remove_file(&file);
    let mut file = File::create(&file).unwrap();
    writeln!(file, "{}", HEADER).unwrap();
    let tests = xsd_tests();
    for test in tests.tests() {
        if test.validity != Validity::Valid || test.test_type != TestType::Schema {
            continue;
        }
        let rel = diff_paths(&test.document, &dir).unwrap();
        writeln!(file, "{}", make_test(test, &rel)).unwrap();
    }
}

const HEADER: &str = r#"
use crate::xsd::Xsd;
use lazy_static::lazy_static;
use std::path::PathBuf;

lazy_static! {
    static ref DIR: PathBuf = {
        let manifest_dir = env!("CARGO_MANIFEST_DIR");
        PathBuf::from(manifest_dir).join("src").join("schematest")
    };
}

fn to(rel: &str) -> PathBuf {
    let un = (&DIR).join(rel);
    un.canonicalize().unwrap_or_else(|err| {
        panic!("unable to canonicalize '{}'", un.display());
    })
}
"#;

fn make_test(test: &Test, rel: &Path) -> String {
    format!(
        r#"#[test]
fn {}() {{
    let path = to("{}");
    let _ = Xsd::load(PathBuf::from(&path)).unwrap();
}}"#,
        &test.unique_symbol,
        rel.display(),
    )
}
