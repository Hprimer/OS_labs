//#pragma once
//
//namespace Client {
//
//	using namespace System;
//	using namespace System::ComponentModel;
//	using namespace System::Collections;
//	using namespace System::Windows::Forms;
//	using namespace System::Data;
//	using namespace System::Drawing;
//
//	/// <summary>
//	/// Сводка для MyForm
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
//		/// Освободить все используемые ресурсы.
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
//		/// Обязательная переменная конструктора.
//		/// </summary>
//		System::ComponentModel::Container ^components;
//
//#pragma region Windows Form Designer generated code
//		/// <summary>
//		/// Требуемый метод для поддержки конструктора — не изменяйте 
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
//			this->ClientSize = System::Drawing::Size(284, 260);
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
#include <fstream>

HANDLE pipe;
std::ofstream file;

namespace Сlient {

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
            CloseHandle(pipe);
            file << "Log ended!\n";

        }

    protected:
    private: System::Windows::Forms::TextBox^ textBox1;
    private: System::Windows::Forms::Button^ button1;
    private: System::Windows::Forms::NumericUpDown^ numericUpDown1;
    private: System::Windows::Forms::ListBox^ listBox1;
    private: System::Windows::Forms::Button^ button2;
    private: System::Windows::Forms::Button^ button3;
    private: System::Windows::Forms::Button^ button4;
    private: System::Windows::Forms::Button^ button5;
    private: System::Windows::Forms::Label^ label1;
    private: System::Windows::Forms::Label^ label2;










    private: System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
           void InitializeComponent(void)
           {
               this->textBox1 = (gcnew System::Windows::Forms::TextBox());
               this->button1 = (gcnew System::Windows::Forms::Button());
               this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
               this->listBox1 = (gcnew System::Windows::Forms::ListBox());
               this->button2 = (gcnew System::Windows::Forms::Button());
               this->button3 = (gcnew System::Windows::Forms::Button());
               this->button4 = (gcnew System::Windows::Forms::Button());
               this->button5 = (gcnew System::Windows::Forms::Button());
               this->label1 = (gcnew System::Windows::Forms::Label());
               this->label2 = (gcnew System::Windows::Forms::Label());
               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->BeginInit();
               this->SuspendLayout();
               // 
               // textBox1
               // 
               this->textBox1->BackColor = System::Drawing::SystemColors::ActiveCaption;
               this->textBox1->ForeColor = System::Drawing::Color::GhostWhite;
               this->textBox1->Location = System::Drawing::Point(27, 34);
               this->textBox1->Multiline = true;
               this->textBox1->Name = L"textBox1";
               this->textBox1->ReadOnly = true;
               this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
               this->textBox1->Size = System::Drawing::Size(387, 323);
               this->textBox1->TabIndex = 1;
               this->textBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
               // 
               // button1
               // 
               this->button1->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
               this->button1->Location = System::Drawing::Point(570, 107);
               this->button1->Name = L"button1";
               this->button1->Size = System::Drawing::Size(125, 65);
               this->button1->TabIndex = 2;
               this->button1->Text = L"Send";
               this->button1->UseVisualStyleBackColor = false;
               this->button1->Click += gcnew System::EventHandler(this, &MyForm::btn_send_Click);
               // 
               // numericUpDown1
               // 
               this->numericUpDown1->DecimalPlaces = 5;
               this->numericUpDown1->Location = System::Drawing::Point(570, 72);
               this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1215752192, 23, 0, 0 });
               this->numericUpDown1->Name = L"numericUpDown1";
               this->numericUpDown1->Size = System::Drawing::Size(125, 29);
               this->numericUpDown1->TabIndex = 3;
               this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &MyForm::numericUpDown1_ValueChanged);
               // 
               // listBox1
               // 
               this->listBox1->FormattingEnabled = true;
               this->listBox1->ItemHeight = 24;
               this->listBox1->Items->AddRange(gcnew cli::array< System::Object^  >(4) { L"+", L"-", L"*", L"/" });
               this->listBox1->Location = System::Drawing::Point(433, 72);
               this->listBox1->Name = L"listBox1";
               this->listBox1->Size = System::Drawing::Size(125, 100);
               this->listBox1->TabIndex = 4;
               this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::listBox1_SelectedIndexChanged);
               // 
               // button2
               // 
               this->button2->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
               this->button2->Location = System::Drawing::Point(433, 215);
               this->button2->Name = L"button2";
               this->button2->Size = System::Drawing::Size(125, 68);
               this->button2->TabIndex = 5;
               this->button2->Text = L"Calculate";
               this->button2->UseVisualStyleBackColor = false;
               this->button2->Click += gcnew System::EventHandler(this, &MyForm::btn_calc_Click);
               // 
               // button3
               // 
               this->button3->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
               this->button3->Location = System::Drawing::Point(570, 289);
               this->button3->Name = L"button3";
               this->button3->Size = System::Drawing::Size(125, 68);
               this->button3->TabIndex = 6;
               this->button3->Text = L"Check";
               this->button3->UseVisualStyleBackColor = false;
               this->button3->Click += gcnew System::EventHandler(this, &MyForm::btn_check_Click);
               // 
               // button4
               // 
               this->button4->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
               this->button4->Location = System::Drawing::Point(570, 215);
               this->button4->Name = L"button4";
               this->button4->Size = System::Drawing::Size(125, 68);
               this->button4->TabIndex = 7;
               this->button4->Text = L"Clear";
               this->button4->UseVisualStyleBackColor = false;
               this->button4->Click += gcnew System::EventHandler(this, &MyForm::btn_clear_Click);
               // 
               // button5
               // 
               this->button5->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
               this->button5->Location = System::Drawing::Point(433, 289);
               this->button5->Name = L"button5";
               this->button5->Size = System::Drawing::Size(125, 68);
               this->button5->TabIndex = 8;
               this->button5->Text = L"Exit";
               this->button5->UseVisualStyleBackColor = false;
               this->button5->Click += gcnew System::EventHandler(this, &MyForm::btn_exit_Click);
               // 
               // label1
               // 
               this->label1->AutoSize = true;
               this->label1->Location = System::Drawing::Point(433, 34);
               this->label1->Name = L"label1";
               this->label1->Size = System::Drawing::Size(112, 24);
               this->label1->TabIndex = 9;
               this->label1->Text = L"Operations";
               this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
               // 
               // label2
               // 
               this->label2->AutoSize = true;
               this->label2->Location = System::Drawing::Point(566, 34);
               this->label2->Name = L"label2";
               this->label2->Size = System::Drawing::Size(64, 24);
               this->label2->TabIndex = 10;
               this->label2->Text = L"Value";
               this->label2->Click += gcnew System::EventHandler(this, &MyForm::label2_Click);
               // 
               // MyForm
               // 
               this->AutoScaleDimensions = System::Drawing::SizeF(12, 24);
               this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
               this->BackColor = System::Drawing::SystemColors::ActiveBorder;
               this->ClientSize = System::Drawing::Size(707, 390);
               this->Controls->Add(this->label2);
               this->Controls->Add(this->label1);
               this->Controls->Add(this->button5);
               this->Controls->Add(this->button4);
               this->Controls->Add(this->button3);
               this->Controls->Add(this->button2);
               this->Controls->Add(this->numericUpDown1);
               this->Controls->Add(this->button1);
               this->Controls->Add(this->textBox1);
               this->Controls->Add(this->listBox1);
               this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->Margin = System::Windows::Forms::Padding(6, 5, 6, 5);
               this->Name = L"MyForm";
               this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
               this->Text = L"Client";
               this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->EndInit();
               this->ResumeLayout(false);
               this->PerformLayout();

           }
