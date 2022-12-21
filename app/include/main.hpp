#pragma once

#include "consts.hpp"
#include "imgui.h"
#include "imgui_helpers.hpp"
#include "util.hpp"
#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

void					   on_frame();
std::optional<std::string> exec(const std::string& cmd);
std::string				   read_file(const std::string& path);
std::optional<std::string> get_binary_location();
std::string				   absolute_path(const std::string& relative_path);
std::vector<Occupant>	   get_occupants_query(const std::string& query, size_t max_results);
std::vector<Occupant>	   get_occupants_scroll(size_t scroll_pos, size_t max_results);

#ifdef __APPLE__
#define SCALE 1.8f
#else
#define SCALE 4.76f // This is for the touchscreen computer who's scaling is different
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
void					 end_calls_with_cameras();
void					 ring(const std::string& phonenumber);

} // namespace sip

std::vector<size_t> match_score(std::string name, std::string query);

// simulating JavaScript's Date.now()
std::chrono::milliseconds::rep date_now();
std::string					   get_ISO_timestamp();
bool						   append_to_logfile(const std::string& content);

// Is this a bad idea? We shall see
class State {
  public:
	State() {
		this->_occupants = get_occupants_scroll(0, consts().n_occupants);
	}

	void start_frame() {
		this->_interaction = false;
	}

	bool interaction() const { return this->_interaction; }

	// query
	void set_query(const std::string& query) {
		if (this->_query != query) {
			this->_interaction = true;
			this->_query = query;
			this->_scroll_position = 0;
			this->_occupants = get_occupants_query(query, consts().n_occupants);
		}
	}
	const std::string& query() const { return this->_query; }

	// scroll
	void update_scroll_position(int change) {
		if (change == 0)
			return;

		// don't scroll when there is a query and the scroll position will not change the displayed names
		if (this->_query != "" && this->_occupants.size() < consts().n_occupants)
			return;
		this->_interaction = true;

		change += this->_scroll_position;
		if (change >= 0 && change + consts().n_occupants <= consts().occupants.size()) {
			this->_scroll_position = (size_t)change;
			this->_occupants = get_occupants_scroll(this->_scroll_position, consts().n_occupants);
		}
	}

	// selected_occupant
	void set_selected_occupant(const Occupant& occ) {
		if (this->_selected_occupant != occ.name) {
			this->_interaction = true;
			this->_selected_occupant = occ.name;
		}
	}
	void unset_selected_occupant() {
		this->_selected_occupant = std::nullopt;
		this->_interaction = true;
	}
	bool is_selected(const Occupant& occ) const {
		return this->_selected_occupant == occ.name;
	}

	const std::vector<Occupant>& occupants() const { return this->_occupants; }

  private:
	bool					   _interaction = false;
	std::string				   _query = "";
	size_t					   _scroll_position = 0;
	std::optional<std::string> _selected_occupant;
	std::vector<Occupant>	   _occupants;
};
