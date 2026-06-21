typedef UnityEngine_Touch_Fields (*GetTouch_t)(int index);
#ifndef MAIN_H
#define MAIN_H

extern bool HasOriginalPos;
extern bool fourFingerPressed;
extern bool HideFullMenu;
extern bool NewBox;

struct Config {
    struct {
        bool enabled;
        bool all;
        bool rage;
        bool legit;
        bool silent;
    } aim;

    struct {
        bool enabled;
        bool box;
        bool line;
        bool Health;
        bool enabledRgb;
    } esp;
};

inline Config cfg; 

#endif
