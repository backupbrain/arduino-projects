C_INCLUDE_PATH=/usr/local/include:$C_INCULDE_PATH
CPLUS_INCLUDE_PATH=/usr/local/include:$CPLUS_INCLUDE_PATH
CC=g++ # compiler
CFLAGS=-w -llua -lm -I.  # compiler flags, including library locations
OBJ=main.cpp APA102FrameManager.cpp APA102Frame.cpp RGBColor.cpp # These are the files being compiled

all: test

test: $(OBJ)  # compile these files
	$(CC) -o main main.cpp  APA102FrameManager.cpp APA102Frame.cpp RGBColor.cpp $(CFLAGS)  # using this command
