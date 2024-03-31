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

void DisconnectAndReconnect(unsigned int);
bool ConnectToNewClient(HANDLE, LPOVERLAPPED);
PIPEINST Pipe[INSTANCES];
HANDLE hEvents[INSTANCES];
void print_time() {
	SYSTEMTIME lt;
	GetLocalTime(&lt);
	printf("%02d:%02d:%02d\t", lt.wHour, lt.wMinute, lt.wMilliseconds);
}
//int main(void)
//{
//	unsigned int i, dwWait, cbRet, dwErr;
//	bool fSuccess;
//	bool pSuccess = true;
//	//Инициализация всех экземляров канала
//	for (i = 0; i < INSTANCES; i++) {
//		//Событие для экземпляра
//		hEvents[i] = CreateEvent(
//			NULL, //Атрибут защиты
//			true, //Ручное управление
//			true, //Начальное состояние - активно
//			NULL); //Имя
//		if (hEvents[i] == NULL) {
//			print_time();
//			cout << "[ERROR] CreateEvent failed with " << GetLastError() << '\n';
//			return 0;
//		}
//		//Присваиваем структуре OVERLAP событие
//		Pipe[i].oOverlap.hEvent = hEvents[i];
//		//Инициализируем сам канал
//		Pipe[i].hPipeInst = CreateNamedPipe(TEXT("\\\\.\\pipe\\mynamedpipe"), //Имя канала
//			PIPE_ACCESS_DUPLEX | //Чтение и запись в канал
//			FILE_FLAG_OVERLAPPED, //Включен перекрывающийся ввод/вывод
//			PIPE_TYPE_MESSAGE | // Канал передает сообщения, а не поток байт
//			PIPE_READMODE_MESSAGE | // Канал считывает сообщения
//			PIPE_WAIT, // Клиент будет ожидать, когда канал станет доступен
//			INSTANCES, // Максимальное количество экземпляров канала
//			BUFSIZE * 4, //Выходной размер буфера
//			BUFSIZE * 4, //Входной размер буфера
//			PIPE_TIMEOUT, // Время ожидания клиента
//			NULL); //Атрибут защиты
//		print_time();
//		cout << "[MESSAGE] Initializing Pipe[" << i << "]\n";
//		if (Pipe[i].hPipeInst == INVALID_HANDLE_VALUE) {
//			print_time();
//			cout << "[ERROR] CreateNamedPipe failed with " << GetLastError() << '\n';
//			return 0;
//		}
//		//Подключаем канал к клиенту
//		Pipe[i].fPendingIO = ConnectToNewClient(Pipe[i].hPipeInst, &Pipe[i].oOverlap);
//		//Смотрим, произошло ли подключение к клиенту
//		Pipe[i].dwState = Pipe[i].fPendingIO ?
//			CONNECTING_STATE : // Нет
//			READING_STATE; //Да
//	}
//	while (1) {
//		int j = 1;
//		//Ждем, пока один из каналов закончит одну из операций
//		dwWait = WaitForMultipleObjects(
//			INSTANCES, // Количество событий
//			hEvents, // События
//			false, // Не ждать всех
//			INFINITE); // Ждать бесконечно
//		//dwWait указывает, какой канал завершил операцию. WAIT_OBJECT_0 -
//		//первый элемент массива каналов
//			i = dwWait - WAIT_OBJECT_0; //Указывает какой канал в массиве завершил операцию
//			if (i < 0 || i >(INSTANCES - 1)) {
//				print_time();
//				cout << "[ERROR] Index out of range.\n";
//				return 0;
//			}
//		if (Pipe[i].fPendingIO) {
//			fSuccess = GetOverlappedResult(
//				Pipe[i].hPipeInst, //Экземпляр канала
//				&Pipe[i].oOverlap, //Структура OVERLAPPED соответствующего канала
//				reinterpret_cast<LPDWORD>(&cbRet), //Количество переданных байтов
//				false); //Не ждать
//			switch (Pipe[i].dwState) {
//				//Ждет начала операции чтения
//			case CONNECTING_STATE:
//				print_time();
//				cout << "[MESSAGE] Connected\n";
//				if (!fSuccess) {
//					print_time();
//					cout << "[ERROR] Error code" << GetLastError() << '\n';
//					return 0;
//				}
//				Pipe[i].dwState = READING_STATE;
//				break;
//				//Ждет начала операции записи
//			case READING_STATE:
//				if (!fSuccess || cbRet == 0) {
//					DisconnectAndReconnect(i);
//					continue;
//				}
//				Pipe[i].cbRead = cbRet;
//				Pipe[i].dwState = WRITING_STATE;
//				break;
//				//Ждет начала операции чтения
//			case WRITING_STATE:
//				if (!fSuccess || cbRet != 4) {
//					DisconnectAndReconnect(i);
//					continue;
//				}
//				Pipe[i].dwState = READING_STATE;
//				break;
//			default: {
//				print_time();
//				cout << "[ERROR] Invalid pipe state.\n";
//				return 0;
//			}
//			}
//		}
//		//Выполняем текущее действие
//		switch (Pipe[i].dwState) {
//			//Читаем данные из канала
//		case READING_STATE:
//			print_time();
//			cout << "[MESSAGE] [" << i << "]:Read\n";
//			fSuccess = ReadFile(
//				Pipe[i].hPipeInst,
//				&Pipe[i].size,
//				4,
//				reinterpret_cast<LPDWORD>( &Pipe[i].cbRead),
//				&Pipe[i].oOverlap);
//			//При успешном завершении чтения
//			if (fSuccess && Pipe[i].cbRead != 0) {
//				Pipe[i].fPendingIO = false;
//				Pipe[i].dwState = WRITING_STATE;
//				continue;
//			}
//			//Если операция чтения еще не закончилась, то пропускаем
//			dwErr = GetLastError();
//			if (!fSuccess && (dwErr == ERROR_IO_PENDING)) {
//				Pipe[i].fPendingIO = true;
//				continue;
//			}
//			//Иначе если произошла ошибка, отключаемся от клиента
//			DisconnectAndReconnect(i);
//			break;
//			//Запись данных в канал
//		case WRITING_STATE: {
//			if (Pipe[i].size > BUFSIZE) {
//				print_time();
//				cout << "[ERROR] The numbers size exceeds the size of the buffer\n";
//				dwErr = -1;
//				fSuccess = WriteFile(Pipe[i].hPipeInst, &dwErr, 4, reinterpret_cast<LPDWORD>(&cbRet),
//					&Pipe[i].oOverlap);
//				break;
//			}
//			print_time();
//			cout << "[MESSAGE] [" << i << "]:Write\n";
//			for (; j <= Pipe[i].size; j++) {
//				Sleep(500);
//				fSuccess = WriteFile(Pipe[i].hPipeInst, &j, 4, reinterpret_cast<LPDWORD>(&cbRet), &Pipe[i].oOverlap);
//				if (!fSuccess && pSuccess) {
//					DisconnectAndReconnect(i);
//					pSuccess = fSuccess;
//				}
//			}
//			int b = -1;
//			fSuccess = WriteFile(Pipe[i].hPipeInst, &b, 4, reinterpret_cast<LPDWORD>(&cbRet), &Pipe[i].oOverlap);
//			
//
//
//
//
//			int dataToSend = 123; // Пример данных для передачи
//			WriteFile(Pipe[i].hPipeInst, &dataToSend, sizeof(dataToSend), reinterpret_cast<LPDWORD>(&cbRet), &Pipe[i].oOverlap);
//
//
//
//
//
//
//
//			
//			//При успешном завершении записи
//			if (fSuccess && cbRet == 4) {
//				Pipe[i].fPendingIO = false;
//				Pipe[i].dwState = READING_STATE;
//				pSuccess = true;
//				j = 1;
//				continue;
//			}
//			//Если запись еще не завершилась, то пропускаем
//			dwErr = GetLastError();
//			if (!fSuccess && (dwErr == ERROR_IO_PENDING)) {
//				Pipe[i].fPendingIO = true;
//				continue;
//			}
//			//Иначе если произошла ошибка, отключаемся от клиента
//			DisconnectAndReconnect(i);
//			break;
//		}
//		default: {
//			print_time();
//			cout << "[ERROR] Invalid pipe state.\n";
//			return 0;
//		}
//		}
//	}
//	return 0;
//}


