#ifndef NOCOPYABLE_H_
#define NOCOPYABLE_H_
class CNocopyable
{
public:
	CNocopyable(){}
	virtual ~CNocopyable(){}
private:
	CNocopyable(const CNocopyable&);
	CNocopyable& operator=(const CNocopyable&);
};
#endif//NOCOPYABLE_H_