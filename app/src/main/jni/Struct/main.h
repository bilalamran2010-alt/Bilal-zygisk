#ifndef MAIN_H
#define MAIN_H

#include "Gui.hpp"

typedef UnityEngine_Touch_Fields (*GetTouch_t)(int index);

extern Config cfg;
extern GetTouch_t old_GetTouch;
extern bool HasOriginalPos;
extern bool fourFingerPressed;
extern bool HideFullMenu;
extern bool NewBox;

#endif
