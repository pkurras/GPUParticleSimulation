#pragma once

// EXTERNAL INCLUDES
// INTERNAL INCLUDES

namespace Math
{
	/**
	 * @brief	This struct defines an two-dimensional vector
	 */
	struct Vec2
	{
		float x;	/**< the x component of the vector */
		float y;	/**< the y component of the vector */

		static const Vec2 zero;			/**< Short hand for Vec3(0, 0) */
		static const Vec2 unit_x;		/**< Short hand for Vec3(1, 0) */
		static const Vec2 unit_y;		/**< Short hand for Vec3(0, 1) */
		static const Vec2 neg_unit_x;	/**< Short hand for Vec3(-1, 0) */
		static const Vec2 neg_unit_y;	/**< Short hand for Vec3(0, -1) */
		static const Vec2 unit_scale;	/**< Short hand for Vec3(1, 1) */
	};

	/**
	 * @brief	This operator provides scalar addition for \link Engine::Math::Vec2 \endlink
	 * @param	lhs is the vector that the operation is executed on
	 * @param	scalar is the scalar that is added to the vector
	 * @return	the reference to the modified version of lhs vector
	 */
	Vec2& operator+=(Vec2& lhs, const float scalar);
	/**
	 * @brief	This operator provides scalar subtraction for \link Engine::Math::Vec2 \endlink
	 * @param	lhs is the vector that the operation is executed on
	 * @param	scalar is the scalar that is subtracted from the vector
	 * @return	the reference to the modified version of lhs vector
	 */
	Vec2& operator-=(Vec2& lhs, const float scalar);
	/**
	 * @brief	This operator provides scalar multiplication for \link Engine::Math::Vec2 \endlink
	 * @param	lhs is the vector that the operation is executed on
	 * @param	scalar is the scalar that is multiplied onto the vector
	 * @return	the reference to the modified version of lhs vector
	 */
	Vec2& operator*=(Vec2& lhs, const float scalar);
	/**
	 * @brief	This operator provides scalar division for \link Engine::Math::Vec2 \endlink
	 * @param	lhs is the vector that the operation is executed on
	 * @param	scalar is the scalar that the vector is devided by
	 * @return	the reference to the modified version of lhs vector
	 */
	Vec2& operator/=(Vec2& lhs, const float scalar);
	/**
	 * @brief	This operator provides addition for \link Engine::Math::Vec2 \endlink
	 * @param	lhs is the vector that the operation is executed on 
	 * @param	rhs is the vector that is added to the first vector
	 * @return	the reference to the modified version of lhs vector
	 */
	Vec2& operator+=(Vec2& lhs, const Vec2& rhs);
	/**
	 * @brief	This operator provides subtraction for \link Engine::Math::Vec2 \endlink
	 * @param	lhs is the vector that the operation is executed on
	 * @param	rhs is the vector that is subtracted from the first vector
	 * @return	the reference to the modified version of lhs vector
	 */
	Vec2& operator-=(Vec2& lhs, const Vec2& rhs);

	/**
	 * @brief	This operator provides scalar addition for \link Engine::Math::Vec2 \endlink
	 * @param	lhs is the vector that the operation is executed on
	 * @param	scalar is the value that is added to the lhs vector
	 * @return	is a new vector that is created by the operation
	 */
	Vec2 operator+(const Vec2& lhs, const float scalar);
	/**
	 * @brief	This operator provides scalar subtraction for \link Engine::Math::Vec2 \endlink
	 * @param	lhs is the vector that the operation is executed on
	 * @param	scalar is the value that is subtracted from the lhs vector
	 * @return	is a new vector that is created by the operation
	 */
	Vec2 operator-(const Vec2& lhs, const float scalar);
	/**
	 * @brief	This operator provides scalar multiplication for \link Engine::Math::Vec2 \endlink
	 * @param	lhs is the vector that the operation is executed on
	 * @param	scalar is the value that the lhs vector is multiplied by
	 * @return	is a new vector that is created by the operation
	 */
	Vec2 operator*(const Vec2& lhs, const float scalar);
	/**
	 * @brief	This operator provides scalar division for \link Engine::Math::Vec2 \endlink
	 * @param	lhs is the vector that the operation is executed on
	 * @param	scalar is the value that the lhs vector is divided by
	 * @return	is a new vector that is created by the operation
	 */
	Vec2 operator/(const Vec2& lhs, const float scalar);
	/**
	 * @brief	This operator provides addition for \link Engine::Math::Vec2 \endlink
	 * @param	lhs is the lhs vector that the operation is executed on
	 * @param	rhs is the rhs vector that is added to the lhs vector
	 * @return	is a new vector that is created by the operation
	 */
	Vec2 operator+(const Vec2& lhs, const Vec2& rhs);
	/**
	 * @brief	This operator provides subtraction for \link Engine::Math::Vec2 \endlink
	 * @param	lhs is the lhs vector that the operation is executed on
	 * @param	rhs is the rhs vector that is subtracted from the lhs vector
	 * @return	is a new vector that is created by the operation
	 */
	Vec2 operator-(const Vec2& lhs, const Vec2& rhs);

	/**
	 * @brief	This method calculates the distance between two \link Math::Vec2 \endlink.
	 * @param	lhs is a reference to a lhs vector
	 * @param	rhs is a reference to a rhs vector
	 * @return	the distance between the two vectors as a scalar number.
	 */
	float Distance(const Vec2& lhs, const Vec2& rhs);
	/**
	 * @brief	This method calculates the dot product between two \link Math::Vec2 \endlink
	 * @param	lhs is a reference to a lhs vector
	 * @param	rhs is a reference to a rhs vector
	 * @return	the projection of \p other onto the lhs vector.\n
	 *			That is the strength of the rhs vector on the lhs vector direction.
	 */
	float Dot(const Vec2& lhs, const Vec2& rhs);
	/**
	 * @brief	This method calculates the length of the vector in reference to the world's origin.
	 * @param	vector is a reference to the base vector.
	 * @return	the length of the vector as a scalar number.
	 */
	float Length(const Vec2& vector);
	/**
	 * @brief	This method normalizes the vector. That is to say it's length will be 1.0f.
	 * @param	vector is a reference to the vector that is to be normalized.
	 * @return	the previous length of the vector.
	 */
	float Normalize(Vec2& vector);
	/**
	 * @brief	This method calculates the distance between two vectors.\n
				It does not calculate the square root thus it is more performant than \link Engine::Math::Distance \endlink\n
				This method can be used to compare two distances relative to each other without taking the actual value\n
				into account. This can be helpful if you want to know which one is closer or further away than the other.
	 * @param	vector is a reference to the base vector.
	 * @return	the distance between the two vectors as a scalar number.
	 */
	float SquareDistance(const Vec2& lhs, const Vec2& rhs);
	/**
	 * @brief	This method calculates the length of the vector in reference to the world's origin.\n
				It does not calculate the square root thus is more performant than \link Engine::Math::Length \endlink\n
				This method can be used to compare two distances relative to each other without taking the actual value\n
				into account. This can be helpful if you want to know which one is closer or further away than the other.
	 * @param	vector is a reference to the base vector.
	 * @return	the length of the vector as a scalar number.
	 */
	float SquareLength(const Vec2& vector);
}