#include "doubly_list.h"
#include <assert.h>

DoublyList2D::DoublyList2D() {
    document = new lineNode();
    current_row = document;
}

DoublyList2D::~DoublyList2D() {
    /* Free memory allocated */
    current_row = document;
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

void DoublyList2D::insert_row() {
    /* for creating newlines, check if new line inserted at beginning, middle or end */
    /* Consider same case for delete/backspace as well */
    // if ()
    /* Inserting row at the end */
    if (!current_row->next) {
        lineNode *temp_row = new lineNode();
        current_row->next = temp_row;
        temp_row->prev = current_row;
        /* TODO: Update length of rows as well */
        /* If current_col was pointing to null or before first element */
        if (!current_col) {
            current_col = nullptr;
        }
        /* If current_col was pointing to first element */
        else if (current_row->head == current_col) {
            // temp_row->head = current_col->
        }
        /* If current_col was pointing to some middle element */
        else {
            temp_row->head = current_col->next;
            temp_row->tail = current_row->tail;
            current_row->tail = current_col;
            current_col->next = nullptr;
            // current_col->nullptr;
        }
        current_row = temp_row;
    }
}

void DoublyList2D::delete_row() {
    lineNode *temp_row = current_row;

    /* First row, can't delete first row */
    if (current_row == document) {
        /* Do nothing - or do later */
    }
    /* Deleting row in between and last */
    else if (current_row->prev) {
        current_row->prev->next = current_row->next;
        /* This logic only works for delete in between */
        if (current_row->next) {
            current_row->next->prev = current_row->prev;
        }
        current_row = current_row->prev;
        current_col = current_row->tail;
        delete temp_row;
    }
    /* Shouldn't be possible, if yes, check the logic */
    else {
        assert(0);
    }
}

void DoublyList2D::insert_col(int x) {
    charNode *temp_col = new charNode(x);

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
}

void DoublyList2D::delete_col() {
    charNode *temp_col = current_col;

    /* At beginning of row, not pointing to any node */
    if (!current_col) {
        delete_row();
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
}

int DoublyList2D::get_row_size() {
    return current_row->length;
}

lineNode* DoublyList2D::get_first_row() {
    return document;
}
