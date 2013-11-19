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

#define PA_HANDLE_ERR(FN_CALL) portaudio_handle_error((FN_CALL));
#define PA_BEGIN               PA_HANDLE_ERR( Pa_Initialize() );
#define PA_END                 PA_HANDLE_ERR( Pa_Terminate() );

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
        data->left_phase += 0.0001f;
        // when signal reaches top, drop back down.
        if (data->left_phase >= maxVolume) data->left_phase -= maxVolume;
        // higher pitch so we can distinguish left and right.
        data->right_phase += 0.0003f;
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
    PA_HANDLE_ERR(
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

void portaudio_wait_seconds(int s)
{
    Pa_Sleep(s*1000);
}

int main () {
    PaStream * stream;
    PA_BEGIN
        stream = portaudio_playback();
        PA_HANDLE_ERR( Pa_StartStream(stream) );
            portaudio_wait_seconds(1);
        PA_HANDLE_ERR( Pa_StopStream(stream) );
    PA_END
    return 0;
}
