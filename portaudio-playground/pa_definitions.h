#ifndef __PA_DEFINITIONS_H_INCLUDED__
#define __PA_DEFINITIONS_H_INCLUDED__

#define SAMPLE_RATE       (44100)
#define SAMPLE_FORMAT     paFloat32
#define SAMPLE_SILENCE    0.0f
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

#define PA_SECONDS(s)           ((s)*1000)

enum enumStreamType
    { RECORD
    , PLAYBACK
    };

typedef enum enumStreamType StreamType;
typedef float SAMPLE;

#endif
