#ifndef __PA_UTILS_H_INCLUDED__
#define __PA_UTILS_H_INCLUDED__

#include <portaudio.h>
#include "pa_definitions.h"
#include "macros.h"

void portaudio_print_error (err)
{
    printf("PortAudio error: %s\n", Pa_GetErrorText(err));
}

PaStream *
__portaudio_open_stream
    ( StreamType  streamType
    , void       *streamCallback
    , void       *userData
    )
{
    PaStream *stream;
    PaError err;
    err = Pa_OpenDefaultStream
        ( &stream
        , streamType == RECORD ? INPUT_STEREO  : NO_INPUT
        , streamType == RECORD ? NO_OUTPUT     : OUTPUT_STEREO
        , SAMPLE_FORMAT
        , SAMPLE_RATE
        , FRAMES_PER_BUFFER
        , streamCallback
        , userData  // will be passed to callback
        );
    if (err != paNoError)            {
        portaudio_print_error(err);
        return NULL;                 }
    else                             {
        return stream;               }
}

PaStream *
portaudio_record (void *streamCallback, void *userData)
{
    return __portaudio_open_stream(RECORD, streamCallback, userData);
}

PaStream *
portaudio_playback (void *streamCallback, void *userData)
{
    return __portaudio_open_stream(PLAYBACK, streamCallback, userData);
}

#endif
