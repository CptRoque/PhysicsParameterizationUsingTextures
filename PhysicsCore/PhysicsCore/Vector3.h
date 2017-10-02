#pragma once

#include <math.h>

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3(float x, float y, float z) : x(x), y(y), z(z)
	{
	}

	Vector3(const Vector3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	float Length()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	float SquaredLength()
	{
		return x * x + y * y + z * z;
	}

	void Normalize()
	{
		float length = Length();
		if (length == 0) return;
		x /= length;
		y /= length;
		z /= length;
	}

	Vector3 GetNormalized()
	{
		Vector3 result(*this);
		result.Normalize();
		return result;
	}

	bool operator ==(const Vector3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	Vector3 operator +(const Vector3& other) const
	{
		Vector3 result(*this);
		result += other;
		return result;
	}

	void operator +=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}

	Vector3 operator -(const Vector3& other) const
	{
		Vector3 result(*this);
		result -= other;
		return result;
	}

	void operator -=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}

	Vector3 operator *(const Vector3& other) const
	{
		Vector3 result(*this);
		result *= other;
		return result;
	}

	void operator *=(const Vector3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
	}

	Vector3 operator /(const Vector3& other) const
	{
		Vector3 result(*this);
		result /= other;
		return result;
	}

	void operator /=(const Vector3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
	}

	Vector3 operator *(float other) const
	{
		Vector3 result(*this);
		result *= other;
		return result;
	}

	void operator *=(float other)
	{
		x *= other;
		y *= other;
		z *= other;
	}

	Vector3 operator /(float other) const
	{
		Vector3 result(*this);
		result /= other;
		return result;
	}

	void operator /=(float other)
	{
		x /= other;
		y /= other;
		z /= other;
	}

	float& operator[](int index)
	{
		switch (index)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			return x;
		}
	}

	void epsilonToZero(float epsilon)
	{
		if (x <= epsilon)
		{
			x = 0;
		}
		if (y <= epsilon)
		{
			y = 0;
		}
		if (z <= epsilon)
		{
			z = 0;
		}
	}
};
