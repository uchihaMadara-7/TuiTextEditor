/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __EDITOR_H__
#define __EDITOR_H__

/* standard imports */
#include <fstream>
#include <string>
#include <vector>
#include <utility>

/* custom imports */
#include "curses_tui.h"
#include "vector_ds.h"

#define DATA_STRUCTURE VectorDS

#ifdef DOUBLY_LIST_DS
    #include "doubly_list.h"
    #define DATA_STRUCTURE DoublyList2D
#endif /* DOUBLY_LIST_DS */

/* #if defined(SOME_OTHER_DS) */
#ifdef VECTOR_DS
    #include "vector_ds.h"
    #define DATA_STRUCTURE VectorDS
#endif /* VECTOR_DS */

#define SPACE ' '
#define EDITOR_START_ROW 1
#define EDITOR_START_COL 2
#define EDITOR_END_ROW_OFFSET 2
#define EDITOR_END_COL_OFFSET 0

#define NORMAL_MODE_STR "---- NORMAL MODE ----"
#define INSERT_MODE_STR "---- INSERT MODE ----"
#define COMMAND_MODE_STR "---- COMMAND MODE ----"
#define SAVE_FILE_STR "---- FILE SAVED ----"
#define NO_FILE_STR "---- NO FILE OPENED ----"
#define INVALID_COMMAND "---- INVALID COMMAND ----"

#define LOG_DS Editor::getInstance()._log_ds

#define CHECK_NOT_INSERT_MODE if (m_mode != EditorMode::INSERT_MODE) return;

extern std::string editor_filename;

enum class Error {
    NO_FILE,
    NO_ERROR,
};

enum class EditorMode {
    INSERT_MODE,
    NORMAL_MODE,
    COMMAND_MODE
};

class Editor {
 public:
    Editor();
    ~Editor();

    /* Delete the copy constructor and copy operator */
    Editor(const Editor&) = delete;
    Editor& operator=(const Editor&) = delete;

    /* Make Editor class as Singleton */
    static Editor& getInstance() {
        static Editor instance;
        return instance;
    }

    void init();
    bool is_initialized();

    bool set_file(std::string filename);
    Error save_file();
    void set_mode(EditorMode mode);
    EditorMode get_mode();
    void remove_file();
    bool get_remove_required();

    void left_arrow();
    void right_arrow();
    void down_arrow();
    void up_arrow();

    void backspace();
    void enter_key();

    void re_render();
    void reset_cursor();
    void set_line_placeholder();

    int read();
    void write(int c);

    void enter_command_mode();
    void command_write(int c);
    void clear_command_mode();
    void _print_command_banner(std::string);
    void command_mode_banner();
    void invalid_command();

    /* buffer to screen mapping  */
    void set_buf_col(int col);
    void set_buf_row(int row);
    void update_buf_col(int delta_col);
    void update_buf_row(int delta_row);

    void _log_ds();

 private:
    void set_init(bool);

 private:
    CursesWindow m_window, m_banner_win, m_command_win, m_line_win;
    DATA_STRUCTURE m_ds_db;
    EditorMode m_mode;
    std::fstream m_file_handler;
    std::string m_filename;
    bool m_initialised = false;
    bool m_newfile = false;
    bool m_saved_once = false;
    /* Mapping between screen and buffer */
    size_t m_buf_row = 0;
    size_t m_buf_col = 0;
    std::vector<std::vector<std::pair<size_t, size_t>>> buf_to_screen;
};

#endif /* __EDITOR_H__ */
