use exile::Element;
use indexmap::set::IndexSet;
use libmxgen::generate::paths::Paths;
use std::collections::HashMap;

fn main() {
    let paths = Paths::default();
    let xml_str = std::fs::read_to_string(paths.xsd_3_0).unwrap();
    let doc = exile::parse(&xml_str).unwrap();
    let children_types = print_sequence_children(doc.root(), "choice");
    println!("==========================================================================================");
    println!("\nAll Subtypes:");
    for child_type in children_types {
        println!("{}", child_type)
    }
}

#[allow(unused)]
fn find_xs_nodes(node: &exile::Element, xs_nodes: &mut IndexSet<String>) {
    if let Some(ns) = node.prefix() {
        if ns == "xs" {
            xs_nodes.insert(node.fullname().to_owned());
        }
    }
    for child in node.children() {
        find_xs_nodes(child, xs_nodes);
    }
}

#[allow(unused)]
fn print_group_children(root: &Element) {
    println!("==========================================================================================");
    println!("GROUPS");
    println!("==========================================================================================");
    let mut the_stuf = HashMap::new();
    for child in root.children() {
        if child.name() == "group" {
            let name = child.attribute("name").unwrap().clone();
            let mut xs_nodes = IndexSet::<String>::new();
            for xs_thingy in child.children() {
                if xs_thingy.name() != "annotation" {
                    xs_nodes.insert(xs_thingy.fullname().into());
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

fn print_sequence_children(root: &Element, target_element: &str) -> IndexSet<String> {
    println!("==========================================================================================");
    println!("{}", target_element.to_uppercase());
    println!("==========================================================================================");
    let mut children_types: IndexSet<String> = IndexSet::new();
    print_sequence_children_recursively(root, target_element, "", &mut children_types);
    children_types
}

fn print_sequence_children_recursively(
    node: &Element,
    target_element: &str,
    parent_name: &str,
    children_types: &mut IndexSet<String>,
) {
    let mut most_recent_name = parent_name.to_owned();
    if let Some(nm) = node.attribute("name") {
        most_recent_name = nm.into();
    }
    let mut attributes = Vec::new();
    for (k, _) in node.attributes() {
        attributes.push(k.clone());
    }
    attributes.sort();
    if node.name() == target_element {
        print_sequence_children_the_children_part(
            node,
            target_element,
            most_recent_name.as_str(),
            &attributes,
            children_types,
        );
    } else {
        for child in node.children() {
            print_sequence_children_recursively(
                child,
                target_element,
                most_recent_name.as_str(),
                children_types,
            );
        }
    }
}

fn print_sequence_children_the_children_part(
    node: &Element,
    target_element: &str,
    parent_name: &str,
    attributes: &Vec<String>,
    children_types: &mut IndexSet<String>,
) {
    if node.name() != target_element {
        panic!("wtf");
    }
    println!("");
    println!("{}:", parent_name);
    println!("[{}]", attributes.join(" "));
    for child in node.children() {
        println!("{}", child.name());
        children_types.insert(child.name().into());
    }
}
