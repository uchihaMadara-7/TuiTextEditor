#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "curses_tui.h"

#ifdef DOUBLY_LIST_DS
    #include "doubly_list.h"
    #define DATA_STRUCTURE DoublyList2D

/* SOME_OTHER_DS is placehold for some other DS */
#elif defined(SOME_OTHER_DS)

#else
    #include "vector_ds.h"
    #define DATA_STRUCTURE VectorDS

#endif /* DOUBLY_LIST_DS and other directive checks */

#define SPACE ' '
#define EDITOR_START_ROW 1
#define EDITOR_START_COL 2
#define EDITOR_END_ROW_OFFSET 2
#define EDITOR_END_COL_OFFSET 0

#define NORMAL_MODE_STR "---- NORMAL MODE ----"
#define INSERT_MODE_STR "---- INSERT MODE ----"
#define COMMAND_MODE_STR "---- COMMAND MODE ----"
#define INVALID_COMMAND "---- INVALID COMMAND ----"

#define LOG_DS Editor::getInstance()._log_ds

#define CHECK_NOT_INSERT_MODE if (m_mode != INSERT_MODE) return;

enum EditorMode {
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

    void set_mode(EditorMode mode);
    EditorMode get_mode();

    void left_arrow();
    void right_arrow();
    void down_arrow();
    void up_arrow();

    void backspace();
    void enter_key();

    void re_render(const std::string &text);
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

    void _log_ds();

private:
    CursesWindow m_window, m_banner_win, m_command_win, m_line_win;
    DATA_STRUCTURE m_ds_db;
    EditorMode m_mode;
};

#endif // __EDITOR_H__
