CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LIBS = -lSDL2

# NOM DU FICHIER SOURCE (change seulement cette ligne si tu changes le nom)
SRC = main3.c

# NOM DE L'EXÉCUTABLE
OUT = puissance4

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
