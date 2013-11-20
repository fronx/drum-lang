CC         = gcc
CFLAGS     = -c -Wall -std=c99
LDFLAGS    =

define DEPS
	portaudio-2.0 \
	aubio
endef

CFLAGS    += `pkg-config --cflags ${DEPS}`
LDFLAGS   += `pkg-config --libs   ${DEPS}`

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
