// thread_sync.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <random>
#include <sstream>
#include "../../ssdp_linux/thread_sync_2/sync_console_out.h"
using SCO = RB::sync_console_out;

std::thread::id thread_proc(int timeout)
{
	SCO::write_line("Start of worker thread: ", std::this_thread::get_id(), "(", timeout, ")");
	if (timeout == 5)
	{
		std::stringstream ss;
		ss << std::this_thread::get_id();
		throw std::runtime_error(std::string("Thread exception - ") + ss.str());
	}
	std::this_thread::sleep_for(std::chrono::seconds(timeout));

	SCO::write_line("End of worker thread ", std::this_thread::get_id());
	return std::this_thread::get_id();
}

int control_thread(int num_threads)
{
	SCO::write_line("Start of control thread: ");
	std::random_device rnd_dev;
	std::default_random_engine dre{ rnd_dev() };
	std::uniform_int_distribution<int> dist(1, 10);

	{
		std::vector<std::future<std::thread::id>> threads;
		for (int i = 0; i < num_threads; i++)
		{
			threads.push_back(std::async(std::launch::async, thread_proc, dist(dre)));
		}
		SCO::write_line("All worker threads started.");
		// If we can ignore exceptions, then wait for threads end is called at threads goes out of scope
		/*
		try
		{
			for (auto& t : threads)
			{
				std::thread::id id = t.get();
				SCO::write_line("result of thread ", id);
			}
		}
		catch (const std::exception& ex)
		{
			SCO::write_line("Thread exited with EXCEPTION: ", ex.what());
		}
		*/
	}
	SCO::write_line("End of control thread.");
	return 0;
}

int main()
{
	try
	{
		std::thread t(control_thread, 10);
		t.detach();
		SCO::write_line("Main thread wait...");
		char c;
		std::cin >> c;
	}
	catch (const std::system_error& ex)
	{
		std::cout << ex.what();
		return 1;
	}
	return 0;
}