#[macro_use]
use crate::error::Result;
use crate::xsd::Xsd;

// expected: &str, node: &exile::Element, xsd: &Xsd
macro_rules! check {
    ($expected:expr, $node:expr, $xsd:expr) => {{
        if let Some(ns) = &$node.namespace {
            if ns.as_str() != $xsd.prefix() {
                return raise!(
                    "expected namespace prefix '{}', got '{}'.",
                    $xsd.prefix(),
                    ns.as_str()
                );
            }
        } else {
            return raise!("no namespace prefix, expected '{}'.", $xsd.prefix());
        };
        Ok(())
    } as crate::error::Result<()>};
}

// fn expect(expected: &str, node: &exile::Element, xsd: &Xsd) -> Result<()> {
//     if let Some(ns) = &node.namespace {
//         if ns.as_str() != xsd.prefix() {
//             return raise!(
//                 "expected namespace prefix '{}', got '{}'.",
//                 xsd.prefix(),
//                 ns.as_str()
//             );
//         }
//     } else {
//         return raise!("no namespace prefix, expected '{}'.", xsd.prefix());
//     };
//     Ok(())
// }
