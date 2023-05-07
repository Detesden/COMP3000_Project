#pragma once


#include <numbers>
#include <cmath>


inline float Deg2Rad(const float deg)
{
	return deg * (std::numbers::pi_v<float> / 180.f);
}

struct Vec2
{
	float x{};
	float y{};
};

struct Vec4
{
	float x{};
	float y{};
	float z{};
	float w{};
};



class CVector
{
public:
	float x{};
	float y{};
	float z{};

	bool isNull(const float tolerance = 1e-6f) const // 6 decimal points by default
	{
		return (std::abs(x) < tolerance && std::abs(y) < tolerance && std::abs(z) < tolerance);
	}

	CVector AngleToVector() const
	{
		return
		{
			std::cos(Deg2Rad(x)) * std::cos(Deg2Rad(y)),
			std::cos(Deg2Rad(x)) * std::sin(Deg2Rad(y)),
			-std::sin(Deg2Rad(x))
		};
	}

	CVector VectorToAngle() const
	{
		return
		{
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f
		};
	}

	static float GetDistance(const CVector& src, const CVector& dst)
	{
		CVector delta { dst - src };

		return std::sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
	}

	static float Length(const CVector& vector)
	{
		return std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	}

	float Length() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	// ------------------------------------------------ Operator overloads ------------------------------------------------
	// 
	// Reminder: A friend function is a function that can access the private members of a class as though it was a member of that class.
	friend bool operator==(const CVector& firstOperand, const CVector& secondOperand) {
		return firstOperand.x == secondOperand.x && firstOperand.y == secondOperand.y && firstOperand.z == secondOperand.z;
	}

	friend bool operator!=(const CVector& firstOperand, const CVector& secondOperand) {
		return !(firstOperand == secondOperand);
	}

	// These can be made 'friend' too
	CVector operator-(const CVector& secondOperand) const {
		return { x - secondOperand.x, y - secondOperand.y, z - secondOperand.z };
	}

	CVector operator+(const CVector& secondOperand) const {
		return { x + secondOperand.x, y + secondOperand.y, z + secondOperand.z };
	}

	CVector operator/(const CVector& secondOperand) const {
		return { x / secondOperand.x, y / secondOperand.y, z / secondOperand.z };
	}

	CVector operator*(const CVector& secondOperand) const {
		return { x * secondOperand.x, y * secondOperand.y, z * secondOperand.z };
	}

	CVector operator/(const float& factor) const {
		return { x / factor, y / factor, z / factor };
	}

	CVector operator*(const float& scale) const {
		return { x * scale, y * scale, z * scale };
	}
};