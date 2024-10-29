#ifndef UTILS_H
#define UTILS_H


#include <stdint.h>
#include <stdbool.h>

#define UNDEFINED -1

typedef struct {
    uint32_t fpsStartTime, frameCount, timerStartTime, lastFrameTime;
    double fps, dt;
} Clock;

void Clock_Init(Clock* clock);
void Clock_Update(Clock* clock, int targetFps);
bool Clock_TimePassed(Clock* clock, int miliSeconds);

int Utils_Abs(int x);

char* getAbsolutePath(const char* relativePath);
int randint(int min, int max);


//debug 
#define LogNullError(objectName, additionalMsg) LogNullErrorEx(__func__, objectName, additionalMsg)
void LogNullErrorEx(const char* baseFunctionName, const char* objectName, const char* additionalMsg);


#endif // UTILS_H