#include "imgui_helpers.hpp"

void sameLineRightAlign(float width) {
	ImVec2 last_tooltip_size = ImGui::GetWindowSize();
	ImGui::SameLine(std::max(0.0f, last_tooltip_size.x - width));
}
