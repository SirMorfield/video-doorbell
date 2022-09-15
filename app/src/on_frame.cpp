#include "IconsMaterialDesign.h"
#include "imgui_helpers.hpp"
#include "main.hpp"
#include <algorithm>

// Allows all the sizing to be relative, like in css
#define SCALE 3.0f
constexpr float	 scale(float n) { return n * SCALE; }
constexpr ImVec2 scale(const ImVec2& v) {
	return ImVec2(v.x * SCALE, v.y * SCALE);
}

std::vector<Occupant> get_occupants() {
	std::vector<Occupant>	 occupants;
	std::string				 apt_file = read_file("./occupants.csv");
	std::vector<std::string> lines = ft_split(apt_file, "\n");
	for (const std::string& line : lines) {
		std::vector<std::string> fields = ft_split(line, ",");
		if (fields.size() < 2)
			continue;
		occupants.push_back((Occupant){.number = fields.at(0), .name = fields.at(1)});
	}
	ASSERT(occupants.size(), >=, 1);
	return occupants;
}

// Returns equability score of name and query
// Higher score means better match with name
// Score 0~length of query
unsigned int match_score(const std::string& name, const std::string& query) {
	std::vector<std::string> words = ft_split(name, " ");

	unsigned int			 score = 0;
	for (const std::string& word : words) {

		size_t i = 0;
		if (query.size()) {
			size_t start = word.find_first_of(query[0]);
			if (start != std::string::npos)
				i = start;
		}
		for (; i < word.size() &&
			   score < query.size() &&
			   std::tolower(word[i]) == std::tolower(query[score]);
			 i++) {
			score++;
		}
	}
	return score;
}

std::vector<Occupant> get_occupants(const std::string& query, size_t max_results) {
	static std::vector<Occupant> occupants = get_occupants();
	std::sort(occupants.begin(), occupants.end(), [](const Occupant& a, const Occupant& b) { return a.name < b.name; });
	std::vector<unsigned int> scores;

	for (const Occupant& occupant : occupants) {
		scores.push_back(match_score(occupant.name, query));
	}

	std::vector<Occupant> results;
	unsigned int		  highest_score = *std::max_element(scores.begin(), scores.end());
	max_results = std::min(max_results, occupants.size());
	while (highest_score >= 0) {
		for (size_t i = 0; i < scores.size(); i++) {
			if (scores[i] == highest_score) {
				results.push_back(occupants[i]);
				if (results.size() == max_results)
					goto end;
				scores[i] = -1;
			}
		}
		highest_score--;
	}
end:
	return results;
}

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
		get_line_pressed("qwertyuiop"),
		get_line_pressed(" asdfghjkl"),
		get_line_pressed("  zxcvbnm")};

	for (size_t i = 0; i < sizeof(c) / sizeof(c[0]); i++)
		if (c[i])
			return c[i];

	return 0;
}

void print_occupant(const std::string& name, const std::string& query, ImGui_text& imText) {
	std::string backlog;
	size_t		query_pos = 0;

	for (size_t i = 0; i < name.size(); i++) {
		std::string s;
		s += name[i];
		if (query_pos < query.size() &&
			std::tolower(name[i]) == std::tolower(query[query_pos])) {
			imText.text(ImGui_text::Font::Bold, s);

			query_pos++;
		}
		else
			imText.text(ImGui_text::Font::Regular, s);
		if (i + 1 != name.size()) {
			ImGui::SameLine();
		}
	};
	imText.set_font(ImGui_text::Font::Material_design);
	sameLineRightAlign(scale(30));
	ImGui::Button(ICON_MD_PHONE, scale(ImVec2(25, 15)));
}

void on_frame(ImGui_text& imText) {

	static std::string			query = "";
	const std::vector<Occupant> occupants = get_occupants(query, 5);

	for (const Occupant& occupant : occupants) {
		print_occupant(occupant.name, query, imText);
	}
	imText.text(ImGui_text::Font::Bold, query);
	imText.set_font(ImGui_text::Font::Regular);
	if (ImGui::Button("Clear", ImVec2(100, 0)))
		query.resize(query.size() ? query.size() - 1 : 0);
	char key_pressed = get_key_pressed();
	if (key_pressed) {
		query += key_pressed;
	}
	if (ImGui::Button("call")) {
		const std::string cmd = commands::ring(query);
		std::cout << "running: " << cmd << std::endl;
		ASSERT(exec(cmd).has_value(), ==, true);
	}
}
