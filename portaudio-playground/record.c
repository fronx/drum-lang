#include <stdio.h>
#include <portaudio.h>
#include "pa_definitions.h"
#include "pa_utils.h"

typedef struct
{
    int      frameIndex;
    int      maxFrameIndex;
    SAMPLE  *recordedSamples;
} paTestData;

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
    // float [] *data = (float [] *)userData;
    unsigned int i;
    for (i=0; i<framesPerBuffer; i++)
    {
    }
    return 0;
}

int main () {
    PaStream *stream;
    float userData [100000];
    PA_BEGIN;
        stream = portaudio_record(&record, &userData);
        PA_START_STREAM(stream);
            Pa_Sleep( PA_SECONDS(1) );
        PA_STOP_STREAM(stream);
        PA_HANDLE_ERR( Pa_CloseStream(stream) );
    PA_END;
    return 0;
}
