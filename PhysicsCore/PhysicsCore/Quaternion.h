#pragma once

#include <math.h>
#include "Vector3.h"

struct Quaternion {
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;

	float Length()
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	float LengthSquared()
	{
		return x * x + y * y + z * z + w * w;
	}

	void Normalize()
	{
		float length = Length();
		if (length == 0) return;
		w /= length;
		x /= length;
		y /= length;
		z /= length;
	}

	Quaternion Inversed()
	{
		Quaternion result;
		result.x = x * -1;
		result.y = y * -1;
		result.z = z * -1;
		result.w = w;
		return result;
	}

	Quaternion AngleAxis(const Vector3 &axis, float angle) const
	{
		float factor = sinf(angle / 2.0f);

		Quaternion rotation;
		rotation.x = axis.x * factor;
		rotation.y = axis.y * factor;
		rotation.z = axis.z * factor;
		rotation.w = cosf(angle / 2.0f);
		rotation.Normalize();

		return rotation;
	}

	Quaternion operator * (const Quaternion &other) const
	{
		Quaternion result;
		result.x = w * other.x + x * other.w + y * other.z - z * other.y;
		result.y = w * other.y + y * other.w + z * other.x - x * other.z;
		result.z = w * other.z + z * other.w + x * other.y - y * other.x;
		result.w = w * other.w - x * other.x - y * other.y - z * other.z;
		return result;
	}

	void operator *= (const Quaternion &other)
	{
		Quaternion temp;
		temp.x = w * other.x + x * other.w + y * other.z - z * other.y;
		temp.y = w * other.y + y * other.w + z * other.x - x * other.z;
		temp.z = w * other.z + z * other.w + x * other.y - y * other.x;
		temp.w = w * other.w - x * other.x - y * other.y - z * other.z;

		x = temp.x;
		y = temp.y;
		z = temp.z;
		w = temp.w;
	}
};