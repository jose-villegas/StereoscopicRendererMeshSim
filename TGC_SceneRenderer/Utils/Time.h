#pragma once
namespace Utils {

    class Time {
        private:
            double _deltaTime;
            double _totalTime;
            static Time *timeInstance;
        public:
            double deltaTime() { return this->_deltaTime; };
            double totalTime() { return this->_totalTime; };
            void deltaTime(const double &value) { this->_deltaTime = value; };
            void totalTime(const double &value) { this->_totalTime = value; };
            static Time *Instance();
        protected:
            Time(void);
    };
}

