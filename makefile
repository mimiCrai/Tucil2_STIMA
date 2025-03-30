# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -O2

# Folders
SRC_DIR = src
BIN_DIR = bin

# Target binary
TARGET = $(BIN_DIR)/main

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Compile and link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp into bin/*.o
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure bin directory exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Run the compiled binary
run: all
	./$(TARGET)

# Clean up
clean:
	rm -rf $(BIN_DIR)
