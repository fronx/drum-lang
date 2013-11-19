CC         = gcc
CFLAGS     = -c -Wall -std=c99
LDFLAGS    =

CFLAGS    += `pkg-config --cflags portaudio-2.0`
LDFLAGS   += `pkg-config --libs   portaudio-2.0`

SOURCES    = sawtooth.c
OBJECTS    = $(SOURCES:.c=.o)

EXECUTABLE = sawtooth

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
