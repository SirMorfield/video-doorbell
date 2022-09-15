#pragma once
#include "imgui.h"
#include <algorithm>
#include <iostream>
#include <string>

extern ImFont* g_fonts[];
ImFont*		   add_default_font(float pixel_size);
extern ImFont* g_font;

class ImGui_text {
  public:
	enum class Font {
		Regular = 0,
		Bold,
		Material_design,
		_Count, // do not use
	};

	ImGui_text(){};
	ImGui_text(ImFont* fonts[static_cast<size_t>(Font::_Count)]) {
		for (size_t i = 0; i < static_cast<size_t>(Font::_Count); ++i) {
			assert(fonts[i]);
			this->_fonts[i] = fonts[i];
		}
		this->_font = Font::_Count;
	}

	void set_font(ImGui_text::Font font) {
		assert(font <= Font::_Count);
		if (this->_font != Font::_Count)
			ImGui::PopFont();
		this->_font = font;
		ImGui::PushFont(this->_fonts[static_cast<size_t>(this->_font)]);
	}

	void text(ImGui_text::Font font, const std::string& text) {
		if (font != this->_font)
			this->set_font(font);

		ImGui::Text("%s", text.c_str());
	}

	void text(const std::string& text) {
		this->text(this->_font, text);
	}

	void clear_stack() {
		if (this->_font != Font::_Count) {
			ImGui::PopFont();
			this->_font = Font::_Count;
		}
	}

	// ImGui_text() = delete;
	// ImGui_text(const ImGui_text&) = delete;
	// ImGui_text& operator=(const ImGui_text&) = delete;

  private:
	Font	_font;
	ImFont* _fonts[(int)Font::_Count];
};

extern ImGui_text ImText;
void			  sameLineRightAlign(float width);
