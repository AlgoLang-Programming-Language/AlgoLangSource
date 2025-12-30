# Getting Started with Algolang

Welcome to Algolang! This guide will help you get up and running quickly.

## Installation

### Prerequisites

- **C Compiler**: GCC or Clang
- **Make**: Build automation tool
- **Standard C Library**: Usually pre-installed
- **Math Library**: libm (usually included with GCC)

### Building from Source

1. **Clone or download the Algolang repository**

2. **Navigate to the project directory**:
   ```bash
   cd algolang
   ```

3. **Build the executable**:
   ```bash
   make
   ```
   
   Or use the build script:
   ```bash
   chmod +x build.sh
   ./build.sh
   ```

4. **Verify installation**:
   ```bash
   ./algolang
   ```
   
   You should see the REPL prompt:
   ```
   Algolang 0.1.0
   Type 'exit' to quit
   
   >
   ```

### Platform-Specific Notes

**Linux**:
```bash
make
sudo make install  # Optional: install system-wide
```

**macOS**:
```bash
make
# May need to install command line tools first:
# xcode-select --install
```

**Windows (MinGW/MSYS2)**:
```bash
make
# Or manually compile:
gcc -Wall -Wextra -std=c11 -Iinclude -O2 src/**/*.c -o algolang.exe -lm
```

## Your First Program

### Using the REPL

The REPL (Read-Eval-Print Loop) is great for experimentation:

```bash
./algolang
```

Try some expressions:
```algo
> 2 + 2
4
> let x = 10
> x * 5
50
> print x
10
```

### Writing a Script

Create a file `hello.algo`:

```algo
# My first Algolang program
fn greet(name) {
  print name
}

let message = 42
greet(message)
```

Run it:
```bash
./algolang hello.algo
```

## Language Basics

### Variables

Declare variables with `let`:

```algo
let x = 10
let y = 20
let sum = x + y
```

Variables are dynamically typed and can be reassigned:

```algo
let x = 10
x = 20
x = x + 5
```

### Numbers

All numbers are 64-bit floating point:

```algo
let integer = 42
let decimal = 3.14159
let negative = -17
let calculation = (5 + 3) * 2
```

### Booleans

```algo
let flag = true
let condition = false
let result = 10 > 5
```

### Printing

Use `print` to output values:

```algo
print 42
print true
print 2 + 2
```

### Comments

Single-line comments start with `#`:

```algo
# This is a comment
let x = 10  # Inline comment
```

## Control Flow

### If Statements

```algo
let x = 10

if x > 5 {
  print true
}

if x < 5 {
  print false
} else {
  print true
}

if x < 5 {
  print 1
} else if x < 15 {
  print 2
} else {
  print 3
}
```

### While Loops

```algo
let i = 0
while i < 10 {
  print i
  i = i + 1
}
```

Example: Sum first 10 numbers
```algo
let sum = 0
let i = 1

while i <= 10 {
  sum = sum + i
  i = i + 1
}

print sum
```

## Functions

### Defining Functions

```algo
fn add(a, b) {
  return a + b
}

let result = add(5, 3)
print result
```

### Functions without Return

```algo
fn printSquare(n) {
  print n * n
}

printSquare(7)
```

### Recursive Functions

```algo
fn factorial(n) {
  if n <= 1 {
    return 1
  }
  return n * factorial(n - 1)
}

print factorial(5)
```

## Operators

### Arithmetic
- `+` Addition
- `-` Subtraction
- `*` Multiplication
- `/` Division
- `%` Modulo

```algo
print 10 + 5
print 10 - 5
print 10 * 5
print 10 / 5
print 10 % 3
```

### Comparison
- `==` Equal
- `!=` Not equal
- `<` Less than
- `<=` Less than or equal
- `>` Greater than
- `>=` Greater than or equal

```algo
print 10 == 10
print 10 != 5
print 5 < 10
print 10 >= 10
```

### Logical
- `and` Logical AND
- `or` Logical OR
- `!` Logical NOT

```algo
print true and false
print true or false
print !false
```

