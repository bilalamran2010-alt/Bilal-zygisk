#ifndef DEFINES_H
#define DEFINES_H

struct Config {
    struct { bool enabled, all, rage, legit, silent; } aim;
    struct { bool enabled, box, line, Health, enabledRgb; } esp;
};
typedef UnityEngine_Touch_Fields (*GetTouch_t)(int index);

#endif
