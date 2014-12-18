#pragma once
namespace utils {

    class Time {
        private:
#define SMOOTH_STEP 0.7
            double _deltaTime;
            double _totalTime;
            static Time *timeInstance;
        public:
            double deltaTime() { return this->_deltaTime; };
            double totalTime() { return this->_totalTime; };
            void deltaTime(const double &value) { this->_deltaTime = value * SMOOTH_STEP + this->_deltaTime * (1 - SMOOTH_STEP); };
            void totalTime(const double &value) { this->_totalTime = value; };
            static Time *Instance();
        protected:
            Time(void);
    };
}

