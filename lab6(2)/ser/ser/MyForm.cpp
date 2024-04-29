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
//	Server::MyForm form;//название формы 
//	Application::Run(% form);//запуск формы
//
//}

#include "MyForm.h"
#include <Windows.h>

//using namespace Server;
using namespace System::Windows::Forms;
//namespace Server {
//    MyForm::MyForm() {
//        // Конструктор формы
//    }

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    //Application::Run(gcnew MyForm);
    Server::MyForm form;//название формы 
    Application::Run(% form);//запуск формы
    return 0;
}

