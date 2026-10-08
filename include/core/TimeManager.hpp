#ifndef TIME_MANAGER_HPP
#define TIME_MANAGER_HPP

#include "Common.hpp"

class TimeManager {
private:
    int lastTimeMs;
    float deltaTime;
    float totalTime;

public:
    TimeManager();
    void update();
    float getDeltaTime() const;
    float getTotalTime() const;
};

#endif // TIME_MANAGER_HPP