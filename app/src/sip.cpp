#include "main.hpp"

namespace commands {

std::string ring(const std::string& phonenumber) {
	return "docker exec asterisk asterisk -rx 'channel originate sip/" + phonenumber + " extension " + consts().front_door_number + "@internal'";
}

std::string get_channels() {
	return "docker exec -i asterisk asterisk -rx 'core show channels concise'";
}

std::string hangup(const std::string& phonenumber) {
	return "docker exec asterisk asterisk -rx 'hangup request " + phonenumber + "'";
}

}; // namespace commands

namespace sip {

// converting "SIP/7004-00000032!..."" to "SIP/7004-00000032"
std::vector<std::string> get_channels() {
	std::optional<std::string> channels = exec(commands::get_channels());
	if (!channels.has_value())
		return {};
	std::vector<std::string> channel_names = ft_split(channels.value(), "\n");
	for (std::string& name : channel_names) {
		name = name.substr(0, name.find_first_of('!'));
	}
	return channel_names;
}

// let the front door camera ring the phonenumber
void ring(const std::string& phonenumber) {
	// first make sure that the front door camera is not already in a call

	const std::vector<std::string> channels = get_channels();
	for (const std::string& channel : channels) {
		if (channel.find(consts().front_door_number) != std::string::npos) {
			std::cout << "request hangup for channel " << channel << std::endl;
			exec(commands::hangup(channel));
		}
	}

	exec(commands::ring(phonenumber));
}

} // namespace sip
