//#pragma once
//
//namespace Server {
//
//	using namespace System;
//	using namespace System::ComponentModel;
//	using namespace System::Collections;
//	using namespace System::Windows::Forms;
//	using namespace System::Data;
//	using namespace System::Drawing;
//
//	/// <summary>
//	/// —водка дл€ MyForm
//	/// </summary>
//	public ref class MyForm : public System::Windows::Forms::Form
//	{
//	public:
//		MyForm(void)
//		{
//			InitializeComponent();
//			//
//			//TODO: добавьте код конструктора
//			//
//		}
//
//	protected:
//		/// <summary>
//		/// ќсвободить все используемые ресурсы.
//		/// </summary>
//		~MyForm()
//		{
//			if (components)
//			{
//				delete components;
//			}
//		}
//
//	private:
//		/// <summary>
//		/// ќб€зательна€ переменна€ конструктора.
//		/// </summary>
//		System::ComponentModel::Container ^components;
//
//#pragma region Windows Form Designer generated code
//		/// <summary>
//		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
//		/// содержимое этого метода с помощью редактора кода.
//		/// </summary>
//		void InitializeComponent(void)
//		{
//			this->SuspendLayout();
//			// 
//			// MyForm
//			// 
//			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
//			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
//			this->ClientSize = System::Drawing::Size(297, 420);
//			this->Name = L"MyForm";
//			this->Text = L"MyForm";
//			this->ResumeLayout(false);
//
//		}
//#pragma endregion
//	};
//}


#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>


double strToNum(std::string input, bool* chislo)  //перевод из строки в число;
{
	bool min = false;//минус
	int p = -1;  //дл€ дробных чисел;
	double num = 0;
	input.erase(input.begin(), input.begin() + 1);
	for (int i = 0; i < input.length() && *chislo; i++)
	{
		if (input[i] == ',' && i) {
			p = i;
		}
		else if (input[i] >= '0' && input[i] <= '9') {
			if (p == -1)
				num = num * 10 + (input[i] - '0');
			else
				num += (input[i] - '0') * pow(10, p - i);
		}
		else *chislo = false;
	}
	if (input[0] == '-') {
		min = true;
		input.erase(input.begin(), input.begin() + 1);
	}
	if (min)
		num *= -1;
	return num;
}

