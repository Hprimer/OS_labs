#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include<winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996) // для inet_addr
using namespace std;

//проверка на то, отправилось и считалось ли
bool printCausedBy(int Result, const char* nameOfOper) {
	if (!Result) {
		cout << "Connection closed.\n";
		return false;
	}
	else if (Result < 0) {
		cout << "ERROR " << nameOfOper;
		cout << " failed:\n", WSAGetLastError();
		return false;
	}
	return true;
}

int main()
{
	//Загрузка библиотеки
	WSAData wsaData; //создаем структуру для загрузки
	//запрашиваемая версия библиотеки winsock
	WORD DLLVersion = MAKEWORD(2, 1);
	//проверка подключения библиотеки
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cerr << "Error: failed to link library.\n";
		return 1;
	}
	//Заполняем информацию об адресе сокета
	SOCKADDR_IN addr;
	static int sizeOfAddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");//преобразование в 32 битный формат
	addr.sin_port = htons(1111);// устанавливает порт сокета
	addr.sin_family = AF_INET;//использование протокола IPv4
	//сокет для прослушвания порта
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	//привязка адреса сокету
	if (bind(sListen, (SOCKADDR*)&addr, sizeOfAddr) == SOCKET_ERROR)
	{
		printf("Error bind %d\n", WSAGetLastError());
		closesocket(sListen);
		WSACleanup();
		return 1;
	}
	//подкючение прослушивания с максимальной очередью
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) {
		cerr << "Listen failed.\n";
		closesocket(sListen);
		WSACleanup();
		return 1;
	}
	HANDLE hSemaphore;
	//Создание семафора, контролирующего до 2 обращений одновременно
	hSemaphore = CreateSemaphore(NULL, 2, 2, L"semaphore");
	if (hSemaphore == NULL)
		printf("CreateSemaphore error: %d\n", GetLastError());
	size_t client_number = 0;
	vector <SOCKET> Sockets(2); //вектор для сокетов
	size_t c_num = 0;
	int n; //для количества резидентов (клиентов)
	HANDLE pool[3] = {
	CreateEvent(NULL,FALSE,FALSE,L"0"),
	CreateEvent(NULL,FALSE,FALSE,L"1"),
	CreateEvent(NULL,FALSE,FALSE,L"end"),
	};
	for (uint8_t i = 0; i < 2; ++i)
		if (!pool[i])
			return GetLastError();
	cout << "Enter the number of residents 1 or 2 >> ";
	cin >> n;
	while (cin.fail() || n < 1 || n > 2 || cin.peek() != '\n') {
		cin.clear();
		cin.ignore(32768, '\n');
		cout << "I don't understand you, sorry. Please, try again.\n";
		cout << "Enter the number of residents 1 or 2 >> ";
		cin >> n;
	}
	//задаем информацию для окна открытия
	STARTUPINFO si;//структура
	PROCESS_INFORMATION pi;// структура с информацией о процессе
	ZeroMemory(&si, sizeof(si));//обнуление сттруктур
	si.cb = sizeof(si);// указываем размер
	ZeroMemory(&pi, sizeof(pi));
	//создаём новые окна для клиентов
	for (uint8_t i = 0; i < n; i++)
	{
		if (!CreateProcess(L"D:\\C++\\OS_lab5\\Client\\x64\\Debug\\Client.exe", // module name
			NULL, // Command line
			NULL, // Дескриптор процесса не наследуется
			NULL, // Дескриптор потока не наследуется
			FALSE, // Наследование дескриптора установлено на FALSE
			CREATE_NEW_CONSOLE, //Флаги создания процесса
			NULL, // Использование блока окружения родительского процесса
			NULL, // Использование рабочего каталога родительского процесса
			&si, // Указатель на структуру STARTUPINFO
			&pi) // Указатель на структуру PROCESS_INFORMATION
			)
		{
			printf("CreateProcess failed (%d).\n", GetLastError());
			return 1;
		}
		Sleep(50);
	}
	for (uint8_t i = 0; i < n; i++) { //сокеты для соединения с клиентом
		Sockets[i] = accept(sListen, (SOCKADDR*)&addr, &sizeOfAddr);
	}

	while (n) {
		//приостанавливает поток пока любой из объектов не перейдет
		// в сигнальное состояние или не закончится время ожидания
		int ind = WaitForMultipleObjects(3, pool, FALSE, INFINITE);
		bool err = false;
		if (ind == 0)
		{
			c_num = client_number;
			cout << "Intelligence agency is ready to receive a message!\n";
			++client_number;
		}
		if (ind == 1) {
			cout << "Resident " << c_num << " is ready to connect.\n";
			cout << "We receive a message...\n";

			char buffer[1000];
			if (!printCausedBy(recv(Sockets[c_num], buffer, sizeof(buffer), NULL), "Recv")) {
				--n;//чтобы потом завершить работу
			}

			// Разбор данных от клиента
			string str = string(buffer);
			double num1, num2;
			char operation;
			stringstream ss(buffer);
			ss >> operation >> num1 >> num2;

			// Вычисление результата
			double result;
			switch (operation) {
			case '+':
				result = num1 + num2;
				break;
			case '-':
				result = num1 - num2;
				break;
			case '*':
				result = num1 * num2;
				break;
			case '/':
				if (num2 != 0)
					result = num1 / num2;
				else {
					cout << "Error: Division by zero" << endl;
					err = true;
					// Отправка ответа клиенту
					string errorMessage = "Error: Division by zero";
					printCausedBy(send(Sockets[c_num],
						errorMessage.c_str(), errorMessage.size() + 1, NULL), "Send");

				}
				break;
			default:
				cout << "Error: Invalid operation" << endl;				
				err = true;
				// Отправка ответа клиенту
				string errorMessage = "Error: Invalid operation";
				printCausedBy(send(Sockets[c_num], errorMessage.c_str(), errorMessage.size() + 1, NULL), "Send");
			}
			if (!err) {//если не было ошибки
				// Отправка ответа клиенту
				string response = "Result: " + to_string(result);
				printCausedBy(send(Sockets[c_num], response.c_str(), response.size() + 1, NULL), "Send");
				// Вывод информации о полученной операции и результате на сервере
				cout << "Received from client " << c_num << " operation: " << num1 << " "
					<< operation << " " << num2 << ". Result: " << result << endl;
			}
		}
		if (ind == 2)
		{
			cout << "Resident " << c_num << " was disconnected.\n";
			ReleaseSemaphore(hSemaphore, 1, NULL);
			//отключение клиента
			--n;
		}
	}

	cout << "Work completed successfully!\n";
	// Close process and thread handles.
	CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);
	for (uint8_t i = 0; i < 3; ++i)
	{
		CloseHandle(pool[i]);
	}
	for (uint8_t i = 0; i <= c_num; ++i)
	{
		closesocket(Sockets[c_num]);
	}
	CloseHandle(hSemaphore);
	if (closesocket(sListen) == SOCKET_ERROR)
		cerr << "Failed to terminate connection.\n Error code: " << WSAGetLastError();
	WSACleanup();

	return 0;
}



