#include"ThreadPool.h"
#include<iostream>

//������ȫ�ֱ���
extern std::mutex mtx;
extern std::condition_variable cv;

extern int num;//share value by producer and consumer���൱��һ���ź���,��¼���е��е�����ĸ���
extern SafeQueue<ReadTarget>  safequeue;


void ThreadPool::consumer()
{
	while (true)//consumerһֱ����ת
	{
		int tem_num;
		ReadTarget rt;
		{//ͬ������
			std::unique_lock<std::mutex> lck(mtx);
			while (num == 0)
			{
				cv.wait(lck);
			}
			tem_num = num;
			safequeue.dequeue(rt);
			num--;
		}
		//ͬ�����ƽ��������̵߳������У������߼�
		std::cout << "�߳�id��" << std::this_thread::get_id() << " ";//check
		std::cout << "�����numΪ��" << tem_num << " ";//check
		rt.output();
		//�߼����н�����׼��������һ������
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
			std::unique_lock <std::mutex> lck(mtx);//������������ʹ������,num��safequeue,cv��֪���ǲ���
			num++;
			safequeue.enqueue(rt);
			cv.notify_one();//����һ���߳�
		}
		i++;
	}
}
void ThreadPool::start()
{
	std::thread consumers[6];//����10�������ߺ�10��������
	for (int i = 0; i < 6; i++)
	{
		consumers[i] = std::thread(consumer);
		//consumers[i].join();
	}
	std::thread producers(producer);//ֱ�Ӵ�����һ���������߳�
	producers.join();
	for (int i = 0; i < 6; i++)
	{
		consumers[i].join();
	}
}

void ThreadPool::shutdown()//��ʱ�Ȳ�ʵ��
{
	
}