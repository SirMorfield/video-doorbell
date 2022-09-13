#include "main.hpp"
#include <algorithm>
// Allows all the sizing to be relative, like in css
#define SCALE 3.0f
constexpr float	 scale(float n) { return n * SCALE; }
constexpr ImVec2 scale(const ImVec2& v) {
	return ImVec2(v.x * SCALE, v.x * SCALE);
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
	return occupants;
}

int match_score(const std::string& name, const std::string& query) {
	std::vector<std::string> words = ft_split(name, " ");

	int						 score = 0;
	for (const std::string& word : words) {
		for (size_t i = 0;
			 i < query.size() &&
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
	std::vector<int> scores;

	for (const Occupant& occupant : occupants) {
		scores.push_back(match_score(occupant.name, query));
	}

	std::vector<Occupant> results;
	int					  highest_score = *std::max_element(scores.begin(), scores.end());
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

void on_frame() {
	static std::string text = "";
	ImGui::Text("%s", text.c_str());
	const std::vector<Occupant> occupants = get_occupants(text, 5);
	for (const Occupant& occupant : occupants) {
		ImGui::Text("%s", occupant.name.c_str());
	}

	if (ImGui::Button("Clear", ImVec2(100, 0)))
		text.resize(text.size() ? text.size() - 1 : 0);
	char key_pressed = get_key_pressed();
	if (key_pressed) {
		text += key_pressed;
	}
	if (ImGui::Button("call")) {
		const std::string cmd = commands::ring(text);
		std::cout << "running: " << cmd << std::endl;
		ASSERT(exec(cmd).has_value(), ==, true);
	}
}
