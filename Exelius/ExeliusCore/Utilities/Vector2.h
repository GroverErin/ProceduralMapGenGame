#pragma once
#include <cmath>

namespace Exelius
{
	template <class NumberType>
	class Vector2
	{
		static_assert(std::is_integral_v<NumberType> || std::is_floating_point_v<NumberType>, "Type must be of integral number type.");

	public:
		NumberType x;
		NumberType y;

		//Multiplies vector by a one-dimensional scalar.
		template<class ScalarType>
		Vector2 operator*(ScalarType right)
		{
			return Vector2{ x * right, y * right };
		}

		//Calculates a vector passing from A to B.
		Vector2 operator-(Vector2 b)
		{
			return Vector2{ this->x - b.x, this->y - b.y };
		}

		Vector2<NumberType>& operator*=(const Vector2<NumberType>& right)
		{
			x *= right.x;
			y *= right.y;
			return *this;
		}

		//Calculates the resultant of A and B.
		Vector2 operator+(const Vector2& b)
		{
			return Vector2{ this->x + b.x, this->y + b.y };
		}

		Vector2 operator/(float right)
		{
			return Vector2{ this->x / right, this->y / right };
		}

		bool operator==(const Vector2& right)
		{
			return (this->x == right.x && this->y == right.y);
		}

		bool operator!=(const Vector2& right)
		{
			return !(this->x == right.x && this->y == right.y);
		}

		float Dot(const Vector2& b)
		{
			return this->x * b.x + this->y * b.y;
		}

		float Det(const Vector2& b)
		{
			return this->x * b.y - this->y * b.x;
		}

		//Returns the squared length of this vector.
		NumberType GetSquareLength()
		{
			return (x * x) + (y * y);
		}

		float Length()
		{
			return std::sqrt(GetSquareLength());
		}

		//Return a vector with the same component proportions but length of one.
		template <class _NumberType = NumberType>
		typename std::enable_if_t<std::is_floating_point_v<_NumberType>, Vector2<_NumberType>> Normalize()
		{
			if (x == 0 && y == 0)
				return { 0,0 };

			float length = std::sqrt(GetSquareLength());
			return { x / length, y / length };
		}
	};

	using Vector2f = Vector2<float>;
	using Vector2i = Vector2<int>;
	using Vector2u = Vector2<unsigned int>;
}