// EXTERNAL INCLUDES
#include <cmath>
#include <assert.h>
// INTERNAL INCLUDES
#include "math/vec3.h"

using namespace Math;

const Vec3 Vec3::zero = { 0.0f, 0.0f, 0.0f };
const Vec3 Vec3::unit_x = { 1.0f, 0.0f, 0.0f };
const Vec3 Vec3::unit_y = { 0.0f, 1.0f, 0.0f };
const Vec3 Vec3::unit_z = { 0.0f, 0.0f, 1.0f };
const Vec3 Vec3::neg_unit_x = { -1.0f, 0.0f, 0.0f };
const Vec3 Vec3::neg_unit_y = { 0.0f, -1.0f, 0.0f };
const Vec3 Vec3::neg_unit_z = { 0.0f, 0.0f, -1.0f };
const Vec3 Vec3::unit_scale = { 1.0f, 1.0f, 1.0f };

Vec3& Math::operator+=(Vec3& lhs, const float scalar)
{
	lhs.x += scalar;
	lhs.y += scalar;
	lhs.z += scalar;
	return lhs;
}
Vec3& Math::operator-=(Vec3& lhs, const float scalar)
{
	lhs.x -= scalar;
	lhs.y -= scalar;
	lhs.z -= scalar;
	return lhs;
}
Vec3& Math::operator*=(Vec3& lhs, const float scalar)
{
	lhs.x *= scalar;
	lhs.y *= scalar;
	lhs.z *= scalar;
	return lhs;
}
Vec3& Math::operator/=(Vec3& lhs, const float scalar)
{
	// scalar might be 0.0f which would crash
	// so we need to intercept here
	assert(scalar != 0.0f);
	lhs.x /= scalar;
	lhs.y /= scalar;
	lhs.z /= scalar;
	return lhs;
}
Vec3& Math::operator+=(Vec3& lhs, const Vec3& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}
Vec3& Math::operator-=(Vec3& lhs, const Vec3& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

Vec3 Math::operator+(const Vec3& lhs, const float scalar)
{
	return {
		(lhs.x + scalar),
		(lhs.y + scalar),
		(lhs.z + scalar)
	};
}
Vec3 Math::operator-(const Vec3& lhs, const float scalar)
{
	return {
		(lhs.x - scalar),
		(lhs.y - scalar),
		(lhs.z - scalar)
	};
}
Vec3 Math::operator*(const Vec3& lhs, const float scalar)
{
	return {
		(lhs.x * scalar),
		(lhs.y * scalar),
		(lhs.z * scalar)
	};
}
Vec3 Math::operator/(const Vec3& lhs, const float scalar)
{
	// scalar might be 0.0f which would crash
	// so we need to intercept here
	assert(scalar != 0.0f);
	return {
		(lhs.x / scalar),
		(lhs.y / scalar),
		(lhs.z / scalar)
	};
}
Vec3 Math::operator+(const Vec3& lhs, const Vec3& rhs)
{
	return {
		(lhs.x + rhs.x),
		(lhs.y + rhs.y),
		(lhs.z + rhs.z)
	};
}
Vec3 Math::operator-(const Vec3& lhs, const Vec3& rhs)
{
	return {
		(lhs.x - rhs.x),
		(lhs.y - rhs.y),
		(lhs.z - rhs.z)
	};
}

float Math::Distance(const Vec3& lhs, const Vec3& rhs)
{
	return Length(lhs - rhs);
}
float Math::Dot(const Vec3 & lhs, const Vec3 & rhs)
{
	return {
		(lhs.x * rhs.x) +
		(lhs.y * rhs.y) +
		(lhs.z * rhs.z)
	};
}
float Math::Length(const Vec3 & vector)
{
	return sqrt(SquareLength(vector));
}
float Math::Normalize(Vec3 & vector)
{
	float len = Length(vector);

	// length might be 0.0f which would crash
	// so we need to intercept here
	if (len > 0.0f)
	{
		float invLen = 1.0f / len;
		vector *= invLen;
	}

	return len;
}
float Math::SquareDistance(const Vec3& lhs, const Vec3& rhs)
{
	return SquareLength(lhs - rhs);
}
float Math::SquareLength(const Vec3& vector)
{
	return (
		(vector.x * vector.x) +
		(vector.y * vector.y) +
		(vector.z * vector.z)
	);
}
