#include "vector_ds.h"

void VectorDS::insert_row(int row, int col) {
    int remaining = m_meta_data[row] - col;
    DEBUG_TRACE("New line at (%d, %d): %d", row, col, remaining);
    m_meta_data[row] = col;
    insert(m_meta_data, row+1, remaining);
}

void VectorDS::delete_row() {

}

void VectorDS::insert_col(int row, int col, int value) {
    /* Maintaing characters per row - meta data */
    if (m_meta_data.size() > (size_t)row) m_meta_data[row] += 1;
    else m_meta_data.push_back(1);

    int position = get_position(row, col);
    DEBUG_TRACE("Insert at %d: %c (%d, %d)", position, value, row, col);
    insert(m_container, position, value);
}

void VectorDS::delete_col() {

}

int VectorDS::get_position(int row, int col) {
    int position = 0;
    for (int i=0; i<row; ++i) {
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

std::string VectorDS::get_document() {
    std::string document = std::string();
    int size = m_meta_data.size();
    for (int row=0; row<size; ++row) {
        int position = get_position(row, 0);
        for (int i=position; i<position+m_meta_data[row]; ++i) {
            document.append(1, m_container[i]);
        }
        document.append(1, NEW_LINE);
    }
    return document;
}

void VectorDS::print_meta() {
    int size = m_meta_data.size();
    for (int i=0; i<size; ++i) {
        DEBUG_TRACE("(%d): %d", i, m_meta_data[i]);
    }
}

void VectorDS::printDS() {
    int size = m_meta_data.size();
    for (int row=0; row<size; ++row) {
        std::string row_str = std::string();
        int position = get_position(row, 0);
        for (int i=position; i<position+m_meta_data[row]; ++i) {
            row_str.append(1, m_container[i]);
        }
        DEBUG_TRACE("%s (%d)", row_str, m_meta_data[row]);
    }
}