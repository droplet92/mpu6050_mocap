#pragma once
#include <WinSock2.h>

struct Sensor;

namespace SocketIO
{
	class Postbox
	{
	public:
		~Postbox();

		static Postbox& instance()
		{
			static Postbox* postbox = new Postbox;
			return *postbox;
		}

		void CalibrateSensor();

		Sensor GetSensorData();

	private:
		explicit Postbox();

		WSADATA wsadata;
		SOCKADDR_IN server_addr, client_addr;
		SOCKET socket_fd, client_fd;
		int client_len;
	};
}