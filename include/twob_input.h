#pragma once 
#include <map>

using namespace std;

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
		static map<int, CheckRelease> KeyPressed;
		static map<int, bool> KeyHold;
	};
}