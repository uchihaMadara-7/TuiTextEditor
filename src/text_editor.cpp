#include <iostream>
#include "editor.h"
#include "logger.h"

#define LOG_FILE "editor.log"

int main() {

    /* Setting up logger */
    Logger &logger = Logger::getInstance(LOG_FILE);
    if (!logger.is_initialized()) {
        /* Early return due to init failure */
        return 1;
    }
    logger.set_level(LOG_DEBUG);

    Editor& editor = Editor::getInstance();
    std::string command;
    bool command_mode_quit, app_quit = false;
    int c;

    while (!app_quit && (c = getch())) {
        switch(c) {
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
                if (editor.get_mode() != COMMAND_MODE) {
                    DEBUG_TRACE("Command mode activated!");
                    editor.set_mode(COMMAND_MODE);
                    editor.command_banner();
                }
                break;
            case KEY_INSERT:
                if (editor.get_mode() == COMMAND_MODE) {
                    DEBUG_TRACE("Insert mode activated!");
                    editor.set_mode(INSERT_MODE);
                    editor.command_banner();
                }
                else editor.write(c);
                break;
            case KEY_COMMAND:
                DEBUG_TRACE("Command line entered!");
                editor.clear_command_mode();
                editor.command_mode();
                command = std::string();
                command_mode_quit = true;
                while (command_mode_quit && (c = getch())) {
                    switch(c) {
                        case KEY_ESC:
                            editor.clear_command_mode();
                            command_mode_quit = false;
                            break;
                        case KEY_ENTER:
                            DEBUG_TRACE("Command: %s", command);
                            editor.clear_command_mode();
                            if (command == "i" || command == "insert") {
                                editor.set_mode(INSERT_MODE);
                                editor.command_banner();
                            }
                            else if (command == "q" || command == "quit") app_quit = true;
                            command_mode_quit = false;
                            break;
                        default:
                            command.append(1, c);
                            editor.command_write(c);
                    }
                }
                break;
            default:
                editor.write(c);
        }
    }
    LOG_DS();
    return 0;
}
