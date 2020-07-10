use crate::model::Model;
use crate::xsd::{Entry, Xsd};
use std::fmt::{Display, Formatter};

/// # Create Trait
///
/// An object that takes a top-level `Entry` from the XSD document, and converts it into one or more
/// `Model` objects.
///
/// # Purpose
///
/// This library will provide a default `Create` implementation, but the trait allows users to
/// customize the interpretation of their XSD. Any number of `Create` implementations can be used
/// during the processing of the XSD. Each `Create` object will be tried, in order, and the first
/// one that returns `Some` will end the chain of attempts.
///
/// # Return
///
/// If `None` is returned, it indicates that the `Create` object ignored the input, i.e. subsequent
/// `Create` objects should try to handle the input. If `Some` is returned, but the `Vec` is empty,
/// then it means the `Create` object handled the input, chose not to produce any models, and
/// subsequent `Create` objects should not try to handle the input. Finally, a non-empty `Vec` means
/// that one or models were produced, again indicating that subsequent `Create` objects should not
/// attempt to create models.
///
/// # Error
///
/// We define a simple error type instead of using generics.
///  
pub trait Create {
    /// The name of this `Create` object (for debugging).
    fn name(&self) -> &'static str;

    /// Creates models based on a top-level XSD entry. Returns `None` if the XSD entry cannot be
    /// handled.
    fn create(&self, entry: &Entry, xsd: &Xsd) -> CreateResult;
}

pub type CreateResult = std::result::Result<Option<Vec<Model>>, CreateError>;

#[derive(Debug, Clone, Eq, PartialEq, Ord, PartialOrd)]
pub struct CreateError {
    pub message: String,
}

impl Display for CreateError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.message.as_str())
    }
}

impl std::error::Error for CreateError {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        None
    }
}

impl CreateError {
    pub fn new<S: AsRef<str>>(message: S) -> Self {
        Self {
            message: message.as_ref().into(),
        }
    }
}
