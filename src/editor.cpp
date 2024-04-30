#include "editor.h"
#include "logger.h"

Editor::Editor() {
    init();
}

Editor::~Editor() {}

void Editor::init() {
    window.cbreak();
    window.set_echo(false);
    window.cursor_mode(CURSOR_INVISIBLE_HIGH);
    std::string heading = "[Text Editor]";
    int head_position = window.get_width()/2 - heading.size()/2;
    window.print(0, head_position, heading);
    window.move(EDITOR_START_ROW, EDITOR_START_COL);
    set_mode(COMMAND_MODE);
}

void Editor::set_mode(EditorMode mode)  {
    this->mode = mode;
}

EditorMode Editor::get_mode() {
    return mode;
}

void Editor::left_arrow() {
    int row = window.get_cursor_y();
    int col = window.get_cursor_x();
    /* If starting of line is reached, resetting to end of previous line */
    if (col == EDITOR_START_COL) {
        /* Cannot move past the first line */
        if (row > EDITOR_START_ROW) {
            int length = ds_db.get_row_size(row-EDITOR_START_ROW-1);
            window.move(row-1, length);
        }
    }
    else window.movex(col-1);
}

void Editor::right_arrow() {
    int row = window.get_cursor_y();
    int col = window.get_cursor_x();
    int length = ds_db.get_row_size(row - EDITOR_START_ROW);
    /* If end of line is reached, resetting to beginning of next line */
    if (col == length) {
        /* Cannot move past the last line */
        if ((row - EDITOR_START_ROW + 1) < ds_db.get_total_rows()) {
            window.move(row+1, EDITOR_START_COL);
        }
    }
    else window.movex(col+1);
}

void Editor::down_arrow() {
    int row = window.get_cursor_y();
    int col = window.get_cursor_x();

    /* Can only move if it is not the last row */
    if (row < ds_db.get_total_rows()) {
        int length = ds_db.get_row_size(row - EDITOR_START_ROW + 1);
        window.move(row+1, std::min(col, length));
    }
}

void Editor::up_arrow() {
    int row = window.get_cursor_y();
    int col = window.get_cursor_x();

    /* Can only move if it is not the first row */
    if (row > EDITOR_START_ROW) {
        int length = ds_db.get_row_size(row - EDITOR_START_ROW - 1);
        window.move(row-1, std::min(col, length));
    }
}

void Editor::backspace() {
    CHECK_COMMAND_MODE
    int row = window.get_cursor_y();
    int col = window.get_cursor_x();

    /* If starting of line is reached, resetting to end of previous line */
    if (col == EDITOR_START_COL) {
        if (row > EDITOR_START_ROW) {
            int length = ds_db.get_row_size(row-EDITOR_START_ROW-1);
            ds_db.delete_row(row-EDITOR_START_ROW);
            std::string document = ds_db.get_document();
            re_render(document);
            window.move(row-1, length);
        }
    }
    else {
        ds_db.delete_col(row - EDITOR_START_ROW, col - EDITOR_START_COL-1);
        std::string document = ds_db.get_document();
        re_render(document);
        window.move(row, col-1);
    }
}

void Editor::enter_key() {
    CHECK_COMMAND_MODE
    int row = window.get_cursor_y() - EDITOR_START_ROW;
    int col = window.get_cursor_x() - EDITOR_START_COL;
    ds_db.insert_row(row, col);

    /* Re-render everything after this enter position */
    std::string document = ds_db.get_document();
    re_render(document);
    window.move(EDITOR_START_ROW + row + 1, EDITOR_START_COL);
}

void Editor::re_render(const std::string &text) {
    window.clear();
    window.move(EDITOR_START_ROW, EDITOR_START_COL);
    for (int c : text) {
        window.print(c);
    }
}

void Editor::write(int c) {
    CHECK_COMMAND_MODE
    int row = window.get_cursor_y() - EDITOR_START_ROW;
    int col = window.get_cursor_x() - EDITOR_START_COL;
    ds_db.insert_col(row, col, c);
    std::string document = ds_db.get_document();
    re_render(document);
    window.move(row + EDITOR_START_ROW, col + EDITOR_START_COL + 1);
    // window.print(c);
}

void Editor::command_mode() {
    int row = window.get_height() - 1;
    window.print(row, 0, KEY_COMMAND);
}

void Editor::command_write(int c) {
    window.print(c);
}

void Editor::clear_command_mode() {
    int row = window.get_height() - 1;
    int max_col = window.get_width() - 1;
    for (int i=0; i<max_col; ++i) {
        window.print(row, i, KEY_SPACE);
    }
    window.move(EDITOR_START_ROW, EDITOR_START_COL);
}

void Editor::command_banner() {
    clear_command_mode();
    int row = window.get_height() - 1;
    if (get_mode() == COMMAND_MODE) window.print(row, 0, "Command Mode entered!");
    else window.print(row, 0, "Insert Mode entered!");
    window.move(EDITOR_START_ROW, EDITOR_START_COL);
}

void Editor::_log_ds() {
    ds_db.printDS();
}
