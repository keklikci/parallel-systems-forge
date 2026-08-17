# Native Examples

## Character counters

The stream-based C, C++, and memory-mapped implementations share this
interface:

```sh
<program> <input-file> [character]
```

The character defaults to `a`. `fixture.txt` is a small deterministic input
for smoke tests and examples.

## Memory manager

```sh
memory-management-api [thread-count] [seed] [capacity]
```

The program services bounded allocation requests through a synchronized queue
and reports requests that exceed the configured capacity.

## Seat reservation

```sh
seat-reservation [seed] [reservations-per-agency]
```

Two agencies reserve seats concurrently. The reservation table is protected by
a mutex and the run terminates after the requested number of attempts.
