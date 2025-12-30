# Getting Started with Algolang

Welcome to **Algolang** - a math-first, algorithm-focused programming language designed for clarity, correctness, and learning. This guide will get you up and running in minutes.

---

## Table of Contents

1. [Installation](#installation)
2. [Your First Program](#your-first-program)
3. [Language Fundamentals](#language-fundamentals)
4. [Control Flow](#control-flow)
5. [Functions](#functions)
6. [Standard Library](#standard-library)
7. [Examples](#examples)
8. [Best Practices](#best-practices)
9. [Next Steps](#next-steps)

---

## Installation

### Prerequisites

Before building Algolang, ensure you have:

- **C Compiler**: GCC 7+ or Clang 10+
- **Make**: Build automation tool
- **Git**: For cloning the repository

Most Linux and macOS systems have these pre-installed. On Windows, use MinGW or MSYS2.

### Quick Installation

**Clone and build in 3 commands:**

```bash
git clone https://github.com/AlgoLang-Programming-Language/AlgoLangSource.git
cd AlgoLangSource
make
```

That's it! The `algolang` executable is now ready.

### Verify Installation

Start the REPL to confirm everything works:

```bash
./algolang
```

You should see:

```
Algolang 0.1.0
Type 'exit' to quit

>
```

Try a simple expression:

```algo
> 2 + 2
4
> print 42
42
```

Type `exit` to quit the REPL.

### Platform-Specific Notes

<details>
<summary><b>Linux (Ubuntu/Debian)</b></summary>

```bash
# Install dependencies (if needed)
sudo apt update
sudo apt install build-essential git

# Clone and build
git clone https://github.com/AlgoLang-Programming-Language/AlgoLangSource.git
cd AlgoLangSource
make

# Optional: Install system-wide
sudo make install
```
</details>

<details>
<summary><b>macOS</b></summary>

```bash
# Install Xcode Command Line Tools (if needed)
xcode-select --install

# Clone and build
git clone https://github.com/AlgoLang-Programming-Language/AlgoLangSource.git
cd AlgoLangSource
make

# Optional: Install to /usr/local/bin
sudo make install
```
</details>

<details>
<summary><b>Windows (MSYS2)</b></summary>

```bash
# In MSYS2 terminal, install GCC
pacman -S mingw-w64-x86_64-gcc make git

# Clone and build
git clone https://github.com/AlgoLang-Programming-Language/AlgoLangSource.git
cd AlgoLangSource
make

# The algolang.exe is now ready
```
</details>

### Alternative: Manual Compilation

If `make` doesn't work, compile manually:

```bash
gcc -Wall -Wextra -std=c11 -O2 -Iinclude \
    src/main.c \
    src/lexer/lexer.c \
    src/parser/parser.c \
    src/parser/ast.c \
    src/bytecode/compiler.c \
    src/vm/vm.c \
    src/vm/globals.c \
    src/runtime/value.c \
    src/stdlib/stdlib.c \
    -o algolang -lm
```

---

## Your First Program

### Hello World (REPL)

The easiest way to start is the REPL (Read-Eval-Print Loop):

```bash
./algolang
```

Try these commands:

```algo
> print 42
42
> let name = 100
> print name
100
> name + 50
150
```

### Hello World (Script)

Create a file named `hello.algo`:

```algo
# My first Algolang program
print 42
print true
print 3.14159
```

Run it:

```bash
./algolang hello.algo
```

Output:
```
42
true
3.14159
```

### Simple Function

Create `greet.algo`:

```algo
fn greet(name) {
  print name
}

let message = 100
greet(message)
```

Run:
```bash
./algolang greet.algo
```

Output:
```
100
```

---

## Language Fundamentals

### Variables

Declare variables with `let`:

```algo
let x = 10
let y = 20
let sum = x + y
print sum
```

Variables can be reassigned:

```algo
let counter = 0
counter = counter + 1
counter = counter + 1
print counter
```

### Data Types

Algolang has three primitive types:

**Numbers** (64-bit floating point):
```algo
let integer = 42
let decimal = 3.14159
let negative = -17
let scientific = 1.5
```

**Booleans**:
```algo
let yes = true
let no = false
let result = 10 > 5
print result
```

**Nil** (absence of value):
```algo
let empty = nil
```

### Operators

**Arithmetic**:
```algo
print 10 + 5      # Addition
print 10 - 5      # Subtraction
print 10 * 5      # Multiplication
print 10 / 5      # Division
print 10 % 3      # Modulo (remainder)
```

**Comparison**:
```algo
print 10 == 10    # Equal to
print 10 != 5     # Not equal to
print 5 < 10      # Less than
print 10 <= 10    # Less than or equal
print 15 > 10     # Greater than
print 10 >= 10    # Greater than or equal
```

**Logical**:
```algo
print true and false    # Logical AND
print true or false     # Logical OR
print !false           # Logical NOT
```

**Operator Precedence** (highest to lowest):
1. Parentheses: `()`
2. Unary: `!`, `-`
3. Multiplicative: `*`, `/`, `%`
4. Additive: `+`, `-`
5. Comparison: `<`, `<=`, `>`, `>=`
6. Equality: `==`, `!=`
7. Logical AND: `and`
8. Logical OR: `or`
9. Assignment: `=`

### Comments

Single-line comments start with `#`:

```algo
# This is a comment
let x = 10  # Inline comment

# Comments can explain your code
# They are ignored by the compiler
```

---

## Control Flow

### If Statements

**Simple if**:
```algo
let x = 10

if x > 5 {
  print true
}
```

**If-else**:
```algo
let age = 20

if age >= 18 {
  print true
} else {
  print false
}
```

**If-else-if chains**:
```algo
let score = 85

if score >= 90 {
  print 1
} else if score >= 80 {
  print 2
} else if score >= 70 {
  print 3
} else {
  print 4
}
```

**Nested conditions**:
```algo
let x = 15
let y = 20

if x > 10 {
  if y > 10 {
    print true
  }
}
```

### While Loops

**Basic loop**:
```algo
let i = 0
while i < 5 {
  print i
  i = i + 1
}
```

**Counting down**:
```algo
let countdown = 10
while countdown > 0 {
  print countdown
  countdown = countdown - 1
}
```

**Loop with condition**:
```algo
let sum = 0
let n = 1

while sum < 100 {
  sum = sum + n
  n = n + 1
}

print sum
print n
```

### Blocks

Blocks create new scopes:

```algo
let x = 10

{
  let x = 20
  print x    # Prints 20
}

print x      # Prints 10
```

---

## Functions

### Defining Functions

Basic function:
```algo
fn add(a, b) {
  return a + b
}

let result = add(5, 3)
print result
```

Function without parameters:
```algo
fn sayHello() {
  print 42
}

sayHello()
```

Function without explicit return:
```algo
fn printSquare(n) {
  print n * n
}

printSquare(7)
```

### Return Values

Functions can return values:

```algo
fn multiply(a, b) {
  return a * b
}

let product = multiply(6, 7)
print product
```

Early returns:
```algo
fn absolute(n) {
  if n < 0 {
    return -n
  }
  return n
}

print absolute(-42)
print absolute(42)
```

### Recursion

Functions can call themselves:

**Factorial**:
```algo
fn factorial(n) {
  if n <= 1 {
    return 1
  }
  return n * factorial(n - 1)
}

print factorial(5)
```

**Fibonacci**:
```algo
fn fib(n) {
  if n <= 1 {
    return n
  }
  return fib(n - 1) + fib(n - 2)
}

print fib(10)
```

**Greatest Common Divisor**:
```algo
fn gcd(a, b) {
  if b == 0 {
    return a
  }
  return gcd(b, a % b)
}

print gcd(48, 18)
```

### Scope

Variables are scoped to their blocks:

```algo
let global = 100

fn example() {
  let local = 200
  print global  # Can access global
  print local   # Can access local
}

example()
# print local would error - local is not accessible here
```

---

## Standard Library

Algolang includes built-in mathematical functions:

### Math Functions

**abs(x)** - Absolute value:
```algo
print abs(-42)     # 42
print abs(3.14)    # 3.14
```

**min(a, b)** - Minimum of two numbers:
```algo
print min(10, 20)  # 10
print min(-5, 3)   # -5
```

**max(a, b)** - Maximum of two numbers:
```algo
print max(10, 20)  # 20
print max(-5, 3)   # 3
```

**sqrt(x)** - Square root:
```algo
print sqrt(16)     # 4
print sqrt(2)      # 1.41421...
print sqrt(144)    # 12
```

**pow(x, y)** - Power (x^y):
```algo
print pow(2, 10)   # 1024
print pow(3, 3)    # 27
print pow(5, 2)    # 25
```

**floor(x)** - Round down:
```algo
print floor(3.7)   # 3
print floor(-2.3)  # -3
```

**ceil(x)** - Round up:
```algo
print ceil(3.2)    # 4
print ceil(-2.8)   # -2
```

### Using Standard Library

Combine functions for complex calculations:

```algo
let a = -15.7
let b = 23.2

print abs(a)                    # 15.7
print max(abs(a), abs(b))       # 23.2
print floor(a)                  # -16
print ceil(b)                   # 24
print sqrt(pow(3, 2) + pow(4, 2))  # 5 (Pythagorean theorem)
```

---

## Examples

### Example 1: Sum First N Numbers

```algo
fn sumN(n) {
  let sum = 0
  let i = 1
  
  while i <= n {
    sum = sum + i
    i = i + 1
  }
  
  return sum
}

print sumN(10)    # 55
print sumN(100)   # 5050
```

### Example 2: Check Prime Number

```algo
fn isPrime(n) {
  if n < 2 {
    return false
  }
  
  if n == 2 {
    return true
  }
  
  if n % 2 == 0 {
    return false
  }
  
  let i = 3
  while i * i <= n {
    if n % i == 0 {
      return false
    }
    i = i + 2
  }
  
  return true
}

print isPrime(2)      # true
print isPrime(17)     # true
print isPrime(100)    # false
print isPrime(97)     # true
```

### Example 3: Fibonacci Sequence

```algo
fn fib(n) {
  if n <= 1 {
    return n
  }
  
  let a = 0
  let b = 1
  let i = 2
  
  while i <= n {
    let temp = a + b
    a = b
    b = temp
    i = i + 1
  }
  
  return b
}

let i = 0
while i <= 10 {
  print fib(i)
  i = i + 1
}
```

### Example 4: Power Function (Iterative)

```algo
fn power(base, exp) {
  let result = 1
  let i = 0
  
  while i < exp {
    result = result * base
    i = i + 1
  }
  
  return result
}

print power(2, 10)   # 1024
print power(3, 4)    # 81
```

### Example 5: Collatz Conjecture

```algo
fn collatz(n) {
  let steps = 0
  
  while n != 1 {
    if n % 2 == 0 {
      n = n / 2
    } else {
      n = 3 * n + 1
    }
    steps = steps + 1
  }
  
  return steps
}

print collatz(27)    # 111
print collatz(10)    # 6
```

---

## Best Practices

### 1. Use Descriptive Names

**Good**:
```algo
let totalScore = 0
let playerCount = 10
```

**Bad**:
```algo
let ts = 0
let pc = 10
```

### 2. Initialize Variables

Always give variables an initial value:

```algo
let sum = 0       # Good
let count = 0     # Good
let value         # Bad - defaults to nil
```

### 3. Keep Functions Small

Each function should do one thing:

```algo
fn isEven(n) {
  return n % 2 == 0
}

fn countEvens(limit) {
  let count = 0
  let i = 0
  
  while i <= limit {
    if isEven(i) {
      count = count + 1
    }
    i = i + 1
  }
  
  return count
}
```

### 4. Comment Complex Logic

```algo
# Binary search requires sorted input
fn binarySearch(target, low, high) {
  while low <= high {
    let mid = floor((low + high) / 2)
    
    if mid == target {
      return mid
    } else if mid < target {
      low = mid + 1
    } else {
      high = mid - 1
    }
  }
  
  return -1
}
```

### 5. Prefer Iteration Over Recursion

**Recursive** (elegant but slower):
```algo
fn fib(n) {
  if n <= 1 {
    return n
  }
  return fib(n - 1) + fib(n - 2)
}
```

**Iterative** (faster, uses less memory):
```algo
fn fib(n) {
  if n <= 1 {
    return n
  }
  
  let a = 0
  let b = 1
  let i = 2
  
  while i <= n {
    let temp = a + b
    a = b
    b = temp
    i = i + 1
  }
  
  return b
}
```

### 6. Handle Edge Cases

```algo
fn divide(a, b) {
  if b == 0 {
    print 0
    return 0
  }
  return a / b
}

fn sqrt_safe(n) {
  if n < 0 {
    print 0
    return 0
  }
  return sqrt(n)
}
```

### 7. Test Your Code

Start with simple test cases:

```algo
fn add(a, b) {
  return a + b
}

# Test cases
print add(2, 3)      # Should print 5
print add(0, 0)      # Should print 0
print add(-5, 5)     # Should print 0
print add(100, 200)  # Should print 300
```

---

## Next Steps

### Run the Examples

Algolang includes several example programs:

```bash
# Basic examples
./algolang examples/hello.algo

# Fibonacci sequence
./algolang examples/fib.algo

# GCD algorithm
./algolang examples/gcd.algo

# Sorting algorithms
./algolang examples/sorting.algo

# Prime numbers
./algolang examples/primes.algo

# Run all examples
make test
```

### Read More Documentation

- **[Syntax Reference](syntax.md)** - Complete language grammar
- **[Bytecode Specification](bytecode.md)** - How the VM works
- **[Development Roadmap](roadmap.md)** - Future features

### Try These Exercises

1. **Sum of Squares**: Write a function that computes 1² + 2² + ... + n²
2. **Armstrong Numbers**: Check if a number equals the sum of its digits cubed
3. **Palindrome Numbers**: Check if a number reads the same forwards and backwards
4. **Binary to Decimal**: Convert binary numbers to decimal
5. **LCM Calculator**: Find the least common multiple using GCD

### Example Solution: Sum of Squares

```algo
fn sumOfSquares(n) {
  let sum = 0
  let i = 1
  
  while i <= n {
    sum = sum + i * i
    i = i + 1
  }
  
  return sum
}

print sumOfSquares(5)   # 1 + 4 + 9 + 16 + 25 = 55
print sumOfSquares(10)  # 385
```

---

## Getting Help

### Common Issues

**Q: `algolang: command not found`**  
A: Run `./algolang` from the project directory, or run `sudo make install`

**Q: Compilation errors**  
A: Ensure GCC 7+ is installed: `gcc --version`

**Q: Math functions not working**  
A: Make sure to link with `-lm` flag when compiling manually

### Resources

- **GitHub Repository**: [AlgoLangSource](https://github.com/AlgoLang-Programming-Language/AlgoLangSource)
- **Report Issues**: [GitHub Issues](https://github.com/AlgoLang-Programming-Language/AlgoLangSource/issues)
- **Documentation**: `docs/` directory in the repository

### Community

Join the Algolang community:
- Open issues for bugs or feature requests
- Submit pull requests to contribute
- Share your programs and algorithms
- Help improve documentation

---

## Summary

You've learned:
- ✅ How to install and run Algolang
- ✅ Basic syntax and data types
- ✅ Control flow (if, while)
- ✅ Functions and recursion
- ✅ Standard library functions
- ✅ Best practices and patterns

**Next**: Try implementing your favorite algorithms in Algolang!

---

**Happy coding!** 🚀

*Algolang v0.1.0 | Last Updated: December 2025*
