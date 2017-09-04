#ifndef ANY_H_
#define ANY_H_
#include <typeindex>
#include <memory>



class Any
{
	class CPlaceHolder;

	template<typename T>
	class CHolder;

	template<typename T>
	friend T& any_cast(Any& val);

public:

	Any() :
		m_ContextPtr(nullptr)
		, m_ValueType(std::type_index(typeid(void)))
	{}

	template<typename T>
	Any(const T& pValue) :
		m_ContextPtr(new CHolder<T>(pValue))
		, m_ValueType(std::type_index(typeid(T)))
	{}

	explicit Any(const Any& right)
		:m_ContextPtr(right.m_ContextPtr ? right.m_ContextPtr->Clone() : nullptr)
		, m_ValueType(right.m_ValueType)
	{
	}

	Any(Any&& rright)
		:m_ContextPtr(rright.m_ContextPtr ? rright.m_ContextPtr->Clone() : nullptr)
		, m_ValueType(rright.m_ValueType)
	{
		rright.m_ContextPtr = nullptr;
		rright.m_ValueType = typeid(void);
	}

	~Any(){}


	template<typename T>
	Any& operator=(const T& val)
	{
		Any(val).swap(*this);
		return *this;
	}

	Any& operator=(const Any& val)
	{
		Any(val).swap(*this);
		return *this;
	}

private:
	Any swap(Any& val)
	{
		std::swap(val.m_ContextPtr, m_ContextPtr);
		std::swap(val.m_ValueType, m_ValueType);
		return *this;
	}

	typedef std::unique_ptr<CPlaceHolder> CPlaceHolderUniquePtr;

	class CPlaceHolder
	{
	public:
		CPlaceHolder(){}
		virtual ~CPlaceHolder(){}
		virtual CPlaceHolderUniquePtr Clone(void) const = 0;
	};

	//template<typename ValueType>
	//class CHolder : public CPlaceHolder {
	//public:
	//	CHolder(const ValueType& value)
	//		: m_Context(value) {}

	//	virtual CPlaceHolderUniquePtr Clone(void) const override
	//	{
	//		return CPlaceHolderUniquePtr(new CHolder<ValueType>(m_Context));
	//	}

	//	ValueType m_Context;
	//};

	template<typename T>
	class CHolder : public CPlaceHolder
	{
	public:

		CHolder(const T& pValue) :
			m_Context(pValue)
		{}

		virtual ~CHolder() = default;

		virtual CPlaceHolderUniquePtr Clone(void) const override
		{
			return CPlaceHolderUniquePtr(new CHolder<T>(m_Context));
		}

		T	m_Context;
	};

	CPlaceHolderUniquePtr m_ContextPtr;
	std::type_index		  m_ValueType;
};


template<typename T>
T& any_cast(Any& val)
{
	if (std::type_index(typeid(T)) != val.m_ValueType){
		throw std::bad_cast("error type");
	}
	return static_cast<Any::CHolder<T>*>(val.m_ContextPtr.get())->m_Context;
}
#endif//ANY_H_