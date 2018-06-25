// EXTERNAL INCLUDES
#include <cmath>
#include <assert.h>
// INTERNAL INCLUDES
#include "math/vec2.h"

using namespace Math;

const Vec2 Vec2::zero = { 0.0f, 0.0f };
const Vec2 Vec2::unit_x = { 1.0f, 0.0f };
const Vec2 Vec2::unit_y = { 0.0f, 1.0f };
const Vec2 Vec2::neg_unit_x = { -1.0f, 0.0f };
const Vec2 Vec2::neg_unit_y = { 0.0f, -1.0f };
const Vec2 Vec2::unit_scale = { 1.0f, 1.0f };

Vec2& Math::operator+=(Vec2& lhs, const float scalar)
{
	lhs.x += scalar;
	lhs.y += scalar;
	return lhs;
}
Vec2& Math::operator-=(Vec2& lhs, const float scalar)
{
	lhs.x -= scalar;
	lhs.y -= scalar;
	return lhs;
}
Vec2& Math::operator*=(Vec2& lhs, const float scalar)
{
	lhs.x *= scalar;
	lhs.y *= scalar;
	return lhs;
}
Vec2& Math::operator/=(Vec2& lhs, const float scalar)
{
	// scalar might be 0.0f which would crash
	// so we need to intercept here
	assert(scalar != 0.0f);
	lhs.x /= scalar;
	lhs.y /= scalar;
	return lhs;
}
Vec2& Math::operator+=(Vec2& lhs, const Vec2& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}
Vec2& Math::operator-=(Vec2& lhs, const Vec2& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

Vec2 Math::operator+(const Vec2& lhs, const float scalar)
{
	return {
		(lhs.x + scalar),
		(lhs.y + scalar)
	};
}
Vec2 Math::operator-(const Vec2& lhs, const float scalar)
{
	return {
		(lhs.x - scalar),
		(lhs.y - scalar)
	};
}
Vec2 Math::operator*(const Vec2& lhs, const float scalar)
{
	return {
		(lhs.x * scalar),
		(lhs.y * scalar)
	};
}
Vec2 Math::operator/(const Vec2& lhs, const float scalar)
{
	// scalar might be 0.0f which would crash
	// so we need to intercept here
	assert(scalar != 0.0f);
	return {
		(lhs.x / scalar),
		(lhs.y / scalar)
	};
}
Vec2 Math::operator+(const Vec2& lhs, const Vec2& rhs)
{
	return {
		(lhs.x + rhs.x),
		(lhs.y + rhs.y)
	};
}
Vec2 Math::operator-(const Vec2& lhs, const Vec2& rhs)
{
	return {
		(lhs.x - rhs.x),
		(lhs.y - rhs.y)
	};
}

float Math::Distance(const Vec2& lhs, const Vec2& rhs)
{
	return Length(lhs - rhs);
}
float Math::Dot(const Vec2& lhs, const Vec2& rhs)
{
	return {
		(lhs.x * rhs.x) +
		(lhs.y * rhs.y)
	};
}
float Math::Length(const Vec2& vector)
{
	return sqrt(SquareLength(vector));
}
float Math::Normalize(Vec2& vector)
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
float Math::SquareDistance(const Vec2& lhs, const Vec2& rhs)
{
	return SquareLength(lhs - rhs);
}
float Math::SquareLength(const Vec2& vector)
{
	return (
		(vector.x * vector.x) +
		(vector.y * vector.y)
	);
}
