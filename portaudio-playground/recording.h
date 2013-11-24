#ifndef __RECORDING_H_INCLUDED__
#define __RECORDING_H_INCLUDED__

#include "pa_definitions.h"

typedef struct
{
    int      frame_index;
    int      max_frame_index;
    SAMPLE  *samples;
} Recording;

Recording
recording_new (long n_frames);

int
recording_frames_left (Recording *recording);

void
recording_forward (Recording *recording, unsigned int n_fwd);

SAMPLE *
recording_get_writer (Recording *recording);

void
recording_print (Recording *recording, int limit);

#endif
