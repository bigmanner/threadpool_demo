#include"ThreadPool.h"
#include<iostream>

//声明的全局变量
extern std::mutex mtx;
extern std::condition_variable cv;

extern int num;//share value by producer and consumer，相当于一个信号量,记录队列当中的任务的个数
extern SafeQueue<ReadTarget>  safequeue;


void ThreadPool::consumer()
{
	while (true)//consumer一直在运转
	{
		int tem_num;
		ReadTarget rt;
		{//同步机制
			std::unique_lock<std::mutex> lck(mtx);
			while (num == 0)
			{
				cv.wait(lck);
			}
			tem_num = num;
			safequeue.dequeue(rt);
			num--;
		}
		//同步机制结束，子线程单独运行，处理逻辑
		std::cout << "线程id：" << std::this_thread::get_id() << " ";//check
		std::cout << "输出的num为：" << tem_num << " ";//check
		rt.output();
		//逻辑运行结束，准备处理下一个任务
	}
}
void ThreadPool::producer()
{
	int i = 1;
	while (i < 7)
	{
		char name[31] = "your_name";
		ReadTarget rt(name, 1, i);
		{
			std::unique_lock <std::mutex> lck(mtx);//用这个锁区访问共享变量,num和safequeue,cv不知道是不是
			num++;
			safequeue.enqueue(rt);
			cv.notify_one();//唤醒一个线程
		}
		i++;
	}
}
void ThreadPool::start()
{
	std::thread consumers[6];//创建10个生产者和10个消费者
	for (int i = 0; i < 6; i++)
	{
		consumers[i] = std::thread(consumer);
		//consumers[i].join();
	}
	std::thread producers(producer);//直接创建了一个生产者线程
	producers.join();
	for (int i = 0; i < 6; i++)
	{
		consumers[i].join();
	}
}

void ThreadPool::shutdown()//暂时先不实现
{
	
}