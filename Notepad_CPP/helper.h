#pragma once

#include <iostream>
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
		notifications->Text = "New file created";
	}

	
	public: void openFile(OpenFileDialog^ openFileDialog1, Label^ nameFile, Label^ notifications , RichTextBox^ richTextBox1)
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
						notifications->Text = "the file was opened successfully";
					}
				}


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

	public: void fileSaveAs(RichTextBox^ richTextBox1, Label^ nameFile, Label^ notifications)
	{
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
			}
			catch (Exception^ ex)
			{
				notifications->Text = "Error when saving: " + ex->Message;
			}
		}
	}
};
