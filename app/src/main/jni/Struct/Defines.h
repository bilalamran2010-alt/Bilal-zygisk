#ifndef DEFINES_H
#define DEFINES_H

struct Config {
    struct { bool enabled, all, rage, legit, silent; } aim;
    struct { bool enabled, box, line, Health, enabledRgb; } esp;
};

// Use void* to avoid type dependency issues for now
typedef void* (*GetTouch_t)(int index);

#endif
