#include "IconsMaterialDesign.h"
#include "imgui_helpers.hpp"
#include "main.hpp"
// #include "test.hpp"
#include <algorithm>

// Allows all the sizing to be relative, like in css

char get_line_pressed(const std::string& line) {
	char pressed = 0;
	for (size_t i = 0; i < line.size(); i++) {
		char name[2] = {line[i], 0};
		if (line[i] == ' ')
			ImGui::InvisibleButton(name, scale(ImVec2(10, 10)));
		else if (ImGui::Button(name, scale(ImVec2(15, 15))))
			pressed = line[i];
		if (i + 1 != line.size())
			ImGui::SameLine();
	}
	return pressed;
}

char get_key_pressed() {
	// ImText.set_font(ImGui_text::Font::Bold); // Whatever you prefer
	char c[] = {
		get_line_pressed("QWERTYUIOP"),
		get_line_pressed(" ASDFGHJKL"),
		get_line_pressed("  ZXCVBNM")};

	for (size_t i = 0; i < sizeof(c) / sizeof(c[0]); i++)
		if (c[i])
			return c[i];

	return 0;
}

// returns true if the call key was pressed
bool print_occupant(const Occupant& occupant, const std::string& query, bool selected) {
	const std::vector<size_t> score = match_score(occupant.name, query);
	const ImVec2			  pos = ImGui::GetCursorPos();
	const ImVec2			  spacing = ImGui::GetStyle().ItemSpacing;

	ImGui::GetStyle().ItemSpacing.x = 0.0f;
	for (size_t i = 0; i < occupant.name.size(); i++) {
		std::string s;
		s += occupant.name[i];

		if (selected || std::find(score.begin(), score.end(), i) != score.end())
			ImText.text(ImGui_text::Font::Bold, s);
		else
			ImText.text(ImGui_text::Font::Regular, s);
		if (i + 1 != occupant.name.size())
			ImGui::SameLine();
	}
	ImGui::GetStyle().ItemSpacing = spacing;

	// overlay text with button
	const ImVec2 size = ImVec2(consts().window_width, ImGui::GetCursorPos().y - pos.y);
	ImGui::SetCursorPos(pos);
	if (ImGui::InvisibleButton(std::string("##" + occupant.name).c_str(), size)) {
		sip::end_calls_with_cameras();
		sip::ring(occupant.number);
		return true;
	}

	return false;
}
#define CONTROL_BUTTON scale(ImVec2(30, 0))

// returns true if there was an update
bool update_scroll_pos(size_t& pos) {
	const size_t start_value = pos;

	ImText.set_font(ImGui_text::Font::Material_design);
	if (ImGui::Button(ICON_MD_EXPAND_LESS, CONTROL_BUTTON) && pos > 0)
		pos--;
	ImGui::SameLine();
	if (ImGui::Button(ICON_MD_EXPAND_MORE, CONTROL_BUTTON) && pos + consts().n_occupants < consts().occupants.size())
		pos++;
	ImGui::SameLine();

	return pos != start_value;
}

bool end_call_button() {
	return ImGui::Button(ICON_MD_NOTIFICATIONS_OFF, CONTROL_BUTTON);
}

// returns true if there was an update
bool update_query(std::string& query) {
	const std::string start_value = query;

	if (ImGui::Button(ICON_MD_BACKSPACE, CONTROL_BUTTON))
		query.resize(query.size() ? query.size() - 1 : 0);

	ImGui::SameLine();
	if (end_call_button())
		sip::end_calls_with_cameras();

	ImText.text(ImGui_text::Font::Bold, query);
	ImText.set_font(ImGui_text::Font::Regular);

	char key_pressed = get_key_pressed();
	if (key_pressed) {
		query += key_pressed;
	}
	return start_value != query;
}

void on_frame() {
	static std::string	  query = "";
	static size_t		  scroll_position = 0;
	static std::string	  selected_occupant;
	static Timeout		  timeout(consts().call_timeout_seconds);
	std::vector<Occupant> occupants;

	// Used to reset the frontend after n seconds of inactivity
	if ((query != "" || scroll_position) && timeout.expired()) {
		query = "";
		scroll_position = 0;
		selected_occupant = "";
		std::cout << "Timeout reached, resetting to default" << std::endl;
	}

	if (scroll_position)
		occupants = get_occupants_scroll(scroll_position, consts().n_occupants);
	else
		occupants = get_occupants_query(query, consts().n_occupants);
	for (const Occupant& occupant : occupants) {
		if (print_occupant(occupant, query, occupant.name == selected_occupant)) {
			timeout.update();
			selected_occupant = occupant.name;
		}
	}

	if (update_scroll_pos(scroll_position))
		timeout.update();
	if (update_query(query)) {
		timeout.update();
		scroll_position = 0;
	}
}
