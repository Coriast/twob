#pragma once 
#include <functional>

namespace twob
{
	template <typename ReturnType, typename... Args> using Fn = std::function<ReturnType(Args...)>;
	typedef unsigned int u_int;
}