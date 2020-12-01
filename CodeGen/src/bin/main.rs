use libmxgen::generate::{run, GenArgs};
use log::{debug, info, trace, LevelFilter};
use std::io::Write;
use std::time::Instant;

fn main() {
    init_logger();
    debug!("debug logging enabled");
    trace!("trace logging enabled");
    let start = Instant::now();
    let exit_code = match run(GenArgs::default()) {
        Ok(()) => 0,
        Err(err) => {
            eprintln!("{}", err);
            1
        }
    };
    let duration = start.elapsed();
    info!("Runtime: {:?}", duration);
    std::process::exit(exit_code);
}

const DEFAULT_LOG_LEVEL: LevelFilter = LevelFilter::Trace;

/// Initialized the `env_logger` using the environment variable, if present, defaulting to `Info`
/// if not present.
fn init_logger() {
    if std::env::var("RUST_LOG").is_ok() {
        // if the env var for controlling env_logger is there, use it
        env_logger::init();
    } else {
        // if the env var is not there, set to info logging for our crate only
        env_logger::Builder::new()
            .format(|buf, record| {
                writeln!(
                    buf,
                    "{}:{} {} [{}] - {}",
                    record.file().unwrap_or("unknown"),
                    record.line().unwrap_or(0),
                    chrono::Local::now().format("%Y-%m-%dT%H:%M:%S"),
                    record.level(),
                    record.args()
                )
            })
            .filter(Some("mxgen"), DEFAULT_LOG_LEVEL)
            .filter(Some("libmxgen"), DEFAULT_LOG_LEVEL)
            .init()
    }
}
