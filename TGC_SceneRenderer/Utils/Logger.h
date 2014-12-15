#pragma once
#include <string>

#define LOG_CONTEXT_WARNING System::Drawing::Color::Yellow
#define LOG_CONTEXT_DANGER System::Drawing::Color::Red
#define LOG_CONTEXT_SUCCESS System::Drawing::Color::Green
#define LOG_CONTEXT_INFO System::Drawing::Color::Blue
#define LOG_CONTEXT_ACTIVE System::Drawing::Color::Gray

namespace Utils {
    public ref class Logger {
        private:
            static System::String ^logFileLocation = System::IO::Directory::GetCurrentDirectory() + "\\utils_logger.log";
            static System::ComponentModel::BindingList<System::Windows::Forms::Label ^> ^_Log = gcnew
                    System::ComponentModel::BindingList<System::Windows::Forms::Label ^>();
            static System::Windows::Forms::ToolStripStatusLabel ^_windowStatusLabel;
            static System::Boolean _writeToLogFile = true;
        public:
            static void Write(System::String ^text);
            static void Write(System::String ^text, System::Drawing::Color labelForeColor);
            static void Write(System::String ^text, bool writeToStatusLabel);
            static void Write(System::String ^text, bool writeToStatusLabel, System::Drawing::Color labelForeColor);
            static void Write(std::string text);
            static void Write(std::string text, System::Drawing::Color labelForeColor);
            static void Write(std::string text, bool writeToStatusLabel);
            static void Write(std::string text, bool writeToStatusLabel, System::Drawing::Color labelForeColor);
            static void ClearLog();
            static void SetStatusLabel(System::Windows::Forms::ToolStripStatusLabel ^statusLabel);
            static void UseLogFile(System::Boolean writeToLogFile);
            static System::String ^GetLastLog();
            static System::ComponentModel::BindingList<System::Windows::Forms::Label ^> ^GetLog() { return _Log; };
    };
}

