#include "editor.h"

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
    window.print(heading, 0, head_position);
    window.move(EDITOR_START_Y, EDITOR_START_X);
}

void Editor::left_arrow() {
    window.movex(window.get_cursor_x()-1);
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
    if (window.get_cursor_x() == 0) {
        window.movey(window.get_cursor_y()-1);
    }
    window.print(' ', window.get_cursor_y(), window.get_cursor_x()-1);
    window.movex(window.get_cursor_x()-1);
}

void Editor::enter_key() {
    window.move(window.get_cursor_y()+1, EDITOR_START_X);
}

void Editor::write(int c) {
    window.print(c);
}