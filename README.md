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

## Running the program

Having cmake installed,
it is sufficient to do the following commands to compile the project.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Which will build the lattemp1 executable, which starts a rudimentary repl.
The program just reads expressions and evaluates them.
The file sample.txt provides some code samples which can be executed with

```
./lattempt1 < ../sample.txt
```
