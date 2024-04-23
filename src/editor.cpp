#include "editor.h"
#include "logger.h"

#define ERROR_TRACE Logger::getInstance().error
#define INFO_TRACE Logger::getInstance().info
#define DEBUG_TRACE Logger::getInstance().debug

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
    window.move(EDITOR_START_Y, EDITOR_START_X);
}

void Editor::left_arrow() {
    if (window.get_cursor_x() == 0) {
        if (window.get_cursor_y() > EDITOR_START_Y)
            window.move(window.get_cursor_y()-1, window.get_width()-1);
    }
    else window.movex(window.get_cursor_x()-1);
}

void Editor::right_arrow() {
    window.movex(window.get_cursor_x()+1);
}

void Editor::down_arrow() {
    window.movey(window.get_cursor_y()+1);
}

void Editor::up_arrow() {
    window.movey(window.get_cursor_y()-1);
}

void Editor::backspace() {
    ds_db.delete_col();
    int length = ds_db.get_row_size();
    if (window.get_cursor_x() == 0) {
        if (window.get_cursor_y() > EDITOR_START_Y)
            window.move(window.get_cursor_y()-1, length);
    }
    else {
        window.print(window.get_cursor_y(), window.get_cursor_x()-1, SPACE);
        window.movex(window.get_cursor_x()-1);
    }
}

void Editor::enter_key() {
    ds_db.insert_row();
    window.move(window.get_cursor_y()+1, EDITOR_START_X);
}

void Editor::write(int c) {
    ds_db.insert_col(c);
    window.print(c);
}

void Editor::_log_ds() {

    lineNode *current_row = ds_db.get_first_row();
    while (current_row) {
        charNode *current_col = current_row->head;
        std::string row_str = "";
        while (current_col) {
            row_str.append(1, current_col->value);
            current_col = current_col->next;
        }
        DEBUG_TRACE("%s (%d)", row_str, current_row->length);
        current_row = current_row->next;
    }
}
