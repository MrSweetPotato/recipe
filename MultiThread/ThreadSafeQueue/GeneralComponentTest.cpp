// ThreadSafeQueue.cpp : �������̨Ӧ�ó������ڵ㡣
//

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "stdafx.h"
#include <iostream>
#include <future>
#include <memory>
#include <string>
#include "BlockingQueue.h"
#include "BoundBlockingQueue.h"
#include "WeakCallback.h"
#include "any.h"


//
//TEST_CASE("vectors can be sized and resized", "[vector]") {
//
//	std::vector<int> v(5);
//
//	REQUIRE(v.size() == 5);
//	REQUIRE(v.capacity() >= 5);
//
//	SECTION("resizing bigger changes size and capacity") {
//		v.resize(10);
//
//		REQUIRE(v.size() == 10);
//		REQUIRE(v.capacity() >= 10);
//	}
//	SECTION("resizing smaller changes size but not capacity") {
//		v.resize(0);
//
//		REQUIRE(v.size() == 0);
//		REQUIRE(v.capacity() >= 5);
//	}
//	SECTION("reserving bigger changes capacity but not size") {
//		v.reserve(10);
//
//		REQUIRE(v.size() == 5);
//		REQUIRE(v.capacity() >= 10);
//	}
//	SECTION("reserving smaller does not change size or capacity") {
//		v.reserve(0);
//
//		REQUIRE(v.size() == 5);
//		REQUIRE(v.capacity() >= 5);
//	}
//}


TEST_CASE("class any test")
{
	Any any1, any2, any3, any4, any5;
	int const a = 1;
	double const b = 3.1415;
	char const *p = "hello world";
	std::string const str1 = "how are you";
	std::string const *strptr = &str1;

	any1 = a;
	any2 = b;
	any3 = p;
	any4 = str1;
	any5 = strptr;

	SECTION("test any contain int")
	{
		int a1 = any_cast<int>(any1);
		REQUIRE(a1 == a);
	}

	SECTION("test any contain double")
	{
		double b1 = any_cast<double>(any2);
		REQUIRE(b1 == b);
	}

	SECTION("test any contain char*")
	{
		const char* p1 = any_cast<char const*>(any3);
		REQUIRE(std::string(p1) == std::string(p));
	}

	SECTION("test any contain string")
	{
		std::string str2 = any_cast<std::string>(any4);
		REQUIRE(str2 == str1);
	}

	SECTION("test any contain string pointer")
	{
		const std::string *strptr1 = any_cast<std::string const*>(any5);
		REQUIRE(*strptr == *strptr1);
	}
}

void BlockingQueueTestStop(void)
{
	std::shared_ptr<CBlockingQueue<int>> intQueuePtr(new CBlockingQueue<int>);
	std::async([intQueuePtr](){
		std::this_thread::sleep_for(std::chrono::seconds(5));
		intQueuePtr->Stop();
	});
	auto data = intQueuePtr->Take();
	if (data){
		std::cout << "take from queue:" << *data << std::endl;
	}
	std::cout << "BlockingQueue Stop!" << std::endl;
}

void BlockingQueueTestPut(void)
{
	CBlockingQueue<int> queue;
	queue.Put(1);
	queue.Put(2);
	queue.Put(3);
	std::cout << "BlockingQueue'size is:" << queue.Size() << std::endl;
}

void BoundBlockingQueue(void)
{
	CBoundBlockingQueue<int> BoundQueue(1);
	BoundQueue.Put(1);
	auto future = std::async([&BoundQueue](){return BoundQueue.Take(); });
	BoundQueue.Put(2);
	
	std::cout << "BoundBlockingQueue's size is:" << BoundQueue.Size() << std::endl
		<< "Data take from queue is:" << *(future.get()) << std::endl;
}

class CWeakCallbackTest
{
public:
	void Print(int a, int b)
	{
		std::cout << "arg1 : " << a
			<<",arg2 : "<<b<< std::endl;
	}
};

typedef void (CWeakCallbackTest::*pFunction)(int a, int b);

void WeakCallbackTest(void)
{
	std::cout << "begin weakcallback test" << std::endl;
	int a = 1, b = 2;
	std::shared_ptr<CWeakCallbackTest> object(new CWeakCallbackTest);

	CWeakCallback<CWeakCallbackTest, int, int>  weakcallbackobject = makeweakcallback(object, &CWeakCallbackTest::Print);
	weakcallbackobject(std::move(a), std::move(b));

	std::cout << "end of weakcallback test" << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//BlockingQueueTestStop();
	//BlockingQueueTestPut();
	//BoundBlockingQueue();

	//WeakCallbackTest();
	int result = Catch::Session().run(argc, argv);

	// global clean-up...

	system("pause");
	return (result < 0xff ? result : 0xff);
}

