#pragma once
#include<mutex>
#include<condition_variable>
#include"mqueue.h"
#include"ReadTarget.h"
class ThreadPool
{
private:
	int max_thread_num = 0;
public:
	ThreadPool(int n) :max_thread_num(n) {}//¹¹Ôìº¯Êý
	static void consumer();
	static void producer();
	static void start();
	static void shutdown();
};