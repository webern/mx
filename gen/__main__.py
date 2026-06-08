"""mx code generator entry point.

Usage: python3 -m gen <config.toml>

Reads a MusicXML 4.0 XSD specification and generates typed document
serialization/deserialization code for the target described in the given
config file.
"""

import sys


def main() -> int:
    print("error: generator not implemented", file=sys.stderr)
    return 1


if __name__ == "__main__":
    sys.exit(main())
