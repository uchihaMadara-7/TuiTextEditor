/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* custom imports */
#include "include/vector_ds.h"
#include "include/logger.h"

void VectorDS::insert_row(int row, int col) {
    int remaining = m_meta_data[row] - col;
    DEBUG_TRACE("Insert line at (%d, %d): %d", row, col, remaining);
    m_meta_data[row] = col;
    insert(m_meta_data, row+1, remaining);
}

void VectorDS::delete_row(int row) {
    int remaining = m_meta_data[row];
    DEBUG_TRACE("Delete line at (%d, 0): %d", row, remaining);
    m_meta_data[row-1] += remaining;
    remove(m_meta_data, row);
}

void VectorDS::insert_col(int row, int col, int value) {
    int position = get_position(row, col);
    DEBUG_TRACE("Insert at %d: (ascii: %d) %c (%d, %d)", position, value, value, row, col);
    insert(m_container, position, value);

    /* Maintaing characters per row - meta data */
    if (m_meta_data.size() > static_cast<size_t>(row)) {
        m_meta_data[row] += 1;
    } else {
        m_meta_data.push_back(1);
    }
}

void VectorDS::delete_col(int row, int col) {
    int position = get_position(row, col);
    DEBUG_TRACE("Delete at %d: %c (%d, %d)", position, m_container[position], row, col);
    remove(m_container, position);

    if (m_meta_data[row]) {
        m_meta_data[row] -= 1;
    } else {
        remove(m_meta_data, row);
    }
}

int VectorDS::get_position(int row, int col) {
    int position = 0;
    for (int i=0; i < row; ++i) {
        position += m_meta_data[i];
    }
    position += col;
    return position;
}

int VectorDS::get_row_size(int row) {
    return m_meta_data[row];
}

int VectorDS::get_total_rows() {
    return m_meta_data.size();
}

std::string VectorDS::get_row(int row) {
    std::string row_str = std::string();
    int position = get_position(row, 0);
    for (int i=position; i < position+m_meta_data[row]; ++i) {
        row_str.append(1, m_container[i]);
    }
    return row_str;
}

std::string VectorDS::get_document() {
    std::string document = std::string();
    int size = m_meta_data.size();
    for (int row=0; row < size; ++row) {
        document.append(get_row(row));
        if (row < size-1)
            document.append(1, NEW_LINE);
    }
    return document;
}

void VectorDS::print_meta() {
    int size = m_meta_data.size();
    for (int i=0; i < size; ++i) {
        DEBUG_TRACE("(%d): %d", i, m_meta_data[i]);
    }
}

void VectorDS::printDS() {
    int size = m_meta_data.size();
    for (int row=0; row < size; ++row) {
        std::string row_str = std::string();
        int position = get_position(row, 0);
        for (int i=position; i < position+m_meta_data[row]; ++i) {
            row_str.append(1, m_container[i]);
        }
        DEBUG_TRACE("%s (%d)", row_str, m_meta_data[row]);
    }
}
