# Algolang

**A math-first, algorithm-focused programming language designed for clarity and deterministic execution.**

Algolang is a statically-scoped, bytecode-compiled language built for learning algorithms, computer science education, and mathematical reasoning. It prioritizes readability, correctness, and algorithmic clarity.

## Features

- **Clean Syntax**: Minimal, intuitive syntax focused on algorithm expression
- **Bytecode Compilation**: Source code is compiled to efficient bytecode
- **Stack-based VM**: Fast execution on a custom virtual machine
- **First-class Functions**: Functions are values that can be passed around
- **Lexical Scoping**: Variables follow block scope rules
- **Standard Library**: Built-in mathematical functions (abs, min, max, sqrt, pow, floor, ceil)

## Quick Start

### Building from Source

```bash
make
```

This produces the `algolang` executable.

### Running Programs

Execute a file:
```bash
./algolang examples/fib.algo
```

Start the REPL:
```bash
./algolang
```

### Example Program

```algo
fn fib(n) {
  if n <= 1 {
    return n
  }
  return fib(n - 1) + fib(n - 2)
}

print fib(10)
```

## Language Overview

### Variables

```algo
let x = 10
let y = 20
let sum = x + y
```

### Functions

```algo
fn add(a, b) {
  return a + b
}

let result = add(5, 3)
print result
```

### Control Flow

```algo
if x > 10 {
  print true
} else {
  print false
}

while x < 100 {
  x = x * 2
}
```

### Operators

- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
- Logical: `and`, `or`, `!`
- Assignment: `=`

### Data Types

- **Numbers**: 64-bit floating point (e.g., `42`, `3.14`)
- **Booleans**: `true`, `false`
- **Nil**: `nil` (absence of value)

### Standard Library

- `abs(x)` - Absolute value
- `min(a, b)` - Minimum of two numbers
- `max(a, b)` - Maximum of two numbers
- `sqrt(x)` - Square root
- `pow(x, y)` - x raised to the power y
- `floor(x)` - Round down to integer
- `ceil(x)` - Round up to integer

## Architecture

Algolang follows a classic compiler pipeline:

```
Source Code → Lexer → Parser → AST → Compiler → Bytecode → VM
```

### Components

1. **Lexer** (`src/lexer/`): Tokenizes source code
2. **Parser** (`src/parser/`): Builds Abstract Syntax Tree using recursive descent
3. **Compiler** (`src/bytecode/`): Generates bytecode from AST
4. **Virtual Machine** (`src/vm/`): Executes bytecode on a stack-based VM
5. **Runtime** (`src/runtime/`): Value representation and memory management
6. **Standard Library** (`src/stdlib/`): Built-in native functions

## Project Structure

```
algolang/
├── include/          # Header files
├── src/              # Source code
│   ├── lexer/
│   ├── parser/
│   ├── bytecode/
│   ├── vm/
│   ├── runtime/
│   └── stdlib/
├── examples/         # Example programs
├── docs/             # Documentation
├── Makefile          # Build configuration
└── README.md
```

## Building and Testing

```bash
# Build the compiler
make

# Run tests
make test

# Clean build artifacts
make clean

# Install system-wide (requires sudo)
sudo make install
```

## Requirements

- C compiler (GCC or Clang)
- Standard C library
- Math library (libm)

## Documentation

- [Syntax Guide](docs/syntax.md) - Complete language reference
- [Bytecode Specification](docs/bytecode.md) - VM instruction set
- [Roadmap](docs/roadmap.md) - Future development plans

## Examples

See the `examples/` directory for sample programs:

- `hello.algo` - Basic syntax demonstration
- `fib.algo` - Fibonacci sequence (recursive and iterative)
- `gcd.algo` - Greatest Common Divisor algorithm

## Philosophy

Algolang is designed with these principles:

1. **Clarity over Cleverness**: Code should be self-documenting
2. **Correctness over Performance**: Deterministic, predictable behavior
3. **Simplicity over Features**: Minimal core, maximum expressiveness
4. **Education First**: Ideal for learning algorithms and language implementation

## License

MIT License - See LICENSE file for details

## Contributing

Contributions are welcome! This is an educational project focused on language design and implementation.

## Version

Current version: 0.1.0
