#include "editor.h"
#include "logger.h"
#include "utility.h"

#define LOG_FILE "editor.log"

bool read_command(Editor &editor) {
    editor.enter_command_mode();
    std::string command = std::string();
    bool command_mode_quit = false;
    int c;
    while (!command_mode_quit && (c = editor.read())) {
        switch(c) {
            case KEY_ESC:
                editor.set_mode(EditorMode::NORMAL_MODE);
                editor.command_mode_banner();
                command_mode_quit = true;
                break;
            case KEY_ENTER:
                DEBUG_TRACE("Command: %s", command);
                switch((int) get_command_code(command)) {
                    case (int) CommandType::INSERT:
                        editor.set_mode(EditorMode::INSERT_MODE);
                        editor.command_mode_banner();
                        break;
                    case (int) CommandType::WRITE:
                        editor.set_mode(EditorMode::NORMAL_MODE);
                        editor.save_file();
                        break;
                    case (int) CommandType::WRITE_CLOSE:
                        editor.save_file();
                        /* Intentionally break skipped, to fall back to quit */
                    case (int) CommandType::QUIT:
                        if (editor.get_remove_required()) editor.remove_file();
                        return true;
                    default:
                        editor.set_mode(EditorMode::NORMAL_MODE);
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
    Logger &logger = Logger::getInstance(LOG_FILE);
    if (!logger.is_initialized()) {
        /* Early return due to init failure */
        printf("Log initialisation failed!");
        return 1;
    }
    logger.set_level(LOG_DEBUG);

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
                if (editor.get_mode() == EditorMode::INSERT_MODE) {
                    editor.set_mode(EditorMode::NORMAL_MODE);
                    editor.command_mode_banner();
                }
                break;
            case KEY_INSERT_1:
            case KEY_INSERT_2:
                if (editor.get_mode() ==EditorMode:: NORMAL_MODE) {
                    editor.set_mode(EditorMode::INSERT_MODE);
                    editor.command_mode_banner();
                }
                else editor.write(c);
                break;
            case KEY_COMMAND:
                if (editor.get_mode() ==EditorMode:: NORMAL_MODE) {
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
