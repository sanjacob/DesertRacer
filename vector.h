/**
 * @file vector.h
 * 3D / 2D Vector structs with some operations (addition, substraction, etc) defined
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_VECTOR_H
#define DESERT_RACER_VECTOR_H

#include <cmath> // sqrt
#include <string> // Output vector as string (mainly for debugging)

#include <iostream>


namespace desert
{
	// SVector interface
	struct SVector
	{
		virtual float length() const = 0;
		virtual bool isZero() const = 0;
		virtual void zeroOut() = 0;
	};

	/**
	* 3D Vector struct
	* Holds the data for a tridimensional vector
	* Offers several operator overloads for convenience
	*/
	struct SVector3D : SVector
	{
		float x = 0;
		float y = 0;
		float z = 0;

		SVector3D() {};
		SVector3D(float xComp, float yComp, float zComp) : x(xComp), y(yComp), z(zComp) {};

		// Vector negation
		SVector3D operator-() const
		{
			return { -x, -y, -z };
		}

		// Vector addition
		SVector3D operator+(const SVector3D& other) const
		{
			return { x + other.x, y + other.y, z + other.z };
		}

		// Vector substraction
		SVector3D operator-(const SVector3D& other) const
		{
			return { x - other.x, y - other.y, z - other.z };
		}

		// Vector multiplication
		SVector3D operator*(const SVector3D& other) const
		{
			return { x * other.x, y * other.y, z * other.z };
		}

		// Scalar addition
		SVector3D operator+(const float scalar) const
		{
			return { x + scalar, y + scalar, z + scalar };
		}

		// Scalar subtraction
		SVector3D operator-(const float scalar) const
		{
			return { x - scalar, y - scalar, z - scalar };
		}

		// Scalar multiplication
		SVector3D operator*(const float scalar) const
		{
			return { x * scalar, y * scalar, z * scalar };
		}

		// Scalar division
		SVector3D operator/(const float scalar) const
		{
			return { x / scalar, y / scalar, z / scalar };
		}

		// Vector addition assignment
		void operator+=(const SVector3D& other)
		{
			x += other.x;
			y += other.y;
			z -= other.z;
		}

		// Vector subtraction assignment
		void operator-=(const SVector3D& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}

		// Vector multiplication assignment
		void operator*=(const SVector3D& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
		}

		// Scalar multiplication assignment
		void operator*=(const float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
		}

		// Return vector of each component squared
		SVector3D squared() const
		{
			return (*this) * (*this);
		}

		// Vector length
		float length() const
		{
			SVector3D s = squared();
			return sqrt(s.x + s.y + s.z);
		}

		// Returns unit vector
		SVector3D unit() const
		{
			return (*this) / length();
		}

		// Vector dot multiplication
		float dot(const SVector3D& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		// Vector cross multiplication
		SVector3D cross(const SVector3D& other) const
		{
			return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
		}

		// String representation of vector
		std::string asString() const
		{
			std::string output = "X:" + std::to_string(x) + " Y:" + std::to_string(y) + " Z:" + std::to_string(z);
			return output;
		}

		bool isZero() const
		{
			return x == 0 && y == 0 && z == 0;
		}

		// Reset all components to zero
		void zeroOut()
		{
			x = 0;
			y = 0;
			z = 0;
		}
	};

	/**
	* 2D Vector struct
	* Holds the data for a 2-dimensional vector
	* Offers several operator overloads for convenience
	*/
	struct SVector2D : SVector
	{
		float x = 0;
		float y = 0;

		SVector2D() {};
		SVector2D(float xComp, float yComp) : x(xComp), y(yComp) {};

		// Vector negation
		SVector2D operator-() const
		{
			return { -x, -y };
		}

		// Vector addition
		SVector2D operator+(const SVector2D& other) const
		{
			return { x + other.x, y + other.y };
		}

		// Vector subtraction
		SVector2D operator-(const SVector2D& other) const
		{
			return { x - other.x, y - other.y };
		}

		// Vector multiplication
		SVector2D operator*(const SVector2D& other) const
		{
			return { x * other.x, y * other.y };
		}

		// Scalar addition
		SVector2D operator+(const float scalar) const
		{
			return { x + scalar, y + scalar };
		}

		// Scalar subtraction
		SVector2D operator-(const float scalar) const
		{
			return { x - scalar, y - scalar };
		}

		// Scalar multiplication
		SVector2D operator*(const float scalar) const
		{
			return { x * scalar, y * scalar };
		}

		// Scalar division
		SVector2D operator/(const float scalar) const
		{
			return { x / scalar, y / scalar };
		}

		// Vector addition assignment
		void operator+=(const SVector2D& other)
		{
			x += other.x;
			y += other.y;
		}

		// Vector subtraction assignment
		void operator-=(const SVector2D& other)
		{
			x -= other.x;
			y -= other.y;
		}

		// Vector multiplication assignment
		void operator*=(const SVector2D& other)
		{
			x *= other.x;
			y *= other.y;
		}

		// Scalar multiplication assignment
		void operator*=(const float scalar)
		{
			x *= scalar;
			y *= scalar;
		}

		// Returns vector with all components squared
		SVector2D squared() const
		{
			return (*this) * (*this);
		}

		// Vector length
		float length() const
		{
			SVector2D s = squared();
			return sqrt(s.x + s.y);
		}

		// Unit vector
		SVector2D unit() const
		{
			return (*this) / length();
		}

		// Dot product
		float dot(const SVector2D& other) const
		{
			return x * other.x + y * other.y;
		}

		// String representation
		std::string asString() const
		{
			std::string output = "X:" + std::to_string(x) + " Y:" + std::to_string(y);
			return output;
		}

		bool isZero() const
		{
			return x == 0 && y == 0;
		}

		// Reset components to zero
		void zeroOut()
		{
			x = 0;
			y = 0;
		}
	};
}

#endif
