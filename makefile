CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = main.c steg.c
OBJ = $(SRC:.c=.o)

TARGET = stegprog

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c include/steg.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

# Windows compatibility (MinGW)
cleanwin:
	del *.o $(TARGET).exe 2>nul
