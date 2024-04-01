#include <windows.h>
#include <iostream>
#include <thread>
#include <sstream>
#define CONNECTING_STATE 0
#define READING_STATE 1
#define WRITING_STATE 2
#define INSTANCES 1
#define PIPE_TIMEOUT 5000
#define BUFSIZE 512
using namespace std;

typedef struct
{
    OVERLAPPED oOverlap; //Структура, позволяющая производить асинхронные операции
    HANDLE hPipeInst; //Дескриптор канала
    unsigned int cbRead; //Хранит кол-во прочитанных байт
    unsigned int size;
    unsigned int dwState; //Состояние работы канала: запись, чтение или подключение
    bool fPendingIO; //Состояние, отображающее, подключен ли к каналу клиент
} PIPEINST;

bool ConnectToNewClient(HANDLE, LPOVERLAPPED);
PIPEINST Pipe[INSTANCES];
HANDLE hEvents[INSTANCES];
void print_time() {
    SYSTEMTIME lt;
    GetLocalTime(&lt);
    printf("%02d:%02d:%02d\t", lt.wHour, lt.wMinute, lt.wMilliseconds);
}


double secondAnalysis(istringstream& iss) {
    double result;
    char symbol;

    if (!(iss >> result))
        throw runtime_error("Error");

    while (iss >> symbol) {
        if (symbol == '*' || symbol == '/') {
            double operand;
            if (!(iss >> operand))
                throw runtime_error("Missing operand");
            if (symbol == '*')
                result *= operand;
            else if (operand == 0)
                throw runtime_error("Division by zero");
            else
                result /= operand;
        }
        else {
            iss.putback(symbol);
            break;
        }
    }
    return result;
}

