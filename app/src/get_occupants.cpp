#include "main.hpp"
#include <algorithm>
#include <exception>

std::ostream& operator<<(std::ostream& os, const Occupant& occ) {
	os << occ.name << " " << occ.number;
	return os;
}

[[noreturn]] void occupants_error(const std::string& msg) {
	log("\n\n========================================");
	log(msg);
	log("========================================\n\n ");
	exit(1);
}

// expecting line format of <name>,<number>[,<number>...]\n
std::vector<Occupant> read_occupants(const std::array<std::string, Camera_type::N>& cameras, size_t max_occupant_name_length) {
	std::vector<Occupant>	 occupants;
	static const std::string path = get_binary_location().value() + "/occupants.conf";
	std::string				 apt_file = read_file(path);

	apt_file.erase(std::remove(apt_file.begin(), apt_file.end(), '\r'), apt_file.end()); // remove carriage returns that come with windows line endings
	apt_file.erase(std::remove(apt_file.begin(), apt_file.end(), '\t'), apt_file.end()); // remove tabs

	std::vector<std::string> lines = ft_split(apt_file, "\n");

	for (const std::string& line : lines) {
		size_t comma = line.find(',');
		if (comma == std::string::npos) {
			occupants_error("Error: occupants.conf on line " + line + " is not formatted correctly. Expected format is <name>,<number>[,<number>...]");
		}
		std::string number = line.substr(comma + 1);
		number.erase(std::remove(number.begin(), number.end(), ','), number.end()); // remove ,

		Occupant occ = {.number = number, .name = line.substr(0, comma)};

		if (occ.name.size() > max_occupant_name_length)
			occupants_error("Occupant name \"" + occ.name + "\" too long, max is " + std::to_string(max_occupant_name_length));
		if (std::find(cameras.begin(), cameras.end(), occ.number) != cameras.end())
			occupants_error("Phone number \"" + occ.number + "\" is already in use by the front door camera");

		occupants.push_back(occ);
	}
	if (occupants.size() == 0)
		occupants_error("occupants.conf is empty");

	std::sort(occupants.begin(), occupants.end(), [](const Occupant& a, const Occupant& b) { return a.name < b.name; });
	return occupants;
}

static std::vector<size_t> match_score_continuous(const std::string& name, const std::string& query) {
	std::vector<size_t> score;
	const size_t		substring_i = name.find(query);

	if (substring_i != std::string::npos && (substring_i == 0 || name.at(substring_i - 1) == ' ')) {
		for (size_t i = 0; i < query.size(); i++)
			score.push_back(substring_i + i);
	}
	return score;
}

// Returns equability score of name (as in human name like John Doe) and query
// Returns a array of the indexes matched, the longer the array the higher the match score
// Score 0~length of query
// 0 means that the query does not match the name or that 0 chars matched the name string
// it also ignores all non alphabetical chars, like "-" and "." that are commonly found in human names
static std::vector<size_t> match_score_interrupted(const std::string& name, const std::string& query) {
	std::vector<size_t> score;
	size_t				i_name = 0;
	size_t				i_query = 0;

	while (1) {
		while (i_name < name.size() && i_query < query.size()) {
			if (!std::isalpha(name[i_name]) && name[i_name] != ' ') {
				i_name++;
				continue;
			}

			if (name[i_name] == query[i_query]) {
				score.push_back(i_name);
				i_name++;
				i_query++;
			}
			else
				break;
		}

		// reached end of query so return number of matched chars
		if (i_query >= query.size())
			return score;

		// skip to next word in name
		while (i_name < name.size() && name[i_name++] != ' ')
			;

		if (i_name >= name.size())
			return i_query < query.size() ? std::vector<size_t>() : score;
	}
}

std::vector<size_t> match_score(std::string name, std::string query) {
	std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
	std::transform(query.begin(), query.end(), query.begin(), [](unsigned char c) { return std::tolower(c); });

	auto continuous = match_score_continuous(name, query);
	auto interrupted = match_score_interrupted(name, query);

	return continuous.size() > interrupted.size() ? continuous : interrupted;
}

std::vector<Occupant> get_occupants_query(const std::string& query, size_t max_results) {
	std::vector<size_t>	  scores;
	std::vector<Occupant> results;
	size_t				  max_score = 0;

	for (const Occupant& occupant : consts().occupants)
		max_score = std::max(max_score, match_score(occupant.name, query).size());

	if (max_score == 0) // if the query matches no occupant names
		return get_occupants_scroll(0, max_results);

	while (max_score) {
		for (const Occupant& occupant : consts().occupants) {
			if (match_score(occupant.name, query).size() == max_score)
				results.push_back(occupant);
			if (results.size() == max_results)
				return results;
		}
		max_score--;
	}

	return results;
}

std::vector<Occupant> get_occupants_scroll(size_t scroll_pos, size_t max_results) {
	std::vector<Occupant>::const_iterator end = std::min(consts().occupants.end(), consts().occupants.begin() + scroll_pos + max_results);
	std::vector<Occupant>				  results(consts().occupants.begin() + scroll_pos, end);
	return results;
}
