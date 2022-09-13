#pragma once

#include "constants.hpp"
#include "imgui.h"
#include "util.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

void					   on_frame();
std::optional<std::string> exec(const std::string& cmd);
std::string				   read_file(const std::string& path);

typedef struct {
	std::string				 number;
	std::vector<std::string> occupants;
} Apartment;
std::vector<Apartment> get_apartments();

namespace commands {

std::string ring(const std::string& client_phonenumber);

};
