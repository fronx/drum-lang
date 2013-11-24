#include <stdio.h>
#include <portaudio.h>
#include "pa_definitions.h"
#include "pa_utils.h"

typedef struct
{
   float left_phase;
   float right_phase;
} paTestData;

static int
sawtoothCallback
    ( const void                      *inputBuffer
    , void                            *outputBuffer
    , unsigned long                    framesPerBuffer
    , const PaStreamCallbackTimeInfo  *timeInfo
    , PaStreamCallbackFlags            statusFlags
    , void                            *userData
    )
{
    // cast data passed through stream to our structure.
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    float maxVolume  = 0.05f;
    float maxVolume2 = 2 * maxVolume;

    for (i=0; i<framesPerBuffer; i++)
    {
        *out++ = data->left_phase;
        *out++ = data->right_phase;
        // generate simple sawtooth phaser that ranges between -1.0 and 1.0.
        data->left_phase += 0.0001f;
        // when signal reaches top, drop back down.
        if (data->left_phase >= maxVolume) data->left_phase -= maxVolume2;
        // higher pitch so we can distinguish left and right.
        data->right_phase += 0.0003f;
        if (data->right_phase >= maxVolume) data->right_phase -= maxVolume2;
    }
    return 0;
}

int main (void)
{
    PaStream *stream;
    paTestData userData;
    PaError err;
    err = Pa_Initialize();
    if (err != paNoError) goto error;
    stream = portaudio_playback(&sawtoothCallback, &userData);
    if (stream != NULL)                      {
        err = Pa_StartStream(stream);
        if (err != paNoError) goto error;
            Pa_Sleep( PA_SECONDS(1) );
        err = Pa_StopStream(stream);
        if (err != paNoError) goto error;
        err = Pa_CloseStream(stream);
        if (err != paNoError) goto error;    };
    goto fin;
error:
    portaudio_print_error(err);
fin:
    Pa_Terminate();
    return 0;
}