## Standard Library

Algolang includes mathematical functions:

```algo
# Absolute value
print abs(-42)

# Min and max
print min(10, 20)
print max(10, 20)

# Square root
print sqrt(16)

# Power
print pow(2, 10)

# Rounding
print floor(3.7)
print ceil(3.2)
```

## Complete Example Programs

### Fibonacci Sequence

```algo
fn fib(n) {
  if n <= 1 {
    return n
  }
  return fib(n - 1) + fib(n - 2)
}

let i = 0
while i <= 10 {
  print fib(i)
  i = i + 1
}
```

### Prime Number Checker

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

## Running the Examples

Algolang comes with several example programs:

```bash
# Basic examples
./algolang examples/hello.algo

# Fibonacci (recursive and iterative)
./algolang examples/fib.algo

# GCD and LCM
./algolang examples/gcd.algo

# Sorting algorithms
./algolang examples/sorting.algo

# Prime numbers
./algolang examples/primes.algo

# Run all examples
make test
```

## Tips and Best Practices

### 1. Use Meaningful Variable Names

**Good**:
```algo
let sum = 0
let counter = 1
```

**Bad**:
```algo
let s = 0
let c = 1
```

### 2. Keep Functions Small

Each function should do one thing well:

```algo
fn calculateSum(n) {
  let sum = 0
  let i = 1
  while i <= n {
    sum = sum + i
    i = i + 1
  }
  return sum
}
```

### 3. Use Iterative Over Recursive When Possible

Recursion is elegant but can be slower and use more memory:

**Recursive** (simple but slower):
```algo
fn fib(n) {
  if n <= 1 {
    return n
  }
  return fib(n - 1) + fib(n - 2)
}
```

**Iterative** (faster):
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

### 4. Initialize Variables

Always initialize variables:

```algo
let sum = 0  # Good
let count    # Bad - defaults to nil
```

### 5. Test Edge Cases

Always test your functions with edge cases:

```algo
fn divide(a, b) {
  if b == 0 {
    print 0  # Handle division by zero
    return 0
  }
  return a / b
}
```

## Common Patterns

### Accumulator Pattern

```algo
fn sum(n) {
  let total = 0
  let i = 1
  while i <= n {
    total = total + i
    i = i + 1
  }
  return total
}
```

### Counter Pattern

```algo
fn countEvens(limit) {
  let count = 0
  let i = 0
  while i <= limit {
    if i % 2 == 0 {
      count = count + 1
    }
    i = i + 1
  }
  return count
}
```

### Search Pattern

```algo
fn find(target, limit) {
  let i = 0
  while i <= limit {
    if i == target {
      return i
    }
    i = i + 1
  }
  return -1
}
```

## Debugging Tips

### Print Variable Values

```algo
let x = 10
print x  # See what x is

let y = x * 2
print y  # Check intermediate calculations
```

### Break Down Complex Expressions

Instead of:
```algo
let result = (a + b) * (c - d) / (e + f)
```

Do:
```algo
let sum1 = a + b
let diff = c - d
let sum2 = e + f
let result = sum1 * diff / sum2
```

### Test Functions Independently

```algo
fn helper(x) {
  return x * 2
}

# Test it first
print helper(5)  # Should print 10

fn main() {
  let result = helper(10)
  print result
}

main()
```

## Next Steps

1. **Read the syntax guide**: See `docs/syntax.md` for complete language reference
2. **Study the examples**: Look at programs in `examples/` directory
3. **Try the exercises**: Write your own algorithms
4. **Explore the bytecode**: Read `docs/bytecode.md` to understand how Algolang works

## Getting Help

- **Check the documentation**: `docs/` directory
- **Read error messages**: They usually point to the problem
- **Start with simple programs**: Build complexity gradually
- **Use the REPL**: Test expressions interactively

## What's Next?

Now that you know the basics, try:

1. Implementing classic algorithms (binary search, merge sort)
2. Solving mathematical problems (Project Euler)
3. Writing your own utility functions
4. Contributing to Algolang development

Happy coding!
