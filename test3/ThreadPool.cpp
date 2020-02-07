#include"ThreadPool.h"
#include<iostream>

//声明的全局变量
extern std::mutex mtx;
extern std::condition_variable cv;

extern bool turn_on;
extern int num;//share value by producer and consumer，相当于一个信号量,记录队列当中的任务的个数
extern SafeQueue<ReadTarget>  safequeue;


void ThreadPool::consumer()
{
	while (turn_on)//consumer一直在运转
	{
		int tem_num;
		ReadTarget rt;
		{//同步机制
			std::unique_lock<std::mutex> lck(mtx);
			while (num == 0)
			{
				cv.wait(lck);
				if (turn_on == false)
					break;
			}
			if (turn_on == false)
				break;
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
	std::cout << "线程" << std::this_thread::get_id() << "退出。" << std::endl;
}
void ThreadPool::producer()//到指定数字生产者线程就会自动停止
{
	int i = 1;
	while (turn_on)
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
	std::cout << "线程" << std::this_thread::get_id() << "退出。" << std::endl;
}
void ThreadPool::start()
{
	turn_on = true;//将所有线程运行权限设置为开
	std::thread consumers[6];//创建10个生产者和10个消费者
	for (int i = 0; i < 6; i++)
	{
		consumers[i] = std::thread(consumer);
		//consumers[i].join();
	}
	std::thread producers(producer);//直接创建了一个生产者线程
	std::thread offer(shutdown);//接受键盘输入，自动关闭线程
	producers.join();
	for (int i = 0; i < 6; i++)
	{
		consumers[i].join();
	}
	offer.join();
}

void ThreadPool::shutdown()//暂时先不实现
{
	int flag = 0;
	std::cin >> flag;
	if(flag)
	turn_on = false;
	std::cout << "flag is：" << flag << std::endl;
	cv.notify_all();//确保正在睡眠当中的consumer线程也能收到shutdown信号
	std::cout << "hello world!" << std::endl;
}