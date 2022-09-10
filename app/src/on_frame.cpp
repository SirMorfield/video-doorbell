#include "main.hpp"

std::string text;
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
	char key_pressed = 0;

	if (!key_pressed)
		key_pressed = get_line_pressed("qwertyuiop");
	if (!key_pressed)
		key_pressed = get_line_pressed(" asdfghjkl");
	if (!key_pressed)
		key_pressed = get_line_pressed("  zxcvbnm");
	return key_pressed;
}

void on_frame() {
	ImGui::Begin("Virtual Keyboard");
	ImGui::Text("%s", text.c_str());

	char key_pressed = get_key_pressed();
	if (key_pressed) {
		text += key_pressed;
	}
	if (ImGui::Button("call")) {
		const std::string cmd = commands::ring(text);
		std::cout << "running: " << cmd << std::endl;
		ASSERT(exec(cmd).has_value(), ==, true);
	}

	ImGui::End();
}
