#include"ThreadPool.h"
#include<iostream>

//������ȫ�ֱ���
extern std::mutex mtx;
extern std::condition_variable cv;

extern bool turn_on;
extern int num;//share value by producer and consumer���൱��һ���ź���,��¼���е��е�����ĸ���
extern SafeQueue<ReadTarget>  safequeue;


void ThreadPool::consumer()
{
	while (turn_on)//consumerһֱ����ת
	{
		int tem_num;
		ReadTarget rt;
		{//ͬ������
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
		//ͬ�����ƽ��������̵߳������У������߼�
		std::cout << "�߳�id��" << std::this_thread::get_id() << " ";//check
		std::cout << "�����numΪ��" << tem_num << " ";//check
		rt.output();
		//�߼����н�����׼��������һ������
	}
	std::cout << "�߳�" << std::this_thread::get_id() << "�˳���" << std::endl;
}
void ThreadPool::producer()//��ָ�������������߳̾ͻ��Զ�ֹͣ
{
	int i = 1;
	while (turn_on)
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
	std::cout << "�߳�" << std::this_thread::get_id() << "�˳���" << std::endl;
}
void ThreadPool::start()
{
	turn_on = true;//�������߳�����Ȩ������Ϊ��
	std::thread consumers[6];//����10�������ߺ�10��������
	for (int i = 0; i < 6; i++)
	{
		consumers[i] = std::thread(consumer);
		//consumers[i].join();
	}
	std::thread producers(producer);//ֱ�Ӵ�����һ���������߳�
	std::thread offer(shutdown);//���ܼ������룬�Զ��ر��߳�
	producers.join();
	for (int i = 0; i < 6; i++)
	{
		consumers[i].join();
	}
	offer.join();
}

void ThreadPool::shutdown()//��ʱ�Ȳ�ʵ��
{
	int flag = 0;
	std::cin >> flag;
	if(flag)
	turn_on = false;
	std::cout << "flag is��" << flag << std::endl;
	cv.notify_all();//ȷ������˯�ߵ��е�consumer�߳�Ҳ���յ�shutdown�ź�
	std::cout << "hello world!" << std::endl;
}