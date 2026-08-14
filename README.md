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
- Python [invoke](https://www.pyinvoke.org/) (optional -- wraps the below
  into `invoke build`/`test`/etc.; see
  [Orchestration (invoke)](#orchestration-invoke) below)

## Building

```sh
cmake -S . -B build/native
cmake --build build/native
```

Builds treat warnings as errors (`-Werror`), alongside `-Wall -Wextra
-Wpedantic` and a few extra hardening flags (`-Wvla -Wwrite-strings
-Waggregate-return -Wstack-usage=512`).

All build trees live under `build/`, one subdirectory per configuration --
never directly in `build/` itself -- so the repo root only ever gains that
one entry.

This produces:

- `build/native/libdlog.a` -- the static library
- `build/native/dlog_demo` -- a small demo calling `DLOG()` from a couple
  of different call sites

Run the demo with:

```sh
./build/native/dlog_demo
```

## Testing

```sh
cmake -S . -B build/native -DCMAKE_BUILD_TYPE=Debug
cmake --build build/native
cd build/native && ctest --output-on-failure
```

Currently just a smoke test proving the Unity/CTest harness itself works --
it calls `DLOG()` and checks it doesn't crash. It does **not** yet assert on
the actual `stderr` output, since `TEST_ASSERT_*` can't observe that
directly; that needs a capture strategy (e.g. `freopen`/pipe + read-back),
tracked as a follow-up rather than done yet.

### Static analysis

```sh
cmake --build build/native --target tidy
```

Runs `clang-tidy` over `src/dlog.c` with the same curated check list as
`tpool` (`cert*`, `bugprone*`, `misc*`, `readability*`, minus
`cert-err33-c`). Currently clean. Not wired into the default build --
opt in explicitly. Skips itself (with a configure-time note) if
`clang-tidy` isn't installed.

## Orchestration (invoke)

The commands above are what's actually happening; `tasks.py` (using
[Python invoke](https://www.pyinvoke.org/), `pip install invoke` or
`pipx install invoke`) is a thin convenience wrapper around logger's one
`native` configuration:

```sh
invoke build   # configure + build
invoke test    # build (if needed) + ctest
invoke tidy    # clang-tidy
invoke demo    # build + run dlog_demo
invoke clean   # remove build/ entirely
```

## Layout

```
src/       dlog.c / dlog.h -- the library
demo/      main.c           -- example usage
tests/     test_dlog.c      -- Unity test suite (smoke test only, see Testing)
tasks.py   invoke task definitions
```

## API

| Macro               | Description                                                |
| -------------------- | ----------------------------------------------------------- |
| `DLOG(fmt, ...)`     | printf-style debug message to `stderr`, prefixed with the calling function and line number. No-op when `NDEBUG` is defined. |

See `src/dlog.h` for full documentation.

## Design notes

`dlog_impl()` isn't thread-safe against interleaved output from concurrent
callers (`fprintf`/`vfprintf` aren't atomic across calls) -- deliberately
left alone rather than adding a mutex around every write, since this is
debug-only output and not worth a bottleneck unless it actually gets in
the way of reading a trace.
