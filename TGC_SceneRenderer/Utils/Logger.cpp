#include "Logger.h"
using namespace Utils;

void Logger::ClearLog()
{
    _Log->Clear();
}

void Logger::Write(System::String ^text)
{
    System::Windows::Forms::Label ^newLabel = gcnew System::Windows::Forms::Label();
    newLabel->Text = text;
    _Log->Add(newLabel);

    if (_writeToLogFile) {
        System::IO::StreamWriter ^tempWriter = gcnew System::IO::StreamWriter(logFileLocation, true);
        tempWriter->WriteLine(text);
        tempWriter->Close();
    }
}

void Logger::Write(System::String ^text, bool writeToStatusLabel)
{
    System::Windows::Forms::Label ^newLabel = gcnew System::Windows::Forms::Label();
    newLabel->Text = text;
    _Log->Add(newLabel);

    if (writeToStatusLabel && _windowStatusLabel) {
        _windowStatusLabel->Text = text;
        _windowStatusLabel->ForeColor = System::Drawing::Color::Black;
        _windowStatusLabel->GetCurrentParent()->Refresh();
    }

    if (_writeToLogFile) {
        System::IO::StreamWriter ^tempWriter = gcnew System::IO::StreamWriter(logFileLocation, true);
        tempWriter->WriteLine(text);
        tempWriter->Close();
    }
}

void Logger::Write(System::String ^text, bool writeToStatusLabel, System::Drawing::Color labelForeColor)
{
    System::Windows::Forms::Label ^newLabel = gcnew System::Windows::Forms::Label();
    newLabel->Text = text;
    newLabel->ForeColor = labelForeColor;
    _Log->Add(newLabel);

    if (writeToStatusLabel && _windowStatusLabel) {
        _windowStatusLabel->Text = text;
        _windowStatusLabel->ForeColor = labelForeColor;
        _windowStatusLabel->GetCurrentParent()->Refresh();
    }

    if (_writeToLogFile) {
        System::IO::StreamWriter ^tempWriter = gcnew System::IO::StreamWriter(logFileLocation, true);
        tempWriter->WriteLine(text);
        tempWriter->Close();
    }
}

void Logger::Write(System::String ^text, System::Drawing::Color labelForeColor)
{
    System::Windows::Forms::Label ^newLabel = gcnew System::Windows::Forms::Label();
    newLabel->Text = text;
    newLabel->ForeColor = labelForeColor;
    _Log->Add(newLabel);

    if (_writeToLogFile) {
        System::IO::StreamWriter ^tempWriter = gcnew System::IO::StreamWriter(logFileLocation, true);
        tempWriter->WriteLine(text);
        tempWriter->Close();
    }
}

void Utils::Logger::Write(std::string text)
{
    Logger::Write(gcnew System::String(text.c_str()));
}

void Utils::Logger::Write(std::string text, System::Drawing::Color labelForeColor)
{
    Logger::Write(gcnew System::String(text.c_str()), labelForeColor);
}

void Utils::Logger::Write(std::string text, bool writeToStatusLabel)
{
    Logger::Write(gcnew System::String(text.c_str()), writeToStatusLabel);
}

void Utils::Logger::Write(std::string text, bool writeToStatusLabel, System::Drawing::Color labelForeColor)
{
    Logger::Write(gcnew System::String(text.c_str()), writeToStatusLabel, labelForeColor);
}

void Utils::Logger::UseLogFile(System::Boolean writeToLogFile)
{
    _writeToLogFile = writeToLogFile;
}

System::String ^Logger::GetLastLog()
{
    return ((System::Windows::Forms::Label ^)_Log[_Log->Count - 1])->Text;
}

void Logger::SetStatusLabel(System::Windows::Forms::ToolStripStatusLabel ^statusLabel)
{
    _windowStatusLabel = statusLabel;
}
