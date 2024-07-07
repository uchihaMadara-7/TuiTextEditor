/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* custom imports */
#include "include/commands.h"
#include "include/editor.h"
#include "include/logger.h"
#include "include/utils.h"

#define LOG_FILE "editor.log"

bool read_command(Editor &editor) {
    editor.enter_command_mode();
    std::string command = std::string();
    bool command_mode_quit = false;
    int c;
    while (!command_mode_quit && (c = editor.read())) {
        switch (c) {
            case KEY_ESC:
                editor.set_mode(EditorMode::NORMAL_MODE);
                command_mode_quit = true;
                break;
            case KEY_ENTER:
                DEBUG_TRACE("Command: %s", command);
                switch (static_cast<int>(get_command_code(command))) {
                    case static_cast<int>(CommandType::INSERT):
                        editor.set_mode(EditorMode::INSERT_MODE);
                        break;
                    case static_cast<int>(CommandType::WRITE):
                        csave(editor);
                        editor.set_mode(EditorMode::NORMAL_MODE, false);
                        break;
                    case static_cast<int>(CommandType::WRITE_CLOSE):
                        csave(editor);
                        /* Intentionally break skipped, to fall back to quit */
                    case static_cast<int>(CommandType::QUIT):
                        if (editor.get_remove_required()) editor.remove_file();
                        return true;
                    default:
                        editor.set_mode(EditorMode::NORMAL_MODE, false);
                        editor.invalid_command();
                }
                command_mode_quit = true;
                break;
            default:
                command.append(1, c);
                editor.command_write(c);
        }
    }
    return false;
}

void usage(int argc, char* prog) {
    printf("Too many arguments (expected: 1, provided: %d)\n", argc);
    printf("Usage: %s <file_path>\n", basename(std::string(prog)).c_str());
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        usage(argc, argv[0]);
        return 1;
    }

    /* Setting up logger */
    #ifdef DEBUG_FLAG
    Logger &logger = Logger::getInstance(LOG_FILE);
    if (!logger.is_initialized()) {
        /* Early return due to init failure */
        printf("Log initialisation failed!");
        return 1;
    }
    logger.set_level(LOG_DEBUG);
    #endif

    Editor& editor = Editor::getInstance();
    if (!editor.is_initialized()) {
        printf("Editor Initialisation failed!");
        return 1;
    }

    /* If second argument is present, open file */
    if (argc == 2 && !editor.set_file(std::string(argv[1]))) {
        printf("Failed to open file!");
        return 1;
    }

    /* setup signal handlers */
    setup_signal_handlers();

    bool app_quit = false;
    int c;

    editor.set_mode(EditorMode::NORMAL_MODE);
    while (!app_quit && (c = editor.read())) {
        switch (c) {
            case KEY_LEFT:
                editor.left_arrow();
                break;
            case KEY_RIGHT:
                editor.right_arrow();
                break;
            case KEY_UP:
                editor.up_arrow();
                break;
            case KEY_DOWN:
                editor.down_arrow();
                break;
            case KEY_BACKSPACE:
                editor.backspace();
                break;
            case KEY_ENTER:
                editor.enter_key();
                break;
            case KEY_ESC:
                if (editor.get_mode() == EditorMode::INSERT_MODE) {
                    editor.set_mode(EditorMode::NORMAL_MODE);
                }
                break;
            case KEY_INSERT_1:
            case KEY_INSERT_2:
                if (editor.get_mode() == EditorMode::NORMAL_MODE) {
                    editor.set_mode(EditorMode::INSERT_MODE);
                } else {
                    editor.write(c);
                }
                break;
            case KEY_COMMAND:
                if (editor.get_mode() == EditorMode::NORMAL_MODE) {
                    app_quit = read_command(editor);
                } else {
                    editor.write(c);
                }
                break;
            default:
                editor.write(c);
        }
    }
    LOG_DS();
    return 0;
}
