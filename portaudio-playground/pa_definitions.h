#ifndef __PA_DEFINITIONS_H_INCLUDED__
#define __PA_DEFINITIONS_H_INCLUDED__

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

#define PA_HANDLE_ERR(FN_CALL)  portaudio_handle_error((FN_CALL))
#define PA_BEGIN                PA_HANDLE_ERR( Pa_Initialize() )
#define PA_END                  PA_HANDLE_ERR( Pa_Terminate() )
#define PA_SECONDS(s)           ((s)*1000)
#define PA_START_STREAM(stream) PA_HANDLE_ERR( Pa_StartStream((stream)) )
#define PA_STOP_STREAM(stream)  PA_HANDLE_ERR( Pa_StopStream((stream)) )

enum enumStreamType { RECORD
                    , PLAYBACK
                    };

typedef enum enumStreamType StreamType;

#endif
