# Compiler and Flags
CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99 -O2
LIBS     = -liup -lm

# Target and Source Files
TARGET   = app
SRCS     = main.c gui.c image.c
OBJS     = $(SRCS:.c=.o)
HEADERS  = gui.h image.h

# Default Rule
all: $(TARGET)

# Linking Executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

# Compiling Object Files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup Rule
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean