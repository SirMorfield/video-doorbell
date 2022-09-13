#pragma once

#include "constants.hpp"
#include "imgui.h"
#include "imgui_helpers.hpp"
#include "util.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

void					   on_frame(ImGui_text& text);
std::optional<std::string> exec(const std::string& cmd);
std::string				   read_file(const std::string& path);

typedef struct {
	std::string number;
	std::string name;
} Occupant;
std::vector<Occupant> get_occupants();

namespace commands {

std::string ring(const std::string& client_phonenumber);

};
