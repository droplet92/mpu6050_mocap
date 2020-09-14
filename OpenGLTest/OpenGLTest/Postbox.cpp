#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include "Postbox.h"
#include <Windows.h>
#include <iostream>
#include <cstring>
#include <chrono>
#include "dof.h"

#define SERVER_PORT		48050
#define BACKLOG_SIZE	5
#define BUFSIZE			128


Sensor base;


namespace SocketIO
{
	Postbox::Postbox()
		: client_len(sizeof client_addr)
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		{
			fprintf(stderr, "WSAStartup() failed.\n");
			return;
		}
		if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		{
			fprintf(stdout, "socket() failed.\n");
			return;
		}
		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		server_addr.sin_port = htons(SERVER_PORT);

		if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		{
			fprintf(stdout, "bind() failed.\n");
			return;
		}
		listen(socket_fd, BACKLOG_SIZE);

		client_fd = accept(socket_fd, (struct sockaddr*)&client_addr, &client_len);
		if (client_fd < 0)
		{
			fprintf(stdout, "accept() failed.\n");
		}
	}

	Postbox::~Postbox()
	{
		closesocket(client_fd);
		closesocket(socket_fd);
		WSACleanup();
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
			recv(client_fd, (char*)&c_sensor, sizeof c_sensor, 0);
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
		if (recv(client_fd, (char*)&c_sensor, sizeof c_sensor, 0) > 0)
		{
			auto dt = (high_resolution_clock::now() - start);
			sensor = Sensor(c_sensor) - base;
			sensor.gyro *= (dt.count() / 1'000'000'000.0);
		}
		return sensor;
	}
}