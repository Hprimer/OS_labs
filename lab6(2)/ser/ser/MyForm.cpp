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
//	Server::MyForm form;//�������� ����� 
//	Application::Run(% form);//������ �����
//
//}

#include "MyForm.h"
#include <Windows.h>

//using namespace Server;
using namespace System::Windows::Forms;
//namespace Server {
//    MyForm::MyForm() {
//        // ����������� �����
//    }

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    //Application::Run(gcnew MyForm);
    Server::MyForm form;//�������� ����� 
    Application::Run(% form);//������ �����
    return 0;
}

