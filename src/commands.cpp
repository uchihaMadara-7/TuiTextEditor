/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* custom imports */
#include "include/commands.h"
#include "include/editor.h"

std::string read_command_args(Editor &editor) {
    std::string command = std::string();
    int c;
    while ((c = editor.read())) {
        switch (c) {
            case KEY_ESC:
                return std::string();
                break;
            case KEY_ENTER:
                return command;
                break;
            default:
                command.append(1, c);
                editor.command_write(c);
                break;
        }
    }
    return std::string();
}

void csave(Editor &editor) {
    Error status = editor.save_file();
    if (status == Error::NO_FILE) {
        std::string filename = read_command_args(editor);
        if (filename.empty()) return;
        if (!editor.set_new_file(filename)) return;
        status = editor.save_file();
        return;
    }

    if (status == Error::NO_ERROR) return;
    editor._print_command_banner(WRITE_FAILED);
}
