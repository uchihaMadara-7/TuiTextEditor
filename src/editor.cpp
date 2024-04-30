#include "editor.h"
#include "logger.h"

Editor::Editor() {
    init();
}

Editor::~Editor() {}

void Editor::init() {
    m_window.create_window(LINES - EDITOR_END_ROW_OFFSET,
        COLS - EDITOR_END_COL_OFFSET,
        EDITOR_START_ROW, EDITOR_START_COL);

    m_window.cbreak();
    m_window.set_echo(false);
    m_window.cursor_mode(CURSOR_INVISIBLE_HIGH);

    /* Banner window - No updates during runtime */
    m_banner_win.create_window(1, COLS, 0, 0);
    std::string heading = "[Tui Text Editor]";
    int head_position = m_window.get_width()/2 - heading.size()/2;
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
            std::string document = m_ds_db.get_document();
            re_render(document);
            m_window.move(row-1, length);
        }
    }
    else {
        m_ds_db.delete_col(row, col - 1);
        std::string document = m_ds_db.get_document();
        re_render(document);
        m_window.move(row, col-1);
    }
}

void Editor::enter_key() {
    CHECK_NOT_INSERT_MODE
    int row = m_window.get_cursor_y();
    int col = m_window.get_cursor_x();
    m_ds_db.insert_row(row, col);

    /* Re-render everything after this enter position */
    std::string document = m_ds_db.get_document();
    re_render(document);
    m_window.move(row + 1, 0);
}

void Editor::re_render(const std::string &text) {
    m_window.clear();
    m_window.print(0, 0, text);
}

void Editor::reset_cursor() {
    /* TODO: last known position instead of 0, 0 */
    m_window.move(0, 0);
}

void Editor::set_line_placeholder() {
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
    std::string document = m_ds_db.get_document();
    re_render(document);
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
    m_command_win.clear();
    m_command_win.print(0, 0, msg);
}

void Editor::command_mode_banner() {
    if (get_mode() == COMMAND_MODE) _print_command_banner(COMMAND_MODE_STR);
    else if (get_mode() == NORMAL_MODE) _print_command_banner(NORMAL_MODE_STR);
    else _print_command_banner(INSERT_MODE_STR);
    reset_cursor();
}

void Editor::invalid_command() {
    _print_command_banner(INVALID_COMMAND);
    reset_cursor();
}

void Editor::_log_ds() {
    m_ds_db.printDS();
}
