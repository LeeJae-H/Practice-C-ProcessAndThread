TARGET = proj
CC = gcc
CFLAGS = -O3
LDFLAGS = -lrt
OBJECTS = main.o proj1.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
main.o: main.c
	$(CC) $(CFLAGS) -c $^
proj1.o: proj1.c
	$(CC) $(CFLAGS) -c $^
clean:
	rm -f $(OBJECTS) $(TARGET)

