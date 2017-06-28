ROOT = $(HOME)/Desktop/cacheSimProject/fully_associative

CPPFLAGS := \
	-I/usr/include

CFLAGS := \
	-std=c++14 \
	-stdlib=libc++
	
TARGET = $(ROOT)/fullyAssCacheSim


all: $(TARGET)

SOURCE = $(ROOT)/src/fullyAssCacheSim.cpp
OBJECT = $(SOURCE:.cpp=.o)
BUILD = $(TARGET) $(OBJECT)

%.o : %.cpp
	g++ $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

all : $(TARGET)
	@echo ALL complete...

$(TARGET) : $(OBJECT)
	g++ -g -Wall -o $@ $^

clean : $(OBJECT)
	rm -f $(OBJECT) $(TARGET)
	@echo Clean completed...
