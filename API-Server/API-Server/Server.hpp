#pragma once
#define _WEBSOCKETPP_CPP11_THREAD_
#define _WEBSOCKETPP_CPP11_CHRONO_
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <thread>
#include <memory>
#include <map>

template<
	class DelegateType,
	class ClientType
>
class Server : public websocketpp::server<websocketpp::config::asio> {
	DelegateType *delegate_;
	std::map<websocketpp::connection_hdl, std::shared_ptr<ClientType>, std::owner_less<websocketpp::connection_hdl>> clients_;
	std::thread thread_;

public:
	Server(DelegateType *delegate) : delegate_(delegate) {
		set_access_channels(websocketpp::log::alevel::none);
		clear_access_channels(websocketpp::log::alevel::none);

		init_asio();

		set_open_handler([this](auto hdl) {
			auto ptr = std::make_shared<ClientType>(hdl, (websocketpp::server<websocketpp::config::asio> &)*this);
			clients_[hdl] = ptr;

			ptr->onConnect();
			delegate_->serverOnConnection(ptr);
		});

		set_message_handler([this](auto hdl, auto msg) {
			auto payload = msg->get_payload();
			auto client = clients_[hdl];

			client->onMessage(payload);
			delegate_->serverOnMessage(client, payload);
		});

		set_close_handler([this](auto hdl) {
			auto client = clients_[hdl];

			client->onClose();
			delegate_->serverOnClose(client);

			clients_.erase(hdl);
		});
	}

	~Server() {
		stopServer();
	}

	void runServer(uint16_t port) {
		thread_ = std::thread([this, port]() {
			listen(port);
			start_accept();
			run();
		});
	}

	void stopServer() {
		stop();

		if (thread_.joinable()) {
			thread_.join();
		}
	}
};
