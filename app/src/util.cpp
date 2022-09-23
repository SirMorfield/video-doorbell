#include "main.hpp"
#include <array>
#include <fstream>
#include <memory>
#include <streambuf>
#include <string>

// for get_binary_location
#ifdef __APPLE__
#include <mach-o/dyld.h>
#else
#include <linux/limits.h>
#include <unistd.h>
#endif

std::optional<std::string> exec(const std::string& cmd) {
	std::cout << "Exectuting     : " << cmd << std::endl;
	std::array<char, 128>					 buffer;
	std::string								 result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
	if (!pipe) {
		return {};
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	std::cout << "Done exectuting: " << cmd << ", result: " << result << std::endl;
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

// returns directory in witch this executable finds itself
std::optional<std::string> get_binary_location() {
	char buf[PATH_MAX * 2];

#ifdef __APPLE__
	uint32_t size = sizeof(buf);
	if (_NSGetExecutablePath(buf, &size) != 0)
		return {};
#else
	size_t	size = sizeof(buf);
	ssize_t ret = readlink("/proc/self/exe", buf, size);
	if (ret < 0)
		return {};
	buf[ret] = '\0';
#endif

	std::string path = buf;
	return path.substr(0, path.find_last_of('/'));
}

std::string absolute_path(const std::string& relative_path) {
	static std::optional<std::string> path = get_binary_location();
	std::string						  absolute = path.value_or("");

	if (relative_path.size() && relative_path[0] != '/')
		absolute += '/';
	absolute += relative_path;
	return absolute;
}
