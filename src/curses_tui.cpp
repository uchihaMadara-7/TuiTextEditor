#include "curses_tui.h"

/* Initialize the static member */
bool CursesWindow::m_initialized = false;

CursesWindow::CursesWindow() {
    init();
    m_win = stdscr;
    get_dimension(m_height, m_width);
}

CursesWindow::~CursesWindow() {
    ::delwin(m_win);
    /* closes the window and restore the terminal settings */
    ::endwin();
}

void CursesWindow::init() {
    if (!m_initialized) {
        /* Initialises the screen */
        ::initscr();
        /* Allow handling of control characters like arrow keys, enter, ... */
        ::keypad(stdscr, TRUE);
        ::refresh();
        m_initialized = true;
    }
}

void CursesWindow::cbreak() {
    /*
     * One-character-a-time. To disable the buffering of typed characters by
     * the TTY driver and get a character-at-a-time input without cbreak(),
     * getch() will read input after newline (till then input will be queued)
     */
    // ::raw();
    ::cbreak();
}

void CursesWindow::set_echo(bool flag) {
    /* noecho(): To suppress the automatic echoing of typed characters */
    flag? ::echo() : ::noecho();
}

void CursesWindow::cursor_mode(int mode) {
    ::curs_set(mode);
}

void CursesWindow::move(int y, int x) {
    ::wmove(m_win, y, x);
}

void CursesWindow::movex(int x) {
    ::wmove(m_win, ::getcury(m_win), x);
}

void CursesWindow::movey(int y) {
    ::wmove(m_win, y, ::getcurx(m_win));
}

void CursesWindow::get_dimension(int &height, int &width) {
    height = ::getmaxy(m_win);
    width = ::getmaxx(m_win);
}

int CursesWindow::get_height() {
    return m_height;
}

int CursesWindow::get_width() {
    return m_width;
}

int CursesWindow::get_cursor_x() {
    return ::getcurx(m_win);
}

int CursesWindow::get_cursor_y() {
    return ::getcury(m_win);
}

void CursesWindow::print(char c) {
    ::waddch(m_win, c);
}

void CursesWindow::print(char c, int y, int x) {
    mvwaddch(m_win, y, x, c);
}

void CursesWindow::print(std::string msg) {
    ::waddstr(m_win, msg.c_str());
}

void CursesWindow::print(std::string msg, int y, int x) {
    mvwaddstr(m_win, y, x, msg.c_str());
}