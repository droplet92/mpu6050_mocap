#pragma once

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
		Postbox();

		struct Impl;
		Impl* pimpl;
	};
}