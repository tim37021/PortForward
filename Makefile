EXEC = pf
.PHONY: all

all: $(EXEC)
CFLAGS = -std=c99 -pthread
LFLAGS = -pthread
OBJS = \
	main.o \
	thread.o \
	network.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

install:
	cp pf /usr/bin/

clean:
	rm $(OBJS) $(EXEC)
