#include "MyForm.h"
#include <Windows.h>

using namespace System::Windows::Forms;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    �lient::MyForm form;//�������� ����� 
    Application::Run(% form);//������ �����
    return 0;
}