# TuiTextEditor

Text-based User Interface (TUI) Editor (like vim, nano) in C++

# Build TuiTextEditor

## build without debug logs

```Shell
~$ make vector
```

## build with debug logs

Log file will be generated in project root directory with name `editor.log`

```Shell
~$ make vector DEBUG=1
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
├── vector_ds.o
└── commands.o


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

# Usage/Interface Details
## modes
```
command mode (:)
normal mode (Esc)
insert mode (a or i)
```
## valid commands in command mode
<pre>
w    (description: for saving file)
     (<b>Note:</b> addtionaly asks for filename if no file is linked to buffer)
q    (description: quit tui)
wq   (description: save and quit tui)
     (<b>Note:</b> addtionaly asks for filename if no file is linked to buffer)
</pre>
