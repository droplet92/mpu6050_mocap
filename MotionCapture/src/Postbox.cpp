#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include "Postbox.h"
#include "Angles.h"

#include <WinSock2.h>
#include <Windows.h>
#include <chrono>

#define SERVER_PORT		48050
#define BACKLOG_SIZE	5
#define BUFSIZE			128


Sensor base;

namespace SocketIO
{
	struct Postbox::Impl
	{
		WSADATA wsadata;
		SOCKADDR_IN server_addr, client_addr;
		SOCKET socket_fd;
		int client_len;
	};

	Postbox::Postbox()
		: pimpl(new Impl())
	{
		pimpl->client_len = sizeof(pimpl->client_addr);

		if (WSAStartup(MAKEWORD(2, 2), &(pimpl->wsadata)) != 0)
		{
			fprintf(stderr, "WSAStartup() failed.\n");
			return;
		}
		if ((pimpl->socket_fd = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		{
			fprintf(stdout, "socket() failed.\n");
			return;
		}
		memset(&(pimpl->server_addr), 0, sizeof(pimpl->server_addr));
		pimpl->server_addr.sin_family = AF_INET;
		pimpl->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		pimpl->server_addr.sin_port = htons(SERVER_PORT);

		if (bind(pimpl->socket_fd, (struct sockaddr*)&(pimpl->server_addr), sizeof(pimpl->server_addr)) < 0)
		{
			fprintf(stdout, "bind() failed.\n");
			return;
		}
	}

	Postbox::~Postbox()
	{
		closesocket(pimpl->socket_fd);
		WSACleanup();
		delete pimpl;
	}

	void Postbox::CalibrateSensor()
	{
		using namespace std::chrono;
		C_Sensor c_sensor{};
		int count = 0;
		
		for (auto start = high_resolution_clock::now();
			duration_cast<seconds>(high_resolution_clock::now() - start).count() < 3;
			++count)
		{
			recvfrom(pimpl->socket_fd, (char*)&c_sensor, sizeof(c_sensor), 0,
					(struct sockaddr*)&(pimpl->client_addr), &pimpl->client_len);
			base += c_sensor;
		}
		base /= static_cast<double>(count);
	}

	Sensor Postbox::GetSensorData()
	{
		using namespace std::chrono;
		C_Sensor c_sensor{};
		Sensor sensor{};

		auto start = high_resolution_clock::now();
		if (recvfrom(pimpl->socket_fd, (char*)&c_sensor, sizeof(c_sensor), 0,
					(struct sockaddr*)&(pimpl->client_addr), &pimpl->client_len) > 0)
		{
			auto dt = (high_resolution_clock::now() - start);
			sensor = Sensor(c_sensor) - base;
			sensor.gyro *= (dt.count() / 1'000'000'000.0);
		}
		return sensor;
	}
}