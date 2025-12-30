#!/bin/bash

set -e

echo "========================================="
echo "  Building Algolang v0.1.0"
echo "========================================="
echo ""

if ! command -v gcc &> /dev/null; then
    echo "Error: GCC not found. Please install GCC to build Algolang."
    exit 1
fi

echo "Cleaning previous build..."
make clean 2>/dev/null || true

echo ""
echo "Compiling Algolang..."
make

if [ $? -eq 0 ]; then
    echo ""
    echo "========================================="
    echo "  Build Successful!"
    echo "========================================="
    echo ""
    echo "Executable: ./algolang"
    echo ""
    echo "Quick start:"
    echo "  ./algolang                    # Start REPL"
    echo "  ./algolang examples/fib.algo  # Run example"
    echo "  make test                     # Run all examples"
    echo ""
else
    echo ""
    echo "========================================="
    echo "  Build Failed"
    echo "========================================="
    exit 1
fi
