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

int main() {
    HANDLE hPipe;
    bool fSuccess = false;
    DWORD cbIO;

    // Подключение к существующему каналу
    hPipe = CreateFile(TEXT("\\\\.\\pipe\\mynamedpipe"),
        GENERIC_READ | GENERIC_WRITE,// Чтение/запись в канал.
        0, // Режим совместного использования в том числе на чтение, запись, удаление, их комбинаций
        NULL,// Атрибуты защиты.
        OPEN_EXISTING,// Флаг открытия канала, для работы с существующими каналами
        0,//  Если задан 0 , то тогда флаги и атрибуты будут определены по умолчанию
        NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        print_time();
        cout << "[ERROR] CreateFile failed with " << GetLastError() << '\n';
        return -1;
    }

    print_time();
    cout << "[MESSAGE] Connected to server.\n";

    while (true) {
        string message;


        // Проверка на ввод
        while (true) {
            print_time();
            cout << "[MESSAGE] Enter message to send to server: ";
            getline(cin, message);

            if (message == "calc" || message == "check" || message == "clear" || message == "break") {
                break;
            }
            else {
                // Проверка на наличие двух точек в одном числе и проверка запятых
                bool validInput = true;
                bool digitStarted = false;
                int dotCount = 0;

                int i = 0;
                // Если начинавется на арифметичекий знак то идем дальше 
                if (message.size() > 1 && (message[i] == '+' || message[i] == '-' ||
                    message[i] == '/' || message[i] == '*')) {
                    ++i;
                    //break;

                    // Проход по каждому символу
                    for (i; i < message.size(); ++i) {
                        if (isdigit(message[i])) {
                            digitStarted = true;
                        }
                        else if (message[i] == '.' && (i + 1) < message.size() && isdigit(message[i + 1])) {
                            if (!digitStarted) {
                                validInput = false;
                                break;
                            }
                            else {
                                dotCount++;
                                if (dotCount > 1) {
                                    validInput = false;
                                    break;
                                }
                            }
                        }
                        else {
                            //digitStarted = false;
                            validInput = false;
                            break;
                        }
                    }

                    if (!validInput) {
                        print_time();
                        cout << "[ERROR] Invalid input. Please enter a valid command or an arithmetic expression." << endl;
                    }
                    else {
                        break;
                    }
                }
                else {
                    print_time();
                    cout << "[ERROR] Invalid input. Please enter a valid command or an arithmetic expression." << endl;

                }
            }

        }

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
        if (response.find("[ERROR] ") != string::npos || response.find("Program is closed") != string::npos) {
            // Если получено сообщение об ошибке от сервера, выводим его и завершаем работу клиента
            print_time();
            cout << response << endl;
            CloseHandle(hPipe); // Закрываем канал
            break;
        }

        // Печать ответа от сервера
        print_time();
        cout << "[MESSAGE] Server response: " << string(buf, cbIO) << endl;
    }



    //print_time();
    //cout << "[MESSAGE] Closing pipe and exit\n";
    //CloseHandle(hPipe);
    return 0;
}