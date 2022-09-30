#include "main.hpp"
#include <algorithm>
#include <exception>

[[noreturn]] void occupants_error(const std::string& msg) {
	std::cout << "\n\n========================================" << std::endl;
	std::cout << msg << std::endl;
	std::cout << "========================================\n\n " << std::endl;
	exit(1);
}

std::vector<Occupant> read_occupants(const std::array<std::string, Camera_type::N>& cameras, size_t max_occupant_name_length) {
	std::vector<Occupant>	 occupants;
	static const std::string path = get_binary_location().value() + "/occupants.csv";
	std::string				 apt_file = read_file(path);
	std::vector<std::string> lines = ft_split(apt_file, "\n");
	for (const std::string& line : lines) {
		if (line.find('\t') != std::string::npos)
			occupants_error("Found disallowed tab in line \"" + line + "\"");

		std::vector<std::string> fields = ft_split(line, ",");
		if (fields.size() < 2)
			continue;
		if (fields.size() > 2)
			occupants_error("Invalid line \"" + line + "\" should only contain 2 elements");
		Occupant occ = {.number = fields.at(0), .name = fields.at(1)};
		if (occ.name.size() > max_occupant_name_length)
			occupants_error("Occupant name \"" + occ.name + "\" too long, max is " + std::to_string(max_occupant_name_length));
		if (std::find(cameras.begin(), cameras.end(), occ.number) != cameras.end())
			occupants_error("Phone number \"" + occ.number + "\" is already in use by the front door camera");
		occupants.push_back(occ);
	}
	if (occupants.size() == 0)
		occupants_error("occupants.csv is empty");

	std::sort(occupants.begin(), occupants.end(), [](const Occupant& a, const Occupant& b) { return a.name < b.name; });
	return occupants;
}

// Returns equability score of name (as in human name like John Doe) and query
// Higher score means better match with name
// Score 0~length of query
// 0 means that the query does not match the name or that 0 chars matched the name string
// it also ignoreas all non alphabetical chars, like "-" and "." that are commonly found in human names
std::vector<size_t> match_score(const std::string& name, const std::string& query) {
	std::vector<size_t> score;
	size_t				i_name = 0;
	size_t				i_query = 0;

	while (1) {
		while (i_name < name.size() && i_query < query.size()) {
			if (!std::isalpha(name[i_name]) && name[i_name] != ' ') {
				i_name++;
				continue;
			}

			if (std::tolower(name[i_name]) == std::tolower(query[i_query])) {
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
