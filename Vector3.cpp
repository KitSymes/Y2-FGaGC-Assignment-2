#include "Vector3.h"
#include <assert.h>
#include <math.h>

Vector3::Vector3() : Vector3(0.0f, 0.0f, 0.0f) {}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(const Vector3& copyFrom) : Vector3(copyFrom.x, copyFrom.y, copyFrom.z) {}

Vector3 Vector3::operator+(const Vector3& rhs)
{
	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3& Vector3::operator+=(const Vector3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

Vector3 Vector3::operator-(const Vector3& rhs)
{
	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3& Vector3::operator-=(const Vector3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

Vector3 Vector3::operator*(float value)
{
	return Vector3(x * value, y * value, z * value);
}

Vector3& Vector3::operator*=(float value)
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

Vector3 Vector3::operator/(float value)
{
	assert(value != 0);
	return Vector3(x / value, y / value, z / value);
}

Vector3& Vector3::operator/=(float value)
{
	assert(value != 0);
	x /= value;
	y /= value;
	z /= value;
	return *this;
}

Vector3& Vector3::operator=(const Vector3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}

bool Vector3::operator==(const Vector3& rhs)
{
	return (x == rhs.x && y == rhs.y && z == rhs.z);
}

float Vector3::DotProduct(const Vector3& vec)
{
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}

Vector3 Vector3::CrossProduct(const Vector3& vec)
{
	return Vector3((y * vec.z) - (z * vec.y),
		(z * vec.x) - (x * vec.z),
		(x * vec.y) - (y * vec.x));
}

float Vector3::Magnitude()
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

bool Vector3::IsNormalised()
{
	return (x * x) + (y * y) + (z * z) == 1.0f;
}

void Vector3::Normalise()
{
	if (x == 0.0f && y == 0.0f && z == 0.0f)
		return;
	if (IsNormalised())
		return;

	float magnitude = Magnitude();
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
}

DirectX::XMFLOAT3 Vector3::ToXMFLOAT3()
{
	return DirectX::XMFLOAT3(x, y, z);
}
