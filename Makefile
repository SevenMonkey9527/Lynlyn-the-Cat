TARGET  := start
CC      := g++
CCFLAGS := -O
OBJECTS	:= main.o gameflow.o map.o

all: $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(CCFLAGS)

%.o: %.cc
	$(CC) -o $@ -c $< $(CCFLAGS)

.PHONY: clean
clean:
	rm $(OBJECTS)
	rm $(TARGET)
