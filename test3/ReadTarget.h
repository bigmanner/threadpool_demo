#pragma once
#include<iostream>
class ReadTarget//�趨Ϊ�ṹ�������
{
private:
	char instrument_id[31] = "";//��Լ����
	int dir = 0;//��������
	int vol = 0;//��������
public:
	ReadTarget()
	{

	}
	ReadTarget(char* para_id, int d, int v)
	{
		dir = d;
		vol = v;
		for (int i = 0; i < 31; i++)
		{
			instrument_id[i] = para_id[i];
		}
	}
	void output()
	{
		std::cout << "instrument id:" << instrument_id << " dir:" << dir << " vol:" << vol << std::endl;
	}
};