// tcp_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void connect_thread(std::atomic<bool>& end_thread)
{
	WSADATA wsa;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsa);

	sockaddr_in dest_addr = {};
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(5003);
	inet_pton(dest_addr.sin_family, "10.7.1.10", &dest_addr.sin_addr);
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(s, (sockaddr*)&dest_addr, sizeof(dest_addr)) != SOCKET_ERROR)
	{
		char msg[] = "\r\n";
		send(s, msg, strlen(msg), 0);
		char buff[32];
		cout << "Connected to " << inet_ntop(AF_INET, &dest_addr.sin_addr, buff, sizeof(buff)) << endl;
		while (!end_thread)
		{
			int n = recv(s, buff, 31, 0);
			if (n > 0)
			{
				buff[n] = 0;
				cout << "Data: " << buff;
			}
		}
	}

	closesocket(s);
	WSACleanup();
}

int main()
{
	std::atomic<bool> end_flag = false;
	thread c_thread(connect_thread, std::ref(end_flag));
	for (char c = 0; c != 'q'; cin >> c)
	{
	}
	end_flag = true;
	c_thread.join();
}

