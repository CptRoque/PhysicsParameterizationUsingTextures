#pragma once

struct Vector2 {
	float x;
	float y;

	Vector2()
	{
		x = 0;
		y = 0;
	}

	Vector2(float x, float y) : x(x), y(y)
	{
	}

	Vector2(const Vector2 &other)
	{
		x = other.x;
		y = other.y;
	}

	float SquaredLength()
	{
		return x * x + y * y;
	}

	Vector2 operator + (const Vector2 &other) const
	{
		Vector2 result(*this);
		result += other;
		return result;
	}

	void operator += (const Vector2 &other)
	{
		x += other.x;
		y += other.y;
	}

	Vector2 operator - (const Vector2 &other) const
	{
		Vector2 result(*this);
		result -= other;
		return result;
	}

	void operator -= (const Vector2 &other)
	{
		x -= other.x;
		y -= other.y;
	}

	Vector2 operator * (const Vector2 &other) const
	{
		Vector2 result(*this);
		result *= other;
		return result;
	}

	void operator *= (const Vector2 &other)
	{
		x *= other.x;
		y *= other.y;
	}

	Vector2 operator * (float other) const
	{
		Vector2 result(*this);
		result *= other;
		return result;
	}

	void operator *= (float other)
	{
		x *= other;
		y *= other;
	}
	
	bool operator == (const Vector2 &other) const
	{
		return  x == other.x && y == other.y;
	}
};