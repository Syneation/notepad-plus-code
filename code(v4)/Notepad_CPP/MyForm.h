#pragma once

#include <tchar.h>
#include <Windows.h>
#include <WindowsX.h>

#include "MyForm1.h"	
#include "MyForm2.h"
#include "MyForm3.h"
#include "PrintHelper.cpp"
#include "helper.h"

namespace NotepadCPP {

	using namespace System;
	using namespace System::IO;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections;
	using namespace System::ComponentModel;
	using namespace System::Windows::Forms;

	/// <summary>
	/// —водка дл€ MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	private:
		System::Windows::Forms::Timer^ typingTimer;
		System::Windows::Forms::ToolStripMenuItem^ colorBGToolStripMenuItem;
		System::Windows::Forms::ToolStripButton^ toolStripButton2;
		System::Drawing::Color savedBackColor;
		System::Windows::Forms::ToolStripMenuItem^ resetTheTextSettingsToolStripMenuItem;


		System::Windows::Forms::ToolStripMenuItem^ settingToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^ lineBreakCheck;
	private: System::Windows::Forms::ToolStripMenuItem^ themeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ whiteToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ blackToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ themeToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^ whiteToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^ blackToolStripMenuItem1;
	private: System::Windows::Forms::Label^ encoding;
	private: System::Windows::Forms::ToolStripButton^ fillText;


	private: System::Windows::Forms::ToolStripButton^ centerTextBtn;
	private: System::Windows::Forms::ToolStripButton^ leftTextBtn;
	private: System::Windows::Forms::ToolStripButton^ rightTextBtn;
	private: System::Windows::Forms::ToolStripMenuItem^ resetTextSettingsToolStripMenuItem;

	//////////////////////////////////////
	///////for DragDrop//////////////////
	////////////////////////////////////
	private:
		System::Void richTextBox1_DragEnter(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
		{
			if (e->Data->GetDataPresent(DataFormats::FileDrop))
				e->Effect = DragDropEffects::Copy;
			else
				e->Effect = DragDropEffects::None;
		}

		System::Void richTextBox1_DragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
		{
			helperRich HRich;

			try
			{
				array<String^>^ files = (array<String^>^)e->Data->GetData(DataFormats::FileDrop);

				if (files->Length > 0)
				{
					helperRich HRich;

					String^ filePath = files[0];
					String^ extension = Path::GetExtension(filePath)->ToLower();

					if (extension == ".txt" || extension == ".rtf")
					{
						if (extension == ".txt")
						{
							// TXT
							StreamReader^ reader = gcnew StreamReader(filePath, true);
							String^ content = reader->ReadToEnd();
							reader->Close();

							richTextBox1->Text = content;
						}
						else // RTF
						{
							richTextBox1->LoadFile(filePath, RichTextBoxStreamType::RichText);
						}

						nameFile->Text = filePath;
						this->Text = "Notepad+ - " + Path::GetFileName(filePath);
						encoding->Text = "Encoding: " + HRich.DetectEncodingSimple(filePath);
						notifications->Text = "File loaded successfully";
					}
					else
					{
						// trying to open a file as a text file
						try
						{
							StreamReader^ reader = gcnew StreamReader(filePath, true);
							String^ content = reader->ReadToEnd();
							reader->Close();

							richTextBox1->Text = content;
							nameFile->Text = Path::GetFileName(filePath);
							this->Text = "Notepad+ - " + nameFile->Text;
							notifications->Text = "File loaded as text";
							encoding->Text = "Encoding: " + HRich.DetectEncodingSimple(filePath);
						}
						catch (Exception^ ex)
						{
							nameFile->Text = "None";
							MessageBox::Show("Cannot open file: " + ex->Message,
								"Error",
								MessageBoxButtons::OK,
								MessageBoxIcon::Error);
						}
					}
				}
			}
			catch (Exception^ ex)
			{
				MessageBox::Show("Error opening file: " + ex->Message,
					"Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error);
			}
		}


	bool checkAutoSave = false;
	

	public:

		MyForm(void)
		{
			InitializeComponent();

			// Set up keyboard event handler
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyDown);
			this->KeyPreview = true;

			//rich Text Box
			this->AllowDrop = true;
			richTextBox1->AllowDrop = true;
			richTextBox1->DragEnter += gcnew DragEventHandler(this, &MyForm::richTextBox1_DragEnter);
			richTextBox1->DragDrop += gcnew DragEventHandler(this, &MyForm::richTextBox1_DragDrop);

			// Initialize auto-save timer
			typingTimer = gcnew System::Windows::Forms::Timer();
			typingTimer->Interval = 1000; // 1 second interval
			typingTimer->Tick += gcnew System::EventHandler(this, &MyForm::OnTypingStopped);
			checkAutoSave = false; // Auto-save disabled by default

		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}

			// Clean up timer
			if (typingTimer != nullptr)
			{
				typingTimer->Stop();
				delete typingTimer;
				typingTimer = nullptr;
			}
		}

	private: System::Windows::Forms::RichTextBox^ richTextBox1;
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^ openFile;
	private: System::Windows::Forms::ToolStripMenuItem^ newFile;
	private: System::Windows::Forms::ToolStripMenuItem^ saveFile;
	private: System::Windows::Forms::ToolStripMenuItem^ saveFileAs;




	private: System::Windows::Forms::ToolStripMenuItem^ editToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ cancelToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ cutToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ copyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ pasteToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ deleteToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ findToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ replaceToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^ selectAllToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ dateAndTimeToolStripMenuItem;
	private: System::Windows::Forms::ToolStrip^ toolStrip1;
	private: System::Windows::Forms::ToolStripButton^ toolStripButton1;
	private: System::Windows::Forms::ToolStripButton^ openFileIcon;
	private: System::Windows::Forms::ToolStripMenuItem^ print;


	private: System::Windows::Forms::ToolStripMenuItem^ aboutToolStripMenuItem;
	private: System::Windows::Forms::ToolStripButton^ toolStripButton3;
	private: System::Windows::Forms::ToolStripButton^ toolStripButton4;
	private: System::Windows::Forms::ToolStripButton^ toolStripButton5;


	private: System::Windows::Forms::ToolStripButton^ toolStripButton6;
	private: System::Windows::Forms::ToolStripButton^ toolStripButton7;
	private: System::Windows::Forms::ToolStripButton^ toolStripButton8;
	private: System::Windows::Forms::ToolStripButton^ toolStripButton9;

	private: System::Windows::Forms::ToolStripButton^ toolStripButton11;
	private: System::Windows::Forms::ToolStripButton^ toolStripButton12;
	private: System::Windows::Forms::ToolStripButton^ toolStripButton13;
	private: System::Windows::Forms::ToolStripButton^ toolStripButton14;
	private: System::Windows::Forms::ToolStripButton^ toolStripButton15;
private: System::Windows::Forms::ToolStripMenuItem^ autoSave;

	private: System::Windows::Forms::ToolStripButton^ toolStripButton16;
	private: System::Windows::Forms::ToolStripMenuItem^ copyrightToolStripMenuItem;


	private: System::Windows::Forms::ToolStripMenuItem^ dateTimeToolStripMenuItem1;
	private: System::Windows::Forms::Label^ infoText;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::Label^ nameFile;
	private: System::Windows::Forms::ToolStripMenuItem^ exit;
	private: System::Windows::Forms::Label^ notifications;
	private: System::Windows::Forms::ToolStripMenuItem^ trademarkToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ deleteAllToolStripMenuItem;
	private: System::Windows::Forms::ToolStripButton^ loup;
	private: System::DirectoryServices::DirectorySearcher^ DirectorySearcher;
	private: System::Windows::Forms::ToolStripButton^ toolStripButtonReaplace;
	private: System::Windows::Forms::FontDialog^ fontDialog1;
	private: System::Windows::Forms::ToolStripButton^ toolStripButtonIncrease;
	private: System::Windows::Forms::ToolStripButton^ toolStripButtonReduce;
	private: System::Windows::Forms::ColorDialog^ colorDialog1;



