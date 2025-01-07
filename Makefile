
CC = gcc
CFLAGS = -Wall -Wextra -I./external/raylib/src -I./external/raygui/src
LDFLAGS = -L./external/raylib/src -L./external/raygui/src -lraylib -lraygui -lm -lpthread -lwinmm -lgdi32 -luser32 -lcomdlg32 -lversion

SRC = main.c
OBJ = $(SRC:.c=.o)
OUT = main

all: $(OUT)


$(OUT): $(OBJ)
	$(CC) -o $(OUT) $(OBJ) $(LDFLAGS)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(OUT)

rebuild: clean all
