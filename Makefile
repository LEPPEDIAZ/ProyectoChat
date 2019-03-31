CC = g++

CFLAGS = -std=c++11 -fpermissive

TARGET = dev_message_tester

all:$(TARGET)

$(TARGET):
	g++ $(TARGET).cpp -std=c++11 -fpermissive  -w
