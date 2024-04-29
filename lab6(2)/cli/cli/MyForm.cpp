//#include "MyForm.h"
//
//using namespace System;
//using namespace System::Windows::Forms;
//
//[STAThreadAttribute]
//
//void main(array<String^>^ args) {
//	Application::EnableVisualStyles();
//	Application::SetCompatibleTextRenderingDefault(false);
//
//	Client::MyForm form;//название формы 
//	Application::Run(% form);//запуск формы
//
//}


#include "MyForm.h"
#include <Windows.h>

//using namespace Server;
using namespace System::Windows::Forms;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    //Application::Run(gcnew MyForm);
    Сlient::MyForm form;//название формы 
    Application::Run(% form);//запуск формы
    return 0;
}