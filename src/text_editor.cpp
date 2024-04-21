#include <iostream>
#include "editor.h"

int main() {

    Editor& editor = Editor::getInstance();
    while (int c = getch()) {
        if (c == 'q') break;
        switch(c) {
            case KEY_LEFT:
                // Case-1 check if it can go out-of-bounds
                // Case-2 When on start of line, move y to y-1 and x to last character on line y-1
                editor.left_arrow();
                break;
            case KEY_RIGHT:
                // Case-1 can't move beyond last character
                // Case-2 When on last of line, move y to y+1 and x to 0
                editor.right_arrow();
                break;
            case KEY_UP:
                //  TODO check if it can go out-of-bounds (1st row)
                editor.up_arrow();
                break;
            case KEY_DOWN:
                // TODO can't move beyond last line
                editor.down_arrow();
                break;
            case KEY_BACKSPACE:
                // When on empty line, move y to y-1 and x to last character on line y-1
                editor.backspace();
                break;
            case KEY_ENTER:
                // Case, if enter is pressed on middle of line (with words on left and right)
                // then, all the characters on right should move to next line
                editor.enter_key();
                break;
            default:
                // Case-1, writing in middle of text (shift all to right)
                editor.write(c);
        }
    }
    // editor.all();
    // getch();
    return 0;
}
