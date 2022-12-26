#include "main.hpp"
#include <array>
#include <fstream>
#include <memory>
#include <streambuf>
#include <string>
#include <sys/time.h>

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

	return std::string(std::istreambuf_iterator<char>(t), std::istreambuf_iterator<char>());
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

// returns current time in milliseconds since epoch
// same as JS Date.now()
std::chrono::milliseconds::rep date_now() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string get_ISO_timestamp() {
	timeval curTime;
	gettimeofday(&curTime, NULL);

	int	  milli = curTime.tv_usec / 1000;
	char  buf[sizeof "2011-10-08T07:07:09.000Z"];
	char* p = buf + strftime(buf, sizeof buf, "%FT%T", gmtime(&curTime.tv_sec));
	sprintf(p, ".%dZ", milli);

	return buf;
}

// return true on success
bool append_to_logfile(const std::string& content) {
	if (content.empty())
		return true;

	std::ofstream file("./out.log", std::ios::app);
	if (!file.is_open())
		return false;
	file << get_ISO_timestamp() << " | " << content;
	if (content.at(content.size() - 1) != '\n')
		file << std::endl;
	file.close();
	return true;
}
