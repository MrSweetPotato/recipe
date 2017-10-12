// GeneralComponent.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

void Task(int num)
{
	std::stringstream oss;
	oss << "ThreadId:" << std::this_thread::get_id() << ",Get number:" << num << std::endl;
	std::string info(oss.str());
	printf(info.c_str());
}

class WeakCallbackTestClass
{
public:
	WeakCallbackTestClass() = default;
	virtual ~WeakCallbackTestClass() = default;

	void Print(void)
	{
		printf("this is weakcallback function");
	}
};
TEST_CASE("class any test")
{
	Any any1;
	int iA = 1;
	double dA = 1.20;
	std::string strA = "string";
	const std::string cstr = "const string";

	SECTION("int any")
	{
		any1 = iA;
		int a = any_cast<int>(any1);
		REQUIRE(a == iA);
	}

	SECTION("double any")
	{
		any1 = dA;
		double b = any_cast<double>(any1);
		REQUIRE(b == dA);
	}

	SECTION("string any")
	{
		any1 = strA;
		std::string str = any_cast<std::string>(any1);
		REQUIRE(str == strA);
	}

	SECTION("const string any")
	{
		any1 = cstr;
		std::string str = any_cast<std::string>(any1);
		REQUIRE(str == cstr);
	}
}

TEST_CASE("blocking queue test")
{
	std::cout << "=======blocking queue test case=======" << std::endl;
	CBlockingQueue<int> bqueue;
	bqueue.Put(1);
	bqueue.Put(2);
	bqueue.Put(3);
	bqueue.Put(4);
	auto f = [&bqueue](){
		while (!bqueue.IsStop()){
			int tData = 0;
			bqueue.Take(tData);
			std::cout << "take data:" << tData << " blocking queue's size:" << bqueue.Size() << std::endl;
		}
		REQUIRE(bqueue.IsStop());
	};

	SECTION("actively stop blocking queue")
	{
		std::async(f);
		std::this_thread::sleep_for(std::chrono::seconds(5));
		bqueue.Stop();
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}

TEST_CASE("BoundBlockingQueue test")
{
	std::cout << "=======BoundBlockingQueue test case=======" << std::endl;
	CBoundBlockingQueue<int> boundQueue(6);

	auto f = [&boundQueue](){
		for (int i = 5; i < 20; i++){
			boundQueue.Put(i);
		}
	};
	
	auto takef = [&boundQueue](){
		while (!boundQueue.IsStop()){
			int iData = 0;
			boundQueue.Take(iData);
			std::cout << "take data:"<< iData << " bondqueue's size:" << boundQueue.Size() << std::endl;
		}
		std::cout << "bound queue had stop!" << std::endl;
	};

	SECTION("actively stop bound blocking queue")
	{
		std::async(f);
		std::this_thread::sleep_for(std::chrono::seconds(5));
		std::async(takef);
		std::this_thread::sleep_for(std::chrono::seconds(10));
		boundQueue.Stop();
		std::this_thread::sleep_for(std::chrono::seconds(5));
		REQUIRE(boundQueue.IsStop());
	}
}

TEST_CASE("CountDownLatch test")
{
	std::cout << "=======CountDownLatch test case=======" << std::endl;
	CCoundownLatch countdown(2);
	auto f = [&countdown](){
		countdown.CountDown();
		std::cout << "Get count:" << countdown.GetCount() << std::endl;
	};

	SECTION("CountDownLatch test countdown")
	{
		std::async(f);
		std::this_thread::sleep_for(std::chrono::seconds(5));
		std::async(f);
		countdown.Wait();
		std::cout << std::endl;
		std::cout << "countdown wait over" << std::endl;
	}
}

TEST_CASE("Thread pool test")
{
	std::cout << "=======Thread pool test case=======" << std::endl;
	CThreadPool threadpool(4);
	threadpool.Start();

	SECTION("Thread pool test")
	{
		for (int num = 0; num < 20; num++){
			threadpool.Run(std::bind(&Task, num));
		}
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}

TEST_CASE("WeakCallback test case")
{
	SECTION("WeakCallback test case")
	{
		std::cout << "=======WeakCallback test case=======" << std::endl;
		std::shared_ptr<WeakCallbackTestClass> testobj(new WeakCallbackTestClass());
		CWeakCallback<WeakCallbackTestClass> object = makeweakcallback(testobj, &WeakCallbackTestClass::Print);
		object();
		std::cout << "\nafter reset shared_ptr call object();" << std::endl;
		testobj.reset();
		object();
	}
}

int _tmain(int argc, char* argv[])
{
	int result = Catch::Session().run(argc, argv);
	system("pause");
	return (result < 0xff ? result : 0xff);
}

