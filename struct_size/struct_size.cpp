// struct_size.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>

void thread_proc(int timeout)
{
	std::cout << "Start thread:" << std::this_thread::get_id();
	std::cout << " for " << timeout << " seconds" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(timeout));
	std::cout << "Stop thread:" << std::this_thread::get_id() << std::endl;
}

void control_thread()
{
	std::cout << "Control thread started\n";
	std::random_device rnd_dev;
	std::mt19937 engine{ rnd_dev() };
	std::uniform_int_distribution<> dist(1, 9);

	std::vector<std::thread> thread_list;
	for (int i = 0; i < 5; i++)
	{
		thread_list.push_back(std::thread(thread_proc, dist(engine)));
	}
	std::cout << "All thread created, waiting for them\n";

	for (std::thread& a : thread_list)
	{
		a.join();
	}
	std::cout << "All threads ended\n";
}

int main()
{
	std::thread t(control_thread);
	t.detach();
	
	std::cout << "end of main\n";
	char c;
	std::cin >> c;
}