#pragma endregion
    private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e)
    {
        file.open("C:\\Users\\наиль\\Desktop\\уник\\4сем\\OS_labs\\lab6(2)\\log.txt", std::fstream::in | std::fstream::out | std::fstream::app);
        file << "Log started!\n";
        pipe = CreateFile(L"\\\\.\\pipe\\mypipe", GENERIC_READ | GENERIC_WRITE, 0, NULL,
            OPEN_EXISTING, 0, NULL);
        if (!pipe) {
            textBox1->AppendText("Server connection error!");
            file << "Server connection error!\n";
            textBox1->Update();
            textBox1->Invalidate();
            return;
        }
    }
    private: System::Void numericUpDown1_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
    }
    private: System::Void btn_send_Click(System::Object^ sender, System::EventArgs^ e)
    {

        if (!listBox1->SelectedItem) {
            String^ message = "Arithmetic operation not selected";
            String^ caption = "Please, selected arithmetic operation!";
            MessageBoxButtons buttons = MessageBoxButtons::OK;
            System::Windows::Forms::DialogResult result;

            //Displays the MessageBox
            result = MessageBox::Show(this, caption, message);
            return;
        }
        double value = (double)numericUpDown1->Value;//число 
        String^ str = listBox1->GetItemText(listBox1->SelectedItem);//операция
        str += value.ToString();
        char buf[256];
        DWORD dwBytes;
        for (int i = 0; i < str->Length; i++) //копирование строки в буфер
            buf[i] = str[i];
        buf[str->Length] = 0;//нуль терминатор
        WriteFile(pipe, buf, sizeof(buf), &dwBytes, NULL);  // отправляем строку на сервер;
        ReadFile(pipe, &buf, sizeof(buf), &dwBytes, NULL);  // получаем ответ с сервера;
        String^ output = gcnew String(buf);
        textBox1->Text = "";
        textBox1->AppendText(output);
        textBox1->AppendText("\n");
        file << buf << "\n";
        textBox1->Update();
        textBox1->Invalidate();

    }
    private: System::Void btn_calc_Click(System::Object^ sender, System::EventArgs^ e)
    {
        char buf[256];
        DWORD dwBytes;
        String^ str = gcnew String("Calc");
        for (int i = 0; i < str->Length; i++)
            buf[i] = str[i];
        buf[str->Length] = 0;
        WriteFile(pipe, buf, sizeof(buf), &dwBytes, NULL);  //отправляем строку на сервер;
        ReadFile(pipe, &buf, sizeof(buf), &dwBytes, NULL);  //получаем ответ с сервера;
        String^ output = gcnew String(buf);
        textBox1->Text = "";
        textBox1->AppendText(output);
        file << buf << "\n";
        textBox1->AppendText("\n");
        textBox1->Update();
        textBox1->Invalidate();
    }
    private: System::Void btn_clear_Click(System::Object^ sender, System::EventArgs^ e)
    {
        char buf[256];
        DWORD dwBytes;
        String^ str = gcnew String("Clear");
        for (int i = 0; i < str->Length; i++) 
            buf[i] = str[i];
        buf[str->Length] = 0;
        WriteFile(pipe, buf, sizeof(buf), &dwBytes, NULL);  //отправляем строку на сервер;
        ReadFile(pipe, &buf, sizeof(buf), &dwBytes, NULL);  //получаем ответ с сервера;
        String^ output = gcnew String(buf);
        textBox1->Text = "";
        textBox1->AppendText(output);
        file << buf << "\n";
        textBox1->AppendText("\n");
        textBox1->Update();
        textBox1->Invalidate();
    }
    private: System::Void btn_check_Click(System::Object^ sender, System::EventArgs^ e)
    {
        char buf[256];
        DWORD dwBytes;
        String^ str = gcnew String("Check");
        for (int i = 0; i < str->Length; i++) 
            buf[i] = str[i];
        buf[str->Length] = 0;
        WriteFile(pipe, buf, sizeof(buf), &dwBytes, NULL);  //отправляем строку на сервер;
        ReadFile(pipe, &buf, sizeof(buf), &dwBytes, NULL);  //получаем ответ с сервера;
        String^ output = gcnew String(buf);
        textBox1->Text = "";
        textBox1->AppendText(output);
        file << buf << "\n";
        textBox1->AppendText("\n");
        textBox1->Update();
        textBox1->Invalidate();
    }
    private: System::Void btn_exit_Click(System::Object^ sender, System::EventArgs^ e)
    {
        char buf[256];
        DWORD dwBytes;
        String^ str = gcnew String("Exit");
        for (int i = 0; i < str->Length; i++) 
            buf[i] = str[i];
        buf[str->Length] = 0;
        textBox1->Text = "";
        WriteFile(pipe, buf, sizeof(buf), &dwBytes, NULL);  //отправляем строку на сервер;
        ReadFile(pipe, &buf, sizeof(buf), &dwBytes, NULL);  //получаем ответ с сервера;
        String^ output = gcnew String(buf);
        textBox1->AppendText(output);
        file << buf << "\n";
        textBox1->AppendText("\n");
        textBox1->Update();
        textBox1->Invalidate();
    }


    private: System::Void label2_Click(System::Object^ sender, System::EventArgs^ e) {
    }
    private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
    }
    private: System::Void listBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
    }
    private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
    }
};
}

