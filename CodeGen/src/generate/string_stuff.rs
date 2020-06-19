pub(crate) fn pascal_case<S: AsRef<str>>(s: S) -> String {
    let mut out = String::new();
    let mut upper = true;
    for c in s.as_ref().chars() {
        if c == '-' || c == ' ' {
            upper = true;
            continue;
        }
        if upper {
            let uc = c.to_uppercase().next().unwrap();
            out.push(uc);
        } else {
            out.push(c);
        }
        upper = false;
    }
    out
}

pub(crate) fn camel_case<S: AsRef<str>>(s: S) -> String {
    let mut out = String::new();
    let mut upper = false;
    for c in s.as_ref().chars() {
        if c == '-' || c == ' ' {
            upper = true;
            continue;
        }
        if upper {
            let uc = c.to_uppercase().next().unwrap();
            out.push(uc);
        } else {
            let uc = c.to_lowercase().next().unwrap();
            out.push(c);
        }
        upper = false;
    }

    // StepEnum is coming out wrong, don't know why.
    if out.len() == 1 {
        return out.to_lowercase();
    }

    out
}
