#ifndef __PA_UTILS_H_INCLUDED__
#define __PA_UTILS_H_INCLUDED__

#include <portaudio.h>
#include "pa_definitions.h"

void portaudio_error (err)
{
    printf("PortAudio error: %s\n", Pa_GetErrorText(err));
}

void portaudio_handle_error (PaError err)
{
    if (err != paNoError) portaudio_error(err);
}

PaStream *
__portaudio_open_stream
(
  StreamType streamType,
  void       *streamCallback,
  void       *userData
)
{
    PaStream *stream;
    PA_HANDLE_ERR(
        Pa_OpenDefaultStream(
            &stream,
            streamType == RECORD ? INPUT_STEREO  : NO_INPUT,
            streamType == RECORD ? NO_OUTPUT     : OUTPUT_STEREO,
            paFloat32,         // sampleFormat
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            streamCallback,
            userData           // will be passed to callback
        )
    );
    return stream;
}

// PaStream *
// portaudio_record ()
// {
//     return __portaudio_open_stream(RECORD);
// }

PaStream *
portaudio_playback (void *streamCallback, void *userData)
{
    return __portaudio_open_stream(PLAYBACK, streamCallback, userData);
}

#endif
