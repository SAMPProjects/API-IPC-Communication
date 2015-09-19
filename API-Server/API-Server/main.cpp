#include "Server.hpp"
#include "Client.hpp"
#include <iostream>


class CustomClient : public Client<CustomClient> {
public:
	using Client::Client;

	void onConnect() {
		std::cout << __func__ << std::endl;
	}

	void onMessage(const std::string &msg) {
		std::cout << __func__ << std::endl;
		send(msg);
	}

	void onClose() {
		std::cout << __func__ << std::endl;
	}
};

class Controller {
	typedef CustomClient ClientType;
	Server<Controller, ClientType> server_;

public:
	Controller() : server_(this) {
		server_.runServer(9000);
	}

	void serverOnConnection(ClientType::Ptr client) {
		std::cout << __func__ << std::endl;
	}

	void serverOnMessage(ClientType::Ptr client, const std::string& message) {
		std::cout << __func__ << std::endl;
	}

	void serverOnClose(ClientType::Ptr client) {
		std::cout << __func__ << std::endl;
	}
};

int main() {
	try {{
			Controller controller;
			std::cin.get();
		}
		std::cin.get();
	}

	catch (const websocketpp::exception& er) {
		std::cout << "Error occured while starting server: " << er.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
