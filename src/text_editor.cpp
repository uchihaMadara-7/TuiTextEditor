#include <iostream>
#include "curses_tui.h"

#define EDITOR_START_Y 1
#define EDITOR_START_X 0

int main() {

    CursesWindow editor;
    editor.cbreak();
    editor.set_echo(false);
    editor.cursor_mode(CURSOR_INVISIBLE_HIGH);
    std::string heading = "[Text Editor]";
    int head_position = editor.get_width()/2 - heading.size()/2;
    editor.print(heading, 0, head_position);
    editor.move(EDITOR_START_Y, EDITOR_START_X);
    while (int c = getch()) {
        if (c == 'q') break;
        switch(c) {
            case KEY_LEFT:
                // Case-1 check if it can go out-of-bounds
                // Case-2 When on start of line, move y to y-1 and x to last character on line y-1
                editor.movex(editor.get_cursor_x()-1);
                break;
            case KEY_RIGHT:
                // Case-1 can't move beyond last character
                // Case-2 When on last of line, move y to y+1 and x to 0
                editor.movex(editor.get_cursor_x()+1);
                break;
            case KEY_UP:
                //  TODO check if it can go out-of-bounds (1st row)
                editor.movey(editor.get_cursor_y()-1);
                break;
            case KEY_DOWN:
                // TODO can't move beyond last line
                editor.movey(editor.get_cursor_y()+1);
                break;
            case KEY_BACKSPACE:
                // When on empty line, move y to y-1 and x to last character on line y-1
                if (editor.get_cursor_x() == 0) {
                    editor.movey(editor.get_cursor_y()-1);
                }
                editor.print(' ', editor.get_cursor_y(), editor.get_cursor_x()-1);
                editor.movex(editor.get_cursor_x()-1);
                break;
            case KEY_ENTER:
                // Case, if enter is pressed on middle of line (with words on left and right)
                // then, all the characters on right should move to next line
                editor.move(editor.get_cursor_y()+1, EDITOR_START_X);
                break;
            default: editor.print(c);
        }
    }
    return 0;
}