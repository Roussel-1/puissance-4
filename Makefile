CC = gcc
CFLAGS = -Wall -Wextra -O2 $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)
TARGET = puissance4
SRC = main2.c

# --- Règle par défaut (ce que "make" fera) ---
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# --- Nettoyer les fichiers compilés ---
clean:
	rm -f $(TARGET)
