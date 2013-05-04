CC=gcc
CFLAGS=-g -O3 -march=corei7-avx -Wall -std=gnu99  -fgnu89-inline
TARGET=Placer 

INC = -I ./inc

OBJS = main.o \
      rand.o \
      grid.o

%.o: %.c
	$(CC) $(INC) $(CFLAGS) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(INC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o placer

