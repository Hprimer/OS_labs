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
//	Client::MyForm form;//�������� ����� 
//	Application::Run(% form);//������ �����
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
    �lient::MyForm form;//�������� ����� 
    Application::Run(% form);//������ �����
    return 0;
}