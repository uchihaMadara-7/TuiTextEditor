/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __COMMANDS_H__
#define __COMMANDS_H__

/* standard imports */
#include <string>

/* custom imports */
#include "editor.h"

std::string read_command_args(Editor &editor);
void csave(Editor &editor);

#endif
