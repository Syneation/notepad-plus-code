#pragma once

#include <iostream>
#include <string>
#include <ctime>

using namespace System;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

class helperRich
{
	public: String^ getDate()
	{
		time_t seconds = time(NULL);
		tm timeinfo;
		localtime_s(&timeinfo, &seconds);

		char buffer[26];
		asctime_s(buffer, sizeof(buffer), &timeinfo);

		return gcnew String(buffer);
	}
	
	public: void getDateTimeForRichTextBox(RichTextBox^ richTextBox)
	{
		time_t seconds = time(NULL);
		tm timeinfo;
		localtime_s(&timeinfo, &seconds);
	
		char buffer[26];
		asctime_s(buffer, sizeof(buffer), &timeinfo);
		richTextBox->AppendText(gcnew String(buffer));
	}

	 //functions
	//change font from quick panel
	public: void ToggleFontStyle(FontStyle style, System::Windows::Forms::RichTextBox^ richTextBox1, Label^ notifications)
	{
		// checking if there is a selected text.
		if (richTextBox1->SelectionLength > 0)
		{
			// Getting the current font of the selected text
			System::Drawing::Font^ currentFont = richTextBox1->SelectionFont;
	
			if (currentFont != nullptr)
			{
				// Switching the specified style (XOR)
				FontStyle newStyle = currentFont->Style ^ style;
	
				// Applying a new font
				richTextBox1->SelectionFont = gcnew System::Drawing::Font(
					currentFont->FontFamily,
					currentFont->Size,
					newStyle,
					currentFont->Unit
				);
			}
			notifications->Text = "the font change was successful";
		}
		else
		{
			//If nothing is highlighted, change the font for the entire text.
			System::Drawing::Font^ currentFont = richTextBox1->Font;
			FontStyle newStyle = currentFont->Style ^ style;
	
			richTextBox1->Font = gcnew System::Drawing::Font(
				currentFont->FontFamily,
				currentFont->Size,
				newStyle,
				currentFont->Unit
			);
			notifications->Text = "the font change was successful";
		}
	
	}	

	public: String^ DetectEncodingSimple(String^ filePath)
	{
		try
		{
			// Reading the first 4 bytes of the file
			array<unsigned char>^ bytes = File::ReadAllBytes(filePath);

			if (bytes->Length >= 4)
			{
				// Checking the BOM (encoding markers)
				if (bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF)
					return "UTF-8";
				else if (bytes[0] == 0xFF && bytes[1] == 0xFE)
					return "UTF-16 (LE)";
				else if (bytes[0] == 0xFE && bytes[1] == 0xFF)
					return "UTF-16 (BE)";
			}
			return "ANSI";
		}
		catch (Exception^ e)
		{
			return "Detection error: " + e->Message;
		}
	}
	
};

//Logs
class WorkWithLogTxt
{
	public: void writeLog(String^ nameFileLog, String^ text)
	{
		helperRich HelperRich;
		try
		{
			String^ settingsFileLog = nameFileLog;
			StreamWriter^ stringWriterLog = gcnew StreamWriter(settingsFileLog);
			stringWriterLog->WriteLine(text + " " + HelperRich.getDate());
			stringWriterLog->Close();
		}
		catch (...)
		{
			//Ignore Error
		}
	}
};

//Files
class WorkWithFiles
{

	public: void createNewFile(RichTextBox^ richTextBox1, Label^ nameFile, Label^ notifications)
	{
		richTextBox1->Clear();
		nameFile->Text = "No File";
		richTextBox1->Modified = false;  // Resetting the change flag
		notifications->Text = "New file created and Auto-save disabled";
	}

	
	public: void openFile(OpenFileDialog^ openFileDialog1, Label^ nameFile, Label^ notifications,Label^ encoding, RichTextBox^ richTextBox1)
	{
		try {
			Stream^ stream;
			openFileDialog1->Filter = "Text Files (*.txt)|*.txt|Formatted Text (*.rtf)|*.rtf|All files (*.*)|*.*";

			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				nameFile->Text = openFileDialog1->FileName;

				if (openFileDialog1->FileName->EndsWith("rtf"))
				{
					nameFile->Text = openFileDialog1->FileName;

					richTextBox1->LoadFile(openFileDialog1->FileName, RichTextBoxStreamType::RichText);
				}

				else
				{
					//changes the label name to the file name
					nameFile->Text = openFileDialog1->FileName;

					if ((stream = openFileDialog1->OpenFile()) != nullptr)
					{
						stream->Close();
						richTextBox1->Text = System::IO::File::ReadAllText(openFileDialog1->FileName);
					}
				}

				helperRich HRich;
				encoding->Text = HRich.DetectEncodingSimple(nameFile->Text);
				notifications->Text = "The file has been opened successfully";
			}
		}
		catch (Exception^ ex) {
			notifications->Text = "Error when opening a file";
			MessageBox::Show("Error when opening a file: " + ex->Message,
				"Error",
				MessageBoxButtons::OK,
				MessageBoxIcon::Error);
		}
	}

	public: void fileSaveAs(RichTextBox^ richTextBox1, Label^ nameFile, Label^ notifications, Label^ encoding)
	{
		helperRich HRich;

		SaveFileDialog^ saveFileDialog1 = gcnew SaveFileDialog;
		saveFileDialog1->Filter = "Text Files (*.txt)|*.txt|Formatted Text (*.rtf)|*.rtf|All files (*.*)|*.*";
		saveFileDialog1->RestoreDirectory = true;
	
		if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			try
			{
				if (saveFileDialog1->FileName->EndsWith("rtf"))
				{
					// Save as RTF
					richTextBox1->SaveFile(saveFileDialog1->FileName, RichTextBoxStreamType::RichText);
				}
				else
				{
					// Save as plain text
					richTextBox1->SaveFile(saveFileDialog1->FileName, RichTextBoxStreamType::PlainText);
				}
				nameFile->Text = saveFileDialog1->FileName;
				notifications->Text = "The file was saved successfully";
				encoding->Text = HRich.DetectEncodingSimple(nameFile->Text);
			}
			catch (Exception^ ex)
			{
				notifications->Text = "Error when saving: " + ex->Message;
			}
		}
	}

	public: void fileSave(RichTextBox^ richTextBox1, Label^ nameFile, Label^ notifications, Label^ encoding)
	{
		if (nameFile->Text != "No File")
		{
			try
			{
				if (nameFile->Text->EndsWith(".rtf", StringComparison::OrdinalIgnoreCase))
				{
					// save as rtf
					richTextBox1->SaveFile(nameFile->Text, RichTextBoxStreamType::RichText);
				}
				else
				{
					// save as txt
					File::WriteAllText(nameFile->Text, richTextBox1->Text);
				}
				notifications->Text = "File saved successfully";
				richTextBox1->Modified = false;
			}
			catch (...)
			{
				notifications->Text = "The file wasn't saved";
			}
		}
		else
		{
			fileSaveAs(richTextBox1, nameFile, notifications, encoding); // If the file is new, call "Save as"
		}
	}

	
};

