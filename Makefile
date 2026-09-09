CC = clang
CFLAGS = -Wall -Wextra
TARGET = lordit

all: $(TARGET)

$(TARGET): src/lordit.c
	$(CC) $(CFLAGS) src/lordit.c -o $(TARGET) -lncurses

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

