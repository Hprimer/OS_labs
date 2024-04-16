#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<winsock2.h>
#include "Client.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)//для inet_addr
using namespace std;

//проверка на то, отправилось и считалось ли
bool printCausedBy(int Result, const char* nameOfOper) {
	if (!Result) {
		cout << "Connection closed.\n";
		return false;
	}
	else if (Result < 0) {
		throw runtime_error(string(nameOfOper) + " failed:\n");
		//return false;
	}
	return true;
}
bool isValidInput(const string& input) {
	stringstream ss(input);
	char op;
	double num1, num2;
	if (!(ss >> op >> num1 >> num2)) {
		return false; // Не удалось считать операцию и два числа
	}
	if (ss.rdbuf()->in_avail() != 0) {
		return false; // Есть лишние символы после второго числа
	}
	if (op != '+' && op != '-' && op != '*' && op != '/') {
		return false; // Недопустимая операция
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
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //адрес
	addr.sin_port = htons(1111); //порт
	addr.sin_family = AF_INET; //семейство протоколов
	//сокет для прослушвания порта
	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	//проверка на подключение к серверу
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr))) {
		cout << "Unable to connect to intelligence agency.\n";
		return 1;
	}
	HANDLE hSemaphore;
	//открываем семафор
	hSemaphore = OpenSemaphore(SYNCHRONIZE, FALSE, L"semaphore");
	if (hSemaphore == NULL) {
		cerr << "Error open semaphore.\n";
		return GetLastError();
	}
	HANDLE pool[3] = {
	OpenEvent(EVENT_ALL_ACCESS,FALSE,L"0"),
	OpenEvent(EVENT_ALL_ACCESS,FALSE,L"1"),
	OpenEvent(EVENT_ALL_ACCESS,FALSE,L"end"),
	};
	for (int i = 0; i != 2; ++i)
		if (!pool[i])
			return GetLastError();
	// останавливает выполнение программы пока семафор не окажется
	// в сигнальном состоянии
	WaitForSingleObject(hSemaphore, INFINITE);
	cout << "Connection with intelligence agency established!\n";
	cout << "Enter the command <start> in order to begin >> ";
	string c;
	string str;
	cin >> c;
	while (cin.fail() || c != "start" || cin.peek() != '\n') {
		cin.clear();
		cin.ignore(32768, '\n');
		cout << "I don't understand you, sorry. Please, try again.\n";
		cout << "Enter the command <start> in order to begin >> ";
		cin >> c;
	}
	SetEvent(pool[0]); //устанавливаем событие в сигнальное состояние
	cout << "Enter the command <message> in order to enter and send the message.\n" << "Enter the command <end> in order to finish\n";
	cin >> c;
	try {
		while (c != "end")
		{
			while (c != "message" && c != "end" || cin.fail() || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(32768, '\n');
				cout << "Input error, please enter message or end >> ";
				cin >> c;
			}
			if (c == "message")
			{
				cin.clear();
				cin.ignore(32768, '\n');
				// Отправка сигнала серверу о готовности отправить сообщение
				SetEvent(pool[1]);

				// Ввод сообщения от пользователя (арифметической операции)
				cout << "Enter arithmetic operation in the format '+ 5 6': ";
				getline(cin, str);
				while (!isValidInput(str)) {
					cout << "Invalid input format. Please enter again: ";
					getline(cin, str);
				}

				// Отправка арифметической операции на сервер
				char buffer[1000];
				strcpy(buffer, str.c_str());
				printCausedBy(send(Connection, buffer, sizeof(buffer), NULL), "Send");

				// Ожидание ответа от сервера и вывод результата вычисления
				cout << "Waiting for server response..." << endl;
				printCausedBy(recv(Connection, buffer, sizeof(buffer), NULL), "Recv");
				string response = string(buffer);
				cout << "Server response: " << response << endl;

				// Вывод информации и получение следующей команды от пользователя
				cout << "Message sent. Enter the next command ('message' or 'end'): ";
				cin >> c;

			}
		}
	}
	catch (exception& ex) {
		cerr << "ERROR " << ex.what() << endl;
		cin.clear();
		cout << "Press Enter to exit...";
		cin.get(); // Ожидание нажатия Enter
	}
	SetEvent(pool[2]);
	for (int i = 0; i < 3; ++i)
		CloseHandle(pool[i]);
	CloseHandle(hSemaphore);
	if (closesocket(Connection) == SOCKET_ERROR)
		cerr << "Failed to terminate connection.\n Error code: " << WSAGetLastError();
	WSACleanup();
}


