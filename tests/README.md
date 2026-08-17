# Tests

Native tests are registered with CTest after configuring the CMake build. The
headless Java test is available through the `java-test` build target.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
cmake --build build --target java-test
```
