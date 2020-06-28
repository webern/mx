use exile::Element;
use indexmap::set::IndexSet;
use libmxgen::generate::paths::Paths;
use std::collections::HashMap;

fn main() {
    let paths = Paths::default();
    let xml_str = std::fs::read_to_string(paths.xsd_3_0).unwrap();
    let doc = exile::parse(&xml_str).unwrap();
    print_group_children(doc.root());
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

fn print_group_children(root: &Element) {
    let mut the_stuf = HashMap::new();
    for child in root.children() {
        if child.name.as_str() == "group" {
            let name = child.attributes.map().get("name").unwrap().clone();
            let mut xs_nodes = IndexSet::<String>::new();
            for xs_thingy in child.children() {
                if xs_thingy.name.as_str() != "annotation" {
                    xs_nodes.insert(xs_thingy.fullname());
                }
            }
            the_stuf.insert(name, xs_nodes);
        }
    }
    for (key, val) in the_stuf {
        println!("");
        println!("{}:", key);
        for x in val {
            println!("{}", x);
        }
    }
}