namespace Server {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();  //TODO: Add the constructor code here
		}

	protected:

		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Button^ btn_start;
	protected:

	private: System::Windows::Forms::Label^ label1;
	//private: System::Windows::Forms::Button^ buttonExit;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->btn_start = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->textBox1->ForeColor = System::Drawing::SystemColors::ControlText;
			this->textBox1->Location = System::Drawing::Point(12, 32);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox1->Size = System::Drawing::Size(290, 357);
			this->textBox1->TabIndex = 0;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
			// 
			// btn_start
			// 
			this->btn_start->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->btn_start->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btn_start->Location = System::Drawing::Point(12, 395);
			this->btn_start->Name = L"btn_start";
			this->btn_start->Size = System::Drawing::Size(290, 45);
			this->btn_start->TabIndex = 1;
			this->btn_start->Text = L"Start";
			this->btn_start->UseVisualStyleBackColor = false;
			this->btn_start->Click += gcnew System::EventHandler(this, &MyForm::btn_start_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(29, -1);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(100, 24);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Event Log:";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(11, 24);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->ClientSize = System::Drawing::Size(322, 452);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btn_start);
			this->Controls->Add(this->textBox1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->Margin = System::Windows::Forms::Padding(5, 4, 5, 4);
			this->Name = L"MyForm";
			this->Text = L"Server";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
	}

//////////////////
	//private: System::Void buttonExit_Click(System::Object^ sender, System::EventArgs^ e)
	//{
	//	Application::Exit(); // «авершить приложение
	//}
///////////////////////////////
	private: System::Void btn_start_Click(System::Object^ sender, System::EventArgs^ e)
	{
		std::ofstream file;

		file.open("C:\\Users\\наиль\\Desktop\\уник\\4сем\\OS_labs\\lab6(2)\\log.txt", 
			std::fstream::in | std::fstream::out | std::fstream::app);
		file << "Log started!\n";
		char buf[256];
		DWORD dwBytes;
		std::string input = "";
		std::vector<int> oper;  //вектор дл€ операций;
		std::vector<double> number;  //вектор дл€ чисел;
		std::string kekwait = "*/+-"; //возможные операции;
		HANDLE pipe = CreateNamedPipe(L"\\\\.\\pipe\\mypipe", 
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES, 1, 1, 0, NULL); //создаЄм канал;

		if (pipe == INVALID_HANDLE_VALUE)
		{
			textBox1->AppendText("Error occured creating pipe!\n");
			return;
		}
		system("start C:\\Users\\наиль\\Desktop\\уник\\4сем\\OS_labs\\lab6(2)\\cli\\x64\\Debug\\cli.exe");

		if (!ConnectNamedPipe(pipe, NULL))
		{
			textBox1->AppendText("Error connection pipe!\n");
			CloseHandle(pipe);
			return;
		}

		while (input != "Finished!")
		{
			ReadFile(pipe, &buf, sizeof(buf), &dwBytes, NULL);  //считываем строку;
			if (GetLastError() == ERROR_BROKEN_PIPE)
			{
				textBox1->AppendText("Client disconnected!\n");
				return;
			}
			std::string str(buf);
			textBox1->AppendText("Server recieved\r\n");
			file << "Server recieved\r\n";
			textBox1->Update();
			textBox1->Invalidate();
			input = str;
			if (input == "Calc")  //считываем выражение;
			{
				textBox1->AppendText("Calculate equation\r\n");
				textBox1->Update();
				textBox1->Invalidate();
				file << "Calculate equation\r\n";
				//bool zero = 0;
				//for (int i = 0; i < number.size(); i++)
				//	if (number[i] == 0 && oper[i] == 1)
				//		zero = 1;
				//if (zero) 
				//	input = "Division by 0";
				bool incorrect = false;//проверка на корректность 
				for (int i = 0; i < number.size(); i++)
					if (oper[0] == 0 || oper[0] == 1 || (number[i] == 0 && oper[i] == 1))
						incorrect = true;					
				if (incorrect)
					input = "Incorrect expression";
				else
				{
					double curr = 0;
					double answer = 0;
					for (int i = 0; i < number.size(); i++) {
						if (oper[i] == 0) curr *= number[i];
						else if (oper[i] == 1 && number[i] != 0) 
							curr /= number[i];
						else if (oper[i] == 2) {
							answer += curr;
							curr = number[i];
						}
						else if (oper[i] == 3) {
							answer += curr;
							curr = -1 * number[i];
						}
						std::string ans = std::to_string(answer + curr);
						input = "Answer: " + ans;
					}
				}
				//////////////
				number.clear();
				oper.clear();
				///////////////
			}
			else if (input == "Check")  //проверка делени€ на 0;
			{
				textBox1->AppendText("Check division by zero\r\n");
				textBox1->Update();
				textBox1->Invalidate();
				file << "Check division by zero\r\n";
				/*bool zero = 0;
				for (int i = 0; i < number.size(); i++)
					if (number[i] == 0 && oper[i] == 1)
						zero = 1;
				if (zero)
					input = "Division by 0!\r\n";
				else 
					input = "No division by 0\r\n";*/
				bool incorrect = false;//проверка на корректность 
				for (int i = 0; i < number.size(); i++)
					if (oper[0] == 0 || oper[0] == 1 || (number[i] == 0 && oper[i] == 1))
						incorrect = true;
				if (incorrect)
					input = "Incorrect expression\r\n";
				else
					input = "Correct expression\r\n";
			}
			else if (input == "Clear")  //дл€ очищени€ формулы;
			{
				textBox1->AppendText("Clear equation\r\n");
				textBox1->Update();
				textBox1->Invalidate();
				file << "Clear equation\r\n";
				number.clear();
				oper.clear();
				input = "Cleared!";
			}
			else if (input == "Exit")  //завершение работы клиента;
			{
				textBox1->AppendText("Finish working\r\n");
				textBox1->Update();
				textBox1->Invalidate();
				file << "Finish working\r\n";
				input = "Finished!";
			}
			else
			{
				String^ str = gcnew String(buf);
				textBox1->AppendText(str);
				textBox1->AppendText("\r\n");
				textBox1->Update();
				file << buf << "\r\n";
				textBox1->Invalidate();
				bool ok = true;
				double num = strToNum(input, &ok);
				if (!ok)
					input = "Incorrect operation!";
				else
				{
					number.push_back(num);
					switch (input[0]) {
					case '*': {oper.push_back(0); break; }
					case '/': {oper.push_back(1); break; }
					case '+': {oper.push_back(2); break; }
					case '-': {oper.push_back(3); break; }
					default: {
						ok = false;
						number.pop_back();
						break; }
					}
					if (!ok) 
						input = "Incorrect operation!";
					else
					{
						input.clear();
						for (int i = 0; i < number.size(); i++)  //выражение целиком
							input += std::string(1, kekwait[oper[i]]) +	std::to_string(number[i]);
					}
				}
			}
			for (int i = 0; i < input.length(); i++)//копирование в буфер
				buf[i] = input[i];
			buf[input.length()] = 0;

			WriteFile(pipe, buf, sizeof(buf), &dwBytes, NULL);
		}
		file << "Log ended!";
		file.close();
		DisconnectNamedPipe(pipe);  //отключаемс€ от канала;

	}
	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	}

private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
}
};
}