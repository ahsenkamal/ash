# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Iinclude -g

# Directories
SRC_DIR = src
BUILD_DIR = build

# Files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
TARGET = $(BUILD_DIR)/ash

# Default rule
all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@
	rm -f $(BUILD_DIR)/*.o

# Compile .c files to .o files in build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

