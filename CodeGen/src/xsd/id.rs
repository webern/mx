use crate::error::Result;
use crate::xsd::constants::{
    ANNOTATION, ATTRIBUTE_GROUP, COMPLEX_TYPE, ELEMENT, GROUP, IMPORT, NAME, REF, SIMPLE_TYPE, TYPE,
};
use std::borrow::Cow;
use std::cmp::Ordering;
use std::collections::hash_map::DefaultHasher;
use std::fmt::{Display, Formatter};
use std::hash::{Hash, Hasher};

#[derive(Clone, Debug, Eq, PartialEq, Ord, PartialOrd)]
pub enum Id {
    Root(RootNodeId),
    Child(ChildNodeId),
}

impl Id {
    pub fn new(type_: RootNodeType, name: String) -> Self {
        Id::Root(RootNodeId {
            index: None,
            type_,
            name,
        })
    }

    pub fn root_with_index(index: u64, type_: RootNodeType, name: String) -> Self {
        Id::Root(RootNodeId {
            index: Some(index),
            type_,
            name,
        })
    }

    pub fn child<S1, S2>(parent: &Id, type_: S1, name: Option<S2>) -> Self
    where
        S1: AsRef<str>,
        S2: AsRef<str>,
    {
        Id::Child(ChildNodeId {
            type_: type_.as_ref().into(),
            name: if let Some(s) = name {
                Some(s.as_ref().into())
            } else {
                None
            },
            parent: Box::new(parent.clone()),
        })
    }

    pub fn name(&self) -> Cow<'_, str> {
        match &self {
            Id::Root(r) => r.name(),
            Id::Child(c) => c.name(),
        }
    }

    pub fn parent(&self) -> Option<&Id> {
        match self {
            Id::Root(_) => None,
            Id::Child(ch) => Some(&ch.parent),
        }
    }

    pub fn index(&self) -> Option<u64> {
        match self {
            Id::Root(r) => r.index,
            Id::Child(_) => None,
        }
    }

    pub fn root_node_type(&self) -> Option<RootNodeType> {
        match self {
            Id::Root(r) => Some(r.type_),
            Id::Child(_) => None,
        }
    }

    pub fn display(&self) -> String {
        format!("{}", &self)
    }

    pub(crate) fn make(lineage: Lineage, node: &exile::Element) -> Result<(Self, Lineage)> {
        let attrs = node.attributes.map();
        let name = if let Some(s) = attrs.get("id") {
            s.clone()
        } else if let Some(s) = attrs.get(NAME) {
            s.clone()
        } else if let Some(s) = attrs.get(TYPE) {
            s.clone()
        } else if let Some(s) = attrs.get(REF) {
            s.clone()
        } else {
            let mut hasher = DefaultHasher::new();
            node.hash(&mut hasher);
            let result = hasher.finish();
            format!("{}", result)
        };
        let type_ = node.name.as_str();
        match lineage {
            Lineage::Index(i) => {
                let id = Id::Root(RootNodeId {
                    index: Some(i),
                    type_: RootNodeType::parse(type_)?,
                    name: name.into(),
                });
                Ok((id.clone(), Lineage::Parent(id.clone())))
            }
            Lineage::Parent(p) => {
                let id = Id::Child(ChildNodeId {
                    type_: type_.into(),
                    name: if name.is_empty() {
                        None
                    } else {
                        Some(name.into())
                    },
                    parent: Box::new(p),
                });
                Ok((id.clone(), Lineage::Parent(id.clone())))
            }
        }
    }
}

impl Display for Id {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        match self {
            Id::Root(x) => Display::fmt(x, f),
            Id::Child(x) => Display::fmt(x, f),
        }
    }
}

#[derive(Clone, Copy, Debug, Eq, PartialEq, Ord, PartialOrd)]
pub enum RootNodeType {
    Annotation,
    AttributeGroup,
    ComplexType,
    Element,
    Group,
    Import,
    SimpleType,
}

impl Display for RootNodeType {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        let s = match self {
            RootNodeType::Annotation => ANNOTATION,
            RootNodeType::AttributeGroup => ATTRIBUTE_GROUP,
            RootNodeType::ComplexType => COMPLEX_TYPE,
            RootNodeType::Element => ELEMENT,
            RootNodeType::Group => GROUP,
            RootNodeType::Import => IMPORT,
            RootNodeType::SimpleType => SIMPLE_TYPE,
        };
        write!(f, "{}", s)
    }
}

impl RootNodeType {
    pub fn parse<S: AsRef<str>>(s: S) -> Result<RootNodeType> {
        let et = match s.as_ref() {
            ANNOTATION => RootNodeType::Annotation,
            ATTRIBUTE_GROUP => RootNodeType::AttributeGroup,
            COMPLEX_TYPE => RootNodeType::ComplexType,
            ELEMENT => RootNodeType::Element,
            GROUP => RootNodeType::Group,
            IMPORT => RootNodeType::Import,
            SIMPLE_TYPE => RootNodeType::SimpleType,
            _ => return raise!("unexpected string '{}'", s.as_ref()),
        };
        Ok(et)
    }
}

#[derive(Clone, Debug, Eq)]
pub struct RootNodeId {
    index: Option<u64>,
    type_: RootNodeType,
    name: String,
}

impl RootNodeId {
    pub fn index(&self) -> Option<u64> {
        self.index
    }
    pub fn name(&self) -> Cow<'_, str> {
        Cow::Borrowed(self.name.as_str())
    }
    pub fn type_(&self) -> RootNodeType {
        self.type_
    }
}

impl Display for RootNodeId {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}:{}", self.type_, self.name.as_str())
    }
}

impl PartialEq for RootNodeId {
    fn eq(&self, other: &Self) -> bool {
        self.type_ == other.type_ && self.name == other.name
    }
}

impl Ord for RootNodeId {
    fn cmp(&self, other: &Self) -> Ordering {
        if let Some(left) = self.index {
            if let Some(right) = other.index {
                return left.cmp(&right);
            }
        }
        if self.type_ == other.type_ {
            self.name.cmp(&other.name)
        } else {
            self.type_.cmp(&other.type_)
        }
    }
}

impl PartialOrd for RootNodeId {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

#[derive(Clone, Debug, Eq, PartialEq, Ord, PartialOrd)]
pub struct ChildNodeId {
    type_: String,
    name: Option<String>,
    parent: Box<Id>,
}

impl Display for ChildNodeId {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        Display::fmt(self.parent.as_ref(), f)?;
        write!(f, ":{}", self.type_.as_str())?;
        if let Some(name) = &self.name {
            write!(f, ":{}", name.as_str())?;
        }
        Ok(())
    }
}

impl ChildNodeId {
    pub fn name(&self) -> Cow<'_, str> {
        Cow::Owned(format!("{}", self))
    }
}

#[derive(Clone, Debug, Eq, PartialEq, Ord, PartialOrd)]
pub enum Lineage {
    Index(u64),
    Parent(Id),
}

impl Lineage {
    pub fn index(&self) -> Option<u64> {
        match self {
            Lineage::Index(i) => Some(*i),
            Lineage::Parent(_) => None,
        }
    }

    pub fn parent(&self) -> Option<&Id> {
        match self {
            Lineage::Index(_) => None,
            Lineage::Parent(id) => Some(id),
        }
    }
}
