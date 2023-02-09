
# LA1

## Parser Syntax

```

value: literal | list | ticked-value

literal: symbol | number

ticked-value: tick value

list: '(' opt_values ')'

opt_values: values | ''

values: value values | value

terminals: symbol, tick, number, '(', ')'

```