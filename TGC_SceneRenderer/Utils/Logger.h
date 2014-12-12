#pragma once

namespace Utils {
    public ref class Logger {
        private:
            static System::ComponentModel::BindingList<System::Windows::Forms::Label ^> ^Log = gcnew
                    System::ComponentModel::BindingList<System::Windows::Forms::Label ^>();
            static System::Windows::Forms::ToolStripStatusLabel ^windowStatusLabel;
        public:
            static void Write(System::String ^text);
            static void Write(System::String ^text, System::Drawing::Color labelForeColor);
            static void Write(System::String ^text, bool writeToStatusLabel);
            static void Write(System::String ^text, bool writeToStatusLabel, System::Drawing::Color labelForeColor);
            static void ClearLog();
            static void SetStatusLabel(System::Windows::Forms::ToolStripStatusLabel ^statusLabel);
            static System::String ^GetLastLog();
            static System::ComponentModel::BindingList<System::Windows::Forms::Label ^> ^GetLog() { return Log; };
    };
}

