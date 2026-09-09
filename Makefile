CC = clang
CFLAGS = -Wall -Wextra
TARGET = kmode

all: $(TARGET)

$(TARGET): src/kmode.c
	$(CC) $(CFLAGS) src/lordit.c -o $(TARGET) -lncurses

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

