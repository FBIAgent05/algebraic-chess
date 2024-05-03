CC = clang
CFLAGS = -Wall
TARGET = chess
SRCS = ./src/main.c ./src/chess_lib.c ./src/gen_tools.c ./src/UI.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
