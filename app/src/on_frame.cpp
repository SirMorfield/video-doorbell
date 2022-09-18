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
	char c[] = {
		get_line_pressed("QWERTYUIOP"),
		get_line_pressed(" ASDFGHJKL"),
		get_line_pressed("  ZXCVBNM")};

	for (size_t i = 0; i < sizeof(c) / sizeof(c[0]); i++)
		if (c[i])
			return c[i];

	return 0;
}

void print_occupant(const Occupant& occupant, const std::string& query) {
	const std::vector<size_t> score = match_score(occupant.name, query);

	//
	const ImVec2 spacing = ImGui::GetStyle().ItemSpacing;
	ImGui::GetStyle().ItemSpacing.x = 0.0f;
	for (size_t i = 0; i < occupant.name.size(); i++) {
		std::string s;
		s += occupant.name[i];

		if (std::find(score.begin(), score.end(), i) != score.end())
			ImText.text(ImGui_text::Font::Bold, s);
		else
			ImText.text(ImGui_text::Font::Regular, s);
#ifndef PRODUCTION
		if (i + 1 == occupant.name.size()) {
			ImGui::SameLine();
			ImText.text(ImGui_text::Font::Regular, " " + std::to_string(score.size()));
		}
#endif
		if (i + 1 != occupant.name.size())
			ImGui::SameLine();
	}
	ImText.set_font(ImGui_text::Font::Material_design);
	sameLineRightAlign(scale(30));
	std::string name = std::string(ICON_MD_PHONE) + std::string("###") + occupant.number + occupant.name; // making the label unique
	if (ImGui::Button(name.c_str(), scale(ImVec2(25, 15))))
		sip::ring(occupant.number);
	ImGui::GetStyle().ItemSpacing = spacing;
}

// returns true if there was an update
bool update_scroll_pos(size_t& pos) {
	const size_t start_value = pos;

	if (ImGui::Button(ICON_MD_EXPAND_LESS) && pos > 0)
		pos--;
	ImGui::SameLine();
	if (ImGui::Button(ICON_MD_EXPAND_MORE) && pos + consts().n_occupants < consts().occupants.size())
		pos++;
	ImGui::SameLine();

	return pos != start_value;
}

// returns true if there was an update
bool update_query(std::string& query) {
	const std::string start_value = query;

	if (ImGui::Button(ICON_MD_BACKSPACE, scale(ImVec2(50, 0))))
		query.resize(query.size() ? query.size() - 1 : 0);
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

	std::vector<Occupant> occupants;
	if (scroll_position)
		occupants = get_occupants_scroll(scroll_position, consts().n_occupants);
	else
		occupants = get_occupants_query(query, consts().n_occupants);
	for (const Occupant& occupant : occupants) {
		print_occupant(occupant, query);
	}
	for (size_t i = occupants.size(); i < consts().n_occupants; i++) // spacing
		ImGui::InvisibleButton("##a", scale(ImVec2(25, 15)));

	update_scroll_pos(scroll_position);
	if (update_query(query))
		scroll_position = 0;
}
