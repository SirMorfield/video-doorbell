#include "main.hpp"

std::vector<Occupant> read_occupants() {
	std::vector<Occupant>	 occupants;
	std::string				 apt_file = read_file("./occupants.csv");
	std::vector<std::string> lines = ft_split(apt_file, "\n");
	for (const std::string& line : lines) {
		std::vector<std::string> fields = ft_split(line, ",");
		if (fields.size() < 2)
			continue;
		occupants.push_back((Occupant){.number = fields.at(0), .name = fields.at(1)});
	}
	ASSERT(occupants.size(), >=, 1);
	std::sort(occupants.begin(), occupants.end(), [](const Occupant& a, const Occupant& b) { return a.name < b.name; });
	return occupants;
}

// Returns equability score of name and query
// Higher score means better match with name
// Score 0~length of query
unsigned int match_score(const std::string& name, const std::string& query) {
	std::vector<std::string> words = ft_split(name, " ");

	unsigned int			 score = 0;
	for (const std::string& word : words) {

		size_t i = 0;
		if (query.size()) {
			size_t start = word.find_first_of(query[0]);
			if (start != std::string::npos)
				i = start;
		}
		for (; i < word.size() &&
			   score < query.size() &&
			   std::tolower(word[i]) == std::tolower(query[score]);
			 i++) {
			score++;
		}
	}
	return score;
}

std::vector<Occupant> get_occupants_query(const std::string& query, size_t max_results) {
	std::vector<unsigned int> scores;

	for (const Occupant& occupant : consts().occupants) {
		scores.push_back(match_score(occupant.name, query));
	}

	std::vector<Occupant> results;
	unsigned int		  highest_score = *std::max_element(scores.begin(), scores.end());
	max_results = std::min(max_results, consts().occupants.size());
	do {
		for (size_t i = 0; i < scores.size(); i++) {
			if (scores[i] == highest_score) {
				results.push_back(consts().occupants[i]);
				if (results.size() == max_results)
					goto end;
				scores[i] = -1;
			}
		}

	} while (highest_score--);
end:
	return results;
}

std::vector<Occupant> get_occupants_scroll(size_t scroll_pos, size_t max_results) {
	static const std::vector<Occupant> occupants = read_occupants();
	std::vector<Occupant>			   results(occupants.begin() + scroll_pos, occupants.begin() + scroll_pos + max_results);
	return results;
}
