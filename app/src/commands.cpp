#include "main.hpp"

namespace commands {

std::string ring(const std::string& client_phonenumber) {
	return "docker exec -it asterisk asterisk -rx 'channel originate sip/" + client_phonenumber + " extension 7004@internal'";
}

}; // namespace commands
