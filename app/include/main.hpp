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

#ifdef __APPLE__
#define SCALE 1.8f
#else
#define SCALE 4.76f
#endif
constexpr float	 scale(float n) { return n * SCALE; }
constexpr ImVec2 scale(const ImVec2& v) {
	return ImVec2(v.x * SCALE, v.y * SCALE);
}

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
	size_t				  n_occupants;
	std::vector<Occupant> occupants;
	std::string			  font_door_number;
	size_t				  max_occupant_name_length;
} Constants;

const Constants& consts();
