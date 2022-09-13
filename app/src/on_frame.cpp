#include "main.hpp"

std::vector<Apartment> get_apartments() {
	std::vector<Apartment>	 apartments;
	std::string				 apt_file = read_file("./apartments.csv");
	std::vector<std::string> lines = ft_split(apt_file, "\n");
	for (const std::string& line : lines) {
		std::vector<std::string> fields = ft_split(line, ",");
		if (fields.size() < 2)
			continue;
		Apartment apt;
		apt.number = fields[0];
		fields.erase(fields.begin());
		apt.occupants = fields;
		apartments.push_back(apt);
	}
	return apartments;
}

const std::vector<Apartment> g_apartments = get_apartments();
std::string					 text;
//

char get_line_pressed(const std::string& line) {
	char pressed = 0;
	for (size_t i = 0; i < line.size(); i++) {
		char name[2] = {line[i], 0};
		if (line[i] == ' ')
			ImGui::InvisibleButton(name, ImVec2(32, 32));
		else if (ImGui::Button(name, ImVec2(46, 46)))
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
	ImGui::Text("%s", text.c_str());
	ImGui::SameLine();
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
