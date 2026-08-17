# Contributing

## Development workflow

Configure an out-of-tree build and compile all native targets:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
cmake --build build --target java-test
./scripts/check-format.sh
```

Keep generated files out of the repository. Run the relevant example locally
before submitting changes, and update documentation when a command, interface,
or prerequisite changes.

## Code style

Native code follows the repository `.clang-format` configuration. New code
should use explicit error handling, bounded resource lifetimes, and clear
thread-synchronization ownership.

## Commits

Use Conventional Commits with a meaningful scope, for example:

```text
fix(memory-map): handle empty input files
```
