
struct EulerAngle
{
	double roll, pitch, yaw;

	EulerAngle(double r = 0.0, double p = 0.0, double y = 0.0);

	EulerAngle operator+(const EulerAngle& rhs) const;

	EulerAngle operator+=(const EulerAngle& rhs);

	EulerAngle operator-(const EulerAngle& rhs) const;

	EulerAngle operator*(double amount) const;

	EulerAngle operator*=(double amount);

	EulerAngle operator/=(double amount);
};

struct Vector3D
{
	double x, y, z;

	Vector3D(double x = 0.0, double y = 0.0, double z = 0.0);

	Vector3D operator+=(const Vector3D& rhs);

	Vector3D operator-(const Vector3D& rhs) const;

	Vector3D operator*(double amount) const;

	Vector3D operator*=(double amount);

	Vector3D operator/=(double amount);
};

struct C_Sensor
{
	double gyro_x, gyro_y, gyro_z;
	double accel_x, accel_y, accel_z;
};

struct Sensor
{
	Vector3D gyro;
	Vector3D accel;

	explicit Sensor(Vector3D gyro = {}, Vector3D accel = {});

	Sensor(const C_Sensor& raw);

	Sensor operator-(const Sensor rhs) const;

	Sensor operator+=(const Sensor rhs);

	Sensor operator/=(double amount);

	EulerAngle GetEulerAngleByAccel() const;

	EulerAngle GetEulerAngleByGyro() const;
};

struct ComplementaryFilter
{
	static EulerAngle Filterate(const EulerAngle& accel, const EulerAngle& gyro);
};