#include <stdio.h>
#include <portaudio.h>

#define SAMPLE_RATE       (44100)
#define NO_INPUT          (0)
#define NO_OUTPUT         (0)
#define INPUT_STEREO      (2)
#define OUTPUT_STEREO     (2)
#define FRAMES_PER_BUFFER (256) /* frames per buffer, i.e. the number
                                   of sample frames that PortAudio will
                                   request from the callback. Many apps
                                   may want to use
                                   paFramesPerBufferUnspecified, which
                                   tells PortAudio to pick the best,
                                   possibly changing, buffer size.
                                */

enum enumStreamType { RECORD
                    , PLAYBACK
                    };

typedef enum enumStreamType StreamType;

typedef struct
{
   float left_phase;
   float right_phase;
}
paTestData;

static paTestData userData;

/* in the callback: http://portaudio.com/docs/v19-doxydocs/writing_a_callback.html
don't do anything like allocating or freeing memory, reading or writing files, printf(), or anything else that might take an unbounded amount of time or rely on the OS or require a context switch
*/

void portaudio_error (err)
{
    printf("PortAudio error: %s\n", Pa_GetErrorText(err));
}

void portaudio_handle_error (PaError err)
{
    if (err != paNoError) portaudio_error(err);
}

void portaudio_begin ()
{
    portaudio_handle_error(
        Pa_Initialize()
    );
}

void portaudio_end ()
{
    portaudio_handle_error(
        Pa_Terminate()
    );
}

/* This routine will be called by the PortAudio engine when audio is needed.
   It may called at interrupt level on some machines so don't do anything
   that could mess up the system like calling malloc() or free().
*/
static int
patestCallback
(
    const void                      *inputBuffer,
    void                            *outputBuffer,
    unsigned long                   framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags           statusFlags,
    void                            *userData
)
{
    // cast data passed through stream to our structure.
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    float maxVolume  = 0.1f;
    float maxVolume2 = 2 * maxVolume;

    for (i=0; i<framesPerBuffer; i++)
    {
        *out++ = data->left_phase;
        *out++ = data->right_phase;
        // generate simple sawtooth phaser that ranges between -1.0 and 1.0.
        data->left_phase += 0.001f;
        // when signal reaches top, drop back down.
        if (data->left_phase >= maxVolume) data->left_phase -= maxVolume;
        // higher pitch so we can distinguish left and right.
        data->right_phase += 0.003f;
        if (data->right_phase >= maxVolume) data->right_phase -= maxVolume;
    }
    return 0;
}

PaStream *
__portaudio_open_stream
(
  StreamType streamType,
  void *streamCallback
)
{
    PaStream *stream;
    portaudio_handle_error(
        Pa_OpenDefaultStream(
            &stream,
            streamType == RECORD ? INPUT_STEREO  : NO_INPUT,
            streamType == RECORD ? NO_OUTPUT     : OUTPUT_STEREO,
            paFloat32,         // sampleFormat
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            streamCallback,
            &userData          // will be passed to callback
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
portaudio_playback ()
{
    return __portaudio_open_stream(PLAYBACK, &patestCallback);
}

void portaudio_start_stream (PaStream * stream)
{
    portaudio_handle_error(
        Pa_StartStream(stream)
    );
}

void portaudio_stop_stream (PaStream * stream)
{
    portaudio_handle_error(
        Pa_StopStream(stream)
    );
}

void portaudio_wait_seconds(int s)
{
    Pa_Sleep(s*1000);
}

void list_audio_devices ()
{

}

int main () {
    PaStream * stream;
    portaudio_begin();
        stream = portaudio_playback();
        portaudio_start_stream(stream);
            portaudio_wait_seconds(2);
        portaudio_stop_stream(stream);
        list_audio_devices();
    portaudio_end();
    return 0;
}
