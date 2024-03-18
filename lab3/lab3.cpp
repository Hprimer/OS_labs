#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <string.h>
#include <sstream>
#include <cmath>
#include <cstring>
#include <regex>

using namespace std; 


bool fileFlag;
const char* input_filename;
const char* output_filename;

bool checkNum(const string& num, int base) {// Проверяем, что база системы счисления корректна    
    if (base < 2 || base > 36) {
        return false;
    }

    for (char c : num) {
        
        if (!isdigit(c)) {
            if (base > 10 && isalpha(c)) {                
                c = toupper(c);                
                if (c < 'A' || c > 'Z' || c - 'A' + 10 >= base) {//проверяем, что символ находится в допустимом диапазоне
                    return false;
                }
            } else {
                return false;
            }
        } else {//проверка цифры
            
            if (c - '0' >= base) {//eсли цифра, проверяем, что она меньше основания
                return false;
            }
        }
    }

    return true;
}
void fillFromFile(const char* fileName, string *num, int *base){
    ifstream file(fileName);
    regex firstNum("^[a-zA-Z0-9]+$");

    if(!file.is_open()){
        throw runtime_error("Error with open file");
    }

    if (!(file >> *num >> *base)) {
        cout << "Ошибка чтения из файла " << fileName << endl;
        throw runtime_error("Ошибка чтения из файла");
    }
    if (!regex_match(*num, firstNum)) {
        cout << "Ошибка: Некорректные данные в числе." << endl;
        file.close();
        throw runtime_error("Ошибка: Некорректные данные в числе.");
        
    }

    if (!checkNum(*num, *base)){
        throw runtime_error("Число содержит символы, отсутствующие в выбранной системе счисления");
    }

    file.close();
  
}
void printToFile(const char* fileName, int res1, string res2){
    ofstream file(fileName);

    if(!file.is_open()){
        throw runtime_error("Error with open file");
    }
    file << "Результат в десятичной системе счисления : " << res1 <<" и в изначальной: "<< res2;
    file.close();  
}
void fillFromConsole(string *num, int * base){
    regex firstNum("^[a-zA-Z0-9]+$");

    cout << "Введите число в произвольной системе счисления (для выхода введите 'exit'): ";
    cin >> *num;        

    while(!regex_match(*num, firstNum)) {
        cout << "Ошибка: Некорректные данные в числе." << endl;
        //throw runtime_error("Ошибка: Некорректные данные в числе.");
        cin>> *num;
    }
    cout << "Введите основание системы счисления: ";
    cin >> *base;
    while(!checkNum(*num, *base)){
        throw runtime_error("Ошибка, число содержит символы, отсутствующие в выбранной системе счисления.");
    }
}

int toDecimal(const string &num, int base) {//перевод числа из произвольной системы счисления в десятичную
    int result = 0;
    int len = num.length();
    for (int i = len - 1; i >= 0; --i) {
        result += (isdigit(num[i]) ? (num[i] - '0') : (toupper(num[i]) - 'A' + 10)) * pow(base, len - 1 - i);
    }
    return result;
}
string decimalToArbitrary(int num,  int base) {//перевод числа из десятичную системы счисления в произвольной 
    string result = "";
    while (num > 0) {
        int ostat = num % base;
        if (ostat < 10)
            result = (char)(ostat + '0') + result;
        else
            result = (char)(ostat - 10 + 'A') + result;
        num /= base;
    }
    if (result.empty())
        result = "0";
    return result;
}

void parent_process(int *fdin, int *fdout) {//родительскпй процесс
    string number;
    int base;

    if(fileFlag)
    {
        fillFromFile(input_filename, &number, &base);
    }
    else
    {
        fillFromConsole(&number, &base);
    }
    
    int len= number.size()+1;// Включая нулевой символ
    write (fdin[1], &len, sizeof(int));
    write(fdin[1], number.c_str(), len); 
    write(fdin[1], &base, sizeof(int));
    cout << "Отправлено на сервер: " << number << "   "<<base  <<"  " <<len<< endl;


    // Чтение результата 
    int result;
    char *res2 = new char[len];
    
    read(fdout[0], res2, len);
    read(fdout[0], &result, sizeof(result));

    if(fileFlag)
        printToFile(output_filename, result, string(res2));    
    else
        cout << "Результат в десятичной системе счисления : " << result <<" и в изначальной: "<< string(res2) << endl; 

    delete[] res2;
} 

void child_process(int *fdin, int *fdout) {
    
    int base;    
    int len;
    //cout<<"родительский процесс запущен\n";
    read(fdin[0], &len, sizeof(int));

    char *number = new char[len];

    read(fdin[0], number, len);
    read(fdin[0], &base, sizeof(base));
    cout << "получено на сервер: " << number <<"  "<< base<<"  "<<len  << endl;


    // Перевод числа туда сюда
    int decimal = toDecimal(string(number), base);
    string dec2 = decimalToArbitrary(decimal,base);

    // Отправляем результат обратно 
    write(fdout[1], dec2.c_str(),len);
    write(fdout[1], &decimal, sizeof(decimal));

    delete[] number;
}
void helpFoo(){
    cout << "Эта программа позволяет конвертировать числа из произвольной системы счисления в десятичную и обратно." << endl;
    cout << "Формат команды: --file [ИМЯ_ВХОДНОГО_ФАЙЛА] [ИМЯ_ВЫХОДНОГО_ФАЙЛА]" << endl;

}
void start() {
    int pid;
    int fdin[2];
    int fdout[2];
    try{       

        if (pipe(fdin)== -1 or pipe(fdout) == -1) {
            runtime_error("ошибка");
        }

        pid = fork();

        if (pid == -1) {
            runtime_error("ошибка, копия процесса не была создана");
        }
        else if (pid>0) { 
            parent_process(fdin, fdout);
        }    
        else  if (pid == 0) { 
            child_process(fdin, fdout);
        }
    }
    catch(exception &ex)
    {
        cerr << "Error: " << ex.what() << endl;
    }
    for (int i = 0; i < 2; ++i)
    {
        close(fdin[i]);
        close(fdout[i]);
    }
}

int main(int argc, char* argv[]){

    if(argc == 2 && !strcmp(argv[1], "--help"))
    {
        helpFoo();
    }
    else if(argc == 4 && !strcmp(argv[1], "--file"))
    {
        fileFlag = true;
        input_filename = argv[2];
        output_filename = argv[3];
        start();
    }else
    {
        fileFlag = false;
        start();
    }

    return 0;
}