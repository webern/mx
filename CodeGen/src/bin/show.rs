use indexmap::set::IndexSet;
use libmxgen::generate::paths::Paths;

fn main() {
    let paths = Paths::default();
    let xml_str = std::fs::read_to_string(paths.xsd_3_0).unwrap();
    let doc = exile::parse(&xml_str).unwrap();
    let mut xs_nodes = IndexSet::new();
    find_xs_nodes(doc.root(), &mut xs_nodes);
    for xs_node in xs_nodes {
        println!("{}", xs_node);
    }
}

fn find_xs_nodes(node: &exile::Element, xs_nodes: &mut IndexSet<String>) {
    if let Some(ns) = &node.namespace {
        if ns == "xs" {
            xs_nodes.insert(node.fullname());
        }
    }
    for child in node.children() {
        find_xs_nodes(child, xs_nodes);
    }
}
