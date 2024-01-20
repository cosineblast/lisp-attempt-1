# LAttempt1

LAttempt1 is my first attempt at implementing a functioning interpreter
for a lisp language. It is very rudimentary, but it served well for my learning
purposes. The language design was inspired by clojure in certain topics,
but severally limited for simplicity purposes.

An example program can be found inside sample.txt

## Characteristics

- Pure s-expression syntax (no `[]`s or `{}`s)
- No ' or \` quoting
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

To build the project, use `zig build`, which installs the executable at` zig-out/bin`.

You can run the project with `zig build run`, or with the executable.

The lattempt1 executable starts a rudimentary repl for the language.
the file sample.txt provides some code samples which can be executed with
```
./lattempt1 < ../sample.txt
```
using a POSIX shell.


