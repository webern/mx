use exile::Element;
use indexmap::set::IndexSet;
use libmxgen::generate::paths::Paths;
use std::collections::HashMap;

fn main() {
    let paths = Paths::default();
    let xml_str = std::fs::read_to_string(paths.xsd_3_0).unwrap();
    let doc = exile::parse(&xml_str).unwrap();
    print_sequence_children(doc.root());
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
    println!(
        "=========================================================================================="
    );
    println!("GROUPS");
    println!(
        "=========================================================================================="
    );
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

fn print_sequence_children(root: &Element) {
    println!(
        "=========================================================================================="
    );
    println!("SEQUENCES");
    println!(
        "=========================================================================================="
    );
    print_sequence_children_recursively(root, "");
}

fn print_sequence_children_recursively(node: &Element, parent_name: &str) {
    let mut most_recent_name = parent_name.to_owned();
    if let Some(nm) = node.attributes.map().get("name") {
        most_recent_name = nm.clone();
    }
    if node.name.as_str() == "sequence" {
        print_sequence_children_the_children_part(node, most_recent_name.as_str());
    } else {
        for child in node.children() {
            print_sequence_children_recursively(child, most_recent_name.as_str());
        }
    }
}

fn print_sequence_children_the_children_part(node: &Element, parent_name: &str) {
    if node.name.as_str() != "sequence" {
        panic!("wtf");
    }
    println!("");
    println!("{}:", parent_name);
    for child in node.children() {
        println!("{}", child.name.as_str())
    }
}
