#include "editor.h"
#include "logger.h"

Editor::Editor() {
    init();
}

Editor::~Editor() {}

void Editor::init() {
    /* Main text editor window */
    m_window.create_window(LINES - EDITOR_END_ROW_OFFSET,
        COLS - EDITOR_END_COL_OFFSET,
        EDITOR_START_ROW, EDITOR_START_COL);
    m_window.cbreak();
    m_window.set_echo(false);
    m_window.cursor_mode(CURSOR_INVISIBLE_HIGH);

    /* Banner window - No updates during runtime */
    m_banner_win.create_window(1, COLS, 0, 0);
    std::string heading = "[Tui Text Editor]";
    int head_position = m_banner_win.get_width()/2 - heading.size()/2;
    m_banner_win.print(0, head_position, heading);

    /* Command window - Only updates in command mode */
    m_command_win.create_window(1, COLS, LINES-1, 0);

    /* Line window */
    m_line_win.create_window(LINES - EDITOR_END_ROW_OFFSET, 1,
        EDITOR_START_ROW, 0);
    set_line_placeholder();

    m_window.move(0, 0);
    set_mode(NORMAL_MODE);
    // set_mode(INSERT_MODE);
}

void Editor::set_mode(EditorMode mode)  {
    m_mode = mode;
}

EditorMode Editor::get_mode() {
    return m_mode;
}

void Editor::left_arrow() {
    int row = m_window.get_cursor_y();
    int col = m_window.get_cursor_x();
    /* If starting of line is reached, resetting to end of previous line */
    if (col == 0) {
        /* Cannot move past the first line */
        if (row > 0) {
            int length = m_ds_db.get_row_size(row - 1);
            m_window.move(row - 1, length);
        }
    }
    else m_window.movex(col-1);
}

void Editor::right_arrow() {
    int row = m_window.get_cursor_y();
    int col = m_window.get_cursor_x();
    int length = m_ds_db.get_row_size(row);
    /* If end of line is reached, resetting to beginning of next line */
    if (col == length) {
        /* Cannot move past the last line */
        if ((row + 1) < m_ds_db.get_total_rows()) {
            m_window.move(row+1, 0);
        }
    }
    else m_window.movex(col+1);
}

void Editor::down_arrow() {
    int row = m_window.get_cursor_y();
    int col = m_window.get_cursor_x();

    /* Can only move if it is not the last row */
    if (row < (m_ds_db.get_total_rows() - 1)) {
        int length = m_ds_db.get_row_size(row + 1);
        m_window.move(row+1, std::min(col, length));
    }
}

void Editor::up_arrow() {
    int row = m_window.get_cursor_y();
    int col = m_window.get_cursor_x();

    /* Can only move if it is not the first row */
    if (row > 0) {
        int length = m_ds_db.get_row_size(row - 1);
        m_window.move(row-1, std::min(col, length));
    }
}

void Editor::backspace() {
    CHECK_NOT_INSERT_MODE
    int row = m_window.get_cursor_y();
    int col = m_window.get_cursor_x();

    /* If starting of line is reached, resetting to end of previous line */
    if (col == 0) {
        if (row > 0) {
            int length = m_ds_db.get_row_size(row - 1);
            m_ds_db.delete_row(row);

            re_render();
            m_window.move(row-1, length);
        }
    }
    else {
        m_ds_db.delete_col(row, col - 1);

        re_render();
        m_window.move(row, col-1);
    }
}

void Editor::enter_key() {
    CHECK_NOT_INSERT_MODE
    int row = m_window.get_cursor_y();
    int col = m_window.get_cursor_x();
    m_ds_db.insert_row(row, col);

    /* Re-render everything after this enter position */
    re_render();
    m_window.move(row + 1, 0);
}

void Editor::re_render() {
    int total_rows = m_ds_db.get_total_rows();
    for (int row=0; row<total_rows; ++row) {
        std::string row_str = m_ds_db.get_row(row);
        m_window.print(row, 0, row_str);
    }
}

void Editor::reset_cursor() {
    /* TODO: last known position instead of 0, 0 */
    m_window.move(0, 0);
}

void Editor::set_line_placeholder() {
    /* Setting color to line window */
    init_pair(LINE_COLOR, COLOR_YELLOW, -1);
    ::wattron(m_line_win.get_window(), COLOR_PAIR(LINE_COLOR));
    int total_lines = m_line_win.get_height();
    for (int i=0; i<total_lines; ++i) {
        m_line_win.print('~');
    }
}

int Editor::read() {
    if (get_mode() == COMMAND_MODE) return m_command_win.read();
    return m_window.read();
}

void Editor::write(int c) {
    CHECK_NOT_INSERT_MODE
    int row = m_window.get_cursor_y();
    int col = m_window.get_cursor_x();
    m_ds_db.insert_col(row, col, c);
    re_render();
    /* ncurses auto move to next line when line end is reached */
    m_window.move(row, col + 1);
}

void Editor::enter_command_mode() {
    clear_command_mode();
    m_command_win.print(0, 0, KEY_COMMAND);
    set_mode(COMMAND_MODE);
}

void Editor::command_write(int c) {
    m_command_win.print(c);
}

void Editor::clear_command_mode() {
    m_command_win.clear();
}

void Editor::_print_command_banner(std::string msg) {
    m_command_win.print(0, 0, msg);
}

void Editor::command_mode_banner() {
    if (get_mode() == COMMAND_MODE) _print_command_banner(COMMAND_MODE_STR);
    else if (get_mode() == NORMAL_MODE) _print_command_banner(NORMAL_MODE_STR);
    else _print_command_banner(INSERT_MODE_STR);
    reset_cursor();
}

void Editor::invalid_command() {
    init_pair(INVALID_COLOR, COLOR_WHITE, COLOR_RED);
    ::wattron(m_command_win.get_window(), COLOR_PAIR(INVALID_COLOR));
    _print_command_banner(INVALID_COMMAND);
    wstandend(m_command_win.get_window());
    reset_cursor();
}

void Editor::_log_ds() {
    m_ds_db.printDS();
}
