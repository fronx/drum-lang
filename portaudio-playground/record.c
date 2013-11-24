#include <stdio.h>
#include <portaudio.h>
#include "pa_definitions.h"
#include "pa_utils.h"

#define min(a,b) ((a) < (b) ? (a) : (b))

typedef struct
{
    int      frame_index;
    int      max_frame_index;
    SAMPLE  *samples;
} Recording;

int
recording_frames_left (Recording *recording)
{
    return recording->max_frame_index - recording->frame_index;
}

void recording_forward (Recording *recording, unsigned int n_fwd)
{
    recording->frame_index += n_fwd;
}

SAMPLE *
recording_get_writer (Recording *recording)
{
    return &recording->samples[recording->frame_index];
}

static int
recordBuffer( const SAMPLE  *reader
            , unsigned long  frames_per_buffer
            , Recording     *recording
            )
{
    SAMPLE *writer = recording_get_writer(recording);
    unsigned long n_frames = min(recording_frames_left(recording), frames_per_buffer);
    int return_val = n_frames > 0 ? paContinue : paComplete;
    long i;

    if (reader == NULL)                   {
        for (i=0; i < n_frames; i++)    {
            *writer++ = SAMPLE_SILENCE; } }
    else                                  {
        for (i=0; i < n_frames; i++)    {
            *writer++ = *reader++;      } }

    return return_val;
}


static int
record
(
    const void                     *inputBuffer,
    void                           *outputBuffer,
    unsigned long                   framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags           statusFlags,
    void                           *userData
)
{
    return recordBuffer
        ( (const SAMPLE*)inputBuffer
        , framesPerBuffer
        , (Recording*)userData
        );
}

int main () {
    PaStream *stream;
    float userData [100000];
    PA_BEGIN;
        stream = portaudio_record(&record, &userData);
        PA_START_STREAM(stream);
            Pa_Sleep( PA_SECONDS(2) );
        PA_STOP_STREAM(stream);
        PA_HANDLE_ERR( Pa_CloseStream(stream) );
    PA_END;

    return 0;
}
