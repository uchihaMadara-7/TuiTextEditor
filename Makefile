# Compiler options
CXX = g++

CXXFLAGS = -std=c++20

WARNINGS = -Wno-macro-redefined
WARNINGS += -Wall
WARNINGS += -Wextra
# WARNINGS += -Wno-unused-command-line-argument
CXXFLAGS += $(WARNINGS)

INCLUDES = -Iinclude
CXXFLAGS += $(INCLUDES)

LDFLAGS = -lncurses

MACROS = -DLINUX

BUILD = build

SOURCE_DIR = src
SRC = src/text_editor.cpp
SRC += src/curses_tui.cpp
SRC += src/editor.cpp

OBJS = $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD)/%.o, $(SRC))
# $(info $(OBJS))

TARGET = $(BUILD)/text_editor

all: $(BUILD) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $(TARGET)

$(BUILD)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(MACROS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)

.PHONY: all clean