# logger

An "improved" `fprintf()` for debug logging in C. `DLOG()` writes a
printf-style message to `stderr`, prefixed with `[DEBUG][function:line]`.
Defining `NDEBUG` (same convention as `assert()`) compiles every `DLOG()`
call down to nothing, so release builds pay no cost and don't need `dlog.c`
linked in at all.

```c
#include "dlog.h"

DLOG("processing value=%d", value);
/* [DEBUG][do_work:5] processing value=42 */
```

## Requirements

- A C99 compiler
- CMake >= 3.15
- Internet access on first configure of the test suite (to fetch
  [Unity](https://github.com/ThrowTheSwitch/Unity))
- clang-tidy (optional -- powers the `tidy` target)

## Building

```sh
cmake -S . -B build
cmake --build build
```

Builds treat warnings as errors (`-Werror`), alongside `-Wall -Wextra
-Wpedantic` and a few extra hardening flags (`-Wvla -Wwrite-strings
-Waggregate-return -Wstack-usage=512`).

This produces:

- `build/libdlog.a` -- the static library
- `build/dlog_demo` -- a small demo calling `DLOG()` from a couple of
  different call sites

Run the demo with:

```sh
./build/dlog_demo
```

## Testing

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
cd build && ctest --output-on-failure
```

Currently just a smoke test proving the Unity/CTest harness itself works --
it calls `DLOG()` and checks it doesn't crash. It does **not** yet assert on
the actual `stderr` output, since `TEST_ASSERT_*` can't observe that
directly; that needs a capture strategy (e.g. `freopen`/pipe + read-back),
tracked as a follow-up rather than done yet.

### Static analysis

```sh
cmake --build build --target tidy
```

Runs `clang-tidy` over `src/dlog.c` with the same curated check list as
`tpool` (`cert*`, `bugprone*`, `misc*`, `readability*`, minus
`cert-err33-c`). Currently clean. Not wired into the default build --
opt in explicitly. Skips itself (with a configure-time note) if
`clang-tidy` isn't installed.

## Layout

```
src/    dlog.c / dlog.h -- the library
demo/   main.c           -- example usage
tests/  test_dlog.c      -- Unity test suite (smoke test only, see above)
```

## API

| Macro               | Description                                                |
| -------------------- | ----------------------------------------------------------- |
| `DLOG(fmt, ...)`     | printf-style debug message to `stderr`, prefixed with the calling function and line number. No-op when `NDEBUG` is defined. |

See `src/dlog.h` for full documentation.

## Design notes

`dlog_impl()` isn't thread-safe against interleaved output from concurrent
callers (`fprintf`/`vfprintf` aren't atomic across calls) -- deliberately
left alone rather than adding a mutex around every write. See `TODO.md` for
the reasoning.
