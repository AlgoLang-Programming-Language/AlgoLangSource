# Algolang Bytecode Specification

## Overview

Algolang compiles to a custom bytecode format executed by a stack-based virtual machine. This document describes the instruction set, execution model, and bytecode structure.

## Virtual Machine Architecture

### Stack Machine

Algolang uses a **stack-based architecture** similar to the JVM, Lua VM, and Python bytecode:

- Operations pop operands from the stack
- Results are pushed back onto the stack
- No explicit registers

### Call Frames

Function calls create new **call frames** containing:

- Function pointer
- Instruction pointer (IP)
- Stack slot pointer (local variables base)

Maximum call depth: **64 frames**

### Value Stack

- Maximum stack size: **256 values**
- Each value is a tagged union (8 bytes on 64-bit systems)
- Values can be: Number, Boolean, Nil, or Object reference

## Instruction Set

### Constants and Literals

#### OP_CONSTANT (0x00)
**Format**: `OP_CONSTANT <index>`

Pushes a constant from the constant pool onto the stack.

```
[constant_index] → value
```

**Example**:
```
OP_CONSTANT 0    # Push constants[0]
```

#### OP_NIL (0x01)
**Format**: `OP_NIL`

Pushes nil value onto the stack.

```
[] → nil
```

#### OP_TRUE (0x02)
**Format**: `OP_TRUE`

Pushes boolean true onto the stack.

```
[] → true
```

#### OP_FALSE (0x03)
**Format**: `OP_FALSE`

Pushes boolean false onto the stack.

```
[] → false
```

### Stack Manipulation

#### OP_POP (0x04)
**Format**: `OP_POP`

Removes top value from the stack.

```
[value] → []
```

### Variable Access

#### OP_GET_LOCAL (0x05)
**Format**: `OP_GET_LOCAL <slot>`

Pushes local variable from stack slot onto the stack.

```
[] → value
```

**Slot**: Relative to current frame's base pointer.

#### OP_SET_LOCAL (0x06)
**Format**: `OP_SET_LOCAL <slot>`

Stores top of stack into local variable slot (leaves value on stack).

```
[value] → [value]
```

#### OP_GET_GLOBAL (0x07)
**Format**: `OP_GET_GLOBAL <name_index>`

Pushes global variable value onto the stack.

```
[] → value
```

**name_index**: Index into constant pool (must be a string).

#### OP_DEFINE_GLOBAL (0x08)
**Format**: `OP_DEFINE_GLOBAL <name_index>`

Defines a new global variable and pops initial value from stack.

```
[value] → []
```

#### OP_SET_GLOBAL (0x09)
**Format**: `OP_SET_GLOBAL <name_index>`

Updates existing global variable (leaves value on stack).

```
[value] → [value]
```

### Comparison Operations

#### OP_EQUAL (0x0A)
**Format**: `OP_EQUAL`

Compares two values for equality.

```
[a, b] → [a == b]
```

#### OP_GREATER (0x0B)
**Format**: `OP_GREATER`

Greater than comparison (numeric only).

```
[a, b] → [a > b]
```

#### OP_LESS (0x0C)
**Format**: `OP_LESS`

Less than comparison (numeric only).

```
[a, b] → [a < b]
```

### Arithmetic Operations

#### OP_ADD (0x0D)
**Format**: `OP_ADD`

Addition (numeric only).

```
[a, b] → [a + b]
```

#### OP_SUBTRACT (0x0E)
**Format**: `OP_SUBTRACT`

Subtraction (numeric only).

```
[a, b] → [a - b]
```

#### OP_MULTIPLY (0x0F)
**Format**: `OP_MULTIPLY`

Multiplication (numeric only).

```
[a, b] → [a * b]
```

#### OP_DIVIDE (0x10)
**Format**: `OP_DIVIDE`

Division (numeric only).

```
[a, b] → [a / b]
```

#### OP_MODULO (0x11)
**Format**: `OP_MODULO`

Modulo operation (numeric only).

```
[a, b] → [a % b]
```

### Unary Operations

#### OP_NOT (0x12)
**Format**: `OP_NOT`

Logical negation.

```
[value] → [!value]
```

**Falsey values**: `nil` and `false`

#### OP_NEGATE (0x13)
**Format**: `OP_NEGATE`

Numeric negation.

```
[value] → [-value]
```

### Control Flow

#### OP_JUMP (0x15)
**Format**: `OP_JUMP <offset_high> <offset_low>`

Unconditional jump forward by offset bytes.

```
IP += offset
```

**Offset**: 16-bit unsigned integer (big-endian).

#### OP_JUMP_IF_FALSE (0x16)
**Format**: `OP_JUMP_IF_FALSE <offset_high> <offset_low>`

Conditional jump: if top of stack is falsey, jump forward.

**Does not pop the value.**

```
[value] → [value]
if (falsey) IP += offset
```

#### OP_LOOP (0x17)
**Format**: `OP_LOOP <offset_high> <offset_low>`

Jump backward by offset bytes (used for loops).

```
IP -= offset
```

### Function Operations

#### OP_CALL (0x18)
**Format**: `OP_CALL <arg_count>`

Calls a function with arg_count arguments.

