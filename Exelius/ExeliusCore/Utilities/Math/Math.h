#pragma once
#include "Utilities/Vector2.h"
#include <cmath>

namespace Exelius
{
	static constexpr float PI = 3.1415927f;

	static constexpr inline float Lerp(float min, float max, float weight) {
		return (1.0f - weight) * min + weight * max;
	}

	static constexpr inline float SmootherStep(float x)
	{
		float newX = x * x * x * (x * (x * 6 - 15) + 10);
		if (newX >= 1.0f)
			return 1.0f;
		else if (newX <= 0.0f)
			return 0.0f;
		return newX;
	}

	static constexpr inline float Normalize(float x, float min, float max)
	{
		return (x - min) / (max - min);
	}

	static constexpr inline float SquareDistance(const Vector2f& pos1, const Vector2f& pos2)
	{
		return (pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y);
	}

	static inline float Distance(const Vector2f& pos1, const Vector2f& pos2)
	{
		return std::sqrtf(SquareDistance(pos1, pos2));
	}
}