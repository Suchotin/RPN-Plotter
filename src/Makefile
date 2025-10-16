CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
SRC = graph.c parser.c shunting_yard.c stack.c calculate.c render.c
BUILD_DIR = ../build
TARGET = graph.exe

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
