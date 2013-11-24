#include <stdlib.h>
#include <stdio.h>
#include <portaudio.h>
#include "pa_definitions.h"
#include "pa_utils.h"
#include "recording.h"

static int
recordBuffer
    ( const SAMPLE  *reader
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
    ( const void                     *inputBuffer
    , void                           *outputBuffer
    , unsigned long                   framesPerBuffer
    , const PaStreamCallbackTimeInfo *timeInfo
    , PaStreamCallbackFlags           statusFlags
    , void                           *userData
    )
{
    return recordBuffer
        ( (const SAMPLE*)inputBuffer
        , framesPerBuffer
        , (Recording*)userData
        );
}

int main (void)
{
    PaStream *stream;
    Recording recording = recording_new(100000);
    PaError err;
    err = Pa_Initialize();
    if (err != paNoError) goto error;
    stream = portaudio_record(&callback_record, &recording);
    if (stream != NULL)                      {
        err = Pa_StartStream(stream);
        if (err != paNoError) goto error;
            Pa_Sleep( PA_SECONDS(2) );
        err = Pa_StopStream(stream);
        if (err != paNoError) goto error;
        err = Pa_CloseStream(stream);
        if (err != paNoError) goto error;    };
    goto fin;
error:
    portaudio_print_error(err);
fin:
    Pa_Terminate();
    recording_print(&recording, 100);
    free(recording.samples);
    printf("done.");
    return 0;
}
