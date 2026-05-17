use libmxgen::generate::paths::Paths;
use libmxgen::xsd::Xsd;

#[test]
fn xsd() {
    let paths = Paths::default();
    let xsd = Xsd::load(&paths.xsd_3_0).unwrap();
    println!("{}", xsd);
}
