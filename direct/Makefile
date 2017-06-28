ROOT = $(HOME)/Desktop/cacheSimProject/direct

CPPFLAGS := \
	-I/usr/include

CFLAGS := \
	-std=c++14 \
	-stdlib=libc++
	
TARGET = $(ROOT)/directCacheSim


all: $(TARGET)

SOURCE = $(ROOT)/src/directCacheSim.cpp
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
