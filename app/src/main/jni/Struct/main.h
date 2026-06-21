#ifndef MAIN_H
#define MAIN_H

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
    } esp;
};

inline Config cfg; 

#endif
