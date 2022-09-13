#include "main.hpp"
#include <array>
#include <fstream>
#include <memory>
#include <streambuf>
#include <string>

std::optional<std::string> exec(const std::string& cmd) {
	std::array<char, 128>					 buffer;
	std::string								 result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
	if (!pipe) {
		return {};
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

std::string read_file(const std::string& path) {
	std::ifstream t(path);
	ASSERT(t.is_open(), ==, true);

	return std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
}

// splits s in any of the delimiters in delim
std::vector<std::string> ft_split(const std::string& s, const std::string& charset) {
	std::vector<std::string> out;
	size_t					 start;
	size_t					 end = 0;

	while ((start = s.find_first_not_of(charset, end)) != std::string::npos) {
		end = std::string::npos;
		for (size_t i = 0; i < charset.size(); i++)
			end = std::min(end, s.find(charset[i], start));

		out.push_back(s.substr(start, end - start));
	}
	return out;
}
