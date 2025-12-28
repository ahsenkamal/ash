# ash — Shell but with notifications
A small POSIX-like shell but with notification support for background jobs, implemented in C.

## Features
- Builtins: `exit`, `echo`, `type`, `pwd`, `cd`
- Executes programs via PATH lookup and `execvp`
- Output redirection using `>`

## Build
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