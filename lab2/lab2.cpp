
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
    if (!stat(file, &info)) {//����� ���������� ����� � ����

        cout << "File size: " << info.st_size << " bytes" << endl;
        cout << "Last access: " << ctime(&info.st_atime);
        cout << "Last modification: " << ctime(&info.st_mtime);
        cout << "Last state change: " << ctime(&info.st_ctime);//��������� ����� �������

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
    if (!strcmp(name1, name2)) {// ���������� �������� ��� ���� � ������
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

    // ����������� ������ �� ������ ����� � ������
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

    in.close(); // ��������� ����

    // ��������� ����� ����� ��� ����
    const char* file = strrchr(filename, '/'); // ���� ������ � ����� /
    if (!file) {// ���� ����������� ���������� �� ������, ��� ����� ���������� ����� ����� ���������� �������
        file = filename;
    }
    else {
        file++; // ���������� ��������� /
    }

    string newfilename = string(dirname) + "/" + string(file);//����� ���� � ��� �����


    if (rename(filename, newfilename.c_str())) {// ������ ���� � �����
        cout << "Failed to move file!" << endl;
        return;
    }

    cout << "File has been moved successfully!" << endl;

}
string getAccess(char mode) {

    switch (mode) {
    case '0': return "---"; // ��� ����
    case '1': return "--x"; // ����� �� ������ �����
    case '2': return "-w-"; // ����� �� ������ � ��������� �����
    case '3': return "-wx"; // ����� �� ������ � ��������� �����
    case '4': return "r--"; // ����� �� ������ �����
    case '5': return "r-x"; // ����� �� ������ � ������ �����
    case '6': return "rw-"; // ����� �� ������ � ��������� �����
    case '7': return "rwx"; // ����� �� ������, ��������� � ������ �����

    }
    return " ";
}
mode_t getMasks(const char* mode) {
    mode_t mode_acs = 0;// ����� ������� � �����
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
    if (regex_match(mode, digitType)) {// ���� ����� ������� �� ����

        for (int i = 0; i < 3; ++i)
            access += getAccess(mode[i]);
        const char* access_cstr = access.c_str();

        mode_t md = getMasks(access_cstr);

        if (chmod(file, md))
            cout << ("Error") << endl;
        else
            cout << "Mode changed" << endl;

    }
    else if (regex_match(mode, symbolType))// ���� ����� ������� �� ���� 
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
