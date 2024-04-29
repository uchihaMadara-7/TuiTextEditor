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

}

void Editor::enter_key() {
    int row = window.get_cursor_y() - EDITOR_START_ROW;
    int col = window.get_cursor_x() - EDITOR_START_COL;
    ds_db.insert_row(row, col);

    /* Re-render everything after this enter position */
    std::string document = ds_db.get_document();
    re_render(document);
    window.move(EDITOR_START_ROW + row + 1, EDITOR_START_COL);
}

void Editor::re_render(const std::string &text) {
    window.move(EDITOR_START_ROW, EDITOR_START_COL);
    for (int c : text) {
        window.print(c);
    }
}

void Editor::write(int c) {
    int row = window.get_cursor_y() - EDITOR_START_ROW;
    int col = window.get_cursor_x() - EDITOR_START_COL;
    ds_db.insert_col(row, col, c);
    window.print(c);
}

void Editor::_log_ds() {
    ds_db.printDS();
}