	protected:


	protected:
	private: System::ComponentModel::IContainer^ components;

	protected:

	protected:

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>


#pragma region 
/// <summary>
/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
/// содержимое этого метода с помощью редактора кода.
/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->newFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveFileAs = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->print = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cancelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->copyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pasteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->deleteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->deleteAllToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->findToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->replaceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->selectAllToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->dateAndTimeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->copyrightToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->trademarkToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->autoSave = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->dateTimeToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->colorBGToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->themeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->whiteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->blackToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resetTheTextSettingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->settingToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->lineBreakCheck = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->themeToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->whiteToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->blackToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resetTextSettingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->openFileIcon = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton3 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton4 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton5 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton6 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton7 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton8 = (gcnew System::Windows::Forms::ToolStripButton());
			this->loup = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButtonReaplace = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton9 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton11 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton12 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton13 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->leftTextBtn = (gcnew System::Windows::Forms::ToolStripButton());
			this->centerTextBtn = (gcnew System::Windows::Forms::ToolStripButton());
			this->rightTextBtn = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton14 = (gcnew System::Windows::Forms::ToolStripButton());
			this->fillText = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton15 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButtonIncrease = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButtonReduce = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton16 = (gcnew System::Windows::Forms::ToolStripButton());
			this->infoText = (gcnew System::Windows::Forms::Label());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->nameFile = (gcnew System::Windows::Forms::Label());
			this->notifications = (gcnew System::Windows::Forms::Label());
			this->DirectorySearcher = (gcnew System::DirectoryServices::DirectorySearcher());
			this->fontDialog1 = (gcnew System::Windows::Forms::FontDialog());
			this->colorDialog1 = (gcnew System::Windows::Forms::ColorDialog());
			this->encoding = (gcnew System::Windows::Forms::Label());
			this->menuStrip1->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// richTextBox1
			// 
			this->richTextBox1->AcceptsTab = true;
			this->richTextBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->richTextBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->richTextBox1->ImeMode = System::Windows::Forms::ImeMode::NoControl;
			this->richTextBox1->Location = System::Drawing::Point(0, 52);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(1052, 410);
			this->richTextBox1->TabIndex = 1;
			this->richTextBox1->Text = L"";
			this->richTextBox1->WordWrap = false;
			this->richTextBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::richTextBox1_TextChanged);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^>(4) {
				this->fileToolStripMenuItem1,
					this->editToolStripMenuItem, this->aboutToolStripMenuItem, this->settingToolStripMenuItem1
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1052, 24);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem1
			// 
			this->fileToolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->openFile,
					this->newFile, this->saveFile, this->saveFileAs, this->print, this->exit
			});
			this->fileToolStripMenuItem1->Name = L"fileToolStripMenuItem1";
			this->fileToolStripMenuItem1->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem1->Text = L"File";
			// 
			// openFile
			// 
			this->openFile->Name = L"openFile";
			this->openFile->Size = System::Drawing::Size(122, 22);
			this->openFile->Text = L"Open";
			this->openFile->Click += gcnew System::EventHandler(this, &MyForm::openToolStripMenuItem_Click);
			// 
			// newFile
			// 
			this->newFile->Name = L"newFile";
			this->newFile->Size = System::Drawing::Size(122, 22);
			this->newFile->Text = L"New File";
			this->newFile->Click += gcnew System::EventHandler(this, &MyForm::newFile_Click);
			// 
			// saveFile
			// 
			this->saveFile->Name = L"saveFile";
			this->saveFile->Size = System::Drawing::Size(122, 22);
			this->saveFile->Text = L"Save";
			this->saveFile->Click += gcnew System::EventHandler(this, &MyForm::saveFile_Click);
			// 
			// saveFileAs
			// 
			this->saveFileAs->Name = L"saveFileAs";
			this->saveFileAs->Size = System::Drawing::Size(122, 22);
			this->saveFileAs->Text = L"Save as...";
			this->saveFileAs->Click += gcnew System::EventHandler(this, &MyForm::saveFileAs_Click);
			// 
			// print
			// 
			this->print->Name = L"print";
			this->print->Size = System::Drawing::Size(122, 22);
			this->print->Text = L"Print";
			this->print->Click += gcnew System::EventHandler(this, &MyForm::print_Click);
			// 
			// exit
			// 
			this->exit->Name = L"exit";
			this->exit->Size = System::Drawing::Size(122, 22);
			this->exit->Text = L"Extra Exit";
			this->exit->Click += gcnew System::EventHandler(this, &MyForm::exit_Click);
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(15) {
				this->cancelToolStripMenuItem,
					this->cutToolStripMenuItem, this->copyToolStripMenuItem, this->pasteToolStripMenuItem, this->deleteToolStripMenuItem, this->deleteAllToolStripMenuItem,
					this->findToolStripMenuItem, this->replaceToolStripMenuItem, this->selectAllToolStripMenuItem, this->dateAndTimeToolStripMenuItem,
					this->autoSave, this->dateTimeToolStripMenuItem1, this->colorBGToolStripMenuItem, this->themeToolStripMenuItem, this->resetTheTextSettingsToolStripMenuItem
			});
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(39, 20);
			this->editToolStripMenuItem->Text = L"Edit";
			// 
			// cancelToolStripMenuItem
			// 
			this->cancelToolStripMenuItem->Name = L"cancelToolStripMenuItem";
			this->cancelToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->cancelToolStripMenuItem->Text = L"Cancel";
			this->cancelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::cancelToolStripMenuItem_Click);
			// 
			// cutToolStripMenuItem
			// 
			this->cutToolStripMenuItem->Name = L"cutToolStripMenuItem";
			this->cutToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->cutToolStripMenuItem->Text = L"Cut";
			this->cutToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::cutToolStripMenuItem_Click);
			// 
			// copyToolStripMenuItem
			// 
			this->copyToolStripMenuItem->Name = L"copyToolStripMenuItem";
			this->copyToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->copyToolStripMenuItem->Text = L"Copy";
			this->copyToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::copyToolStripMenuItem_Click);
			// 
			// pasteToolStripMenuItem
			// 
			this->pasteToolStripMenuItem->Name = L"pasteToolStripMenuItem";
			this->pasteToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->pasteToolStripMenuItem->Text = L"Paste";
			this->pasteToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::pasteToolStripMenuItem_Click);
			// 
			// deleteToolStripMenuItem
			// 
			this->deleteToolStripMenuItem->Name = L"deleteToolStripMenuItem";
			this->deleteToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->deleteToolStripMenuItem->Text = L"Delete";
			// 
			// deleteAllToolStripMenuItem
			// 
			this->deleteAllToolStripMenuItem->Name = L"deleteAllToolStripMenuItem";
			this->deleteAllToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->deleteAllToolStripMenuItem->Text = L"Delete All";
			this->deleteAllToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::deleteAllToolStripMenuItem_Click);
			// 
			// findToolStripMenuItem
			// 
			this->findToolStripMenuItem->Name = L"findToolStripMenuItem";
			this->findToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->findToolStripMenuItem->Text = L"Find...";
			this->findToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::findToolStripMenuItem_Click);
			// 
			// replaceToolStripMenuItem
			// 
			this->replaceToolStripMenuItem->Name = L"replaceToolStripMenuItem";
			this->replaceToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->replaceToolStripMenuItem->Text = L"Replace...";
			this->replaceToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::replaceToolStripMenuItem_Click_1);
			// 
			// selectAllToolStripMenuItem
			// 
			this->selectAllToolStripMenuItem->Name = L"selectAllToolStripMenuItem";
			this->selectAllToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->selectAllToolStripMenuItem->Text = L"Select All";
			this->selectAllToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::selectAllToolStripMenuItem_Click);
			// 
			// dateAndTimeToolStripMenuItem
			// 
			this->dateAndTimeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->copyrightToolStripMenuItem,
					this->trademarkToolStripMenuItem
			});
			this->dateAndTimeToolStripMenuItem->Name = L"dateAndTimeToolStripMenuItem";
			this->dateAndTimeToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->dateAndTimeToolStripMenuItem->Text = L"Icon";
			// 
			// copyrightToolStripMenuItem
			// 
			this->copyrightToolStripMenuItem->Name = L"copyrightToolStripMenuItem";
			this->copyrightToolStripMenuItem->Size = System::Drawing::Size(150, 22);
			this->copyrightToolStripMenuItem->Text = L"Copyright (©)";
			this->copyrightToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::copyrightToolStripMenuItem_Click);
			// 
			// trademarkToolStripMenuItem
			// 
			this->trademarkToolStripMenuItem->Name = L"trademarkToolStripMenuItem";
			this->trademarkToolStripMenuItem->Size = System::Drawing::Size(150, 22);
			this->trademarkToolStripMenuItem->Text = L"trademark (Ѓ)";
			this->trademarkToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::trademarkToolStripMenuItem_Click);
			// 
			// autoSave
			// 
			this->autoSave->CheckOnClick = true;
			this->autoSave->DoubleClickEnabled = true;
			this->autoSave->Name = L"autoSave";
			this->autoSave->Size = System::Drawing::Size(171, 22);
			this->autoSave->Text = L"autosave";
			this->autoSave->Click += gcnew System::EventHandler(this, &MyForm::dateTimeToolStripMenuItem_Click);
			// 
			// dateTimeToolStripMenuItem1
			// 
			this->dateTimeToolStripMenuItem1->Name = L"dateTimeToolStripMenuItem1";
			this->dateTimeToolStripMenuItem1->Size = System::Drawing::Size(171, 22);
			this->dateTimeToolStripMenuItem1->Text = L"Date Time";
			this->dateTimeToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::dateTimeToolStripMenuItem1_Click);
			// 
			// colorBGToolStripMenuItem
			// 
			this->colorBGToolStripMenuItem->Name = L"colorBGToolStripMenuItem";
			this->colorBGToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->colorBGToolStripMenuItem->Text = L"Color BG";
			this->colorBGToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::colorBGToolStripMenuItem_Click);
			// 
			// themeToolStripMenuItem
			// 
			this->themeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->whiteToolStripMenuItem,
					this->blackToolStripMenuItem
			});
			this->themeToolStripMenuItem->Name = L"themeToolStripMenuItem";
			this->themeToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->themeToolStripMenuItem->Text = L"Theme";
			this->themeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::themeToolStripMenuItem_Click);
			// 
			// whiteToolStripMenuItem
			// 
			this->whiteToolStripMenuItem->Name = L"whiteToolStripMenuItem";
			this->whiteToolStripMenuItem->Size = System::Drawing::Size(105, 22);
			this->whiteToolStripMenuItem->Text = L"White";
			this->whiteToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::whiteToolStripMenuItem_Click);
			// 
			// blackToolStripMenuItem
			// 
			this->blackToolStripMenuItem->Name = L"blackToolStripMenuItem";
			this->blackToolStripMenuItem->Size = System::Drawing::Size(105, 22);
			this->blackToolStripMenuItem->Text = L"Black";
			this->blackToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::blackToolStripMenuItem_Click);
			// 
			// resetTheTextSettingsToolStripMenuItem
			// 
			this->resetTheTextSettingsToolStripMenuItem->Name = L"resetTheTextSettingsToolStripMenuItem";
			this->resetTheTextSettingsToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->resetTheTextSettingsToolStripMenuItem->Text = L"Reset Text Settings";
			this->resetTheTextSettingsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::resetTheTextSettingsToolStripMenuItem_Click);
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(52, 20);
			this->aboutToolStripMenuItem->Text = L"About";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::aboutToolStripMenuItem_Click);
			// 
			// settingToolStripMenuItem1
			// 
			this->settingToolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->lineBreakCheck,
					this->themeToolStripMenuItem1, this->resetTextSettingsToolStripMenuItem
			});
			this->settingToolStripMenuItem1->Name = L"settingToolStripMenuItem1";
			this->settingToolStripMenuItem1->Size = System::Drawing::Size(56, 20);
			this->settingToolStripMenuItem1->Text = L"Setting";
			// 
			// lineBreakCheck
			// 
			this->lineBreakCheck->CheckOnClick = true;
			this->lineBreakCheck->DoubleClickEnabled = true;
			this->lineBreakCheck->Name = L"lineBreakCheck";
			this->lineBreakCheck->Size = System::Drawing::Size(180, 22);
			this->lineBreakCheck->Text = L"line break";
			this->lineBreakCheck->Click += gcnew System::EventHandler(this, &MyForm::lineBreakToolStripMenuItem_Click);
			// 
			// themeToolStripMenuItem1
			// 
			this->themeToolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->whiteToolStripMenuItem1,
					this->blackToolStripMenuItem1
			});
			this->themeToolStripMenuItem1->Name = L"themeToolStripMenuItem1";
			this->themeToolStripMenuItem1->Size = System::Drawing::Size(180, 22);
			this->themeToolStripMenuItem1->Text = L"Theme";
			// 
			// whiteToolStripMenuItem1
			// 
			this->whiteToolStripMenuItem1->Name = L"whiteToolStripMenuItem1";
			this->whiteToolStripMenuItem1->Size = System::Drawing::Size(105, 22);
			this->whiteToolStripMenuItem1->Text = L"White";
			this->whiteToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::whiteToolStripMenuItem1_Click);
			// 
			// blackToolStripMenuItem1
			// 
			this->blackToolStripMenuItem1->Name = L"blackToolStripMenuItem1";
			this->blackToolStripMenuItem1->Size = System::Drawing::Size(105, 22);
			this->blackToolStripMenuItem1->Text = L"Black";
			this->blackToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::blackToolStripMenuItem1_Click);
			// 
			// resetTextSettingsToolStripMenuItem
			// 
			this->resetTextSettingsToolStripMenuItem->Name = L"resetTextSettingsToolStripMenuItem";
			this->resetTextSettingsToolStripMenuItem->Size = System::Drawing::Size(180, 22);
			this->resetTextSettingsToolStripMenuItem->Text = L"Reset Text Settings";
			this->resetTextSettingsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::resetTextSettingsToolStripMenuItem_Click);
			// 
			// toolStrip1
			// 
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(24) {
				this->toolStripButton1,
					this->openFileIcon, this->toolStripButton3, this->toolStripButton4, this->toolStripButton5, this->toolStripButton6, this->toolStripButton7,
					this->toolStripButton8, this->loup, this->toolStripButtonReaplace, this->toolStripButton9, this->toolStripButton11, this->toolStripButton12,
					this->toolStripButton13, this->toolStripButton2, this->leftTextBtn, this->centerTextBtn, this->rightTextBtn, this->toolStripButton14,
					this->fillText, this->toolStripButton15, this->toolStripButtonIncrease, this->toolStripButtonReduce, this->toolStripButton16
			});
			this->toolStrip1->Location = System::Drawing::Point(0, 24);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->toolStrip1->Size = System::Drawing::Size(1052, 25);
			this->toolStrip1->TabIndex = 3;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// toolStripButton1
			// 
			this->toolStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton1.Image")));
			this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton1->Name = L"toolStripButton1";
			this->toolStripButton1->Size = System::Drawing::Size(23, 22);
			this->toolStripButton1->Text = L"New_File";
			this->toolStripButton1->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton1_Click);
			// 
			// openFileIcon
			// 
			this->openFileIcon->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->openFileIcon->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"openFileIcon.Image")));
			this->openFileIcon->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->openFileIcon->Name = L"openFileIcon";
			this->openFileIcon->Size = System::Drawing::Size(23, 22);
			this->openFileIcon->Text = L"Open_File";
			this->openFileIcon->Click += gcnew System::EventHandler(this, &MyForm::openFileIcon_Click);
			// 
			// toolStripButton3
			// 
			this->toolStripButton3->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton3->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton3.Image")));
			this->toolStripButton3->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton3->Name = L"toolStripButton3";
			this->toolStripButton3->Size = System::Drawing::Size(23, 22);
			this->toolStripButton3->Text = L"Save_File";
			this->toolStripButton3->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton3_Click);
			// 
			// toolStripButton4
			// 
			this->toolStripButton4->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton4->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton4.Image")));
			this->toolStripButton4->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton4->Name = L"toolStripButton4";
			this->toolStripButton4->Size = System::Drawing::Size(23, 22);
			this->toolStripButton4->Text = L"Save_as...";
			this->toolStripButton4->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton4_Click);
			// 
			// toolStripButton5
			// 
			this->toolStripButton5->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton5->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton5.Image")));
			this->toolStripButton5->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton5->Name = L"toolStripButton5";
			this->toolStripButton5->Size = System::Drawing::Size(23, 22);
			this->toolStripButton5->Text = L"print";
			this->toolStripButton5->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton5_Click);
			// 
			// toolStripButton6
			// 
			this->toolStripButton6->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton6->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton6.Image")));
			this->toolStripButton6->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton6->Name = L"toolStripButton6";
			this->toolStripButton6->Size = System::Drawing::Size(23, 22);
			this->toolStripButton6->Text = L"copy";
			this->toolStripButton6->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton6_Click);
			// 
			// toolStripButton7
			// 
			this->toolStripButton7->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton7->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton7.Image")));
			this->toolStripButton7->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton7->Name = L"toolStripButton7";
			this->toolStripButton7->Size = System::Drawing::Size(23, 22);
			this->toolStripButton7->Text = L"cut";
			this->toolStripButton7->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton7_Click);
			// 
			// toolStripButton8
			// 
			this->toolStripButton8->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton8->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton8.Image")));
			this->toolStripButton8->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton8->Name = L"toolStripButton8";
			this->toolStripButton8->Size = System::Drawing::Size(23, 22);
			this->toolStripButton8->Text = L"paste";
			this->toolStripButton8->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton8_Click);
			// 
			// loup
			// 
			this->loup->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->loup->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"loup.Image")));
			this->loup->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->loup->Name = L"loup";
			this->loup->Size = System::Drawing::Size(23, 22);
			this->loup->Text = L"find";
			this->loup->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton2_Click);
			// 
			// toolStripButtonReaplace
			// 
			this->toolStripButtonReaplace->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButtonReaplace->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButtonReaplace.Image")));
			this->toolStripButtonReaplace->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButtonReaplace->Name = L"toolStripButtonReaplace";
			this->toolStripButtonReaplace->Size = System::Drawing::Size(23, 22);
			this->toolStripButtonReaplace->Text = L"replace";
			this->toolStripButtonReaplace->Click += gcnew System::EventHandler(this, &MyForm::toolStripButtonReaplace_Click);
			// 
			// toolStripButton9
			// 
			this->toolStripButton9->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton9->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton9.Image")));
			this->toolStripButton9->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton9->Name = L"toolStripButton9";
			this->toolStripButton9->Size = System::Drawing::Size(23, 22);
			this->toolStripButton9->Text = L"back";
			this->toolStripButton9->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton9_Click);
			// 
			// toolStripButton11
			// 
			this->toolStripButton11->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton11->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton11.Image")));
			this->toolStripButton11->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton11->Name = L"toolStripButton11";
			this->toolStripButton11->Size = System::Drawing::Size(23, 22);
			this->toolStripButton11->Text = L"Bold";
			this->toolStripButton11->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton11_Click);
			// 
			// toolStripButton12
			// 
			this->toolStripButton12->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton12->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton12.Image")));
			this->toolStripButton12->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton12->Name = L"toolStripButton12";
			this->toolStripButton12->Size = System::Drawing::Size(23, 22);
			this->toolStripButton12->Text = L"italics";
			this->toolStripButton12->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton12_Click);
			// 
			// toolStripButton13
			// 
			this->toolStripButton13->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton13->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton13.Image")));
			this->toolStripButton13->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton13->Name = L"toolStripButton13";
			this->toolStripButton13->Size = System::Drawing::Size(23, 22);
			this->toolStripButton13->Text = L"underline";
			this->toolStripButton13->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton13_Click);
			// 
			// toolStripButton2
			// 
			this->toolStripButton2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton2.Image")));
			this->toolStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton2->Name = L"toolStripButton2";
			this->toolStripButton2->Size = System::Drawing::Size(23, 22);
			this->toolStripButton2->Text = L"cross out the text";
			this->toolStripButton2->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton2_Click_1);
			// 
			// leftTextBtn
			// 
			this->leftTextBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->leftTextBtn->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"leftTextBtn.Image")));
			this->leftTextBtn->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->leftTextBtn->Name = L"leftTextBtn";
			this->leftTextBtn->Size = System::Drawing::Size(23, 22);
			this->leftTextBtn->Text = L"Left_text";
			this->leftTextBtn->Click += gcnew System::EventHandler(this, &MyForm::leftTextBtn_Click);
			// 
			// centerTextBtn
			// 
			this->centerTextBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->centerTextBtn->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"centerTextBtn.Image")));
			this->centerTextBtn->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->centerTextBtn->Name = L"centerTextBtn";
			this->centerTextBtn->Size = System::Drawing::Size(23, 22);
			this->centerTextBtn->Text = L"Center_text";
			this->centerTextBtn->Click += gcnew System::EventHandler(this, &MyForm::centerTextBtn_Click);
			// 
			// rightTextBtn
			// 
			this->rightTextBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->rightTextBtn->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"rightTextBtn.Image")));
			this->rightTextBtn->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->rightTextBtn->Name = L"rightTextBtn";
			this->rightTextBtn->Size = System::Drawing::Size(23, 22);
			this->rightTextBtn->Text = L"Right_Text";
			this->rightTextBtn->Click += gcnew System::EventHandler(this, &MyForm::rightTextBtn_Click);
			// 
			// toolStripButton14
			// 
			this->toolStripButton14->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton14->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton14.Image")));
			this->toolStripButton14->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton14->Name = L"toolStripButton14";
			this->toolStripButton14->Size = System::Drawing::Size(23, 22);
			this->toolStripButton14->Text = L"Font_size";
			this->toolStripButton14->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton14_Click);
			// 
			// fillText
			// 
			this->fillText->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->fillText->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"fillText.Image")));
			this->fillText->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->fillText->Name = L"fillText";
			this->fillText->Size = System::Drawing::Size(23, 22);
			this->fillText->Text = L"Filling_the_text_background";
			this->fillText->Click += gcnew System::EventHandler(this, &MyForm::fillText_Click);
			// 
			// toolStripButton15
			// 
			this->toolStripButton15->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton15->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton15.Image")));
			this->toolStripButton15->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton15->Name = L"toolStripButton15";
			this->toolStripButton15->Size = System::Drawing::Size(23, 22);
			this->toolStripButton15->Text = L"color";
			this->toolStripButton15->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton15_Click);
			// 
			// toolStripButtonIncrease
			// 
			this->toolStripButtonIncrease->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButtonIncrease->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButtonIncrease.Image")));
			this->toolStripButtonIncrease->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButtonIncrease->Name = L"toolStripButtonIncrease";
			this->toolStripButtonIncrease->Size = System::Drawing::Size(23, 22);
			this->toolStripButtonIncrease->Text = L"increase";
			this->toolStripButtonIncrease->Click += gcnew System::EventHandler(this, &MyForm::toolStripButtonIncrease_Click);
			// 
			// toolStripButtonReduce
			// 
			this->toolStripButtonReduce->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButtonReduce->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButtonReduce.Image")));
			this->toolStripButtonReduce->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButtonReduce->Name = L"toolStripButtonReduce";
			this->toolStripButtonReduce->Size = System::Drawing::Size(23, 22);
			this->toolStripButtonReduce->Text = L"reduce";
			this->toolStripButtonReduce->Click += gcnew System::EventHandler(this, &MyForm::toolStripButtonReduce_Click);
			// 
			// toolStripButton16
			// 
			this->toolStripButton16->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton16->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton16.Image")));
			this->toolStripButton16->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton16->Name = L"toolStripButton16";
			this->toolStripButton16->Size = System::Drawing::Size(23, 22);
			this->toolStripButton16->Text = L"info";
			this->toolStripButton16->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton16_Click);
			// 
			// infoText
			// 
			this->infoText->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->infoText->AutoSize = true;
			this->infoText->Location = System::Drawing::Point(685, 474);
			this->infoText->Name = L"infoText";
			this->infoText->Size = System::Drawing::Size(33, 13);
			this->infoText->TabIndex = 4;
			this->infoText->Text = L"None";
			this->infoText->Click += gcnew System::EventHandler(this, &MyForm::infoText_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// nameFile
			// 
			this->nameFile->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->nameFile->AutoSize = true;
			this->nameFile->Location = System::Drawing::Point(12, 474);
			this->nameFile->Name = L"nameFile";
			this->nameFile->Size = System::Drawing::Size(40, 13);
			this->nameFile->TabIndex = 5;
			this->nameFile->Text = L"No File";
			this->nameFile->Click += gcnew System::EventHandler(this, &MyForm::nameFile_Click);
			// 
			// notifications
			// 
			this->notifications->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->notifications->AutoSize = true;
			this->notifications->Location = System::Drawing::Point(348, 474);
			this->notifications->Name = L"notifications";
			this->notifications->Size = System::Drawing::Size(63, 13);
			this->notifications->TabIndex = 6;
			this->notifications->Text = L"notifications";
			// 
			// DirectorySearcher
			// 
			this->DirectorySearcher->ClientTimeout = System::TimeSpan::Parse(L"-00:00:01");
			this->DirectorySearcher->ServerPageTimeLimit = System::TimeSpan::Parse(L"-00:00:01");
			this->DirectorySearcher->ServerTimeLimit = System::TimeSpan::Parse(L"-00:00:01");
			// 
			// encoding
			// 
			this->encoding->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->encoding->AutoSize = true;
			this->encoding->Location = System::Drawing::Point(956, 474);
			this->encoding->Name = L"encoding";
			this->encoding->Size = System::Drawing::Size(84, 13);
			this->encoding->TabIndex = 7;
			this->encoding->Text = L"Encoding: None";
			this->encoding->Click += gcnew System::EventHandler(this, &MyForm::codeText_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->AutoValidate = System::Windows::Forms::AutoValidate::Disable;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(1052, 496);
			this->Controls->Add(this->encoding);
			this->Controls->Add(this->notifications);
			this->Controls->Add(this->nameFile);
			this->Controls->Add(this->infoText);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->richTextBox1);
			this->ForeColor = System::Drawing::Color::Black;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MinimumSize = System::Drawing::Size(1068, 535);
			this->Name = L"MyForm";
			this->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->StartPosition = System::Windows::Forms::FormStartPosition::WindowsDefaultBounds;
			this->Text = L"Notepad+";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

#pragma endregion

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) 
	{
		LoadSettings();
	}
	

	private: System::Void replaceToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	}

	private: System::Void richTextBox1_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		richTextBox1->Modified = true;

		// Update character and line count statistics
		String^ text = richTextBox1->Text;
		int totalchars = text->Length;
		int nonSpaceChars = text->Replace(" ", "")->Length;
		int lineCount = richTextBox1->Lines->Length;

		infoText->Text = String::Format(
			"Characters: {0}; Without spaces: {1}; Lines: {2}",
			totalchars, nonSpaceChars, lineCount);

		// Reset auto-save timer if enabled
		if (checkAutoSave && typingTimer != nullptr)
		{
			typingTimer->Stop();
			typingTimer->Start();
		}

	}


	//exit
	private: System::Void MyForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
	{

		SaveSettings();

		// Debug check - verify this handler is being called
		System::Diagnostics::Debug::WriteLine("FormClosing event triggered");

		// More comprehensive check for unsaved changes
		bool hasUnsavedChanges = richTextBox1->Modified ||
			(richTextBox1->Text->Length > 0 && nameFile->Text == "No File");

		if (hasUnsavedChanges && checkAutoSave == false)
		{
			System::Windows::Forms::DialogResult result = MessageBox::Show(
				"Do you want to save changes before exiting?",
				"Notepad+",
				MessageBoxButtons::YesNoCancel,
				MessageBoxIcon::Question);

			switch (result)
			{
			case Windows::Forms::DialogResult::Yes:
			{
				bool saveSuccessful = false;

				// Case 1: Existing file
				if (nameFile->Text != "No File")
				{
					try
					{
						File::WriteAllText(nameFile->Text, richTextBox1->Text);
						richTextBox1->Modified = false;
						saveSuccessful = true;
					}
					catch (Exception^ ex)
					{
						MessageBox::Show("Save failed: " + ex->Message, "Error");
						e->Cancel = true;
						return;
					}
				}
				// Case 2: New unsaved file
				else
				{
					SaveFileDialog^ saveDlg = gcnew SaveFileDialog();
					saveDlg->Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
					saveDlg->Title = "Save As";
					saveDlg->OverwritePrompt = true;

					if (saveDlg->ShowDialog() == Windows::Forms::DialogResult::OK)
					{
						try
						{
							File::WriteAllText(saveDlg->FileName, richTextBox1->Text);
							nameFile->Text = saveDlg->FileName;
							richTextBox1->Modified = false;
							saveSuccessful = true;
						}
						catch (Exception^ ex)
						{
							MessageBox::Show("Save failed: " + ex->Message, "Error");
							e->Cancel = true;
							return;
						}
					}
					else
					{
						// User canceled save dialog
						e->Cancel = true;
						return;
					}
				}

				// Only continue closing if save was successful
				if (!saveSuccessful)
				{
					e->Cancel = true;
					return;
				}
			}
			break;

			case Windows::Forms::DialogResult::No:
				// Continue with exit
				break;

			case Windows::Forms::DialogResult::Cancel:
				e->Cancel = true;
				return;
			}
		}

		// Cleanup resources
		if (typingTimer != nullptr)
		{
			typingTimer->Stop();
			delete typingTimer;
			typingTimer = nullptr;
		}
	}


	private: System::Void infoText_Click(System::Object^ sender, System::EventArgs^ e)
	{

	}

		   //Save Settings and Load
	private: void SaveSettings() {
		try {
			String^ settingsFile = "notepad_settings.cfg";
			StreamWriter^ sw = gcnew StreamWriter(settingsFile);

			// Saving the background color
			sw->WriteLine("BackColor=" + savedBackColor.ToArgb().ToString());

			// Saving the current font
			sw->WriteLine("FontName=" + richTextBox1->Font->Name);
			sw->WriteLine("FontSize=" + richTextBox1->Font->Size.ToString());
			sw->WriteLine("FontStyle=" + ((int)richTextBox1->Font->Style).ToString());

			sw->Close();
		}
		catch (...) 
		{
			WorkWithLogTxt workWithLogTxt;
			workWithLogTxt.writeLog("notepadPlusSave.log", "error saving text styles");
		}
	}

	private: void LoadSettings() {
		try {
			String^ settingsFile = "notepad_settings.cfg";
			if (File::Exists(settingsFile)) {
				StreamReader^ sr = gcnew StreamReader(settingsFile);
				String^ line;

				while ((line = sr->ReadLine()) != nullptr) {
					if (line->StartsWith("BackColor=")) {
						int argb = Int32::Parse(line->Substring(10));
						savedBackColor = Color::FromArgb(argb);
						richTextBox1->BackColor = savedBackColor;
					}
					else if (line->StartsWith("FontName=")) {
						String^ fontName = line->Substring(9);
						float fontSize = 12.0f; 
						FontStyle fontStyle = FontStyle::Regular;

						// We read the following line for the font size
						line = sr->ReadLine();
						if (line->StartsWith("FontSize=")) {
							fontSize = Single::Parse(line->Substring(9));
						}

						// We read the following line for the font style
						line = sr->ReadLine();
						if (line->StartsWith("FontStyle=")) {
							fontStyle = (FontStyle)Int32::Parse(line->Substring(10));
						}

						richTextBox1->Font = gcnew System::Drawing::Font(fontName, fontSize, fontStyle);
					}
				}

				sr->Close();
			}
		}
		catch (...) 
		{
			WorkWithLogTxt workWithLogText;
			workWithLogText.writeLog("notepadPlusLoad.log", "Error load text styles");
		}
	}

		   //Open File 
	private: System::Void openToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		WorkWithFiles Files;
		Files.openFile(openFileDialog1, nameFile, notifications, encoding, richTextBox1, this->Text);
	}
		   //icon
	private: System::Void openFileIcon_Click(System::Object^ sender, System::EventArgs^ e)
	{
		WorkWithFiles Files;
		Files.openFile(openFileDialog1, nameFile, notifications, encoding, richTextBox1, this->Text);

	}

		   //Ctrl+different keys 
	private: System::Void MyForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
	{
		//save
		if (e->Control && e->KeyCode == Keys::S)
		{
			WorkWithFiles Files;
			Files.fileSave(richTextBox1, nameFile, notifications, encoding, this->Text);
		}
		//new File
		else if (e->Control && e->KeyCode == Keys::N)
		{
			WorkWithFiles Files;
			offToolStripMenuItem_Click(this, e);
			Files.createNewFile(richTextBox1, nameFile, notifications);
		}
		//undo
		else if (e->Control && e->KeyCode == Keys::Z)
		{
			if (richTextBox1->CanUndo)
			{
				richTextBox1->Undo();
				richTextBox1->ClearUndo();
				notifications->Text = "the cancellation of the last changes is successful";
			}
		}
		//copy
		else if (e->Control && e->KeyCode == Keys::C)
			notifications->Text = "the text has been copied successfully";
		//cut
		else if (e->Control && e->KeyCode == Keys::X)
		{
			if (richTextBox1->SelectedText->Length > 0)
			{
				Clipboard::SetText(richTextBox1->SelectedText);
				richTextBox1->SelectedText = "";
				notifications->Text = "successfully cut out!";
			}
			else
				notifications->Text = "highlight something!";
		}
		//paste
		else if (e->Control && e->KeyCode == Keys::V)
			notifications->Text = "the text was inserted successfully!";
		//selectAll
		else if (e->Control && e->KeyCode == Keys::A)
			notifications->Text = "everything was highlighted successfully";

		//open file
		else if (e->Control && e->KeyCode == Keys::O)
		{
			WorkWithFiles Files;
			Files.openFile(openFileDialog1, nameFile, notifications, encoding,richTextBox1, this->Text);
		}

		//increase
		else if (e->Control && e->KeyCode == Keys::Add || e->Control && e->KeyCode == Keys::Oemplus)
		{
			try
			{
				int selectionStart = richTextBox1->SelectionStart;
				int selectionLength = richTextBox1->SelectionLength;

				if (selectionLength > 0)
				{
					//selected text
					System::Drawing::Font^ currentFont = richTextBox1->SelectionFont;

					if (currentFont != nullptr)
					{
						float newSize = Math::Min(currentFont->Size + 1.0f, 72.0f);
						richTextBox1->SelectionFont = gcnew System::Drawing::Font(
							currentFont->FontFamily,
							newSize,
							currentFont->Style
						);
					}
				}
				else
				{
					//for all text
					System::Drawing::Font^ currentFont = richTextBox1->Font;
					float newSize = Math::Min(currentFont->Size + 1.0f, 72.0f);

					richTextBox1->Font = gcnew System::Drawing::Font(
						currentFont->FontFamily,
						newSize,
						currentFont->Style,
						currentFont->Unit
					);
				}

				//recovery selected
				richTextBox1->SelectionStart = selectionStart;
				richTextBox1->SelectionLength = selectionLength;

				notifications->Text = "the text magnification was successful";
			}
			catch (Exception^ ex)
			{
				notifications->Text = "Error when enlarging the font:";
				MessageBox::Show("Error when enlarging the font: " + ex->Message,
					"Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error);
			}
		}

		//reduce
		else if (e->Control && e->KeyCode == Keys::Subtract || e->Control && e->KeyCode == Keys::OemMinus)
		{
			try
			{
				int selectionStart = richTextBox1->SelectionStart;
				int selectionLength = richTextBox1->SelectionLength;

				if (selectionLength > 0)
				{
					//selected text
					System::Drawing::Font^ currentFont = richTextBox1->SelectionFont;

					if (currentFont != nullptr)
					{
						float newSize = Math::Min(currentFont->Size - 1.0f, 72.0f);
						richTextBox1->SelectionFont = gcnew System::Drawing::Font(
							currentFont->FontFamily,
							newSize,
							currentFont->Style,
							currentFont->Unit
						);
					}
				}
				else
				{
					//for all text
					System::Drawing::Font^ currentFont = richTextBox1->Font;
					float newSize = Math::Min(currentFont->Size - 1.0f, 72.0f);

					richTextBox1->Font = gcnew System::Drawing::Font(
						currentFont->FontFamily,
						newSize,
						currentFont->Style,
						currentFont->Unit
					);
				}

				//recovery selected
				richTextBox1->SelectionStart = selectionStart;
				richTextBox1->SelectionLength = selectionLength;

				notifications->Text = "text reduction was successful";
			}
			catch (Exception^ ex)
			{
				notifications->Text = "Error when enlarging the font";
				MessageBox::Show("Error when enlarging the font: " + ex->Message,
					"Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error);
			}
		}
		//find (Ctrl+F)
		else if (e->Control && e->KeyCode == Keys::F)
		{
			MyForm1^ myForm1 = gcnew MyForm1(richTextBox1);
			myForm1->Show();
		}
		//crossed out text
		else if (e->Control && e->KeyCode == Keys::D)
		{
			helperRich HelperRich;
			HelperRich.ToggleFontStyle(FontStyle::Strikeout, richTextBox1, notifications);
			
		}


	}

		   

		   //new file
	private: System::Void newFile_Click(System::Object^ sender, System::EventArgs^ e)
	{
		WorkWithFiles Files;
		offToolStripMenuItem_Click(this, e);
		Files.createNewFile(richTextBox1, nameFile, notifications);
	}
		   //icon
	private: System::Void toolStripButton1_Click(System::Object^ sender, System::EventArgs^ e)
	{
		WorkWithFiles Files;
		offToolStripMenuItem_Click(this, e);
		Files.createNewFile(richTextBox1, nameFile, notifications);
	}

		   //save file
	private: System::Void saveFile_Click(System::Object^ sender, System::EventArgs^ e)
	{
		WorkWithFiles Files;
		Files.fileSave(richTextBox1, nameFile, notifications, encoding, this->Text);
	}

		   //icon
	private: System::Void toolStripButton3_Click(System::Object^ sender, System::EventArgs^ e)
	{
		WorkWithFiles Files;
		Files.fileSave(richTextBox1, nameFile, notifications, encoding, this->Text);
	}

		   //save as...
	private: System::Void saveFileAs_Click(System::Object^ sender, System::EventArgs^ e)
	{
		WorkWithFiles Files;
		Files.fileSaveAs(richTextBox1, nameFile, notifications, encoding, this->Text);
	}
		   //icon
	private: System::Void toolStripButton4_Click(System::Object^ sender, System::EventArgs^ e)
	{
		WorkWithFiles Files;
		Files.fileSaveAs(richTextBox1, nameFile, notifications, encoding, this->Text);
	}

		   //print
	private: System::Void print_Click(System::Object^ sender, System::EventArgs^ e)
	{
		PrintHelper^ printer = gcnew PrintHelper(richTextBox1);
		printer->Print(notifications);
	}
		   //icon
	private: System::Void toolStripButton5_Click(System::Object^ sender, System::EventArgs^ e)
	{
		PrintHelper^ printer = gcnew PrintHelper(richTextBox1);
		printer->Print(notifications);
	}

		   //Exit
	private: System::Void exit_Click(System::Object^ sender, System::EventArgs^ e)
	{
		Application::ExitThread();
		Environment::Exit(0);
	}

		   //copyright
	private: System::Void copyrightToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		richTextBox1->AppendText("©");
	}
		   //trademark
	private: System::Void trademarkToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		richTextBox1->AppendText("Ѓ");
	}

		   //copy
	private: System::Void copyToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		String^ textToCopy = richTextBox1->Text;
		if (!String::IsNullOrEmpty(textToCopy))
		{
			Clipboard::SetText(textToCopy);
			notifications->Text = "the text has been copied successfully";
		}
	}

		   //icon
	private: System::Void toolStripButton6_Click(System::Object^ sender, System::EventArgs^ e)
	{
		String^ textToCopy = richTextBox1->Text;
		if (!String::IsNullOrEmpty(textToCopy))
		{
			Clipboard::SetText(textToCopy);
			notifications->Text = "the text has been copied successfully";
		}
	}

		   //CTRL + Z
	private: System::Void cancelToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (richTextBox1->CanUndo)
		{
			richTextBox1->Undo();
			richTextBox1->ClearUndo();
			notifications->Text = "the cancellation of the last changes is successful";
		}
	}

		   //icon
	private: System::Void toolStripButton9_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (richTextBox1->CanUndo)
		{
			richTextBox1->Undo();
			richTextBox1->ClearUndo();
			notifications->Text = "the cancellation of the last changes is successful";
		}
	}

		   //delete all
	private: System::Void deleteAllToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		richTextBox1->Text = "";
		notifications->Text = "the entire text was deleted successfully.";
	}

		   //cut
	private: System::Void cutToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (richTextBox1->SelectedText->Length > 0)
		{
			Clipboard::SetText(richTextBox1->SelectedText);
			richTextBox1->SelectedText = "";
			notifications->Text = "successfully cut out!";
		}
		else
			notifications->Text = "highlight something!";

	}

		   //icon
	private: System::Void toolStripButton7_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (richTextBox1->SelectedText->Length > 0)
		{
			Clipboard::SetText(richTextBox1->SelectedText);
			richTextBox1->SelectedText = "";
			notifications->Text = "successfully cut out!";
		}
		else
			notifications->Text = "highlight something!";
	}

		   //paste
	private: System::Void pasteToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (Clipboard::ContainsText())
		{
			richTextBox1->Paste();
			notifications->Text = "the text was inserted successfully!";
		}
	}
		   //icon
	private: System::Void toolStripButton8_Click(System::Object^ sender, System::EventArgs^ e)
	{
		richTextBox1->Paste();
		notifications->Text = "the text was inserted successfully!";
	}

		   //find
	private: System::Void findToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		MyForm1^ findForm = gcnew MyForm1(richTextBox1);
		findForm->Show();
	}

		   //icon
	private: System::Void toolStripButton2_Click(System::Object^ sender, System::EventArgs^ e)
	{
		MyForm1^ findForm = gcnew MyForm1(richTextBox1);

		findForm->Show();
	}
	private: System::Void searchButton_Click(System::Object^ sender, System::EventArgs^ e)
	{



	}




		   //select all
	private: System::Void selectAllToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		richTextBox1->SelectAll();
		notifications->Text = "everything was highlighted successfully";
	}

		   //replace
	private: System::Void replaceToolStripMenuItem_Click_1(System::Object^ sender, System::EventArgs^ e)
	{
		MyForm2^ replaceForm = gcnew MyForm2(richTextBox1);
		replaceForm->Show();
	}

		   //icon replace
	private: System::Void toolStripButtonReaplace_Click(System::Object^ sender, System::EventArgs^ e)
	{
		MyForm2^ replaceForm = gcnew MyForm2(richTextBox1);
		replaceForm->Show();
	}

		   //font size (icon)
	private: System::Void toolStripButton14_Click(System::Object^ sender, System::EventArgs^ e)
	{
		FontDialog^ fontDialog = gcnew FontDialog();

		// Dialog settings as in Notepad
		fontDialog->ShowColor = false;
		fontDialog->ShowEffects = false;
		fontDialog->AllowScriptChange = false;
		fontDialog->AllowVerticalFonts = false;
		fontDialog->FontMustExist = true;
		fontDialog->ShowApply = false;
		fontDialog->ShowHelp = false;

		fontDialog->Font = richTextBox1->Font;

		if (fontDialog->ShowDialog(this) == System::Windows::Forms::DialogResult::OK)
			richTextBox1->Font = fontDialog->Font;

		delete fontDialog;
	}

		   //Date Time
	private: System::Void dateTimeToolStripMenuItem1_Click(System::Object^ sender, System::EventArgs^ e)
	{
		helperRich HelperRich;
		HelperRich.getDateTimeForRichTextBox(richTextBox1);
	}

		   //icon font Bold
	private: System::Void toolStripButton11_Click(System::Object^ sender, System::EventArgs^ e)
	{
		helperRich HelperRich;
		HelperRich.ToggleFontStyle(FontStyle::Bold, richTextBox1, notifications);
	}

		   //icon font italic
	private: System::Void toolStripButton12_Click(System::Object^ sender, System::EventArgs^ e)
	{
		helperRich HelperRich;
		HelperRich.ToggleFontStyle(FontStyle::Italic, richTextBox1, notifications);
	}

		   //icon font underline
	private: System::Void toolStripButton13_Click(System::Object^ sender, System::EventArgs^ e)
	{
		helperRich HelperRich;
		HelperRich.ToggleFontStyle(FontStyle::Underline, richTextBox1, notifications);
	}

		   //crossed out text
			//icon
	private: System::Void toolStripButton2_Click_1(System::Object^ sender, System::EventArgs^ e) 
	{
		helperRich HelperRich;
		HelperRich.ToggleFontStyle(FontStyle::Strikeout, richTextBox1, notifications);
	}

		   //increase icon
	private: System::Void toolStripButtonIncrease_Click(System::Object^ sender, System::EventArgs^ e)
	{
		try
		{
			int selectionStart = richTextBox1->SelectionStart;
			int selectionLength = richTextBox1->SelectionLength;

			if (selectionLength > 0)
			{
				//selected text
				System::Drawing::Font^ currentFont = richTextBox1->SelectionFont;

				if (currentFont != nullptr)
				{
					float newSize = Math::Min(currentFont->Size + 1.0f, 72.0f);
					richTextBox1->SelectionFont = gcnew System::Drawing::Font(
						currentFont->FontFamily,
						newSize,
						currentFont->Style
					);
				}
			}
			else
			{
				//for all text
				System::Drawing::Font^ currentFont = richTextBox1->Font;
				float newSize = Math::Min(currentFont->Size + 1.0f, 72.0f);

				richTextBox1->Font = gcnew System::Drawing::Font(
					currentFont->FontFamily,
					newSize,
					currentFont->Style,
					currentFont->Unit
				);
			}

			//recovery selected
			richTextBox1->SelectionStart = selectionStart;
			richTextBox1->SelectionLength = selectionLength;
		}
		catch (Exception^ ex)
		{
			notifications->Text = "Error when enlarging the font:";
			MessageBox::Show("Error when enlarging the font: " + ex->Message,
				"Error",
				MessageBoxButtons::OK,
				MessageBoxIcon::Error);
		}
	}

		   //decrease icon
	private: System::Void toolStripButtonReduce_Click(System::Object^ sender, System::EventArgs^ e)
	{
		try
		{
			int selectionStart = richTextBox1->SelectionStart;
			int selectionLength = richTextBox1->SelectionLength;

			if (selectionLength > 0)
			{
				//selected text
				System::Drawing::Font^ currentFont = richTextBox1->SelectionFont;

				if (currentFont != nullptr)
				{
					float newSize = Math::Min(currentFont->Size - 1.0f, 72.0f);
					richTextBox1->SelectionFont = gcnew System::Drawing::Font(
						currentFont->FontFamily,
						newSize,
						currentFont->Style
					);
				}
			}
			else
			{
				//for all text
				System::Drawing::Font^ currentFont = richTextBox1->Font;
				float newSize = Math::Min(currentFont->Size - 1.0f, 72.0f);

				richTextBox1->Font = gcnew System::Drawing::Font(
					currentFont->FontFamily,
					newSize,
					currentFont->Style,
					currentFont->Unit
				);
			}

			//recovery selected
			richTextBox1->SelectionStart = selectionStart;
			richTextBox1->SelectionLength = selectionLength;
		}
		catch (Exception^ ex)
		{
			notifications->Text = "Error when reducing the text:";
			MessageBox::Show("Error when reducing the text:" + ex->Message,
				"Error",
				MessageBoxButtons::OK,
				MessageBoxIcon::Error);
		}
	}

		   //color font icon
	private: System::Void toolStripButton15_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (colorDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			richTextBox1->SelectionColor = colorDialog1->Color;
			notifications->Text = "the text change was successful";
		}
	}

		   //BG Color
	private: System::Void colorBGToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		colorDialog1->Color = richTextBox1->BackColor;
		if (colorDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			richTextBox1->BackColor = colorDialog1->Color;
			savedBackColor = colorDialog1->Color; // save color
			notifications->Text = "The background color has been changed";
		}
	}

		   //
		   // Logics AutoSave
		   // 

		   // It is triggered when the user does not type for 1 second
	public: System::Void OnTypingStopped(System::Object^ sender, System::EventArgs^ e)
	{
		// Only proceed if auto-save is enabled
		if (!checkAutoSave || typingTimer == nullptr) return;

		typingTimer->Stop();

		// Auto-save file if it has a name
		if (nameFile->Text != "No File")
		{
			WorkWithFiles Files;
			Files.fileSave(richTextBox1, nameFile, notifications, encoding, this->Text);
		}
		else
		{
			notifications->Text = "Cannot save - no file is open";
			MessageBox::Show("Cannot save because no file is open",
				"Error",
				MessageBoxButtons::OK,
				MessageBoxIcon::Error);
		}
	}

	//auto save
	private: System::Void dateTimeToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		bool currentStateAutoSave = this->autoSave->Checked;

		// Enable auto-save
		if (currentStateAutoSave)
		{
			if (nameFile->Text != "No File")
			{
				checkAutoSave = true;
				notifications->Text = "Auto-save enabled";
			}
			else
			{
				MessageBox::Show("cannot save - no file is open",
					"Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error);
				notifications->Text = "Please create a new file or open an existing one!";
				currentStateAutoSave = false;
			}
		}
		else
		{
			offToolStripMenuItem_Click(this, e);
		}

	}

	   // Disable auto-save
		private: System::Void offToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
		{
			checkAutoSave = false;
			notifications->Text = "Auto-save disabled";
		}



	//info about
	private: System::Void aboutToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		MyForm3^ info = gcnew MyForm3();
		info->Show();
	}
	//icon
	private: System::Void toolStripButton16_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		MyForm3^ info = gcnew MyForm3();
		info->Show();

	}

	//reset the text settings
	private: System::Void resetTheTextSettingsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		this->richTextBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, 
			System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(204)));
		
		if (toolStrip1->BackColor != System::Drawing::Color::FromArgb(60, 60, 60) && 
			richTextBox1->BackColor == System::Drawing::Color::FromArgb(60, 60, 60))
		{
			richTextBox1->BackColor = System::Drawing::Color::FromArgb(60, 60, 60);
			richTextBox1->SelectionColor = Color::White;
			richTextBox1->SelectionBackColor = System::Drawing::Color::White;
		}
		else if (toolStrip1->BackColor != System::Drawing::Color::FromArgb(60, 60, 60) && 
			richTextBox1->BackColor != System::Drawing::Color::FromArgb(60, 60, 60))
		{
			richTextBox1->SelectionColor = Color::Black;
			richTextBox1->BackColor = DefaultBackColor;
		}

		richTextBox1->Invalidate();
		notifications->Text = "settings reset successful";
	}

	//show encoding
	private: System::Void codeText_Click(System::Object^ sender, System::EventArgs^ e)
	{
		MessageBox::Show(
			"To find out the encoding, you need to save the file or open it!", 
			"Info",
			MessageBoxButtons::OK,
			MessageBoxIcon::Information
		);
	}

	private: System::Void fillText_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		if (colorDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			richTextBox1->SelectionBackColor = colorDialog1->Color;
			notifications->Text = "the text change was successful";
		}
	}

	///////////////////////////////////
	/////////text setting/////////////
	//////////////////////////////////

	//center icon
	private: System::Void centerTextBtn_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		richTextBox1->SelectionAlignment = System::Windows::Forms::HorizontalAlignment::Center;
	}

	//left icon
	private: System::Void leftTextBtn_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		richTextBox1->SelectionAlignment = System::Windows::Forms::HorizontalAlignment::Left;
	}

	//right icon
	private: System::Void rightTextBtn_Click(System::Object^ sender, System::EventArgs^ e)
	{
		richTextBox1->SelectionAlignment = System::Windows::Forms::HorizontalAlignment::Right;
	}

	
	//////////////////////////////////
	/////////setting/////////////////
	////////////////////////////////

	//line break
	private: System::Void lineBreakToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		bool currentStateCheckLineBreak = this->lineBreakCheck->Checked;

		if (currentStateCheckLineBreak)
		{
			notifications->Text = "line break on";
			richTextBox1->WordWrap = true;
		}
		else
		{
			notifications->Text = "line break off";
			this->richTextBox1->WordWrap = false;
		}
	}
	
	//Theme
	private: System::Void themeToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{

	}
	//black theme
	private: System::Void blackToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		WorkWithForm WForm;
		WForm.blackTheme(this, richTextBox1, toolStrip1, menuStrip1, notifications);
	}
	//White theme
	private: System::Void whiteToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		WorkWithForm WForm;
		WForm.whiteTheme(this, richTextBox1, toolStrip1, menuStrip1, notifications);
	}
	//Black theme (setting)
	private: System::Void blackToolStripMenuItem1_Click(System::Object^ sender, System::EventArgs^ e)
	{
		WorkWithForm WForm;
		WForm.blackTheme(this, richTextBox1, toolStrip1, menuStrip1, notifications);
	}
	//White theme (setting)
	private: System::Void whiteToolStripMenuItem1_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		WorkWithForm WForm;
		WForm.whiteTheme(this, richTextBox1, toolStrip1, menuStrip1, notifications);
	}

	// Reset text
	private: System::Void resetTextSettingsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		this->richTextBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12,
			System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(204)));

		if (toolStrip1->BackColor != System::Drawing::Color::FromArgb(60, 60, 60) &&
			richTextBox1->BackColor == System::Drawing::Color::FromArgb(60, 60, 60))
		{
			richTextBox1->BackColor = System::Drawing::Color::FromArgb(60, 60, 60);
			richTextBox1->SelectionColor = Color::White;
			richTextBox1->SelectionBackColor = System::Drawing::Color::White;
		}
		else if (toolStrip1->BackColor != System::Drawing::Color::FromArgb(60, 60, 60) &&
			richTextBox1->BackColor != System::Drawing::Color::FromArgb(60, 60, 60))
		{
			richTextBox1->SelectionColor = Color::Black;
			richTextBox1->BackColor = DefaultBackColor;
		}

		richTextBox1->Invalidate();
		notifications->Text = "settings reset successful";
	}

	private: System::Void nameFile_Click(System::Object^ sender, System::EventArgs^ e) {
	}

	};
	
	

}