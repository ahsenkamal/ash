# ash — A shell but with notifications

A small POSIX-like shell with notification support for background jobs, implemented in C.

## Features
- Builtins: `exit`, `echo`, `type`, `pwd`, `cd`, `jobs`, `bg`, `fg` (see [`builtins`](src/builtins.c))
- Executes programs via PATH lookup and `execvp` (see [`run_executable`](include/path_execs.h), implementation in [src/path_execs.c](src/path_execs.c))
- Output redirection using `>`
- Pipes using `|`
- Background jobs using `&` and notifications when they exit
- Job control and listing via the `jobs`, `bg`, and `fg` builtins (see [include/jobs.h](include/jobs.h) and [src/jobs.c](src/jobs.c))

## Build
Make sure to use gnu17 as the C standard in your code editor.

Uses the included `Makefile`:

```bash
# build
make
# result: build/ash
```

## Run
After building:

```bash
./build/ash
```
