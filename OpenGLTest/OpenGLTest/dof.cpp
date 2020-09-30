#define _USE_MATH_DEFINES

#include "dof.h"
#include <math.h>


EulerAngle::EulerAngle(double r, double p, double y)
    : roll(r)
    , pitch(p)
    , yaw(y)
{
}

EulerAngle EulerAngle::operator+(const EulerAngle& rhs) const
{
    return EulerAngle{
        roll + rhs.roll,
        pitch + rhs.pitch,
        yaw + rhs.yaw
    };
}

EulerAngle EulerAngle::operator+=(const EulerAngle& rhs)
{
    roll += rhs.roll;
    pitch += rhs.pitch;
    yaw += rhs.yaw;
    return *this;
}

EulerAngle EulerAngle::operator-(const EulerAngle& rhs) const
{
    return EulerAngle{
        roll - rhs.roll,
        pitch - rhs.pitch,
        yaw - rhs.yaw
    };
}

EulerAngle EulerAngle::operator*(double amount) const
{
    return EulerAngle{
        roll * amount,
        pitch * amount,
        yaw * amount
    };
}

EulerAngle EulerAngle::operator*=(double amount)
{
    roll *= amount;
    pitch *= amount;
    yaw *= amount;
    return *this;
}

EulerAngle EulerAngle::operator/=(double amount)
{
    roll /= amount;
    pitch /= amount;
    yaw /= amount;
    return *this;
}

Vector3D::Vector3D(double x, double y, double z)
    : x(x)
    , y(y)
    , z(z)
{
}

Vector3D Vector3D::operator+=(const Vector3D& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Vector3D Vector3D::operator-(const Vector3D& rhs) const
{
    return Vector3D{
        x - rhs.x,
        y - rhs.y,
        z - rhs.z
    };
}

Vector3D Vector3D::operator*(double amount) const
{
    return Vector3D{
        x * amount,
        y * amount,
        z * amount
    };
}

Vector3D Vector3D::operator*=(double amount)
{
    x *= amount;
    y *= amount;
    z *= amount;
    return *this;
}

Vector3D Vector3D::operator/=(double amount)
{
    x /= amount;
    y /= amount;
    z /= amount;
    return *this;
}

Sensor::Sensor(Vector3D gyro, Vector3D accel)
    : gyro(gyro)
    , accel(accel)
{
}

Sensor::Sensor(const C_Sensor& raw)
{
    gyro = { raw.gyro_x, raw.gyro_y, raw.gyro_z };
    accel = { raw.accel_x, raw.accel_y, raw.accel_z };
}

Sensor Sensor::operator-(const Sensor rhs) const
{
    return Sensor{
        gyro - rhs.gyro,
        accel - rhs.accel
    };
}

Sensor Sensor::operator+=(const Sensor rhs)
{
    gyro += rhs.gyro;
    accel += rhs.accel;
    return *this;
}

Sensor Sensor::operator/=(double amount)
{
    gyro /= amount;
    accel /= amount;
    return *this;
}

EulerAngle Sensor::GetEulerAngleByAccel() const
{
    constexpr double radians_to_degrees = 180.0 / M_PI;
    return EulerAngle{
        atan2(accel.y, sqrt(accel.x * accel.x + accel.z * accel.z)),
        atan2(-accel.x, sqrt(accel.y * accel.y + accel.z * accel.z)),
        0.0
    } * radians_to_degrees;
}

EulerAngle Sensor::GetEulerAngleByGyro() const
{
    return EulerAngle{
        gyro.x,
        gyro.y,
        gyro.z
    };
}

EulerAngle ComplementaryFilter::Filterate(const EulerAngle& accel, const EulerAngle& gyro)
{
    constexpr double gyro_gain = 0.80;
    constexpr double accel_gain = 1.0 - gyro_gain;
    static EulerAngle filtered_gyro = {};
    filtered_gyro += gyro;
    return (filtered_gyro = (filtered_gyro * gyro_gain + accel * accel_gain));
}
