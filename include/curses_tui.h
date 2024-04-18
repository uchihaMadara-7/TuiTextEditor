#ifndef __CURSES_TUI_H__
#define __CURSES_TUI_H__

#include <ncurses.h>
#include <sstream>

#define CURSOR_INVISIBLE 0
#define CURSOR_VISIBLE_NORMAL 1
#define CURSOR_INVISIBLE_HIGH 2

#ifdef LINUX
    /* Redefintion of macros based on system */
    #define KEY_BACKSPACE 127
    #define KEY_ENTER 10
#endif

class CursesWindow {
public:
    CursesWindow();
    ~CursesWindow();

    void init();
    void cbreak();
    void set_echo(bool flag);
    void cursor_mode(int mode);
    void move(int y, int x);
    void movex(int x);
    void movey(int y);
    void get_dimension(int &height, int &width);
    int get_height();
    int get_width();
    int get_cursor_x();
    int get_cursor_y();
    void print(char c);
    void print(char c, int y, int x);
    void print(std::string msg);
    void print(std::string msg, int y, int x);

    template<typename T>
    static auto convert(const T& arg) {
        if constexpr(std::is_same_v<T, std::string>) {
            return arg.c_str();
        } else {
            return arg;
        }
    }

    template<typename... Args>
    void print(std::string format, Args&&... args) {
        ::printw(format.c_str(), convert(std::forward<Args>(args))...);
    }

private:
    static bool m_initialized;
    WINDOW* m_win;
    int m_height;
    int m_width;
};

#endif