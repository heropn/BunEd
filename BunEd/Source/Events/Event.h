#pragma once
#include <vector>
#include <functional>

#define CALLBACK_0(fun, obj) std::bind(fun, obj)
#define CALLBACK_1(fun, obj) std::bind(fun, obj, std::placeholders::_1)
#define CALLBACK_2(fun, obj) std::bind(fun, obj, std::placeholders::_1, std::placeholders::_2)
#define CALLBACK_3(fun, obj) std::bind(fun, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define CALLBACK_4(fun, obj) std::bind(fun, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

template<class... Args>
class EventDispatcher
{
public:

	void Dispatch(Args... args)
	{
		for (const auto& f : functions)
		{
			f(args...);
		}
	}

	void Bind(std::function<void(Args...)> fun)
	{
		functions.push_back(fun);
	}

private:

	std::vector<std::function<void(Args...)>> functions;
};