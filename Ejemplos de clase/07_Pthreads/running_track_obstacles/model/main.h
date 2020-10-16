#ifndef MAIN_H
#define MAIN_H
#include "running_track.h"

    extern running_track_t* running_track;
    pthread_mutex_t mutex_running_track;

    int start_race();

#endif