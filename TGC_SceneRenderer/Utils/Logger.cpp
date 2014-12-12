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
    }
}

void Logger::Write(System::String ^text, System::Drawing::Color labelForeColor)
{
    System::Windows::Forms::Label ^newLabel = gcnew System::Windows::Forms::Label();
    newLabel->Text = text;
    newLabel->ForeColor = labelForeColor;
    Log->Add(newLabel);
}

System::String ^Logger::GetLastLog()
{
    return ((System::Windows::Forms::Label ^)Log[Log->Count - 1])->Text;
}

void Logger::SetStatusLabel(System::Windows::Forms::ToolStripStatusLabel ^statusLabel)
{
    windowStatusLabel = statusLabel;
}
