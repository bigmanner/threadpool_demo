#pragma once
#include<iostream>
class ReadTarget//设定为结构体的类型
{
private:
	char instrument_id[31] = "";//合约名称
	int dir = 0;//买卖方向
	int vol = 0;//买卖数量
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