class WorkWithForm
{
	public: void blackTheme(Control^ targerControl, RichTextBox^ richTextBox1,
		ToolStrip^ toolStrip1, MenuStrip^ menuStrip1, Label^ notifications)
	{
		targerControl->BackColor = System::Drawing::Color::FromArgb(60, 60, 60);
		targerControl->ForeColor = System::Drawing::Color::White;
	
		richTextBox1->BackColor = System::Drawing::Color::FromArgb(60, 60, 60);
		richTextBox1->ForeColor = System::Drawing::Color::White;
	
		toolStrip1->BackColor = System::Drawing::Color::FromArgb(60, 60, 60);
	
		menuStrip1->BackColor = System::Drawing::Color::FromArgb(60, 60, 60);
		menuStrip1->ForeColor = System::Drawing::Color::White;
	
		notifications->Text = "The theme has been changed to dark";
	}

	public: void whiteTheme(Control^ targerControl, RichTextBox^ richTextBox1,
		ToolStrip^ toolStrip1, MenuStrip^ menuStrip1, Label^ notifications)
	{
		targerControl->BackColor = System::Drawing::Color::White;
		targerControl->ForeColor = System::Drawing::Color::Black;
	
		richTextBox1->BackColor = System::Drawing::Color::White;
		richTextBox1->ForeColor = System::Drawing::Color::Black;
	
		toolStrip1->BackColor = System::Drawing::Color::White;
	
		menuStrip1->BackColor = System::Drawing::Color::White;
		menuStrip1->ForeColor = System::Drawing::Color::Black;
	
		notifications->Text = "The theme has been changed to a light one";
	}
};

class altPressedSelect
{
	private: 
		Point mouseStartPos;
		bool isAltPressed = false;

	public:
		void SelectVerticalBlock(int startX, int endX, RichTextBox^ richTextBox1) {
			richTextBox1->SelectAll();
			richTextBox1->SelectionBackColor = richTextBox1->BackColor; // Resetting the selection

			for (int i = 0; i < richTextBox1->Lines->Length; i++) 
			{
				String^ line = richTextBox1->Lines[i];
				int lineStart = richTextBox1->GetFirstCharIndexFromLine(i);

				// Defining the selection boundaries for the current row
				int startPos = GetCharIndexFromX(line, startX);
				int endPos = GetCharIndexFromX(line, endX);

				if (startPos >= 0 && endPos >= 0) {
					int selectStart = lineStart + min(startPos, endPos);
					int selectLength = abs(endPos - startPos);

					richTextBox1->Select(selectStart, selectLength);
					richTextBox1->SelectionBackColor = Color::LightBlue;
				}
			}
		}

		int GetCharIndexFromX(String^ line, int xPos) {
			return min(xPos / 8, line->Length); // 8 is the approximate width of the character.
		}

	//Processing keystrokes
	public: System::Void RichTextBox_KeyDown(System::Object^ sender, KeyEventArgs^ e) {
		isAltPressed = e->Alt;
	}

	public: System::Void RichTextBox_KeyUp(System::Object^ sender, KeyEventArgs^ e) {
		isAltPressed = false;
	}

	//Processing mouse movement
	public: System::Void RichTextBox_MouseDown(System::Object^ sender, MouseEventArgs^ e) {
		if (isAltPressed && e->Button == MouseButtons::Left) {
			mouseStartPos = e->Location;
		}
	}

	public: System::Void RichTextBox_MouseMove(System::Object^ sender, MouseEventArgs^ e, RichTextBox^ richTextBox1) {
		if (isAltPressed && e->Button == MouseButtons::Left) {
			// We get the starting and ending positions
			int startX = mouseStartPos.X;
			int endX = e->X;
	
			// We select the vertical block
			SelectVerticalBlock(startX, endX, richTextBox1);
		}
	}
};
