#include "Logger.h"

void Utils::Logger::ClearLog()
{
    Log->Clear();
}

void Utils::Logger::Write(System::String ^text)
{
    Log->Add(text);
}

void Utils::Logger::Write(System::String ^text, bool writeToStatusLabel)
{
    Log->Add(text);

    if (writeToStatusLabel && windowStatusLabel) {
        windowStatusLabel->Text = text;
        windowStatusLabel->ForeColor = System::Drawing::Color::Black;
    }
}

void Utils::Logger::Write(System::String ^text, bool writeToStatusLabel, System::Drawing::Color labelForeColor)
{
    Log->Add(text);

    if (writeToStatusLabel && windowStatusLabel) {
        windowStatusLabel->Text = text;
        windowStatusLabel->ForeColor = labelForeColor;
    }
}

System::String ^Utils::Logger::GetLastLog()
{
    return (System::String ^)Log[Log->Count - 1];
}

void Utils::Logger::SetStatusLabel(System::Windows::Forms::ToolStripStatusLabel ^statusLabel)
{
    windowStatusLabel = statusLabel;
}
