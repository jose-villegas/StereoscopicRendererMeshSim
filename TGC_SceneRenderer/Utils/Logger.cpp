#include "Logger.h"
using namespace Utils;

void Logger::ClearLog()
{
    Log->Clear();
}

void Logger::Write(System::String ^text)
{
    System::Windows::Forms::Label ^newLabel = gcnew System::Windows::Forms::Label();
    newLabel->Text = text;
    Log->Add(newLabel);
}

void Logger::Write(System::String ^text, bool writeToStatusLabel)
{
    System::Windows::Forms::Label ^newLabel = gcnew System::Windows::Forms::Label();
    newLabel->Text = text;
    Log->Add(newLabel);

    if (writeToStatusLabel && windowStatusLabel) {
        windowStatusLabel->Text = text;
        windowStatusLabel->ForeColor = System::Drawing::Color::Black;
        windowStatusLabel->GetCurrentParent()->Refresh();
    }
}

void Logger::Write(System::String ^text, bool writeToStatusLabel, System::Drawing::Color labelForeColor)
{
    System::Windows::Forms::Label ^newLabel = gcnew System::Windows::Forms::Label();
    newLabel->Text = text;
    newLabel->ForeColor = labelForeColor;
    Log->Add(newLabel);

    if (writeToStatusLabel && windowStatusLabel) {
        windowStatusLabel->Text = text;
        windowStatusLabel->ForeColor = labelForeColor;
        windowStatusLabel->GetCurrentParent()->Refresh();
    }
}

void Logger::Write(System::String ^text, System::Drawing::Color labelForeColor)
{
    System::Windows::Forms::Label ^newLabel = gcnew System::Windows::Forms::Label();
    newLabel->Text = text;
    newLabel->ForeColor = labelForeColor;
    Log->Add(newLabel);
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

System::String ^Logger::GetLastLog()
{
    return ((System::Windows::Forms::Label ^)Log[Log->Count - 1])->Text;
}

void Logger::SetStatusLabel(System::Windows::Forms::ToolStripStatusLabel ^statusLabel)
{
    windowStatusLabel = statusLabel;
}
