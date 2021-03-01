#pragma once
#include "Utilities/Vector2.h"

namespace Exelius
{
	// A structure representing a rectangle.
	struct Rectangle
	{
		Rectangle(int xPos = 0, int yPos = 0, int width = 0, int height = 0)
			: x(xPos)
			, y(yPos)
			, w(width)
			, h(height)
		{
			//
		}

		Vector2i GetCenter() const
		{
			return Vector2i({ x + w / 2, y + h / 2 });
		}

		int GetArea()
		{
			return (w * h);
		}

		int x;
		int y;
		int w;
		int h;
	};

	// A point struct with dimentions as float.
	struct FPoint
	{
		FPoint(float nx = 0.0f, float ny = 0.0f)
			: x{ nx }
			, y{ ny }
		{
			//
		}

		float x;
		float y;
	};
}