#pragma once

template<class T>
class Client : public std::enable_shared_from_this<T> {
	websocketpp::connection_hdl hdl_;
	websocketpp::server<websocketpp::config::asio> &server_;

public:
	typedef std::shared_ptr<T> Ptr;

	Client(websocketpp::connection_hdl hdl, websocketpp::server<websocketpp::config::asio>& server)
		: hdl_(hdl), server_(server) { }

	bool send(const std::string& msg, bool isText = true) {
		try {
			server_.send(hdl_, msg, isText ? websocketpp::frame::opcode::text : websocketpp::frame::opcode::binary);
			return true;
		} catch (...) {
			return false;
		}
	}
};
