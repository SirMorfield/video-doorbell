#include "main.hpp"

namespace commands {

std::string ring(const std::string& client_phonenumber) {
	std::cout << "ringing " << client_phonenumber << std::endl;
	return "docker exec -it asterisk asterisk -rx 'channel originate sip/" + client_phonenumber + " extension 7004@internal'";
}

}; // namespace commands
