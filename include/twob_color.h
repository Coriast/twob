#pragma once

namespace 
{
	struct Color
	{
		Color() {};
		constexpr Color(int r, int g, int b) : r(r), g(g), b(b) {}
		int r, g, b;

		static const Color red;
		static const Color purple;
	};
	const Color Color::red = Color(255, 0, 0);
	const Color Color::purple = Color(255, 0, 255);
}