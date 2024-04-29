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
#define EDITOR_START_COL 0

#define LOG_DS Editor::getInstance()._log_ds

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

    void left_arrow();
    void right_arrow();
    void down_arrow();
    void up_arrow();

    void backspace();
    void enter_key();

    void re_render(const std::string &text);

    void write(int c);
    void _log_ds();

private:
    CursesWindow window;
    DATA_STRUCTURE ds_db;
};

#endif // __EDITOR_H__