```
[callee, arg1, arg2, ...] → [result]
```

**Stack layout before call**:
```
[... callee arg1 arg2 arg3 ...]
          ^
       stack_top - arg_count - 1
```

Creates new call frame with:
- Function from stack
- Arguments already on stack
- Return address

#### OP_RETURN (0x19)
**Format**: `OP_RETURN`

Returns from current function with value on stack.

```
[return_value] → [return_value]
```

**Behavior**:
1. Pop return value
2. Close call frame
3. Restore previous frame's IP
4. Push return value

If returning from top-level script, terminates execution.

### I/O Operations

#### OP_PRINT (0x14)
**Format**: `OP_PRINT`

Prints top of stack to stdout and pops it.

```
[value] → []
```

## Bytecode File Structure

While Algolang currently compiles and executes in one pass, the bytecode structure is designed for potential serialization:

```
Chunk {
    u8[] code              // Instruction stream
    i32[] lines            // Line number for each instruction
    Value[] constants      // Constant pool
}

Function {
    i32 arity              // Number of parameters
    ObjString* name        // Function name (or NULL for script)
    Chunk chunk            // Bytecode chunk
}
```

## Compilation Examples

### Simple Expression

**Source**:
```algo
print 2 + 3
```

**Bytecode**:
```
OP_CONSTANT 0      # Push 2
OP_CONSTANT 1      # Push 3
OP_ADD             # Add them
OP_PRINT           # Print result
OP_NIL             # Script return value
OP_RETURN          # Return from script
```

### Variable Declaration

**Source**:
```algo
let x = 10
print x
```

**Bytecode**:
```
OP_CONSTANT 0          # Push 10
OP_DEFINE_GLOBAL 1     # Define "x" (string in constants[1])
OP_GET_GLOBAL 1        # Get "x"
OP_PRINT               # Print it
OP_NIL
OP_RETURN
```

### If Statement

**Source**:
```algo
if x > 5 {
  print true
} else {
  print false
}
```

**Bytecode**:
```
OP_GET_GLOBAL 0        # Get x
OP_CONSTANT 1          # Push 5
OP_GREATER             # x > 5
OP_JUMP_IF_FALSE 8     # Jump to else if false
OP_POP                 # Pop condition
OP_TRUE                # Then branch
OP_PRINT
OP_JUMP 6              # Jump over else
OP_POP                 # Else branch (pop condition)
OP_FALSE
OP_PRINT
```

### While Loop

**Source**:
```algo
while x < 10 {
  x = x + 1
}
```

**Bytecode**:
```
# Loop start (offset 0)
OP_GET_GLOBAL 0        # Get x
OP_CONSTANT 1          # Push 10
OP_LESS                # x < 10
OP_JUMP_IF_FALSE 15    # Exit if false
OP_POP                 # Pop condition
OP_GET_GLOBAL 0        # Get x
OP_CONSTANT 2          # Push 1
OP_ADD                 # x + 1
OP_SET_GLOBAL 0        # Store to x
OP_POP                 # Pop result
OP_LOOP 17             # Jump back to start
OP_POP                 # Pop condition (after exit)
```

### Function Call

**Source**:
```algo
fn add(a, b) {
  return a + b
}
add(3, 5)
```

**Bytecode for `add` function**:
```
OP_GET_LOCAL 0         # Get parameter 'a'
OP_GET_LOCAL 1         # Get parameter 'b'
OP_ADD                 # Add them
OP_RETURN              # Return result
```

**Main script bytecode**:
```
OP_CONSTANT 0          # Push function object
OP_DEFINE_GLOBAL 1     # Define "add"
OP_GET_GLOBAL 1        # Get "add"
OP_CONSTANT 2          # Push 3
OP_CONSTANT 3          # Push 5
OP_CALL 2              # Call with 2 args
OP_POP                 # Discard result
```

## Optimization Opportunities

Current implementation is straightforward. Potential optimizations:

1. **Constant folding**: Evaluate constant expressions at compile time
2. **Dead code elimination**: Remove unreachable code
3. **Peephole optimization**: Replace instruction sequences with faster equivalents
4. **Register allocation**: Convert to register-based VM for fewer stack operations
5. **Tail call optimization**: Reuse stack frame for tail-recursive calls
6. **Inline caching**: Cache global variable lookups

## Comparison with Other VMs

| Feature | Algolang | Lua | Python | JVM |
|---------|----------|-----|--------|-----|
| Architecture | Stack | Register | Stack | Stack |
| Max Stack | 256 | 250 | ~1000 | 65535 |
| Instruction Size | 1-3 bytes | 4 bytes | 1-3 bytes | 1-3 bytes |
| Typing | Dynamic | Dynamic | Dynamic | Static |
| GC | Simple mark | Incremental | Generational | Generational |

## Disassembler Output Format

A bytecode disassembler would output:

```
== <script> ==
0000  OP_CONSTANT         0  '2'
0002  OP_CONSTANT         1  '3'
0004  OP_ADD
0005  OP_PRINT
0006  OP_NIL
0007  OP_RETURN
```

Format:
- Offset in hex
- Instruction name
- Operand(s) if any
- Constant value if applicable
