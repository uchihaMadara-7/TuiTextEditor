#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "curses_tui.h"

#define SPACE ' '
#define EDITOR_START_Y 1
#define EDITOR_START_X 0

struct charNode {
    charNode *prev;
    charNode *next;
    int value;

    charNode() {
        prev = next = nullptr;
    }

    charNode(int value) {
        prev = next = nullptr;
        this->value = value;
    }
};

struct lineNode {
    lineNode *prev;
    lineNode *next;
    charNode *head;
    charNode *tail;
    int length;

    lineNode() {
        prev = next = nullptr;
        head = tail = nullptr;
        length = 0;
    }
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

    void left_arrow();
    void right_arrow();
    void down_arrow();
    void up_arrow();

    void backspace();
    void enter_key();

    void write(int c);
    void all();

private:
    CursesWindow window;
    lineNode *document = nullptr;
    lineNode *current_row = nullptr;
    charNode *current_col = nullptr;
};

#endif // __EDITOR_H__
