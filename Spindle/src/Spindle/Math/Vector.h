#pragma once

#include "../Core.h"
#include "baseMaths.h"


#include <iostream>

// cartesian system

//// STATIC MEMBER VARIABLES TODO ADD orthogonal unit vectors
//// Vector2f
//const Vector2f Vector2f::i{ 1.0f, 0.0f };
//
//struct Vector2f {
//	// orthogonal unit vectors
//	static const Vector2f i; // (1, 0) x axis
//};

namespace Spindle {
	template <typename T>
	struct Vector2 {
		T x, y;

		Vector2(T p_x = T(), T p_y = T())
			:x(p_x), y(p_y) {}

		// magnitude
		T magnitude() const {
			return MAGN(x, y);
		}

		// addition
		Vector2 operator+(const Vector2& operand) const {
			return Vector2(x + operand.x, y + operand.y);
		}

		// subtraction
		Vector2 operator-(const Vector2& operand) const {
			return Vector2(x - operand.x, y - operand.y);
		}

		// scalar multiplication
		Vector2 operator*(float scalar) const {
			return Vector2(x * scalar, y * scalar);
		}

		// dot product
		float dot(const Vector2& operand) const {
			return x * operand.x + y * operand.y;
		}

		void print() const {
			std::cout << "Vector2(" << x << ", " << y << ") | Magnitude: " << magnitude() << std::endl;
		}
	};

	template <typename T>
	struct Vector3 {
		T x, y, z;

		Vector3(T p_x = T(), T p_y = T(), T p_z = T())
			:x(p_x), y(p_y), z(p_z) {}

		// magnitude
		T magnitude() const {
			return MAGN(x, y, z);
		}

		// addition
		Vector3 operator+(const Vector3& operand) const {
			return Vector3(x + operand.x, y + operand.y, z + operand.z);
		}

		// subtraction
		Vector3 operator-(const Vector3& operand) const {
			return Vector3(x - operand.x, y - operand.y, z - operand.z);
		}


		void print() const {
			std::cout << "Vector3(" << x << ", " << y << ", " << z << ") | Magnitude: " << magnitude() << std::endl;
		}
	};

}

