CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -O2
LDFLAGS = -lm

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = .

SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/lexer/lexer.c \
          $(SRC_DIR)/parser/parser.c \
          $(SRC_DIR)/parser/ast.c \
          $(SRC_DIR)/bytecode/compiler.c \
          $(SRC_DIR)/vm/vm.c \
          $(SRC_DIR)/runtime/value.c \
          $(SRC_DIR)/stdlib/stdlib.c

OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

TARGET = $(BIN_DIR)/algolang

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)/lexer
	@mkdir -p $(BUILD_DIR)/parser
	@mkdir -p $(BUILD_DIR)/bytecode
	@mkdir -p $(BUILD_DIR)/vm
	@mkdir -p $(BUILD_DIR)/runtime
	@mkdir -p $(BUILD_DIR)/stdlib

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Clean complete"

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	@echo "Running examples..."
	@./$(TARGET) examples/hello.algo
	@echo ""
	@./$(TARGET) examples/fib.algo
	@echo ""
	@./$(TARGET) examples/gcd.algo

install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/algolang
