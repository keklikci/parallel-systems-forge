#!/bin/sh
set -eu

if ! command -v clang-format >/dev/null 2>&1; then
    echo "clang-format is required for format checks" >&2
    exit 1
fi

status=0
for file in $(find examples -type f \( -name '*.c' -o -name '*.cpp' \) -print); do
    if ! clang-format --dry-run --Werror "$file"; then
        status=1
    fi
done
exit "$status"
