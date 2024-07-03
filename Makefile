# Compiler options
CXX = g++

CXXFLAGS = -std=c++20
# DEBUG SYMBOLS TO BINARY
# CXXFLAGS += -g

WARNINGS = -Wno-macro-redefined
WARNINGS += -Wall
WARNINGS += -Wextra
# WARNINGS += -Wno-unused-command-line-argument
CXXFLAGS += $(WARNINGS)

INCLUDES = -Iinclude
CXXFLAGS += $(INCLUDES)

LDFLAGS = -lncurses

MACROS = -DLINUX

VECTOR_DS_BUILD = build-vector
LOG_FILE = editor.log

SOURCE_DIR = src

VECTOR_DS_SRC = $(SOURCE_DIR)/tui.cpp
VECTOR_DS_SRC += $(SOURCE_DIR)/utils.cpp
VECTOR_DS_SRC += $(SOURCE_DIR)/curses_tui.cpp
VECTOR_DS_SRC += $(SOURCE_DIR)/editor.cpp
VECTOR_DS_SRC += $(SOURCE_DIR)/vector_ds.cpp
VECTOR_DS_SRC += $(SOURCE_DIR)/logger.cpp

OBJS = $(patsubst $(SOURCE_DIR)/%.cpp, $(VECTOR_DS_BUILD)/%.o, $(VECTOR_DS_SRC))
# $(info $(OBJS))

VECTOR_DS_TARGET = $(VECTOR_DS_BUILD)/tui

all: $(VECTOR_DS_BUILD) $(VECTOR_DS_TARGET)

$(VECTOR_DS_BUILD):
	mkdir -p $(VECTOR_DS_BUILD)

$(VECTOR_DS_BUILD)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(MACROS) -c $< -o $@

$(VECTOR_DS_TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $(VECTOR_DS_TARGET)

clean:
	rm -rf $(VECTOR_DS_BUILD)
	rm -rf $(LOG_FILE)

.PHONY: all clean
