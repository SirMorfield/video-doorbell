#pragma once

#include "imgui.h"
#include "util.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

void					   on_frame();
std::optional<std::string> exec(const std::string& cmd);

namespace commands {
std::string ring(const std::string& client_phonenumber);

};
