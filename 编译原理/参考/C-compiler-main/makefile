.PHONY: all clean

CC := g++
CFLAGS := -g -Wall -fpermissive
INC_PATH := ./include/
OBJ_PATH := ./objs/
SOURCE := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp,$(OBJ_PATH)%.o,$(SOURCE))
HEADER := $(wildcard $(INC_PATH)*.h)
TAR := parser

all: $(TAR)

$(TAR): $(OBJ)
	$(CC) -I$(INC_PATH) -o $@ $^ -lpthread

$(OBJ_PATH)%.o: %.cpp $(HEADER)
	$(CC) $(CFLAGS) -I$(INC_PATH) -c $< -o $@

clean:
	rm -f $(TAR) $(OBJ)