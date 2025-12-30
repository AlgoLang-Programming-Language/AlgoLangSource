
---

<div align="center">

# Algolang

**A math-first, algorithm-focused programming language for clarity and deterministic execution**

<img src="https://img.shields.io/badge/status-experimental-0a0e14?style=for-the-badge&logo=beaker&logoColor=white">
<img src="https://img.shields.io/badge/version-0.1.0-0a0e14?style=for-the-badge&logo=semantic-release&logoColor=white">
<img src="https://img.shields.io/badge/language-C-0a0e14?style=for-the-badge&logo=c&logoColor=white">
<img src="https://img.shields.io/badge/license-MIT-020617?style=for-the-badge&logo=open-source-initiative&logoColor=white">

</div>

---

## ✦ Overview

Algolang is a **statically-scoped, bytecode-compiled programming language** designed for **learning algorithms, computer science fundamentals, and mathematical reasoning**.

It emphasizes:

* **Clarity**: code should read like the algorithm itself
* **Determinism**: predictable, reproducible execution
* **Education**: ideal for learning language design, algorithms, and mathematical computation

This language is perfect for students, educators, and developers who want to understand **how languages work internally**.

---

## ✦ Features

* **Clean Syntax**: Minimal and intuitive, focused on algorithmic expression
* **Bytecode Compilation**: Efficient and predictable execution
* **Stack-based VM**: Lightweight, fast, and easy to inspect
* **First-class Functions**: Functions can be passed, returned, and stored
* **Lexical Scoping**: Variables follow strict block-level rules
* **Standard Library**: Core mathematical functions (`abs`, `min`, `max`, `sqrt`, `pow`, `floor`, `ceil`)

---

## ✦ Quick Start

### Clone & Build

```bash
git clone https://github.com/AlgoLang-Programming-Language/AlgoLangSource.git
cd AlgoLangSource

# Build the compiler
make
```

This produces the `algolang` executable.

### Run a Program

```bash
./algolang examples/fib.algo
```

### Start the REPL

```bash
./algolang
```

---

## ✦ Example Program

```algo
fn fib(n) {
  if n <= 1 {
    return n
  }
  return fib(n - 1) + fib(n - 2)
}

print fib(10)
```

---

## ✦ Language Overview

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

* Arithmetic: `+`, `-`, `*`, `/`, `%`
* Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
* Logical: `and`, `or`, `!`
* Assignment: `=`

### Data Types

* **Numbers**: 64-bit floating point (`42`, `3.14`)
* **Booleans**: `true`, `false`
* **Nil**: `nil` (represents absence of value)

---

## ✦ Standard Library

```algo
abs(x)   # Absolute value
min(a,b) # Minimum
max(a,b) # Maximum
sqrt(x)  # Square root
pow(x,y) # x to the power y
floor(x) # Round down
ceil(x)  # Round up
```

---

## ✦ Architecture

```
Source Code → Lexer → Parser → AST → Compiler → Bytecode → VM
```

### Core Components

| Component | Description                                     |
| --------- | ----------------------------------------------- |
| Lexer     | Tokenizes source code                           |
| Parser    | Builds Abstract Syntax Tree (AST)               |
| Compiler  | Converts AST to bytecode                        |
| VM        | Stack-based virtual machine                     |
| Runtime   | Memory and value management                     |
| Stdlib    | Built-in mathematical and algorithmic functions |

---

## ✦ Project Structure

```
algolang/
├── include/          # Public headers
├── src/              # Compiler, VM, and runtime code
│   ├── lexer/
│   ├── parser/
│   ├── bytecode/
│   ├── vm/
│   ├── runtime/
│   └── stdlib/
├── examples/         # Example programs
├── docs/             # Documentation and specifications
├── Makefile          # Build instructions
└── README.md
```

---

## ✦ Building & Testing

```bash
# Build compiler
make

# Run tests
make test

# Clean build artifacts
make clean

# Install system-wide (requires sudo)
sudo make install
```

---

## ✦ Requirements

* GCC or Clang
* Standard C library
* Math library (`libm`)

---

## ✦ Documentation

* [Syntax Guide](docs/syntax.md) – Complete language reference
* [Bytecode Specification](docs/bytecode.md) – VM instruction set
* [Roadmap](docs/roadmap.md) – Future development plans

---

## ✦ Examples

* `hello.algo` – Basic syntax demonstration
* `fib.algo` – Fibonacci sequence (recursive & iterative)
* `gcd.algo` – Greatest common divisor algorithm

---

## ✦ Philosophy

1. **Clarity over Cleverness** – code should be self-documenting
2. **Correctness over Performance** – deterministic and predictable
3. **Simplicity over Features** – minimal core, maximum expressiveness
4. **Education First** – designed for learning algorithms and language design

---

## ✦ Contributing

Contributions are welcome! This project emphasizes:

* Readable, maintainable code
* Clear commits and explanations
* Educational clarity above all

Visit the repository: [AlgoLang GitHub](https://github.com/AlgoLang-Programming-Language/AlgoLangSource)

---

## ✦ License

MIT License – See `LICENSE` file for details

---

