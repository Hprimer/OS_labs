
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <regex>

using namespace std;

void helpFoo() {
    cout << "  --copy [PATH TO FILE] [PATH TO NEW FILE]" << endl;
    cout << "  --move [PATH TO FILE] [PATH TO NEW DIRECTIRY]" << endl;
    cout << "  --info [PATH TO FILE]" << endl;
    cout << "  --chmod [PATH TO FILE] [MODE]" << endl;
}
void infoFoo(char* file) {
    struct stat info;
    if (!stat(file, &info)) {//сразу записывает стату в инфо

        cout << "File size: " << info.st_size << " bytes" << endl;
        cout << "Last access: " << ctime(&info.st_atime);
        cout << "Last modification: " << ctime(&info.st_mtime);
        cout << "Last state change: " << ctime(&info.st_ctime);//последняя смена статуса

    }
    else {
        cout << "Error input filename\n" << endl;
    }
}
void copyFoo(char* name1, char* name2) {
    if (!filesystem::exists(name1)) {
        cout << "File does not exist" << endl;
        return;
    }
    if (!strcmp(name1, name2)) {// Одинаковые названия или пути к файлам
        cout << "Files or file paths are equal\n";
        return;
    }


    ifstream infile(name1, ios::binary);
    if (!infile.is_open()) {
        cout << "error opening 1 file" << endl;
        return;
    }
    ofstream outfile(name2, ios::binary);
    if (!outfile.is_open()) {
        cout << "error opening 2 file" << endl;
        infile.close();
        return;
    }

    // Копирование данных из одного файла в другой
    const int buffer_size = 4;
    vector<char> buffer(buffer_size);
    while (infile.read(buffer.data(), buffer_size)) {
        outfile.write(buffer.data(), infile.gcount());
    }

    cout << "File has been copied successfully!" << endl;

    infile.close();
    outfile.close();

}

void moveFoo(const char* filename, const char* dirname) {
    ifstream in(filename, ios::binary);
    if (!in.is_open()) {
        cout << "File doesn't exist!";
        return;
    }

    in.close(); // Закрываем файл

    // Получение имени файла без пути
    const char* file = strrchr(filename, '/'); // Ищем первый с конца /
    if (!file) {// Если разделитель директорий не найден, имя файла начинается сразу после последнего символа
        file = filename;
    }
    else {
        file++; // Пропускаем найденный /
    }

    string newfilename = string(dirname) + "/" + string(file);//новый путь и имя файла


    if (rename(filename, newfilename.c_str())) {// меняем путь к файлу
        cout << "Failed to move file!" << endl;
        return;
    }

    cout << "File has been moved successfully!" << endl;

}
string getAccess(char mode) {

    switch (mode) {
    case '0': return "---"; // Нет прав
    case '1': return "--x"; // Права на запуск файла
    case '2': return "-w-"; // Права на запись и изменение файла
    case '3': return "-wx"; // Права на запуск и изменение файла
    case '4': return "r--"; // Права на чтение файла
    case '5': return "r-x"; // Права на чтение и запуск файла
    case '6': return "rw-"; // Права на чтение и изменение файла
    case '7': return "rwx"; // Права на чтение, изменение и запуск файла

    }
    return " ";
}
mode_t getMasks(const char* mode) {
    mode_t mode_acs = 0;// Режим доступа к файлу
    int i;
    for (i = 0; i < 3; ++i) {
        switch (mode[i]) {
        case 'r':
            mode_acs |= S_IRUSR;
            break;
        case 'w':
            mode_acs |= S_IWUSR;
            break;
        case 'x':
            mode_acs |= S_IXUSR;
            break;
        }
    }


    for (; i < 6; ++i) {
        switch (mode[i]) {
        case 'r':
            mode_acs |= S_IRGRP;
            break;
        case 'w':
            mode_acs |= S_IWGRP;
            break;
        case 'x':
            mode_acs |= S_IXGRP;
            break;

        }
    }

    for (; i < 9; ++i) {
        switch (mode[i]) {
        case 'r':
            mode_acs |= S_IROTH;
            break;
        case 'w':
            mode_acs |= S_IWOTH;
            break;
        case 'x':
            mode_acs |= S_IXOTH;
            break;
        }
    }
    return mode_acs;
}
void chmodFoo(const char* mode, const char* file) {
    regex digitType("([01324567])([01324567])([01324567])");
    regex symbolType("([r-])([w-])([x-])([r-])([w-])([x-])([r-])([w-])([x-])");
    string access;
    if (regex_match(mode, digitType)) {// Если режим доступа из цифр

        for (int i = 0; i < 3; ++i)
            access += getAccess(mode[i]);
        const char* access_cstr = access.c_str();

        mode_t md = getMasks(access_cstr);

        if (chmod(file, md))
            cout << ("Error") << endl;
        else
            cout << "Mode changed" << endl;

    }
    else if (regex_match(mode, symbolType))// Если режим доступа из букв 
    {
        mode_t md = getMasks(mode);

        if (chmod(file, md))
            cout << ("Error") << endl;
        else
            cout << "Mode changed" << endl;
    }
    else {
        cout << ("Error") << endl;
    }

}


int main(int arg, char* args[]) {
    if (arg == 2 && !strcmp(args[1], "--help")) {
        helpFoo();
    }
    else if (arg == 3 && !strcmp(args[1], "--info")) {
        infoFoo(args[2]);
    }
    else if (arg == 4 && !strcmp(args[1], "--copy")) {
        copyFoo(args[2], args[3]);
    }
    else if (arg == 4 && !strcmp(args[1], "--move")) {
        moveFoo(args[2], args[3]);
    }
    else if (arg == 4 && !strcmp(args[1], "--chmode")) {
        chmodFoo(args[2], args[3]);
    }
    else cout << "ERROR input\nUse --help\n";;


    return 0;
}
