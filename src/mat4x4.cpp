// EXTERNAL INCLUDES
#include <cmath>
// INTERNAL INCLUDES
#include "math/mat4x4.h"

using namespace Math;

const Mat4x4 Mat4x4::identity = Mat4x4{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};
const Mat4x4 Mat4x4::zero = Mat4x4{
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};
Mat4x4::Mat4x4()
{

}
Mat4x4::Mat4x4(const Mat4x4 &mat)
{

}
Mat4x4::Mat4x4(const float *data)
{
}
Mat4x4::Mat4x4( float _11, float _12, float _13, float _14,
				float _21, float _22, float _23, float _24,
				float _31, float _32, float _33, float _34,
				float _41, float _42, float _43, float _44)
{
	this->_11 = _11;
	this->_12 = _12;
	this->_13 = _13;
	this->_14 = _14;

	this->_21 = _21;
	this->_22 = _22;
	this->_23 = _23;
	this->_24 = _24;

	this->_31 = _31;
	this->_32 = _32;
	this->_33 = _33;
	this->_34 = _34;

	this->_41 = _41;
	this->_42 = _42;
	this->_43 = _43;
	this->_44 = _44;
}

Mat4x4 Mat4x4::Transpose(void) const
{
	return Mat4x4 {
		_11, _21, _31, _41,
		_12, _22, _32, _42,
		_13, _23, _33, _43,
		_14, _24, _34, _44
	};
}

Mat4x4 Mat4x4::operator+ (const Mat4x4& other) const
{
	return Mat4x4 {
		this->_11 + other._11,
		this->_12 + other._12,
		this->_13 + other._13,
		this->_14 + other._14,

		this->_21 + other._21,
		this->_22 + other._22,
		this->_23 + other._23,
		this->_24 + other._24,

		this->_31 + other._31,
		this->_32 + other._32,
		this->_33 + other._33,
		this->_34 + other._34,

		this->_41 + other._41,
		this->_42 + other._42,
		this->_43 + other._43,
		this->_44 + other._44,
	};
}
Mat4x4 Mat4x4::operator- (const Mat4x4& other) const
{
	return Mat4x4{
		this->_11 - other._11,
		this->_12 - other._12,
		this->_13 - other._13,
		this->_14 - other._14,

		this->_21 - other._21,
		this->_22 - other._22,
		this->_23 - other._23,
		this->_24 - other._24,

		this->_31 - other._31,
		this->_32 - other._32,
		this->_33 - other._33,
		this->_34 - other._34,

		this->_41 - other._41,
		this->_42 - other._42,
		this->_43 - other._43,
		this->_44 - other._44,
	};
}

Mat4x4 Mat4x4::operator* (const Mat4x4& other) const
{
	return Mat4x4{
		(this->_11 * other._11 + this->_12 * other._21 + this->_13 * other._31 + this->_14 * other._41),
		(this->_11 * other._12 + this->_12 * other._22 + this->_13 * other._32 + this->_14 * other._42),
		(this->_11 * other._13 + this->_12 * other._23 + this->_13 * other._33 + this->_14 * other._43),
		(this->_11 * other._14 + this->_12 * other._24 + this->_13 * other._34 + this->_14 * other._44),

		(this->_21 * other._11 + this->_22 * other._21 + this->_23 * other._31 + this->_24 * other._41),
		(this->_21 * other._12 + this->_22 * other._22 + this->_23 * other._32 + this->_24 * other._42),
		(this->_21 * other._13 + this->_22 * other._23 + this->_23 * other._33 + this->_24 * other._43),
		(this->_21 * other._14 + this->_22 * other._24 + this->_23 * other._34 + this->_24 * other._44),

		(this->_31 * other._11 + this->_32 * other._21 + this->_33 * other._31 + this->_34 * other._41),
		(this->_31 * other._12 + this->_32 * other._22 + this->_33 * other._32 + this->_34 * other._42),
		(this->_31 * other._13 + this->_32 * other._23 + this->_33 * other._33 + this->_34 * other._43),
		(this->_31 * other._14 + this->_32 * other._24 + this->_33 * other._34 + this->_34 * other._44),

		(this->_41 * other._11 + this->_42 * other._21 + this->_43 * other._31 + this->_44 * other._41),
		(this->_41 * other._12 + this->_42 * other._22 + this->_43 * other._32 + this->_44 * other._42),
		(this->_41 * other._13 + this->_42 * other._23 + this->_43 * other._33 + this->_44 * other._43),
		(this->_41 * other._14 + this->_42 * other._24 + this->_43 * other._34 + this->_44 * other._44),
	};
}
void Mat4x4::operator*= (const Mat4x4& other) const
{

}

Mat4x4 Mat4x4::operator*(float scalar) const
{
	return Mat4x4{
		this->_11 * scalar, this->_12 * scalar, this->_13 * scalar, this->_14 * scalar,
		this->_21 * scalar, this->_22 * scalar, this->_23 * scalar, this->_24 * scalar,
		this->_31 * scalar, this->_32 * scalar, this->_33 * scalar, this->_34 * scalar,
		this->_41 * scalar, this->_42 * scalar, this->_43 * scalar, this->_44 * scalar
	};
}

void Mat4x4::SetTranslation(const Math::Vec3& trans)
{
	(*this) = Mat4x4::identity;
	this->_41 = trans.x;
	this->_42 = trans.y;
	this->_43 = trans.z;
}
void Mat4x4::SetScale(const Math::Vec3& scale)
{
	(*this) = Mat4x4::identity;
	this->_11 = scale.x;
	this->_22 = scale.y;
	this->_33 = scale.z;
}

void Mat4x4::RotateX(float angle)
{
	this->_22 *= cosf(angle);
	this->_23 *= -sin(angle);
	this->_33 *= sinf(angle);
	this->_34 *= cosf(angle);
}
void Mat4x4::RotateY(float angle)
{
	this->_11 *= cosf(angle);
	this->_13 *= sinf(angle);
	this->_31 *= -sinf(angle);
	this->_33 *= cosf(angle);
}
void Mat4x4::RotateZ(float angle)
{
	this->_11 *= cosf(angle);
	this->_12 *= -sinf(angle);
	this->_21 *= sinf(angle);
	this->_22 *= cosf(angle);
}
