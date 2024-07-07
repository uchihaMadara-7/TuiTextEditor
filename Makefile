# Compiler options
CXX = g++

CXXFLAGS = -std=c++20
# DEBUG SYMBOLS TO BINARY
# CXXFLAGS += -g

WARNINGS = -Wno-macro-redefined
WARNINGS += -Wall
WARNINGS += -Wextra

CXXFLAGS += $(WARNINGS)

INCLUDES = -Iinclude
CXXFLAGS += $(INCLUDES)

LDFLAGS = -lncurses

MACROS = -DLINUX
ifeq ($(DEBUG),1)
	MACROS += -DDEBUG_FLAG
endif

VECTOR_DS_BUILD = build-vector
VECTOR_OBJ_BUILD = $(VECTOR_DS_BUILD)/objs
LOG_FILE = editor.log

SOURCE_DIR = src

SRC = $(SOURCE_DIR)/tui.cpp
SRC += $(SOURCE_DIR)/utils.cpp
SRC += $(SOURCE_DIR)/curses_tui.cpp
SRC += $(SOURCE_DIR)/editor.cpp
SRC += $(SOURCE_DIR)/logger.cpp
SRC += $(SOURCE_DIR)/commands.cpp

VECTOR_DS_SRC = $(SRC)
VECTOR_DS_SRC += $(SOURCE_DIR)/vector_ds.cpp

VECTOR_DS_TARGET = $(VECTOR_DS_BUILD)/tui

VOBJS = $(patsubst $(SOURCE_DIR)/%.cpp, $(VECTOR_OBJ_BUILD)/%.o, $(VECTOR_DS_SRC))

all: vector

# makefile rules for the vector data structure
vector: $(VECTOR_DS_BUILD) $(VECTOR_DS_TARGET)

$(VECTOR_DS_BUILD):
	$(eval MACROS += -DVECTOR_DS)
	mkdir -p $(VECTOR_DS_BUILD)
	mkdir -p $(VECTOR_OBJ_BUILD)

$(VECTOR_OBJ_BUILD)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(MACROS) -c $< -o $@

$(VECTOR_DS_TARGET): $(VOBJS)
	$(CXX) $(LDFLAGS) $(VOBJS) -o $(VECTOR_DS_TARGET)
# end makefile rules for the vector data structure

clean:
	rm -rf $(VECTOR_DS_BUILD)
	rm -rf $(LOG_FILE)

.PHONY: all clean
