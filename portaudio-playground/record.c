#include <stdlib.h>
#include <stdio.h>
#include <portaudio.h>
#include "pa_definitions.h"
#include "pa_utils.h"
#include "recording.h"

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

    recording_forward(recording, n_frames);
    return return_val;
}


static int
callback_record
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

int main (void) {
    PaStream *stream;
    Recording recording = recording_new(100000);
    PA_BEGIN;
        stream = portaudio_record(&callback_record, &recording);
        PA_START_STREAM(stream);
            Pa_Sleep( PA_SECONDS(2) );
        PA_STOP_STREAM(stream);
        PA_HANDLE_ERR( Pa_CloseStream(stream) );
    PA_END;
    recording_print(&recording, 100);
    printf("done.");
    return 0;
}
