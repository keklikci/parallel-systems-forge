# Operating Systems Examples

A collection of portable systems-programming examples covering file processing,
memory mapping, thread coordination, bounded allocation, and the Dining
Philosophers problem.

## Requirements

- CMake 3.20 or newer
- A C11/C++17 compiler
- POSIX threads for the native concurrency examples
- Java LTS for the Dining Philosophers example

## Build

```sh
cmake -S . -B build
cmake --build build
cmake --build build --target java-compile
```

Native executables are written to the build directory. The Java classes are
written to `build/java-classes`.

## Examples

The native examples are located in `examples/memory-map/`. The character
counter programs currently use the input file expected by their original
examples; their command-line interface will be standardized in a later phase.

The Java Dining Philosophers application is located in
`examples/dining-philosophers/`. Its Swing interface requires a graphical
desktop environment.

## Development

See [CONTRIBUTING.md](CONTRIBUTING.md) for formatting, testing, and change
guidelines.

