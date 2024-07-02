/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __VECTOR_DS__
#define __VECTOR_DS__

#include <vector>
#include "logger.h"

template<typename stl_type>
static inline void insert(stl_type &stl, int position, int value) {
    stl.insert(stl.begin() + position, value);
}

template<typename stl_type>
static inline void remove(stl_type &stl, int position) {
    stl.erase(stl.begin() + position);
}

class VectorDS {
public:
    void insert_row(int row, int col);
    void delete_row(int row);
    void insert_col(int row, int col, int value);
    void delete_col(int row, int col);

    int get_position(int row, int col);
    int get_row_size(int row);
    int get_total_rows();

    std::string get_row(int row);
    std::string get_document();

    /* This APIs are for debugging purpose and not utility */
    void print_meta();
    void printDS();

private:
    std::vector<int> m_container;
    std::vector<int> m_meta_data;
};

#endif /* __VECTOR_DS__  */
