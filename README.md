
---

<div align="center">

<!-- LOGO / TITLE -->

<h1>Algolang</h1>

<p>
<b>a math-first, algorithm-centric programming language</b><br>
clarity • determinism • education
</p>

<!-- BADGES (DARK THEME OPTIMIZED) -->

<p>
  <img src="https://img.shields.io/badge/status-experimental-0a0e14?style=for-the-badge&logo=beaker&logoColor=white">
  <img src="https://img.shields.io/badge/version-0.1.0-0a0e14?style=for-the-badge&logo=semantic-release&logoColor=white">
  <img src="https://img.shields.io/badge/language-C-0a0e14?style=for-the-badge&logo=c&logoColor=white">
</p>

<p>
  <img src="https://img.shields.io/badge/compiler-bytecode-111827?style=for-the-badge">
  <img src="https://img.shields.io/badge/vm-stack--based-111827?style=for-the-badge">
  <img src="https://img.shields.io/badge/scoping-lexical-111827?style=for-the-badge">
</p>

<p>
  <img src="https://img.shields.io/badge/license-MIT-020617?style=for-the-badge&logo=open-source-initiative&logoColor=white">
  <img src="https://img.shields.io/badge/platform-linux%20%7C%20macOS-020617?style=for-the-badge&logo=linux&logoColor=white">
</p>

</div>

---

## ✦ Overview

**Algolang** is a statically scoped, bytecode-compiled programming language designed for **learning algorithms, computer science fundamentals, and mathematical reasoning**.

It strips programming down to what matters:

* expressing algorithms clearly
* understanding how languages work internally
* predictable, deterministic execution

Algolang is not designed to replace production languages —
it is designed to **teach how they work**.

---

## ✦ Why Algolang?

Most languages optimize for convenience.
Algolang optimizes for **understanding**.

| Principle   | Description                                     |
| ----------- | ----------------------------------------------- |
| Clarity     | Code should read like the algorithm itself      |
| Determinism | No hidden behavior, no undefined magic          |
| Minimalism  | Small core, high expressiveness                 |
| Education   | Designed to be read, traced, and reasoned about |

---

## ✦ Features

* **Clean, Minimal Syntax**
  Focused on algorithmic expression without boilerplate

* **Bytecode Compilation**
  Source code compiles to compact, predictable bytecode

* **Custom Stack-Based Virtual Machine**
  Simple execution model, easy to inspect and extend

* **Lexical (Static) Scoping**
  Clear block-based variable rules

* **First-Class Functions**
  Functions are values that can be passed and returned

* **Built-in Math-Focused Standard Library**
  Designed for numerical reasoning and algorithms

---

## ✦ Example

```algo
fn fib(n) {
  if n <= 1 {
    return n
  }
  return fib(n - 1) + fib(n - 2)
}

print fib(10)
```

Readable. Deterministic. Algorithm-first.

---

## ✦ Quick Start

### Build from Source

```bash
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

## ✦ Language Basics

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

---

## ✦ Data Types

* **Number** — 64-bit floating point
* **Boolean** — `true`, `false`
* **Nil** — `nil` (absence of value)

---

## ✦ Operators

* Arithmetic: `+` `-` `*` `/` `%`
* Comparison: `==` `!=` `<` `<=` `>` `>=`
* Logical: `and` `or` `!`
* Assignment: `=`

---

## ✦ Standard Library

Mathematical primitives built into the runtime:

* `abs(x)`
* `min(a, b)`
* `max(a, b)`
* `sqrt(x)`
* `pow(x, y)`
* `floor(x)`
* `ceil(x)`

---

## ✦ Architecture

```
Source Code
   ↓
Lexer
   ↓
Parser
   ↓
AST
   ↓
Compiler
   ↓
Bytecode
   ↓
Virtual Machine
```

### Core Components

| Component | Description             |
| --------- | ----------------------- |
| Lexer     | Tokenizes source code   |
| Parser    | Recursive descent → AST |
| Compiler  | AST → bytecode          |
| VM        | Stack-based execution   |
| Runtime   | Values & memory         |
| Stdlib    | Native math functions   |

---

## ✦ Project Structure

```
algolang/
├── include/          # Public headers
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

## ✦ Building & Testing

```bash
make          # build
make test     # run tests
make clean    # clean artifacts
sudo make install
```

---

## ✦ Documentation

* `docs/syntax.md` — full language reference
* `docs/bytecode.md` — VM instruction set
* `docs/roadmap.md` — future plans

---

## ✦ Philosophy

Algolang follows four non-negotiable rules:

1. **Clarity over cleverness**
2. **Correctness over performance**
3. **Simplicity over features**
4. **Education above all**

If a feature makes the language harder to understand, it does not belong.

---

## ✦ Roadmap

Planned improvements:

* arrays & simple collections
* richer standard library
* better error diagnostics
* debugging & tracing tools
* optional compile-time optimizations

---

## ✦ Contributing

Contributions are welcome.

This project values:

* readable code
* clear commit messages
* well-explained design decisions

Educational clarity matters more than raw feature count.

---

## ✦ License

MIT License
See `LICENSE` for details.

---

<div align="center">

**Algolang v0.1.0** <i>a language built to be understood</i>

</div>

