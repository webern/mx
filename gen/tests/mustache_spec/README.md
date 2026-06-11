# Vendored Mustache spec test suite

The five core modules of the official Mustache specification test suite
(https://github.com/mustache/spec, MIT license), vendored so the press
(gen/press/engine.py) is held to the spec everywhere it does not
deliberately deviate (see the engine docstring: strict missing keys, no
HTML escaping, no lambdas). gen/tests/test_press.py runs every case.

Do not edit these files; refresh them from the upstream repository.
