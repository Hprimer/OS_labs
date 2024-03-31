#include <windows.h>
#include <iostream>
#include <string>
#define BUFSIZE 512
using namespace std;
void print_time() {
	SYSTEMTIME lt;
	GetLocalTime(&lt);
	printf("%02d:%02d:%02d\t", lt.wHour, lt.wMinute, lt.wMilliseconds);
}
//int main(int argc, char* argv[])
//{
//	HANDLE hPipe; // Канал
//	bool fSuccess = false; // Переменная для проверки корректности операций
//	unsigned int cbIO; //Количество считанных/записанных байт
//	int number_count; //Количество чисел, которые надо передать
//	//Пытаемся открыть канал и ждем его, если недоступен
//	while (1)
//	{
//		hPipe = CreateFile(TEXT("\\\\.\\pipe\\mynamedpipe"), //Имя канала
//			GENERIC_READ | //Чтение и запись из канала
//			GENERIC_WRITE,
//			0, //Уровень общего доступа
//			NULL, //Атрибуты безопасности
//			OPEN_EXISTING, //Открыть существующий
//			0, //Атрибуты файла
//			NULL); //Файл шаблона
//		if (hPipe != INVALID_HANDLE_VALUE)
//			break;
//		//Если не удалось открыть канал
//		if (GetLastError() != ERROR_PIPE_BUSY) {
//			print_time();
//			cout << "[ERROR] Could not open pipe. GLE=" << GetLastError() << '\n';
//			return -1;
//		}
//		//Если все экземпляры канала заняты, ждем 20 секунд
//		if (!WaitNamedPipe(TEXT("\\\\.\\pipe\\mynamedpipe"), 20000)) {
//			print_time();
//			cout << "[ERROR] Could not open pipe: 20 second wait timed out.";
//			return -1;
//		}
//	}
//	number_count = 1;
//	//Отправляем серверу, какое кол-во значений надо получить клиенту
//	//print_time();
//	//cout << "[MESSAGE] Write\n";
//	fSuccess = WriteFile(
//		hPipe, //Канал
//		&number_count, //Данные для передачи		
//		4, //Количество переданных байт
//		reinterpret_cast<LPDWORD>(&cbIO), //Сколько байт передалось на самом деле
//		NULL); //Не асинхронный вывод
//	if (!fSuccess) {
//		print_time();
//		cout << "[ERROR] WriteFile to pipe failed. GLE=" << GetLastError() << '\n';
//		return -1;
//	}
//	print_time();
//	cout << "[MESSAGE] Message sent to server, receiving reply as follows:\n";
//	//Читаем из канала необходимые значения
//	print_time();
//	cout << "[MESSAGE] Read\n";
//	int* x = new int[number_count];
//	do {
//		for (int i = 0; i < number_count; i++) {
//			fSuccess = ReadFile(
//				hPipe, // Канал
//				&(*(x + i)), //Куда записываем данные
//				4, //Какое кол-во байт записываем
//				reinterpret_cast<LPDWORD>(&cbIO), //Сколько записали байт
//				NULL); //Не асинхронное чтение
//
//			int receivedData;
//			int receivedData2;
//			ReadFile(hPipe, &receivedData, sizeof(receivedData), reinterpret_cast<LPDWORD>(&cbIO), NULL);
//			ReadFile(hPipe, &receivedData2, sizeof(receivedData), reinterpret_cast<LPDWORD>(&cbIO), NULL);
//
//
//			if ((*(x + i) == 1 && i != 0) || *(x + i) == -1) break;
//			cout << x[i] << '\n';
//			cout << receivedData2 << endl;
//
//		}
//		if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
//			break;
//	} while (!fSuccess);
//
//
//
//
//
//
//
//	if (!fSuccess) {
//		print_time();
//		cout << "[ERROR] ReadFile from pipe failed. GLE=" << GetLastError() << '\n';
//		return -1;
//	}
//	print_time();
//	cout << "[MESSAGE] Closing pipe and exit\n";
//	CloseHandle(hPipe);
//	return 0;
//}


int main() {
    HANDLE hPipe;
    bool fSuccess = false;
    DWORD cbIO;

    // Подключение к существующему каналу
    hPipe = CreateFile(TEXT("\\\\.\\pipe\\mynamedpipe"),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        print_time();
        cout << "[ERROR] CreateFile failed with " << GetLastError() << '\n';
        return -1;
    }

    print_time();
    cout << "[MESSAGE] Connected to server.\n";
    try {
        while (true) {
            // Ввод сообщения с клавиатуры
            string message;
           

            // Проверка на ввод
            while (true) {
                // Ввод сообщения с клавиатуры
                cout << "Enter message to send to server: ";
                getline(cin, message);

                if (message == "calc" || message == "check" || message == "clear" || message == "break") {
                    break;
                }
                else if (message.size() > 0 && (message[0] == '+' || message[0] == '-' ||
                    message[0] == '/' || message[0] == '*')) {
                    break;
                }
                else {
                    cout << "Invalid input. Please enter a valid command or an arithmetic expression." << endl;
                }
            }
            //getline(cin, message);

            //if (message[0] != '+' or message[0] != '-' or message != "calc" && message != "check" && message != "clear" && message != "break") {
            //    throw runtime_error("invalid input");
            //}

            //if (message == "calc") {//здесь программа должна вывести результат и закончить работу
            //    calculate();
            //}
            //else if (message == "check") {//программа проверяет на коррект и продолжает работу с предложением очистить строку
            //    check_correctness();
            //}
            //if (message == "clear") {
            //    clear_str();
            //}


            // Отправка сообщения серверу
            fSuccess = WriteFile(hPipe, message.c_str(), message.size(), &cbIO, NULL);
            if (!fSuccess) {
                print_time();
                cout << "[ERROR] WriteFile failed with " << GetLastError() << '\n';
                break;
            }

            // Чтение ответа от сервера
            char buf[BUFSIZE];
            fSuccess = ReadFile(hPipe, buf, BUFSIZE, &cbIO, NULL);
            if (!fSuccess || cbIO == 0) {
                print_time();
                cout << "[ERROR] ReadFile failed with " << GetLastError() << '\n';
                break;
            }

            string response(buf, cbIO);
            if (response.find("Error:") != string::npos || response.find("Program is closed") != string::npos) {
                // Если получено сообщение об ошибке от сервера, выводим его и завершаем работу клиента
                cout << response << endl;
                CloseHandle(hPipe); // Закрываем канал
                break;
            }

            // Печать ответа от сервера
            cout << "Server response: " << string(buf, cbIO) << endl;
        }

    }
    catch (exception& ex)
    {
        cerr << "Error: " << ex.what() << endl;
    }

    //CloseHandle(hPipe);
    return 0;
}