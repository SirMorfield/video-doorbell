#include "main.hpp"

namespace commands {

std::string ring(const std::string& phonenumber) {
	return "asterisk -rx 'channel originate sip/" + phonenumber + " extension " + consts().camera_numbers[Camera_type::FRONT_DOOR] + "@internal'";
}

std::string get_channels() {
	return "asterisk -rx 'core show channels concise'";
}

std::string hangup(const std::string& phonenumber) {
	return "asterisk -rx 'hangup request " + phonenumber + "'";
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

bool is_channel_with_camera(const std::string& channel) {
	for (const std::string& camera_number : consts().camera_numbers) {
		if (channel.find("SIP/" + camera_number) != std::string::npos)
			return true;
	}
	return false;
}

void end_calls_with_cameras() {
	const std::vector<std::string> channels = get_channels();
	for (const std::string& channel : channels) {
		if (is_channel_with_camera(channel)) {
			std::cout << "request hangup for channel " << channel << std::endl;
			exec(commands::hangup(channel));
		}
	}
}

// let the front door camera ring the phonenumber
void ring(const std::string& phonenumber) {
	// first make sure that none of the cameras are already in a call
	end_calls_with_cameras();
	append_to_logfile("ringing " + phonenumber);
	exec(commands::ring(phonenumber));
}

} // namespace sip
