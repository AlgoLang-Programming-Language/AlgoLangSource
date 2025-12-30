# Algolang Syntax Reference

## Table of Contents

1. [Lexical Structure](#lexical-structure)
2. [Types](#types)
3. [Expressions](#expressions)
4. [Statements](#statements)
5. [Functions](#functions)
6. [Scope and Variables](#scope-and-variables)
7. [Standard Library](#standard-library)

## Lexical Structure

### Comments

Single-line comments start with `#`:

```algo
# This is a comment
let x = 42  # Inline comment
```

### Identifiers

Identifiers must start with a letter or underscore, followed by letters, digits, or underscores:

```algo
valid_name
_internal
counter2
```

### Keywords

Reserved words in Algolang:

- `let` - Variable declaration
- `fn` - Function declaration
- `if`, `else` - Conditional execution
- `while` - Loop construct
- `return` - Function return
- `print` - Output statement
- `true`, `false` - Boolean literals
- `and`, `or` - Logical operators

### Operators

**Arithmetic**: `+` `-` `*` `/` `%`

**Comparison**: `==` `!=` `<` `<=` `>` `>=`

**Logical**: `and` `or` `!`

**Assignment**: `=`

### Delimiters

- `()` - Parentheses for grouping and function calls
- `{}` - Braces for blocks
- `,` - Comma for separating arguments
- `;` - Optional statement terminator

## Types

Algolang has three primitive types:

### Number

64-bit floating-point numbers:

```algo
let integer = 42
let decimal = 3.14159
let negative = -17
let scientific = 1.5
```

### Boolean

True or false values:

```algo
let flag = true
let condition = false
```

### Nil

Represents absence of value:

```algo
let empty = nil
```

## Expressions

### Literals

```algo
42          # Number
3.14        # Number
true        # Boolean
false       # Boolean
```

### Variables

```algo
x           # Variable reference
```

### Binary Operations

```algo
a + b       # Addition
a - b       # Subtraction
a * b       # Multiplication
a / b       # Division
a % b       # Modulo

a == b      # Equality
a != b      # Inequality
a < b       # Less than
a <= b      # Less than or equal
a > b       # Greater than
a >= b      # Greater than or equal

a and b     # Logical AND
a or b      # Logical OR
```

### Unary Operations

```algo
-x          # Negation
!condition  # Logical NOT
```

### Function Calls

```algo
func()              # No arguments
func(arg)           # Single argument
func(arg1, arg2)    # Multiple arguments
```

### Assignment

```algo
x = 10
x = x + 1
```

### Grouping

```algo
(a + b) * c
```

## Statements

### Expression Statement

Any expression can be used as a statement:

```algo
print x
add(5, 3)
x = 10
```

### Variable Declaration

```algo
let x = 10
let y = x + 5
let uninitialized  # Defaults to nil
```

### Block Statement

```algo
{
  let x = 10
  print x
}
```

### If Statement

```algo
if condition {
  # then branch
}

if condition {
  # then branch
} else {
  # else branch
}

if condition1 {
  # branch 1
} else if condition2 {
  # branch 2
} else {
  # default branch
}
```

### While Loop

```algo
while condition {
  # loop body
}
```

Example:
```algo
let i = 0
while i < 10 {
  print i
  i = i + 1
}
```

### Print Statement

```algo
print expression
```

## Functions

### Function Declaration

```algo
fn name(param1, param2) {
  # function body
}
```

### Function Parameters

Functions can have zero or more parameters:

```algo
fn noParams() {
  return 42
}

fn oneParam(x) {
  return x * 2
}

fn twoParams(a, b) {
  return a + b
}
```

### Return Statement

```algo
fn add(a, b) {
  return a + b
}

fn noReturn() {
  print "side effect"
  # implicitly returns nil
}
```

### Recursion

Functions can call themselves:

```algo
fn factorial(n) {
  if n <= 1 {
    return 1
  }
  return n * factorial(n - 1)
}
```

## Scope and Variables

### Block Scope

Variables declared with `let` are scoped to their enclosing block:

```algo
let x = 10

{
  let x = 20  # Different variable
  print x     # Prints 20
}

print x       # Prints 10
```

### Function Scope

Function parameters and local variables are scoped to the function:

```algo
fn example(param) {
  let local = 42
  print param
  print local
}

# param and local are not accessible here
```

### Global Scope

Variables declared at the top level are global:

```algo
let global = 100

fn useGlobal() {
  print global  # Can access global
}
```

### Shadowing

Inner scopes can shadow outer variables:

```algo
let x = 1

fn shadow() {
  let x = 2  # Shadows global x
  print x    # Prints 2
}

print x      # Prints 1
```

## Standard Library

### Mathematical Functions

**abs(x)** - Absolute value

```algo
print abs(-42)    # 42
print abs(3.14)   # 3.14
```

**min(a, b)** - Minimum of two numbers

```algo
print min(10, 20)   # 10
print min(-5, 3)    # -5
```

**max(a, b)** - Maximum of two numbers

```algo
print max(10, 20)   # 20
print max(-5, 3)    # 3
```

**sqrt(x)** - Square root

```algo
print sqrt(16)     # 4
print sqrt(2)      # 1.41421...
```

**pow(x, y)** - x raised to power y

```algo
print pow(2, 10)   # 1024
print pow(3, 3)    # 27
```

**floor(x)** - Round down to integer

```algo
print floor(3.7)   # 3
print floor(-2.3)  # -3
```

**ceil(x)** - Round up to integer

```algo
print ceil(3.2)    # 4
print ceil(-2.8)   # -2
```

## Grammar (EBNF)

```ebnf
program        → declaration* EOF ;

declaration    → letDecl
               | fnDecl
               | statement ;

letDecl        → "let" IDENTIFIER ( "=" expression )? ;
fnDecl         → "fn" IDENTIFIER "(" parameters? ")" block ;
parameters     → IDENTIFIER ( "," IDENTIFIER )* ;

statement      → exprStmt
               | printStmt
               | blockStmt
               | ifStmt
               | whileStmt
               | returnStmt ;

exprStmt       → expression ;
printStmt      → "print" expression ;
blockStmt      → "{" declaration* "}" ;
ifStmt         → "if" expression block ( "else" ( ifStmt | block ) )? ;
whileStmt      → "while" expression block ;
returnStmt     → "return" expression? ;

expression     → assignment ;
assignment     → IDENTIFIER "=" assignment
               | logicOr ;
logicOr        → logicAnd ( "or" logicAnd )* ;
logicAnd       → equality ( "and" equality )* ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" | "%" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | call ;
call           → primary ( "(" arguments? ")" )* ;
arguments      → expression ( "," expression )* ;
primary        → NUMBER | "true" | "false" | "nil"
               | IDENTIFIER | "(" expression ")" ;
```

## Examples

### Computing Fibonacci

```algo
fn fib(n) {
  if n <= 1 {
    return n
  }
  return fib(n - 1) + fib(n - 2)
}

print fib(10)
```

### Greatest Common Divisor

```algo
fn gcd(a, b) {
  while b != 0 {
    let temp = b
    b = a % b
    a = temp
  }
  return a
}

print gcd(48, 18)
```

### Factorial

```algo
fn factorial(n) {
  if n <= 1 {
    return 1
  }
  return n * factorial(n - 1)
}

print factorial(5)
```

### Prime Check

```algo
fn isPrime(n) {
  if n < 2 {
    return false
  }
  
  let i = 2
  while i * i <= n {
    if n % i == 0 {
      return false
    }
    i = i + 1
  }
  
  return true
}

print isPrime(17)
print isPrime(100)
```
