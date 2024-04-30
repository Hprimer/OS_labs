#include "MyForm.h"
#include <Windows.h>

using namespace System::Windows::Forms;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Сlient::MyForm form;//название формы 
    Application::Run(% form);//запуск формы
    return 0;
}