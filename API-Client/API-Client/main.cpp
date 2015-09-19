
#define _WEBSOCKETPP_CPP11_THREAD_
#define _WEBSOCKETPP_CPP11_CHRONO_
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <thread>
#include <functional>

template<typename DelegateType>
class Client : public websocketpp::client<websocketpp::config::asio> {
	DelegateType *delegate_;
	std::string url_;
	std::thread thread_;
	websocketpp::connection_hdl hdl_;
	websocketpp::client<websocketpp::config::asio>::connection_ptr conPtr_;

public:
	Client(DelegateType *delegate, const std::string& url) : delegate_(delegate), url_(url) { 
		init_asio();

		set_open_handler(std::bind(&DelegateType::clientOpened, delegate_, this));

		set_fail_handler(std::bind(&DelegateType::clientFailed, delegate_, this));

		set_close_handler(std::bind(&DelegateType::clientClosed, delegate_, this));

		set_message_handler([](auto hdl, auto msg) {

		});
	}
	
	~Client() {
		stopClient();
	}

	void startClient() {
		websocketpp::lib::error_code ec;
		conPtr_ = get_connection(url_, ec);
		if (ec)
			throw websocketpp::exception(ec);

		hdl_ = conPtr_->get_handle();

		connect(conPtr_);

		thread_ = std::thread([this]() {
			run();
		});
	}

	void stopClient() {
		stop();

		if (thread_.joinable())
			thread_.join();
	}
};

class Controller {
	Client<Controller> client_;

public:
	Controller() : client_(this, "ws://localhost:9000") {
		client_.startClient();
	}

	void clientOpened(Client<Controller> *client) {

	}

	void clientFailed(Client<Controller> *client) {

	}

	void clientClosed(Client<Controller> *client) {

	}
};

int main() {

	try {{
			Controller controller;
			std::cin.get();
		}

		std::cin.get();
	}
	catch (const websocketpp::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
}