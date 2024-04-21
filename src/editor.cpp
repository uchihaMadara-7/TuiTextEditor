#include "editor.h"
#include <assert.h>

Editor::Editor() {
    init();
}

Editor::~Editor() {
    /* Free memory allocated */
    while (current_row) {
        current_col = current_row->head;
        while (current_col) {
            charNode *temp_col = current_col;
            current_col = current_col->next;
            delete temp_col;
        }
        lineNode *temp_row = current_row;
        current_row = current_row->next;
        delete temp_row;
    }
}

void Editor::init() {
    current_row = new lineNode();
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
    charNode *temp_col = current_col;

    /* At beginning of row, not pointing to any node */
    if (!current_col) {
        if (current_row->prev) {
            lineNode *temp_row = current_row;
            current_row = current_row->prev;
            current_col = current_row->prev->tail;
            delete temp_row;
        }
    }
    /* Delete node at front */
    else if (current_row->head == current_col) {
        /* If node is only node in row */
        if (current_row->tail == current_col) {
            current_row->head = current_row->tail = nullptr;
        }
        /* Deleting front but row is not empty */
        else {
            current_row->head = current_col->next;
            /* Safety check but shouldn't required if logic is correct */
            if (current_col->next) current_col->next->prev = nullptr;
        }
        current_col = nullptr;
        delete temp_col;
        current_row->length -= 1;
    }
    /* Delete node in between */
    else if (current_col->prev && current_col->next) {
        current_col->prev->next = current_col->next;
        current_col->next->prev = current_col->prev;
        current_col = current_col->prev;
        delete temp_col;
        current_row->length -= 1;
    }
    /* Delete last node in row (min of 2 nodes) */
    else if (!current_col->next) {
        current_col->prev->next = nullptr;
        current_row->tail = current_col->prev;
        current_col = current_col->prev;
        delete temp_col;
        current_row->length -= 1;
    }
    /* Shouldn't be possible, if yes, check the logic */
    else {
        assert(0);
    }

    if (window.get_cursor_x() == 0) {
        if (window.get_cursor_y() > EDITOR_START_Y)
            window.move(window.get_cursor_y()-1, current_row->length);
    }
    else {
        window.print(window.get_cursor_y(), window.get_cursor_x()-1, ' ');
        window.movex(window.get_cursor_x()-1);
    }
}

void Editor::enter_key() {
    window.move(window.get_cursor_y()+1, EDITOR_START_X);
}

void Editor::write(int c) {
    charNode *temp_col = new charNode(c);

    /* Adding node at the beginning */
    if (!current_col) {
        /* First ever node in row */
        if (!current_row->head) {
            current_row->head = temp_col;
            current_row->tail = temp_col;
        }
        /* Adding to front but row is not empty */
        else {
            temp_col->next = current_row->head;
            temp_col->next->prev = temp_col;
            current_row->head = temp_col;
        }
    }

    /* Adding node in middle */
    else if (current_col && current_col->next) {
        temp_col->next = current_col->next;
        temp_col->next->prev = temp_col;
        current_col->next = temp_col;
        temp_col->prev = current_col;
    }

    /* Adding node at the end */
    else if (!current_col->next) {
        current_row->tail = temp_col;
        current_col->next = temp_col;
        temp_col->prev = current_col;
    }
    /* Shouldn't be possible, if yes, check the logic */
    else {
        assert(0);
    }

    current_row->length += 1;
    current_col = temp_col;
    window.print(temp_col->value);
}

void Editor::all() {
    window.move(8, 0);
    while (current_row) {
        current_col = current_row->head;
        while (current_col) {
            window.print(current_col->value);
            current_col = current_col->next;
        }
        window.print(" (%d)", current_row->length);
        window.movey(window.get_cursor_y()-1);
        current_row = current_row->next;
    }
}
