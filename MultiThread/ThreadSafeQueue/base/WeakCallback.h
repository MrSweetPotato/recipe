#ifndef WEAKCALLBACK_H_
#define WEAKCALLBACK_H_
#include <memory>
#include <functional>

template<typename CLASS, typename... ARGS>
class CWeakCallback
{
	typedef void (CLASS::*MemberFun)(ARGS...);
public:
	CWeakCallback(const std::weak_ptr<CLASS> &pObject,
		const MemberFun pFunction) :
		m_Object(pObject)
		, m_Function(pFunction)
	{
	}

	void operator()(ARGS&&... args)
	{
		std::shared_ptr<CLASS> ptr(m_Object.lock());
		if (ptr){
			(ptr.get()->*m_Function)(std::forward<ARGS>(args)...);
		}
	}

private:
	std::weak_ptr<CLASS> m_Object;
	MemberFun			 m_Function;
};

template<typename CLASS, typename... ARGS>
CWeakCallback<CLASS, ARGS...> makeweakcallback(const std::shared_ptr<CLASS> &pObject, 
	void (CLASS::*function)(ARGS...))
{
	return CWeakCallback<CLASS, ARGS...>(pObject, function);
}

template<typename CLASS, typename... ARGS>
CWeakCallback<CLASS, ARGS...> makeWeakCallback(const std::shared_ptr<CLASS>& object,
	void (CLASS::*function)(ARGS...) const)
{
	return CWeakCallback<CLASS, ARGS...>(object, function);
}
#endif//WEAKCALLBACK_H_