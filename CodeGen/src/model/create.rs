use crate::model::Model;
use crate::xsd::{Entry, Xsd};

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
/// Any error type may be used. Returning `Err` will stop all processing.
///  
pub trait Create {
    type E;
    fn create(entry: &Entry, xsd: &Xsd) -> std::result::Result<Option<Vec<Model>>, Self::E>;
}
