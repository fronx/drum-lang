#include <stdio.h>
#include <stdlib.h>
#include "recording.h"
#include "macros.h"

Recording
recording_new (long n_frames)
{
    Recording recording =
        { .frame_index = 0
        , .max_frame_index = (n_frames - 1)
        , .samples = (SAMPLE *) malloc(n_frames)
        };
    return recording;
}

int
recording_frames_left (const Recording *recording)
{
    return recording->max_frame_index - recording->frame_index;
}

void
recording_forward (Recording *recording, unsigned int n_fwd)
{
    recording->frame_index += n_fwd;
}

SAMPLE *
recording_get_writer (const Recording *recording)
{
    return &recording->samples[recording->frame_index];
}

void
recording_print (const Recording *recording, int limit)
{
    printf("frame_index: %d\n", recording->frame_index);
    printf("max_frame_index: %d\n", recording->max_frame_index);
    int i;
    int n_samples = min(recording->frame_index, limit);
    SAMPLE *reader = recording->samples;
    for (i=0; i < n_samples; i++)            {
        printf("%d: %f\n", i, *reader++);    }
}
