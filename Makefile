# change application name here (executable output name)
TARGET=ekc

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread
#$(DEBUG) $(OPT)
CCFLAGS=$(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`
CURlCON=`curl-config --cc`
CURLIB=`curl-config --cflags --libs`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB)

OBJS=    main.o

all: $(OBJS)
main.o: src/solpos.c src/main.c
	$(CURlCON) -o ekc src/solpos.c src/main.c $(CURLIB) $(GTKLIB) $(PTHREAD)
test.o: src/solpos.c src/test.c
	$(CC) src/solpos.c src/test.c -o spa $(PTHREAD)
clean:
	rm -f *.o $(TARGET)


#`curl-config --cc` -o example test.c `curl-config --cflags --libs` `pkg-config --cflags --libs gtk+-3.0` -pthread
# `curl-config --cc` -o example main.c `curl-config --cflags --libs` `pkg-config --cflags --libs gtk+-3.0` -pthread