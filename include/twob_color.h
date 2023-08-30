#pragma once

namespace twob
{
	struct Color
	{
		int r, g, b;
		constexpr Color() : r(0), g(0), b(0) {}
		constexpr Color(int r, int g, int b) : r(r), g(g), b(b) {}

		static constexpr Color red()	{ return Color(255, 0, 0); }
		static constexpr Color purple() { return Color(255, 0, 255); }
	};
	
}