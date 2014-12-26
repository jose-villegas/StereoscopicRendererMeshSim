#pragma once
namespace utils {

    class Time {
        public:
            double deltaTime;
            double totalTime;
            static Time *Instance();
        private:
            static Time *timeInstance;
            Time(void);
    };
}

