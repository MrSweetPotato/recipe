// ThreadSafeQueue.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <future>
#include <memory>
#include "BlockingQueue.h"

void BlockingQueueTest(void)
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
int _tmain(int argc, _TCHAR* argv[])
{
	BlockingQueueTest();
	system("pause");
	return 0;
}

