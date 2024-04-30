#include <iostream>
#include "editor.h"
#include "logger.h"
#include <map>

#define LOG_FILE "editor.log"

std::map<std::string, int> command_codes = {
    std::make_pair("i", 1),
    std::make_pair("insert", 1),
    std::make_pair("q", 2),
    std::make_pair("quit", 2)
};

int get_command_code(std::string command) {
    if (command_codes.count(command)) {
        return command_codes[command];
    }
    return 0;
}

bool read_command(Editor &editor) {
    editor.enter_command_mode();
    std::string command = std::string();
    bool command_mode_quit = false;
    int c;
    while (!command_mode_quit && (c = editor.read())) {
        switch(c) {
            case KEY_ESC:
                editor.clear_command_mode();
                editor.set_mode(NORMAL_MODE);
                command_mode_quit = true;
                break;
            case KEY_ENTER:
                DEBUG_TRACE("Command: %s", command);
                switch(get_command_code(command)) {
                    case 1:
                        editor.set_mode(INSERT_MODE);
                        editor.command_mode_banner();
                        break;
                    case 2: return true;
                    default:
                        editor.set_mode(NORMAL_MODE);
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

int main() {

    /* Setting up logger */
    Logger &logger = Logger::getInstance(LOG_FILE);
    if (!logger.is_initialized()) {
        /* Early return due to init failure */
        return 1;
    }
    logger.set_level(LOG_DEBUG);

    Editor& editor = Editor::getInstance();
    bool app_quit = false;
    int c;

    while (!app_quit && (c = editor.read())) {
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
                if (editor.get_mode() == INSERT_MODE) {
                    DEBUG_TRACE("Normal mode activated!");
                    editor.set_mode(NORMAL_MODE);
                    editor.command_mode_banner();
                }
                break;
            case KEY_INSERT_1:
            case KEY_INSERT_2:
                if (editor.get_mode() == NORMAL_MODE) {
                    DEBUG_TRACE("Insert mode activated!");
                    editor.set_mode(INSERT_MODE);
                    editor.command_mode_banner();
                }
                else editor.write(c);
                break;
            case KEY_COMMAND:
                if (editor.get_mode() == NORMAL_MODE) {
                    DEBUG_TRACE("Command mode activated!");
                    app_quit = read_command(editor);
                }
                else editor.write(c);
                break;
            default:
                editor.write(c);
        }
    }
    LOG_DS();
    return 0;
}
