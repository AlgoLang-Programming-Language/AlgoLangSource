
<div align="center">

# Algolang

**A math-first, algorithm-focused programming language for clarity and deterministic execution**

---

### Badges (all dark-themed, high contrast)

<p> <img src="https://img.shields.io/github/actions/workflow/status/AlgoLang-Programming-Language/AlgoLangSource/build.yml?style=for-the-badge&logo=github&logoColor=white&color=0a0e14&label=build"> <img src="https://img.shields.io/github/v/release/AlgoLang-Programming-Language/AlgoLangSource?style=for-the-badge&logo=semantic-release&logoColor=white&color=0a0e14&label=version"> <img src="https://img.shields.io/github/license/AlgoLang-Programming-Language/AlgoLangSource?style=for-the-badge&logo=open-source-initiative&logoColor=white&color=020617"> <img src="https://img.shields.io/github/languages/top/AlgoLang-Programming-Language/AlgoLangSource?style=for-the-badge&logo=c&logoColor=white&color=0a0e14"> <img src="https://img.shields.io/github/contributors/AlgoLang-Programming-Language/AlgoLangSource?style=for-the-badge&logo=github&logoColor=white&color=0a0e14"> <img src="https://img.shields.io/github/issues/AlgoLang-Programming-Language/AlgoLangSource?style=for-the-badge&logo=github&logoColor=white&color=0a0e14&label=issues"> <img src="https://img.shields.io/github/issues-closed-raw/AlgoLang-Programming-Language/AlgoLangSource?style=for-the-badge&logo=github&logoColor=white&color=0a0e14&label=closed%20issues"> <img src="https://img.shields.io/github/issues-pr/AlgoLang-Programming-Language/AlgoLangSource?style=for-the-badge&logo=github&logoColor=white&color=0a0e14&label=PRs"> <img src="https://img.shields.io/github/issues-pr-closed/AlgoLang-Programming-Language/AlgoLangSource?style=for-the-badge&logo=github&logoColor=white&color=0a0e14&label=PRs%20closed"> <img src="https://img.shields.io/github/stars/AlgoLang-Programming-Language/AlgoLangSource?style=for-the-badge&logo=github&logoColor=white&color=0a0e14"> <img src="https://img.shields.io/github/forks/AlgoLang-Programming-Language/AlgoLangSource?style=for-the-badge&logo=github&logoColor=white&color=0a0e14"> <img src="https://img.shields.io/github/downloads/AlgoLang-Programming-Language/AlgoLangSource/total?style=for-the-badge&logo=github&logoColor=white&color=0a0e14"> <img src="https://img.shields.io/github/repo-size/AlgoLang-Programming-Language/AlgoLangSource?style=for-the-badge&logo=github&logoColor=white&color=0a0e14"> <img src="https://img.shields.io/github/last-commit/AlgoLang-Programming-Language/AlgoLangSource?style=for-the-badge&logo=github&logoColor=white&color=0a0e14"> </p> </div>

---

## ✦ Overview

Algolang is a **statically scoped, bytecode-compiled programming language** designed for **learning algorithms, computer science fundamentals, and mathematical reasoning**.

It emphasizes:

* **Clarity** – Code reads like the algorithm itself
* **Determinism** – Predictable, reproducible execution
* **Education** – Ideal for learning language design, algorithms, and computation

---

## ✦ Features

* **Clean Syntax** – Minimal, intuitive, focused on algorithms
* **Bytecode Compilation** – Efficient and predictable execution
* **Stack-Based VM** – Lightweight and inspectable
* **First-Class Functions** – Pass and return functions like values
* **Lexical Scoping** – Strict block-level variable rules
* **Standard Library** – Core mathematical functions (`abs`, `min`, `max`, `sqrt`, `pow`, `floor`, `ceil`)

---

## ✦ Quick Start

### Clone & Build

```bash
git clone https://github.com/AlgoLang-Programming-Language/AlgoLangSource.git
cd AlgoLangSource

# Build the compiler
make
```

Produces the `algolang` executable.

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

print add(5, 3)
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

### Data Types

* **Number** – 64-bit float (`42`, `3.14`)
* **Boolean** – `true`, `false`
* **Nil** – `nil` (no value)

### Operators

* Arithmetic: `+ - * / %`
* Comparison: `== != < <= > >=`
* Logical: `and or !`
* Assignment: `=`

---

## ✦ Standard Library

```algo
abs(x)   # Absolute value
min(a,b) # Minimum
max(a,b) # Maximum
sqrt(x)  # Square root
pow(x,y) # x^y
floor(x) # Round down
ceil(x)  # Round up
```

---

## ✦ Architecture

```
Source → Lexer → Parser → AST → Compiler → Bytecode → VM
```

| Component | Description             |
| --------- | ----------------------- |
| Lexer     | Tokenizes source code   |
| Parser    | Builds AST              |
| Compiler  | Converts AST → Bytecode |
| VM        | Stack-based execution   |
| Runtime   | Values & memory         |
| Stdlib    | Built-in functions      |

---

## ✦ Project Structure

```
algolang/
├── include/
├── src/
│   ├── lexer/
│   ├── parser/
│   ├── bytecode/
│   ├── vm/
│   ├── runtime/
│   └── stdlib/
├── examples/
├── docs/
├── Makefile
└── README.md
```

---

## ✦ Philosophy

* Clarity over Cleverness
* Correctness over Performance
* Simplicity over Features
* Education First

---

## ✦ Contributing

* Readable code & clear commits
* Educational clarity is priority
* All contributions welcome

Repository: [GitHub](https://github.com/AlgoLang-Programming-Language/AlgoLangSource)

---

## ✦ License

MIT License – See `LICENSE`

---

## ✦ Version

`0.1.0`



Do you want me to do that next?
