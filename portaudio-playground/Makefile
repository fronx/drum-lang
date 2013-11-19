CC         = gcc
CFLAGS     = -c -Wall -std=c99
LDFLAGS    =

CFLAGS    += `pkg-config --cflags portaudio-2.0`
LDFLAGS   += `pkg-config --libs   portaudio-2.0`

SOURCES    = sawtooth.c record.c
OBJECTS    = $(SOURCES:.c=.o)

all: $(SOURCES) sawtooth record

sawtooth: sawtooth.o
	$(CC) sawtooth.o -o $@ $(LDFLAGS)

record: record.o
	$(CC) record.o -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) sawtooth record
