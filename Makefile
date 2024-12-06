CC = g++
CFLAGS = -Wall -std=c++17

SRC = main.cpp process_monitor.cpp process_control.cpp utils.cpp
OBJ = $(SRC:.cpp=.o)

all: monitor

monitor: $(OBJ)
	$(CC) $(CFLAGS) -o monitor $(OBJ)

clean:
	rm -f $(OBJ) monitor
