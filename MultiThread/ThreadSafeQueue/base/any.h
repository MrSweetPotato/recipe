#ifndef ANY_H_
#define ANY_H_
#include <typeindex>
#include <memory>

class CPlaceHolder;
typedef std::unique_ptr<CPlaceHolder> CPlaceHolderUniquePtr;

class CPlaceHolder
{
public:
	CPlaceHolder(){}
	virtual ~CPlaceHolder(){}
private:
	virtual std::type_index GetType(void) const = 0;
	virtual CPlaceHolderUniquePtr Clone(void) const = 0;
};


template<typename ValueType>
class CHolder : public CPlaceHolder
{
public:
	CHolder(ValueType pValue) :
		m_Context(pValue)
	{}

	virtual ~CHolder() = default;

private:
	virtual std::type_index GetType(void) const override
	{
		return typeid(m_Context);
	}

	virtual CPlaceHolderUniquePtr Clone(void) const override
	{
		return CPlaceHolderUniquePtr(new CHolder<T>(m_Context));
	}

	T	m_Context;
};

class Any
{
public:
	Any() :
		m_ContextPtr(nullptr)
		, m_ValueType(typeid(void))
	{}

	template<typename ValueType>
	Any(ValueType pValue) : 
		m_ContextPtr(new CHolder<ValueType>(pValue))
		, m_ValueType(typeid(ValueType))
	{}

	Any(const Any&);

	Any(Any&&);
	
	Any& operator=(const Any&);

	~Any();

private:
	CPlaceHolderUniquePtr m_ContextPtr;
	std::type_index		  m_ValueType;
};
#endif//ANY_H_