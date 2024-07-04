# TuiTextEditor
Text-based User Interface (TUI) Editor (like vim, nano) in C++

# Build TuiTextEditor
```Shell
~$ make
mkdir -p build_vector
g++ -std=c++20 -Wno-macro-redefined -Wall -Wextra -Iinclude -DLINUX -c src/text_editor.cpp -o build_vec/tui.o
g++ -std=c++20 -Wno-macro-redefined -Wall -Wextra -Iinclude -DLINUX -c src/curses_tui.cpp -o build_vec/curses_tui.o
g++ -std=c++20 -Wno-macro-redefined -Wall -Wextra -Iinclude -DLINUX -c src/editor.cpp -o build_vec/editor.o
g++ -std=c++20 -Wno-macro-redefined -Wall -Wextra -Iinclude -DLINUX -c src/vector_ds.cpp -o build_vec/vector_ds.o
g++ -std=c++20 -Wno-macro-redefined -Wall -Wextra -Iinclude -DLINUX -c src/logger.cpp -o build_vec/logger.o
g++ -lncurses build_vec/text_editor.o build_vec/curses_tui.o build_vec/editor.o build_vec/vector_ds.o build_vec/logger.o -o build_vec/text_editor
```

# Build Directory
For vector Data-Structure, build directory will be `build_vec`
```Shell
~$ tree build_vector
build_vector
├── curses_tui.o
├── editor.o
├── logger.o
├── tui
├── tui.o
└── vector_ds.o

1 directory, 6 files
```

# Run/Execute
Binary/executable is generated undere build directory
In case of vector DS, it is `build_vector/tui`

## with no file, open empty buffer
```Shell
~$ build_vector/tui
```
## with file, open from filepath
```Shell
~$ build-vector/tui <filepath>
```

# Log file
Log file will be generated in project root directory with name `editor.log`

# Usage/Interface Details
