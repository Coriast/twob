#pragma once 
#include <map>

namespace twob
{

	struct CheckRelease
	{
		bool pressed = false;
		bool released = true;
	};

	class Input
	{
	public:
		static std::map<int, CheckRelease> KeyPressed;
		static std::map<int, bool> KeyHold;
	};
}