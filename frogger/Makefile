#compiler
CC = g++

#compiler flags
CFLAGS = -g -Wall

#build target executable
TARGET = game.app

LIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: $(TARGET)

$(TARGET): main.cpp
	$(CC) -c main.cpp
	$(CC) $(CFLAGS) -o $(TARGET) main.o $(LIBS)