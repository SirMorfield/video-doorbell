#pragma once

#include "imgui.h"
#include "imgui_helpers.hpp"
#include "util.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

void					   on_frame();
std::optional<std::string> exec(const std::string& cmd);
std::string				   read_file(const std::string& path);
std::optional<std::string> get_binary_location();
std::string				   absolute_path(const std::string& relative_path);

typedef struct {
	std::string number;
	std::string name;
} Occupant;
std::vector<Occupant> read_occupants();
std::vector<Occupant> get_occupants_query(const std::string& query, size_t max_results);
std::vector<Occupant> get_occupants_scroll(size_t scroll_pos, size_t max_results);

namespace commands {

std::string ring(const std::string& client_phonenumber);

};

namespace sip {

std::vector<std::string> get_channels();
void					 ring(const std::string& phonenumber);

} // namespace sip

unsigned int match_score(const std::string& name, const std::string& query);

typedef struct {
	float				  window_width;
	float				  window_height;
	size_t				  n_occupants; // Number of occupants to list on the frontend
	std::vector<Occupant> occupants;
	std::string			  font_door_number; // The phone number of the front door SIP camera
} Constants;

const Constants& consts();
