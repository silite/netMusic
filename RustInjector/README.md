rustup override set nightly
rustup target add i686-pc-windows-msvc
cargo build --release --target i686-pc-windows-msvc