//void calculate(string exp) {
//    if (exp[0] == '/' || exp[0] == '*')
//        throw runtime_error("incorrect expression");
//    for (int i = 0; i < exp.length(); ++i) {
//
//    }
//};

double parseFactor(std::istringstream& iss) {
    double result;
    char symbol;

    if (!(iss >> result))
        throw std::runtime_error("Missing factor");

    while (iss >> symbol) {
        if (symbol == '*' || symbol == '/') {
            double operand;
            if (!(iss >> operand))
                throw std::runtime_error("Missing operand");
            if (symbol == '*')
                result *= operand;
            else if (operand == 0)
                throw std::runtime_error("Division by zero");
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

double parseTerm(std::istringstream& iss) {
    double result = parseFactor(iss);
    char symbol;

    while (iss >> symbol) {
        if (symbol == '+' || symbol == '-') {
            double nextFactor = parseFactor(iss);
            if (symbol == '+')
                result += nextFactor;
            else
                result -= nextFactor;
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
    // Проверка, что выражение не начинается с '/' или '*'
    if (expression[0] == '/' || expression[0] == '*') {
        std::cerr << "Error: Expression cannot start with '/' or '*'" << std::endl;
        return false;
    }

    // Проверка, что после '/' нет 0
    for (size_t i = 0; i < expression.length() - 1; ++i) {
        if (expression[i] == '/' && expression[i + 1] == '0') {
            std::cerr << "Error: Division by zero is not allowed" << std::endl;
            return false;
        }
    }

    // Проверка, что не встречаются подряд */ или +- или другие аномалии
    for (size_t i = 0; i < expression.length() - 1; ++i) {
        if (isOperator(expression[i]) && isOperator(expression[i + 1])) {
            std::cerr << "Error: Consecutive operators are not allowed" << std::endl;
            return false;
        }
    }

    // Проверка, что после арифметической операции идет цифра
    for (size_t i = 0; i < expression.length() - 1; ++i) {
        if (isOperator(expression[i]) && !isdigit(expression[i + 1])) {
            std::cerr << "Error: Digit must follow arithmetic operator" << std::endl;
            return false;
        }
    }

    // Если все проверки пройдены успешно
    return true;
}
string calculate(const string& exp) {
    if (!check_correctness(exp))
        throw runtime_error("incorrect expression");

    istringstream iss(exp); // Создаем поток ввода из строки

    //double result; // Результат вычисления
    //char op; // Оператор


    double result = parseTerm(iss);
    std::cout << "Result: " << result << std::endl;

    //if (!(iss >> op)) // Читаем первый символ, должен быть оператором
    //    throw std::runtime_error("Empty expression");
    //
    //if (op == '/' || op == '*') // Проверяем корректность первого оператора
    //    throw std::runtime_error("Incorrect expression");
    //
    //if (!(iss >> result)) // Читаем первое число
    //    throw std::runtime_error("Missing operand");
    //
    //// Вычисляем выражение
    //while (iss >> op) {
    //    double operand;
    //    if (!(iss >> operand)) // Читаем следующее число
    //        throw std::runtime_error("Missing operand");
    //
    //    // Выполняем операцию в зависимости от оператора
    //    switch (op) {
    //    case '+':
    //        result += operand;
    //        break;
    //    case '-':
    //        result -= operand;
    //        break;
    //    case '*':
    //        result *= operand;
    //        break;
    //    case '/':
    //        if (operand == 0) // Проверяем деление на ноль
    //            throw std::runtime_error("Division by zero");
    //        result /= operand;
    //        break;
    //    default:
    //        throw std::runtime_error("Invalid operator");
    //    }
    //}

    //cout << "Result: " << result << endl; // Выводим результат
   
    string str = " = " + to_string(result);
    return str;
}

//void clear_str(string);

int main() {
    HANDLE hPipe;
    bool fSuccess = false;
    DWORD cbIO;

    // Создание и ожидание подключения к каналу
    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\mynamedpipe"),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        BUFSIZE,
        BUFSIZE,
        0,
        NULL);
    try {
        if (hPipe == INVALID_HANDLE_VALUE) {
            print_time();
            cout << "[ERROR] CreateNamedPipe failed with " << GetLastError() << '\n';
            return -1;
        }

        print_time();
        cout << "[MESSAGE] Waiting for client connection...\n";

        if (!ConnectNamedPipe(hPipe, NULL)) {
            print_time();
            cout << "[ERROR] ConnectNamedPipe failed with " << GetLastError() << '\n';
            CloseHandle(hPipe);
            return -1;
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
            cout << "Received message from client: " << str << endl;

            if (str == "calc") {//здесь программа должна вывести результат и закончить работу
                expression+=calculate(expression);
                fSuccess = WriteFile(hPipe, expression.c_str(), expression.length(), &cbIO, NULL);
                if (!fSuccess) {
                    print_time();
                    cout << "[ERROR] WriteFile failed with " << GetLastError() << '\n';
                    break;
                }
                expression = "";
            }
            else if (str == "check") {//программа проверяет на коррект и продолжает работу с предложением очистить строку
                string message;
                if (check_correctness(expression)) {
                    message = "Expression correct\n";
                }
                else {
                    message = "Expression incorrect\n";
                }
                fSuccess = WriteFile(hPipe, message.c_str(), message.length(), &cbIO, NULL);
                if (!fSuccess) {
                    print_time();
                    cout << "[ERROR] WriteFile failed with " << GetLastError() << '\n';
                    break;
                }
            }
            else if (str == "clear") {
                expression="";
            }
            else if (str == "break") {
                if (!DisconnectNamedPipe(hPipe)) {
                    print_time(); 
                    cout << "[ERROR] DisconnectNamedPipe failed with " << GetLastError() << '\n';
                }

            }
            else {
                expression += str;


                // Пример: отправка ответа клиенту
                //const char* response = "Server received your message.";
                //fSuccess = WriteFile(hPipe, response, strlen(response), &cbIO, NULL);
                //if (!fSuccess) {
                //    print_time();
                //    cout << "[ERROR] WriteFile failed with " << GetLastError() << '\n';
                //    break;
                //}

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
        cerr << "Error: " << ex.what() << endl;
    }

    CloseHandle(hPipe);
    return 0;
}



//Отключаемся от текущего клиента и ждем нового
void DisconnectAndReconnect(unsigned int i)
{
	//Отключаемся от текущего
	//print_time();
	//cout << "[MESSAGE] [" << i << "]:Disconnecting\n";
	if (!DisconnectNamedPipe(Pipe[i].hPipeInst)) {
		print_time();
		cout << "[ERROR] DisconnectNamedPipe failed with " << GetLastError() << '\n';
	}
	//Подключаемся к новому клиенту
	Pipe[i].fPendingIO = ConnectToNewClient(Pipe[i].hPipeInst, &Pipe[i].oOverlap);
	//Проверяем состояние
	Pipe[i].dwState = Pipe[i].fPendingIO ?
		CONNECTING_STATE : //Ждем подключения
		READING_STATE; //Подключен
}
//Если клиент подключился, то fPendingIO = false
bool ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo)
{
	BOOL fConnected, fPendingIO = false;
	//Пытаемся подключиться к каналу
	fConnected = ConnectNamedPipe(hPipe, lpo);
	if (fConnected) {
		print_time();
		cout << "[ERROR] ConnectNamedPipe failed with " << GetLastError() << '\n';
		return 0;
	}
	switch (GetLastError())
	{
		//Подключение в процессе. Функцию выполнили, и если подключения не произошло, выбрасывается данная ошибка
	case ERROR_IO_PENDING:
		//print_time();
		//cout << "[MESSAGE] Waiting\n";
		fPendingIO = true;
		break;
		//Если клиент уже ожидал подключения. Просто обновляем событие у канала
	case ERROR_PIPE_CONNECTED:
		print_time();
		cout << "[MESSAGE] Connected\n";
		if (SetEvent(lpo->hEvent))
			break;
		//Ошибка
	default: {
		print_time();
		cout << "[ERROR] ConnectNamedPipe failed with " << GetLastError() << '\n';
		return 0;
	}
	}
	return fPendingIO;
}