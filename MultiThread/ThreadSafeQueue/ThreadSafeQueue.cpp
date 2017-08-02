// ThreadSafeQueue.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <future>
#include <memory>
#include "BlockingQueue.h"
#include "BoundBlockingQueue.h"

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

int _tmain(int argc, _TCHAR* argv[])
{
	BlockingQueueTestStop();
	BlockingQueueTestPut();
	BoundBlockingQueue();
	system("pause");
	return 0;
}