double firstAnalysis(istringstream& iss) {
    double result = secondAnalysis(iss);
    char symbol;

    while (iss >> symbol) {
        if (symbol == '+' || symbol == '-') {
            double next = secondAnalysis(iss);
            if (symbol == '+')
                result += next;
            else
                result -= next;
        }
        else {
            iss.putback(symbol);//возвращает обратно символ
            break;
        }
    }
    return result;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

bool check_correctness(const string& expression) {
    // Проверка, чтобы выражение не было пустым
    if (expression == "") {
        print_time();
        cerr << "[ERROR] Expression cannot be empty" << endl;
        return false;
    }
    // Проверка, что выражение не начинается с '/' или '*'
    if (expression[0] == '/' || expression[0] == '*') {
        print_time();
        cerr << "[ERROR] Expression cannot start with '/' or '*'" << endl;
        return false;
    }

    // Проверка, что после '/' нет 0
    for (int i = 0; i < expression.length() - 1; ++i) {
        if (expression[i] == '/' && expression[i + 1] == '0' && (!((i + 2) < expression.length() - 1) && expression[i + 2] != '.')) {
            print_time();
            cerr << "[ERROR]: Division by zero is not allowed" << endl;
            return false;
        }
    }

    // Проверка, что не встречаются подряд */ или +- или другие 
    for (int i = 0; i < expression.length() - 1; ++i) {
        if (isOperator(expression[i]) && isOperator(expression[i + 1])) {
            print_time();
            cerr << "[ERROR] Consecutive operators are not allowed" << endl;
            return false;
        }
    }

    // Проверка, что после арифметической операции идет цифра
    for (int i = 0; i < expression.length() - 1; ++i) {
        if (isOperator(expression[i]) && !isdigit(expression[i + 1])) {
            print_time();
            cerr << "[ERROR] Digit must follow arithmetic operator" << endl;
            return false;
        }
    }

    // Если все проверки пройдены успешно
    return true;
}
string calculate(const string& exp) {
    if (!check_correctness(exp))
        throw runtime_error("Incorrect expression");

    istringstream iss(exp); // Создаем поток ввода из строки

    double result = firstAnalysis(iss);
    print_time();
    cout << "[MESSAGE] Result: " << result << endl;

    string str = " = " + to_string(result);
    return str;
}


int main() {
    HANDLE hPipe;
    bool fSuccess = false;
    DWORD cbIO;

    // Создание и ожидание подключения к каналу
    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\mynamedpipe"),//Имя канала
        PIPE_ACCESS_DUPLEX,//Чтение и запись в канал
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,// режим передачи данных
        PIPE_UNLIMITED_INSTANCES,// максимальное количество экземпляров канала 
        BUFSIZE,// размер выходного буфера
        BUFSIZE,// размер входного буфера
        0,// время ожидания связи с клиентом 
        NULL);// атрибуты защиты
    try {
        if (hPipe == INVALID_HANDLE_VALUE) {
            print_time();
            cout << "[ERROR] CreateNamedPipe failed with " << GetLastError() << '\n';
            return 0;
        }

        print_time();
        cout << "[MESSAGE] Waiting for client connection...\n";

        if (!ConnectNamedPipe(hPipe, NULL)) {// Проверка того, что клиент подключился к каналу
            print_time();
            cout << "[ERROR] ConnectNamedPipe failed with " << GetLastError() << '\n';
            CloseHandle(hPipe);
            return 0;
        }

        print_time();
        cout << "[MESSAGE] Client connected, waiting for messages...\n";

        string expression;//выражение
        while (true) {
            char buf[BUFSIZE];
            fSuccess = ReadFile(hPipe, buf, BUFSIZE, &cbIO, NULL);

            if (!fSuccess || cbIO == 0) {
                DWORD dwErr = GetLastError();
                if (dwErr == ERROR_BROKEN_PIPE) {
                    print_time();
                    cout << "[MESSAGE] Client disconnected.\n";
                }
                else {
                    print_time();
                    cout << "[ERROR] ReadFile failed with " << dwErr << '\n';
                }
                break;
            }


            // Печать полученного сообщения
            string str = string(buf, cbIO);
            print_time();
            cout << "[MESSAGE] Received message from client: " << str << endl;

            if (str == "calc") {//здесь программа должна вывести результат и очистать выражение
                expression += calculate(expression);
                fSuccess = WriteFile(hPipe, expression.c_str(), expression.length(), &cbIO, NULL);
                if (!fSuccess) {
                    print_time();
                    cout << "[ERROR] WriteFile failed with " << GetLastError() << '\n';
                    break;
                }
                expression = "";
            }
            else if (str == "check") {//программа проверяет на коррект и продолжает работу
                string message;
                if (check_correctness(expression)) {                   
                    message = "Expression correct";
                }
                else {
                    message = "Expression incorrect";
                }
                fSuccess = WriteFile(hPipe, message.c_str(), message.length(), &cbIO, NULL);
                if (!fSuccess) {
                    print_time();
                    cout << "[ERROR] WriteFile failed with " << GetLastError() << '\n';
                    break;
                }
            }
            else if (str == "clear") {// Очистка выражения, программа продолжает работать
                expression = "";
                fSuccess = WriteFile(hPipe, "Expression is cleared", strlen("Expression is cleared"), &cbIO, NULL);
                if (!fSuccess) {
                    print_time();
                    cout << "[ERROR] WriteFile failed with " << GetLastError() << '\n';
                    break;
                }
            }
            else if (str == "break") {// Выход из программы

                fSuccess = WriteFile(hPipe, "[MESSAGE] Program is closed", strlen("[MESSAGE] Program is closed"), &cbIO, NULL);
                if (!DisconnectNamedPipe(hPipe)) {
                    print_time();
                    cout << "[ERROR] DisconnectNamedPipe failed with " << GetLastError() << '\n';
                }
                print_time();
                cout << "[MESSAGE] Client disconnected\n";
                break;
            }
            else {
                expression += str;

                fSuccess = WriteFile(hPipe, expression.c_str(), expression.length(), &cbIO, NULL);
                if (!fSuccess) {
                    print_time();
                    cout << "[ERROR] WriteFile failed with " << GetLastError() << '\n';
                    break;
                }
            }

        }
    }
    catch (exception& ex)
    {
        print_time();
        cerr << "[ERROR] " << ex.what() << endl;
        // Передача ошибки клиенту и отключение
        string errorMessage = "[ERROR] ";
        errorMessage += ex.what();
        fSuccess = WriteFile(hPipe, errorMessage.c_str(), errorMessage.size(), &cbIO, NULL);
        if (!DisconnectNamedPipe(hPipe)) {
            print_time();
            cout << "[ERROR] DisconnectNamedPipe failed with " << GetLastError() << '\n';
        }
        print_time();
        cout << "[MESSAGE] Client disconnected\n";
        
    }

    CloseHandle(hPipe);
    return 0;
}