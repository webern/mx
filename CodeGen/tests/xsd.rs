use libmxgen::generate::paths::Paths;
use libmxgen::xsd::XSD;

#[test]
fn xsd() {
    let paths = Paths::default();
    let xsd = XSD::load(&paths.xsd_3_0).unwrap();
    println!("hi");
}
