#ifndef FUNCTION_TRAITS_H_
#define FUNCTION_TRAITS_H_
#include <functional>
//common function_traits https://github.com/qicosmos/cosmos/blob/master/function_traits.hpp
template<class T>
struct function_traits{};

template<class Ret, class... Args>
struct function_traits<Ret(Args...)>
{
	enum{ arity = sizeof...(Args) };
	typedef Ret(function_type)(Args...);
	typedef Ret return_type;

	template<size_t INDEX>
	struct args
	{
		static_assert(INDEX < arity,"index is out of range, index must less than sizeof Args");
		using type = typename std::tuple_element<INDEX, std::tuple<Args...>>::type;
	};

	using tuple_type = std::tuple<std::remove_cv_t<std::remove_reference<Args>>...>;
};

//function pointer
template<typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)>:function_traits<Ret(Args...)>
{
	using function_type = Ret(*)(Args...);
};

//std::function
template<typename Ret, typename... Args>
struct function_traits<std::function<Ret(Args...)>>::function_traits<Ret(Args...)>
{
	using function_type = std::function<Ret(Args...)>;
};

//member function.
#define MEMBER_FUNCTION_TRAITS(...)\
template <typename Ret, typename CLASS, typename... Args>\
struct function_traits<Ret(CLASS::*)(Args...) __VA_ARGS__>::function_traits<Ret(Args...)>\
{\
	using function_type = ReturnType(ClassType::*)(Args...)__VA_ARGS__; \
	using this_type = ClassType; \
}; \


MEMBER_FUNCTION_TRAITS();
MEMBER_FUNCTION_TRAITS(const);
MEMBER_FUNCTION_TRAITS(volatile);
MEMBER_FUNCTION_TRAITS(const volatile);

//callable object
template<typename Callable>
struct function_traits :function_traits<decltype(&Callable::operator())>{};

#endif//FUNCTION_TRAITS_H_