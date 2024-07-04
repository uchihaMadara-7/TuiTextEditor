/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard imports */
#include <cstdio>
#include <filesystem>
#include <utility>

/* custom imports */
#include "include/editor.h"
#include "include/logger.h"

Editor::Editor() {
    init();
}

Editor::~Editor() {
    if (m_file_handler.is_open()) {
        m_file_handler.close();
        DEBUG_TRACE("File closed!");
    }
}

void Editor::init() {
    /* Main text editor window */
    m_window.create_window(LINES - EDITOR_END_ROW_OFFSET - EDITOR_START_ROW,
        COLS - EDITOR_END_COL_OFFSET - EDITOR_START_COL,
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

    /* set initial mapping between buffer and screen */
    buf_to_screen = {{}};
    buf_to_screen[0].emplace_back(std::make_pair(0, 0));

    m_window.move(0, 0);
    set_mode(EditorMode::NORMAL_MODE);
    set_init(true);
}

bool Editor::is_initialized() {
    return m_initialised;
}

void Editor::set_init(bool value) {
    m_initialised = value;
}

bool  Editor::set_file(std::string filename) {
    /* Try opening file if possible */
    m_filename = filename;
    m_file_handler.open(m_filename, std::ios::in);
    if (m_file_handler.is_open()) {
        DEBUG_TRACE("Reading file!");
        std::string line_text;
        set_mode(EditorMode::INSERT_MODE);
        while (std::getline(m_file_handler, line_text)) {
            for (int c : line_text) write(c);
            enter_key();
        }
        set_mode(EditorMode::NORMAL_MODE);
        m_file_handler.close();
    }

    /* Re-opening in write mode: out|in doesn't destroy the existing content */
    if (std::filesystem::exists(filename)) {
        m_file_handler.open(m_filename, std::ios::out | std::ios::in);
    } else {
        /* file was give to tui as input, so can open as newfile */
        m_file_handler.open(m_filename, std::ios::out);
        m_newfile = true;
    }

    if (!m_file_handler.is_open()) {
        return false;
    }

    return true;
}

bool Editor::set_new_file(std::string filename) {
    /* file creation after opening tui,
     * cannot override exisiting file on the system
     */
    if (std::filesystem::exists(filename)) {
        _print_command_banner(EXISTING_FILE);
        return false;
    }
    m_filename = filename;
    m_file_handler.open(m_filename);
    m_newfile = true;
    if (!m_file_handler.is_open()) return false;
    return true;
}

Error Editor::save_file() {
    if (!m_file_handler.is_open()) {
        DEBUG_TRACE("No file was opened!");
        _print_command_banner("filename: ");
        return Error::NO_FILE;
    }

    DEBUG_TRACE("Saving file!");
    m_file_handler.seekp(std::ios::beg);
    int total_rows = m_ds_db.get_total_rows();
    for (int row=0; row < total_rows; ++row) {
        std::string row_str = m_ds_db.get_row(row);
        m_file_handler << row_str << std::endl;
    }
    m_saved_once = true;
    _print_command_banner(SAVE_FILE_STR);
    reset_cursor();
    return Error::NO_ERROR;
}

void Editor::remove_file() {
    std::remove(m_filename.c_str());
}

void Editor::set_mode(EditorMode mode)  {
    if (mode == EditorMode::INSERT_MODE) {
        DEBUG_TRACE("Insert mode activated!");
    } else if (mode == EditorMode::NORMAL_MODE) {
        DEBUG_TRACE("Normal mode activated!");
    } else {
        DEBUG_TRACE("Command mode activated!");
    }
    m_mode = mode;
}

EditorMode Editor::get_mode() {
    return m_mode;
}

bool Editor::get_remove_required() {
    return m_newfile && !m_saved_once;
}

void Editor::left_arrow() {
    update_buf_col(-1);
    reset_cursor();
}

void Editor::right_arrow() {
    update_buf_col(1);
    reset_cursor();
}

void Editor::down_arrow() {
    update_buf_row(1);
    reset_cursor();
}

void Editor::up_arrow() {
    update_buf_row(-1);
    reset_cursor();
}

void Editor::backspace() {
    CHECK_NOT_INSERT_MODE
    /* no more characters to delete */
    if (m_buf_col == 0 && m_buf_row == 0) {
        return;
    }

    if (m_buf_col == 0) {
        /* at the beginning of line */
        m_ds_db.delete_row(m_buf_row);
    } else {
        /* either middle or end of line */
        m_ds_db.delete_col(m_buf_row, m_buf_col-1);
    }
    re_render();
    /* update the buffer and screen co-ordinates */
    update_buf_col(-1);
    reset_cursor();
}

void Editor::enter_key() {
    CHECK_NOT_INSERT_MODE
    m_ds_db.insert_row(m_buf_row, m_buf_col);

    /* Re-render everything after this enter position */
    re_render();
    /* update the buffer and screen co-ordinates */
    set_buf_col(0);
    update_buf_row(1);
    reset_cursor();
}

/* TODO: Efficeint re-renders, only render from current position till last
    last character in the buffer
*/
void Editor::re_render() {
    m_window.clear();
    size_t width = m_window.get_width();
    size_t total_rows = m_ds_db.get_total_rows();
    size_t screen_row = 0;
    for (size_t row=0; row < total_rows; ++row) {
        std::string row_str = m_ds_db.get_row(row);
        /* update window with row_str */
        m_window.print(screen_row, 0, row_str);
        /* update the buf_to_screen matrix  */
        size_t row_size = row_str.size();

        /* if there are character(s) on row */
        for (size_t col=0; col <= row_size; ++col) {
            if (row >= buf_to_screen.size()) {
                buf_to_screen.emplace_back(std::vector<std::pair<size_t, size_t>>());
            }
            if (col >= buf_to_screen[row].size()) {
                buf_to_screen[row].emplace_back(std::make_pair(-1, -1));
            }
            /* add mapping from buffer to screen */
            buf_to_screen[row][col] = std::make_pair(
                screen_row+col/width, col%width);
        }
        /* move screen co-ordinate to next position  */
        // screen_row += std::ceil(row_size*1.0/width);
        screen_row += row_size/width + 1;
    }
}

void Editor::reset_cursor() {
    auto &x = buf_to_screen[m_buf_row][m_buf_col];
    m_window.move(x.first, x.second);
}

void Editor::set_line_placeholder() {
    /* Setting color to line window */
    init_pair(LINE_COLOR, COLOR_YELLOW, -1);
    ::wattron(m_line_win.get_window(), COLOR_PAIR(LINE_COLOR));
    int total_lines = m_line_win.get_height();
    for (int i=0; i < total_lines; ++i) {
        m_line_win.print('~');
    }
}

int Editor::read() {
    if (get_mode() == EditorMode::COMMAND_MODE) return m_command_win.read();
    return m_window.read();
}

void Editor::write(int c) {
    CHECK_NOT_INSERT_MODE
    m_ds_db.insert_col(m_buf_row, m_buf_col, c);
    re_render();
    /* update the buffer co-ordinates */
    update_buf_col(1);
    reset_cursor();
}

void Editor::enter_command_mode() {
    clear_command_mode();
    m_command_win.print(0, 0, KEY_COMMAND);
    set_mode(EditorMode::COMMAND_MODE);
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
    if (get_mode() == EditorMode::COMMAND_MODE) {
        _print_command_banner(COMMAND_MODE_STR);
    } else if (get_mode() == EditorMode::NORMAL_MODE) {
        _print_command_banner(NORMAL_MODE_STR);
    } else {
        _print_command_banner(INSERT_MODE_STR);
    }
    reset_cursor();
}

void Editor::invalid_command() {
    init_pair(INVALID_COLOR, COLOR_WHITE, COLOR_RED);
    ::wattron(m_command_win.get_window(), COLOR_PAIR(INVALID_COLOR));
    _print_command_banner(INVALID_COMMAND);
    wstandend(m_command_win.get_window());
    reset_cursor();
}

void Editor::set_buf_col(int col) {
    m_buf_col = col;
}

void Editor::set_buf_row(int row) {
    m_buf_row = row;
}

void Editor::update_buf_row(int delta_row) {
    int tmp_buf_row = static_cast<int>(m_buf_row) + delta_row;
    int total_rows = m_ds_db.get_total_rows();
    if (tmp_buf_row >= total_rows) {
        set_buf_row(total_rows-1);
    } else if (tmp_buf_row < 0) {
        set_buf_row(0);
    } else {
        set_buf_row(tmp_buf_row);
        size_t row_size = m_ds_db.get_row_size(m_buf_row);
        if (row_size < m_buf_col) {
            set_buf_col(row_size);
        }
    }
}

void Editor::update_buf_col(int delta_col) {
    int tmp_buf_col = static_cast<int>(m_buf_col) + delta_col;
    int row_size = m_ds_db.get_row_size(m_buf_row);
    if (tmp_buf_col > row_size) {
        if (m_buf_row == static_cast<size_t>(m_ds_db.get_total_rows()) - 1) return;
        set_buf_col(tmp_buf_col % (row_size+1));
        update_buf_row(1);
    } else if (tmp_buf_col < 0) {
        if (m_buf_row == 0) {
            set_buf_col(0);
            return;
        }
        update_buf_row(-1);
        set_buf_col(tmp_buf_col + m_ds_db.get_row_size(m_buf_row) + 1);
        set_buf_col(m_buf_col % (m_ds_db.get_row_size(m_buf_row) + 1));
    } else {
        set_buf_col(tmp_buf_col);
    }
}

void Editor::_log_ds() {
    int row = 0, col = 0;
    for (auto &vec : buf_to_screen) {
        for (auto x : vec) {
            DEBUG_TRACE("(%d, %d) -> (%d, %d)", row, col, x.first, x.second);
            ++col;
        }
        col = 0;
        ++row;
    }
    m_ds_db.printDS();
}
