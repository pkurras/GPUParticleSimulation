#pragma once

// EXTERNAL INCLUDES
// INTERNAL INCLUDES
#include "math/vec3.h"

namespace Math
{
	struct Float4x4
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
	};

	class Mat4x4 : Float4x4
	{
	public:

		/**
		 * @brief Construct a new Mat4x4 object
		 */
		Mat4x4();
		/**
		 * @brief Construct a new Mat4x4 object
		 */
		Mat4x4(const Mat4x4 &);
		/**
		 * @brief Construct a new Mat4x4 object
		 */
		Mat4x4(const float *data);
		/**
		 * @brief Construct a new Mat4x4 object
		 */
		Mat4x4(float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44);

		/**
		 * @brief	This method returns the transposed matrix
		 * @return	Mat4x4 is the transposed matrix
		 */
		Mat4x4 Transpose(void) const;

		/**
		 * @brief This method adds a matrix to this matrix
		 * @param other is the rhs matrix
		 * @return Mat4x4 a new matrix that is created by the operation.
		 */
		Mat4x4 Mat4x4::operator+ (const Mat4x4& other) const;
		/**
		 * @brief This method subtracts a matrix from this matrix
		 * @param other is the rhs matrix
		 * @return Mat4x4 a new matrix that is created by the operation.
		 */
		Mat4x4 Mat4x4::operator- (const Mat4x4& other) const;

		Mat4x4 operator* (const Mat4x4& other) const;
		void operator*= (const Mat4x4& other) const;

		Mat4x4 operator*(float scalar) const;

		/**
		 * @brief This method sets the translation of this matrix
		 * @param translation is the translation vector
		 */
		void SetTranslation(const Math::Vec3& translation);
		/**
		 * @brief This method sets the scale of this matrix
		 * @param scale is the scale vector
		 */
		void SetScale(const Math::Vec3& scale);

		/**
		 * @brief	This method rotates the matrix
		 * 			around an angle on the x-axis
		 * @param	angle is the rotation angle
		 */
		void RotateX(float angle);
		/**
		 * @brief	This method rotates the matrix
		 * 			around an angle on the y-axis
		 * @param	angle is the rotation angle
		 */
		void RotateY(float angle);
		/**
		 * @brief	This method rotates the matrix
		 * 			around an angle on the z-axis
		 * @param	angle is the rotation angle
		 */
		void RotateZ(float angle);

		static const Mat4x4 zero;		/**< Short hand for the zero matrix */
		static const Mat4x4 identity;	/**< Short hand for the identity matrix */

	};
}