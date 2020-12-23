C_INCLUDE_PATH=/usr/local/include:$C_INCULDE_PATH
CPLUS_INCLUDE_PATH=/usr/local/include:$CPLUS_INCLUDE_PATH
CC=g++ # compiler
CFLAGS=-w -llua -lm -I.  # compiler flags, including library locations
OBJ=main.cpp MyClass.cpp Caller.cpp # These are the files being compiled

all: test

test: $(OBJ)  # compile these files
	$(CC) -o main main.cpp  MyClass.cpp Caller.cpp $(CFLAGS)  # using this command
