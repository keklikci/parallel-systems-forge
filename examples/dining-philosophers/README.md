# Dining Philosophers

This example demonstrates coordinated access to shared fork resources through
a finite, deterministic simulation and an optional Swing frontend.

Compile it through the root CMake project:

```sh
cmake --build build --target java-compile
java -cp build/java-classes Philosopher [rounds] [seed]
```

The core simulation does not require a display server. The GUI loads its
images from packaged resources, so it does not depend on the current working
directory.
