#include<thread>
#include<mutex>
#include<condition_variable>
#include"mqueue.h"//将队列添加进来
#include"ReadTarget.h"//将任务的最小单位添加进来
#include"ThreadPool.h"
std::mutex mtx;
std::condition_variable cv;

int num = 0;//share value by producer and consumer，相当于一个信号量,记录队列当中的任务的个数
SafeQueue<ReadTarget>  safequeue;

//消费者线程，什么叫做消费者线程                                                                                                      
//void consumer()
//{
//	while (true)//consumer一直在运转
//	{
//		int tem_num;
//		ReadTarget rt;
//		{//同步机制
//			std::unique_lock<std::mutex> lck(mtx);
//			while (num == 0)
//			{
//				cv.wait(lck);
//			}
//			tem_num = num;
//			safequeue.dequeue(rt);
//			num--;
//		}
//		//同步机制结束，子线程单独运行，处理逻辑
//		std::cout << "线程id：" << std::this_thread::get_id() << " ";//check
//		std::cout << "输出的num为：" << tem_num << " ";//check
//		rt.output();
//		//逻辑运行结束，准备处理下一个任务
//	}
//}
//
////生产者线程
//void producer()
//{
//	int i = 1;
//	while (i < 50)
//	{
//		char name[31] = "your_name";
//		ReadTarget rt(name, 1, i);
//		{
//			std::unique_lock <std::mutex> lck(mtx);//用这个锁区访问共享变量,num和safequeue,cv不知道是不是
//			num++;
//			safequeue.enqueue(rt);
//			cv.notify_one();//唤醒一个线程
//		}
//		i++;
//	}
//}

int main()
{
	//std::thread consumers[6];//创建10个生产者和10个消费者
	//for (int i = 0; i < 6; i++)
	//{
	//	consumers[i] = std::thread(consumer);
	//	//consumers[i].join();
	//}
	//std::thread producers(producer);//直接创建了一个生产者线程

	//producers.join();
	//for (int i = 0; i < 6; i++)
	//{
	//	consumers[i].join();
	//}
	////spawn 10 consumers and 10 producers

	ThreadPool tp(8);
	tp.start();
	tp.shutdown();
	return 0;
}