# LAttempt1

LAttempt1 is my first attempt at implementing a functioning interpreter
for a lisp language. It is very rudimentary, but it served well for my learning
purposes. The language design was inspired by clojure in certain topics,
but severally limited for simplicity purposes.

An example program can be found inside in.txt

## Characteristics

- Pure s-expression syntax (no `[]`s or `{}`s)
- No ' or ` quoting
- No string literals
- Lexical Scoping
- Lambdas
- Macros
- Naive Garbage Collection

### Syntax

```
value: literal | list

literal: symbol | number

list: '(' opt_values ')'

opt_values: values | ''

values: value values | value
```

Only non-negative integer literals are supported.

## Building the program

### cmake

Once you have working build of cmake (3.18+)
it is sufficient to do the following to compile the project.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### zig

To build the project, `zig build` is sufficient.
You can also run the project with `zig build run`.

## Running the program

The lattempt1 executable starts a rudimentary repl for the language.
the file sample.txt provides some code samples which can be executed with
```
./lattempt1 < ../sample.txt
```
using a POSIX shell.
