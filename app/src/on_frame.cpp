#include "IconsMaterialDesign.h"
#include "imgui_helpers.hpp"
#include "main.hpp"
#include <algorithm>

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

// returns 1 for up, 0 for no change, -1 for down
int get_scroll_position() {
	int change;

	ImText.set_font(ImGui_text::Font::Material_design);
	if (ImGui::Button(ICON_MD_EXPAND_LESS, CONTROL_BUTTON))
		change = -1;
	ImGui::SameLine();
	if (ImGui::Button(ICON_MD_EXPAND_MORE, CONTROL_BUTTON))
		change = 1;
	ImGui::SameLine();

	return change;
}

bool end_call_button() {
	const bool update = ImGui::Button(ICON_MD_NOTIFICATIONS_OFF, CONTROL_BUTTON);
	ImGui::SameLine();
	if (update)
		sip::end_calls_with_cameras();
	return update;
}

// returns unchanged or updated query
std::string get_query(std::string query) {
	if (ImGui::Button(ICON_MD_BACKSPACE, CONTROL_BUTTON))
		query.resize(query.size() ? query.size() - 1 : 0);

	ImText.text(ImGui_text::Font::Bold, query);
	ImText.set_font(ImGui_text::Font::Regular);

	char key_pressed = get_key_pressed();
	if (key_pressed) {
		query += key_pressed;
	}
	return query;
}

#define NEVER 0
void on_frame() {
	static State						  state;
	static std::chrono::milliseconds::rep last_input = NEVER;

	state.start_frame();

	for (const Occupant& occupant : state.occupants()) {
		if (print_occupant(occupant, state.query(), state.is_selected(occupant))) {
			state.set_selected_occupant(occupant);
		}
	}

#ifdef __APPLE__
	ImGui::SetCursorPosY(scale(220.2f));
#else
	ImGui::SetCursorPosY(scale(192.0f));
#endif

	state.update_scroll_position(get_scroll_position());
	if (end_call_button())
		state.unset_selected_occupant();
	state.set_query(get_query(state.query()));

	if (state.interaction()) {
		last_input = date_now();
	}

	if (last_input != NEVER && date_now() - last_input > consts().call_timeout) {
		state = State();
		last_input = NEVER;
	}
}
