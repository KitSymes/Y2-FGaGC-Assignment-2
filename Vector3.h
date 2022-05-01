#pragma once
#ifndef _VECTOR3_H
#define _VECTOR3_H

#include <DirectXMath.h>
class Quaternion;

class Vector3
{
public:
	float x, y, z;

	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const Vector3& copyFrom);			// Copy constructor

	Vector3 operator+(const Vector3& rhs);		// Add (and return) two vectors
	Vector3& operator+=(const Vector3& rhs);	// Add to this vector
	Vector3 operator-(const Vector3& rhs);		// Subtract (and return) two vectors
	Vector3& operator-=(const Vector3& rhs);	// Subtract from this vector
	Vector3 operator*(float value);				// Multiply (and return) two vectors
	Vector3& operator*=(float value);			// Multiply this vector
	Vector3 operator/(float value);				// Divide (and return) two vectors
	Vector3& operator/=(float value);			// Divide this vector
	Vector3& operator=(const Vector3& rhs);		// Re-assign this vector
	bool operator==(const Vector3& rhs);		// Compare this vector to another

	float DotProduct(const Vector3& vec);
	Vector3 CrossProduct(const Vector3& vec);
	float Magnitude();

	bool IsNormalised();
	void Normalise();

	DirectX::XMFLOAT3 ToXMFLOAT3();
};

Vector3 operator*(float lhs, Vector3& rhs); // Multiply this vector
Vector3 operator*(Quaternion lhs, Vector3& rhs); // Multiply this vector
#endif
