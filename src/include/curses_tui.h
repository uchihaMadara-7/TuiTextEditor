/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __CURSES_TUI_H__
#define __CURSES_TUI_H__

/* standard imports */
/* C imports first */
#include <ncurses.h>

/* C++ imports */
#include <iostream>
#include <string>
#include <utility>

/* custom imports */
#include "logger.h"

#define CURSOR_INVISIBLE 0
#define CURSOR_VISIBLE_NORMAL 1
#define CURSOR_INVISIBLE_HIGH 2

#ifdef LINUX
    /* Redefintion of macros based on system */
    #define KEY_BACKSPACE 127
    #define KEY_ENTER 10
#endif

#define KEY_ESC 27
#define KEY_COMMAND 58
#define KEY_SPACE 32
#define KEY_INSERT_1 105
#define KEY_INSERT_2 97

#define LINE_COLOR 1
#define INVALID_COLOR 2

class CursesWindow {
 public:
    CursesWindow();
    ~CursesWindow();

    void init();
    void create_window(int rows, int cols, int row_start, int col_start);
    void cbreak();
    void set_echo(bool flag);
    void cursor_mode(int mode);
    void clear();
    void move(int y, int x);
    void movex(int x);
    void movey(int y);

    void get_dimension(int &height, int &width);
    int get_height();
    int get_width();
    int get_cursor_x();
    int get_cursor_y();
    WINDOW* get_window();

    int read();
    void print(int c);
    void print(int y, int x, int c);
    void print(std::string msg);
    void print(int y, int x, std::string msg);

    template<typename T>
    static auto convert(const T& arg) {
        if constexpr(std::is_same_v<T, std::string>) {
            return arg.c_str();
        }
        return arg;
    }

    template<typename... Args>
    void print(std::string format, Args&&... args) {
        ::wprintw(m_win, format.c_str(), convert(std::forward<Args>(args))...);
        ::wrefresh(m_win);
    }

 private:
    static bool m_initialized;
    WINDOW* m_win;
    int m_height;
    int m_width;
};

#endif /* __CURSES_TUI_H__ */
